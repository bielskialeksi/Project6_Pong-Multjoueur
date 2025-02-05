#pragma once
#include <SFML/Graphics.hpp>
#include "Client.h"
#include "Entity.h"
class Game
{
public:
	Game();
	void Loop(sf::RenderWindow* window, Client* client);
	void MoveBall();
	void CheckWin();

private:
	sf::Vector2i m_score;				//x = score player 1 / y = score player 2
	Entity* ball;
	Entity* racket_1;
	Entity* racket_2;

	float m_Ballx = 3.0f;
	float m_Bally = 3.0f;

};

