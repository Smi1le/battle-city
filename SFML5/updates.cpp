#include "updates.h"



void  explosion(float time, list<Entity*>::iterator it, bool &explosion, Texture &texture, int number) {
	float x = (*it)->sprite->getPosition().x;
	float y = (*it)->sprite->getPosition().y;
	delete (*it)->sprite;
	(*it)->sprite = new Sprite;
	(*it)->sprite->setTexture(texture);
	(*it)->sprite->setScale(3.0f, 3.0f);
	if (number == 2) {
		if ((*it)->frame < number) {
			(*it)->sprite->setOrigin(13.0f / 2.0f, 12.0f / 2.0f);
			(*it)->sprite->setTextureRect(IntRect(14 * int((*it)->frame), 0, 13, 12));
			(*it)->sprite->setPosition(x, y);
		}
		else {
			(*it)->frame = 0;
			explosion = false;
		}
	}
	else {
		if ((*it)->frame < 2) {
			(*it)->sprite->setOrigin(13.0f / 2.0f, 12.0f / 2.0f);
			(*it)->sprite->setTextureRect(IntRect(14 * int((*it)->frame), 0, 13, 12));
			(*it)->sprite->setPosition(x, y);
		}
		else if ((*it)->frame < 3) {
			(*it)->sprite->setOrigin(31.0f / 2.0f, 28.0f / 2.0f);
			(*it)->sprite->setTextureRect(IntRect(29, 0, 31, 28));
			(*it)->sprite->setPosition(x, y);
		}
		else {
			(*it)->currentFrame = 0;
			explosion = false;
		}
	}
	(*it)->frame += 0.005f * time;;
}

void updateBullesPlayer(objectLevel *map, globalBool *id, images *im, float &time) {
	for (list<Entity*>::iterator iter = map->bullesPlayer.begin(); iter != map->bullesPlayer.end();) {
		if (!(*iter)->alive) {
			id->g_explPlayerBulles = true;
			if (id->g_explPlayerBulles) {
				explosion(time, iter, id->g_explPlayerBulles, im->explTexture, 2);
				iter++;
			}
			if (!id->g_explPlayerBulles) {
				iter--;
				delete (*iter);
				iter = map->bullesPlayer.erase(iter);
			}
		}
		else {
			(*iter)->update(time, map->obj);
			if (map->bullesPlayer.size() != 0) iter++;
		}
	}
}


void updateBullesEnemy(objectLevel *map, globalBool *id, float &time, images *im) {
	for (list<Entity*>::iterator itBulles = map->bullesEnemy.begin(); itBulles != map->bullesEnemy.end();) {
		cout << "(*itBulles)->alive = " << (*itBulles)->alive << endl;
		if (!(*itBulles)->alive) {
			for (list<Entity*>::iterator it = map->entities.begin(); it != map->entities.end(); it++) {
				if ((*it)->individualNumber == (*itBulles)->individualNumber) {
					(*it)->bulletReleased = (*itBulles)->bulletReleased;
				}
			}
			id->g_explEnemyBulles = true;
			if (id->g_explEnemyBulles) {
				explosion(time, itBulles, id->g_explEnemyBulles, im->explTexture, 2);
				itBulles++;
			}
			if (!id->g_explEnemyBulles) {
				itBulles--;
				delete *itBulles;
				itBulles = map->bullesEnemy.erase(itBulles);
			}
		}
		else {
			cout << "///////////" << endl;
			(*itBulles)->update(time, map->obj);
			if (map->bullesEnemy.size() != 0) {
				itBulles++;
			}
		}
	}
}

void updateBrick(list<Entity*> &listBrick) {
	list<Entity*>::iterator itBrick;
	for (itBrick = listBrick.begin(); itBrick != listBrick.end();) {
		if ((*itBrick)->alive == false) { delete (*itBrick); itBrick = listBrick.erase(itBrick); }
		else itBrick++;
	}
}

void updateEnemy(float time, Texture &explTexture, globalBool *id, objectLevel *map) {
	list<Entity*>::iterator it;
	for (it = map->entities.begin(); it != map->entities.end();) {
		if (!(*it)->alive) {
			id->g_explEnemy = true;
			if (id->g_explEnemy) {
				explosion(time, it, id->g_explEnemy, explTexture, 3);
				it++;
			}
			if (!id->g_explEnemy) {
				it--;
				delete (*it);
				it = map->entities.erase(it);
			}
			if (!id->g_explEnemy && id->g_playerWinTime && map->entities.size() == 0) {
				id->g_playerWin = true;
			}
		}
		else {
			(*it)->update(time, map->obj);
			it++;
		}
	}
}