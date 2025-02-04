#include <SFML/Graphics.hpp>

#include "GameMenu.h"

GameMenu::GameMenu()
{
    LoadFont();
    LoadTexture();
}

void GameMenu::LoadFont()
{
    if (!font.loadFromFile(fontFilename))
        std::cout << "Erreur lors du chargement de la police " << fontFilename << std::endl;
    printedPseudo.setFont(font);
    printedPseudo.setPosition(10, 10);
    printedPseudo.setFillColor(sf::Color::Black);
}

void GameMenu::LoadTexture()
{
    if (!texture.loadFromFile(textureFilename))
        std::cout << "Erreur lors du chargement de la texture " << textureFilename << std::endl;
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
            client->Send();
        }
        if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
        {
            pseudo.push_back(static_cast<char>(event.key.code - sf::Keyboard::A + 'a'));
            printedPseudo.setString(pseudo);
            std::cout << pseudo << std::endl;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && pseudo.size() > 0)
        {
            pseudo.pop_back();
            printedPseudo.setString(pseudo);
            std::cout << pseudo << std::endl;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && pseudo.size() > 0)
        {
            //Host
        }
    }

    client->Update();
    window->clear(sf::Color::Red);
    window->draw(printedPseudo);
    window->display();
}
