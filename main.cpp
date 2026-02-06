#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "main.h"

#include "Npc/Npc.h"

const float animationFPS = 12.f;
const float frameDuration = 1.f / animationFPS;

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    sf::Clock clock;
    Player player;
    World world;
    Camera camera;

    Npc goblin(NpcType::Goblin, { 3, 3 }, { 7, 3 });
    Npc goblin2(NpcType::Goblin, { 5, 4 }, { 5, 7 });
    Npc goblin3(NpcType::Goblin, { 4, 7 }, { 4, 7 });

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
        goblin.update(dt, frame);
        goblin2.update(dt, frame);
        //goblin3.update(dt, frame);
        goblin3.faceDirection(LookLeft);
        
		camera.updateCamera(player.getCamPosition(), dt);
        window.setView(camera.viewCam);
        window.clear();

        world.renderTileMaps(window, Floor);
        world.renderTileMaps(window, Walls);
        world.renderTileMaps(window, Objects);

        player.draw(window);
        goblin.draw(window);
        goblin2.draw(window);
        goblin3.draw(window);

        window.display();
    }
}
