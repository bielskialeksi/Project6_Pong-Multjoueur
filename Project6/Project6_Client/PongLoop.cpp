#include "PongLoop.h"

PongLoop::PongLoop()
{
    ball = new Entity();
    racket_1 = new Entity(50, 200, 30, 100);
    racket_2 = new Entity(WINDOW_WIDTH - 80, 200, 30, 100);
}

void PongLoop::Loop(sf::RenderWindow* window, Client* client)
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			client->Disconnect();
			window->close();
		}
        if (event.type == sf::Event::KeyPressed)                //change la state de deplacement en fonction de la touche
        {
            switch (event.key.code)
            {
            case PLAYER_DOWN:             //la raquette descends
                client->Move(0);
                break;
            case PLAYER_UP:               //la raquette monte
                client->Move(1);
                break;
            default:
                break;
            }
        }
	}

    client->Update();
    UpdateElement(client);
    window->clear(sf::Color::Green);
    window->draw(ball->GetShape());
    window->draw(racket_1->GetShape());
    window->draw(racket_2->GetShape());
    window->display();

}

void PongLoop::UpdateElement(Client* client)
{
    ball->Move((float)client->PosBallx, (float)client->PosBally);
    racket_1->Move((float)client->PosPLayer1x, (float)client->PosPLayer1y);
    racket_2->Move((float)client->PosPLayer2x, (float)client->PosPLayer2y);
    m_score = sf::Vector2i(client->score1, client->score2);
}

