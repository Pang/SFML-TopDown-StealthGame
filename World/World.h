#pragma once

#include "../TileMap/TileMap.h"

class World
{
public:
	void loadTileMaps();
	void renderTileMaps(sf::RenderWindow& window);
	std::vector<bool> getCollisionMap() const { return m_collisionMap; }
private:
	std::vector<int> loadCSV(unsigned& outWidth, unsigned& outHeight, const std::string& fileName);
	std::vector<int> loadSpecificMap(TileMap& tileMap, sf::Texture& texture, const std::string& textureFile, const std::string& csvFile);
	void SetBorderCollisionTiles(std::vector<int> tiles);
private:
	sf::Texture m_floorTileset;
	sf::Texture m_dungeonTileset;
	TileMap m_floorsTileMap;
	TileMap m_dungeonTileMap;

	std::vector<bool> m_collisionMap;
};

