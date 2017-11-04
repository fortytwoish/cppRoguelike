#include <cstdio>
#include "libtcod.hpp"
#include "Actor.h"
#include "Map.h"

//test

#include "Coordinate.h"
#include <vector>
#include <algorithm>
#include <iostream>

// Hide the console window
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

//prototypes
void render();
void update();

TCODList<Actor *> actors;
Actor *player;
Map *map;

int consoleWidth = 200;
int consoleHeight = 100;
int seed = -1;
int main()
{
	TCODConsole::initRoot(consoleWidth, consoleHeight+5, "Roguelike C++", false);
	//TCODSystem::forceFullscreenResolution(2560, 1440);
	//TCODConsole::setFullscreen(true);
	TCODConsole::setKeyboardRepeat(33, 33);
	player = new Actor(100, 100, '@', TCODColor::white);
	actors.push(player);
	//actors.push(new Actor(60, 13, '@', TCODColor::yellow));
	map = new Map(250, 250, seed);

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
	switch (key.c)
	{
		case 'q':
		case 'Q':
			exit(0);
		case 'w':
			if (map->tileAt(coordinate(player->x, player->y - 1))->canWalk()) {
				player->y--;
			}
			break;
		case 's':
			if (map->tileAt(coordinate(player->x, player->y + 1))->canWalk()) {
				player->y++;
			}
			break;
		case 'a':
			if (map->tileAt(coordinate(player->x - 1, player->y))->canWalk()) {
				player->x--;
			}
			break;
		case 'd':
			if (map->tileAt(coordinate(player->x + 1, player->y))->canWalk()) {
				player->x++;
			}
			break;

	}

	switch (key.vk) {
	case TCODK_F1:
	{
		delete map;
		map = new Map(200, 200, ++seed);
		player->x = player->y = 100;
		break;
	}
	
	default:break;
	}
}

void render() {
	TCODConsole::root->clear();
	// draw the map
	map->render(coordinate(player->x - consoleWidth/2, player->y - consoleHeight / 2),
				coordinate(player->x + consoleWidth/2, player->y + consoleHeight / 2),
				player->position());

	// draw the actor(s)
	TCODConsole::root->setChar(consoleWidth / 2, consoleHeight / 2, '@');
	TCODConsole::root->setCharForeground(consoleWidth / 2, consoleHeight / 2, TCODColor::yellow);
	
	/*
	for (Actor **iterator = actors.begin(); iterator != actors.end(); iterator++)
	{
		(*iterator)->render();
	}*/

	TCODConsole::root->printEx(0, consoleHeight+1, TCOD_BKGND_NONE, TCOD_LEFT, "Player Coords: (%d,%d) | Seed: %d", player->x, player->y, seed);
	TCODConsole::root->printEx(0, consoleHeight + 2, TCOD_BKGND_NONE, TCOD_LEFT, "F1 - Generate new Map");
	TCODConsole::root->printEx(0, consoleHeight + 3, TCOD_BKGND_NONE, TCOD_LEFT, " Q - Quit");
}