//#include <SFML/Graphics.hpp>
//#define _CrtDumpMemoryLeaks

#include "updates.h"
#include "mainEngine.h"
#include "Menu.h"
#include "restart.h"





using namespace sf;
using namespace std;


void changeDirection_Right(int randomNumber, Entity* &it) {
	if (randomNumber == 2) {
		it->state = it->LEFT; it->speed = 0.1f;
		it->coordinates.x -= 5;
		it->speedBulles = { -0.15f, 0.0f };

	}
	if (randomNumber == 3) {
		it->state = it->DOWN; it->speed = 0.1f;
		it->coordinates.x -= 5;
		it->speedBulles = { 0.0f, 0.15f };
	}
	else if (randomNumber == 4) {
		it->state = it->UP; it->speed = 0.1f;
		it->coordinates.x -= 5;
		it->speedBulles = { 0.0f, -0.15f };
	}
}

void changeDirection_Left(int randomNumber, Entity* &it) {
	if (randomNumber == 1) {
		it->state = it->RIGHT; it->speed = 0.1f;
		it->coordinates.x += 5;
		it->speedBulles = { 0.15f, 0.0f };
	}
	if (randomNumber == 3) {
		it->state = it->DOWN; it->speed = 0.1f;
		it->coordinates.x += 5;
		it->speedBulles = { 0.0f, 0.15f };
	}
	else if (randomNumber == 4) {
		it->state = it->UP; it->speed = 0.1f;
		it->coordinates.x += 5;
		it->speedBulles = { 0.0f, -0.15f };
	}
}

void changeDirection_Up(int randomNumber, Entity* &it) {
	if (randomNumber == 1) {
		it->state = it->RIGHT; it->speed = 0.1f;
		it->coordinates.y += 5;
		it->speedBulles = { 0.15f, 0.0f };
	}
	else if (randomNumber == 2) {
		it->state = it->LEFT; it->speed = 0.1f;
		it->coordinates.y += 5;
		it->speedBulles = { -0.15f, 0.0f };
	}
	else if (randomNumber == 3) {
		it->state = it->DOWN; it->speed = 0.1f;
		it->coordinates.y += 5;
		it->speedBulles = { 0.0f, 0.15f };

	}
}

void changeDirection_Down(int randomNumber, Entity* &it) {
	if (randomNumber == 1) {
		it->state = it->RIGHT; it->speed = 0.1f;
		it->coordinates.y -= 5;
		it->speedBulles = { 0.15f, 0.0f };
	}
	else if (randomNumber == 2) {
		it->state = it->LEFT; it->speed = 0.1f;
		it->coordinates.y -= 5;
		it->speedBulles = { -0.15f, 0.0f };
	}
	else if (randomNumber == 4) {
		it->state = it->UP; it->speed = 0.1f;
		it->coordinates.y -= 5;
		it->speedBulles = { 0.0f, -0.15f };
	}
}

void changeDirection(float Dx, float Dy, Entity* &it) {
	int randomNumber;
	rangeValuesRandomly(randomNumber, 4);
	if (Dx > 0) { //Right
		changeDirection_Right(randomNumber, it);
	}
	if (Dx < 0) { //Left
		changeDirection_Left(randomNumber, it);
	}
	if (Dy < 0) { //Up
		changeDirection_Up(randomNumber, it);
	}
	if (Dy > 0) { //Down
		changeDirection_Down(randomNumber, it);
	}
}

void stoping(Entity* &it,Entity* &entity) {
	if (it->diraction.y > 0) { 
		it->coordinates.y = entity->sprite->getPosition().y - it->h; 
	}
	else if (it->diraction.y < 0) {
		it->coordinates.y = entity->sprite->getPosition().y + entity->h; 
	}
	else if (it->diraction.x > 0) { 
		it->coordinates.x = entity->sprite->getPosition().x - it->w;
	}
	else if (it->diraction.x < 0) { 
		it->coordinates.x = entity->sprite->getPosition().x + entity->w; 
	}
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

void stopingWithEnemyClash(Entity* &it, Entity* &entity) {
	stoping(it, entity);
	if (it->state == it->RIGHT)
		it->state = it->LEFT;
	else if (it->state == it->LEFT)
		it->state = it->RIGHT;
	else if (it->state == it->UP)
		it->state = it->DOWN;
	else if (it->state == it->DOWN)
		it->state = it->UP;
	if (entity->state == entity->RIGHT)
		entity->state = entity->LEFT;
	else if (entity->state == entity->LEFT)
		entity->state = entity->RIGHT;
	else if (entity->state == entity->UP)
		entity->state = entity->DOWN;
	else if (entity->state == entity->DOWN)
		entity->state = entity->UP;
}

void enemiesClash(list<Entity*> &entities, Entity* &it) {
	for (Entity* entity : entities) {
		if (entity->getRect().intersects(it->getRect())) {
			if (it->getRect() != entity->getRect()) {
				if ((it->getRect().intersects(entity->getRect())) && ((it->name == EASY_ENEMY_NAME) || (it->name == SPEED_ENEMY_NAME)) && 
					((entity->name == EASY_ENEMY_NAME) || (entity->name == SPEED_ENEMY_NAME))){
					stopingWithEnemyClash(it, entity);
				}
			}
		}
	}
}

void checkForWinOrLosePlayer(globalBool *id, RenderWindow &window, images *im, text *txt, Player *p) {
	if (!id->g_playerLose) {
		if (id->g_isHit) {
			window.draw(im->bigExplosionSprite);
		}
		else {
			window.draw(*p->sprite);
		}
	}
	else {
		window.draw(txt->youLose);
		window.draw(txt->escape);
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
			id->g_isMenu = true;
		}
	}
	if (id->g_playerWin) {
		window.draw(txt->youWin);
		window.draw(txt->escape);
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
			id->g_isMenu = true;
		}
	}
}

void drawing(RenderWindow &window, engine *en) {
	window.clear();
	ostringstream playerHealthString;
	playerHealthString << en->p->health;
	en->txt->healph.setString("" + playerHealthString.str());
	en->map->lvl.Draw(window);
	window.draw(en->txt->healph);
	for (Entity* brick : en->map->listBrick) {
		window.draw(*brick->sprite);
	}
	for (Entity* bonus : en->map->listPointsBonus) {
		window.draw(*bonus->sprite);
	}
	checkForWinOrLosePlayer(en->id, window, en->im, en->txt, en->p);
	window.draw(*en->targetProtecting->sprite);
	for (Entity* entity : en->map->bullesPlayer) {
		window.draw(*entity->sprite);
	}
	for (Entity* entity : en->map->entities) {
		window.draw(*entity->sprite);
	}
	for (Entity* bulles : en->map->bullesEnemy) {
		window.draw(*bulles->sprite);
	}
	for (Entity* enemy : en->map->numberEnemies) {
		window.draw(*enemy->sprite);
	}
	if (en->id->g_appearanceEnemies) window.draw(en->im->spriteAppEnemies);
	window.display();
}

void cleaning(engine *en) {
	delete en->p->sprite;
	delete en->p->texture;
	delete en->p;
	delete en->map;
	delete en->im;
	delete en->id;
	delete en->txt;
	delete en->targetProtecting;
	delete en->var;
	delete en->sound;
	delete en;
}

void shotEnemy(engine *en) {
	for (Entity* entity : en->map->entities) {
		int random;
		rangeValuesRandomly(random, FREQUENCY_SHOOT_BULLES);
		if (entity->alive) {
			if (random == 1 && !entity->bulletReleased) {
				Vector2f coordBullesEnemy = { float(entity->coordinatesGunTank.x), float(entity->coordinatesGunTank.y) };
				en->map->bullesEnemy.push_back(new Enemy(en->im->Bulles, "bullesEnemy", en->map->lvl, coordBullesEnemy, SIZE_BULLES_ENEMY, nullptr, &entity));
				en->sound->soundShoot.play();
				list<Entity*>::iterator itBulles = en->map->bullesEnemy.end();
				--itBulles;
				(*itBulles)->individualNumber = entity->individualNumber;
				entity->bulletReleased = true;
			}
			enemiesClash(en->map->entities, entity);
			collisionWithBricks(entity, en->map->listBrick);
		}
	}
}

void shootPlayer(engine *en) {
	if ((Keyboard::isKeyPressed(Keyboard::RControl)) && en->map->bullesPlayer.size() == 0) {
		Vector2f coordBullesPlayer = { float(en->p->coordinatesGunTank.x), float(en->p->coordinatesGunTank.y) };
		en->map->bullesPlayer.push_back(new Enemy(en->im->Bulles, "Bulles", en->map->lvl, coordBullesPlayer, SIZE_BULLES_PLAYER, en->p, nullptr));
		en->sound->soundShoot.play();
	}
}

void updates(engine *en, float time) {
	updateBullesPlayer(en->map, en->id, en->im, time, en->sound);
	updateBullesEnemy(en->map, en->id, time, en->im, en->sound);
	updateBrick(en->map->listBrick);
	updateEnemy(time, en->im->bigExplosionTexture, en->id, en->map, en->sound);
}

void collisions(Player *p, objectLevel *map) {
	collisionWithBricksForPlayer(p, map->listBrick, p->changeCourse);
	collisionWithEnemyForPlayer(p, map->entities, p->changeCourse);
}

void checks(float time, RenderWindow &window, int &randomNumber, engine *en) {
	checkForRespawnBonus(en->var, en->id, en->p, en->map, en->im);
	isAppearenceEnemy(en->var, en->id, en->p, en->map, en->im, time);
	isHitForPlayer(time, en->im, en->player, en->p, en->id, en->map, en->sound);
	changeCourseTank(randomNumber, en->var->g_timer, time, en->map->entities);
	checkPlayerBulles(en->map, window, en->targetProtecting, en->id, en->sound);
	checkEnemyBulles(en->map, en->p, en->id, window, en->targetProtecting, en->sound);
	en->p->changeCourse = false;
	checkForBonus(en->p, en->map, en->sound);
}

void events(RenderWindow &window, globalBool *id) {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed)
			window.close();
		if (Keyboard::isKeyPressed(Keyboard::RAlt)) {
			if (!id->g_isPause) {
				id->g_isPause = true;
			}
			else if (id->g_isPause) {
				id->g_isPause = false;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {	
			id->g_isMenu = true;
		}
	}
}




void basicCycle(engine *en) {
	int randomNumber;
	rangeValuesRandomly(randomNumber, 50);
	RenderWindow &window = *en->map->window;
	en->map->clock.restart();
	while (window.isOpen()) {
		if (en->id->g_isMenu) {
			gameMenu(window, en);
			en->map->clock.restart();
		}
		if (!en->id->g_isMenu) {
			if (en->sound->mainTheme.getStatus() != en->sound->mainTheme.Playing) {
				en->sound->mainTheme.play();
			}
			
			float time = float(en->map->clock.getElapsedTime().asMicroseconds());
			en->map->clock.restart();
			time = time / 800.0f;
			events(window, en->id);
			if (en->id->g_isRestart) {
				restart(en->p, en->im, en->map, en->id, en->var);
				en->id->g_isRestart = false;
			}
			if (!en->id->g_playerLose && !en->id->g_playerWin && !en->id->g_isPause) {
				en->p->permittedMovementOptions = vector<int>(4, 0);
				en->p->isBlock = false;
				shootPlayer(en);
				en->var->g_timer += time;
				en->var->g_timerAppearenceEnemy += time;
				updates(en, time);
				shotEnemy(en);
				collisions(en->p, en->map);
				checks(time, window, randomNumber, en);
			}
			drawing(window, en);
			if (en->id->g_isMenu) {
				en->sound->mainTheme.stop();
				if (en->sound->isMove.getStatus() == en->sound->isMove.Playing) {
					en->sound->isMove.stop();
				}
			}
		}
	}
}

int main(){
	engine *en = new engine;
	en->sound->volumeSounds(VOLUME);
	basicCycle(en);
	cleaning(en);
	return 0;
}
