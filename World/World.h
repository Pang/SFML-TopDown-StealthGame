#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include "../TileMap/TileMap.h"
#include "../Enums/DrawLayer.h"
#include "../Enums/WorldEntities.h"

class World
{
public:
	void loadTileMaps();
	void renderTileMaps(sf::RenderWindow& window, DrawLayer layer);
	std::vector<bool> getCollisionMap() const { return m_collisionMap; }

public:
	std::vector<WorldEntities> m_worldEntities;

private:
	std::vector<int> loadCSV(unsigned& outWidth, unsigned& outHeight, const std::string& fileName);
	std::vector<int> loadSpecificMap(TileMap& tileMap, sf::Texture& texture, const std::string& textureFile, const std::string& csvFile);
	void SetBorderCollisionTiles(std::vector<int>& tiles);

private:
	sf::Texture m_floorTileset;
	sf::Texture m_wallsTileset;
	sf::Texture m_doorTileset;
	TileMap m_floorsTileMap;
	TileMap m_wallsTileMap;
	TileMap m_doorTileMap;

	unsigned m_roomWidth;

	std::vector<bool> m_collisionMap;
};
