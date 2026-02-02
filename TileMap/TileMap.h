#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    bool load(
        const sf::Texture& tileset,
        sf::Vector2u tileSize,
        const std::vector<int>& tiles,
        unsigned int width,
        unsigned int height
    );

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray m_vertices;
    const sf::Texture* m_tileset = nullptr;
};
