#pragma once
#include <SFML/Graphics.hpp>

class Entity
{
public:

	enum Movement			//state pour le deplacement, utilisé uniquement pour les raquettes
	{
		Neutral = 0,
		Up,
		Down
	};

	//2 constructeurs
	Entity();
	Entity(float, float, float, float);

	void init();
	void Move(float, float);
	void Reset();
	const sf::RectangleShape GetShape();
	void ChangeState(Movement);
	Entity::Movement GetState();

private:

	Movement m_moveState = Neutral;		//state de l'objet actuelle
	sf::RectangleShape m_shape;			
	sf::Vector2f m_pos;					//pos de m_shape
	sf::Vector2f m_scale;				//taille de m_shape
	sf::Color m_color;					//couleur de m_shape
};

