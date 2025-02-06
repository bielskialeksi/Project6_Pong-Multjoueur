#include "GameAffichage.h"
#include "Global.h"

GameAffichage::GameAffichage()
{
    ball = new Entity();
    racket_1 = new Entity(50, 200, 30, 100);
    racket_2 = new Entity(WINDOW_WIDTH - 80, 200, 30, 100);

    m_BallVelocityX = BALL_DEFAULT_SPEED;
    m_BallyVelocityY = BALL_DEFAULT_SPEED;

}

void GameAffichage::Loop(sf::RenderWindow* window, Client* client)               //Boucle de jeu
{
    sf::Event event;

    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            client->Disconnect();
            window->close();
        }
        if (event.type == sf::Event::KeyPressed)                //change la state de deplacement en fonction de la touche
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
        if (event.type == sf::Event::KeyReleased)               //arrête la raquette concerné par la touche libérée
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

    client->Update(0, 0);
    window->clear(sf::Color::Green);
    window->draw(ball->GetShape());
    window->draw(racket_1->GetShape());
    window->draw(racket_2->GetShape());
    window->display();
}

void GameAffichage::MoveBall()           //deplace la balle en x & y
{
    sf::Vector2f pos = ball->GetShape().getPosition();
    sf::Vector2f scale = ball->GetShape().getSize();

    if (ball->GetShape().getGlobalBounds().intersects(racket_1->GetShape().getGlobalBounds()) || ball->GetShape().getGlobalBounds().intersects(racket_2->GetShape().getGlobalBounds()))     //Collision avec les raquettes
    {
        m_BallVelocityX > 0 ? m_BallVelocityX += BALL_SPEED_UP : m_BallVelocityX -= BALL_SPEED_UP;              //Augmente la vitesse de la balle en x
        m_BallyVelocityY > 0 ? m_BallyVelocityY += BALL_SPEED_UP / 2 : m_BallyVelocityY -= BALL_SPEED_UP / 2;      //Augmente la vitesse de la balle en y
        m_BallVelocityX *= -1;                                                                  //Change la direction de la balle en x
    }

    if (pos.y + m_BallyVelocityY < 0 || pos.y + m_BallyVelocityY > WINDOW_HEIGHT - scale.y)
        m_BallyVelocityY *= -1;                                                                  //Change la direction de la balle en y

    ball->Move(m_BallVelocityX, m_BallyVelocityY);
    CheckWin();
}

void GameAffichage::MoveRacket()                                                                 //Deplace les raquettes en fonction de leurs states + sans depasser l'écran
{
    //raquette player 1
    sf::Vector2f pos = racket_1->GetShape().getPosition();
    sf::Vector2f scale = racket_1->GetShape().getSize();
    if (racket_1->GetState() == Entity::Movement::Up && pos.y - RACKET_VELOCITY >= 0)
        racket_1->Move(0, -RACKET_VELOCITY);
    if (racket_1->GetState() == Entity::Movement::Down && pos.y + RACKET_VELOCITY <= WINDOW_WIDTH - scale.y * 3)
        racket_1->Move(0, RACKET_VELOCITY);

    //raquette player 2
    pos = racket_2->GetShape().getPosition();
    scale = racket_2->GetShape().getSize();
    if (racket_2->GetState() == Entity::Movement::Up && pos.y - RACKET_VELOCITY >= 0)
        racket_2->Move(0, -RACKET_VELOCITY);
    if (racket_2->GetState() == Entity::Movement::Down && pos.y + RACKET_VELOCITY <= WINDOW_WIDTH - scale.y * 3)
        racket_2->Move(0, RACKET_VELOCITY);
}

void GameAffichage::CheckWin()                                                                   //Teste si la balle va sortir des écran latérale. Si positif : +1 au score du gagnant & reset la pos et vitesse de la balle
{
    sf::Vector2f pos = ball->GetShape().getPosition();
    sf::Vector2f scale = ball->GetShape().getSize();

    //ecran droit = victoire player 1
    if (pos.x + m_BallVelocityX > WINDOW_WIDTH - scale.x)
    {
        m_score.x += 1;
        ball->Reset();
        m_BallVelocityX = BALL_DEFAULT_SPEED;
        m_BallyVelocityY = BALL_DEFAULT_SPEED;
        m_BallVelocityX *= -1;
    }

    //ecran gauche = victoire player 2
    else if (pos.x + m_BallVelocityX < 0)
    {
        m_score.y += 1;
        ball->Reset();
        m_BallVelocityX = BALL_DEFAULT_SPEED;
        m_BallyVelocityY = BALL_DEFAULT_SPEED;
        m_BallVelocityX *= -1;
    }
