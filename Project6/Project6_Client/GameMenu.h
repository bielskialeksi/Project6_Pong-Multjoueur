#pragma once
#include "Client.h"
class GameMenu
{
private:
	std::string fontFilename = "arial.ttf";
	std::string textureFilename = "./metal_texture.jpg";

	std::string pseudo;
	sf::Text printedPseudo;
	sf::Font font;
	

	sf::Texture texture;

	
public:
	GameMenu();
	void LoadFont();
	void LoadTexture();
	void Loop(sf::RenderWindow* window, Client* client);
};

