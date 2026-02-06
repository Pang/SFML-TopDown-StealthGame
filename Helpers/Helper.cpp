#include "Helper.h"

const int MAP_WIDTH = 11;
const float FLOOR_TILE_SIZE = 16.f;
const float CHARACTER_TILE_SIZE = 32.f;

sf::Vector2i Helper::pixelToTile(sf::Vector2f pixel)
{
	return sf::Vector2i(
		static_cast<int>(pixel.x / 16.f),
		static_cast<int>(pixel.y / 16.f)
	);
}

sf::Vector2f Helper::tileToPixel(sf::Vector2i tile)
{
	return sf::Vector2f(tile.x * FLOOR_TILE_SIZE, tile.y * FLOOR_TILE_SIZE);
}

int Helper::tileToIndex(sf::Vector2i tile)
{
	return tile.x + tile.y * MAP_WIDTH;
}

sf::Vector2i Helper::animateMovement(int frame, int row, bool& isFacingLeft) {
	frame = (frame + 1) % 4;
	int col = frame * 32;
	if (isFacingLeft) col += CHARACTER_TILE_SIZE;

	return sf::Vector2i({ col, row });
}