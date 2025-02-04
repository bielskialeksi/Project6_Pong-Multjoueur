#pragma once
class GameLoop
{
private:
	bool isPlaying = false;
public:
	GameLoop();

	void Init();
	void Loop();

	sf::RenderWindow* window;
};

