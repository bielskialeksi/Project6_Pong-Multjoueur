#include <SFML/Graphics.hpp>

#include "GameLoop.h"
#include "Game.h"
#include "GameMenu.h"
#include "Global.h"

#include "Client.h"
GameLoop::GameLoop()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "SFML depuis dossier projet !");
    Client client;
    client.Connect();
    GameMenu menu;
    Game game;

    while (window.isOpen()) {
        if (isPlaying)
            game.Loop(&window, &client);
        else
            menu.Loop(&window, &client);
    }
    client.~Client();
}
