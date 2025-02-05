#include <SFML/Graphics.hpp>

#include "GameMenu.h"
#include "Global.h"

GameMenu::GameMenu()
{
    LoadFont();
    LoadTexture();
}

void GameMenu::LoadFont()
{
    if (!font.loadFromFile(fontFilename))
        std::cout << "Erreur lors du chargement de la police " << fontFilename << std::endl;
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
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
        {
            if (selectedButtonIndex + 1 == maxButtonIndex)
                selectedButtonIndex = 0;
            else
                selectedButtonIndex += 1;

            std::cout << selectedButtonIndex << std::endl;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
        {
            if (selectedButtonIndex - 1 == -1)
                selectedButtonIndex = maxButtonIndex - 1;
            else
                selectedButtonIndex -= 1;
            std::cout << selectedButtonIndex << std::endl;
        }
        switch (actualMenuIndex) {
        case Menu::Menu_Main :
            MainMenuEvents(event);
            break;
        case Menu::Menu_Host :
            HostMenuEvents(event);
            break;
        case Menu::Menu_Join :
            JoinMenuEvents(event);
            break;
        default :
            break;
        }
    }

    client->Update(0, 0);
    switch (actualMenuIndex) {
    case Menu::Menu_Main:
        MainMenuDisplay(window);
        break;
    case Menu::Menu_Host:
        HostMenuDisplay(window);
        break;
    case Menu::Menu_Join:
        JoinMenuDisplay(window);
        break;
    default:
        break;
    }
    window->display();
}

void GameMenu::MainMenuEvents(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        if (selectedButtonIndex == MainMenu::MainMenu_HostBtn)
        {
            actualMenuIndex = Menu::Menu_Host;
            maxButtonIndex = HostMenu::HostMenu_Count;
            std::cout << "Host" << std::endl;
        }
        if (selectedButtonIndex == MainMenu::MainMenu_JoinBtn)
        {
            actualMenuIndex = Menu::Menu_Join;
            maxButtonIndex = JoinMenu::JoinMenu_Count;
            std::cout << "Join" << std::endl;
        }
    }
}

void GameMenu::HostMenuEvents(sf::Event event)
{
    if (selectedButtonIndex == HostMenu::HostMenu_PseudoField)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
        {
            pseudo.push_back(static_cast<char>(event.key.code - sf::Keyboard::A + 'a'));
            std::cout << pseudo << std::endl;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && pseudo.size() > 0)
        {
            pseudo.pop_back();
            std::cout << pseudo << std::endl;
        }
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        if (selectedButtonIndex == HostMenu::HostMenu_HostBtn && pseudo.size() > 0)
        {
            //Host
            client->Send("host");
        }
    }
}

void GameMenu::JoinMenuEvents(sf::Event event)
{
    if (selectedButtonIndex == JoinMenu::JoinMenu_PseudoField)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
        {
            pseudo.push_back(static_cast<char>(event.key.code - sf::Keyboard::A + 'a'));
            std::cout << pseudo << std::endl;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && pseudo.size() > 0)
        {
            pseudo.pop_back();
            std::cout << pseudo << std::endl;
        }
    }
    if (selectedButtonIndex == JoinMenu::JoinMenu_GameIdField)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::Numpad0 && event.key.code <= sf::Keyboard::Numpad9)
        {
            lobbyCode.push_back(static_cast<char>(event.key.code - sf::Keyboard::Numpad0 + '0'));
            std::cout << lobbyCode << std::endl;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && lobbyCode.size() > 0)
        {
            lobbyCode.pop_back();
            std::cout << lobbyCode << std::endl;
        }
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        if (selectedButtonIndex == JoinMenu::JoinMenu_JoinBtn && pseudo.size() > 0)
        {
            std::cout << "Join" << std::endl;
            // Join
        }
    }
}

void GameMenu::MainMenuDisplay(sf::RenderWindow* window)
{
    window->clear(sf::Color::Black);

    InitRect(window, BUTTON_SIZE, { 10, 10 }, MainMenu::MainMenu_HostBtn);
    InitRect(window, BUTTON_SIZE, { 10, 80 }, MainMenu::MainMenu_JoinBtn);

    InitText(window, "Host", { 20, 20 });
    InitText(window, "Join", { 20, 90 });
}

void GameMenu::HostMenuDisplay(sf::RenderWindow* window)
{
    window->clear(sf::Color::Black);
    
    InitRect(window, FIELD_SIZE, { 10, 10 }, HostMenu::HostMenu_PseudoField);
    InitRect(window, BUTTON_SIZE, { 10, 80 }, HostMenu::HostMenu_HostBtn);

    InitText(window, pseudo, { 20, 10 });
    InitText(window, "Host", {20, 90});
}

void GameMenu::JoinMenuDisplay(sf::RenderWindow* window)
{
    window->clear(sf::Color::Black);
    
    InitRect(window, FIELD_SIZE, { 10, 10 }, JoinMenu::JoinMenu_PseudoField);
    InitRect(window, FIELD_SIZE, { 10, 50 }, JoinMenu::JoinMenu_GameIdField);
    InitRect(window, BUTTON_SIZE, { 10, 90 }, JoinMenu::JoinMenu_JoinBtn);

    InitText(window, pseudo, { 20, 10 });
    InitText(window, lobbyCode, { 20, 50 });
    InitText(window, "Join", {20, 100});
}

void GameMenu::InitRect(sf::RenderWindow* window, sf::Vector2f size, sf::Vector2f pos, int id)
{
    sf::RectangleShape rect;

    rect.setSize(size);
    rect.setPosition(pos);
    if (id == selectedButtonIndex)
        rect.setFillColor(BUTTON_HOVER_COLOR);
    else
        rect.setFillColor(sf::Color::White);

    window->draw(rect);
}

void GameMenu::InitText(sf::RenderWindow* window, std::string str, sf::Vector2f pos, sf::Color fillColor)
{
    sf::Text text(str, font);

    text.setPosition(pos);
    text.setFillColor(fillColor);

    window->draw(text);
}
