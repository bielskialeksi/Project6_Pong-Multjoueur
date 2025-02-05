#pragma once
class GameLoop
{
private:
	bool isPlaying = true;
public:
	GameLoop();

	void Init();
	void Loop();

	sf::RenderWindow* window;
};

