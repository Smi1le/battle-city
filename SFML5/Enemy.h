#pragma once
#include "Player.h"
#include "levelObject.h"



class Enemy :public Entity {
public:
	Enemy(Image &image, String Name, Level &lvl, Vector2f position, Vector2i size, Player *p, Entity* *entity) :Entity(image, Name, position, size, coordinatesGunTank, lvl) {
		if (name == EASY_ENEMY_NAME) {
			sprite->setTextureRect(IntRect(51, 0, w, h));
			state = DOWN; speed = 0.1f;
			delay = 2;
			individualNumber = p->numberEnemyTank;
		}
		if (name == DISPLAYS_ENEMY_NAME) {
			sprite->setPosition(coordinates.x, coordinates.y);
		}
		checkNameForBullesPlayer(p);
		if (name == EAGLE_NAME) {
			sprite->setPosition(position.x, position.y);
		}
		if (name == BRICK_NAME) {
			sprite->setPosition(position.x, position.y);
		}
		if (name == BONUS_NAME) {
			sprite->setPosition(coordinates.x, coordinates.y);
			individualNumberBonus = p->individualNumberBonus;
		}
		checkNameForBullesEnemy(entity);
	}

	void update(float time, vector<Object> &obj);

private:
	void changeDirection(float Dx, float Dy);
	void checkCollisionWithMap(float time, vector<Object> &obj);
	void animation(int X, int Y, float time, int w, int h, string dir);
	void checkNameForBullesPlayer(Player *p);
	void checkNameForBullesEnemy(Entity* *entity);
};