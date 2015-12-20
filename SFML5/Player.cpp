#include "Player.h"



void Player::control(float time) {
	if (currentFrame > 2) {
		(currentFrame = 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		state = LEFT; speed = 0.1f;
		sprite->setTextureRect(IntRect(145, 52 * int(currentFrame), 40, 40));
		coordinatesGunTank.x = int(coordinates.x);
		coordinatesGunTank.y = int(coordinates.y) + h / 2;
		speedBulles.x = -0.15f;
		speedBulles.y = 0.0f;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right)) {
		state = RIGHT; speed = 0.1f;
		sprite->setTextureRect(IntRect(0, 52 * int(currentFrame), 40, 40));
		coordinatesGunTank.x = int(coordinates.x) + w;
		coordinatesGunTank.y = int(coordinates.y) + h / 2;
		speedBulles.x = 0.15f;
		speedBulles.y = 0.0f;
	}
	else if ((Keyboard::isKeyPressed(Keyboard::Up))) {
		state = UP; speed = 0.1f;
		sprite->setTextureRect(IntRect(100, 52 * int(currentFrame), 40, 40));
		coordinatesGunTank.x = int(coordinates.x) + w / 2;
		coordinatesGunTank.y = int(coordinates.y);
		speedBulles.x = 0.0f;
		speedBulles.y = -0.15f;
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down)) {
		state = DOWN; speed = 0.1f;
		sprite->setTextureRect(IntRect(50, 52 * int(currentFrame), 40, 40));
		coordinatesGunTank.x = int(coordinates.x) + w / 2;
		coordinatesGunTank.y = int(coordinates.y) + h;
		speedBulles.x = 0.0f;
		speedBulles.y = 0.15f;

	}	
	
}

void Player::checkCollisionWithMap(float Dx, float Dy, vector<Object> &obj) {
	for (size_t i = 0; i < obj.size(); ++i){
		if (getRect().intersects(obj[i].rect)) {
			if (obj[i].name == SOLID_NAME) {
				if (Dy > 0) {
					coordinates.y = obj[i].rect.top - h;
						diraction.y = 0;
				}
				if (Dy < 0) {
					coordinates.y = obj[i].rect.top + obj[i].rect.height;
						diraction.y = 0;
				}
				if (Dx > 0) {
					coordinates.x = obj[i].rect.left - w;
				}
				if (Dx < 0) {
					coordinates.x = obj[i].rect.left + obj[i].rect.width;
				}
			}
		}
	}
}

void Player::update(float time, vector<Object> &obj) {
	currentFrame += 0.005f * time;
	control(time);
	switch (state) {
	case RIGHT:
		diraction.x = speed;
		diraction.y = 0;
		break;
	case LEFT:
		diraction.x = -speed;
		diraction.y = 0;
		break;
	case UP:
		diraction.x = 0;
		diraction.y = -speed;
		break;
	case DOWN:
		diraction.x = 0;
		diraction.y = speed;
		break;
	case STAY: break;
	}
	coordinates.x += diraction.x * time;
	coordinates.y += diraction.y * time;
	checkCollisionWithMap(diraction.x, diraction.y, obj);
	speed = 0;
	sprite->setPosition(coordinates.x, coordinates.y);
}