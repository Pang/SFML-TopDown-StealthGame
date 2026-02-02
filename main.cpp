#include <SFML/Graphics.hpp>
#include "TileMap/TileMap.h"
#include "Player/Player.h"
#include "World/World.h"
#include "Camera/Camera.h"

int main()
{
    sf::Clock clock;
    Player player;
    World world;
    Camera camera;

    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML 3 Tilemap");
    TileMap map = world.buildWorld();

    camera.setupCamera(player.getPosition());
    window.setView(camera.viewCam);

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();
        player.handleInput();
        player.update(dt);

		camera.updateCamera(player.getPosition(), dt);
        window.setView(camera.viewCam);
        window.clear();
        window.draw(map);
        player.draw(window);
        window.display();
    }
}
