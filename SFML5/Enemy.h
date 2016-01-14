#pragma once
#include "Player.h"
#include "levelObject.h"



class Enemy :public Entity {
public:
	Enemy(Image &image, String Name, Level &lvl, Vector2f position, Vector2i size, Player *p, Entity* *entity) :Entity(image, Name, position, size, coordinatesGunTank, lvl) {
		if (name == EASY_ENEMY_NAME) {
			sprite->setTextureRect(IntRect(51, 0, w, h));
			state = DOWN; speed = 0.1f;
			speedBulles = { 0.0f, 0.3f};
			delay = 2;
			individualNumber = p->numberEnemyTank;
			sprite->setPosition(position.x, position.y);
		}
		if (name == "speedEnemy") {
			sprite->setTextureRect(IntRect(0, 0, w, h));
			state = DOWN; speed = SPEED_SPEED_ENEMY;
			speedBulles = { 0.0f, 0.3f };
			delay = 2;
			individualNumber = p->numberEnemyTank;
			sprite->setPosition(position.x, position.y);
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

	void update(float time, vector<Object> &obj, music *sound);

private:
	void changeDirection(float Dx, float Dy);
	void checkCollisionWithMap(vector<Object> &obj, music *sound);
	void animation(int X, int Y, int w, int h, string dir);
	void checkNameForBullesPlayer(Player *p);
	void checkNameForBullesEnemy(Entity* *entity);

	void changeDirection_Right_Easy(int randomNumber);
	void changeDirection_Left_Easy(int randomNumber);
	void changeDirection_UP_Easy(int randomNumber);
	void changeDirection_DOWN_Easy(int randomNumber);

	void changeDirection_Right_Speed(int randomNumber);
	void changeDirection_Left_Speed(int randomNumber);
	void changeDirection_UP_Speed(int randomNumber);
	void changeDirection_DOWN_Speed(int randomNumber);
};