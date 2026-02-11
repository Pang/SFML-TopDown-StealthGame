#include "game.h"
#include <iostream>
#include <SFML/Graphics.hpp>

Game::Game(sf::RenderWindow& window, Player& player, World& world) 
	: font("Assets/Fonts/pixantiqua.ttf"), overlayText(font), m_player(player), m_world(world)
{
	world.loadTileMaps();
	m_gameState = GS_MainMenu;
	m_gameLevel = GL_None;

	m_player.onKeyFound.subscribe([this]() {
		std::cout << "Key found!\n";
		m_world.handleOnKeyFound();
	});

	m_player.onExitReached.subscribe([this]() {
		std::cout << "Exit reached! Game over.\n";
		m_gameState = GS_LevelComplete;
	});

	overlay.setFillColor(sf::Color(0, 0, 0, 150));
	overlay.setSize(sf::Vector2f(window.getSize()));

	backgroundTexture.loadFromFile("Assets/bg2.png");
	backgroundSprite = sf::Sprite(backgroundTexture);
	sf::Vector2u windowSize = window.getSize();
	sf::Vector2u textureSize = backgroundTexture.getSize();

	backgroundSprite.setScale({
		static_cast<float>(windowSize.x) / textureSize.x,
		static_cast<float>(windowSize.y) / textureSize.y
		});
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
	m_world.loadTileMaps();

	switch (m_gameLevel)
	{
		case GL_None:
			break;
		case GL_One:
			m_player.resetPlayer({ 2, 10 });
			m_world.loadLevelObjects("1");
			m_npcs.resize(3);
			m_npcs[0] = Npc(NpcType::GoblinRogue, { 3, 3 }, { 7, 3 });
			m_npcs[1] = Npc(NpcType::GoblinBarbarian, { 5, 4 }, { 5, 7 });
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
			m_player.resetPlayer({ 3, 10 });
			m_world.loadLevelObjects("2");
			m_npcs.resize(5);
			m_npcs[0] = Npc(NpcType::Pikeman, { 9, 7 }, { 9, 7 });
			m_npcs[0].faceDirection(LookLeft);
			m_npcs[1] = Npc(NpcType::HeavyKnight, { 4, 8 }, { 8, 8 });
			m_npcs[2] = Npc(NpcType::Pikeman, { 3, 7 }, { 3, 3 });
			m_npcs[3] = Npc(NpcType::HeavyKnight, { 1, 5 }, { 1, 10 });
			m_npcs[4] = Npc(NpcType::Pikeman, { 5, 5 }, { 9, 5 });

			for (Npc& npc : m_npcs) {
				npc.onPlayerFound.subscribe([this]() {
					playerCaught = true;
					});
			}
			m_gameState = GS_Playing;
			break;
		case GL_Three:
			m_player.resetPlayer({ 5, 10 });
			m_world.loadLevelObjects("3");
			m_npcs.resize(6);
			m_npcs[0] = Npc(NpcType::Ghost, { 2, 3 }, { 6, 3 });
			m_npcs[1] = Npc(NpcType::Ghost, { 6, 4 }, { 3, 4 });
			m_npcs[2] = Npc(NpcType::Ghost, { 5, 5 }, { 8, 5 });
			m_npcs[3] = Npc(NpcType::Ghost, { 1, 6 }, { 4, 6 });
			m_npcs[4] = Npc(NpcType::Ghost, { 7, 8 }, { 3, 8 });
			m_npcs[5] = Npc(NpcType::Ghost, { 3, 9 }, { 6, 9 });

			for (Npc& npc : m_npcs) {
				npc.onPlayerFound.subscribe([this]() {
					playerCaught = true;
					});
			}
			m_gameState = GS_Playing;
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
		window.setView(window.getDefaultView());
		window.draw(backgroundSprite);

		startGameButton.draw(window);
		exitGameButton.draw(window);
		startGameButton.update(window);
		exitGameButton.update(window);

		if (startGameButton.isClicked()) {
			m_gameLevel = GL_Three;
			setupLevel();
			m_gameState = GS_Playing;
		}
		if (exitGameButton.isClicked()) {
			window.close();
		}
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

		window.draw(overlay);
		window.draw(overlayText);
		resetButton.draw(window);
		nextLevelButton.draw(window);

		resetButton.update(window);
		nextLevelButton.update(window);

		if (resetButton.isClicked()) {
			std::cout << "Resetting level...\n";
			setupLevel();
		}

		if (nextLevelButton.isClicked()) {
			int currentLevel = (int)m_gameLevel;
			m_gameLevel = (GameLevel)++currentLevel;
			setupLevel();
			m_gameState = GS_Playing;
		}
		break;
	case GS_GameOver:
		window.setView(window.getDefaultView());
		setOverlayText("You got caught!");

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

