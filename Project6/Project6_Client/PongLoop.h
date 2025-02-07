#pragma once
#include "SFML/Graphics.hpp"
#include "Entity.h"
#include "Global.h"
#include "Client.h"

class PongLoop
{
	public:
		PongLoop();
		void Loop(sf::RenderWindow*, Client*);

	private:
		sf::Vector2i m_score;						//x = score player 1 / y = score player 2
		Entity* ball;								//ball principal
		Entity* racket_1;							//raquette player 1
		Entity* racket_2;							//raquette player 2
};

