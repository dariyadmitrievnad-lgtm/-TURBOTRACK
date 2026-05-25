#include "Obstacle.h"

Obstacle::Obstacle(float x, float y, ObstacleType type)
    : Obstacle(x, y, type, false)
{
}

Obstacle::Obstacle(float x, float y, ObstacleType type, bool invisible)
    : m_type(type), m_active(true), m_invisible(invisible)
{
    m_shape.setSize(sf::Vector2f(30, 30));
    m_shape.setPosition(x - 15, y - 15);

    switch (type)
    {
    case ObstacleType::PIT:
        m_shape.setFillColor(sf::Color(50, 50, 50));
        break;
    case ObstacleType::OIL:
        m_shape.setFillColor(sf::Color(180, 160, 50));
        break;
    case ObstacleType::MUD:
        m_shape.setFillColor(sf::Color(101, 67, 33));
        break;
    }

    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(2);
}

void Obstacle::draw(sf::RenderWindow& window)
{
    if (m_active && !m_invisible)
        window.draw(m_shape);
}

sf::FloatRect Obstacle::getBounds() const
{
    return m_shape.getGlobalBounds();
}

ObstacleType Obstacle::getType() const
{
    return m_type;
}

bool Obstacle::isActive() const
{
    return m_active;
}

void Obstacle::deactivate()
{
    m_active = false;
}