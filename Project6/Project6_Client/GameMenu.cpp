#include <SFML/Graphics.hpp>

#include "GameMenu.h"

GameMenu::GameMenu()
{
    sf::Font font;
    if (!font.loadFromFile("PingPong.otf"))
        std::cout << "Couldn't load the font.";
    printedPseudo.setFont(font);
    printedPseudo.setPosition(10, 10);
    printedPseudo.setFillColor(sf::Color::Black);
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
        {
            pseudo.push_back(static_cast<char>(event.key.code - sf::Keyboard::A + 'a'));
            printedPseudo.setString(pseudo);
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && pseudo.size() > 0)
        {
            pseudo.pop_back();
            printedPseudo.setString(pseudo);
        }
    }

    client->Update();
    window->clear(sf::Color::Red);
    window->draw(printedPseudo);
    window->display();
}
