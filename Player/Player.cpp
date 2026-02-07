#include "Player.h"
#include "../Enums/Animation.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

static constexpr int TILE_SIZE = 32;
static constexpr int TILE_ROW_LENGTH = 11;

Player::Player()
{
    resetPlayer();

    if (!m_playerTexture.loadFromFile("Assets/Snoblin Villagers/Human Nobleman/human_nobleman.png"))
    {
        throw std::runtime_error("Couldn't find spritesheet");
    }
}

void Player::resetPlayer() {
    m_tilePos = { 2, 10 };
    sf::Vector2f startPos = { m_tilePos.x * TILE_SIZE * .5f, m_tilePos.y * TILE_SIZE * .5f };
    m_isFacingLeft = false;
    m_currentPos = startPos;
    m_worldPos = startPos;
    m_startWorldPos = startPos;
	m_targetWorldPos = startPos;
}

void Player::handleInput(int frame, std::vector<bool> collisionMap)
{
    if (m_isMoving) return;

    sf::Vector2i dir{ 0, 0 };
    m_isFacingLeft = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        dir.y = -1;
        m_currentPlayerSprite = Helper::animateMovement(frame, RunUp * TILE_SIZE, m_isFacingLeft);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        dir.y = 1;
        m_currentPlayerSprite = Helper::animateMovement(frame, RunDown * TILE_SIZE, m_isFacingLeft);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        dir.x = -1;
        m_isFacingLeft = true;
        m_currentPlayerSprite = Helper::animateMovement(frame, RunSideways * TILE_SIZE, m_isFacingLeft);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        dir.x = 1;
        m_currentPlayerSprite = Helper::animateMovement(frame, RunSideways * TILE_SIZE, m_isFacingLeft);
    }

    if (dir == sf::Vector2i{ 0, 0 }) {
        m_currentPlayerSprite = sf::Vector2i(0, Idle);
		return;
    }

	m_previousTile = m_tilePos;
    sf::Vector2f tilePos = m_tilePos + static_cast<sf::Vector2f>(dir);
    if (collisionMap[tilePos.x + tilePos.y * TILE_ROW_LENGTH]) return;

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

void Player::update(float dt, std::vector<WorldEntities>& worldEntities)
{
    if (!m_isMoving) return;

    m_moveTimer += dt;
    float t = m_moveTimer / m_moveDuration;
    t = std::min(t, 1.f);

    m_worldPos = m_startWorldPos + (m_targetWorldPos - m_startWorldPos) * t;
	m_currentPos = m_worldPos;

    if (t >= 1.f) {
        m_worldPos = m_targetWorldPos;
        m_isMoving = false;
    }

    int prevIndex = Helper::tileToIndex({ static_cast<int>(m_previousTile.x), static_cast<int>(m_previousTile.y) });
    int newIndex = Helper::tileToIndex({ static_cast<int>(m_tilePos.x), static_cast<int>(m_tilePos.y) });

    worldEntities[prevIndex] = WE_NONE;

    if (worldEntities[newIndex] == WE_EXIT_DOOR) {
        onExitReached.invoke();
    }

    worldEntities[newIndex] = WE_PLAYER;
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
