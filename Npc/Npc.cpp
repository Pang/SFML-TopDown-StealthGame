#include "Npc.h"
#include <iostream>

const std::string ghostPath = "Assets/Snoblin Ghosts/Ghost/ghost.png";
const std::string skeletonPath = "Assets/Snoblin Skeletons/Skeleton/skeleton.png";
const std::string skeletonWarriorPath = "Assets/Snoblin Skeletons/Skeleton Warrior/skeleton_warrior.png";

const std::string knightHeavyPath = "Assets/Snoblin Knights/Heavy Knight/heavy_knight2.png";
const std::string knightPikemanPath = "Assets/Snoblin Knights/Pikeman/pikeman.png";

const std::string goblinPath = "Assets/Snoblin Goblin Clan/Goblin Villager/goblin_villager.png";
const std::string goblinRoguePath = "Assets/Snoblin Goblin Clan/Goblin Rogue/goblin_rogue.png";
const std::string goblinBarbarianPath = "Assets/Snoblin Goblin Clan/Goblin Barbarian/goblin_barbarian.png";

const std::string darkElfVillagerPath = "Assets/Snoblin Dark Elves/Dark Elf Villager 1/dark_elf_villager_1.png";
const std::string darkElfWarriorPath = "Assets/Snoblin Dark Elves/Dark Elf Warrior/dark_elf_warrior2.png";


constexpr int TILE_SIZE = 32;
constexpr int TILE_ROW_LENGTH = 11;

Npc::Npc()
{
}

Npc::Npc(NpcType npcType, sf::Vector2i startTile, sf::Vector2i endTile)
{
	m_startTile = startTile;
	m_endTile = endTile;
	m_npcPos = Helper::tileToPixel(startTile);

	m_state = startTile == endTile ? NpcState::Waiting : NpcState::MovingToEndOfPath;

	if (!m_npcTexture.loadFromFile(getPath(npcType)))
		throw std::runtime_error("Couldn't find spritesheet");
}

std::string Npc::getPath(NpcType type) {
	if (type == NpcType::Goblin) return goblinPath;
	else if (type == NpcType::GoblinRogue) return goblinRoguePath;
	else if (type == NpcType::GoblinBarbarian) return goblinBarbarianPath;
	else if (type == NpcType::Skeleton) return skeletonPath;
	else if (type == NpcType::SkeletonWarrior) return skeletonWarriorPath;
	else if (type == NpcType::Ghost) return ghostPath;
	else if (type == NpcType::HeavyKnight) return knightHeavyPath;
	else if (type == NpcType::Pikeman) return knightPikemanPath;
	else if (type == NpcType::DarkElfVillager) return darkElfVillagerPath;
	else if (type == NpcType::DarkElfWarrior) return darkElfWarriorPath;
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
			spottedPlayer(worldEntities, DIR_UP);
			break;
		case Down:
			spottedPlayer(worldEntities, DIR_DOWN);
			break;
		case Left:
			spottedPlayer(worldEntities, DIR_LEFT);
			break;
		case Right:
			spottedPlayer(worldEntities, DIR_RIGHT);
			break;
		default:
			break;
	}
}

void Npc::spottedPlayer(std::vector<WorldEntities>& worldEntities, sf::Vector2i dir) {
	sf::Vector2i npcClosestTile = Helper::pixelToTile(m_npcPos);
	int index = Helper::tileToIndex(npcClosestTile);

	for (int v = 0; v < viewDistanceTiles; v++) {
		if (!NpcState::Waiting) v++;

		int currentRow = index / 11;
		index = Helper::tileToIndex(npcClosestTile + dir * v);
		if (worldEntities.size() < index) break;
		if (dir == DIR_LEFT || dir == DIR_RIGHT) {
			int tileViewingRow = index / 11;
			if (currentRow != tileViewingRow) break;
		}

		if (worldEntities[index] == WE_OBJECT) {
			break;
		}
		if (worldEntities[index] == WE_PLAYER) {
			std::cout << "Player spotted right!\n";
			std::cout << "NPC at tile: " << npcClosestTile.x << ", " << npcClosestTile.y << " looking in direction: " << m_viewDirection << "\n";

			onPlayerFound.invoke();
			return;
		}
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
