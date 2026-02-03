#include "Player.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

sf::Vector2f currentPos;
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
	sf::Vector2f startPos = { 0, 0 };
    if (!m_playerTexture.loadFromFile("Assets/Snoblin Characters/prototype_character_yellow.png"))
    {
        throw std::runtime_error("Couldn't find spritesheet");
    }

    currentPos = startPos;
    m_worldPos = startPos;
    m_startWorldPos = startPos;
}

void Player::handleInput(int frame)
{
    if (m_isMoving)
        return;

    sf::Vector2i dir{ 0, 0 };
    isFacingLeft = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        dir.y = -1;
        currentPlayerSprite = animatePlayerMovement(frame, 160);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        dir.y = 1;
        currentPlayerSprite = animatePlayerMovement(frame, 96);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        dir.x = -1;
        isFacingLeft = true;
        currentPlayerSprite = animatePlayerMovement(frame, 128);
        //currentPlayerSprite = sf::Vector2i(TILE_SIZE, 128);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        dir.x = 1;
        currentPlayerSprite = animatePlayerMovement(frame, 128);
    }

    if (dir == sf::Vector2i{ 0, 0 }) {
        currentPlayerSprite = sf::Vector2i(0, 0);
		return;
    }

    m_startWorldPos = m_worldPos;
    m_tilePos += dir;

    m_targetWorldPos = {
        m_tilePos.x * TILE_SIZE + TILE_SIZE * .5f - (TILE_SIZE / 2),
        m_tilePos.y * TILE_SIZE + TILE_SIZE * .5f - (TILE_SIZE / 2)
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
	currentPos = m_worldPos;

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
	if (isFacingLeft) width = -32;
	else width = 32;

    playerSprite.setTextureRect(sf::IntRect(currentPlayerSprite, sf::Vector2i(width, TILE_SIZE)));
    playerSprite.setPosition(currentPos);
    window.draw(playerSprite);
}

sf::Vector2f Player::getPosition() const {
    return currentPos;
}
sf::Vector2f Player::getCamPosition() const {
    return { currentPos.x + (TILE_SIZE / 2), currentPos.y + (TILE_SIZE / 2) };
}

sf::Vector2i Player::animatePlayerMovement(int frame, int row) {
    frame = (frame + 1) % 4;
    int col = frame * 32;
    if (isFacingLeft) col += TILE_SIZE;

    return sf::Vector2i({ col, row});
}