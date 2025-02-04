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

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            client->Disconnect();
            window->close();
        }
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Z)
            {
                racket_1->Move(0, -10);
            }
            if (event.key.code == sf::Keyboard::S)
            {
                racket_1->Move(0, 10);
            }
            if (event.key.code == sf::Keyboard::Up)
            {
                racket_2->Move(0, -10);
            }
            if (event.key.code == sf::Keyboard::Down)
            {
                racket_2->Move(0, 10);
            }

        }


    }
    MoveBall();
    client->Update(0,0);
    window->clear(sf::Color::Red);
    window->draw(ball->GetShape());
    window->draw(racket_1->GetShape());
    window->draw(racket_2->GetShape());
    window->display();
}

void Game::MoveBall()
{
    sf::Vector2f pos = ball->GetShape().getPosition();
    sf::Vector2f scale = ball->GetShape().getSize();

    if (pos.x + m_Ballx > WINDOW_WIDTH - scale.x || pos.x + m_Ballx < 0)
        m_Ballx *= -1;

    if (pos.y + m_Bally < 0 || pos.y + m_Bally > WINDOW_HEIGHT - scale.y)
        m_Bally *= -1;

    ball->Move(m_Ballx, m_Bally);
}
