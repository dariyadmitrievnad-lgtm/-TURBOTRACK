#pragma once
#include <SFML/Graphics.hpp>

class Menu
{
public:
    enum class MenuResult { Nothing, Play, Levels, Level1, Level2, Level3, Exit, Back };

    Menu(float width, float height);
    MenuResult handleClick(sf::Event event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    void setState(bool inLevelMenu);

private:
    struct Button
    {
        sf::Sprite sprite;
        sf::RectangleShape shape;
        sf::Text text;
        sf::FloatRect bounds;
        MenuResult result;
        bool useTexture;
    };

    void createButtons();
    bool loadFont();
    bool loadBackground();
    bool loadLevelsBackground();

    sf::Font m_font;

    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;
    sf::Texture m_levelsBackgroundTexture;
    sf::Sprite m_levelsBackgroundSprite;

    sf::Texture m_playTexture;
    sf::Texture m_levelsTexture;
    sf::Texture m_exitTexture;

    sf::Texture m_level1Texture;
    sf::Texture m_level2Texture;
    sf::Texture m_level3Texture;
    sf::Texture m_backTexture;

    std::vector<Button> m_buttons;
    std::vector<Button> m_levelButtons;
    Button m_backButton;

    bool m_inLevelMenu;
    float m_width;
    float m_height;
};