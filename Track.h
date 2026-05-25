#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Track
{
public:
    Track(int level);
    void draw(sf::RenderWindow& window);
    bool checkFinish(const sf::Vector2f& position);
    bool checkWallCollision(const sf::FloatRect& carBounds);

private:
    sf::Color getPixelColor(float x, float y);

    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;
    sf::Image m_maskImage;
    int m_level;
};