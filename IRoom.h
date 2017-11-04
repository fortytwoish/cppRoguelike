#pragma once

//=================================
// include guard
#ifndef __IRoom_H_INCLUDED__
#define __IRoom_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Coordinate.h"
#include <vector>
#include "Map.h"

class IRoom
{
protected:
	int maxX;
	int maxY;

private:
	virtual void create() = 0;

public:
	//Pure virtual member -> IRoom can't be instantiated
	virtual coordinate getAnyWall() = 0;
	virtual void draw(Map& map) = 0;
	static bool fits(int availableX, int availableY);

	std::vector<coordinate>* doors;
	coordinate topLeft;
	
	void addDoor(coordinate coord)
	{
		doors->push_back(coord);
	}

	//virtual destructor -> in polymorphy situation, derived class' destructor will be used instead
	virtual ~IRoom()
	{
		doors->clear();
		delete doors;
	};
};

#endif