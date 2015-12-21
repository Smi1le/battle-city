//#include <SFML/Graphics.hpp>
//#define _CrtDumpMemoryLeaks
#include "updates.h"

using namespace sf;
using namespace std;

void changeDirection(float Dx, float Dy, Entity* &it) {
	int randomNumber;
	rangeValuesRandomly(randomNumber, 4);
	if (Dx > 0) { //Right
		if (randomNumber == 2) {
			it->state = it->LEFT; it->speed = 0.1f;
			it->coordinates.x -= 5;

		}
		if (randomNumber == 3) {
			it->state = it->DOWN; it->speed = 0.1f;
			it->coordinates.x -= 5;
		}
		else if (randomNumber == 4) {
			it->state = it->UP; it->speed = 0.1f;
			it->coordinates.x -= 5;
		}
	}
	if (Dx < 0) { //Left
		if (randomNumber == 1) {
			it->state = it->RIGHT; it->speed = 0.1f;
			it->coordinates.x += 5;
		}
		if (randomNumber == 3) {
			it->state = it->DOWN; it->speed = 0.1f;
			it->coordinates.x += 5;
		}
		else if (randomNumber == 4) {
			it->state = it->UP; it->speed = 0.1f;
			it->coordinates.x += 5;
		}
	}
	if (Dy < 0) { //Up
		if (randomNumber == 1) {
			it->state = it->RIGHT; it->speed = 0.1f;
			it->coordinates.y += 5;
		}
		else if (randomNumber == 2) {
			it->state = it->LEFT; it->speed = 0.1f;
			it->coordinates.y += 5;
		}
		else if (randomNumber == 3) {
			it->state = it->DOWN; it->speed = 0.1f;
			it->coordinates.y += 5;

		}
	}
	if (Dy > 0) { //Down
		if (randomNumber == 1) {
			it->state = it->RIGHT; it->speed = 0.1f;
			it->coordinates.y -= 5;
		}
		else if (randomNumber == 2) {
			it->state = it->LEFT; it->speed = 0.1f;
			it->coordinates.y -= 5;
		}
		else if (randomNumber == 4) {
			it->state = it->UP; it->speed = 0.1f;
			it->coordinates.y -= 5;
		}
	}
}

void stoping(Entity* &it,Entity* &entity) {
	if (it->diraction.y > 0) { it->coordinates.y = entity->sprite->getPosition().y - it->h; }
	else if (it->diraction.y < 0) { it->coordinates.y = entity->sprite->getPosition().y + entity->h; }
	else if (it->diraction.x > 0) { it->coordinates.x = entity->sprite->getPosition().x - it->w; }
	else if (it->diraction.x < 0) { it->coordinates.x = entity->sprite->getPosition().x + entity->w; }
}

void collisionWithBricks(Entity* &it, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (it->getRect().intersects(brick->getRect()) && it->alive) {
			stoping(it, brick);
			changeDirection(it->diraction.x, it->diraction.y, it);
			it->delay = 0;
		}
	}
}

void enemiesClash(list<Entity*> &entities, Entity* &it) {
	for (Entity* entity : entities) {
		if (entity->getRect().intersects(it->getRect())) {
			if (it->getRect() != entity->getRect()) {
				if ((it->getRect().intersects(entity->getRect())) && (it->name == EASY_ENEMY_NAME) && (entity->name == EASY_ENEMY_NAME)){
					stoping(it, entity);
					if (it->state == it->LEFT) { it->state = it->RIGHT; }
					else if (it->state == it->RIGHT) { it->state = it->LEFT; }
					else if (it->state == it->DOWN) { it->state = it->UP; }
					else if (it->state == it->UP) { it->state = it->DOWN; }
					if (entity->state == entity->LEFT) { entity->state = entity->RIGHT; }
					else if (entity->state == entity->RIGHT) { entity->state = entity->LEFT; }
					else if (entity->state == entity->DOWN) { entity->state = entity->UP; }
					else if (entity->state == entity->UP) { entity->state = entity->DOWN; }
				}
			}
		}
	}
}

void drawing(RenderWindow &window, Player *p, objectLevel *map, images *im, Enemy &targetProtecting, globalBool *id, text &txt) {
	window.clear();
	ostringstream playerHealthString;
	playerHealthString << p->health;
	txt.healph.setString("" + playerHealthString.str());
	map->lvl.Draw(window);
	window.draw(txt.healph);
	for (Entity* brick : map->listBrick) {
		window.draw(*brick->sprite);
	}
	for (Entity* bonus : map->listPointsBonus) {
		window.draw(*bonus->sprite);
	}
	if (!id->g_playerLose) {
		if (id->g_isHit) {
			window.draw(im->bigExplosionSprite);
		}
		else {
			window.draw(*p->sprite);
		}
	}
	else {
		window.draw(txt.youLose);
	}
	if (id->g_playerWin) {
		window.draw(txt.youWin);
	}
	window.draw(*targetProtecting.sprite);
	for (Entity* entity : map->bullesPlayer) {
		window.draw(*entity->sprite);
	}
	for (Entity* entity : map->entities) {
		window.draw(*entity->sprite);
	}
	for (Entity* bulles : map->bullesEnemy) {
		window.draw(*bulles->sprite);
	}
	for (Entity* enemy : map->numberEnemies) {
		window.draw(*enemy->sprite);
	}
	if (id->g_appearanceEnemies) window.draw(im->spriteAppEnemies);
	window.display();
}

void cleaning(Player *p, objectLevel *map, images *im) {
	delete p;
	delete &map->clock;
	delete map;
	delete im;
}

void shotEnemy(objectLevel *map, images *im) {
	for (Entity* entity : map->entities) {
		int random;
		rangeValuesRandomly(random, 100);
		if (entity->alive) {
			if (random == 1 && !entity->bulletReleased) {
				Vector2f coordBullesEnemy = { float(entity->coordinatesGunTank.x), float(entity->coordinatesGunTank.y) };
				Vector2i sizeBullesEnemy = { 8, 8 };
				map->bullesEnemy.push_back(new Enemy(im->Bulles, "bullesEnemy", map->lvl, coordBullesEnemy, sizeBullesEnemy, nullptr, &entity));
				list<Entity*>::iterator itBulles = map->bullesEnemy.end();
				--itBulles;
				(*itBulles)->individualNumber = entity->individualNumber;
				entity->bulletReleased = true;
			}
			enemiesClash(map->entities, entity);
			collisionWithBricks(entity, map->listBrick);
		}
	}
}

void shotPlayer(objectLevel *map, images *im, Player *p) {
	if ((Keyboard::isKeyPressed(Keyboard::RControl)) && map->bullesPlayer.size() == 0) {
		Vector2f coordBullesPlayer = { float(p->coordinatesGunTank.x), float(p->coordinatesGunTank.y) };
		Vector2i sizeBullesPlayer = { 8, 8 };
		map->bullesPlayer.push_back(new Enemy(im->Bulles, "Bulles", map->lvl, coordBullesPlayer, sizeBullesPlayer, p, nullptr));
	}
}

void updates(objectLevel *map, globalBool *id, images *im, float time) {
	updateBullesPlayer(map, id, im, time);
	updateBullesEnemy(map, id, time, im);
	updateBrick(map->listBrick);
	updateEnemy(time, im->bigExplosionTexture, id, map);
}

void collisions(Player *p, objectLevel *map) {
	collisionWithBricksForPlayer(p, map->listBrick, p->changeCourse);
	collisionWithEnemyForPlayer(p, map->entities, p->changeCourse);
}

void checks(globalVariable &var, globalBool *id, Player *p, objectLevel *map, images *im, Enemy &targetProtecting, float time, Object player, RenderWindow &window, int &randomNumber) {
	checkForRespawnBonus(var, id, p, map, im);
	isAppearenceEnemy(var, id, p, map, im, time);
	isHitForPlayer(time, im, player, p, id, map);
	changeCourseTank(randomNumber, var.g_timer, time, map->entities);
	checkPlayerBulles(map, window, targetProtecting, id);
	checkEnemyBulles(map, p, id, window, targetProtecting);
	p->changeCourse = false;
	checkForBonus(p, map);
}

void events(RenderWindow &window) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void basicCycle(globalVariable &var, objectLevel *map, images *im, Object &player, globalBool *id, Enemy &targetProtecting, text &txt, Player *p) {
	int randomNumber;
	rangeValuesRandomly(randomNumber, 50);
	RenderWindow & window = *map->window;
	map->clock.restart();
	while (window.isOpen()) {
		float time = float(map->clock.getElapsedTime().asMicroseconds());
		map->clock.restart();
		time = time / 800.0f;
		var.g_timer += time;
		var.g_timerAppearenceEnemy += time;
		events(window);
		shotPlayer(map, im, p);
		if (!id->g_playerLose && !id->g_playerWin) {
			updates(map, id, im, time);
			shotEnemy(map, im);
			collisions(p, map);
			checks(var, id, p, map, im, targetProtecting, time, player, window, randomNumber);
		}
		drawing(window, p, map, im, targetProtecting, id, txt);
	}

}

int main(){
	
	images * im = new images;
	im->create();
	globalBool *id = new globalBool;
	objectLevel *map = new objectLevel(im);
	Object player = map->lvl.GetObject("player");
	text txt(map);
	Vector2f coordPlayer{ float(player.rect.left), float(player.rect.top) };
	Player *p = new Player(im->hero, "Player", map->lvl, coordPlayer, SIZE_PLAYER, player);
	Vector2f coordEagle = { float(map->targetDeagle.rect.left), float(map->targetDeagle.rect.top) };
	Enemy targetProtecting(im->eagle, "Deagle", map->lvl, coordEagle, SIZE_EAGLE, nullptr, nullptr);
	globalVariable var;
	basicCycle(var, map, im, player, id, targetProtecting, txt, p);
	
	return 0;
}
