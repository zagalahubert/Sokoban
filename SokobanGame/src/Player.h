#pragma once
#include <SFML/Graphics.hpp>

class Player
{
private:
	sf::Vector2i m_pos;
public:
	Player();
	~Player();
	void move(sf::Vector2i vec);
	void setPos(int x, int y);
	const sf::Vector2i& getPos() const;
};

