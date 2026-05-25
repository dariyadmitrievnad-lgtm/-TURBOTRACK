#include "Game.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>

bool Game::loadFont()
{
    std::vector<std::string> fontPaths = {
        "Oswald-Regular.ttf",
        "repos/Oswald-Regular.ttf",
        "../repos/Oswald-Regular.ttf",
        "res/Oswald-Regular.ttf",
        "C:/Windows/Fonts/arial.ttf"
    };

    for (const auto& path : fontPaths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_font.loadFromFile(path))
            {
                std::cout << "Font loaded: " << path << std::endl;
                return true;
            }
        }
    }

    std::cout << "No font found! Text will not be visible." << std::endl;
    return false;
}

Game::Game()
    : m_window(sf::VideoMode(1254, 900), "Turbotrack: Put' k finishu")
    , m_state(GameState::MENU)
    , m_menu(1254, 900)
    , m_currentLevel(1)
    , m_timer(0)
    , m_isPlaying(false)
    , m_collisionCount(0)
    , m_oilEffectTimer(0)
    , m_mudTimer(0)
    , m_hasShield(false)
    , m_shieldTimer(0)
    , m_speedBoostTimer(0)
    , m_cameraSmoothness(0.1f)
{
    m_window.setFramerateLimit(60);

    loadFont();

    setupText(m_timerText, 20, 20, 30);
    setupText(m_speedText, 20, 60, 30);
    setupText(m_bonusText, 20, 100, 24);
    setupText(m_levelText, 20, 140, 30);
    setupText(m_collisionText, 20, 180, 24);
    setupText(m_messageText, 627, 400, 40);
    m_messageText.setFillColor(sf::Color::Yellow);
    m_messageText.setOrigin(0, 0);

    m_oilOverlay.setSize(sf::Vector2f(1254, 900));
    m_oilOverlay.setFillColor(sf::Color(0, 0, 0, 0));

    m_camera.setSize(1254, 900);
    m_camera.setCenter(627, 450);

    resetLevel();

    std::cout << "Game initialized. Window size: 1254x900 with camera" << std::endl;
}

void Game::setupText(sf::Text& text, float x, float y, int size)
{
    text.setFont(m_font);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}

void Game::run()
{
    while (m_window.isOpen())
    {
        float dt = m_clock.restart().asSeconds();
        if (dt > 0.033f) dt = 0.033f;

        processEvents();
        update(dt);
        render();
    }
}

void Game::updateCamera()
{
    if (!m_player) return;

    sf::Vector2f targetPos = m_player->getPosition();

    float minX = 627;
    float maxX = 1254 - 627;
    float minY = 450;
    float maxY = 1254 - 450;

    targetPos.x = std::max(minX, std::min(maxX, targetPos.x));
    targetPos.y = std::max(minY, std::min(maxY, targetPos.y));

    sf::Vector2f currentCenter = m_camera.getCenter();
    sf::Vector2f newCenter = currentCenter + (targetPos - currentCenter) * m_cameraSmoothness;

    m_camera.setCenter(newCenter);
    m_window.setView(m_camera);
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        if (m_state == GameState::MENU)
        {
            Menu::MenuResult result = m_menu.handleClick(event, m_window);

            if (result == Menu::MenuResult::Play)
            {
                m_currentLevel = 1;
                m_state = GameState::PLAYING;
                m_isPlaying = true;
                resetLevel();
                std::cout << "Starting game at Level 1" << std::endl;
            }
            else if (result == Menu::MenuResult::Levels)
            {
                m_menu.setState(true);
                std::cout << "Entering level selection menu" << std::endl;
            }
            else if (result == Menu::MenuResult::Level1)
            {
                m_currentLevel = 1;
                m_state = GameState::PLAYING;
                m_isPlaying = true;
                resetLevel();
                m_menu.setState(false);
                std::cout << "Starting game at Level 1" << std::endl;
            }
            else if (result == Menu::MenuResult::Level2)
            {
                m_currentLevel = 2;
                m_state = GameState::PLAYING;
                m_isPlaying = true;
                resetLevel();
                m_menu.setState(false);
                std::cout << "Starting game at Level 2" << std::endl;
            }
            else if (result == Menu::MenuResult::Level3)
            {
                m_currentLevel = 3;
                m_state = GameState::PLAYING;
                m_isPlaying = true;
                resetLevel();
                m_menu.setState(false);
                std::cout << "Starting game at Level 3" << std::endl;
            }
            else if (result == Menu::MenuResult::Back)
            {
                m_menu.setState(false);
                std::cout << "Back to main menu" << std::endl;
            }
            else if (result == Menu::MenuResult::Exit)
            {
                m_window.close();
            }
        }
        else if (m_state == GameState::PLAYING)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    m_state = GameState::MENU;
                    m_menu.setState(false);
                    m_camera.setCenter(627, 450);
                    m_window.setView(m_camera);
                    std::cout << "Back to menu" << std::endl;
                }
                else if (event.key.code == sf::Keyboard::R)
                {
                    restartLevel();
                    std::cout << "Restarting current level!" << std::endl;
                }
            }
        }
        else if (m_state == GameState::VICTORY)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return)
                {
                    m_currentLevel++;
                    if (m_currentLevel <= 3)
                    {
                        resetLevel();
                        m_state = GameState::PLAYING;
                        m_isPlaying = true;
                        std::cout << "Next level: " << m_currentLevel << std::endl;
                    }
                    else
                    {
                        m_state = GameState::MENU;
                        m_menu.setState(false);
                        m_camera.setCenter(627, 450);
                        m_window.setView(m_camera);
                        std::cout << "Game completed! Back to menu" << std::endl;
                    }
                }
                else if (event.key.code == sf::Keyboard::R)
                {
                    restartLevel();
                }
            }
        }
        else if (m_state == GameState::GAME_OVER)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::Return)
                {
                    m_state = GameState::MENU;
                    m_menu.setState(false);
                    m_camera.setCenter(627, 450);
                    m_window.setView(m_camera);
                    std::cout << "Game over. Back to menu" << std::endl;
                }
                else if (event.key.code == sf::Keyboard::R)
                {
                    restartLevel();
                }
            }
        }
    }
}

void Game::update(float dt)
{
    if (m_state != GameState::PLAYING) return;
    if (!m_isPlaying) return;
    if (!m_player || !m_track) return;

    if (m_oilEffectTimer > 0)
    {
        m_oilEffectTimer -= dt;
        int alpha = static_cast<int>(100 * (m_oilEffectTimer / 2.0f));
        if (alpha < 0) alpha = 0;
        if (alpha > 100) alpha = 100;
        m_oilOverlay.setFillColor(sf::Color(200, 180, 50, static_cast<sf::Uint8>(alpha)));
    }
    else
    {
        m_oilOverlay.setFillColor(sf::Color(0, 0, 0, 0));
    }

    if (m_mudTimer > 0) m_mudTimer -= dt;
    if (m_shieldTimer > 0) { m_shieldTimer -= dt; m_hasShield = m_shieldTimer > 0; }
    if (m_speedBoostTimer > 0)
    {
        m_speedBoostTimer -= dt;
        if (m_player) m_player->setBoostActive(m_speedBoostTimer > 0);
    }

    bool oilActive = m_oilEffectTimer > 0;
    bool mudActive = m_mudTimer > 0;
    if (m_player) m_player->update(dt, oilActive, mudActive);

    if (m_player)
    {
        sf::Vector2f pos = m_player->getPosition();
        float carHalfWidth = m_player->getWidth() / 2.0f;
        float carHalfHeight = m_player->getHeight() / 2.0f;

        bool isOutOfBounds = false;

        if (pos.x - carHalfWidth < 0 ||
            pos.x + carHalfWidth > 1254 ||
            pos.y - carHalfHeight < 0 ||
            pos.y + carHalfHeight > 1254)
        {
            isOutOfBounds = true;
            std::cout << "Car drove off the track!" << std::endl;
        }

        if (isOutOfBounds && !m_hasShield)
        {
            m_state = GameState::GAME_OVER;
            m_isPlaying = false;
            std::cout << "Game over! Car left the track!" << std::endl;
            return;
        }

        if (!isOutOfBounds || m_hasShield)
        {
            pos.x = std::max(carHalfWidth, std::min(1254.0f - carHalfWidth, pos.x));
            pos.y = std::max(carHalfHeight, std::min(1254.0f - carHalfHeight, pos.y));
            m_player->setPosition(pos);
        }
    }

    updateCamera();

    if (m_track && m_player && m_track->checkFinish(m_player->getPosition()))
    {
        m_state = GameState::VICTORY;
        m_isPlaying = false;
        std::cout << "Victory! Time: " << static_cast<int>(m_timer) << " seconds" << std::endl;
        return;
    }

    m_timer += dt;
    updateBonuses(dt);
    checkCollisions();

    if (m_collisionCount >= 10)
    {
        m_state = GameState::GAME_OVER;
        m_isPlaying = false;
        std::cout << "Game over! Too many collisions: " << m_collisionCount << std::endl;
    }

    if (m_player)
    {
        m_timerText.setString("Time: " + std::to_string(static_cast<int>(m_timer)) + "s");
        int rawSpeed = m_player->getRawSpeed();
        int absSpeed = std::abs(rawSpeed);
        std::string direction = (rawSpeed >= 0) ? "FWD" : "REV";
        m_speedText.setString("Speed: " + std::to_string(absSpeed) + " km/h " + direction);
        m_levelText.setString("Level: " + std::to_string(m_currentLevel));
        m_collisionText.setString("Collisions: " + std::to_string(m_collisionCount) + " / 10");
        std::string bonusStr = "";
        if (m_hasShield) bonusStr += "SHIELD ";
        if (m_speedBoostTimer > 0) bonusStr += "BOOST ";
        m_bonusText.setString(bonusStr);
    }
}

void Game::restartLevel()
{
    m_player = std::make_unique<PlayerCar>(627, 1000, m_currentLevel);
    m_track = std::make_unique<Track>(m_currentLevel);
    m_obstacles.clear();
    m_bonuses.clear();
    m_timer = 0;
    m_collisionCount = 0;
    m_oilEffectTimer = 0;
    m_mudTimer = 0;
    m_hasShield = false;
    m_shieldTimer = 0;
    m_speedBoostTimer = 0;

    loadLevel(m_currentLevel);
    m_state = GameState::PLAYING;
    m_isPlaying = true;

    m_camera.setCenter(627, 450);
    m_window.setView(m_camera);

    std::cout << "=== LEVEL RESTARTED ===" << std::endl;
}

void Game::resetLevel()
{
    m_player = std::make_unique<PlayerCar>(627, 1000, m_currentLevel);
    m_track = std::make_unique<Track>(m_currentLevel);
    m_obstacles.clear();
    m_bonuses.clear();
    m_timer = 0;
    m_collisionCount = 0;
    m_oilEffectTimer = 0;
    m_mudTimer = 0;
    m_hasShield = false;
    m_shieldTimer = 0;
    m_speedBoostTimer = 0;

    loadLevel(m_currentLevel);

    m_camera.setCenter(627, 450);
    m_window.setView(m_camera);
}

void Game::loadLevel(int level)
{
    if (!m_player) return;

    switch (level)
    {
    case 1:
        m_player->setMaxSpeed(300);
        m_player->setTraction(0.98f);
        std::cout << "Level 1 loaded - Asphalt Start" << std::endl;
        break;

    case 2:
        m_player->setMaxSpeed(260);
        m_player->setTraction(0.94f);

        m_obstacles.clear();
        m_bonuses.clear();

        m_obstacles.push_back(Obstacle(400, 740, ObstacleType::PIT, true));
        m_obstacles.push_back(Obstacle(1000, 180, ObstacleType::PIT, true));
        m_obstacles.push_back(Obstacle(185, 850, ObstacleType::OIL, true));

        m_bonuses.push_back(Bonus(860, 180, BonusType::SPEED, true));
        m_bonuses.push_back(Bonus(780, 620, BonusType::SHIELD, true));

        std::cout << "Level 2 loaded - Dangerous Section (invisible objects)" << std::endl;
        std::cout << "Obstacles: " << m_obstacles.size() << ", Bonuses: " << m_bonuses.size() << std::endl;
        break;

    case 3:
        m_player->setMaxSpeed(220);
        m_player->setTraction(0.90f);

        m_bonuses.clear();
        m_bonuses.push_back(Bonus(420, 720, BonusType::SPEED, true));
        m_bonuses.push_back(Bonus(250, 200, BonusType::SHIELD, true));

        std::cout << "Level 3 loaded - Country Road (invisible bonuses)" << std::endl;
        break;
    }
}

void Game::checkCollisions()
{
    if (!m_player || !m_track) return;

    sf::FloatRect carBounds = m_player->getBounds();

    if (m_track->checkWallCollision(carBounds) && !m_hasShield)
    {
        m_collisionCount++;
        m_player->bounce();
        m_player->reduceSpeed(0.5f);
        std::cout << "Wall collision! Total: " << m_collisionCount << std::endl;
    }

    for (auto& obstacle : m_obstacles)
    {
        if (!obstacle.isActive()) continue;

        if (carBounds.intersects(obstacle.getBounds()))
        {
            if (m_hasShield) continue;

            switch (obstacle.getType())
            {
            case ObstacleType::PIT:
                std::cout << "Hit a pit! GAME OVER!" << std::endl;
                m_state = GameState::GAME_OVER;
                m_isPlaying = false;
                return;

            case ObstacleType::OIL:
                m_oilEffectTimer = 2.0f;
                std::cout << "Oil spill! Loss of control for 2 seconds" << std::endl;
                break;

            case ObstacleType::MUD:
                m_player->reduceSpeed(0.6f);
                m_mudTimer = 1.0f;
                std::cout << "Mud! Slowing down" << std::endl;
                break;
            }

            obstacle.deactivate();
        }
    }
}

void Game::updateBonuses(float dt)
{
    if (!m_player) return;

    sf::FloatRect carBounds = m_player->getBounds();

    for (auto& bonus : m_bonuses)
    {
        if (!bonus.isActive()) continue;

        if (carBounds.intersects(bonus.getBounds()))
        {
            switch (bonus.getType())
            {
            case BonusType::SPEED:
                m_speedBoostTimer = 3.0f;
                if (m_player) m_player->setBoostActive(true);
                std::cout << "Speed boost activated!" << std::endl;
                break;

            case BonusType::SHIELD:
                m_shieldTimer = 5.0f;
                m_hasShield = true;
                std::cout << "Shield activated!" << std::endl;
                break;
            }

            bonus.deactivate();
        }
    }
}

void Game::render()
{
    m_window.clear(sf::Color(30, 30, 40));

    if (m_state == GameState::MENU)
    {
        m_window.setView(m_window.getDefaultView());
        m_menu.draw(m_window);
    }
    else
    {
        if (m_track) m_track->draw(m_window);

        for (auto& obstacle : m_obstacles)
            if (obstacle.isActive()) obstacle.draw(m_window);
        for (auto& bonus : m_bonuses)
            if (bonus.isActive()) bonus.draw(m_window);
        if (m_player) m_player->draw(m_window);

        m_window.setView(m_window.getDefaultView());

        m_window.draw(m_timerText);
        m_window.draw(m_speedText);
        m_window.draw(m_bonusText);
        m_window.draw(m_levelText);
        m_window.draw(m_collisionText);
        m_window.draw(m_oilOverlay);

        if (m_state == GameState::VICTORY)
        {
            sf::RectangleShape overlay(sf::Vector2f(1254, 900));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            m_window.draw(overlay);

            std::string victoryMsg = "VICTORY!\nTime: " + std::to_string(static_cast<int>(m_timer)) + "s\nPress any key to continue\nPress R to restart";
            m_messageText.setString(victoryMsg);
            sf::FloatRect textBounds = m_messageText.getLocalBounds();
            m_messageText.setPosition(627 - textBounds.width / 2, 400);
            m_window.draw(m_messageText);
        }
        else if (m_state == GameState::GAME_OVER)
        {
            sf::RectangleShape overlay(sf::Vector2f(1254, 900));
            overlay.setFillColor(sf::Color(0, 0, 0, 200));
            m_window.draw(overlay);

            m_messageText.setString("GAME OVER!\nToo many collisions or left the track!\nPress SPACE for menu\nPress R to restart");
            sf::FloatRect textBounds = m_messageText.getLocalBounds();
            m_messageText.setPosition(627 - textBounds.width / 2, 400);
            m_window.draw(m_messageText);
        }

        if (m_player)
        {
            m_window.setView(m_camera);
        }
    }

    m_window.display();
}