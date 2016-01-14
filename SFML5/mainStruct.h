#pragma once
#include "Text.h"
#include "globalVariableWithGame.h"

struct engine {
	Clock clock;
	images * im = nullptr;
	globalBool *id = nullptr;
	objectLevel *map = nullptr;
	Object player;
	Player *p = nullptr;
	Enemy *targetProtecting = nullptr;
	sf::RenderWindow *window;
	globalVariable var;
	engine();
};