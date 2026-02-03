#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();

    void handleInput(int frame);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getCamPosition() const;
    sf::Vector2f getPosition() const;

private:
    sf::Vector2i animatePlayerMovement(int frame, int row);

private:
    sf::Vector2f m_velocity;
    sf::Texture m_playerTexture;
    sf::Vector2i currentPlayerSprite;

    float m_speed = 200.f;
	bool isFacingLeft;
};