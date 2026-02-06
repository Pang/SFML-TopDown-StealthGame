#include "Npc.h"
#include <iostream>

const std::string ghostPath = "Assets/Snoblin Ghosts/Ghost/ghost.png";
const std::string goblinPath = "Assets/Snoblin Goblin Clan/Goblin Rogue/goblin_rogue.png";
const std::string skeletonPath = "Assets/Snoblin Skeletons/Skeleton/skeleton";

constexpr int TILE_SIZE = 32;
constexpr int TILE_ROW_LENGTH = 11;

Npc::Npc(NpcType npcType, sf::Vector2i startTile, sf::Vector2i endTile)
{
	m_startTile = startTile;
	m_endTile = endTile;
	m_npcPos = Helper::tileToPixel(startTile);

	if (startTile == endTile) {
		m_state = NpcState::Waiting;
	}
	else {
		m_state = NpcState::MovingToEndOfPath;
	}

	switch (npcType)
	{
	case NpcType::Ghost:
		if (!m_npcTexture.loadFromFile(ghostPath)) 
			throw std::runtime_error("Couldn't find spritesheet");
		break;
	case NpcType::Goblin:
		if (!m_npcTexture.loadFromFile(goblinPath)) 
			throw std::runtime_error("Couldn't find spritesheet");
		break;
	case NpcType::Skeleton:
		if (!m_npcTexture.loadFromFile(skeletonPath)) 
			throw std::runtime_error("Couldn't find spritesheet");
		break;
	default:
		if (!m_npcTexture.loadFromFile(ghostPath)) 
			throw std::runtime_error("Couldn't find spritesheet");
		break;
	}
}

void Npc::faceDirection(NpcState state) {
	switch (state) {
		case NpcState::LookUp:
			m_currentNpcSprite = sf::Vector2i(0, RunUp * TILE_SIZE);
			m_viewDirection = Up;
			break;
		case NpcState::LookDown:
			m_currentNpcSprite = sf::Vector2i(0, RunDown * TILE_SIZE);
			m_viewDirection = Down;
			break;
		case NpcState::LookLeft:
			m_currentNpcSprite = sf::Vector2i(TILE_SIZE, RunSideways * TILE_SIZE);
			m_isFacingLeft = true;
			m_viewDirection = Left;
			break;
		case NpcState::LookRight:
			m_currentNpcSprite = sf::Vector2i(0, RunSideways * TILE_SIZE);
			m_isFacingLeft = false;
			m_viewDirection = Right;
			break;
		default:
			break;
	}
}

void Npc::update(float dt, int frame, std::vector<WorldEntities>& worldEntities)
{
	switch (m_state)
	{
		case NpcState::MovingToEndOfPath:
			m_npcTarget = m_endTile;
			moveTowardsTarget(dt, frame, m_endTile);
			break;
		case NpcState::MovingToStartOfPath:
			m_npcTarget = m_startTile;
			moveTowardsTarget(dt, frame, m_startTile);
			break;
		case NpcState::Waiting:
			if (m_startTile != m_endTile) {
				wait(dt);
			}
			break;
		default:
			break;
	}

	checkPlayerInView(worldEntities);
}

void Npc::checkPlayerInView(std::vector<WorldEntities>& worldEntities) {
	sf::Vector2i npcClosestTile = Helper::pixelToTile(m_npcPos);
	int index = Helper::tileToIndex(npcClosestTile);
	switch (m_viewDirection) {
	case Up:
		for (int v = 0; v < viewDistanceTiles; v++) {
			index = Helper::tileToIndex(npcClosestTile + DIR_UP * v);
			if (worldEntities[index] == WE_PLAYER) {
				std::cout << "Player spotted up!\n";
				return;
			}
		}
		break;
	case Down:
		for (int v = 0; v < viewDistanceTiles; v++) {
			index = Helper::tileToIndex(npcClosestTile + DIR_DOWN * v);
			if (worldEntities[index] == WE_PLAYER) {
				std::cout << "Player spotted down!\n";
				return;
			}
		}
		break;
	case Left:
		for (int v = 0; v < viewDistanceTiles; v++) {
			index = Helper::tileToIndex(npcClosestTile + DIR_LEFT * v);
			if (worldEntities[index] == WE_PLAYER) {
				std::cout << "Player spotted left!\n";
				return;
			}
		}
		break;
	case Right:
		for (int v = 0; v < viewDistanceTiles; v++) {
			index = Helper::tileToIndex(npcClosestTile + DIR_RIGHT * v);
			if (worldEntities[index] == WE_PLAYER) {
				std::cout << "Player spotted right!\n";
				return;
			}
		}
		break;
	default:
		break;
	}
}

void Npc::draw(sf::RenderWindow& window)
{
	sf::Sprite playerSprite = sf::Sprite(m_npcTexture);

	int width;
	if (m_isFacingLeft) width = -32;
	else width = 32;

	playerSprite.setOrigin({ 8.f, 12.f });
	playerSprite.setTextureRect(sf::IntRect(m_currentNpcSprite, sf::Vector2i(width, TILE_SIZE)));
	playerSprite.setPosition(m_npcPos);
	window.draw(playerSprite);
}

void Npc::moveTowardsTarget(float dt, int frame, sf::Vector2i& targetTile)
{
	sf::Vector2f targetPos = Helper::tileToPixel(targetTile);
	sf::Vector2f direction = targetPos - m_npcPos;

	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (direction.x < 0) m_isFacingLeft = true;
	else m_isFacingLeft = false;

	if (direction.y > 0) {
		m_currentNpcSprite = Helper::animateMovement(frame, RunDown * TILE_SIZE, m_isFacingLeft);
		m_viewDirection = Down;
	}
	else if (direction.y < 0) {
		m_currentNpcSprite = Helper::animateMovement(frame, RunUp * TILE_SIZE, m_isFacingLeft);
		m_viewDirection = Up;
	}
	else {
		m_currentNpcSprite = Helper::animateMovement(frame, RunSideways * TILE_SIZE, m_isFacingLeft);
		m_viewDirection = m_isFacingLeft ? Left : Right;
	}

	if (distance < 1.f) {
		m_npcPos = targetPos;
		m_npcTile = m_npcTarget;
		m_state = NpcState::Waiting;
		return;
	}

	direction /= distance;
	m_npcPos += direction * npcSpeed * dt;
}

void Npc::wait(float dt)
{
	npcWaitingFor -= dt;
	if (npcWaitingFor <= 0.f) {
		npcWaitingFor = npcWaitTime;
		m_state = (m_state == NpcState::Waiting && m_npcTile == m_endTile) ? NpcState::MovingToStartOfPath : NpcState::MovingToEndOfPath;
	}
}
