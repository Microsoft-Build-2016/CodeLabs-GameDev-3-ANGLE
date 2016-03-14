//
// This file is used by the template to render a basic scene using GL.
//

#include "pch.h"
#include "SimpleRenderer.h"

using namespace Platform;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Breakout;
using namespace WinRT;

SimpleRenderer::SimpleRenderer() :
    mWindowWidth(0),
    mWindowHeight(0),
    mDrawCount(0)
{
    mTimer.reset(CreateTimer());
}

SimpleRenderer::~SimpleRenderer()
{
}

void SimpleRenderer::Draw()
{
    if (mGame != nullptr)
    {
        float deltaTime = static_cast<float>(mTimer->getDeltaTime());
        ProcessEvents();
        mGame->ProcessInput(deltaTime);
        mGame->Update(deltaTime);
        mGame->Render();
    }

    mDrawCount += 1;
}

void SimpleRenderer::UpdateWindowSize(GLsizei width, GLsizei height)
{
    if (mGame == nullptr)
    {
        mGame = std::make_shared<Game>(width, height);
        mGame->Init();
        mTimer->start();
    }
    else if (mWindowWidth != width || mWindowHeight != height)
    {
        mGame->Resize(width, height);
    }

    mWindowWidth = width;
    mWindowHeight = height;
}

void SimpleRenderer::OnPointerPressed(std::shared_ptr<PointerEvent> e)
{
    mGame->CursorDown(e->_x, e->_y);
}

void SimpleRenderer::OnPointerMoved(std::shared_ptr<PointerEvent> e)
{
    mGame->CursorMove(e->_x, e->_y);
}

void SimpleRenderer::OnPointerReleased(std::shared_ptr<PointerEvent> e)
{
    mGame->CursorUp(e->_x, e->_y);
}

void SimpleRenderer::OnKeyDown(std::shared_ptr<KeyEvent> e)
{
    KeyEventArgs^ args = e->m_key.Get();
    SetKeyState(args->VirtualKey, true);
}

void SimpleRenderer::OnKeyUp(std::shared_ptr<KeyEvent> e)
{
    KeyEventArgs^ args = e->m_key.Get();
    SetKeyState(args->VirtualKey, false);
}

void SimpleRenderer::SetKeyState(VirtualKey key, bool state)
{
    GLboolean keyDown = state ? GL_TRUE : GL_FALSE;
    switch (key)
    {
    case VirtualKey::D:
    case VirtualKey::Right:
        mGame->Keys[GLFW_KEY_D] = keyDown;
        break;

    case VirtualKey::A:
    case VirtualKey::Left:
        mGame->Keys[GLFW_KEY_A] = keyDown;
        break;

    case VirtualKey::Space:
        mGame->Keys[GLFW_KEY_SPACE] = keyDown;
        break;
    }
}