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

private:
	sf::Vector2i score;				//x = score player 1 / y = score player 2
	Entity* ball;
	Entity* racket_1;
	Entity* racket_2;

	float m_Ballx = 0.1f;
	float m_Bally = 0.1f;

};

