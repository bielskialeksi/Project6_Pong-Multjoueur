#include <SFML/Graphics.hpp>

#include "GameLoop.h"
#include "GameMenu.h"
#include "PongLoop.h"
#include "Global.h"

#include "Client.h"
GameLoop::GameLoop()
{
    Init();
    Loop();
}

void GameLoop::Init()
{
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML depuis dossier projet !");
    window->setFramerateLimit(60);      //fps limit to 60
}

void GameLoop::Loop()
{
    Client client;
    client.Connect();
    GameMenu menu;
    PongLoop pong;

    while (window->isOpen()) {
        if (isPlaying) {
            pong.Loop(window, &client);
        }
        else
            menu.Loop(window, &client);
    }
    client.~Client();
}
