#pragma once
#include "IRoom.h"
class RectangularRoom : public IRoom
{
private:
	int maxX;
	int maxY;
	coordinate pointOnPerimeter(int offsetFromTopLeft);
	void create();

public:

	//Interface members
	static bool fits(int availableX, int availableY){ return availableX >= 3 && availableY >= 3; }
	coordinate getAnyWall();
	void draw(Map& map);

	RectangularRoom(int availableX, int availableY);
	RectangularRoom(coordinate door, int availableX, int availableY);
	~RectangularRoom();
};

