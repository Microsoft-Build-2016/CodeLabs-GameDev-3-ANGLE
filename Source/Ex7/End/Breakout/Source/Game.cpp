/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "Game.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

// Initial velocity of the ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100, 20);
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);

const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 600.0f;

Game::Game(GLuint width, GLuint height)
    : m_state(GameState::GAME_ACTIVE), Keys(), m_width(width), m_height(height), m_bCursorDown(false)
{
    InitializeBallVelocity();
}

Game::~Game()
{

}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", "sprite");
    ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.frag", "particle");

    // Configure shaders
    m_projection = glm::ortho(0.0f, static_cast<GLfloat>(m_width), static_cast<GLfloat>(m_height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", m_projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", m_projection);

    // Load textures
    ResourceManager::LoadTexture("textures/background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("textures/ball.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("textures/block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
    ResourceManager::LoadTexture("textures/particle.png", GL_TRUE, "particle");

    // Set render-specific controls
    m_renderer = std::make_shared<SpriteRenderer>(ResourceManager::GetShader("sprite"));

    m_particles = std::make_shared<ParticleGenerator>(
        ResourceManager::GetShader("particle"),
        ResourceManager::GetTexture("particle"),
        500
        );

    // Load levels
    GameLevel one; one.Load("levels/one.lvl", m_width, static_cast<GLuint>(m_height * 0.5));
    GameLevel two; two.Load("levels/two.lvl", m_width, static_cast<GLuint>(m_height * 0.5));
    GameLevel three; three.Load("levels/three.lvl", m_width, static_cast<GLuint>(m_height * 0.5));
    GameLevel four; four.Load("levels/four.lvl", m_width, static_cast<GLuint>(m_height * 0.5));
    m_levels.push_back(one);
    m_levels.push_back(two);
    m_levels.push_back(three);
    m_levels.push_back(four);
    m_level = 0;

    // Configure game objects
    glm::vec2 playerPos = glm::vec2(m_width / 2 - PLAYER_SIZE.x / 2, m_height - PLAYER_SIZE.y);
    m_player = std::make_shared<GameObject>(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

    glm::vec2 m_ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    m_ball = std::make_shared<BallObject>(m_ballPos, BALL_RADIUS, m_initialBallVelocity, ResourceManager::GetTexture("face"));

    // set up OpenGL
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}

// scale the ball velocity with the size of the screen
void Game::InitializeBallVelocity()
{
    m_initialBallVelocity.x = (float)m_width / SCREEN_WIDTH * INITIAL_BALL_VELOCITY.x;
    m_initialBallVelocity.y = (float)m_height / SCREEN_HEIGHT * INITIAL_BALL_VELOCITY.y;
}


void Game::CursorDown(GLfloat xpos, GLfloat ypos)
{
    m_bCursorDown = true;
    if (m_ball->Stuck)
    {
        m_ball->Stuck = false;
    }
    else
    {
        m_player->Position.x = xpos - (PLAYER_SIZE.x / 2);
    }
}


void Game::CursorMove(GLfloat xpos, GLfloat ypos)
{
    if (m_bCursorDown)
    {
        m_player->Position.x = xpos - (PLAYER_SIZE.x / 2);
        if (m_ball->Stuck)
        {
            m_ball->Position = m_player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
        }
    }
}

void Game::CursorUp(GLfloat xpos, GLfloat ypos)
{
    m_bCursorDown = false;
    m_player->Position.x = xpos - (PLAYER_SIZE.x / 2);
}

void Game::Update(GLfloat dt)
{
    // Update objects
    m_ball->Move(dt, m_width);

    // Check for collisions
    DoCollisions();

    // Check loss condition
    if (m_ball->Position.y >= m_height) // Did m_ball reach bottom edge?
    {
        ResetLevel();
        ResetPlayer();
    }

    // Update m_particles
    m_particles->Update(dt, *m_ball, 2, glm::vec2(m_ball->Radius / 2));
}


void Game::ProcessInput(GLfloat dt)
{
    if (m_state == GameState::GAME_ACTIVE)
    {
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // Move playerboard
        if (Keys[GLFW_KEY_A])
        {
            if (m_player->Position.x >= 0)
            {
                m_player->Position.x -= velocity;
                if (m_ball->Stuck)
                    m_ball->Position.x -= velocity;
            }
        }
        if (Keys[GLFW_KEY_D])
        {
            if (m_player->Position.x <= m_width - m_player->Size.x)
            {
                m_player->Position.x += velocity;
                if (m_ball->Stuck)
                    m_ball->Position.x += velocity;
            }
        }
        if (Keys[GLFW_KEY_SPACE])
            m_ball->Stuck = false;
    }
}

void Game::Render()
{
    if (m_state == GameState::GAME_ACTIVE)
    {
        glViewport(0, 0, static_cast<int>(m_width), static_cast<int>(m_height));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw background
        m_renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(m_width, m_height), 0.0f);

        // Draw level
        m_levels[m_level].Draw(*m_renderer);

        // Draw player
        m_player->Draw(*m_renderer);

        // Draw particles	
        m_particles->Draw();

        // Draw ball        
        m_ball->Draw(*m_renderer);
    }
}

void Game::ResetLevel()
{
    if (m_level == 0)
        m_levels[0].Load("levels/one.lvl", m_width, static_cast<GLuint>(m_height * 0.5f));
    else if (m_level == 1)
        m_levels[1].Load("levels/two.lvl", m_width, static_cast<GLuint>(m_height * 0.5f));
    else if (m_level == 2)
        m_levels[2].Load("levels/three.lvl", m_width, static_cast<GLuint>(m_height * 0.5f));
    else if (m_level == 3)
        m_levels[3].Load("levels/four.lvl", m_width, static_cast<GLuint>(m_height * 0.5f));
}

void Game::ResetPlayer()
{
    // Reset player/ball states
    m_player->Size = PLAYER_SIZE;

    if (!m_bCursorDown)
    {
        m_player->Position = glm::vec2(m_width / 2 - PLAYER_SIZE.x / 2, m_height - PLAYER_SIZE.y);
    }
    m_ball->Reset(m_player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), m_initialBallVelocity);
}

void Game::DoCollisions()
{
    for (GameObject &box : m_levels[m_level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*m_ball, box);
            if (std::get<0>(collision)) // If collision is true
            {
                // Destroy block if not solid
                if (!box.IsSolid)
                    box.Destroyed = GL_TRUE;
                // Collision resolution
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if (dir == Direction::LEFT || dir == Direction::RIGHT) // Horizontal collision
                {
                    m_ball->Velocity.x = -m_ball->Velocity.x; // Reverse horizontal velocity
                                                          // Relocate
                    GLfloat penetration = m_ball->Radius - std::abs(diff_vector.x);
                    if (dir == Direction::LEFT)
                        m_ball->Position.x += penetration; // Move m_ball to right
                    else
                        m_ball->Position.x -= penetration; // Move m_ball to left;
                }
                else // Vertical collision
                {
                    m_ball->Velocity.y = -m_ball->Velocity.y; // Reverse vertical velocity
                                                          // Relocate
                    GLfloat penetration = m_ball->Radius - std::abs(diff_vector.y);
                    if (dir == Direction::UP)
                        m_ball->Position.y -= penetration; // Move m_ball bback up
                    else
                        m_ball->Position.y += penetration; // Move m_ball back down
                }
            }
        }
    }
    // Also check collisions for player pad (unless stuck)
    Collision result = CheckCollision(*m_ball, *m_player);
    if (!m_ball->Stuck && std::get<0>(result))
    {
        // Check where it hit the board, and change velocity based on where it hit the board
        GLfloat centerBoard = m_player->Position.x + m_player->Size.x / 2;
        GLfloat distance = (m_ball->Position.x + m_ball->Radius) - centerBoard;
        GLfloat percentage = distance / (m_player->Size.x / 2);
        // Then move accordingly
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = m_ball->Velocity;
        m_ball->Velocity.x = m_initialBallVelocity.x * percentage * strength;
        //m_ball->Velocity.y = -m_ball->Velocity.y;
        m_ball->Velocity = glm::normalize(m_ball->Velocity) * glm::length(oldVelocity); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
                                                                                    // Fix sticky paddle
        m_ball->Velocity.y = -1 * abs(m_ball->Velocity.y);
    }
}

GLboolean Game::CheckCollision(const GameObject &one, const GameObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
        two.Position.x + two.Size.x >= one.Position.x;
    // Collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
        two.Position.y + two.Size.y >= one.Position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}

Collision Game::CheckCollision(const BallObject &one, const GameObject &two) // AABB - Circle collision
{
    // Get center point circle first 
    glm::vec2 center(one.Position + one.Radius);
    // Calculate AABB info (center, half-extents)
    glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
    glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
    // Get difference vector between both centers
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // Now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
    glm::vec2 closest = aabb_center + clamped;
    // Now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, Direction::UP, glm::vec2(0, 0));
}

// Calculates which direction a vector is facing (N,E,S or W)
Direction Game::VectorDirection(const glm::vec2 &target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),	// up
        glm::vec2(1.0f, 0.0f),	// right
        glm::vec2(0.0f, -1.0f),	// down
        glm::vec2(-1.0f, 0.0f)	// left
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
} 

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