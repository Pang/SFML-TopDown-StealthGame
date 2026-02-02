#include "Player.h"

sf::Vector2i m_tilePos;
sf::Vector2f m_startWorldPos;
sf::Vector2f m_worldPos;
sf::Vector2f m_targetWorldPos;

bool m_isMoving = false;

float m_moveDuration = 0.25f;
float m_moveTimer = 0.f;

static constexpr int TILE_SIZE = 32;

Player::Player()
{
	sf::Vector2f startPos = { 16, 16 };

    m_shape.setSize({ 32.f, 32.f });
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin(m_shape.getSize() * 0.5f);
    m_shape.setPosition(startPos);
    m_worldPos = startPos;
    m_startWorldPos = startPos;
}

void Player::handleInput()
{
    if (m_isMoving)
        return;

    sf::Vector2i dir{ 0, 0 };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        dir.y = -1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        dir.y = 1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        dir.x = -1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        dir.x = 1;

    if (dir == sf::Vector2i{ 0, 0 })
        return;

    m_startWorldPos = m_worldPos;
    m_tilePos += dir;

    m_targetWorldPos = {
        m_tilePos.x * TILE_SIZE + TILE_SIZE * 0.5f,
        m_tilePos.y * TILE_SIZE + TILE_SIZE * 0.5f
    };

    m_moveTimer = 0.f;
    m_isMoving = true;
}

void Player::update(float dt)
{
    if (!m_isMoving)
        return;

    m_moveTimer += dt;
    float t = m_moveTimer / m_moveDuration;
    t = std::min(t, 1.f);

    m_worldPos = m_startWorldPos + (m_targetWorldPos - m_startWorldPos) * t;

    m_shape.setPosition(m_worldPos);

    if (t >= 1.f)
    {
        m_worldPos = m_targetWorldPos;
        m_isMoving = false;
    }
}

void Player::draw(sf::RenderWindow& window) const
{
    window.draw(m_shape);
}

sf::Vector2f Player::getPosition() const
{
    return m_shape.getPosition();
}
