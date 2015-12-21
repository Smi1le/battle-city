#include "checks.h"

void rangeValuesRandomly(int &number, int upperLimit) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, upperLimit);
	number = dist(gen);
}

void randomDirect(Entity* &entity, float Dx, float Dy) {
	int randomNumber;
	rangeValuesRandomly(randomNumber, 4);
	if (randomNumber == 1) {
		entity->state = entity->RIGHT; entity->speed = 0.1f;
	}
	else if (randomNumber == 2) {
		entity->state = entity->LEFT; entity->speed = 0.1f;
	}
	else if (randomNumber == 3) {
		entity->state = entity->UP; entity->speed = 0.1f;
	}
	else if (randomNumber == 4) {
		entity->state = entity->DOWN; entity->speed = 0.1f;
	}
}

void explosionsPlayer(float time, bool &explosion, Sprite &sprite, Player *p) {
	float x = p->sprite->getPosition().x;
	float y = p->sprite->getPosition().y;
	p->frame += 0.005f * time;
	sprite.setScale(3.0f, 3.0f);
	if (p->frame < 2) {
		sprite.setOrigin(13.0f / 2.0f, 12.0f / 2.0f);
		sprite.setTextureRect(IntRect(14 * int(p->frame), 0, 13, 12));
		sprite.setPosition(x, y);
	}
	else if (p->frame < 3) {
		sprite.setOrigin(31.0f / 2.0f, 28.0f / 2.0f);
		sprite.setTextureRect(IntRect(29, 0, 31, 28));
		sprite.setPosition(x, y);
	}
	else {
		p->frame = 0;
		explosion = false;
	}
}

void checkHitTheEnemy(Entity* &iter, list<Entity*>  &entities) {
	for (Entity* entity : entities) {
		if (entity->alive && iter->alive) {
			if (entity->getRect().intersects(iter->getRect())) {
				entity->alive = false;
				iter->alive = false;
			}
		}
	}
}

void checkHitTheBrick(Entity* &iter, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (iter->getRect().intersects(brick->getRect())) {
			brick->alive = false;
			iter->alive = false;
		}
	}
}

void checkHitTheBulletEnemy(Entity* &iter, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (iter->getRect().intersects(brick->getRect())) {
			brick->alive = false;
			iter->alive = false;
		}
	}
}

void checkHitTheBaseForPlayer(RenderWindow &window, Enemy targetProtecting, Entity* &iter, bool &lose) {
	if (iter->getRect().intersects(targetProtecting.getRect())) {
		lose = true;
		iter->alive = false;
	}
}

void checkHitByPlayer(Player *p, Entity* &bulles, list<Entity*> &entities, bool &isHit) {
	if (bulles->getRect().intersects(p->getRect())) {
		for (Entity* entity : entities) {
			if (entity->individualNumber == bulles->individualNumber) {
				bulles->alive = false;
				bulles->bulletReleased = false;
				p->health--;
				isHit = true;
			}
		}
	}
}

void checkHitTheBrickPlayer(Entity* &bulles, objectLevel *map) {
	for (Entity* brick : map->listBrick) {
		if (bulles->getRect().intersects(brick->getRect())) {
			for (Entity* entity : map->entities) {
				if (entity->individualNumber == bulles->individualNumber) {
					bulles->bulletReleased = false;
					bulles->alive = false;
					brick->alive = false;
				}
			}
		}
	}
}

void checkHitTheBaseForEnemy(RenderWindow &window, Enemy targetProtecting, Entity* &bulles, bool &lose) {
	if (bulles->getRect().intersects(targetProtecting.getRect())) {
		lose = true;
		bulles->alive = false;
		bulles->bulletReleased = false;
	}
}




void checkForRespawnBonus(globalVariable &var, globalBool *id, Player *p, objectLevel *map, images *im) {
	if (var.g_timerAppearenceEnemy > TIME_RESPAWN_ENEMY && p->numberEnemyTank <= 20) {
		id->g_appearanceEnemies = true;
	}
	if (p->numberEnemyTank > 20) {
		id->g_playerWinTime = true;
	}
	id->g_isBonus = false;
	if (p->numberEnemyTank % 9 == 0 && var.g_countBonus == 0) {
		id->g_isBonus = true;
	}
	if (p->numberEnemyTank % 9 != 0) {
		var.g_countBonus = 0;
	}
	if (id->g_isBonus) {
		Vector2f coordIconBonus = { float(map->bonus[var.g_numberBonus].rect.left), float(map->bonus[var.g_numberBonus].rect.top) };
		Vector2i sizeIconBonus = { 45, 42 };
		map->listPointsBonus.push_back(new Enemy(im->imagesBonus[var.g_numberBonus], "bonus", map->lvl, coordIconBonus, sizeIconBonus, p, nullptr));
		if (var.g_numberBonus == 0) {
			var.g_numberBonus++;
			p->individualNumberBonus++;
		}
		else {
			var.g_numberBonus = 0;
			p->individualNumberBonus = 0;
		}
		++var.g_countBonus;
	}
}

void isAppearenceEnemy(globalVariable &var, globalBool *id, Player *p, objectLevel *map, images *im, float &time) {
	if (id->g_appearanceEnemies) {
		bool flag = false;
		while (!flag) {
			int last = var.g_pointRessurection;
			if (p->sprite->getPosition().x + 22  > map->e[var.g_pointRessurection].rect.left && p->sprite->getPosition().x + 22 < map->e[var.g_pointRessurection].rect.left + 45 &&
				p->sprite->getPosition().y + 22  > map->e[var.g_pointRessurection].rect.top && p->sprite->getPosition().y + 22  < map->e[var.g_pointRessurection].rect.top + 45) {
				if (var.g_pointRessurection == 2) {
					var.g_pointRessurection = 0;
				}
				else {
					++var.g_pointRessurection;
				}
			}
			for (Entity* entity : map->entities) {
				if (entity->sprite->getPosition().x + 22 > map->e[var.g_pointRessurection].rect.left && entity->sprite->getPosition().x + 22 < map->e[var.g_pointRessurection].rect.left + 45 &&
					entity->sprite->getPosition().y + 22 > map->e[var.g_pointRessurection].rect.top && entity->sprite->getPosition().y + 22 < map->e[var.g_pointRessurection].rect.top + 45) {
					if (var.g_pointRessurection == 2) {
						var.g_pointRessurection = 0;
					}
					else {
						++var.g_pointRessurection;
					}
				}
			}
			if (last == var.g_pointRessurection) {
				flag = true;
			}
		}
		if (var.g_curFrame < 1) {
			im->spriteAppEnemies.setTextureRect(IntRect(0, 0, 44, 45));
			im->spriteAppEnemies.setPosition(map->e[var.g_pointRessurection].rect.left, map->e[var.g_pointRessurection].rect.top);
		}
		else {
			im->spriteAppEnemies.setTextureRect(IntRect(44, 0, 46, 45));
			im->spriteAppEnemies.setPosition(map->e[var.g_pointRessurection].rect.left, map->e[var.g_pointRessurection].rect.top);
		}
		if (var.g_curFrame > 2) {
			var.g_count++;
			var.g_curFrame = 0;
		}
		var.g_curFrame += 0.005f * time;
		if (var.g_count == 2) {
			Vector2f coordEnemy = { float(map->e[var.g_pointRessurection].rect.left), float(map->e[var.g_pointRessurection].rect.top) };
			Vector2i sizeEnemy = { 45, 45 };
			map->entities.push_back(new Enemy(im->easyEnemyImage, "EasyEnemy", map->lvl, coordEnemy, sizeEnemy, p, nullptr));
			var.g_timerAppearenceEnemy = 0;

			id->g_appearanceEnemies = false;
			var.g_count = 0;
			p->numberEnemyTank++;
			list<Entity*>::iterator iter;
			iter = map->numberEnemies.begin();
			delete (*iter);
			iter = map->numberEnemies.erase(iter);
			if (var.g_pointRessurection == 2) {
				var.g_pointRessurection = 0;
			}
			else {
				var.g_pointRessurection++;
			}
		}
	}
}

void isHitForPlayer(float &time, images *im, Object &player, Player *p, globalBool *id, objectLevel *map) {
	if (id->g_isHit) {
		id->g_isExplPlayer = true;
		if (id->g_isExplPlayer) {
			explosionsPlayer(time, id->g_isExplPlayer, im->bigExplosionSprite, p);
		}
		if (!id->g_isExplPlayer) {
			id->g_isHit = false;
			p->sprite->setPosition(player.rect.left, player.rect.top);
			p->coordinates.x = player.rect.left;
			p->coordinates.y = player.rect.top;
		}
		if (p->health == -1) {
			p->alive = false;
		}
	}
	else {
		if (p->alive) {
			p->update(time, map->obj);
		}
		else {
			id->g_playerLose = true;
		}
	}
}

void changeCourseTank(int &randomNumber, int &timer, const float time, list<Entity*> &entities) {
	if (randomNumber * 100 < timer) {
		for (Entity* entity : entities) {
			if (entity->alive) randomDirect(entity, entity->diraction.x, entity->diraction.y);
		}
		timer = 0;
		rangeValuesRandomly(randomNumber, 50);
	}
}



void checkPlayerBulles(objectLevel *map, RenderWindow &window, Enemy &targetProtecting, globalBool *id) {
	for (Entity* bulles : map->bullesPlayer) {
		if (bulles->alive) {
			checkHitTheEnemy(bulles, map->entities);
		}
		if (bulles->alive) {
			checkHitTheBrick(bulles, map->listBrick);
		}
		if (bulles->alive) {
			checkHitTheBaseForPlayer(window, targetProtecting, bulles, id->g_playerLose);
		}
		if (bulles->alive) {
			checkHitTheBulletEnemy(bulles, map->bullesEnemy);
		}
	}
}

void checkEnemyBulles(objectLevel *map, Player *p, globalBool *id, RenderWindow &window, Enemy targetProtecting) {
	for (Entity* bulles : map->bullesEnemy) {
		if (bulles->alive) {
			checkHitByPlayer(p, bulles, map->entities, id->g_isHit);
		}
		if (bulles->alive) {
			checkHitTheBaseForEnemy(window, targetProtecting, bulles, id->g_playerLose);
		}
		if (bulles->alive) {
			checkHitTheBrickPlayer(bulles, map);
		}
	}
}

void checkForBonus(Player *p, objectLevel *map) {
	for (list<Entity*>::iterator it = map->listPointsBonus.begin(); it != map->listPointsBonus.end(); it++) {
		if (p->getRect().intersects((*it)->getRect())) {
			if ((*it)->individualNumberBonus == 0) {
				for (list<Entity*>::iterator it2 = map->entities.begin(); it2 != map->entities.end(); it2++) {
					(*it2)->alive = false;
				}

				for (Entity* entity : map->entities) {
					entity->alive = false;
				}
			}
			else {
				++p->health;
			}
			delete *it;
			it = map->listPointsBonus.erase(it);
			if (it == map->listPointsBonus.end()) {
				break;
			}
		}
	}
}