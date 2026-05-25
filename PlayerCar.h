#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

class PlayerCar
{
public:
    PlayerCar(float startX, float startY, int level);

    void update(float dt, bool oilEffect, bool mudEffect = false);
    void draw(sf::RenderWindow& window);

    void setPosition(const sf::Vector2f& pos);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;

    float getSpeed() const;
    int getRawSpeed() const;
    void reduceSpeed(float factor);
    void bounce();
    void setBoostActive(bool active);

    void setMaxSpeed(float max);
    void setTraction(float t);

    float getWidth() const { return 90.0f; }
    float getHeight() const { return 90.0f; }

private:
    sf::Sprite m_sprite;
    sf::Texture m_texture;
    sf::Vector2f m_velocity;
    float m_speed;
    float m_direction;
    float m_angle;
    float m_maxSpeed;
    float m_backSpeed;
    float m_traction;
    float m_normalMaxSpeed;
    bool m_boostActive;
    float m_mudTimer;
};