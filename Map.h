#pragma once
#include "libtcod.hpp"
#include "Coordinate.h"

enum TileType { Empty, Floor, Wall, Door, OutOfBounds };

struct Tile {

	TileType type;
	bool canWalk() { return type == Floor || type == Door; }
	Tile() : type(Empty) {}
};

class Map {
public:
	int width, height;

	Map(int width, int height, int seed);
	~Map();
	Tile* tileAt(coordinate c) const;
	TileType tileTypeAt(coordinate c) const;
	void render(coordinate from, coordinate to) const;
	void setWall(coordinate c);
	void setDoor(coordinate c);
	void setFloor(coordinate c);
	void removeTile(coordinate c);

protected:
	Tile *tiles;
};