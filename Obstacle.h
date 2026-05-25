#pragma once
#include <SFML/Graphics.hpp>

enum class ObstacleType { PIT, OIL, MUD };

class Obstacle
{
public:
    Obstacle(float x, float y, ObstacleType type);
    Obstacle(float x, float y, ObstacleType type, bool invisible);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    ObstacleType getType() const;
    bool isActive() const;
    void deactivate();

private:
    sf::RectangleShape m_shape;
    ObstacleType m_type;
    bool m_active;
    bool m_invisible;
};