#include "game.h"

Game::Game()
{
	m_gameState = GS_Playing;
	m_gameLevel = GL_One;
}


void Game::setupLevel()
{
	m_npcs.clear();

	switch (m_gameLevel)
	{
		case GL_None:
			break;
		case GL_One:
			m_npcs.resize(3);
			m_npcs[0] = Npc(NpcType::Goblin, { 3, 3 }, { 7, 3 });
			m_npcs[1] = Npc(NpcType::Goblin, { 5, 4 }, { 5, 7 });
			m_npcs[2] = Npc(NpcType::Goblin, { 4, 7 }, { 4, 7 });
			m_npcs[2].faceDirection(LookLeft);
			break;
		case GL_Two:
			break;
		case GL_Three:
			break;
		case GL_Four:
			break;
		case GL_Five:
			break;
	}
}

void Game::runLevel(float dt, int frame, std::vector<WorldEntities>& worldEntities)
{
	switch (m_gameState)
	{
		case GS_MainMenu:
			break;
		case GS_Playing:
			for (Npc& npc : m_npcs) {
				npc.update(dt, frame, worldEntities);
			}
			break;
		case GS_PauseMenu:
			break;
		case GS_GameOver:
			break;
	}
}

void Game::render(sf::RenderWindow& window)
{
	switch (m_gameState)
	{
		case GS_MainMenu:
			break;
		case GS_Playing:
			for (Npc& npc : m_npcs) {
				npc.draw(window);
			}
			break;
		case GS_PauseMenu:
			break;
		case GS_GameOver:
			break;
	}
}

