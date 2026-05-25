#include "Bonus.h"

Bonus::Bonus(float x, float y, BonusType type)
    : Bonus(x, y, type, false)
{
}

Bonus::Bonus(float x, float y, BonusType type, bool invisible)
    : m_type(type), m_active(true), m_invisible(invisible)
{
    m_shape.setRadius(15);
    m_shape.setPosition(x - 15, y - 15);

    if (type == BonusType::SPEED)
        m_shape.setFillColor(sf::Color(0, 200, 255));
    else
        m_shape.setFillColor(sf::Color(255, 200, 0));

    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOutlineThickness(2);
}

void Bonus::draw(sf::RenderWindow& window)
{
    if (m_active && !m_invisible)
        window.draw(m_shape);
}

sf::FloatRect Bonus::getBounds() const
{
    return m_shape.getGlobalBounds();
}

BonusType Bonus::getType() const
{
    return m_type;
}

bool Bonus::isActive() const
{
    return m_active;
}

void Bonus::deactivate()
{
    m_active = false;
}