#include "spacepartition.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>


const int SpacePartition::w = 200;
const int SpacePartition::h = 200;


SpacePartition::SpacePartition(int x, int y) : x(x), y(y)
{
}


void SpacePartition::draw(sf::RenderTarget & context)
{
	//For debugging purposes, draw a border around the space partition.
	sf::Color color = (sprites.empty() ? sf::Color(255, 255, 255, 16)
					   : sf::Color(0, 255, 0, 64));
	sf::Shape rect = sf::Shape::Rectangle((x-0.5)*w, (y-0.5)*h, (x+0.5)*w, (y+0.5)*h,
										  color, 1, sf::Color(255, 255, 255));
	context.Draw(rect);
}
