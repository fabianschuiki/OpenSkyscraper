#include "spacepartition.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>


const int2 SpacePartition::size(200, 200);


SpacePartition::SpacePartition(int x, int y) : pos(x, y)
{
}

SpacePartition::SpacePartition(int2 pos) : pos(pos)
{
}


void SpacePartition::draw(sf::RenderTarget & context)
{
	//For debugging purposes, draw a border around the space partition.
	sf::Color color = (visible ? sf::Color(255, 255, 255, 16)
					   : sf::Color(255, 0, 0, 32));
	sf::Shape rect = sf::Shape::Rectangle((pos.x-0.5)*size.x, (pos.y-0.5)*size.y, (pos.x+0.5)*size.x, (pos.y+0.5)*size.y,
										  color, 1, sf::Color(255, 255, 255));
	context.Draw(rect);
}
