#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:

	enum Movement
	{
		Neutral = 0,
		Up,
		Down
	};


	Entity();
	Entity(float, float, float, float);

	void init();
	void Move(float, float);
	void Reset();
	const sf::RectangleShape GetShape();
	void ChangeState(Movement);
	Entity::Movement GetState();

private:

	Movement m_moveState = Neutral;
	sf::RectangleShape m_shape;
	sf::Vector2f m_pos;
	sf::Vector2f m_scale;
	sf::Color m_color;
};

