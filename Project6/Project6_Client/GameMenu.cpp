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
            HostMenuEvents(event, client);
            break;
        case Menu::Menu_Join :
            JoinMenuEvents(event, client);
            break;
        default :
            break;
        }
    }

    client->Update(0, 0);

    window->clear({ 41, 41, 41 });
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
    case Menu::Menu_WaitingForOponent:
        WaitingForOpponentDisplay(window, client);
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

void GameMenu::HostMenuEvents(sf::Event event, Client* client)
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
            client->Host(pseudo);
            actualMenuIndex = Menu::Menu_WaitingForOponent;
            clock.restart();
        }
    }
}

void GameMenu::JoinMenuEvents(sf::Event event, Client* client)
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
            client->Join(pseudo, lobbyCode);
        }
    }
}

void GameMenu::MainMenuDisplay(sf::RenderWindow* window)
{
    InitUIBox(window, BUTTON_SIZE, { 10, 10 }, "Host", MainMenu::MainMenu_HostBtn);
    InitUIBox(window, BUTTON_SIZE, { 10, 80 }, "Join", MainMenu::MainMenu_JoinBtn);

    InitText(window, "Up/Down to navigate", { WINDOW_WIDTH - 10, WINDOW_HEIGHT - 30 }, sf::Color::White, { 1, 1 }, 10);
}

void GameMenu::HostMenuDisplay(sf::RenderWindow* window)
{
    InitUIBox(window, FIELD_SIZE, { 10, 10 }, pseudo, HostMenu::HostMenu_PseudoField);
    InitUIBox(window, BUTTON_SIZE, { 10, 80 }, "Host", HostMenu::HostMenu_HostBtn);

    InitText(window, "Up/Down to navigate", { WINDOW_WIDTH - 10, WINDOW_HEIGHT - 30 }, sf::Color::White, { 1, 1 }, 10);
}

void GameMenu::JoinMenuDisplay(sf::RenderWindow* window)
{
    InitUIBox(window, FIELD_SIZE, { 10, 10 }, pseudo, JoinMenu::JoinMenu_PseudoField);
    InitUIBox(window, FIELD_SIZE, { 10, 50 }, lobbyCode, JoinMenu::JoinMenu_GameIdField);
    InitUIBox(window, BUTTON_SIZE, { 10, 90 }, "Join", JoinMenu::JoinMenu_JoinBtn);

    InitText(window, "Up/Down to navigate", { WINDOW_WIDTH - 10, WINDOW_HEIGHT - 30 }, sf::Color::White, { 1, 1 }, 10);
}

void GameMenu::WaitingForOpponentDisplay(sf::RenderWindow* window, Client* client)
{
    std::string waitDots = ".";
    int elapsedTime = static_cast<int>(clock.getElapsedTime().asSeconds());
    if (elapsedTime % 3 < 1)
        waitDots = ".";
    if ((elapsedTime + 2) % 3 < 1)
        waitDots = "..";
    if ((elapsedTime + 1) % 3 < 1)
        waitDots = "...";

    InitText(window, "Waiting for Opponent" + waitDots, { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, sf::Color::White, { 0.5f, 0.5f });
    InitText(window, "Lobby ID "+ client->GetCodeClient(), { WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10 }, sf::Color::White, { 1.f, 1.f }, 18);
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

void GameMenu::InitText(sf::RenderWindow* window, std::string str, sf::Vector2f pos, sf::Color fillColor, sf::Vector2f origin, int fontSize)
{
    sf::Text text(str, font);

    text.setCharacterSize(fontSize);

    sf::Vector2f textSize = { text.getGlobalBounds().width, text.getGlobalBounds().height };
    text.setOrigin({ textSize.x * origin.x, textSize.y * origin.y });

    text.setPosition(pos);
    text.setFillColor(fillColor);

    window->draw(text);
}

void GameMenu::InitUIBox(sf::RenderWindow* window, sf::Vector2f size, sf::Vector2f pos, std::string str, int id)
{
    InitRect(window, size, pos, id);

    sf::Vector2f btnSize = BUTTON_SIZE;
    if (size == btnSize)
    {
        InitText(window, str, { pos.x + size.x * 0.5f, pos.y + size.y * 0.5f }, sf::Color::Black, { 0.5f, 0.5f });
        if (selectedButtonIndex == id)
            InitText(window, "Enter to confirm", { WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10 }, sf::Color::White, { 1, 1 }, 10);
    }
    else
    {
        InitText(window, str, { pos.x + 10, pos.y + size.y * 0.5f }, sf::Color::Black, { 0, 0.5f });
        if (selectedButtonIndex == id)
            InitText(window, "Keyboard to write", { WINDOW_WIDTH - 10, WINDOW_HEIGHT - 10 }, sf::Color::White, { 1, 1 }, 10);
    }
}
