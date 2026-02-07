#include "game.h"
#include <iostream>
#include <SFML/Graphics.hpp>

Game::Game(sf::RenderWindow& window, Player& player, World& world) 
	: font("Assets/Fonts/pixantiqua.ttf"), overlayText(font), m_player(player), m_world(world)
{
	world.loadTileMaps();
	m_gameState = GS_Playing;
	m_gameLevel = GL_One;

	m_player.onExitReached.subscribe([this]() {
		std::cout << "Exit reached! Game over.\n";
		m_gameState = GS_LevelComplete;
	});

	overlay.setFillColor(sf::Color(0, 0, 0, 150));
	overlay.setSize(sf::Vector2f(window.getSize()));
}

void Game::setupLevel()
{
	playerCaught = false;
	for (size_t i = 0; i < m_world.m_worldEntities.size(); ++i) {
		if (m_world.m_worldEntities[i] == WE_PLAYER) {
			m_world.m_worldEntities[i] = WE_NONE;
		}
	}

	m_npcs.clear();
	m_player.resetPlayer();

	switch (m_gameLevel)
	{
		case GL_None:
			break;
		case GL_One:
			m_world.loadTileMaps();
			m_npcs.resize(3);
			m_npcs[0] = Npc(NpcType::Goblin, { 3, 3 }, { 7, 3 });
			m_npcs[1] = Npc(NpcType::Goblin, { 5, 4 }, { 5, 7 });
			m_npcs[2] = Npc(NpcType::Goblin, { 4, 7 }, { 4, 7 });
			m_npcs[2].faceDirection(LookLeft);

			for (Npc& npc : m_npcs) {
				npc.onPlayerFound.subscribe([this]() {
					playerCaught = true;
				});
			}
			m_gameState = GS_Playing;
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

void Game::runLevel(float dt, int frame)
{
	switch (m_gameState)
	{
		case GS_MainMenu:
			break;
		case GS_Playing:
			m_player.handleInput(frame, m_world.getCollisionMap());
			m_player.update(dt, m_world.m_worldEntities);
			for (Npc& npc : m_npcs) {
				npc.update(dt, frame, m_world.m_worldEntities);
			}

			if (playerCaught) {
				std::cout << "Player caught! Game over.\n";
				m_gameState = GS_GameOver;
				for (Npc& npc : m_npcs)
					npc.onPlayerFound.unsubscribeAll();
			}
			break;
		case GS_PauseMenu:
			break;
		case GS_GameOver:
			m_npcs.clear();
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
		m_world.renderTileMaps(window, Floor);
		m_world.renderTileMaps(window, Walls);
		m_world.renderTileMaps(window, Objects);

		m_player.draw(window);
		for (Npc& npc : m_npcs) {
			npc.draw(window);
		}
		break;
	case GS_LevelComplete:
		window.setView(window.getDefaultView());
		setOverlayText("Level complete!");
		resetButton.setText("Reset level");

		window.draw(overlay);
		window.draw(overlayText);
		resetButton.draw(window);
		resetButton.update(window);

		if (resetButton.isClicked()) {
			std::cout << "Resetting level...\n";
			setupLevel();
		}
		break;
	case GS_GameOver:
		window.setView(window.getDefaultView());
		setOverlayText("You got caught!");
		resetButton.setText("Reset level");

		window.draw(overlay);
		window.draw(overlayText);
		resetButton.draw(window);
		resetButton.update(window);

		if (resetButton.isClicked()) {
			std::cout << "Resetting level...\n";
			setupLevel();
		}

		break;
	}
}

void Game::setOverlayText(const std::string& text) {
	overlayText.setString(text);
	overlayText.setCharacterSize(32);
	overlayText.setFillColor(sf::Color::White);

	sf::FloatRect bounds = overlayText.getLocalBounds();
	overlayText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f });
	overlayText.setPosition({ overlay.getSize().x / 2.f, overlay.getSize().y / 2.f });
}

