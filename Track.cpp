#include "Track.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

Track::Track(int level) : m_level(level)
{
    std::cout << "\n========== TRACK INIT ==========" << std::endl;
    std::cout << "Level: " << level << std::endl;

    std::string bgFile;
    if (level == 1)
        bgFile = "asphalt_track.png";
    else if (level == 2)
        bgFile = "asphalt_track2.png";
    else
        bgFile = "mud_track.png";

    std::cout << "Background file: " << bgFile << std::endl;

    std::vector<std::string> bgPaths;
    bgPaths.push_back(bgFile);
    bgPaths.push_back("repos/" + bgFile);
    bgPaths.push_back("../repos/" + bgFile);

    bool bgLoaded = false;
    for (size_t i = 0; i < bgPaths.size(); i++)
    {
        std::ifstream test(bgPaths[i].c_str());
        if (test.good())
        {
            if (m_backgroundTexture.loadFromFile(bgPaths[i]))
            {
                std::cout << "Background loaded: " << bgPaths[i] << std::endl;
                bgLoaded = true;
                break;
            }
        }
    }

    if (!bgLoaded)
    {
        std::cout << "Background not found: " << bgFile << std::endl;
        sf::Image img;
        img.create(1254, 1254, sf::Color::Magenta);
        m_backgroundTexture.loadFromImage(img);
    }

    m_backgroundSprite.setTexture(m_backgroundTexture);

    std::string maskFile = "asphalt_mask.png";
    std::cout << "Loading mask: " << maskFile << std::endl;

    std::vector<std::string> maskPaths;
    maskPaths.push_back(maskFile);
    maskPaths.push_back("repos/" + maskFile);
    maskPaths.push_back("../repos/" + maskFile);
    maskPaths.push_back("res/" + maskFile);

    bool maskLoaded = false;
    for (size_t i = 0; i < maskPaths.size(); i++)
    {
        std::ifstream test(maskPaths[i].c_str());
        if (test.good())
        {
            std::cout << "Found mask at: " << maskPaths[i] << std::endl;
            if (m_maskImage.loadFromFile(maskPaths[i]))
            {
                std::cout << "Mask loaded successfully: " << maskPaths[i] << std::endl;
                std::cout << "Mask size: " << m_maskImage.getSize().x << " x " << m_maskImage.getSize().y << std::endl;
                maskLoaded = true;
                break;
            }
        }
    }

    if (!maskLoaded)
    {
        std::cerr << "ERROR: Mask not found! Creating emergency mask..." << std::endl;
        m_maskImage.create(1254, 1254, sf::Color::White);

        for (int x = 0; x < 1254; x++)
        {
            for (int y = 0; y < 1254; y++)
            {
                if (x < 50 || x > 1204 || y < 50 || y > 1204)
                {
                    m_maskImage.setPixel(x, y, sf::Color::Black);
                }
            }
        }

        for (int x = 560; x < 640; x++)
        {
            for (int y = 1140; y < 1180; y++)
            {
                m_maskImage.setPixel(x, y, sf::Color::Green);
            }
        }
    }

    sf::Color testPixel = m_maskImage.getPixel(627, 627);
    std::cout << "Mask test - center pixel (627,627): R=" << (int)testPixel.r
        << " G=" << (int)testPixel.g << " B=" << (int)testPixel.b << std::endl;

    std::cout << "Track ready for level " << level << std::endl;
    std::cout << "========================================\n" << std::endl;
}

sf::Color Track::getPixelColor(float x, float y)
{
    int ix = (int)x;
    int iy = (int)y;

    if (ix < 0 || ix >= 1254 || iy < 0 || iy >= 1254)
        return sf::Color::Black;

    return m_maskImage.getPixel(ix, iy);
}

bool Track::checkFinish(const sf::Vector2f& position)
{
    for (int dx = -25; dx <= 25; dx += 10)
    {
        for (int dy = -25; dy <= 25; dy += 10)
        {
            sf::Color c = getPixelColor(position.x + dx, position.y + dy);
            if (c.g > 200 && c.r < 100 && c.b < 100)
            {
                std::cout << "FINISH REACHED on level " << m_level << "!" << std::endl;
                return true;
            }
        }
    }
    return false;
}

bool Track::checkWallCollision(const sf::FloatRect& carBounds)
{
    float centerX = carBounds.left + carBounds.width / 2;
    float centerY = carBounds.top + carBounds.height / 2;

    float radius = 38.0f;

    for (int angle = 0; angle < 360; angle += 20)
    {
        float rad = angle * 3.14159f / 180.0f;
        float checkX = centerX + cos(rad) * radius;
        float checkY = centerY + sin(rad) * radius;

        sf::Color c = getPixelColor(checkX, checkY);

        if (c.r < 50 && c.g < 50 && c.b < 50)
        {
            return true;
        }
    }

    sf::Color centerColor = getPixelColor(centerX, centerY);
    if (centerColor.r < 50 && centerColor.g < 50 && centerColor.b < 50)
    {
        return true;
    }

    return false;
}

void Track::draw(sf::RenderWindow& window)
{
    window.draw(m_backgroundSprite);
}