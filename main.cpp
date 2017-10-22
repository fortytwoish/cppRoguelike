#include <cstdio>
#include "libtcod.hpp"
#include "Actor.h"
#include "Map.h"

// Hide the console window
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

//prototypes
void render();
void update();

TCODList<Actor *> actors;
Actor *player;
Map *map;

int consoleWidth = 80;
int consoleHeight = 50;
int centerX = consoleWidth / 2;
int centerY = consoleHeight / 2;

int main()
{
	TCODConsole::initRoot(consoleWidth, consoleHeight+5, "libtcod C++ tutorial", false);
	TCODConsole::setKeyboardRepeat(50, 33);
	player = new Actor(100, 100, '@', TCODColor::white);
	actors.push(player);
	//actors.push(new Actor(60, 13, '@', TCODColor::yellow));
	map = new Map(200, 200);

	while (!TCODConsole::isWindowClosed()) {
		update();
		render();
		TCODConsole::flush();
	}

	actors.clearAndDelete();
	delete map;

	return 0;
}

void update() {
	TCOD_key_t key;
	TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
	switch (key.vk) {
	case TCODK_F1:
	{
		delete map;
		map = new Map(200, 200);
		player->x = player->y = 100;
		break;
	}
	case TCODK_UP:
		if (!map->isWall(player->x, player->y - 1)) {
			player->y--;
		}
		break;
	case TCODK_DOWN:
		if (!map->isWall(player->x, player->y + 1)) {
			player->y++;
		}
		break;
	case TCODK_LEFT:
		if (!map->isWall(player->x - 1, player->y)) {
			player->x--;
		}
		break;
	case TCODK_RIGHT:
		if (!map->isWall(player->x + 1, player->y)) {
			player->x++;
		}
		break;
	default:break;
	}
}

void render() {
	TCODConsole::root->clear();
	// draw the map
	map->render(player->x - consoleWidth/2,		player->x + consoleWidth/2,
				player->y - consoleHeight/2,	player->y + consoleHeight/2);

	// draw the actor(s)
	TCODConsole::root->setChar(consoleWidth / 2, consoleHeight / 2, '@');
	TCODConsole::root->setCharForeground(consoleWidth / 2, consoleHeight / 2, TCODColor::black);
	
	/*
	for (Actor **iterator = actors.begin(); iterator != actors.end(); iterator++)
	{
		(*iterator)->render();
	}*/

	TCODConsole::root->printEx(0, consoleHeight+1, TCOD_BKGND_NONE, TCOD_LEFT, "Player Coords: (%d,%d)", player->x, player->y);
}