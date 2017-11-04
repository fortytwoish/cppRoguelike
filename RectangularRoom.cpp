#include "RectangularRoom.h"
#include <random>
#include <algorithm>

coordinate RectangularRoom::getAnyWall()
{
	int perimeter = 2 * dimensions.x 
				  + 2 * dimensions.y;
	coordinate randWall;
	//Find a random wall location that isn't a door yet
	do
	{
		randWall = pointOnPerimeter(rand() % perimeter);
	} while (std::find(doors->begin(), doors->end(), randWall) != doors->end());

	return randWall;
}



void RectangularRoom::draw(Map & map)
{
	for (int y = 0; y < dimensions.y; y++)
	{
		for (int x = 0; x < dimensions.x; x++)
		{
			if (x == 0 || y == 0 || x==dimensions.x-1 || y == dimensions.y-1) map.setWall(coordinate(topLeft.x+x,topLeft.y+y));
			else map.setFloor(coordinate(topLeft.x+x, topLeft.y+y));
		}
	}

	for (coordinate d : *doors)
	{
		map.setDoor(d);
	}

	//Chance to create pillars
	if (rand() % 2 < 1)
	{
		map.setWall(coordinate(topLeft.x + 2, topLeft.y + 2));
		map.setWall(coordinate(topLeft.x + 2, topLeft.y + dimensions.y - 3));
		map.setWall(coordinate(topLeft.x + dimensions.x - 3, topLeft.y + 2));
		map.setWall(coordinate(topLeft.x +dimensions.x - 3, topLeft.y + dimensions.y - 3));
	}

	//Chance to create Windows
	if(rand() % 5 < 1)
		for (int i = 0; i < rand() % 5; i++)
		{
			map.setWindow(getAnyWall());
		}
}

coordinate RectangularRoom::pointOnPerimeter(int offset)
{
	while (true)
	{
		if (offset > 0 && offset < dimensions.x - 1) return coordinate(topLeft.x + offset, topLeft.y);
		offset -= dimensions.x;

		if (offset > 0 && offset < dimensions.y - 1) return coordinate(topLeft.x + dimensions.x - 1, topLeft.y + offset);
		offset -= dimensions.y;

		if (offset > 0 && offset < dimensions.x - 1) return coordinate(topLeft.x + offset, topLeft.y + dimensions.y - 1);
		offset -= dimensions.x;

		if (offset > 0 && offset < dimensions.y - 1) return coordinate(topLeft.x, topLeft.y + offset);

		offset++;
	}
}

void RectangularRoom::create()
{
	dimensions.x = rand() % MIN(45, maxX) + 5;
	dimensions.y = rand() % MIN(45, maxY) + 5;
}

RectangularRoom::RectangularRoom(int availableX, int availableY)
{
	maxX = availableX;
	maxY = availableY;
	doors = new std::vector<coordinate>();
	create();
}

RectangularRoom::RectangularRoom(coordinate door, int availableX, int availableY)
{
	maxX = availableX;
	maxY = availableY;
	doors = new std::vector<coordinate>({ door });
	create();
}
