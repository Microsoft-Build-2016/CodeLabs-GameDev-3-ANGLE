<a name="HOLTop" />
# Windows 10 Phone #

---


<a name="Exercise6" />
### Exercise 6: Windows 10 Phone ###

Our Breakout game is a Windows 10 UWP app. This means that the same code can run as a desktop/tablet app as well as a Windows Phone app. This exercise will show you how to run Breakout on the Windows 10 Phone emulator. In future exercises we will add support for Windows 10 Phone specific features like supporting the back button.

In this exercise you will complete the following steps:

1. Run the phone on the Windows 10 Phone emulator.
2. Modify the ANGLE template to correctly support the screen resolutions on the Windows 10 Phone.

#### Task 1 - Running the Game on the Windows 10 Phone Emulator ####

1. Open **Breakout.sln** in the **CodeLabs/Workshops/Games/Module3-ANGLE/Source/Ex6/Begin** folder.  

2. Select **Debug x86** from the Project Configuration and Platform menus. Also select **Mobile Emulator 10.0.10586.0 WVGA 4 inch 1GB** from the device menu.

	![Configuring the build target](../../Images/ex6-select-phone-emulator.png?raw=true "Configuring the build target")

	_Configuring the build target and Phone emulator_

3. Press **F5** to build and run the project. Visual Studio will launch the Windows 10 Phone emulator and load your app onto the phone. This process may take a few minutes to complete. Once the app has been loaded onto the phone, it should look like the following.

	![Breakout App on Windows Phone_](../../Images/ex6-phone-wvga-4-inch.png?raw=true "Breakout App on Windows Phone_")

	_Breakout App on Windows Phone_

You should be able to play the game by touching the screen using your mouse.

#### Task 2 - Obtaining Correct Screen Resolution ####

You may have noticed that our game looks kind of fuzzy on the Windows 10 phone screen. This is because by default, ANGLE specifies its window size (or SwapChainPanel size) in DIPs (Device Independent Pixels). This is usually not a problem in desktops apps as there is generally a 1-1 scaling factor between DIPs and logical DPI. However, on the phone, there is almost always the need for some scaling to get the true resolution of the screen. In order to get the ANGLE to use the actual resolution of the screen on a Windows phone, we will need to specify a scaling factor based on the logical DPI of the device.

1. Modify **OpenGLESPage::CreateRenderSurface()** in **OpenGLESPage.xaml.cpp** as follows:

    ````C++
    void OpenGLESPage::CreateRenderSurface()
    {
        if (mOpenGLES && mRenderSurface == EGL_NO_SURFACE)
        {
            // The app can configure the the SwapChainPanel which may boost performance.
            // By default, this template uses the default configuration.
            // mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, nullptr, nullptr);

            Windows::Graphics::Display::DisplayInformation^ info = Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
            mScreenResolutionScale = info->LogicalDpi / 96.0f;

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
            mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, nullptr, &mScreenResolutionScale);
        }
    }
    ````

2. Save your work. Press **F5** to build and run your app. Your app should now look a lot better since it is rendering at the resolution of the screen.

	![Breakout App on Windows Phone_](../../Images/ex6-phone-correct-resolution.png?raw=true "Breakout App on Windows Phone_")

	_Breakout App on Windows Phone_

#### Discussion ####

You have now run the Breakout game on a Windows 10 Phone at the resolution of the actual screen on the device. One important thing to consider is that depending on the complexity of the graphics in your game, you may need to reduce the resolution of the rendering chain in order to achieve 60 FPS. You will need to experiment with the parameters to `mOpenGLES->CreateSurface()` in `OpenGLESPage::CreateRenderSurface()` to find the best balance between resolution and performance.

Take a look at `OpenGLESPage::OnPointerPressed()` around line 200 in **OpenGLESPage.xaml.cpp**.

````C++
void OpenGLESPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
	mRenderer->QueuePointerEvent(PointerEventType::PointerPressed,
		e->CurrentPoint->Position.X * mScreenResolutionScale,
		e->CurrentPoint->Position.Y * mScreenResolutionScale,
		e->CurrentPoint->PointerId);
}
````

You will notice that if we apply a scale factor to ANGLE's swapchain we will need to apply the same scale factor to the pointer coordinates before we send them on to our game.


#### Next ####

This completes the ANGLE workshop.

- Return to [Start](../../README.md)
