#pragma once
#include "../Enums/GameState.h"
#include "../Enums/GameLevel.h"
#include "../Enums/NpcType.h"
#include "../Helpers/Button.h"
#include "../Player/Player.h"
#include "../World/World.h"
#include "../Npc/Npc.h"
#include <SFML/Graphics.hpp>
#include <vector>

class Game {
public:
	Game(sf::RenderWindow& window, Player& player, World& world);
	void setupLevel();
	void runLevel(float dt, int frame);
	void render(sf::RenderWindow& window);

private:
	void setOverlayText(const std::string& text);
	void clearLevels();

private:
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite = sf::Sprite(backgroundTexture);
	sf::RectangleShape overlay;
	sf::Font font;
	sf::Text overlayText;

	GameState m_gameState;
	GameLevel m_gameLevel;

	Player& m_player;
	World& m_world;
	std::vector<Npc> m_npcs;

	bool playerCaught = false;

	Button startGameButton = Button({ 200.f, 40.f }, { 550.f, 50.f }, "Start Game");
	Button exitGameButton = Button({ 200.f, 40.f }, { 550.f, 115.f }, "Exit");
	Button resetButton = Button({ 200.f, 40.f }, { 10.f, 10.f }, "Retry");
	Button nextLevelButton = Button({ 200.f, 40.f }, { 1090.f, 10.f }, "Continue");
};