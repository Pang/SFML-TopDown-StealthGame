#include "Npc.h"

const std::string ghostPath = "Assets/Snoblin Ghosts/Ghost/ghost.png";
const std::string goblinPath = "Assets/Snoblin Goblin Clan/Goblin Rogue/goblin_rogue.png";
const std::string skeletonPath = "Assets/Snoblin Skeletons/Skeleton/skeleton";

constexpr int TILE_SIZE = 32;
constexpr int TILE_ROW_LENGTH = 11;

Npc::Npc(NpcType npcType, sf::Vector2i startTile, sf::Vector2i endTile)
{
	m_startTile = startTile;
	m_endTile = endTile;
	m_npcPos = tileToPixel(startTile);

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
			break;
		case NpcState::LookDown:
			m_currentNpcSprite = sf::Vector2i(0, RunDown * TILE_SIZE);
			break;
		case NpcState::LookLeft:
			m_currentNpcSprite = sf::Vector2i(TILE_SIZE, RunSideways * TILE_SIZE);
			m_isFacingLeft = true;
			break;
		case NpcState::LookRight:
			m_currentNpcSprite = sf::Vector2i(0, RunSideways * TILE_SIZE);
			m_isFacingLeft = false;
			break;
		default:
			break;
	}
}

void Npc::update(float dt, int frame)
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
	sf::Vector2f targetPos = tileToPixel(targetTile);
	sf::Vector2f direction = targetPos - m_npcPos;

	float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

	if (direction.x < 0) m_isFacingLeft = true;
	else m_isFacingLeft = false;

	if (direction.y > 0) {
		m_currentNpcSprite = animateNpcMovement(frame, RunDown * TILE_SIZE);
	}
	else if (direction.y < 0) {
		m_currentNpcSprite = animateNpcMovement(frame, RunUp * TILE_SIZE);
	}
	else {
		m_currentNpcSprite = animateNpcMovement(frame, RunSideways * TILE_SIZE);
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

sf::Vector2f Npc::tileToPixel(sf::Vector2i tile) {
	return sf::Vector2f(tile.x * 16.f, tile.y * 16.f);
}

sf::Vector2i Npc::animateNpcMovement(int frame, int row) {
	frame = (frame + 1) % 4;
	int col = frame * 32;
	if (m_isFacingLeft) col += TILE_SIZE;

	return sf::Vector2i({ col, row });
}