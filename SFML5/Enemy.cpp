#include "Enemy.h"



void Enemy::changeDirection(float Dx, float Dy) {
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

void Enemy::checkCollisionWithMap(float time, vector<Object> &obj) {
	for (Object element : obj) {
		if (getRect().intersects(element.rect)) {
			if (element.name == SOLID_NAME) {
				if (name == EASY_ENEMY_NAME) {
					if (diraction.y > 0) {
						coordinates.y = element.rect.top - h;
					}
					else if (diraction.y < 0) {
						coordinates.y = element.rect.top + element.rect.height;
					}
					else if (diraction.x > 0) {
						coordinates.x = element.rect.left - w;
					}
					else if (diraction.x < 0) {
						coordinates.x = element.rect.left + element.rect.width;
					}
					changeDirection(diraction.x, diraction.y);
					delay = 0;
				}
				if (name == PLAYER_BULLES_NAME || name == ENEMY_BULLES_NAME) {
					alive = false;
					bulletReleased = false;
				}

			}
		}
	}
}

void Enemy::animation(int X, int Y, float time, int w, int h, string dir) {
	if (currentFrame > 2) (currentFrame = 0);
	sprite->setTextureRect(IntRect(X, Y * int(currentFrame), w, h));
	if (dir == RIGHT_DIR_NAME) {
		coordinatesGunTank.x = int(coordinates.x) + w;
		coordinatesGunTank.y = int(coordinates.y) + h / 2;
	}
	else if (dir == LEFT_DIR_NAME) {
		coordinatesGunTank.x = int(coordinates.x);
		coordinatesGunTank.y = int(coordinates.y) + h / 2;
	}
	else if (dir == UP_DIR_NAME) {
		coordinatesGunTank.x = int(coordinates.x) + w / 2;
		coordinatesGunTank.y = int(coordinates.y);
	}
	else if (dir == DOWN_DIR_NAME) {
		coordinatesGunTank.x = int(coordinates.x) + w / 2;
		coordinatesGunTank.y = int(coordinates.y) + h;
	}
}

void Enemy::update(float time, vector<Object> &obj) {
	if (alive) {
		if (name == EASY_ENEMY_NAME) {
			delay += 0.005f* time;
			if (delay > 1) {
				currentFrame += 0.005f * time;
				switch (state) {
				case RIGHT:
					diraction.x = speed;
					diraction.y = 0;
					animation(0, 51, time, 44, 44, "RIGHT");
					break;
				case LEFT:
					diraction.x = -speed;
					diraction.y = 0;
					animation(141, 51, time, 44, 44, "LEFT");
					break;
				case UP:
					diraction.x = 0;
					diraction.y = -speed;
					animation(92, 51, time, 44, 44, "UP");
					break;
				case DOWN:
					diraction.x = 0;
					diraction.y = speed;
					animation(48, 51, time, 44, 44, "DOWN");
					break;
				case STAY: break;
				}
				coordinates.x += diraction.x * time;
				coordinates.y += diraction.y * time;
				sprite->setPosition(coordinates.x, coordinates.y);
				checkCollisionWithMap(time, obj);
			}
			if (health <= 0) { alive = false; }
		}
		
		if (name == PLAYER_BULLES_NAME) {
			checkCollisionWithMap(time, obj);
			coordinates.x += diraction.x * time;
			coordinates.y += diraction.y * time;
			cout << "diraction.x = " << diraction.x << endl;
			cout << "diraction.x = " << diraction.x << endl;
			sprite->setPosition(coordinates.x, coordinates.y);
		}
		if (name == "bullesEnemy") {
			checkCollisionWithMap(time, obj);
			coordinates.x += diraction.x * time;
			coordinates.y += diraction.y * time;
			cout << "sdsdsd = " << diraction.x << endl;
			cout << "sdsdsd = " << diraction.x << endl;
			sprite->setPosition(coordinates.x, coordinates.y);
		}
	}
}

void Enemy::checkNameForBullesPlayer(Player *p) {
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
}

void Enemy::checkNameForBullesEnemy(Entity* *entity) {
	if (name == ENEMY_BULLES_NAME) {
		bulletReleased = true;
		alive = true;
		cout << "coordinates.x = " << coordinates.x << endl;
		cout << "coordinates.y = " << coordinates.y << endl;
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