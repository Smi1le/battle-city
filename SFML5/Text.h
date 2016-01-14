#pragma once
#include "Enemy.h"
#include "boolean.h"
#include <vld.h>

struct text {
	Font font;
	Text healph;
	Text youLose;
	Text youWin;
	Text escape;
	Text game;
	Text continues;
	Text exit;
	text(objectLevel *lvl);
}; 