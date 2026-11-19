#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "EntityManager.h"

class Game
{
    sf::RenderWindow m_window;
    EntityManager m_entityManager;

    // Window settings
    int m_windowWidth = 1920;
    int m_windowHeight = 1080;
    int m_frameLimit = 60;

    // Player settings
    float m_playerRadius = 32.0f;
    float m_playerCollisionRadius = 16.0f;
    float m_playerSpeed = 5.0f;
    sf::Color m_playerFill = sf::Color::White;
    sf::Color m_playerOutline = sf::Color::Red;
    float m_playerOutlineThickness = 2.0f;
    int m_playerVertices = 8;

    // Enemy settings
    float m_enemyRadius = 32.0f;
    float m_enemyCollisionRadius = 16.0f;
    float m_enemyMinSpeed = 1.0f;
    float m_enemyMaxSpeed = 4.0f;
    sf::Color m_enemyOutline = sf::Color::White;
    float m_enemyOutlineThickness = 2.0f;
    int m_enemyMinVertices = 3;
    int m_enemyMaxVertices = 8;
    int m_enemySpawnInterval = 120;
    int m_currentFrame = 0;

    // Bullet settings
    float m_bulletRadius = 5.0f;
    float m_bulletCollisionRadius = 3.0f;
    float m_bulletSpeed = 12.0f;
    sf::Color m_bulletFill = sf::Color::White;
    sf::Color m_bulletOutline = sf::Color::Red;
    float m_bulletOutlineThickness = 1.0f;
    int m_bulletVertices = 8;
    int m_bulletLifespan = 90;

    // Font and score
    sf::Font m_font;
    sf::Text m_scoreText;
    int m_score = 0;

    // Game state
    bool m_running = true;
    bool m_paused = false;
    bool m_fontLoaded = false;

    // Timing
    sf::Clock m_clock;
    float m_deltaTime = 0.0f;

    // Player
    std::shared_ptr<Entity> m_player;

    // Special ability
    sf::Clock m_specialClock;
    bool m_specialActive = false;
    float m_specialDuration = 5.0f;
    float m_specialCooldown = 30.0f;
    float m_specialRemaining = 0.0f;

    // GUI system toggles
    bool m_systemMovement = true;
    bool m_systemEnemySpawner = true;
    bool m_systemCollision = true;
    bool m_systemLifespan = true;
    bool m_systemSpecialAbility = true;

    // Functions
    void loadConfig(const std::string& filename);
    void init();
    void spawnPlayer();
    void spawnEnemy();
    void spawnBullet();

    void sUserInput();
    void sMovement();
    void sEnemySpawner();
    void sCollision();
    void sLifespan();
    void sSpecialAbility();
    void sGUI();
    void sRender();

public:
    Game();
    void run();
};
