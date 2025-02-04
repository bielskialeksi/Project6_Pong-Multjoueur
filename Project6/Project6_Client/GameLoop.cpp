#include <SFML/Graphics.hpp>

#include "GameLoop.h"
#include "Game.h"
#include "GameMenu.h"
#include "Global.h"

#include "Client.h"
GameLoop::GameLoop()
{
    Init();
    Loop();
}

void GameLoop::Init()
{
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "SFML depuis dossier projet !");
}

void GameLoop::Loop()
{
    Client client;
    client.Connect();
    GameMenu menu;
    Game game;

    while (window->isOpen()) {
        if (isPlaying) {

            game.Loop(window, &client);
        }
        else
            menu.Loop(window, &client);
    }
    client.~Client();
}
