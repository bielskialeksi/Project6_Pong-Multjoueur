#include "Entity.h"
#include "Global.h"

Entity::Entity()
{
	m_pos = { WINDOW_WIDTH / 2, 100 };
	m_scale = { 50, 50 };
	m_color = {255, 255, 255};
	init();

}

Entity::Entity(float posX, float posY, float scaleX, float scaleY)
{
	m_pos = { posX, posY };
	m_scale = { scaleX, scaleY };
	m_color = { 255, 255, 255 };
	init();

}

void Entity::init()
{
	m_shape.setPosition(m_pos);
	m_shape.setSize(m_scale);
	m_shape.setFillColor(m_color);
}

void Entity::Move(float x, float y)
{
	m_pos.x += x;
	m_pos.y += y;

	m_shape.setPosition(m_pos);
}

const sf::RectangleShape Entity::GetShape()
{
	return m_shape;
}
