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

    bool m_isMoving = false;
    float m_moveDuration = 0.25f;
    float m_moveTimer = 0.f;

    sf::Vector2i currentPlayerSprite;
    sf::Vector2f currentPos;
    sf::Vector2f m_tilePos;
    sf::Vector2f m_startWorldPos;
    sf::Vector2f m_worldPos;
    sf::Vector2f m_targetWorldPos;

    float m_speed = 200.f;
	bool isFacingLeft;
};