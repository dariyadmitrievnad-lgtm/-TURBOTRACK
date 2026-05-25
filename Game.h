#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "PlayerCar.h"
#include "Track.h"
#include "Obstacle.h"
#include "Bonus.h"
#include "Menu.h"

enum class GameState { MENU, PLAYING, GAME_OVER, VICTORY };

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void resetLevel();
    void loadLevel(int level);
    void checkCollisions();
    void updateBonuses(float dt);
    void setupText(sf::Text& text, float x, float y, int size);
    bool loadFont();
    void restartLevel();
    void updateCamera();

    sf::RenderWindow m_window;
    sf::Clock m_clock;
    GameState m_state;
    Menu m_menu;

    std::unique_ptr<PlayerCar> m_player;
    std::unique_ptr<Track> m_track;
    std::vector<Obstacle> m_obstacles;
    std::vector<Bonus> m_bonuses;

    int m_currentLevel;
    float m_timer;
    bool m_isPlaying;
    int m_collisionCount;

    sf::Font m_font;
    sf::Text m_timerText;
    sf::Text m_speedText;
    sf::Text m_bonusText;
    sf::Text m_levelText;
    sf::Text m_collisionText;
    sf::Text m_messageText;

    sf::RectangleShape m_oilOverlay;
    float m_oilEffectTimer;
    float m_mudTimer;
    bool m_hasShield;
    float m_shieldTimer;
    float m_speedBoostTimer;

    sf::View m_camera;
    float m_cameraSmoothness;
};