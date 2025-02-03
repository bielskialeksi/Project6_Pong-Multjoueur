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


    while (window.isOpen()) {
        if (isPlaying)
            Game();
        else
            GameMenu();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                client.Disconnect();
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                client.Disconnect();
                window.close();
            }

        }
        client.Update();
        window.clear(sf::Color::Red);
        window.display();
    }
    client.~Client();
}
