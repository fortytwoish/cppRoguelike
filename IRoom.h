#pragma once
#include "Coordinate.h"
#include <vector>
#include "Map.h"

class IRoom
{
private:
	virtual void create() = 0;
public:
	//Pure virtual member -> IRoom can't be instantiated
	virtual coordinate getAnyWall() = 0;
	virtual void draw(Map& map) = 0;
	static bool fits(int availableX, int availableY);

	std::vector<coordinate>* doors;
	coordinate topLeft;
	coordinate dimensions;
	
	void addDoor(coordinate coord)
	{
		doors->push_back(coord);
	}
};

