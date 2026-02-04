#include <SFML/Graphics.hpp>
#include "Player/Player.h"
#include "World/World.h"
#include "Camera/Camera.h"

float animationFPS = 12.f;
float frameDuration = 1.f / animationFPS;

int main()
{
    sf::Clock clock;
    Player player;
    World world;
    Camera camera;

    sf::RenderWindow window(sf::VideoMode({1300, 800}), "Top Down RPG", sf::Style::Default, sf::State::Windowed);
    world.loadTileMaps();

    camera.setupCamera(player.getCamPosition());
    window.setView(camera.viewCam);

    float timeAccumulator = 0.f;
    int frame = 0;
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
			window.close();
        }

        float dt = clock.restart().asSeconds();
        timeAccumulator += dt;

        if (timeAccumulator >= frameDuration) {
            frame = (frame + 1) % 4;
            timeAccumulator -= frameDuration;
        }

        player.handleInput(frame, world.getCollisionMap());
        player.update(dt);

		camera.updateCamera(player.getCamPosition(), dt);
        window.setView(camera.viewCam);
        window.clear();
		world.renderTileMaps(window);
        player.draw(window);
        window.display();
    }
}
