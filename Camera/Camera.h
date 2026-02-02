#pragma once
#include <SFML/Graphics.hpp>

class Camera {
public:
	void setupCamera(sf::Vector2f camPosition);
	void updateCamera(sf::Vector2f newCamPos, float dt);
public:
	sf::View viewCam;
};
