#pragma once

#include "pch.h"
#include "Game.h"
#include "Timer.h"
#include <memory>
#include "winrt/Input.h"

namespace Breakout
{
    class SimpleRenderer : public WinRT::Input
    {
    public:
        SimpleRenderer();
        ~SimpleRenderer();
        void Draw();
        void UpdateWindowSize(GLsizei width, GLsizei height);

    private:
        std::shared_ptr<Game> mGame;
        std::unique_ptr<Timer> mTimer;
        GLsizei mWindowWidth;
        GLsizei mWindowHeight;
        int mDrawCount;

        //User input
        virtual void OnPointerPressed(std::shared_ptr<WinRT::PointerEvent> e);
        virtual void OnPointerMoved(std::shared_ptr<WinRT::PointerEvent> e);
        virtual void OnPointerReleased(std::shared_ptr<WinRT::PointerEvent> e);
        virtual void OnKeyDown(std::shared_ptr<WinRT::KeyEvent> e);
        virtual void OnKeyUp(std::shared_ptr<WinRT::KeyEvent> e);
        void SetKeyState(Windows::System::VirtualKey key, bool state);
    };
}