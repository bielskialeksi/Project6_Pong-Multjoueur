#include "PongLoop.h"
#include <sstream> // Pour ostringstream

PongLoop::PongLoop()
{
    ball = new Entity();
    racket_1 = new Entity(50, 200, 30, 100);
    racket_2 = new Entity(WINDOW_WIDTH - 80, 200, 30, 100);
    sf::Vector2i m_score = { 0,0 };
    LoadFont();
    UpdateText();
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
    window->draw(m_scoreText);
    window->display();

}

void PongLoop::UpdateElement(Client* client)
{
    ball->Move((float)client->PosBallx, (float)client->PosBally);
    racket_1->Move((float)client->PosPLayer1x, (float)client->PosPLayer1y);
    racket_2->Move((float)client->PosPLayer2x, (float)client->PosPLayer2y);
    m_score = sf::Vector2i((int)client->score1,(int) client->score2);
    UpdateText();
}

void PongLoop::LoadFont()
{
    if (!m_font.loadFromFile("PingPong.otf"))
        std::cout << "Erreur lors du chargement de la police PingPong.otf " << std::endl;
    else
        m_scoreText.setFont(m_font);

    m_scoreText.setCharacterSize(60);
    m_scoreText.setPosition({WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT / 3});
}

void PongLoop::UpdateText()
{
    m_scoreText.setString(vector2fToString(m_score));
}

sf::String PongLoop::vector2fToString(const sf::Vector2i vector)
{
    std::ostringstream oss;  
    oss << vector.x << " : " << vector.y;               // Convertir les composantes du vecteur en chaîne

    return sf::String(oss.str());                       // Retourner un sf::String
}


