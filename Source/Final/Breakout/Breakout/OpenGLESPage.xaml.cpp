#include "pch.h"
#include "OpenGLESPage.xaml.h"

using namespace Breakout;
using namespace Platform;
using namespace Concurrency;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Input;
using namespace WinRT;

OpenGLESPage::OpenGLESPage() :
    OpenGLESPage(nullptr)
{

}

OpenGLESPage::OpenGLESPage(OpenGLES* openGLES) :
    mOpenGLES(openGLES),
    mRenderSurface(EGL_NO_SURFACE),
    mScreenResolutionScale(1.0f)
{
    InitializeComponent();

    Windows::UI::Core::CoreWindow^ window = Windows::UI::Xaml::Window::Current->CoreWindow;

    window->VisibilityChanged +=
        ref new Windows::Foundation::TypedEventHandler<Windows::UI::Core::CoreWindow^, Windows::UI::Core::VisibilityChangedEventArgs^>(this, &OpenGLESPage::OnVisibilityChanged);

    this->Loaded +=
        ref new Windows::UI::Xaml::RoutedEventHandler(this, &OpenGLESPage::OnPageLoaded);

    window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &OpenGLESPage::OnKeyPressed);
    window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &OpenGLESPage::OnKeyReleased);
    CreateInput();
}

OpenGLESPage::~OpenGLESPage()
{
    StopRenderLoop();
    DestroyRenderSurface();
}

void OpenGLESPage::OnPageLoaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    // The SwapChainPanel has been created and arranged in the page layout, so EGL can be initialized.
    CreateRenderSurface();
    StartRenderLoop();
}

void OpenGLESPage::OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
{
    if (args->Visible && mRenderSurface != EGL_NO_SURFACE)
    {
        StartRenderLoop();
    }
    else
    {
        StopRenderLoop();
    }
}

void OpenGLESPage::CreateRenderSurface()
{
    if (mOpenGLES && mRenderSurface == EGL_NO_SURFACE)
    {
        // The app can configure the the SwapChainPanel which may boost performance.
        // By default, this template uses the default configuration.
        mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, nullptr, nullptr);
        
        // You can configure the SwapChainPanel to render at a lower resolution and be scaled up to
        // the swapchain panel size. This scaling is often free on mobile hardware.
        //
        // One way to configure the SwapChainPanel is to specify precisely which resolution it should render at.
        // Size customRenderSurfaceSize = Size(800, 600);
        // mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, &customRenderSurfaceSize, nullptr);
        //
        // Another way is to tell the SwapChainPanel to render at a certain scale factor compared to its size.
        // e.g. if the SwapChainPanel is 1920x1280 then setting a factor of 0.5f will make the app render at 960x640
        // float customResolutionScale = 0.5f;
        // mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, nullptr, &customResolutionScale);
        // 
    }
}

void OpenGLESPage::DestroyRenderSurface()
{
    if (mOpenGLES)
    {
        mOpenGLES->DestroySurface(mRenderSurface);
    }
    mRenderSurface = EGL_NO_SURFACE;
}

void OpenGLESPage::RecoverFromLostDevice()
{
    // Stop the render loop, reset OpenGLES, recreate the render surface
    // and start the render loop again to recover from a lost device.

    StopRenderLoop();

    {
        critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

        DestroyRenderSurface();
        mOpenGLES->Reset();
        CreateRenderSurface();
    }

    StartRenderLoop();
}

void OpenGLESPage::StartRenderLoop()
{
    // If the render loop is already running then do not start another thread.
    if (mRenderLoopWorker != nullptr && mRenderLoopWorker->Status == Windows::Foundation::AsyncStatus::Started)
    {
        return;
    }

    // Create a task for rendering that will be run on a background thread.
    auto workItemHandler = ref new Windows::System::Threading::WorkItemHandler([this](Windows::Foundation::IAsyncAction ^ action)
    {
        critical_section::scoped_lock lock(mRenderSurfaceCriticalSection);

        mOpenGLES->MakeCurrent(mRenderSurface);

        if (mRenderer.get() == nullptr)
        {
            mRenderer = std::make_shared<SimpleRenderer>();
        }

        while (action->Status == Windows::Foundation::AsyncStatus::Started)
        {
            EGLint panelWidth = 0;
            EGLint panelHeight = 0;
            mOpenGLES->GetSurfaceDimensions(mRenderSurface, &panelWidth, &panelHeight);
            
            // Logic to update the scene could go here
            mRenderer->UpdateWindowSize(panelWidth, panelHeight);
            mRenderer->Draw();

            // The call to eglSwapBuffers might not be successful (i.e. due to Device Lost)
            // If the call fails, then we must reinitialize EGL and the GL resources.
            if (mOpenGLES->SwapBuffers(mRenderSurface) != GL_TRUE)
            {
                // XAML objects like the SwapChainPanel must only be manipulated on the UI thread.
                swapChainPanel->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([=]()
                {
                    RecoverFromLostDevice();
                }, CallbackContext::Any));

                return;
            }
        }
    });

    // Run task on a dedicated high priority background thread.
    mRenderLoopWorker = Windows::System::Threading::ThreadPool::RunAsync(workItemHandler, Windows::System::Threading::WorkItemPriority::High, Windows::System::Threading::WorkItemOptions::TimeSliced);
}

void OpenGLESPage::StopRenderLoop()
{
    if (mRenderLoopWorker)
    {
        mRenderLoopWorker->Cancel();
        mRenderLoopWorker = nullptr;
    }
}

void OpenGLESPage::CreateInput()
{
    // Register our SwapChainPanel to get independent input pointer events
    auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^)
    {
        // The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
        mCoreInput = swapChainPanel->CreateCoreIndependentInputSource(
            Windows::UI::Core::CoreInputDeviceTypes::Mouse |
            Windows::UI::Core::CoreInputDeviceTypes::Touch |
            Windows::UI::Core::CoreInputDeviceTypes::Pen
            );

        // Register for pointer events, which will be raised on the background thread.
        mCoreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &OpenGLESPage::OnPointerPressed);
        mCoreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &OpenGLESPage::OnPointerMoved);
        mCoreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &OpenGLESPage::OnPointerReleased);

        // Begin processing input messages as they're delivered.
        mCoreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
    });

    // Run task on a dedicated high priority background thread.
    mInputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void OpenGLESPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
    mRenderer->QueuePointerEvent(PointerEventType::PointerPressed, e->CurrentPoint->Position.X * mScreenResolutionScale, e->CurrentPoint->Position.Y * mScreenResolutionScale, e->CurrentPoint->PointerId);
}

void OpenGLESPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e)
{
    mRenderer->QueuePointerEvent(PointerEventType::PointerMoved, e->CurrentPoint->Position.X * mScreenResolutionScale, e->CurrentPoint->Position.Y * mScreenResolutionScale, e->CurrentPoint->PointerId);
}

void OpenGLESPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e)
{
    mRenderer->QueuePointerEvent(PointerEventType::PointerReleased, e->CurrentPoint->Position.X * mScreenResolutionScale, e->CurrentPoint->Position.Y * mScreenResolutionScale, e->CurrentPoint->PointerId);
}

void OpenGLESPage::OnKeyPressed(CoreWindow^ sender, KeyEventArgs^ e)
{
    if (!e->KeyStatus.WasKeyDown)
    {
        //log("OpenGLESPage::OnKeyPressed %d", e->VirtualKey);
        if (mRenderer)
        {
            mRenderer->QueueKeyEvent(KeyEventType::KeyDown, e);
        }
    }
}

void OpenGLESPage::OnKeyReleased(CoreWindow^ sender, KeyEventArgs^ e)
{
    //log("OpenGLESPage::OnKeyReleased %d", e->VirtualKey);
    if (mRenderer)
    {
        mRenderer->QueueKeyEvent(KeyEventType::KeyUp, e);
    }
}