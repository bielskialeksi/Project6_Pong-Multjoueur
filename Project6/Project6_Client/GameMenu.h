#pragma once
#include "Client.h"
class GameMenu
{
private:
	std::string pseudo;
public:
	GameMenu();
	void Loop(sf::RenderWindow* window, Client* client);
};

