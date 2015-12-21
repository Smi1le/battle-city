#pragma once
#include "Text.h"
#include "globalVariableWithGame.h"

void rangeValuesRandomly(int &number, int upperLimit);
void checkForRespawnBonus(globalVariable &var, globalBool *id, Player *p, objectLevel *map, images *im);
void isAppearenceEnemy(globalVariable &var, globalBool *id, Player *p, objectLevel *map, images *im, float &time);
void isHitForPlayer(float &time, images *im, Object &player, Player *p, globalBool *id, objectLevel *map);
void changeCourseTank(int &randomNumber, int &timer, const float time, list<Entity*> &entities);
void checkPlayerBulles(objectLevel *map, RenderWindow &window, Enemy &targetProtecting, globalBool *id);
void checkEnemyBulles(objectLevel *map, Player *p, globalBool *id, RenderWindow &window, Enemy targetProtecting);
void checkForBonus(Player *p, objectLevel *map);