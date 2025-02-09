#include <SFML/Graphics.hpp>

#include "GameLoop.h"
#include "GameMenu.h"
#include "PongLoop.h"
#include "Global.h"

GameLoop::GameLoop()
{
    Init();
    Loop();
}

void GameLoop::Init()
{
    client.Connect();
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML depuis dossier projet !");
    window->setFramerateLimit(60);      //fps limit to 60
}

void GameLoop::Loop()
{
    if (client.Connect() == 1) return;
    GameMenu menu;
    PongLoop pong;

    while (window->isOpen()) {
        if (client.IsConected()) {
            if (!hasPlayed)
                hasPlayed = true;
            pong.Loop(window, &client);
        }
        else
        {
            if (hasPlayed) {
                menu.actualMenuIndex = Menu::Menu_Main;
                hasPlayed = false;
            }
            menu.Loop(window, &client);

        }
    }
}
