#pragma once
#include "Enemy.h"

class objectLevel {
public:
	std::list<Entity*>  entities;
	std::list<Entity*> numberEnemies;
	std::list<Entity*> bullesEnemy;
	std::list<Entity*>  listBrick;
	std::list<Entity*> bullesPlayer;
	std::list<Entity*> listPointsBonus;
	
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;
	std::list<Entity*>::iterator itBulles;
	std::list<Entity*>::iterator itBrick;
	std::list<Entity*>::iterator iter;
	std::list<Entity*>::iterator itEnemies;
	std::list<Entity*>::iterator itBonus;

	Object player;
	Object targetDeagle;
	vector<Object> bonus;
	vector<Object> enemies;
	std::vector<Object> e;
	std::vector<Object> brick;
	//vector<Image> imagesBonus;


	objectLevel(Level &lvl, Player &p, images *im);
};