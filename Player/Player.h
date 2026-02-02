#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();

    void handleInput();
    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    sf::Vector2f getPosition() const;

private:
    sf::RectangleShape m_shape;
    sf::Vector2f m_velocity;

    float m_speed = 200.f; // units per second
};