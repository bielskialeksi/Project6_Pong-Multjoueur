#include "Game.h"
#include "Global.h"

Game::Game()
{
    ball = new Entity();
    racket_1 = new Entity(50,200,30,100);
    racket_2 = new Entity(WINDOW_WIDTH - 80, 200, 30, 100);
    m_Ballx = BALL_DEFAULT_SPEED;
    m_Bally = BALL_DEFAULT_SPEED;

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
            switch (event.key.code)
            {
            case PLAYER_1_UP:
                racket_1->ChangeState(Entity::Movement::Up);
                break;
            case PLAYER_1_DOWN:
                racket_1->ChangeState(Entity::Movement::Down);
                break;
            case PLAYER_2_UP:
                racket_2->ChangeState(Entity::Movement::Up);
                break;
            case PLAYER_2_DOWN: 
                racket_2->ChangeState(Entity::Movement::Down);
                break;

            default:
                break;
            }

        }
        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == PLAYER_1_UP || event.key.code == PLAYER_1_DOWN)
            {
                racket_1->ChangeState(Entity::Movement::Neutral);
            }
            if (event.key.code == PLAYER_2_UP || event.key.code == PLAYER_2_DOWN)
            {
                racket_2->ChangeState(Entity::Movement::Neutral);
            }
        }


    }

    MoveBall();
    MoveRacket();

    client->Update(0,0);
    window->clear(sf::Color::Green);
    window->draw(ball->GetShape());
    window->draw(racket_1->GetShape());
    window->draw(racket_2->GetShape());
    window->display();
}

void Game::MoveBall()
{
    sf::Vector2f pos = ball->GetShape().getPosition();
    sf::Vector2f scale = ball->GetShape().getSize();

    if (ball->GetShape().getGlobalBounds().intersects(racket_1->GetShape().getGlobalBounds()) || ball->GetShape().getGlobalBounds().intersects(racket_2->GetShape().getGlobalBounds()))
    {
        m_Ballx > 0 ? m_Ballx += BALL_SPEED_UP : m_Ballx -= BALL_SPEED_UP;
        m_Ballx *= -1;
    }


    
    if (pos.x + m_Ballx > WINDOW_WIDTH - scale.x || pos.x + m_Ballx < 0)
        m_Ballx *= -1;
    

    if (pos.y + m_Bally < 0 || pos.y + m_Bally > WINDOW_HEIGHT - scale.y)
        m_Bally *= -1;

    ball->Move(m_Ballx, m_Bally);
    CheckWin();
}

void Game::MoveRacket()
{
    sf::Vector2f pos = racket_1->GetShape().getPosition();
    sf::Vector2f scale = racket_1->GetShape().getSize();
    if (racket_1->GetState() == Entity::Movement::Up && pos.y - RACKET_VELOCITY >= 0)
        racket_1->Move(0, -RACKET_VELOCITY);
    if (racket_1->GetState() == Entity::Movement::Down && pos.y + RACKET_VELOCITY <= WINDOW_WIDTH / 2+ scale.y)
        racket_1->Move(0, RACKET_VELOCITY);

    pos = racket_2->GetShape().getPosition();
    scale = racket_2->GetShape().getSize();
    if (racket_2->GetState() == Entity::Movement::Up && pos.y - RACKET_VELOCITY >= 0)
        racket_2->Move(0, -RACKET_VELOCITY);
    if (racket_2->GetState() == Entity::Movement::Down && pos.y + RACKET_VELOCITY <= WINDOW_WIDTH + scale.y)
        racket_2->Move(0, RACKET_VELOCITY);
}

void Game::CheckWin()
{
    sf::Vector2f pos = ball->GetShape().getPosition();
    sf::Vector2f scale = ball->GetShape().getSize();

    if (pos.x + m_Ballx > WINDOW_WIDTH - scale.x)
    {
        m_score.x += 1;
        ball->Reset();
        m_Ballx = BALL_DEFAULT_SPEED;
        m_Ballx *= -1;
    }
    else if (pos.x + m_Ballx < 0)
    {
        m_score.y += 1;
        ball->Reset();
        m_Ballx = BALL_DEFAULT_SPEED;
        m_Ballx *= -1;
    }
}
