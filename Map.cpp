#include "Map.h"
#include "Coordinate.h"
#include <iostream>
#include <time.h>
#include "RectangularRoom.h"

coordinate center;

int randBetween(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

Map::Map(int width, int height, int seed) : width(width), height(height)
{
	srand(seed);
	center = coordinate((int)width / 2, (int)height / 2);
	tiles = new Tile[width*height];

	// 1. Create a room
	//..

	// 2. Place the room and tell it its top left coordinate
	//..

	if (RectangularRoom::fits(10, 10))
	{
		RectangularRoom* startRoom = new RectangularRoom(10, 10);
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

		roomSpacing availableSpace = determineAvailableSpace(door);

		int xSpc = availableSpace.dL + availableSpace.dR;
		int ySpc = availableSpace.dT + availableSpace.dB;

		if (RectangularRoom::fits(xSpc, ySpc))
		{
			//Finally, build and attach the new room
			randomRoom->addDoor(door);
			RectangularRoom* newRoom = new RectangularRoom(door, xSpc, ySpc);

			//Room placement
			coordinate topLeft;

			if (availableSpace.xDir != 0)
			{
				topLeft.x = availableSpace.xDir == -1
						  ? door.x - newRoom->dimensions.x + 1
						  : door.x;

				//topleft_y range for accessible door:		   [door.y - roomHeight, door.y]
				//topleft_y range for not cutting other rooms: [door.y - dT,		 door.y + dB - roomHeight]
				int topLeftYFrom = MAX(door.y - newRoom->dimensions.y + 2, door.y - availableSpace.dT);
				int topLeftYTo   = MIN(door.y - 1, door.y + availableSpace.dB - (newRoom->dimensions.y - 1));
				if (topLeftYFrom > topLeftYTo)
				{
					delete newRoom;
					continue;
				}
				topLeft.y = randBetween(topLeftYFrom, topLeftYTo);
			}
			else
			{
				topLeft.y = availableSpace.yDir == -1
						  ? door.y - newRoom->dimensions.y + 1
						  : door.y;

				//topleft_x range for accessible door:		   [door.x - roomWidth, door.x]
				//topleft_x range for not cutting other rooms: [door.x - dL,		door.x + dR - roomWidth]
				int topLeftXFrom = MAX(door.x - newRoom->dimensions.x + 2, door.x - availableSpace.dL);
				int topLeftXTo   = MIN(door.x - 1, door.x + availableSpace.dR - (newRoom->dimensions.y - 1));
				if (topLeftXFrom > topLeftXTo)
				{
					delete newRoom;
					continue;
				}
				topLeft.x = randBetween(topLeftXFrom, topLeftXTo);
			}

			newRoom->topLeft = topLeft;
			newRoom->draw(*this);
			rooms.push_back(newRoom);
		}
	}
}

Map::~Map()
{
	for (IRoom* room : rooms) delete room;
	rooms.clear();
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

void Map::setWindow(coordinate c)
{
	tileAt(c)->type = Window;
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

Map::roomSpacing Map::determineAvailableSpace(coordinate fromDoor)
{
	//Determine direction for the door. Direction is opposite its room's floor.
	int xDir = 0, yDir = 0;

	bool succL = true, succR = true, succT = true, succB = true;
	int  dL = 0, dR = 0, dT = 0, dB = 0;

	if (tileTypeAt(coordinate(fromDoor.x - 1, fromDoor.y)) == Floor)
	{
		xDir = 1;
		succL = false;
	}
	else if (tileTypeAt(coordinate(fromDoor.x + 1, fromDoor.y)) == Floor)
	{
		xDir = -1;
		succR = false;
	}
	else if (tileTypeAt(coordinate(fromDoor.x, fromDoor.y - 1)) == Floor)
	{
		yDir = 1;
		succT = false;
	}
	else
	{
		yDir = -1;
		succB = false;
	}

	coordinate start = coordinate(fromDoor.x + xDir, fromDoor.y + yDir);

	while (succL || succR || succT || succB)
	{
		//Check the left vertical line
		for (int y = start.y - dT; succL && y <= start.y + dB; y++)
			if (tileTypeAt(coordinate(start.x - dL - 1, y)) != Empty)
			{
				//Did we hit 
				succL = false;
				break;
			}
		if (succL) dL++;

		//Check the right vertical line
		for (int y = start.y - dT; succR && y <= start.y + dB; y++)
			if (tileTypeAt(coordinate(start.x + dR + 1, y)) != Empty)
			{
				succR = false;
				break;
			}
		if (succR) dR++;

		//Check the top horizontal line
		for (int x = start.x - dL; succT && x <= start.x + dR; x++)
			if (tileTypeAt(coordinate(x, start.y - dT - 1)) != Empty)
			{
				succT = false;
				break;
			}
		if (succT) dT++;

		//Check the bottom horizontal line
		// left  coordinate: [start.x - dL, start.y - dB]
		// right coordinate: [start.x + dR, start.y - dB]
		for (int x = start.x - dL; succB && x <= start.x + dR; x++)
			if (tileTypeAt(coordinate(x, start.y + dB + 1)) != Empty)
			{
				succB = false;
				break;
			}
		if (succB) dB++;
	}

	return roomSpacing(dL, dR, dT, dB, xDir, yDir);
}

void FOV(const Map* map, coordinate& player, int range)
{
	float x, y;
	int i;

	TCOD_mouse_t mouse;
	TCODSystem::checkForEvent(TCOD_EVENT_MOUSE_MOVE, NULL, &mouse);

	double angle = atan2(mouse.cy - 50, mouse.cx - 100) * 180 / 3.141;

	for (i = angle-45.0; i<angle+45.0; i++)
	{
		x = cos((float)i*0.01745f);
		y = sin((float)i*0.01745f);

		int i;
		float ox, oy;
		ox = (float)player.x + 0.5f;
		oy = (float)player.y + 0.5f;
		for (i = 0; i<range; i++)
		{
			if (ox < 0 || ox >= map->width || oy < 0 || oy >= map->height) continue;
			Tile* t = map->tileAt(coordinate((int)ox, (int)oy));
			t->distance = i;
			t->seen = true;
			if (!t->isTransparent())
				break;
			ox += x;
			oy += y;
		};
	};
};

void Map::render(coordinate from, coordinate to, coordinate player) const
{
	int playerVisionRange = 30;

	for (int i = 0; i < width * height; i++) tiles[i].distance = 1000;
	FOV(this, player, playerVisionRange);

	int xOffset = 0, yOffset = 0;
	if (from.x < 0) xOffset = from.x * -1;
	if (from.y < 0) yOffset = from.y * -1;
	for (int ix = xOffset, x = from.x; x - xOffset <= to.x && x - xOffset < width; ix++, x++)
	{
		for (int iy = yOffset, y = from.y; y - yOffset <= to.y && y - yOffset < height; iy++, y++)
		{
			Tile* t = tileAt(coordinate(x + xOffset, y + yOffset));
			if (t == NULL) continue;

			double distanceFactor = 1.0 - t->distance / (double)playerVisionRange;

			switch (t->type)
			{
				case Empty:
					if (t->distance <= playerVisionRange)
						TCODConsole::root->setCharBackground(ix, iy, TCODColor((int)(0 + 100 * distanceFactor), (int)(20 + 100 * distanceFactor), (int)(0 + 100 * distanceFactor)));
					break;
				case Floor:
					if (t->seen)
					{
						TCODConsole::root->setCharForeground(ix, iy, TCODColor(10, 10, 10));
						TCODConsole::root->setChar(ix, iy, '.');
						TCODConsole::root->setCharBackground(ix, iy, TCODColor(10, 10, 10));
					}
					if (t->distance <= playerVisionRange)
						TCODConsole::root->setCharBackground(ix, iy, TCODColor((int) (10+ 100 * distanceFactor), (int)(10 + 100 * distanceFactor), (int)(10 + 100 * distanceFactor)));
					break;
				case Wall:
					if (t->seen)
						TCODConsole::root->setCharBackground(ix, iy, TCODColor(40, 40, 40));
					if(t->distance <= playerVisionRange)
						TCODConsole::root->setCharBackground(ix, iy, TCODColor((int)(40 + 100 * distanceFactor), (int)(40 + 100 * distanceFactor), (int)(40 + 100 * distanceFactor)));
					break;
				case Window:
					if (t->seen)
						TCODConsole::root->setCharBackground(ix, iy, TCODColor(50, 50, 90));
					if (t->distance <= playerVisionRange)
						TCODConsole::root->setCharBackground(ix, iy, TCODColor(130, 130, 170));
					break;
				case Door:
					if (t->seen)
					{
						TCODConsole::root->setCharForeground(ix, iy, TCODColor(70, 70, 70));
						TCODConsole::root->setChar(ix, iy, '.');
					}
					if (t->distance <= playerVisionRange)
						TCODConsole::root->setCharForeground(ix, iy, TCODColor(150, 150, 150));
					break;
					/*
					if (t->seen)
					{
						TCODConsole::root->setCharForeground(ix, iy, TCODColor(45, 45, 45));
						TCODConsole::root->setChar(ix, iy, 'O');
					}
					if (t->visible)
						TCODConsole::root->setCharForeground(ix, iy, TCODColor(125, 125, 125));		
					break;
					*/
				default: break;
			}
		}
	}
}

