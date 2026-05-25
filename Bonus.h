#pragma once
#include <SFML/Graphics.hpp>

enum class BonusType { SPEED, SHIELD };

class Bonus
{
public:
    Bonus(float x, float y, BonusType type);
    Bonus(float x, float y, BonusType type, bool invisible);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    BonusType getType() const;
    bool isActive() const;
    void deactivate();

private:
    sf::CircleShape m_shape;
    BonusType m_type;
    bool m_active;
    bool m_invisible;
};