#pragma once

#include "Client.h"
class GameLoop
{
private:
	Client client;
public:
	GameLoop();

	void Init();
	void Loop();

	sf::RenderWindow* window;
	
	bool isPlaying = false;
	bool hasPlayed = false;
};

