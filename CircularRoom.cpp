#include "CircularRoom.h"

coordinate CircularRoom::getAnyWall()
{
	return coordinate();
}

void CircularRoom::draw(Map & map)
{
	for(int y = 0; y < radius * 2; y++)
		for (int x = 0; x < radius * 2; x++)
		{
			if()
		}
}

CircularRoom::CircularRoom(coordinate door, int availableX, int availableY)
{
	addDoor(door);
}

CircularRoom::~CircularRoom()
{
}

coordinate CircularRoom::pointOnPerimeter()
{
	return coordinate();
}

void CircularRoom::create()
{
	radius = MIN(maxX, maxY) / 20;
}
