#pragma once#include <SFML/Graphics.hpp>
#include "Client.h"
#include "Entity.h"
class GameAffichage
{
public:
	GameAffichage();
	void Loop(sf::RenderWindow* window, Client* client);
	void MoveBall();
	void MoveRacket();
	void CheckWin();

private:
	sf::Vector2i m_score;						//x = score player 1 / y = score player 2
	Entity* ball;								//ball principal
	Entity* racket_1;							//raquette player 1
	Entity* racket_2;							//raquette player 2

	float m_BallVelocityX = 2.0f;				//vitesse de la balle en x
	float m_BallyVelocityY = 2.0f;				//vitesse de la balle en y

	Client* client;

};

