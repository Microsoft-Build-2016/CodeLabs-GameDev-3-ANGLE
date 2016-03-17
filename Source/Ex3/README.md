<a name="HOLTop" />
# Resizing the Window #

---

<a name="Exercise3" />
### Exercise 3: Resizing the Window ###

In this exercise, you will modify the game project from Exercise 2 to add support for resizing the app's window. Windows 10 UWP apps are expected to be resizable on the desktop. Your game will need to correctly handle changes to the window size made by the user.

If your game only supports a fixed size (for example 640 x 480) or aspect ratio, you will need to add letterboxing which we will demonstrate in a future tutorial.

#### Task 1 - Handling Windows Size Changes ####

The ANGLE XAML App for OpenGL ES template generates a Windows 10 XAML app that is correctly set up to use OpenGL ES 2.0 in your app. Your game code will need to be integrated with the OpenGLESPage.xaml page. This page handles the initialization of the OpenGL context and creates a rendering loop in which you will update and render your game. There are a few minor modifications we need to make to the OpenGLESPage XAML code to prepare the page for our game.

1. Open **Breakout.sln** in the **CodeLabs/Workshops/Games/Module3-ANGLE/Source/Ex3/Begin** folder.  

2. Select **Debug x64** from the Project Configuration and Platform dropdowns.

	![Configuring the build target](../../Images/ex2-debug-x64.PNG?raw=true "Configuring the build target")

	_Configuring the build target_

3. Press **F5** to build and run the project. The app should look like the following.

	![Breakout App](../../Images/ex2-breakout-app.png?raw=true "Breakout App")

	_Breakout App_

4. Click the window maximize icon in the top right corner of the window or simply resize the window. You will see how the game does not update its size with the window size.

5. Open the file **SimpleRenderer.cpp** and modify **SimpleRenderer::UpdateWindowSize()** as follows.

	````C++
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
	````

	The rendering loop in `OpenGLESPage::StartRenderLoop()` updates the size of the window each time it is ready to render a new frame. It will call `SimpleRenderer::UpdateWindowSize()` with the current size of the window. If the size has changed you will need notify your game code.

6. Open **Source/Game.h** and add the following `Resize()` public method declaration.

	````C++
	void ResetPlayer();

	// Window Resize
	void Resize(GLuint width, GLuint height);
	````

7. Open Source/Game.cpp and add the following Resize() method at the end of the file.

	````C++
	void Game::Resize(GLuint width, GLuint height)
	{
		if (width != m_width || height != m_height)
		{
			m_width = width;
			m_height = height;
			m_projection = glm::ortho(0.0f, static_cast<GLfloat>(m_width), static_cast<GLfloat>(m_height), 0.0f, -1.0f, 1.0f);

			glViewport(0, 0, static_cast<int>(m_width), static_cast<int>(m_height));

			m_levels[m_level].Resize(m_width, static_cast<GLuint>(m_height * 0.5));

			InitializeBallVelocity();

			m_player->Position = glm::vec2(m_width / 2 - PLAYER_SIZE.x / 2, m_height - PLAYER_SIZE.y);
			m_ball->Position = m_player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);

			m_ball->Velocity = m_initialBallVelocity;

			// update shader uniforms
			ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
			ResourceManager::GetShader("sprite").SetMatrix4("projection", m_projection);
			ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
			ResourceManager::GetShader("particle").SetMatrix4("projection", m_projection);
		}
	}
	````

	Note that when the window size has changed, your game code will need to update its projection matrix and glViewPort to accommodate the new window size. For this particular game, we also needed to add extra code to update the size of the blocks in the game level, the ball velocity and position. Each game is different so you may need to modify your game code to correctly handle the new window size. If you want to maintain a fixed window size, you may need to letterbox to fit your game inside the new window dimensions.

8. Save your work. Press **F5** to build and run your app. You should now be able to resize your game when the window is resized by the user.

#### Discussion ####

ANGLE automatically keeps track of the dimensions of the windows or SwapChainPanel used by the app as its rendering surface. Since we have created our project using the ANGLE XAML App for OpenGL ES template, our app uses a SwapChainPanel for its rendering. The SwapChainPanel is specified in the  OpenGLESPage.xaml file.

````XML
<Page
    x:Class="Breakout.OpenGLESPage"
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
    xmlns:local="using:Breakout"
    xmlns:d="http://schemas.microsoft.com/expression/blend/2008"
    xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006"
    mc:Ignorable="d">

    <SwapChainPanel x:Name="swapChainPanel">
    </SwapChainPanel>
</Page>
````

This SwapChainPanel will be the same size of the window. It will resize itself to the size of the window whenever the window size changes.

When the ANGLE app creates its rendering surface, it passes the SwapChainPanel to ANGLE.

````C++
void OpenGLESPage::CreateRenderSurface()
{
    if (mOpenGLES && mRenderSurface == EGL_NO_SURFACE)
    {
        // The app can configure the the SwapChainPanel which may boost performance.
        // By default, this template uses the default configuration.
        EGLint panelWidth = 0;
        EGLint panelHeight = 0;
        mRenderSurface = mOpenGLES->CreateSurface(swapChainPanel, nullptr, nullptr);
        mRenderer->UpdateWindowSize(panelWidth, panelHeight);
        ...     
    }
}
````

ANGLE will monitor the size of the SwapChainPanel which can be determined with GetSurfaceDimensions(). The rendering loop checks these dimensions each time it is ready to draw a frame.

````C++
void OpenGLESPage::StartRenderLoop()
{
    ...
        EGLint panelWidth = 0;
        EGLint panelHeight = 0;
        mOpenGLES->GetSurfaceDimensions(mRenderSurface, &panelWidth, &panelHeight);
        mRenderer->UpdateWindowSize(panelWidth, panelHeight);
    ...
}    
````

#### Next ####

We will now add support for handling keyboard and pointer events.

- Continue on to [Exercise 4: Handling Keyboard and Pointer Events](../..//Source/Ex4/README.md)
- Return to [Start](../../README.md)
