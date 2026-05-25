#include "PlayerCar.h"
#include <cmath>
#include <iostream>
#include <algorithm>

PlayerCar::PlayerCar(float startX, float startY, int level)
    : m_speed(0)
    , m_direction(-1.57f)
    , m_angle(-90)
    , m_maxSpeed(400)
    , m_backSpeed(150)
    , m_traction(0.98f)
    , m_boostActive(false)
    , m_mudTimer(0)
{
    std::vector<std::string> carPaths = {
        "car.png",
        "repos/car.png",
        "../repos/car.png",
        "res/car.png"
    };

    bool textureLoaded = false;
    for (const auto& path : carPaths)
    {
        std::ifstream file(path.c_str());
        if (file.good())
        {
            if (m_texture.loadFromFile(path))
            {
                std::cout << "Car texture loaded: " << path << std::endl;
                textureLoaded = true;
                break;
            }
        }
    }

    if (!textureLoaded)
    {
        std::cout << "Car texture not found, using red rectangle" << std::endl;
        sf::Image img;
        img.create(90, 90, sf::Color(220, 50, 50));
        m_texture.loadFromImage(img);
    }

    m_sprite.setTexture(m_texture);

    sf::Vector2u textureSize = m_texture.getSize();
    float scaleX = 90.0f / (float)textureSize.x;
    float scaleY = 90.0f / (float)textureSize.y;
    m_sprite.setScale(scaleX, scaleY);

    m_sprite.setOrigin(45, 45);
    m_sprite.setPosition(startX, startY);
    m_normalMaxSpeed = m_maxSpeed;
    m_velocity = sf::Vector2f(0, 0);
}

void PlayerCar::update(float dt, bool oilEffect, bool mudEffect)
{
    float turnSpeed = 5.5f;
    float accelerationMultiplier = 1.0f;

    if (oilEffect) turnSpeed = 1.0f;

    if (mudEffect)
    {
        turnSpeed = 2.5f;
        accelerationMultiplier = 0.5f;
        m_mudTimer = 0.5f;
    }

    if (m_mudTimer > 0)
    {
        m_mudTimer -= dt;
        if (accelerationMultiplier == 1.0f) accelerationMultiplier = 0.7f;
    }

    float speedFactor = std::max(0.3f, std::min(1.0f, std::abs(m_speed) / m_maxSpeed));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        m_speed += 150 * dt * accelerationMultiplier;
        if (m_speed > m_maxSpeed) m_speed = m_maxSpeed;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_speed -= 100 * dt * accelerationMultiplier;
        if (m_speed < -m_backSpeed) m_speed = -m_backSpeed;
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_speed *= 0.96f;
        if (std::abs(m_speed) < 0.5f) m_speed = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (std::abs(m_speed) > 3)
            m_direction -= turnSpeed * dt * speedFactor;
        else if (std::abs(m_speed) > 0)
            m_direction -= 2.0f * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (std::abs(m_speed) > 3)
            m_direction += turnSpeed * dt * speedFactor;
        else if (std::abs(m_speed) > 0)
            m_direction += 2.0f * dt;
    }

    m_velocity.x = sin(m_direction) * m_speed * dt;
    m_velocity.y = -cos(m_direction) * m_speed * dt;
    m_sprite.move(m_velocity);

    m_angle = m_direction * 180.0f / 3.14159f;
    m_sprite.setRotation(m_angle);
}

void PlayerCar::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void PlayerCar::setPosition(const sf::Vector2f& pos)
{
    m_sprite.setPosition(pos);
}

sf::Vector2f PlayerCar::getPosition() const
{
    return m_sprite.getPosition();
}

sf::FloatRect PlayerCar::getBounds() const
{
    return m_sprite.getGlobalBounds();
}

float PlayerCar::getSpeed() const
{
    return std::abs(m_speed);
}

int PlayerCar::getRawSpeed() const
{
    return static_cast<int>(m_speed);
}

void PlayerCar::reduceSpeed(float factor)
{
    m_speed *= factor;
    if (std::abs(m_speed) < 0.5f) m_speed = 0;
}

void PlayerCar::bounce()
{
    if (m_speed > 0)
        m_speed = -20.0f;
    else if (m_speed < 0)
        m_speed = 20.0f;
    else
        m_speed = -20.0f;
}

void PlayerCar::setBoostActive(bool active)
{
    m_boostActive = active;
    m_maxSpeed = active ? m_normalMaxSpeed * 1.6f : m_normalMaxSpeed;
}

void PlayerCar::setMaxSpeed(float max)
{
    m_maxSpeed = max;
    m_normalMaxSpeed = max;
}

void PlayerCar::setTraction(float t)
{
    m_traction = t;
}