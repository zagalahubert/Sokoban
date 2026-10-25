#pragma once
#include <SFML/Graphics.hpp>

class Box
{
private:
	sf::Vector2i m_pos;
public:
	Box(int i, int j);
	~Box();
	void move(sf::Vector2i vec);
	const sf::Vector2i& getPos() const;
};

