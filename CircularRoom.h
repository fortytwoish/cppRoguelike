#pragma once
//=================================
// include guard
#ifndef __CircularRoom_H_INCLUDED__
#define __CircularRoom_H_INCLUDED__

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include "IRoom.h"

class CircularRoom : public IRoom
{
public:
	//=================================
	//Interface members
	coordinate getAnyWall();
	void draw(Map& map);
	static bool fits(int availableX, int availableY) { return MIN(availableX, availableY) >= 6; }

	CircularRoom(coordinate door, int availableX, int availableY);
	~CircularRoom();

private:
	coordinate pointOnPerimeter();
	void create();
	int radius;
};

#endif