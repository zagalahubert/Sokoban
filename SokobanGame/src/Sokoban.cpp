#include "Sokoban.h"

#include <iostream>
#include <fstream>

constexpr unsigned int TEXTURES = 10;
constexpr unsigned int VICTORY_TEXTURES = 3;

//private
void Sokoban::movePlayer(const sf::Vector2i& direction)
{
    // future player position
    sf::Vector2i fPos = m_player.getPos() + direction;
    bool boxAhead = false;
    int boxIdx = 0;

    // check if the box is on the player's way, boxIdx ends up on its index
    for (; boxIdx < m_boxes.size(); boxIdx++)
    {
        const Box& box = m_boxes[boxIdx];
        const sf::Vector2i& boxPos = box.getPos();

        if (boxPos == fPos) 
        {
            boxAhead = true;
            break;
        }
    }

    // if box found
    if (boxAhead)
    {
        //check the area behind the box for it to be pushed
        const sf::Vector2i behindBox = fPos + direction;
        if (m_map[behindBox.y][behindBox.x] != Tile::WALL)
        {
            // loop through boxes to see if another box is behind
            for (int i = 0; i < m_boxes.size(); i++)
            {
                if (i == boxIdx) continue;

                const Box& box = m_boxes[i];
                const sf::Vector2i& boxPos = box.getPos();

                // if another box is behind, not able to move
                if (boxPos == behindBox) 
                {
                    return;
                }
            }

            // finally move both the player and the box
            m_boxes[boxIdx].move(direction);
            m_player.move(direction);
        } // in case of wall behind the box
        else
            return;
    }

    // if there is no box and no wall ahead move the player
    if (!boxAhead && (m_map[fPos.y][fPos.x] != Tile::WALL))
        m_player.move(direction);
}

void Sokoban::loadMap(const std::string& mapFileName)
{
    std::ifstream in(mapFileName);

    if (in.fail())
        throw std::exception("Wrong file name!");
    else
        std::cout << "Map: " << mapFileName << " loaded succesfully." << std::endl;
    
    std::string str;
    std::vector<Tile> temp;
    int i = 0;

    // Creating a map based on .txt file
    while (std::getline(in, str))
    {
        for (int j = 0; j < str.size(); j++)
        {   
            switch (str[j])
            {
            case ' ':
                temp.push_back(Tile::FLOOR);
                break;
            case '*':
                temp.push_back(Tile::NONE);
                break;
            case 'X':
                temp.push_back(Tile::WALL);
                break;
            case 'P':
                temp.push_back(Tile::PARK);
                break;
            case 'B':
                temp.push_back(Tile::FLOOR);
                m_boxes.push_back(Box{j,i});
                break;
            case 'S':
                temp.push_back(Tile::FLOOR);
                m_player.setPos(j,i);
                break;
            default:
                std::cout << "Unknown character: " << str[j] << ", defaulting to FLOOR." << std::endl;
                temp.push_back(Tile::FLOOR);
                break;
            }
        }
        i++;
        m_map.push_back({ temp });
        temp.clear();
    }
}

void Sokoban::checkWin()
{
    // Look for box not on a PARK tile
    for (const auto& box : m_boxes)
    {
        const sf::Vector2i& boxPos = box.getPos();
        if (m_map[boxPos.y][boxPos.x] != Tile::PARK) return;
    }
    m_gameWon = true;
}

void Sokoban::updateTime()
{
    // if the game is not won, save time
    if (!m_gameWon)
        m_timeAsSeconds = (int)m_clock.getElapsedTime().asSeconds();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////public
Sokoban::Sokoban(std::string mapFileName, std::string fontFileName, const sf::Vector2u& size)
	: m_player(), m_windowSize(size), m_boxes(), m_map(), m_clock(),
    m_textures(TEXTURES), m_mapFileName(mapFileName), m_gameWon(false), m_font()
{
    if (!m_font.openFromFile(fontFileName))
        std::cout << "Could not find file named: " << fontFileName << std::endl;
    else
        std::cout << "Font: " << mapFileName << " loaded succesfully." << std::endl;
    loadMap(mapFileName);

    /** index and its corresponding texture
    * 0 - box
    * 1 - done box 
    * 2 - floor
    * 3 - none
    * 4 - park
    * 5 - player 
    * 6 - wall
    * 7 - win first part
    * 8 - win second part
    * 9 - win third part
    */
    for (int i = 0; i < m_textures.size(); i++)
    {
        const std::string imgFileName = "res/icons/image" + std::to_string(i + 1) + ".png";
        if (!m_textures[i].loadFromFile(imgFileName))
            std::cout << "Failed to load file: " << imgFileName << std::endl;
        else
            std::cout << "Icon: " << imgFileName << " loaded succesfully." << std::endl;
    }
}

Sokoban::~Sokoban()
{
}

void Sokoban::handleInputs() 
{
    if (m_gameWon) return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        movePlayer(sf::Vector2i{-1, 0});
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        movePlayer(sf::Vector2i{ 1, 0 });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        movePlayer(sf::Vector2i{ 0, -1 });
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        movePlayer(sf::Vector2i{ 0, 1 });
}

void Sokoban::setDrawParameters() 
{

    // computing tile size based on which side of the widnow is smaller
    float tileSize = std::min(
        std::floor((float)m_windowSize.x / (float)m_map[0].size()),
        std::floor((float)m_windowSize.y / (float)m_map.size())
    );
    m_tileSize = sf::Vector2f(tileSize, tileSize);

    // computing where to start drawing tiles
    m_shift = sf::Vector2f(
        ((float)m_windowSize.x - tileSize * m_map[0].size()) / 2.0f,
        ((float)m_windowSize.y - tileSize * m_map.size()) / 2.0f
    );
}

void Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Sprite sprite(m_textures[3]);
     
    if (m_map.empty() || m_map[0].empty()) return;

    for (int i = 0; i < m_map.size(); i++)
    {
        for (int j = 0; j < m_map[0].size(); j++)
        {
            // Position of current tile to be drawn
            sf::Vector2f spritePos = sf::Vector2f{
                m_tileSize.x * j + m_shift.x,
                m_tileSize.y * i + m_shift.y
            };
            
            switch (m_map[i][j])
            {
            case Tile::FLOOR:
                sprite.setTexture(m_textures[2]); break;
            case Tile::NONE:
                sprite.setTexture(m_textures[3]); break;
            case Tile::PARK:
                sprite.setTexture(m_textures[4]); break;
            case Tile::WALL:
                sprite.setTexture(m_textures[6]); break;
            }
            
            sf::Vector2f scale = {
                m_tileSize.x / sprite.getTexture().getSize().x,
                m_tileSize.y / sprite.getTexture().getSize().y
            };
            
            // Draw map tile
            sprite.setPosition(spritePos);
            sprite.setScale(scale);
            target.draw(sprite);


            // Check for player and boxes on the tile
            if (j == m_player.getPos().x && i == m_player.getPos().y)
                sprite.setTexture(m_textures[5]);

            for (const Box& box : m_boxes)
            {
                if (j == box.getPos().x && i == box.getPos().y)
                {
                    // if box on tile park, change its sprite to done
                    if (m_map[i][j] == Tile::PARK)
                        sprite.setTexture(m_textures[1]);
                    else 
                        sprite.setTexture(m_textures[0]);
                    break;
                }
            }

            target.draw(sprite);
        }
    }

    {
        std::string elapsedTimeStr = std::to_string(m_timeAsSeconds);
        sf::Text text(m_font, elapsedTimeStr);

        target.draw(text);
    }

    // Draw Victory if game is won  
    if (m_gameWon)
    {
        // Size 2x tile 
        float winTileLength = m_tileSize.x * 2.0f;
        float winTileHeight = m_tileSize.y * 2.0f;

        // Computing where to start drawing 
        sf::Vector2f winStart = {
            std::floor(((float)m_windowSize.x - winTileLength * VICTORY_TEXTURES )/ 2.0f),
            std::floor(((float)m_windowSize.y - winTileHeight) / 2.0f)
        };

        for (int i = 0; i < VICTORY_TEXTURES; i++)
        {
            sprite.setTexture(m_textures[7 + i]);

            sf::Vector2f scale = {
                winTileLength / sprite.getTexture().getSize().x,
                winTileHeight / sprite.getTexture().getSize().y
            };

            sprite.setPosition(winStart);
            sprite.setScale(scale);
            target.draw(sprite);

            winStart.x += winTileLength;
        }
    
    }
}

void Sokoban::setSize(const sf::Vector2u& size)
{
    m_windowSize = size;
}

void Sokoban::restartGame()
{
    m_boxes.clear();
    m_map.clear();
    m_gameWon = false;
    loadMap(m_mapFileName);
    m_clock.restart();
}

void Sokoban::onUpdate()
{
    checkWin();
    updateTime();
}

