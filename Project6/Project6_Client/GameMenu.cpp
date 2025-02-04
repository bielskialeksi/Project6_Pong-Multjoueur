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

    printedLobbyCode.setFont(font);
    printedLobbyCode.setPosition(10, 10);
    printedLobbyCode.setFillColor(sf::Color::Black);
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
            printedPseudo.setString(pseudo);
            std::cout << pseudo << std::endl;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && pseudo.size() > 0)
        {
            pseudo.pop_back();
            printedPseudo.setString(pseudo);
            std::cout << pseudo << std::endl;
        }
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        if (selectedButtonIndex == HostMenu::HostMenu_HostBtn && pseudo.size() > 0)
        {
            std::cout << "Host" << std::endl;
            //Host
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
            printedPseudo.setString(pseudo);
            std::cout << pseudo << std::endl;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && pseudo.size() > 0)
        {
            pseudo.pop_back();
            printedPseudo.setString(pseudo);
            std::cout << pseudo << std::endl;
        }
    }
    if (selectedButtonIndex == JoinMenu::JoinMenu_GameIdField)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code >= sf::Keyboard::Numpad0 && event.key.code <= sf::Keyboard::Numpad9)
        {
            lobbyCode.push_back(static_cast<char>(event.key.code - sf::Keyboard::Numpad0 + '0'));
            printedLobbyCode.setString(lobbyCode);
            std::cout << lobbyCode << std::endl;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace && lobbyCode.size() > 0)
        {
            lobbyCode.pop_back();
            printedLobbyCode.setString(lobbyCode);
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
    Btn1.setSize({ 800 / 10, 600 / 10 });
    Btn1.setPosition({ 10, 10 });
    Btn1.setFillColor(sf::Color::White);

    Btn2.setSize({ 800 / 10, 600 / 10 });
    Btn2.setPosition({ 10, 80 });
    Btn2.setFillColor(sf::Color::White);

    switch (selectedButtonIndex) {
    case MainMenu::MainMenu_HostBtn :
        Btn1.setFillColor({ 255, 255, 120 });
        break;
    case MainMenu::MainMenu_JoinBtn :
        Btn2.setFillColor({ 255, 255, 120 });
        break;
    default :
        break;
    }

    sf::Text host("Host", font);
    host.setPosition({ 20, 20 });
    host.setFillColor(sf::Color::Black);

    sf::Text join("Join", font);
    join.setPosition({ 20, 90 });
    join.setFillColor(sf::Color::Black);

    window->clear(sf::Color::Black);
    window->draw(Btn1);
    window->draw(Btn2);
    window->draw(host);
    window->draw(join);
}

void GameMenu::HostMenuDisplay(sf::RenderWindow* window)
{
    Field1.setSize({ 800 / 4, 600 / 20 });
    Field1.setPosition({ 10, 10 });
    Field1.setFillColor(sf::Color::White);

    Btn1.setSize({ 800 / 10, 600 / 10 });
    Btn1.setPosition({ 10, 80 });
    Btn1.setFillColor(sf::Color::White);

    printedPseudo.setFont(font);
    printedPseudo.setPosition(20, 10);
    printedPseudo.setFillColor(sf::Color::Black);

    switch (selectedButtonIndex) {
    case HostMenu::HostMenu_PseudoField :
        Field1.setFillColor({ 255, 255, 120 });
        break;
    case HostMenu::HostMenu_HostBtn :
        Btn1.setFillColor({ 255, 255, 120 });
        break;
    default:
        break;
    }

    sf::Text host("Host", font);
    host.setPosition({ 20, 90 });
    host.setFillColor(sf::Color::Black);

    window->clear(sf::Color::Black);
    window->draw(Btn1);
    window->draw(Field1);
    window->draw(host);
    window->draw(printedPseudo);
}

void GameMenu::JoinMenuDisplay(sf::RenderWindow* window)
{
    Field1.setSize({ 800 / 4, 600 / 20 });
    Field1.setPosition({ 10, 10 });
    Field1.setFillColor(sf::Color::White);

    Field2.setSize({ 800 / 4, 600 / 20 });
    Field2.setPosition({ 10, 50 });
    Field2.setFillColor(sf::Color::White);

    Btn1.setSize({ 800 / 10, 600 / 10 });
    Btn1.setPosition({ 10, 90 });
    Btn1.setFillColor(sf::Color::White);

    printedPseudo.setFont(font);
    printedPseudo.setPosition(20, 10);
    printedPseudo.setFillColor(sf::Color::Black);

    printedLobbyCode.setFont(font);
    printedLobbyCode.setPosition(20, 50);
    printedLobbyCode.setFillColor(sf::Color::Black);

    sf::Text host("Join", font);
    host.setPosition({ 20, 100 });
    host.setFillColor(sf::Color::Black);

    switch (selectedButtonIndex) {
    case JoinMenu::JoinMenu_PseudoField :
        Field1.setFillColor({ 255, 255, 120 });
        break;
    case JoinMenu::JoinMenu_GameIdField:
        Field2.setFillColor({ 255, 255, 120 });
        break;
    case JoinMenu::JoinMenu_JoinBtn:
        Btn1.setFillColor({ 255, 255, 120 });
        break;
    default:
        break;
    }

    window->clear(sf::Color::Black);
    window->draw(Field1);
    window->draw(Field2);
    window->draw(Btn1);
    window->draw(host);
    window->draw(printedPseudo);
    window->draw(printedLobbyCode);
}
