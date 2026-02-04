#include "World.h"
#include <fstream>

static constexpr int TILE_SIZE = 16;

void World::loadTileMaps() {
    //loadSpecificMap(m_floorsTileMap, m_floorTileset, "Assets/Snoblin Dungeon/Tiles/ground_dungeon.png", "Assets/world.csv");
    std::vector<int> tiles = loadSpecificMap(m_dungeonTileMap, m_dungeonTileset, "Assets/Snoblin Dungeon/Tiles/walls_dungeon.png", "Assets/walls.csv");
    SetBorderCollisionTiles(tiles);
}

std::vector<int> World::loadSpecificMap(TileMap& tileMap, sf::Texture& texture, const std::string& textureFile, const std::string& csvFile) {
    if (!texture.loadFromFile(textureFile)) throw std::runtime_error("Couldn't find spritesheet");

    unsigned width, height;
    std::vector<int> tiles = loadCSV(width, height, csvFile);
    if (tiles.size() != width * height)
        throw std::runtime_error("Tiles count does not match map dimensions");

    if (!tileMap.load(texture, { TILE_SIZE, TILE_SIZE }, tiles, width, height))
        throw std::runtime_error("Failed to load tilemap");

    return tiles;
}

void World::renderTileMaps(sf::RenderWindow& window) {
    //window.draw(m_floorsTileMap);
    window.draw(m_dungeonTileMap);
}

void World::SetBorderCollisionTiles(std::vector<int> tiles) {
    unsigned width, height;
    std::vector<int> tilesA = loadCSV(width, height, "Assets/collisions.csv");
    if (tilesA.size() != width * height)
        throw std::runtime_error("Tiles count does not match map dimensions");

    m_collisionMap.resize(tilesA.size());
    for (unsigned i = 0; i < tilesA.size(); i++) {
        if (tilesA[i] == 1) m_collisionMap[i] = true;
    }
}

std::vector<int> World::loadCSV(unsigned& outWidth, unsigned& outHeight, const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file) throw std::runtime_error("Failed to open CSV");

    std::vector<int> tiles;
    std::string line;

    outWidth = 0;
    outHeight = 0;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string cell;
        unsigned rowWidth = 0;

        while (std::getline(ss, cell, ','))
        {
            tiles.push_back(std::stoi(cell));
            rowWidth++;
        }

        if (outWidth == 0)
            outWidth = rowWidth;
        else if (rowWidth != outWidth)
            throw std::runtime_error("CSV rows have inconsistent widths");

        outHeight++;
    }

    return tiles;
}