
#include "Game.h"
#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace
{
    const float PI = 3.14159265f;

    int randomInt(int min, int max)
    {
        return min + std::rand() % (max - min + 1);
    }

    float randomFloat(float min, float max)
    {
        return min + static_cast<float>(std::rand()) /
            static_cast<float>(RAND_MAX) * (max - min);
    }

    sf::Color randomColor()
    {
        return sf::Color(
            randomInt(50, 255),
            randomInt(50, 255),
            randomInt(50, 255)
        );
    }

    Vec2<float> directionBetween(
        const Vec2<float>& from,
        const Vec2<float>& to)
    {
        return (to - from).normalize();
    }
}

Game::Game()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    loadConfig("config.txt");

    m_window.create(
        sf::VideoMode(m_windowWidth, m_windowHeight),
        "Assignment 2"
    );

    m_window.setFramerateLimit(m_frameLimit);

    // Initialize Dear ImGui after creating the SFML window.
    ImGui::SFML::Init(m_window);

    if (m_fontLoaded)
    {
        m_scoreText.setFont(m_font);
        m_scoreText.setCharacterSize(24);
        m_scoreText.setFillColor(sf::Color::White);
        m_scoreText.setPosition(10.0f, 10.0f);
    }

    init();
}

void Game::loadConfig(const std::string& filename)
{
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "Could not open config.txt. Using defaults.\n";
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "Window")
        {
            ss >> m_windowWidth >> m_windowHeight >> m_frameLimit;
        }
        else if (type == "Font")
        {
            std::string path;
            unsigned int r, g, b;
            unsigned int size;

            ss >> path >> size >> r >> g >> b;

            m_fontLoaded = m_font.loadFromFile("D:\\Work\\C++\\ConsoleApplication1\\ARIAL.ttf");

            if (m_fontLoaded)
            {
                m_scoreText.setCharacterSize(size);
                m_scoreText.setFillColor(sf::Color(r, g, b));
            }
            else
            {
                std::cout << "Could not load font: " << path << "\n";
            }
        }
        else if (type == "Player")
        {
            unsigned int fr, fg, fb;
            unsigned int or_, og, ob;

            ss >> m_playerRadius
                >> m_playerCollisionRadius
                >> m_playerSpeed
                >> fr >> fg >> fb
                >> or_ >> og >> ob
                >> m_playerOutlineThickness
                >> m_playerVertices;

            m_playerFill = sf::Color(fr, fg, fb);
            m_playerOutline = sf::Color(or_, og, ob);
        }
        else if (type == "Enemy")
        {
            unsigned int r, g, b;

            ss >> m_enemyRadius
                >> m_enemyCollisionRadius
                >> m_enemyMinSpeed
                >> m_enemyMaxSpeed
                >> r >> g >> b
                >> m_enemyOutlineThickness
                >> m_enemyMinVertices
                >> m_enemyMaxVertices
                >> m_enemySpawnInterval;

            m_enemyOutline = sf::Color(r, g, b);
        }
        else if (type == "Bullet")
        {
            unsigned int fr, fg, fb;
            unsigned int or_, og, ob;

            ss >> m_bulletRadius
                >> m_bulletCollisionRadius
                >> m_bulletSpeed
                >> fr >> fg >> fb
                >> or_ >> og >> ob
                >> m_bulletOutlineThickness
                >> m_bulletVertices
                >> m_bulletLifespan;

            m_bulletFill = sf::Color(fr, fg, fb);
            m_bulletOutline = sf::Color(or_, og, ob);
        }
    }
}

void Game::init()
{
    spawnPlayer();

    // Start with one enemy.
    spawnEnemy();

    m_entityManager.Update();
}

void Game::spawnPlayer()
{
    m_player = m_entityManager.addEntity("Player");

    m_player->add<CTransform>(
        Vec2<float>(
            m_windowWidth / 2.0f,
            m_windowHeight / 2.0f
        ),
        Vec2<float>(0.0f, 0.0f),
        randomFloat(0.0f, 360.0f)
    );

    m_player->add<CShape>(
        m_playerRadius,
        m_playerVertices,
        m_playerFill,
        m_playerOutline,
        m_playerOutlineThickness
    );

    m_player->add<CCollision>(m_playerCollisionRadius);
    m_player->add<CInput>();
}

void Game::spawnEnemy()
{
    int vertices = randomInt(
        m_enemyMinVertices,
        m_enemyMaxVertices
    );

    float x = randomFloat(
        m_enemyRadius,
        m_windowWidth - m_enemyRadius
    );

    float y = randomFloat(
        m_enemyRadius,
        m_windowHeight - m_enemyRadius
    );

    Vec2<float> velocity(
        randomFloat(-m_enemyMaxSpeed, m_enemyMaxSpeed),
        randomFloat(-m_enemyMaxSpeed, m_enemyMaxSpeed)
    );

    if (velocity.length() < m_enemyMinSpeed)
    {
        velocity = Vec2<float>(m_enemyMinSpeed, 0.0f);
    }

    auto enemy = m_entityManager.addEntity("Enemy");

    enemy->add<CTransform>(
        Vec2<float>(x, y),
        velocity,
        randomFloat(0.0f, 360.0f)
    );

    enemy->add<CShape>(
        m_enemyRadius,
        vertices,
        randomColor(),
        m_enemyOutline,
        m_enemyOutlineThickness
    );

    enemy->add<CCollision>(m_enemyCollisionRadius);

    enemy->add<CScore>(vertices * 100);
}

void Game::spawnBullet()
{
    if (!m_player || !m_player->IsActive())
    {
        return;
    }

    auto& playerTransform = m_player->get<CTransform>();

    sf::Vector2i mouse = sf::Mouse::getPosition(m_window);

    Vec2<float> target(
        static_cast<float>(mouse.x),
        static_cast<float>(mouse.y)
    );

    Vec2<float> direction = directionBetween(
        playerTransform.position,
        target
    );

    if (direction.length() == 0.0f)
    {
        return;
    }

    auto bullet = m_entityManager.addEntity("Bullet");

    bullet->add<CTransform>(
        playerTransform.position,
        direction * m_bulletSpeed,
        playerTransform.angle
    );

    bullet->add<CShape>(
        m_bulletRadius,
        m_bulletVertices,
        m_bulletFill,
        m_bulletOutline,
        m_bulletOutlineThickness
    );

    bullet->add<CCollision>(m_bulletCollisionRadius);
    int bulletLifespanMs =
        m_bulletLifespan * 1000 / m_frameLimit;

    bullet->add<CLifespan>(bulletLifespanMs);
}

void Game::sUserInput()
{
    sf::Event event;

    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }

        if (event.type == sf::Event::KeyPressed &&
            !ImGui::GetIO().WantCaptureKeyboard)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                m_window.close();
            }

            if (event.key.code == sf::Keyboard::P)
            {
                m_paused = !m_paused;
            }

            if (event.key.code == sf::Keyboard::Space)
            {
                if (!m_specialActive &&
                    m_specialClock.getElapsedTime().asSeconds() >= m_specialCooldown)
                {
                    m_specialActive = true;
                    m_specialRemaining = m_specialDuration;
                }
            }
        }

        if (event.type == sf::Event::MouseButtonPressed &&
            !ImGui::GetIO().WantCaptureMouse)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                spawnBullet();
            }
        }
    }
}


void Game::sMovement()
{
    // STEP 1: Move the player.
    if (m_player && m_player->IsActive())
    {
        auto& transform = m_player->get<CTransform>();

        Vec2<float> movement(0.0f, 0.0f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            movement.y -= 1.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            movement.y += 1.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            movement.x -= 1.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            movement.x += 1.0f;

        if (movement.length() > 0.0f)
        {
            transform.velocity = movement.normalize() * m_playerSpeed;
        }
        else
        {
            transform.velocity = Vec2<float>(0.0f, 0.0f);
        }

        transform.position +=
            transform.velocity * m_deltaTime * 60.0f;

        // Keep player inside the window.
        transform.position.x = std::max(
            m_playerRadius,
            std::min(
                transform.position.x,
                m_windowWidth - m_playerRadius
            )
        );

        transform.position.y = std::max(
            m_playerRadius,
            std::min(
                transform.position.y,
                m_windowHeight - m_playerRadius
            )
        );

        // Rotate player towards the mouse.
        sf::Vector2i mouse = sf::Mouse::getPosition(m_window);

        float dx = mouse.x - transform.position.x;
        float dy = mouse.y - transform.position.y;

        transform.angle = std::atan2(dy, dx) * 180.0f / PI;
    }

    // STEP 2: Move all other entities.
    for (auto& entity : m_entityManager.getEntities())
    {
        if (!entity->IsActive())
            continue;

        // Player was already moved above.
        if (entity->Tag() == "Player")
            continue;

        if (!entity->has<CTransform>())
            continue;

        auto& transform = entity->get<CTransform>();

        bool isEnemy =
            entity->Tag() == "Enemy" ||
            entity->Tag() == "SmallEnemy";

        // Freeze enemies during special ability.
        if (!(m_specialActive && isEnemy))
        {
            transform.position +=
                transform.velocity * m_deltaTime * 60.0f;
        }

        // Rotate entities.
        transform.angle += 1.0f;

        // Bounce regular enemies off window boundaries.
        if (entity->Tag() == "Enemy")
        {
            float radius = entity->get<CCollision>().radius;

            if (transform.position.x <= radius ||
                transform.position.x >= m_windowWidth - radius)
            {
                transform.velocity.x *= -1.0f;
            }

            if (transform.position.y <= radius ||
                transform.position.y >= m_windowHeight - radius)
            {
                transform.velocity.y *= -1.0f;
            }
        }
    }
}

void Game::sEnemySpawner()
{
    m_currentFrame++;

    if (m_currentFrame >= m_enemySpawnInterval)
    {
        spawnEnemy();
        m_currentFrame = 0;
    }
}

void Game::sCollision()
{
    auto& entities = m_entityManager.getEntities();

    for (auto& bullet : entities)
    {
        if (!bullet->IsActive() || bullet->Tag() != "Bullet")
        {
            continue;
        }

        for (auto& enemy : entities)
        {
            if (!enemy->IsActive() ||
                enemy->Tag() != "Enemy")
            {
                continue;
            }

            auto& bt = bullet->get<CTransform>();
            auto& et = enemy->get<CTransform>();

            float distance = bt.position.distance(et.position);

            float collisionDistance =
                bullet->get<CCollision>().radius +
                enemy->get<CCollision>().radius;

            if (distance <= collisionDistance)
            {
                m_score += enemy->get<CScore>().score;

                int vertices =
                    enemy->get<CShape>().circle.getPointCount();

                auto position = et.position;
                auto color = enemy->get<CShape>().circle.getFillColor();

                enemy->Destroy();
                bullet->Destroy();

                // Split large enemies into smaller enemies.
                for (int i = 0; i < vertices; i++)
                {
                    float angle =
                        (360.0f / vertices) * i * PI / 180.0f;

                    Vec2<float> velocity(
                        std::cos(angle) * m_enemyMaxSpeed,
                        std::sin(angle) * m_enemyMaxSpeed
                    );

                    auto small = m_entityManager.addEntity("SmallEnemy");

                    small->add<CTransform>(
                        position,
                        velocity,
                        randomFloat(0.0f, 360.0f)
                    );

                    small->add<CShape>(
                        m_enemyRadius / 2.0f,
                        vertices,
                        color,
                        m_enemyOutline,
                        m_enemyOutlineThickness
                    );

                    small->add<CCollision>(
                        m_enemyCollisionRadius / 2.0f
                    );

                    small->add<CScore>(vertices * 200);
                    small->add<CLifespan>(1200);
                }

                break;
            }
        }
    }

    // Small enemy lifespan and collisions.
    for (auto& small : entities)
    {
        if (!small->IsActive() ||
            small->Tag() != "SmallEnemy")
        {
            continue;
        }

        for (auto& bullet : entities)
        {
            if (!bullet->IsActive() ||
                bullet->Tag() != "Bullet")
            {
                continue;
            }

            float distance =
                small->get<CTransform>().position.distance(
                    bullet->get<CTransform>().position
                );

            float collisionDistance =
                small->get<CCollision>().radius +
                bullet->get<CCollision>().radius;

            if (distance <= collisionDistance)
            {
                m_score += small->get<CScore>().score;
                small->Destroy();
                bullet->Destroy();
                break;
            }
        }
    }

    // Player collision with enemies.
    if (m_player && m_player->IsActive())
    {
        for (auto& enemy : entities)
        {
            if (!enemy->IsActive())
            {
                continue;
            }

            if (enemy->Tag() != "Enemy" &&
                enemy->Tag() != "SmallEnemy")
            {
                continue;
            }

            float distance =
                m_player->get<CTransform>().position.distance(
                    enemy->get<CTransform>().position
                );

            float collisionDistance =
                m_player->get<CCollision>().radius +
                enemy->get<CCollision>().radius;

            if (distance <= collisionDistance)
            {
                enemy->Destroy();

                // Respawn the player in the center.
                m_player->get<CTransform>().position = Vec2<float>(
                    m_windowWidth / 2.0f,
                    m_windowHeight / 2.0f
                );

                break;
            }
        }
    }
}

void Game::sLifespan()
{
    for (auto& entity : m_entityManager.getEntities())
    {
        if (!entity->IsActive() || !entity->has<CLifespan>())
        {
            continue;
        }

        auto& lifespan = entity->get<CLifespan>();

        lifespan.remaining -= static_cast<int>(
            m_deltaTime * 1000.0f
            );

        if (lifespan.remaining <= 0)
        {
            entity->Destroy();
            continue;
        }

        if (entity->has<CShape>())
        {
            float alpha =
                255.0f * lifespan.remaining /
                static_cast<float>(lifespan.lifespan);

            alpha = std::max(0.0f, std::min(255.0f, alpha));

            auto color = entity->get<CShape>().circle.getFillColor();
            color.a = static_cast<sf::Uint8>(alpha);

            entity->get<CShape>().circle.setFillColor(color);
        }
    }
}

void Game::sSpecialAbility()
{
    if (m_specialActive)
    {
        m_specialRemaining -= m_deltaTime;

        if (m_specialRemaining <= 0.0f)
        {
            m_specialActive = false;
            m_specialClock.restart();
        }
    }
}

void Game::sGUI()
{
    ImGui::Begin("Assignment 2 - Game Controls");

    ImGui::Text("Score: %d", m_score);
    ImGui::Separator();
    ImGui::Text("Systems");

    ImGui::Checkbox("Movement system", &m_systemMovement);
    ImGui::Checkbox("Enemy spawning system", &m_systemEnemySpawner);
    ImGui::Checkbox("Collision system", &m_systemCollision);
    ImGui::Checkbox("Lifespan system", &m_systemLifespan);
    ImGui::Checkbox("Special ability system", &m_systemSpecialAbility);

    ImGui::Separator();
    ImGui::Text("Enemy spawning");
    ImGui::SliderInt("Spawn interval (frames)", &m_enemySpawnInterval, 1, 600);
    if (ImGui::Button("Spawn enemy now"))
    {
        spawnEnemy();
    }

    ImGui::Separator();
    ImGui::Text("Entities");
    const auto& allEntities = m_entityManager.getEntities();
    ImGui::Text("Total active entities: %d", static_cast<int>(allEntities.size()));

    auto drawEntityList = [](const char* sectionName, const EntityVec& entities)
        {
            if (ImGui::TreeNode(sectionName))
            {
                for (const auto& entity : entities)
                {
                    if (!entity || !entity->IsActive())
                        continue;

                    const std::size_t id = entity->Id();
                    ImGui::PushID(static_cast<int>(id));

                    float x = 0.0f;
                    float y = 0.0f;
                    if (entity->has<CTransform>())
                    {
                        const auto& position = entity->get<CTransform>().position;
                        x = position.x;
                        y = position.y;
                    }

                    ImGui::BulletText("ID: %llu | Tag: %s | Position: (%.1f, %.1f)",
                        static_cast<unsigned long long>(id),
                        entity->Tag().c_str(), x, y);

                    ImGui::SameLine();
                    if (ImGui::SmallButton("Destroy"))
                    {
                        entity->Destroy();
                    }

                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
        };

    drawEntityList("All entities", allEntities);

    const auto& players = m_entityManager.getEntities("Player");
    const auto& enemies = m_entityManager.getEntities("Enemy");
    const auto& smallEnemies = m_entityManager.getEntities("SmallEnemy");
    const auto& bullets = m_entityManager.getEntities("Bullet");

    drawEntityList("Players", players);
    drawEntityList("Enemies", enemies);
    drawEntityList("Small enemies", smallEnemies);
    drawEntityList("Bullets", bullets);

    ImGui::End();
}

void Game::sRender()
{
    m_window.clear(sf::Color(15, 15, 25));

    for (auto& entity : m_entityManager.getEntities())
    {
        if (!entity->IsActive() ||
            !entity->has<CTransform>() ||
            !entity->has<CShape>())
        {
            continue;
        }

        auto& transform = entity->get<CTransform>();
        auto& shape = entity->get<CShape>().circle;

        shape.setPosition(transform.position.x, transform.position.y);
        shape.setRotation(transform.angle);

        m_window.draw(shape);
    }

    if (m_fontLoaded)
    {
        m_scoreText.setString(
            "Score: " + std::to_string(m_score)
        );

        m_window.draw(m_scoreText);
    }

    if (m_paused && m_fontLoaded)
    {
        sf::Text pausedText("PAUSED", m_font, 40);
        pausedText.setFillColor(sf::Color::White);
        pausedText.setPosition(
            m_windowWidth / 2.0f - 80.0f,
            m_windowHeight / 2.0f - 30.0f
        );

        m_window.draw(pausedText);
    }

    sGUI();
    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::run()
{
    while (m_window.isOpen() && m_running)
    {
        m_deltaTime = m_clock.restart().asSeconds();

        ImGui::SFML::Update(m_window, sf::seconds(m_deltaTime));

        sUserInput();

        if (!m_paused)
        {
            if (m_systemMovement)
                sMovement();
            if (m_systemEnemySpawner)
                sEnemySpawner();
            if (m_systemCollision)
                sCollision();
            if (m_systemLifespan)
                sLifespan();
            if (m_systemSpecialAbility)
                sSpecialAbility();
        }

        m_entityManager.Update();

        sRender();
    }

    ImGui::SFML::Shutdown();
}
