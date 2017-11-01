#pragma once
#include "libtcod.hpp"
#include "Coordinate.h"
class Actor
{
public:
	int x, y; // position on map
	int ch; // ascii code
	TCODColor col; // color

	Actor(int x, int y, int ch, const TCODColor &col);
	void render() const;
	coordinate position() const
	{
		return coordinate(x, y);
	}
};

