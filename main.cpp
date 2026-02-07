#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "main.h"

const float animationFPS = 12.f;
const float frameDuration = 1.f / animationFPS;

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	bool isPaused = false;

    sf::RenderWindow window(sf::VideoMode({ 1300, 800 }), "2d Stealth Game", sf::Style::Default, sf::State::Windowed);
    sf::Clock clock;
    Player player;
    World world;

    Game game = Game(window, player, world);
    game.setupLevel();

    Camera camera;
    camera.setupCamera(player.getCamPosition());
    window.setView(camera.viewCam);

    float timeAccumulator = 0.f;
    int frame = 0;
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
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
        
		game.runLevel(dt, frame);
		camera.updateCamera(player.getCamPosition(), dt);
        window.setView(camera.viewCam);

        window.clear();
		game.render(window);
        window.display();
    }
}
