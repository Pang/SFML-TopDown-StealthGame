#include "World.h"
#include <fstream>

TileMap World::buildWorld()
{
    if (!m_tileset.loadFromFile("Assets/Pixel Art Pack/TX Tileset Grass.png"))
    {
        throw std::runtime_error("Couldn't find spritesheet");
    }

    unsigned width, height;
	std::vector<int> tiles = loadCSV(width, height);

    if (tiles.size() != levelWidth * levelHeight)
        throw std::runtime_error("Tile count does not match map dimensions");

    TileMap map;
    if (!map.load(m_tileset, { 32, 32 }, tiles, width, height))
        throw std::runtime_error("Failed to load tilemap");

    return map;
}

std::vector<int> World::loadCSV(unsigned& outWidth, unsigned& outHeight)
{
    std::ifstream file("Assets/world.csv");
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