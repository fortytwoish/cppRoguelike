#pragma once
#include "libtcod.hpp"

struct Tile {
	bool canWalk; // can we walk through this tile?
	Tile() : canWalk(true) {}
};

class Map {
public:
	int width, height;

	Map(int width, int height);
	~Map();
	bool isWall(int x, int y) const;
	void render(int x0, int x1, int y0, int y1) const;
protected:
	Tile *tiles;

	void setWall(int x, int y);
	void removeWall(int x, int y);
};