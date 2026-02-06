#pragma once
#include <SFML/Graphics.hpp>
#include "../Helpers/Helper.h"
#include "../Enums/NpcType.h"
#include "../Enums/NpcState.h"
#include "../Enums/Animation.h"
#include "../Enums/WorldEntities.h"
#include "../Enums/NpcViewDirection.h"

class Npc
{
public:
	Npc();
	Npc(NpcType npcType, sf::Vector2i startTile, sf::Vector2i endTile);

	void update(float dt, int frame, std::vector<WorldEntities>& worldEntities);
	void draw(sf::RenderWindow& window);
	void faceDirection(NpcState state);
private:
	void moveTowardsTarget(float dt, int frame, sf::Vector2i& targetTile);
	void wait(float dt);

	void checkPlayerInView(std::vector<WorldEntities>& worldEntities);

private:
	sf::Vector2i m_currentNpcSprite;
	sf::Texture m_npcTexture;
	NpcType m_type;
	NpcState m_state;
	
	sf::Vector2i m_startTile;
	sf::Vector2i m_endTile;

	sf::Vector2i m_npcTile; 
	sf::Vector2i m_npcTarget;
	sf::Vector2f m_npcPos;

	bool m_isFacingLeft = false;
	float npcSpeed = 25.f;
	float npcWaitTime = 3.f;
	float npcWaitingFor = 3.f;

	NpcViewDirection m_viewDirection;
	int viewDistanceTiles = 4;

	sf::Vector2i DIR_UP = { 0, -1 };
	sf::Vector2i DIR_DOWN = { 0,  1 };
	sf::Vector2i DIR_LEFT = { -1, 0 };
	sf::Vector2i DIR_RIGHT = { 1, 0 };
};
