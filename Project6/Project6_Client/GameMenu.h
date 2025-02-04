#pragma once
#include "Client.h"

enum Menu {
	Main = 0,
	Host,
	Join
};

class GameMenu
{
private:
	std::string fontFilename = "arial.ttf";
	std::string textureFilename = "./metal_texture.jpg";

	std::string pseudo;
	sf::Text printedPseudo;
	sf::Font font;
	

	sf::Texture texture;

	int actualMenuIndex = 0;
	int selectedButtonIndex = 0;

	
public:
	GameMenu();
	void LoadFont();
	void LoadTexture();
	void Loop(sf::RenderWindow* window, Client* client);
};

