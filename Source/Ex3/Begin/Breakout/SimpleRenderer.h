#pragma once

#include "pch.h"
#include "Game.h"
#include "Timer.h"
#include <memory>

namespace Breakout
{
    class SimpleRenderer
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
    };
}