#pragma once
#include <SFML/Graphics.hpp>
#include "../Enums/NpcType.h"
#include "../Enums/NpcState.h"
#include "../Enums/Animation.h"

class Npc
{
public:
	Npc(NpcType npcType, sf::Vector2i startTile, sf::Vector2i endTile);

	void update(float dt, int frame);
	void draw(sf::RenderWindow& window);
	void faceDirection(NpcState state);

private:
	sf::Vector2f tileToPixel(sf::Vector2i tile);
	void moveTowardsTarget(float dt, int frame, sf::Vector2i& targetTile);
	void wait(float dt);
	sf::Vector2i animateNpcMovement(int frame, int row);

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
};

