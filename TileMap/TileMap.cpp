#include "TileMap.h"

bool TileMap::load(
    const sf::Texture& tileset,
    sf::Vector2u tileSize,
    const std::vector<int>& tiles,
    unsigned int width,
    unsigned int height)
{
    if (tileset.getSize().x == 0 || tileset.getSize().y == 0)
        return false;

    if (tileSize.x == 0 || tileSize.y == 0)
        return false;

    unsigned int tilesetColumns = tileset.getSize().x / tileSize.x;
    if (tilesetColumns == 0)
        return false;

    m_tileset = &tileset;

    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(width * height * 6);

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            int tileNumber = tiles[x + y * width];

            int tu = tileNumber % tilesetColumns;
            int tv = tileNumber / tilesetColumns;

            sf::Vertex* quad = &m_vertices[(x + y * width) * 6];

            float px = static_cast<float>(x * tileSize.x);
            float py = static_cast<float>(y * tileSize.y);

            float tx = static_cast<float>(tu * tileSize.x);
            float ty = static_cast<float>(tv * tileSize.y);

            // Triangle 1
            quad[0].position = { px, py };
            quad[1].position = { px + tileSize.x, py };
            quad[2].position = { px + tileSize.x, py + tileSize.y };

            quad[0].texCoords = { tx, ty };
            quad[1].texCoords = { tx + tileSize.x, ty };
            quad[2].texCoords = { tx + tileSize.x, ty + tileSize.y };

            // Triangle 2
            quad[3].position = { px, py };
            quad[4].position = { px + tileSize.x, py + tileSize.y };
            quad[5].position = { px, py + tileSize.y };

            quad[3].texCoords = { tx, ty };
            quad[4].texCoords = { tx + tileSize.x, ty + tileSize.y };
            quad[5].texCoords = { tx, ty + tileSize.y };
        }
    }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = m_tileset;
    target.draw(m_vertices, states);
}
