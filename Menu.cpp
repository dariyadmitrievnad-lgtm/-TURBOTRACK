#include "Menu.h"
#include <iostream>
#include <fstream>

bool Menu::loadFont()
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
                std::cout << "Menu font loaded: " << path << std::endl;
                return true;
            }
        }
    }

    std::cout << "No font found for menu!" << std::endl;
    return false;
}

bool Menu::loadBackground()
{
    std::vector<std::string> bgPaths = {
        "menu_background.png",
        "repos/menu_background.png",
        "../repos/menu_background.png",
        "res/menu_background.png"
    };

    for (const auto& path : bgPaths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_backgroundTexture.loadFromFile(path))
            {
                std::cout << "Menu background loaded: " << path << std::endl;
                m_backgroundSprite.setTexture(m_backgroundTexture);

                sf::Vector2u textureSize = m_backgroundTexture.getSize();
                float scaleX = m_width / (float)textureSize.x;
                float scaleY = m_height / (float)textureSize.y;
                m_backgroundSprite.setScale(scaleX, scaleY);
                return true;
            }
        }
    }

    std::cout << "Menu background not found! Using colored background." << std::endl;
    return false;
}

bool Menu::loadLevelsBackground()
{
    std::vector<std::string> bgPaths = {
        "choose_level.png",
        "repos/choose_level.png",
        "../repos/choose_level.png",
        "res/choose_level.png"
    };

    for (const auto& path : bgPaths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_levelsBackgroundTexture.loadFromFile(path))
            {
                std::cout << "Levels background loaded: " << path << std::endl;
                m_levelsBackgroundSprite.setTexture(m_levelsBackgroundTexture);

                sf::Vector2u textureSize = m_levelsBackgroundTexture.getSize();
                float scaleX = m_width / (float)textureSize.x;
                float scaleY = m_height / (float)textureSize.y;
                m_levelsBackgroundSprite.setScale(scaleX, scaleY);
                return true;
            }
        }
    }

    std::cout << "Levels background not found! Using colored background." << std::endl;
    return false;
}

Menu::Menu(float width, float height) : m_width(width), m_height(height), m_inLevelMenu(false)
{
    loadFont();
    loadBackground();
    loadLevelsBackground();
    createButtons();
}

void Menu::createButtons()
{

    float btnWidth = 380;
    float btnHeight = 140;
    float btnX = m_width / 2 - btnWidth / 2;

    float mainSpacing = 115;
    float startY = 330;  

    Button playBtn;
    playBtn.useTexture = true;
    std::vector<std::string> playPaths = {
        "play_button.png",
        "repos/play_button.png",
        "../repos/play_button.png",
        "res/play_button.png"
    };

    bool playLoaded = false;
    for (const auto& path : playPaths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_playTexture.loadFromFile(path))
            {
                std::cout << "Play button texture loaded: " << path << std::endl;
                playBtn.sprite.setTexture(m_playTexture);
                playLoaded = true;
                break;
            }
        }
    }

    if (playLoaded)
    {
        sf::Vector2u texSize = m_playTexture.getSize();
        float scaleX = btnWidth / (float)texSize.x;
        float scaleY = btnHeight / (float)texSize.y;
        playBtn.sprite.setScale(scaleX, scaleY);
        playBtn.sprite.setPosition(btnX, startY);
        playBtn.bounds = playBtn.sprite.getGlobalBounds();
        playBtn.useTexture = true;
    }
    else
    {
        playBtn.shape.setSize(sf::Vector2f(btnWidth, btnHeight));
        playBtn.shape.setFillColor(sf::Color(0, 180, 0));
        playBtn.shape.setOutlineColor(sf::Color::White);
        playBtn.shape.setOutlineThickness(4);
        playBtn.shape.setPosition(btnX, startY);
        playBtn.bounds = playBtn.shape.getGlobalBounds();
        playBtn.text.setFont(m_font);
        playBtn.text.setString("PLAY");
        playBtn.text.setCharacterSize(48);
        playBtn.text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = playBtn.text.getLocalBounds();
        playBtn.text.setPosition(btnX + (btnWidth - textBounds.width) / 2, startY + (btnHeight - textBounds.height) / 2 - 5);
        playBtn.useTexture = false;
    }
    playBtn.result = MenuResult::Play;
    m_buttons.push_back(playBtn);

    Button levelsBtn;
    levelsBtn.useTexture = true;
    std::vector<std::string> levelsPaths = {
        "levels_button.png",
        "repos/levels_button.png",
        "../repos/levels_button.png",
        "res/levels_button.png"
    };

    bool levelsLoaded = false;
    for (const auto& path : levelsPaths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_levelsTexture.loadFromFile(path))
            {
                std::cout << "Levels button texture loaded: " << path << std::endl;
                levelsBtn.sprite.setTexture(m_levelsTexture);
                levelsLoaded = true;
                break;
            }
        }
    }

    if (levelsLoaded)
    {
        sf::Vector2u texSize = m_levelsTexture.getSize();
        float scaleX = btnWidth / (float)texSize.x;
        float scaleY = btnHeight / (float)texSize.y;
        levelsBtn.sprite.setScale(scaleX, scaleY);
        levelsBtn.sprite.setPosition(btnX, startY + mainSpacing);
        levelsBtn.bounds = levelsBtn.sprite.getGlobalBounds();
        levelsBtn.useTexture = true;
    }
    else
    {
        levelsBtn.shape.setSize(sf::Vector2f(btnWidth, btnHeight));
        levelsBtn.shape.setFillColor(sf::Color(0, 100, 180));
        levelsBtn.shape.setOutlineColor(sf::Color::White);
        levelsBtn.shape.setOutlineThickness(4);
        levelsBtn.shape.setPosition(btnX, startY + mainSpacing);
        levelsBtn.bounds = levelsBtn.shape.getGlobalBounds();
        levelsBtn.text.setFont(m_font);
        levelsBtn.text.setString("LEVELS");
        levelsBtn.text.setCharacterSize(48);
        levelsBtn.text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = levelsBtn.text.getLocalBounds();
        levelsBtn.text.setPosition(btnX + (btnWidth - textBounds.width) / 2, startY + mainSpacing + (btnHeight - textBounds.height) / 2 - 5);
        levelsBtn.useTexture = false;
    }
    levelsBtn.result = MenuResult::Levels;
    m_buttons.push_back(levelsBtn);

    Button exitBtn;
    exitBtn.useTexture = true;
    std::vector<std::string> exitPaths = {
        "exit_button.png",
        "repos/exit_button.png",
        "../repos/exit_button.png",
        "res/exit_button.png"
    };

    bool exitLoaded = false;
    for (const auto& path : exitPaths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_exitTexture.loadFromFile(path))
            {
                std::cout << "Exit button texture loaded: " << path << std::endl;
                exitBtn.sprite.setTexture(m_exitTexture);
                exitLoaded = true;
                break;
            }
        }
    }

    if (exitLoaded)
    {
        sf::Vector2u texSize = m_exitTexture.getSize();
        float scaleX = btnWidth / (float)texSize.x;
        float scaleY = btnHeight / (float)texSize.y;
        exitBtn.sprite.setScale(scaleX, scaleY);
        exitBtn.sprite.setPosition(btnX, startY + mainSpacing * 2);
        exitBtn.bounds = exitBtn.sprite.getGlobalBounds();
        exitBtn.useTexture = true;
    }
    else
    {
        exitBtn.shape.setSize(sf::Vector2f(btnWidth, btnHeight));
        exitBtn.shape.setFillColor(sf::Color(180, 0, 0));
        exitBtn.shape.setOutlineColor(sf::Color::White);
        exitBtn.shape.setOutlineThickness(4);
        exitBtn.shape.setPosition(btnX, startY + mainSpacing * 2);
        exitBtn.bounds = exitBtn.shape.getGlobalBounds();
        exitBtn.text.setFont(m_font);
        exitBtn.text.setString("EXIT");
        exitBtn.text.setCharacterSize(48);
        exitBtn.text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = exitBtn.text.getLocalBounds();
        exitBtn.text.setPosition(btnX + (btnWidth - textBounds.width) / 2, startY + mainSpacing * 2 + (btnHeight - textBounds.height) / 2 - 5);
        exitBtn.useTexture = false;
    }
    exitBtn.result = MenuResult::Exit;
    m_buttons.push_back(exitBtn);

    float levelBtnWidth = 450;     
    float levelBtnHeight = 170;     
    float levelBtnX = m_width / 2 - levelBtnWidth / 2;

    float levelStartY = 240;      

    float levelSpacing = 125;       

    int levelFontSize = 40;       

    Button level1Btn;
    level1Btn.useTexture = true;
    std::vector<std::string> level1Paths = {
        "level1_asphalt.png",
        "repos/level1_asphalt.png",
        "../repos/level1_asphalt.png",
        "res/level1_asphalt.png"
    };

    bool level1Loaded = false;
    for (const auto& path : level1Paths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_level1Texture.loadFromFile(path))
            {
                std::cout << "Level 1 button texture loaded: " << path << std::endl;
                level1Btn.sprite.setTexture(m_level1Texture);
                level1Loaded = true;
                break;
            }
        }
    }

    if (level1Loaded)
    {
        sf::Vector2u texSize = m_level1Texture.getSize();
        float scaleX = levelBtnWidth / (float)texSize.x;
        float scaleY = levelBtnHeight / (float)texSize.y;
        level1Btn.sprite.setScale(scaleX, scaleY);
        level1Btn.sprite.setPosition(levelBtnX, levelStartY);
        level1Btn.bounds = level1Btn.sprite.getGlobalBounds();
        level1Btn.useTexture = true;
    }
    else
    {
        level1Btn.shape.setSize(sf::Vector2f(levelBtnWidth, levelBtnHeight));
        level1Btn.shape.setFillColor(sf::Color(0, 150, 0));
        level1Btn.shape.setOutlineColor(sf::Color::White);
        level1Btn.shape.setOutlineThickness(4);
        level1Btn.shape.setPosition(levelBtnX, levelStartY);
        level1Btn.bounds = level1Btn.shape.getGlobalBounds();
        level1Btn.text.setFont(m_font);
        level1Btn.text.setString("LEVEL 1 - ASPHALT");
        level1Btn.text.setCharacterSize(levelFontSize);
        level1Btn.text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = level1Btn.text.getLocalBounds();
        level1Btn.text.setPosition(levelBtnX + (levelBtnWidth - textBounds.width) / 2, levelStartY + (levelBtnHeight - textBounds.height) / 2 - 5);
        level1Btn.useTexture = false;
    }
    level1Btn.result = MenuResult::Level1;
    m_levelButtons.push_back(level1Btn);

    Button level2Btn;
    level2Btn.useTexture = true;
    std::vector<std::string> level2Paths = {
        "level2_dangerous.png",
        "repos/level2_dangerous.png",
        "../repos/level2_dangerous.png",
        "res/level2_dangerous.png"
    };

    bool level2Loaded = false;
    for (const auto& path : level2Paths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_level2Texture.loadFromFile(path))
            {
                std::cout << "Level 2 button texture loaded: " << path << std::endl;
                level2Btn.sprite.setTexture(m_level2Texture);
                level2Loaded = true;
                break;
            }
        }
    }

    if (level2Loaded)
    {
        sf::Vector2u texSize = m_level2Texture.getSize();
        float scaleX = levelBtnWidth / (float)texSize.x;
        float scaleY = levelBtnHeight / (float)texSize.y;
        level2Btn.sprite.setScale(scaleX, scaleY);
        level2Btn.sprite.setPosition(levelBtnX, levelStartY + levelSpacing);
        level2Btn.bounds = level2Btn.sprite.getGlobalBounds();
        level2Btn.useTexture = true;
    }
    else
    {
        level2Btn.shape.setSize(sf::Vector2f(levelBtnWidth, levelBtnHeight));
        level2Btn.shape.setFillColor(sf::Color(200, 150, 0));
        level2Btn.shape.setOutlineColor(sf::Color::White);
        level2Btn.shape.setOutlineThickness(4);
        level2Btn.shape.setPosition(levelBtnX, levelStartY + levelSpacing);
        level2Btn.bounds = level2Btn.shape.getGlobalBounds();
        level2Btn.text.setFont(m_font);
        level2Btn.text.setString("LEVEL 2 - DANGEROUS");
        level2Btn.text.setCharacterSize(levelFontSize);
        level2Btn.text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = level2Btn.text.getLocalBounds();
        level2Btn.text.setPosition(levelBtnX + (levelBtnWidth - textBounds.width) / 2, levelStartY + levelSpacing + (levelBtnHeight - textBounds.height) / 2 - 5);
        level2Btn.useTexture = false;
    }
    level2Btn.result = MenuResult::Level2;
    m_levelButtons.push_back(level2Btn);

    Button level3Btn;
    level3Btn.useTexture = true;
    std::vector<std::string> level3Paths = {
        "level3_mud.png",
        "repos/level3_mud.png",
        "../repos/level3_mud.png",
        "res/level3_mud.png"
    };

    bool level3Loaded = false;
    for (const auto& path : level3Paths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_level3Texture.loadFromFile(path))
            {
                std::cout << "Level 3 button texture loaded: " << path << std::endl;
                level3Btn.sprite.setTexture(m_level3Texture);
                level3Loaded = true;
                break;
            }
        }
    }

    if (level3Loaded)
    {
        sf::Vector2u texSize = m_level3Texture.getSize();
        float scaleX = levelBtnWidth / (float)texSize.x;
        float scaleY = levelBtnHeight / (float)texSize.y;
        level3Btn.sprite.setScale(scaleX, scaleY);
        level3Btn.sprite.setPosition(levelBtnX, levelStartY + levelSpacing * 2);
        level3Btn.bounds = level3Btn.sprite.getGlobalBounds();
        level3Btn.useTexture = true;
    }
    else
    {
        level3Btn.shape.setSize(sf::Vector2f(levelBtnWidth, levelBtnHeight));
        level3Btn.shape.setFillColor(sf::Color(150, 0, 0));
        level3Btn.shape.setOutlineColor(sf::Color::White);
        level3Btn.shape.setOutlineThickness(4);
        level3Btn.shape.setPosition(levelBtnX, levelStartY + levelSpacing * 2);
        level3Btn.bounds = level3Btn.shape.getGlobalBounds();
        level3Btn.text.setFont(m_font);
        level3Btn.text.setString("LEVEL 3 - MUD");
        level3Btn.text.setCharacterSize(levelFontSize);
        level3Btn.text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = level3Btn.text.getLocalBounds();
        level3Btn.text.setPosition(levelBtnX + (levelBtnWidth - textBounds.width) / 2, levelStartY + levelSpacing * 2 + (levelBtnHeight - textBounds.height) / 2 - 5);
        level3Btn.useTexture = false;
    }
    level3Btn.result = MenuResult::Level3;
    m_levelButtons.push_back(level3Btn);

    Button backBtn;
    backBtn.useTexture = true;
    std::vector<std::string> backPaths = {
        "back_button.png",
        "repos/back_button.png",
        "../repos/back_button.png",
        "res/back_button.png"
    };

    bool backLoaded = false;
    for (const auto& path : backPaths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_backTexture.loadFromFile(path))
            {
                std::cout << "Back button texture loaded: " << path << std::endl;
                backBtn.sprite.setTexture(m_backTexture);
                backLoaded = true;
                break;
            }
        }
    }

    float backWidth = 240;
    float backHeight = 80;

    if (backLoaded)
    {
        sf::Vector2u texSize = m_backTexture.getSize();
        float scaleX = backWidth / (float)texSize.x;
        float scaleY = backHeight / (float)texSize.y;
        backBtn.sprite.setScale(scaleX, scaleY);
        backBtn.sprite.setPosition(40, m_height - 110);
        backBtn.bounds = backBtn.sprite.getGlobalBounds();
        backBtn.useTexture = true;
    }
    else
    {
        backBtn.shape.setSize(sf::Vector2f(backWidth, backHeight));
        backBtn.shape.setFillColor(sf::Color(100, 100, 100));
        backBtn.shape.setOutlineColor(sf::Color::White);
        backBtn.shape.setOutlineThickness(4);
        backBtn.shape.setPosition(40, m_height - 110);
        backBtn.bounds = backBtn.shape.getGlobalBounds();
        backBtn.text.setFont(m_font);
        backBtn.text.setString("BACK");
        backBtn.text.setCharacterSize(40);
        backBtn.text.setFillColor(sf::Color::White);
        sf::FloatRect textBounds = backBtn.text.getLocalBounds();
        backBtn.text.setPosition(40 + (backWidth - textBounds.width) / 2, m_height - 110 + (backHeight - textBounds.height) / 2 - 5);
        backBtn.useTexture = false;
    }
    backBtn.result = MenuResult::Back;
    m_backButton = backBtn;
}

Menu::MenuResult Menu::handleClick(sf::Event event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mouse(static_cast<float>(sf::Mouse::getPosition(window).x),
            static_cast<float>(sf::Mouse::getPosition(window).y));

        if (!m_inLevelMenu)
        {
            for (const auto& btn : m_buttons)
            {
                if (btn.bounds.contains(mouse))
                    return btn.result;
            }
        }
        else
        {
            for (const auto& btn : m_levelButtons)
            {
                if (btn.bounds.contains(mouse))
                    return btn.result;
            }
            if (m_backButton.bounds.contains(mouse))
                return MenuResult::Back;
        }
    }
    return MenuResult::Nothing;
}

void Menu::draw(sf::RenderWindow& window)
{
    if (!m_inLevelMenu)
    {
        if (m_backgroundTexture.getSize().x > 0)
        {
            window.draw(m_backgroundSprite);
        }
        else
        {
            sf::RectangleShape bg(sf::Vector2f(m_width, m_height));
            bg.setFillColor(sf::Color(20, 30, 55));
            window.draw(bg);
        }

        for (const auto& btn : m_buttons)
        {
            if (btn.useTexture)
            {
                window.draw(btn.sprite);
            }
            else
            {
                window.draw(btn.shape);
                window.draw(btn.text);
            }
        }
    }
    else
    {
        if (m_levelsBackgroundTexture.getSize().x > 0)
        {
            window.draw(m_levelsBackgroundSprite);
        }
        else
        {
            sf::RectangleShape bg(sf::Vector2f(m_width, m_height));
            bg.setFillColor(sf::Color(30, 40, 65));
            window.draw(bg);
        }

        for (const auto& btn : m_levelButtons)
        {
            if (btn.useTexture)
            {
                window.draw(btn.sprite);
            }
            else
            {
                window.draw(btn.shape);
                window.draw(btn.text);
            }
        }

        if (m_backButton.useTexture)
        {
            window.draw(m_backButton.sprite);
        }
        else
        {
            window.draw(m_backButton.shape);
            window.draw(m_backButton.text);
        }
    }
}

void Menu::setState(bool inLevelMenu)
{
    m_inLevelMenu = inLevelMenu;
}