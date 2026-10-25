#include <iostream>

#include "Sokoban.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Sokoban", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    std::string mapFileName = "res/maps/map1.txt";
    std::string fontFileName = "res/fonts/Roboto-Regular.ttf";
    Sokoban sokoban(mapFileName, fontFileName, window.getSize());

    const sf::Event::KeyPressed *keyEvent;

    sokoban.setDrawParameters();

    sf::Time timeToWait = sf::milliseconds(100);
    sf::Clock clock;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (keyEvent = event->getIf<sf::Event::KeyPressed>())
            {
                switch (keyEvent->code)
                {
                case (sf::Keyboard::Key::Escape):
                    window.close();
                    break;
                case (sf::Keyboard::Key::R):
                    sokoban.restartGame();
                    break;
                }
            }
                
            if (const auto size = event->getIf<sf::Event::Resized>())
            {
                sf::Vector2f newSize(size->size.x, size->size.y);
                window.setView(sf::View(sf::FloatRect({ 0.f, 0.f }, newSize)));

                sokoban.setSize(window.getSize());
                sokoban.setDrawParameters();
            }

            if (clock.getElapsedTime() >= timeToWait) 
            {
                sokoban.handleInputs();
                clock.restart();
            }
        }

        sokoban.onUpdate();
        window.clear();
        window.draw(sokoban);
        window.display();
    }
}