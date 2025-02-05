#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:
	Entity();
	Entity(float, float, float, float);

	void init();
	void Move(float, float);
	const sf::RectangleShape GetShape();

private:
	sf::RectangleShape m_shape;
	sf::Vector2f m_pos;
	sf::Vector2f m_scale;
	sf::Color m_color;
};

