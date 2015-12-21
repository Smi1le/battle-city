#pragma once
#include "collisions.h"

void explosion(float time, list<Entity*>::iterator it, bool &explosion, Texture &texture, int number);
void updateBullesPlayer(objectLevel *map, globalBool *id, images *im, float &time);
void updateBullesEnemy(objectLevel *map, globalBool *id, float &time, images *im);
void updateBrick(list<Entity*> &listBrick);
void updateEnemy(float time, Texture &explTexture, globalBool *id, objectLevel *map);