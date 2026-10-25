#pragma once
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Box.h"

enum Tile {
	NONE,
	WALL,
	FLOOR,
	PARK
};

class Sokoban : public sf::Drawable
{
private:
	void movePlayer(const sf::Vector2i& direction);
	void loadMap(const std::string&);
	void updateTime();
	void checkWin();
public:
	Sokoban(std::string mapFileName, std::string fontFileName, const sf::Vector2u& size);
	~Sokoban();
	void handleInputs();
	void setDrawParameters();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void setSize(const sf::Vector2u& size);
	void restartGame();
	void onUpdate();
private:
	bool m_gameWon;
	int m_timeAsSeconds;
	std::string m_mapFileName;
	Player m_player;
	sf::Clock m_clock;
	sf::Font m_font;
	sf::Vector2u m_windowSize;
	sf::Vector2f m_tileSize, m_shift;
	std::vector<Box> m_boxes;
	std::vector<std::vector<Tile>> m_map;
	std::vector<sf::Texture> m_textures;
};

