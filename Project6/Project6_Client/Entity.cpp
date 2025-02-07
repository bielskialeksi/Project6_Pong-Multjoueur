#include "Entity.h"
#include "Global.h"

Entity::Entity()														//constructeur par default : pour la balle
{
	m_pos = { WINDOW_WIDTH / 2, 100 };
	m_scale = { 50, 50 };
	m_color = {255, 255, 255};	//blanc
	init();

}

Entity::Entity(float posX, float posY, float scaleX, float scaleY)		//constructeur avec argument : pour les raquettes
{
	m_pos = { posX, posY };
	m_scale = { scaleX, scaleY };
	m_color = { 255, 255, 255 };	//blanc
	init();

}

void Entity::init()														//initialise les propriétées de m_shape
{
	m_shape.setPosition(m_pos);
	m_shape.setSize(m_scale);
	m_shape.setFillColor(m_color);
}

void Entity::Move(float x, float y)										//déplace m_shape en ajoutant les arguments
{
	m_pos.x = x;
	m_pos.y = y;

	m_shape.setPosition(m_pos);
}

void Entity::Reset()													//reset des positions de la balle
{
	m_pos = {WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2};
	m_shape.setPosition(m_pos);
}


const sf::RectangleShape Entity::GetShape()								//retourne m_shape
{
	return m_shape;
}

void Entity::ChangeState(Movement state)								//change la state de l'objet
{
	m_moveState = state;
}

Entity::Movement Entity::GetState()										//recupère la state actuelle de l'objet
{
	return m_moveState;
}
