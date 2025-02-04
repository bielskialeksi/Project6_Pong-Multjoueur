#include <SFML/Graphics.hpp>

#include "GameMenu.h"

GameMenu::GameMenu()
{

}

void GameMenu::Loop(sf::RenderWindow* window, Client* client)
{
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            client->Disconnect();
            window->close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            client->Disconnect();
            window->close();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
            std::cout << static_cast<char>(event.key.code) << std::endl;
    }
    client->Update(0,0);
    window->clear(sf::Color::Red);
    window->display();
}
