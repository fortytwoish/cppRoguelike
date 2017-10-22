#include "Map.h"
#include <iostream>
#include <time.h>

enum RoomType { corridor, rect, circle, COUNT };

Map::Map(int width, int height) : width(width), height(height)
{
	srand(time(NULL));
	tiles = new Tile[width*height];

	int rtype;
	int xDir, yDir;
	int prevXDir, prevYDir;

	int doorX = width/2, 
		doorY = height/2;

	int dir = rand() % 4;
	if      (dir == 0) { xDir =  1; yDir =  0; }
	else if (dir == 1) { xDir =  0; yDir =  1; }
	else if (dir == 2) { xDir = -1; yDir =  0; }
	else			   { xDir =  0; yDir = -1; }

	prevXDir = xDir;
	prevYDir = yDir;

	for (int i = 0; i < 20; i++)
	{
		//Randomly determine new room type
		rtype = 0;//y rand() % 2;// RoomType::COUNT;

		switch (rtype)
		{
			case corridor:
			{
				//std::cout << "Door: (" << doorX << "," << doorY << ")\n";
				setWall(doorX - xDir, doorY - yDir); //Initial end of corridor
				int len = rand() % 7 + 3;
				for (int n = -1; n < len; n++)
				{
					setWall(doorX + n * xDir + yDir, doorY + n * yDir + xDir);
					setWall(doorX + n * xDir - yDir, doorY + n * yDir - xDir);
				}
				if(i > 0) removeWall(doorX - prevXDir, doorY - prevYDir);
				prevXDir = xDir;
				prevYDir = yDir;
				doorX += xDir * len;
				doorY += yDir * len;
				break;
			}
			case rect:
				break;
			case circle:
				break;
			default:
				break;
		}

		//Choose new dir that's NOT opposite the last
		int nDir;
		do{ 
			nDir = rand() % 4;
		} while 
			(nDir % 2 == dir % 2);

		if      (nDir == 0) { xDir =  1; yDir =  0; }
		else if (nDir == 1) { xDir =  0; yDir =  1; }
		else if (nDir == 2) { xDir = -1; yDir =  0; }
		else			   { xDir =  0; yDir = -1; }

		dir = nDir;
	}

}

Map::~Map()
{
	delete[] tiles;
}

bool Map::isWall(int x, int y) const
{
	return !tiles[x + y*width].canWalk;
}

void Map::setWall(int x, int y)
{
	//std::cout << "SetWall (" << x << "," << y << ")\n";
	tiles[x + y*width].canWalk = false;
}

void Map::removeWall(int x, int y)
{
	//std::cout << "RemoveWall (" << x << "," << y << ")\n";
	tiles[x + y*width].canWalk = true;
}

void Map::render(int x0, int x1, int y0, int y1) const
{
	static const TCODColor darkWall(100,100,100);
	static const TCODColor darkGround(200,200,200);
	int xOffset = 0, yOffset = 0;
	if (x0 < 0) xOffset = x0 * -1;
	if (y0 < 0) yOffset = y0 * -1;
	for (int ix = xOffset, x = x0; x - xOffset <= x1 && x - xOffset < width; ix++, x++)
	{
		for (int iy = yOffset, y = y0; y - yOffset <= y1 && y - yOffset < height; iy++, y++)
		{
			TCODConsole::root->setCharBackground(ix, iy, isWall(x + xOffset, y + yOffset) ? darkWall : darkGround);
		}
	}
}