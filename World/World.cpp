#include "World.h"
#include <fstream>

static constexpr int TILE_SIZE = 16;

void World::loadTileMaps() {
    m_roomWidth = 11;
    std::vector<int> wallTilesVec = loadMapWithCSV(m_wallsTileMap, m_wallsTileset, "Assets/Snoblin Dungeon/Tiles/walls_dungeon.png", "Assets/CSVs/walls.csv");
    SetBorderCollisionTiles(wallTilesVec);

    loadMapWithCSV(m_floorsTileMap, m_floorTileset, "Assets/Snoblin Dungeon/Tiles/ground_dungeon.png", "Assets/CSVs/floor.csv");
    loadDoorObjects();
}

void World::loadDoorObjects() {
    std::vector<int> doorTilesVec = loadMapWithCSV(m_doorTileMap, m_doorTileset, "Assets/Snoblin Dungeon/Tiles/animated_doors.png", "Assets/CSVs/door.csv");
    std::vector<int> indices;
    Helper::getAllNonEmptyTileIndices(doorTilesVec, indices);
    for (int index : indices) {
        m_worldEntities[index] = WE_EXIT_DOOR;
    }
}

void World::loadLevelObjects(std::string level) {
    std::string levelCsv = "Assets/CSVs/Level" + level + "Objects.csv";
    std::vector<int> objTilesVec = loadMapWithCSV(m_objectTileMap, m_objTileset, "Assets/Snoblin Default Tilemap/Objects/chosen_objects.png", levelCsv);

    std::vector<int> indices;
    Helper::getAllNonEmptyTileIndices(objTilesVec, indices);
    for (int index : indices) {
        if (objTilesVec[index] != -1) {
            m_collisionMap[index] = true;
            m_worldEntities[index] = WE_OBJECT;
			if (objTilesVec[index] == WO_Chest_Closed) {
				chest_index = index;
            }
        }
    }
}

std::vector<int> World::loadMapWithCSV(TileMap& tileMap, sf::Texture& texture, const std::string& textureFile, const std::string& csvFile) {
    if (!texture.loadFromFile(textureFile)) throw std::runtime_error("Couldn't find spritesheet");

    unsigned width, height;
    std::vector<int> tiles = loadCSV(width, height, csvFile);
    if (tiles.size() != width * height)
        throw std::runtime_error("Tiles count does not match map dimensions");

    if (!tileMap.load(texture, { TILE_SIZE, TILE_SIZE }, tiles, width, height))
        throw std::runtime_error("Failed to load tilemap");

    return tiles;
}

void World::renderTileMaps(sf::RenderWindow& window, DrawLayer layer) {
    switch (layer)
    {
        case Floor:
            window.draw(m_floorsTileMap);
            break;
        case Walls:
            window.draw(m_wallsTileMap);
            break;
        case Objects:
            window.draw(m_objectTileMap);
            window.draw(m_doorTileMap);
            break;
        default:
            break;
    }
}

void World::SetBorderCollisionTiles(std::vector<int>& tiles) {
    m_collisionMap.resize(tiles.size());
    m_worldEntities.resize(tiles.size());
    for (unsigned i = 0; i < tiles.size(); i++) {
        if (tiles[i] != -1) {
            m_collisionMap[i] = true;
			m_worldEntities[i] = WE_WALL;
        }
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

