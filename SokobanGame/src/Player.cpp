#include "Player.h"

Player::Player()
	: m_pos()
{
}

Player::~Player()
{
}

void Player::move(sf::Vector2i vec)
{
	m_pos.x += vec.x;
	m_pos.y += vec.y;
}

void Player::setPos(int x, int y)
{
	m_pos.x = x;
	m_pos.y = y;
}

const sf::Vector2i& Player::getPos() const
{
	return m_pos;
}