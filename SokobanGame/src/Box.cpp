#include "Box.h"

Box::Box(int i, int j)
	: m_pos(i,j)
{
}

Box::~Box()
{
}

void Box::move(sf::Vector2i vec)
{
	m_pos.x += vec.x;
	m_pos.y += vec.y;
}

const sf::Vector2i& Box::getPos() const
{
	return m_pos;
}
