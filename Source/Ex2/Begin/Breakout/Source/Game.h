/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#ifdef WINRT
#include "keys-winrt.h"
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleGenerator.h"

#include <memory>
#include <vector>

// Represents the current state of the game
enum class GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

// Defines a Collision typedef that represents collision data
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>


// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
     GLboolean              Keys[1024];

    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();

    void CursorDown(GLfloat xpos, GLfloat ypos);
    void CursorMove(GLfloat xpos, GLfloat ypos);
    void CursorUp(GLfloat xpos, GLfloat ypos);

    // GameLoop
    void ProcessInput(GLfloat dt);
    void Update(GLfloat dt);
    void Render();

    // Reset
    void ResetLevel();
    void ResetPlayer();

private:
    void InitializeBallVelocity();

    // Collision
    void DoCollisions();
    Collision CheckCollision(const BallObject &one, const GameObject &two);
    GLboolean CheckCollision(const GameObject &one, const GameObject &two); // AABB - AABB collision
    Direction VectorDirection(const glm::vec2 &target);

    // Game state
    GameState                           m_state;

    // Window dimensions
    GLuint                              m_width;
    GLuint                              m_height;
    glm::mat4                           m_projection;

    // Game levels
    std::vector<GameLevel>              m_levels;
    GLuint                              m_level;

    // game objects
    std::shared_ptr<SpriteRenderer>     m_renderer;
    std::shared_ptr<GameObject>         m_player;
    std::shared_ptr<BallObject>         m_ball;
    std::shared_ptr<ParticleGenerator>  m_particles;

    // Initial ball velocity based on screen size
    glm::vec2                           m_initialBallVelocity;

    // tracks if mouse/pointer down event has started
    bool                                m_bCursorDown;
};

#endif