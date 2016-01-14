#pragma once
#include "checks.h"



void checkCollisionWithMap(Player *p, Entity* brick, bool &changeCourse);
void collisionWithBricksForPlayer(Player *p, list<Entity*> &listBrick, bool &changeCourse);
void collisionWithEnemyForPlayer(Player *p, list<Entity*> &entities, bool &changeCourse);