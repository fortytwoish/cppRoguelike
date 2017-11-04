#pragma once

//=================================
// include guard
#ifndef __MAP_H_INCLUDED__
#define __MAP_H_INCLUDED__

//=================================
// forward declared dependencies
class IRoom;

//=================================
// included dependencies
#include "libtcod.hpp"
#include "Coordinate.h"
#include <vector>

enum TileType { Empty, Floor, Wall, Door, Window, OutOfBounds };

struct Tile 
{
	TileType type;
	bool canWalk()		 { return type == Floor || type == Door; }
	bool isTransparent() { return type == Floor || type == Door || type == Window || type == Empty; }
	bool seen;
	int distance;
	Tile() : type(Empty), seen(false) {}
};

class Map 
{
	public:
		int width, height;

		Map(int width, int height, int seed);
		~Map();
		Tile* tileAt(coordinate c) const;
		TileType tileTypeAt(coordinate c) const;
		void render(coordinate from, coordinate to, coordinate player) const;
		void setWall(coordinate c);
		void setWindow(coordinate c);
		void setDoor(coordinate c);
		void setFloor(coordinate c);
		void removeTile(coordinate c);

	private:
		Tile *tiles;
		std::vector<IRoom*> rooms;
		struct roomSpacing
		{ 
			int dL, dR, dT, dB, xDir, yDir;
			roomSpacing(int dL, int dR, int dT, int dB, int xDir, int yDir) :dL(dL), dR(dR), dT(dT), dB(dB), xDir(xDir), yDir(yDir) {}
		};
		roomSpacing determineAvailableSpace(coordinate fromDoor);
};

#endif
