#pragma once
#include <SFML/Graphics.hpp>

class Helper
{
public:
	static sf::Vector2i pixelToTile(sf::Vector2f pixel);
	static sf::Vector2f tileToPixel(sf::Vector2i tile);
	static int tileToIndex(sf::Vector2i tile);
	static sf::Vector2i animateMovement(int frame, int row, bool& isFacingLeft);
};
