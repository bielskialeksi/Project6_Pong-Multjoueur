#pragma once
#include <SFML/Graphics.hpp>
#include "Client.h"
#include "Entity.h"
class Game
{
public:
	Game();
	void Loop(sf::RenderWindow* window, Client* client);

private:
	sf::Vector2i score;
	Entity* ball;
	Entity* racket_1;
	Entity* racket_2;
};

