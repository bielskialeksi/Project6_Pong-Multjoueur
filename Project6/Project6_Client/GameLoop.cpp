#include <SFML/Graphics.hpp>

#include "GameLoop.h"
#include "Game.h"
#include "GameMenu.h"

#include "Client.h"
GameLoop::GameLoop()
{
    Init();
    Loop();
}

void GameLoop::Init()
{
    window = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML depuis dossier projet !");
}

void GameLoop::Loop()
{
    Client client;
    client.Connect();
    GameMenu menu;
    

    while (window->isOpen()) {
        if (isPlaying) {

            //Game();
        }
        else
            menu.Loop(window, &client);
    }
    client.~Client();
}
