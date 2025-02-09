#pragma once

#include "Client.h"
class GameLoop
{
private:
	
public:
	GameLoop();

	void Init();
	void Loop();

	sf::RenderWindow* window;
	
	bool isPlaying = false;
	bool hasPlayed = false;
};

