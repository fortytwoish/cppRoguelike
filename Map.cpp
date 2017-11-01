#include "Map.h"
#include "RectangularRoom.h"
#include "Coordinate.h"
#include <iostream>
#include <time.h>

int randBetween(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

Map::Map(int width, int height, int seed) : width(width), height(height)
{
	srand(seed);
	tiles = new Tile[width*height];
	std::vector<IRoom*> rooms;

	// 1. Create a room
	//..

	// 2. Place the room and tell it its top left coordinate
	//..
	if (RectangularRoom::fits(10, 10))
	{
		IRoom* startRoom = new RectangularRoom(10, 10);
		rooms.push_back(startRoom);
		startRoom->topLeft = coordinate((int)(width / 2) - (int)(startRoom->dimensions.x/2), 
										(int)(height / 2) - (int)(startRoom->dimensions.y/2));
		startRoom->draw(*this);
	}

	while(rooms.size() < 20)
	{
		// 3. Pick a random wall of an existing room as door
		IRoom* randomRoom = rooms.at(rand() % (int)rooms.size());
		coordinate door = randomRoom->getAnyWall();

		//Determine direction for the door. Direction is opposite its room's floor.
		if (tileTypeAt(coordinate(door.x - 1, door.y)) == Floor)
		{//Go Right
			coordinate start = coordinate(door.x + 1, door.y);
			bool succR = true, succT = true, succB = true;
			int dR = 1, dT = 1, dB = 1;

			while(succR || succT || succB)
			{
				//Check the top horizontal line
				for (int x = start.x; succT && x <= start.x + dR; x++)
					if (tileTypeAt(coordinate(x, start.y - dT)) != Empty)
					{
						succT = false;
						break;
					}

				//Check the bottom horizontal line
				for (int x = start.x; succB && x <= start.x + dR; x++)
				{
					Tile* t = tileAt(coordinate(x, start.y + dB));
					if (tileTypeAt(coordinate(x, start.y + dB)) != Empty)
					{
						succB = false;
						break;
					}
				}

				//Check the right vertical line 
				for (int y = start.y - dT; succR && y <= start.y + dB; y++)
					if (tileTypeAt(coordinate(start.x + dR, y)) != Empty)
					{
						succR = false;
						break;
					}

				if (succT) dT++;
				if (succB) dB++;
				if (succR) dR++;
			}

			int xSpc = dR;
			int ySpc = dT + dB;

			if (RectangularRoom::fits(xSpc, ySpc))
			{
				//Finally, build and attach the new room
				randomRoom->addDoor(door);
				IRoom* newRoom = new RectangularRoom(door, xSpc, ySpc);
				rooms.push_back(newRoom);
				newRoom->topLeft = coordinate(door.x, door.y - (int)newRoom->dimensions.y/2);
				newRoom->draw(*this);
			}
		
		}
		else if (tileTypeAt(coordinate(door.x + 1, door.y)) == Floor)
		{//Go Left

			coordinate start = coordinate(door.x - 1, door.y);
			bool succL = true, succT = true, succB = true;
			int dL = 0, dT = 0, dB = 0;

			while (succL || succT || succB)
			{
				//Check the top horizontal line
				for (int x = start.x; succT && x >= start.x - dL; x--)
					if (tileTypeAt(coordinate(x, start.y - dT)) != Empty)
					{
						succT = false;
						break;
					}

				//Check the bottom horizontal line
				for (int x = start.x; succB && x >= start.x - dL; x--)
					if (tileTypeAt(coordinate(x, start.y + dB)) != Empty)
					{
						succB = false;
						break;
					}

				//Check the right vertical line 
				for (int y = start.y - dT; succL && y <= start.y + dB; y++)
					if (tileTypeAt(coordinate(start.x - dL, y)) != Empty)
					{
						succL = false;
						break;
					}

				if (succT) dT++;
				if (succB) dB++;
				if (succL) dL++;
			}

			//todo: different room types
			int xSpc = dL;
			int ySpc = dT + dB;

			if (RectangularRoom::fits(xSpc, ySpc))
			{
				//Finally, build and attach the new room
				randomRoom->addDoor(door);
				IRoom* newRoom = new RectangularRoom(door, xSpc, ySpc);
				

				//Choose topLeft so that the new room fits within its available space but still reaches the door
				coordinate topLeft;
				topLeft.x = door.x - newRoom->dimensions.x + 1;
				
				//topleft_y range for accessible door:		   [door.y - roomHeight, door.y]
				//topleft_y range for not cutting other rooms: [door.y - dT,		 door.y + dB - roomHeight]
				int topLeftYFrom = MAX(door.y - newRoom->dimensions.y + 2, door.y - dT);
				int topLeftYTo   = MIN(door.y - 1, door.y + dB - (newRoom->dimensions.y - 1));
				if (topLeftYFrom > topLeftYTo)
				{
					delete newRoom;
					continue;
				}
				topLeft.y = randBetween(topLeftYFrom, topLeftYTo);

				newRoom->topLeft = topLeft;
				newRoom->draw(*this);
				rooms.push_back(newRoom);
			}
		}
		else if (tileTypeAt(coordinate(door.x, door.y - 1)) == Floor)
		{//Go Down

			coordinate start = coordinate(door.x, door.y + 1);
			bool succB = true, succL = true, succR = true;
			int dB = 1, dL = 1, dR = 1;

			while (succB || succL || succR)
			{
				//Check the left vertical line
				for (int y = start.y; succL && y <= start.y + dB; y++)
					if (tileTypeAt(coordinate(start.x - dL, y)) != Empty)
					{
						succL = false;
						break;
					}

				//Check the right vertical line
				for (int y = start.y; succR && y <= start.y + dB; y++)
					if (tileTypeAt(coordinate(start.x + dR, y)) != Empty)
					{
						succR = false;
						break;
					}

				//Check the bottom horizontal line
				for (int x = start.x - dL; succB && x <= start.x + dR; x++)
					if (tileTypeAt(coordinate(x, start.y + dB)) != Empty)
					{
						succB = false;
						break;
					}

				if (succL) dL++;
				if (succR) dR++;
				if (succB) dB++;
			}

			int xSpc = dL + dR;
			int ySpc = dB;

			if (RectangularRoom::fits(xSpc, ySpc))
			{
				//Finally, build and attach the new room
				randomRoom->addDoor(door);
				IRoom* newRoom = new RectangularRoom(door, xSpc, ySpc);
				rooms.push_back(newRoom);
				newRoom->topLeft = coordinate(door.x - (int)newRoom->dimensions.x/2, door.y);
				newRoom->draw(*this);
			}
		}
		else
		{//Go Up

			coordinate start = coordinate(door.x, door.y - 1);
			bool succT = true, succL = true, succR = true;
			int dT = 1, dL = 1, dR = 1;

			while (succT || succL || succR)
			{
				//Check the left vertical line
				for (int y = start.y; succL && y >= start.y - dT; y--)
					if (tileTypeAt(coordinate(start.x - dL, y)) != Empty)
					{
						succL = false;
						break;
					}

				//Check the right vertical line
				for (int y = start.y; succR && y >= start.y - dT; y--)
					if (tileTypeAt(coordinate(start.x + dR, y)) != Empty)
					{
						succR = false;
						break;
					}

				//Check the top horizontal line
				for (int x = start.x - dL; succT && x <= start.x + dR; x++)
					if (tileTypeAt(coordinate(x, start.y - dT)) != Empty)
					{
						succT = false;
						break;
					}

				if (succL) dL++;
				if (succR) dR++;
				if (succT) dT++;
			}

			//TODO: Random Room Types
			int xSpc = dL + dR;
			int ySpc = dT;

			if (RectangularRoom::fits(xSpc, ySpc))
			{
				//Finally, build and attach the new room
				randomRoom->addDoor(door);
				IRoom* newRoom = new RectangularRoom(door, xSpc, ySpc);
				rooms.push_back(newRoom);
				newRoom->topLeft = coordinate(door.x - (int)newRoom->dimensions.x / 2, door.y - newRoom->dimensions.y + 1);
				newRoom->draw(*this);
			}
		}
	}
}

Map::~Map()
{
	delete[] tiles;
}

Tile* Map::tileAt(coordinate c) const
{
	if (c.x < 0 || c.y < 0 || c.x >= width || c.y >= height) return NULL;
	return &tiles[c.x + c.y*width];
}

TileType Map::tileTypeAt(coordinate c) const
{
	if (tileAt(c) == NULL) return OutOfBounds;
	return tileAt(c)->type;
}

void Map::setWall(coordinate c)
{
	tileAt(c)->type = Wall;
}

void Map::setDoor(coordinate c)
{
	tileAt(c)->type = Door;
}

void Map::setFloor(coordinate c)
{
	tileAt(c)->type = Floor;
}

void Map::removeTile(coordinate c)
{
	tileAt(c)->type = Empty;
}

void Map::render(coordinate from, coordinate to) const
{
	static const TCODColor darkWall(100,100,100);
	static const TCODColor darkGround(200,200,200);
	int xOffset = 0, yOffset = 0;
	if (from.x < 0) xOffset = from.x * -1;
	if (from.y < 0) yOffset = from.y * -1;
	for (int ix = xOffset, x = from.x; x - xOffset <= to.x && x - xOffset < width; ix++, x++)
	{
		for (int iy = yOffset, y = from.y; y - yOffset <= to.y && y - yOffset < height; iy++, y++)
		{
			switch (tileTypeAt(coordinate(x + xOffset, y + yOffset)))
			{
				case Empty:
					TCODConsole::root->setCharBackground(ix, iy, darkGround);
					break;
				case Floor:
					TCODConsole::root->setChar(ix, iy, '.');
					break;
				case Wall:
					TCODConsole::root->setCharBackground(ix, iy, darkWall);
					break;
				case Door:
					TCODConsole::root->setChar(ix, iy, '+');
					break;
				default: break;
			}
		}
	}
}