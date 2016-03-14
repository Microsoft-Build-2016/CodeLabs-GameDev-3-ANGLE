//
// This file is used by the template to render a basic scene using GL.
//

#include "pch.h"
#include "SimpleRenderer.h"

using namespace Platform;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Breakout;

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