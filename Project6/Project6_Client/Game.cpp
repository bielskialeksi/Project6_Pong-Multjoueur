#include "Game.h"
#include "Global.h"

Game::Game()
{
    ball = new Entity();
    racket_1 = new Entity(50,200,30,100);
    racket_2 = new Entity(WINDOW_WIDTH - 80, 200, 30, 100);
}

void Game::Loop(sf::RenderWindow* window, Client* client)
{
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            client->Disconnect();
            window->close();
        }

    }
    client->Update();
    window->clear(sf::Color::Red);
    window->draw(ball->GetShape());
    window->draw(racket_1->GetShape());
    window->draw(racket_2->GetShape());
    window->display();
}
