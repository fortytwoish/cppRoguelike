#pragma once

//=================================
// include guard
#ifndef __RectangularRoom_H_INCLUDED__
#define __RectangularRoom_H_INCLUDED__

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include "IRoom.h"

class RectangularRoom : public IRoom
{
private:
	coordinate pointOnPerimeter(int offsetFromTopLeft);
	void create();

public:

	//=================================
	//Interface members
	coordinate getAnyWall();
	void draw(Map& map);
	static bool fits(int availableX, int availableY) { return availableX >= 5 && availableY >= 5; }

	coordinate dimensions;

	RectangularRoom(int availableX, int availableY);
	RectangularRoom(coordinate door, int availableX, int availableY);
};

#endif
