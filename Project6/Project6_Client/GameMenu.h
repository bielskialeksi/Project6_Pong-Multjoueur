#pragma once
#include "Client.h"

enum Menu {
	Menu_Main = 0,
	Menu_Host,
	Menu_Join,

	Menu_Count
};

enum MainMenu {
	MainMenu_HostBtn = 0,
	MainMenu_JoinBtn,
	// PlayerNumberBtn,

	MainMenu_Count
};

enum HostMenu {
	HostMenu_PseudoField = 0,
	HostMenu_HostBtn,

	HostMenu_Count
};

enum JoinMenu {
	JoinMenu_PseudoField = 0,
	JoinMenu_GameIdField,
	JoinMenu_JoinBtn,

	JoinMenu_Count
};

class GameMenu
{
private:
	std::string fontFilename = "arial.ttf";
	std::string textureFilename = "metal_texture.jpg";

	std::string pseudo;
	sf::Text printedPseudo;
	
	std::string lobbyCode;
	sf::Text printedLobbyCode;

	sf::Font font;
	sf::Texture texture;

	int actualMenuIndex = 0;
	int selectedButtonIndex = 0;

	int maxButtonIndex = MainMenu::MainMenu_Count;

	sf::RectangleShape Btn1;
	sf::RectangleShape Btn2;
	sf::RectangleShape Field1;
	sf::RectangleShape Field2;

	
public:

	GameMenu();

	void LoadFont();
	void LoadTexture();

	void Loop(sf::RenderWindow* window, Client* client);

	void MainMenuEvents(sf::Event event);
	void HostMenuEvents(sf::Event event);
	void JoinMenuEvents(sf::Event event);

	void MainMenuDisplay(sf::RenderWindow* window);
	void HostMenuDisplay(sf::RenderWindow* window);
	void JoinMenuDisplay(sf::RenderWindow* window);
};

