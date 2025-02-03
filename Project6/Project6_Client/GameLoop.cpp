#include <SFML/Graphics.hpp>

#include "GameLoop.h"
#include "Game.h"
#include "GameMenu.h"

#include "Client.h"
GameLoop::GameLoop()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML depuis dossier projet !");
    Client client;
    client.Connect();
    GameMenu menu;


    while (window.isOpen()) {
        if (isPlaying)
            Game();
        else
            menu.Loop(&window, &client);
    }
    client.~Client();
}
