#include "Player.h"
#include "PlayerAnimation.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

static constexpr int TILE_SIZE = 32;

Player::Player()
{
    m_tilePos = { 1, 1};
    sf::Vector2f startPos = { m_tilePos.x * TILE_SIZE * .5f, m_tilePos.y * TILE_SIZE * .5f };
    if (!m_playerTexture.loadFromFile("Assets/Snoblin Villagers/Human Nobleman/human_nobleman.png"))
    {
        throw std::runtime_error("Couldn't find spritesheet");
    }

    m_isFacingLeft = false;
    m_currentPos = startPos;
    m_worldPos = startPos;
    m_startWorldPos = startPos;
}

void Player::handleInput(int frame, std::vector<bool> collisionMap)
{
    if (m_isMoving)
        return;

    sf::Vector2i dir{ 0, 0 };
    m_isFacingLeft = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        dir.y = -1;
        m_currentPlayerSprite = animatePlayerMovement(frame, RunUp * TILE_SIZE);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        dir.y = 1;
        m_currentPlayerSprite = animatePlayerMovement(frame, RunDown * TILE_SIZE);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        dir.x = -1;
        m_isFacingLeft = true;
        m_currentPlayerSprite = animatePlayerMovement(frame, RunSideways * TILE_SIZE);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        dir.x = 1;
        m_currentPlayerSprite = animatePlayerMovement(frame, RunSideways * TILE_SIZE);
    }

    if (dir == sf::Vector2i{ 0, 0 }) {
        m_currentPlayerSprite = sf::Vector2i(0, Idle);
		return;
    }

    sf::Vector2f tilePos = m_tilePos + static_cast<sf::Vector2f>(dir);

    if (collisionMap[tilePos.x + tilePos.y * 9]) return;

    m_startWorldPos = m_worldPos;
    m_tilePos += static_cast<sf::Vector2f>(dir);

    std::cout << m_tilePos.x << ", " << m_tilePos.y << "\n";

    m_targetWorldPos = {
        m_tilePos.x * TILE_SIZE * .5f,
        m_tilePos.y * TILE_SIZE * .5f
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
	m_currentPos = m_worldPos;

    if (t >= 1.f)
    {
        m_worldPos = m_targetWorldPos;
        m_isMoving = false;
    }
}

void Player::draw(sf::RenderWindow& window) const
{
    sf::Sprite playerSprite = sf::Sprite(m_playerTexture);

	int width;
	if (m_isFacingLeft) width = -32;
	else width = 32;

    playerSprite.setOrigin({ 8.f, 12.f });
    playerSprite.setTextureRect(sf::IntRect(m_currentPlayerSprite, sf::Vector2i(width, TILE_SIZE)));
    playerSprite.setPosition(m_currentPos);
    window.draw(playerSprite);
}

sf::Vector2f Player::getPosition() const {
    return m_currentPos;
}
sf::Vector2f Player::getCamPosition() const {
    return { m_currentPos.x + (TILE_SIZE / 4), m_currentPos.y + (TILE_SIZE / 4) };
}

sf::Vector2i Player::animatePlayerMovement(int frame, int row) {
    frame = (frame + 1) % 4;
    int col = frame * 32;
    if (m_isFacingLeft) col += TILE_SIZE;

    return sf::Vector2i({ col, row});
}