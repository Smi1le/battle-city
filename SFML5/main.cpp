#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <random>
#include "level.h"
#include <vector>
#include <list>
#include <ctime>
#include <crtdbg.h>
#include "config.h"


using namespace sf;
using namespace std;

class Entity {
public:
	std::vector<Object> obj;
	float speed;
	float CurrentFrame;
	float delay;
	int w;
	int h;
	int health;
	int individualNumber;
	int individualNumberBonus;
	int numberEnemyTank = 1;
	enum { LEFT, RIGHT, UP, DOWN, STAY } state;
	Vector2i coordinatesGunTank;
	bool life;
	bool isMove;
	bool onGround;
	bool bulletReleased = false;
	Vector2f coordinates;
	Vector2f diraction;
	Vector2f speedBulles;
	Texture *texture;
	Sprite *sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H, Vector2i coordinatesGunTank) {
		coordinates.x = X; coordinates.y = Y; w = W; h = H; name = Name;
		speed = 0; health = 100; diraction.x = 0; diraction.y = 0;
		life = true; onGround = false; isMove = false;
		texture = new Texture;
		sprite = new Sprite;
		texture->loadFromImage(image);
		sprite->setTexture(*texture);
	}

	FloatRect getRect() {
		return FloatRect(coordinates.x, coordinates.y, float(w), float(h)); 
	}

	virtual void update(float time) = 0;

};

class Player : public Entity {
public:
	
	int playerScore;
	bool cheats = false;
	Player(Image &image, String name, Level &lev, float X, float Y, int W, int H, Object player) :Entity(image, name, X, Y, W, H, coordinatesGunTank) {
		obj = lev.GetAllObjects();
		playerScore = 0; state = STAY;
		health = 2;
		if (name == PLAYER_NAME) {
			sprite->setTextureRect(IntRect(0, 0, w, h));
			coordinatesGunTank.x = int(player.rect.left) + w;
			coordinatesGunTank.y = int(player.rect.top) + h / 2;
		}
	}

	void control(float time) {
		if (CurrentFrame > 2) (CurrentFrame = 0);
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				state = LEFT; speed = 0.1f;
				sprite->setTextureRect(IntRect(145, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = coordinates.x;
				coordinatesGunTank.y = coordinates.y + h / 2;
				speedBulles.x = -0.15f;
				speedBulles.y = 0.0f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = RIGHT; speed = 0.1f;
				sprite->setTextureRect(IntRect(0, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = coordinates.x + w;
				coordinatesGunTank.y = coordinates.y + h / 2;
				speedBulles.x = 0.15f;
				speedBulles.y = 0.0f;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {
				state = UP; speed = 0.1f;
				sprite->setTextureRect(IntRect(100, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = coordinates.x + w / 2;
				coordinatesGunTank.y = coordinates.y;
				speedBulles.x = 0.0f;
				speedBulles.y = -0.15f;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = DOWN; speed = 0.1f;
				sprite->setTextureRect(IntRect(50, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = coordinates.x + w / 2;
				coordinatesGunTank.y = coordinates.y + h;
				speedBulles.x = 0.0f;
				speedBulles.y = 0.15f;

			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i < int(obj.size()); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == SOLID_NAME)
				{
					if (Dy > 0) { coordinates.y = obj[i].rect.top - h;  diraction.y = 0; }
					if (Dy < 0) { coordinates.y = obj[i].rect.top + obj[i].rect.height;   diraction.y = 0; }
					if (Dx > 0) { coordinates.x = obj[i].rect.left - w; }
					if (Dx < 0) { coordinates.x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time) {
		CurrentFrame += 0.005f * time;
		control(time);
		switch (state) {
		case RIGHT:diraction.x = speed; diraction.y = 0; break;
		case LEFT:diraction.x = -speed; diraction.y = 0; break;
		case UP:diraction.x = 0; diraction.y = -speed; break;
		case DOWN:diraction.x = 0; diraction.y = speed; break;
		case STAY: break;
		}
		coordinates.x += diraction.x*time;
		checkCollisionWithMap(diraction.x, 0);
		coordinates.y += diraction.y*time;
		checkCollisionWithMap(0, diraction.y);
		speed = 0;
		sprite->setPosition(coordinates.x, coordinates.y);
	}
};

class Enemy :public Entity {
public:
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, Player *p, Entity* *entity) :Entity(image, Name, X, Y, W, H, coordinatesGunTank) {
		obj = lvl.GetObjects(SOLID_NAME);
		if (name == EASY_ENEMY_NAME) {
			sprite->setTextureRect(IntRect(51, 0, w, h));
			state = DOWN; speed = 0.1f;
			delay = 2;
			individualNumber = p->numberEnemyTank;
		}
		if (name == DISPLAYS_ENEMY_NAME) {
			sprite->setPosition(coordinates.x, coordinates.y);
		}
		if (name == PLAYER_BULLES_NAME) {
			if ((p->speedBulles.x > 0) && (p->speedBulles.y == 0)) {
				diraction.x = 0.30f;
				diraction.y = 0;
				sprite->setTextureRect(IntRect(26, 2, w, h));
			}
			if ((p->speedBulles.x < 0) && (p->speedBulles.y == 0)) {
				diraction.x = -0.30f;
				diraction.y = 0;
				sprite->setTextureRect(IntRect(16, 2, w, h));
			}
			if ((p->speedBulles.x == 0) && (p->speedBulles.y < 0)) {
				diraction.x = 0;
				diraction.y = -0.30f;
				sprite->setTextureRect(IntRect(0, 0, w, h));
			}
			if ((p->speedBulles.x == 0) && (p->speedBulles.y > 0)) {
				diraction.x = 0;
				diraction.y = 0.30f;
				sprite->setTextureRect(IntRect(7, 2, w, h));
			}
		}
		if (name == EAGLE_NAME){
			sprite->setPosition(X, Y);
		}
		if (name == BRICK_NAME) {
			sprite->setPosition(X, Y);
		}
		if (name == BONUS_NAME){
			sprite->setPosition(coordinates.x, coordinates.y);
			individualNumberBonus = p->individualNumberBonus;
		}
		if (name == ENEMY_BULLES_NAME) {
			bulletReleased = true;

			if (((*entity)->diraction.x > 0) && ((*entity)->diraction.y == 0)) {
				diraction.x = 0.30f;
				diraction.y = 0;
				sprite->setTextureRect(IntRect(26, 2, w, h));
			}
			if (((*entity)->diraction.x < 0) && ((*entity)->diraction.y == 0)) {
				diraction.x = -0.30f;
				diraction.y = 0;
				sprite->setTextureRect(IntRect(16, 2, w, h));
			}
			if (((*entity)->diraction.x == 0) && ((*entity)->diraction.y < 0)) {
				diraction.x = 0;
				diraction.y = -0.30f;
				sprite->setTextureRect(IntRect(0, 0, w, h));
			}
			if (((*entity)->diraction.x == 0) && ((*entity)->diraction.y > 0)) {
				diraction.x = 0;
				diraction.y = 0.30f;
				sprite->setTextureRect(IntRect(7, 2, w, h));
			}
		}
	}

	
	void changeDirection(float Dx, float Dy) {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(1, 3);
		int randomNumber = dist(gen);
		if (Dx > 0) { //Right
			if (randomNumber == 1) {
				state = LEFT; speed = 0.1f;
				coordinates.x -= 2;
			}
			if (randomNumber == 2) {
				state = DOWN; speed = 0.1f;
				coordinates.x -= 2;
			}
			else if (randomNumber == 3) {
				state = UP; speed = 0.1f;
				coordinates.x -= 2;
			}
		}
		if (Dx < 0) { //Left
			if (randomNumber == 1) {
				state = RIGHT; speed = 0.1f;
				coordinates.x += 2;
			}
			if (randomNumber == 2) {
				state = DOWN; speed = 0.1f;
				coordinates.x += 2;
			}
			else if (randomNumber == 3) {
				state = UP; speed = 0.1f;
				coordinates.x += 2;
			}
		}
		if (Dy < 0) { //Up
			if (randomNumber == 1) {
				state = RIGHT; speed = 0.1f;
				coordinates.y += 2;
			}
			else if (randomNumber == 2) {
				state = LEFT; speed = 0.1f;
				coordinates.y += 2;
			}
			else if (randomNumber == 3) {
				state = DOWN; speed = 0.1f;
				coordinates.y += 2;
			}
		}
		if (Dy > 0) { //Down
			if (randomNumber == 1) {
				state = RIGHT; speed = 0.1f;
				coordinates.y -= 2;
			}
			else if (randomNumber == 2) {
				state = LEFT; speed = 0.1f;
				coordinates.y -= 2;
			}
			else if (randomNumber == 3) {
				state = UP; speed = 0.1f;
				coordinates.y -= 2;
			}
		}
	}
	
	void checkCollisionWithMap(float time)
	{
		for (int i = 0; i < int(obj.size()); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == SOLID_NAME) {
					if (name == EASY_ENEMY_NAME) {
						if (diraction.y > 0) { coordinates.y = obj[i].rect.top - h; }
						if (diraction.y < 0) { coordinates.y = obj[i].rect.top + obj[i].rect.height; }
						if (diraction.x > 0) { coordinates.x = obj[i].rect.left - w; }
						if (diraction.x < 0) { coordinates.x = obj[i].rect.left + obj[i].rect.width; }
						changeDirection(diraction.x, diraction.y);
						delay = 0;
					}
					if (name == PLAYER_BULLES_NAME || name == ENEMY_BULLES_NAME) {
						life = false;
						bulletReleased = false;
					}
				}
			}
	}	
	void animation(int X, int Y, float time, int w, int h, string dir) {
		if (CurrentFrame > 2) (CurrentFrame = 0);
		sprite->setTextureRect(IntRect(X, Y * int(CurrentFrame), w, h));
		if (dir == RIGHT_DIR_NAME) {
			coordinatesGunTank.x = coordinates.x + float(w);
			coordinatesGunTank.y = coordinates.y + float(h) / 2.0f;
		}
		else if (dir == LEFT_DIR_NAME) {
			coordinatesGunTank.x = coordinates.x;
			coordinatesGunTank.y = coordinates.y + float(h) / 2.0f;
		}
		else if (dir == UP_DIR_NAME) {
			coordinatesGunTank.x = coordinates.x + float(w) / 2.0f;
			coordinatesGunTank.y = coordinates.y;
		}
		else if (dir == DOWN_DIR_NAME) {
			coordinatesGunTank.x = coordinates.x + float(w) / 2.0f;
			coordinatesGunTank.y = coordinates.y + float(h);
		}
	}
	void update(float time)
	{
		if (life) {
			if (name == EASY_ENEMY_NAME) {
				delay += 0.005f* time;
				if (delay > 1) {
					CurrentFrame += 0.005f * time;
					switch (state) {
					case RIGHT:diraction.x = speed; diraction.y = 0; animation(0, 51, time, 44, 44, "RIGHT"); break;
					case LEFT:diraction.x = -speed; diraction.y = 0; animation(141, 51, time, 44, 44, "LEFT"); break;
					case UP:diraction.x = 0; diraction.y = -speed; animation(92, 51, time, 44, 44, "UP"); break;
					case DOWN:diraction.x = 0; diraction.y = speed; animation(48, 51, time, 44, 44, "DOWN"); break;
					case STAY: break;
					}
					coordinates.x += diraction.x * time;
					coordinates.y += diraction.y * time;
					sprite->setPosition(coordinates.x, coordinates.y);
					checkCollisionWithMap(time);
				}
				if (health <= 0) { life = false; }
			}
			if (name == PLAYER_BULLES_NAME || name == ENEMY_BULLES_NAME) {
				checkCollisionWithMap(time);
				coordinates.x += diraction.x * time;
				coordinates.y += diraction.y * time;
				sprite->setPosition(coordinates.x, coordinates.y);
			}
		}
	}
};

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

void changeCourseTank(int &randomNumber, int &timer, const float time, list<Entity*> &entities) {
	if (randomNumber * 100 < timer) {
		for (Entity* entity : entities) {
			if (entity->life) randomDirect(entity, entity->diraction.x, entity->diraction.y);
		}
		timer = 0;
		rangeValuesRandomly(randomNumber, 50);
	}
}


void updateBrick(list<Entity*> &listBrick) {
	list<Entity*>::iterator itBrick;
	for (itBrick = listBrick.begin(); itBrick != listBrick.end();) {
		if ((*itBrick)->life == false) { delete (*itBrick); itBrick = listBrick.erase(itBrick); }
		else itBrick++;
	}
}

void explosionsPlayer(float time, float &frame, bool &explosion, Sprite &sprite, Player p) {
	float x = p.sprite->getPosition().x;
	float y = p.sprite->getPosition().y;
	frame += 0.005f * time;
	sprite.setScale(3.0f, 3.0f);
	if (frame < 2) {
		sprite.setOrigin(13.0f / 2.0f, 12.0f / 2.0f);
		sprite.setTextureRect(IntRect(14 * int(frame), 0, 13, 12));
		sprite.setPosition(x, y);
	}
	else if (frame < 3) {
		sprite.setOrigin(31.0f / 2.0f, 28.0f / 2.0f);
		sprite.setTextureRect(IntRect(29, 0, 31, 28));
		sprite.setPosition(x, y);
	}
	else {
		frame = 0;
		explosion = false;
	}
}

void  explosion(float time, list<Entity*>::iterator it, float &currentFrame, bool &explosion, Texture &texture, int number) {
	float x = (*it)->sprite->getPosition().x;
	float y = (*it)->sprite->getPosition().y;
	delete (*it)->sprite;
	(*it)->sprite = new Sprite;
	(*it)->sprite->setTexture(texture);
	(*it)->sprite->setScale(3.0f, 3.0f);
	if (number == 2){
		if (currentFrame < number) {
			(*it)->sprite->setOrigin(13.0f / 2.0f, 12.0f / 2.0f);
			(*it)->sprite->setTextureRect(IntRect(14 * int(currentFrame), 0, 13, 12));
			(*it)->sprite->setPosition(x, y);
		}
		else {
			currentFrame = 0;
			explosion = false;
		}
	}
	else {
		if (currentFrame < 2) {
			(*it)->sprite->setOrigin(13.0f / 2.0f, 12.0f / 2.0f);
			(*it)->sprite->setTextureRect(IntRect(14 * int(currentFrame), 0, 13, 12));
			(*it)->sprite->setPosition(x, y);
		}
		else if (currentFrame < 3) {
			(*it)->sprite->setOrigin(31.0f / 2.0f, 28.0f / 2.0f);
			(*it)->sprite->setTextureRect(IntRect(29, 0, 31, 28));
			(*it)->sprite->setPosition(x, y);
		}
		else {
			currentFrame = 0;
			explosion = false;
		}
	}
	currentFrame += 0.005f * time;;
}

void updateEnemy(float time, list<Entity*> &entities, bool &explEnemy, float &currentFrame, Texture &explTexture, bool &playerWinTime, bool &playerWin) {
	list<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end();) {
		if (!(*it)->life) {
			explEnemy = true;
			if (explEnemy) {
				explosion(time, it, currentFrame, explEnemy, explTexture, 3);
				it++;
			}
			if (!explEnemy) {
				it--;
				delete (*it);
				it = entities.erase(it);
			}
			if (!explEnemy && playerWinTime && entities.size() == 0) {
				playerWin = true;
			}
		}
		else {
				(*it)->update(time);
				it++;
		}
	}
}

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
	if (it->diraction.y < 0) { it->coordinates.y = entity->sprite->getPosition().y + entity->h; }
	if (it->diraction.x > 0) { it->coordinates.x = entity->sprite->getPosition().x - it->w; }
	if (it->diraction.x < 0) { it->coordinates.x = entity->sprite->getPosition().x + entity->w; }
}

void collisionWithBricks(Entity* &it, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (it->getRect().intersects(brick->getRect()) && it->life) {
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



void checkCollisionWithMap(Player &p, Entity* brick) {
	if (p.diraction.y > 0) { p.coordinates.y = brick->sprite->getPosition().y - p.h;  p.diraction.y = 0; }
	if (p.diraction.y < 0) { p.coordinates.y = brick->sprite->getPosition().y + brick->h;   p.diraction.y = 0; }
	if (p.diraction.x > 0) { p.coordinates.x = brick->sprite->getPosition().x - p.w; }
	if (p.diraction.x < 0) { p.coordinates.x = brick->sprite->getPosition().x + brick->w; }
}

void collisionWithBricksForPlayer(Player &p, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (p.getRect().intersects(brick->getRect())) {
			checkCollisionWithMap(p, brick);
		}
	}
}

void collisionWithEnemyForPlayer(Player &p, list<Entity*> &entities) {
	for (Entity* entity : entities) {
		if (entity->getRect().intersects(p.getRect()) && entity->life) {
			checkCollisionWithMap(p, entity);
			entity->speed = 0;
		}
		else {
			entity->speed = 0.1f;
		}
	}
}

void checkHitTheEnemy(Entity* &iter, list<Entity*>  &entities) {
	for (Entity* entity : entities) {
		if (entity->life && iter->life) {
			if (entity->getRect().intersects(iter->getRect())) {
				entity->life = false;
				iter->life = false;
			}
		}
	}
}

void checkHitTheBrick(Entity* &iter, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (iter->getRect().intersects(brick->getRect())) {
			brick->life = false;
			iter->life = false;
		}
	}
}

void checkHitTheBulletEnemy(Entity* &iter, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (iter->getRect().intersects(brick->getRect())) {
			brick->life = false;
			iter->life = false;
		}
	}
}

void checkHitTheBaseForPlayer(RenderWindow &window, Enemy targetProtecting, Entity* &iter, bool &lose) {
	if (iter->getRect().intersects(targetProtecting.getRect())) {
		lose = true;
		iter->life = false;
	}
}

void checkHitByPlayer(Player &p, Entity* &bulles, list<Entity*> &entities, bool &isHit) {
	if (bulles->getRect().intersects(p.getRect())) {
		for (Entity* entity : entities) {
			if (entity->individualNumber == bulles->individualNumber) {
				bulles->life = false;
				bulles->bulletReleased = false;
				p.health--;
				isHit = true;
			}
		}
	}
}

void checkHitTheBaseForEnemy(RenderWindow &window, Enemy targetProtecting, Entity* &bulles, bool &lose) {
	if (bulles->getRect().intersects(targetProtecting.getRect())) {
		lose = true;
		bulles->life = false;
		bulles->bulletReleased = false;
	}
}

void checkHitTheBrickPlayer(list<Entity*> &listBrick, list<Entity*> &entities, Entity* &bulles) {
	for (Entity* brick : listBrick) {
		if (bulles->getRect().intersects(brick->getRect())) {
			for (Entity* entity : entities) {
				if (entity->individualNumber == bulles->individualNumber) {
					bulles->bulletReleased = false;
					bulles->life = false;
					brick->life = false;
				}
			}
		}
	}
}

void checkForBonus(Player &p, list<Entity*> &listPointsBonus, list<Entity*> &entities) {
	int numberBonus = 1;
	for (list<Entity*>::iterator it = listPointsBonus.begin(); it != listPointsBonus.end(); it++) {
		if (p.getRect().intersects((*it)->getRect())) {
			if ((*it)->individualNumberBonus == 0) {
				for (list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++) {
					(*it2)->life = false;
				}
			}
			else {
				++p.health;
			}
			delete *it;
			it = listPointsBonus.erase(it);
			if (it == listPointsBonus.end()) break;
		}
		numberBonus++;
	}
}

void drawing(RenderWindow &window, Player p, list<Entity*> bullesPlayer, list<Entity*> entities, list<Entity*> bullesEnemy, list<Entity*> listBrick, Level lvl, Enemy targetProtecting, list<Entity*> enemies, bool app, Sprite sprite, Text text, bool isHit, Sprite bigExplosion, bool lose, Text youLose, bool win, Text youWin, list<Entity*> imagesBonus) {
	window.clear();
	ostringstream playerHealthString;
	playerHealthString << p.health;
	text.setString("" + playerHealthString.str());
	lvl.Draw(window);
	window.draw(text);
	for (Entity* brick : listBrick) {
		window.draw(*brick->sprite);
	}
	for (Entity* bonus : imagesBonus) {
		window.draw(*bonus->sprite);
	}
	if (!lose) {
		if (isHit) window.draw(bigExplosion);
		else window.draw(*p.sprite);
	}
	else window.draw(youLose);
	if (win) window.draw(youWin);
	window.draw(*targetProtecting.sprite);
	for (Entity* entity : bullesPlayer) {
		window.draw(*entity->sprite);
	}
	for (Entity* entity : entities) {
		window.draw(*entity->sprite);
	}
	for (Entity* bulles : bullesEnemy) {
		window.draw(*bulles->sprite);
	}
	for (Entity* enemy : enemies) {
		window.draw(*enemy->sprite);
	}
	if (app) window.draw(sprite);
	window.display();
}

int main(){
	
	

	Clock clock;
	

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;

	
	std::list<Entity*> bullesEnemy;
	std::list<Entity*>::iterator itBulles;
	
	std::list<Entity*>  listBrick;
	std::list<Entity*>::iterator itBrick;
	

	std::list<Entity*> bullesPlayer;
	std::list<Entity*>::iterator iter;
	
	list<Entity*> numberEnemies;
	list<Entity*>::iterator itEnemies;
	
	list<Entity*> listPointsBonus;
	list<Entity*>::iterator itBonus;



	Level lvl;
	
	lvl.LoadFromFile("recources/levels/map.tmx");


	
	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("recources/images/easyEnemy.png");
	easyEnemyImage.createMaskFromColor(Color(0, 128, 128));

	Image brickImage;
	brickImage.loadFromFile("recources/images/brick.png");
	
	Image Bulles;
	Bulles.loadFromFile("recources/images/Bulles.png");
	Bulles.createMaskFromColor(Color(0, 128, 128));

	Image displaysEnemies;
	displaysEnemies.loadFromFile("recources/images/enemies.png");
	displaysEnemies.createMaskFromColor(Color(109, 109, 109));
	Texture enemiesTexture;
	enemiesTexture.loadFromImage(displaysEnemies);
	
	Image eagle;
	eagle.loadFromFile("recources/images/eagle.png");
	eagle.createMaskFromColor(Color::Black);

	Image hero;
	hero.loadFromFile("recources/images/hero.png");
	hero.createMaskFromColor(Color::Black);

	Image expl;
	expl.loadFromFile("recources/images/explosionBulles.png");
	expl.createMaskFromColor(Color(0, 128, 128));
	Texture explTexture;
	explTexture.loadFromImage(expl);

	Image appearance;
	appearance.loadFromFile("recources/images/appearance.png");
	appearance.createMaskFromColor(Color(0, 128, 128));
	Texture appTexture;
	appTexture.loadFromImage(appearance);
	Sprite spriteAppEnemies;
	spriteAppEnemies.setTexture(appTexture);

	Image bigExplosion;
	bigExplosion.loadFromFile("recources/images/BigExplosion.png");
	bigExplosion.createMaskFromColor(Color(0, 128, 128));
	Texture bigExplosionTexture;
	bigExplosionTexture.loadFromImage(bigExplosion);
	Sprite bigExplosionSprite;
	bigExplosionSprite.setTexture(bigExplosionTexture);

	Image imageBonusBomb;
	Image imageBonusLife;
	imageBonusBomb.loadFromFile("recources/images/bonus-bomb.png");
	imageBonusLife.loadFromFile("recources/images/bonus-life.png");
	imageBonusBomb.createMaskFromColor(Color(34, 177, 76));
	imageBonusLife.createMaskFromColor(Color(34, 177, 76));		
	Object health = lvl.GetObject("health");

	/*--------------------------------------------------------------------------------------------------------*/
	Font font;//шрифт 
	font.loadFromFile("recources/Font/Monoid.ttf");//передаем нашему шрифту файл шрифта

	Text text("", font, 26);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setColor(Color::Black);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(sf::Text::Bold);
	text.setPosition(health.rect.left, health.rect.top);

	Text youLose(" YOU LOSE ", font, 50);
	youLose.setColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	youLose.setStyle(sf::Text::Bold);
	youLose.setPosition(200, SIZE_WINDOW.y / 2);

	Text youWin(" YOU WIN ", font, 50);
	youWin.setColor(Color::Blue);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	youWin.setStyle(sf::Text::Bold);
	youWin.setPosition(200, SIZE_WINDOW.y / 2);
	/*--------------------------------------------------------------------------------------------------------*/
	Object player = lvl.GetObject("player");

	Object targetDeagle = lvl.GetObject("Deagle");

	vector<Object> bonus = lvl.GetObjects("bonus");

	Player p(hero, "Player", lvl, player.rect.left, player.rect.top, 37.0f, 39.0f, player);

	Enemy targetProtecting(eagle, "Deagle", lvl, targetDeagle.rect.left, targetDeagle.rect.top, 50.0f, 50.0f, nullptr, nullptr);

	std::vector<Object> e = lvl.GetObjects("EasyEnemy");
	std::vector<Object> brick = lvl.GetObjects("destroyedSolid");
	vector<Object> enemies = lvl.GetObjects("enemy");
	vector<Image> imagesBonus = {imageBonusBomb,imageBonusLife };
	

	for (int i = 0; i < int(enemies.size()); i++) {
		numberEnemies.push_back(new Enemy(displaysEnemies, "enemy", lvl, enemies[i].rect.left, enemies[i].rect.top, 26, 26, nullptr, nullptr));
	}

	int pointRessurection = 0;
	for (it = entities.begin(); it != entities.end(); it++) {
		(*it)->individualNumber = p.numberEnemyTank;
		p.numberEnemyTank++;
	}
	for (int i = 0; i < int(brick.size()); i++) {
		listBrick.push_back(new Enemy(brickImage, "brick", lvl, brick[i].rect.left, brick[i].rect.top, 26, 25, nullptr, nullptr));
		brick[i].rect.left;//коорд Х
		brick[i].rect.top;//коорд Y
	
	}
	sf::RenderWindow window(sf::VideoMode(SIZE_WINDOW.x, SIZE_WINDOW.y), WINDOW_NAME);
	clock.restart();
	//float respawnTime = 0;
	
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, 50);
	int randomNumber = dist(gen);
	/*--------------------------------------------------*/
	int timer = 0;
	float currentFrame = 0;
	float frame = 0; // todo Создать вектор для CurrentFrame и запихуть их туда все!
	float timerAppearenceEnemy = 0;
	bool explEnemyBulles = false;
	bool explPlayerBulles = false;
	bool explEnemy = false;
	bool explPlayer = false;
	bool appearanceEnemies = false;
	float curFrame = 0;
	bool isHit = false;
	bool isExplPlayer = false;
	int count = 0;
	int countBonus = 0;
	int numberBonus = 0;
	bool isBonus = false;
	bool playerLose = false;
	bool playerWin = false;
	bool playerWinTime = false;
	/*--------------------------------------------------*/
	while (window.isOpen()) {
		
		float time = float(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time = time / 800.0f;
		timer += time;
		timerAppearenceEnemy += time;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if ((Keyboard::isKeyPressed(Keyboard::RControl)) && bullesPlayer.size() == 0){
			bullesPlayer.push_back(new Enemy(Bulles, "Bulles", lvl, p.coordinatesGunTank.x, p.coordinatesGunTank.y, 8.0f, 8.0f, &p, nullptr));
		}
		if (!playerLose && !playerWin) {
			if (timerAppearenceEnemy > 4000 && p.numberEnemyTank <= 20) {
				appearanceEnemies = true;
			}
			if (p.numberEnemyTank > 20) {
				playerWinTime = true;
			}
			isBonus = false;
			if (p.numberEnemyTank % 9 == 0 && countBonus == 0) {
				isBonus = true;
			}
			if (p.numberEnemyTank % 9 != 0){
				countBonus = 0;
			}
			if (isBonus) {
				listPointsBonus.push_back(new Enemy(imagesBonus[numberBonus], "bonus", lvl, bonus[numberBonus].rect.left, bonus[numberBonus].rect.top, 45, 42, &p, nullptr));
				if (numberBonus == 0) {
					numberBonus++;
					p.individualNumberBonus++;
				}
				else {
					numberBonus = 0;
					p.individualNumberBonus = 0;
				}
				++countBonus;
			}
			if (appearanceEnemies) {
				bool flag = false;
				while (!flag) {
					int last = pointRessurection;
					if (p.sprite->getPosition().x + 22  > e[pointRessurection].rect.left && p.sprite->getPosition().x + 22 < e[pointRessurection].rect.left + 45 &&
						p.sprite->getPosition().y + 22  > e[pointRessurection].rect.top && p.sprite->getPosition().y + 22  < e[pointRessurection].rect.top + 45) {
						if (pointRessurection == 2) {
							pointRessurection = 0;
						}
						else {
							++pointRessurection;
						}
					}
					for (Entity* entity : entities) {
						if (entity->sprite->getPosition().x + 22 > e[pointRessurection].rect.left && entity->sprite->getPosition().x + 22 < e[pointRessurection].rect.left + 45 &&
							entity->sprite->getPosition().y + 22 > e[pointRessurection].rect.top && entity->sprite->getPosition().y + 22 < e[pointRessurection].rect.top + 45) {
							if (pointRessurection == 2) pointRessurection = 0;
							else ++pointRessurection;
						}
					}
					if (last == pointRessurection) {
						flag = true;
					}
				}
				if (curFrame < 1) {
					spriteAppEnemies.setTextureRect(IntRect(0, 0, 44, 45));
					spriteAppEnemies.setPosition(e[pointRessurection].rect.left, e[pointRessurection].rect.top);
				}
				else {
					spriteAppEnemies.setTextureRect(IntRect(44, 0, 46, 45));
					spriteAppEnemies.setPosition(e[pointRessurection].rect.left, e[pointRessurection].rect.top);
				}
				if (curFrame > 2) {
					count++;
					curFrame = 0;
				}
				curFrame += 0.005f * time;
				if (count == 2) {
					entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[pointRessurection].rect.left, e[pointRessurection].rect.top, 45, 45, &p, nullptr));
					timerAppearenceEnemy = 0;

					appearanceEnemies = false;
					count = 0;
					p.numberEnemyTank++;
					list<Entity*>::iterator iter;
					iter = numberEnemies.begin();
					delete (*iter);
					iter = numberEnemies.erase(iter);
					if (pointRessurection == 2) {
						pointRessurection = 0;
					}
					else {
						pointRessurection++;
					}
				}

			}
			if (isHit) {
				isExplPlayer = true;
				if (isExplPlayer) {
					explosionsPlayer(time, frame, isExplPlayer, bigExplosionSprite, p);
				}
				if (!isExplPlayer) {
					isHit = false;
					p.sprite->setPosition(player.rect.left, player.rect.top);
					p.coordinates.x = player.rect.left;
					p.coordinates.y = player.rect.top;
				}

				if (p.health == -1) {
					p.life = false;
				}
			}
			else {
				if (p.life) p.update(time);
				else playerLose = true;
			}
			for (iter = bullesPlayer.begin(); iter != bullesPlayer.end();) {
				if (!(*iter)->life) {
					explPlayerBulles = true;
					if (explPlayerBulles) {
						explosion(time, iter, currentFrame, explPlayerBulles, explTexture, 2);
						iter++;
					}
					if (!explPlayerBulles) {
						iter--;
						delete (*iter);
						iter = bullesPlayer.erase(iter);
					}
				}
				else {
					(*iter)->update(time);
					if (bullesPlayer.size() != 0) iter++;
				}
			}
			for (itBulles = bullesEnemy.begin(); itBulles != bullesEnemy.end();) {
				if (!(*itBulles)->life) {
					for (it = entities.begin(); it != entities.end(); it++) {
						if ((*it)->individualNumber == (*itBulles)->individualNumber) {
							(*it)->bulletReleased = (*itBulles)->bulletReleased;
						}
					}
					explEnemyBulles = true;
					if (explEnemyBulles) {
						explosion(time, itBulles, currentFrame, explEnemyBulles, explTexture, 2);
						itBulles++;
					}
					if (!explEnemyBulles) {
						itBulles--;
						delete *itBulles;
						itBulles = bullesEnemy.erase(itBulles);
					}
				}
				else {
					(*itBulles)->update(time);
					if (bullesEnemy.size() != 0) itBulles++;
				}
			}
			changeCourseTank(randomNumber, timer, time, entities);
			updateBrick(listBrick);
			updateEnemy(time, entities, explEnemy, currentFrame, bigExplosionTexture, playerWinTime, playerWin);
			for (Entity* entity : entities) {
				int random;
				rangeValuesRandomly(random, 200);
				if (entity->life) {
					if (random == 1 && !entity->bulletReleased) {
						bullesEnemy.push_back(new Enemy(Bulles, "bullesEnemy", lvl, entity->coordinatesGunTank.x, entity->coordinatesGunTank.y, 8.0f, 8.0f, nullptr, &entity));
						itBulles = bullesEnemy.end();
						--itBulles;
						(*itBulles)->individualNumber = entity->individualNumber;
						cout << (*itBulles)->individualNumber << endl;
						entity->bulletReleased = true;
					}
					enemiesClash(entities, entity);
					collisionWithBricks(entity, listBrick);
				}
			}
			checkForBonus(p, listPointsBonus, entities);
			collisionWithBricksForPlayer(p, listBrick);
			collisionWithEnemyForPlayer(p, entities);
			for (Entity* bulles : bullesPlayer) {
				if (bulles->life) checkHitTheEnemy(bulles, entities);
				if (bulles->life) checkHitTheBrick(bulles, listBrick);
				if (bulles->life) checkHitTheBaseForPlayer(window, targetProtecting, bulles, playerLose);
				if (bulles->life) checkHitTheBulletEnemy(bulles, bullesEnemy);
			}
			for (Entity* bulles : bullesEnemy) {
				if (bulles->life) checkHitByPlayer(p, bulles, entities, isHit);
				if (bulles->life) checkHitTheBaseForEnemy(window, targetProtecting, bulles, playerLose);
				if (bulles->life) checkHitTheBrickPlayer(listBrick, entities, bulles);
			}
		}
		drawing(window, p, bullesPlayer, entities, bullesEnemy, listBrick, lvl, targetProtecting, numberEnemies, appearanceEnemies, spriteAppEnemies, text, isExplPlayer, bigExplosionSprite, playerLose, youLose, playerWin, youWin, listPointsBonus);
	}

	return 0;
}