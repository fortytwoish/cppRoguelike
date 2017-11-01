#pragma once
struct coordinate
{
	bool operator ==(coordinate other)
	{
		return x == other.x && y == other.y;
	}
	coordinate() {}
	coordinate(int x, int y) : x(x), y(y) {}
	int x;
	int y;
};