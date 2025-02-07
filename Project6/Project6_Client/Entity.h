#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:
	//2 constructeurs
	Entity();
	Entity(float, float, float, float);

	void init();
	void Move(float, float);
	const sf::RectangleShape GetShape();

private:
	sf::RectangleShape m_shape;			
	sf::Vector2f m_pos;					//pos de m_shape
	sf::Vector2f m_scale;				//taille de m_shape
	sf::Color m_color;					//couleur de m_shape
};

