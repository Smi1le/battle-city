#include "Player.h"



void Player::control(music *sound) {
	if (currentFrame > 2) {
		(currentFrame = 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::Left) && permittedMovementOptions[1] == 0) {
		state = LEFT; speed = 0.1f;
		sprite->setTextureRect(IntRect(145, 52 * int(currentFrame), 40, 40));
		coordinatesGunTank = { int(coordinates.x), int(coordinates.y) + h / 2 };
		speedBulles = { -SPEED_BULLES, 0.0f };
	}
	else if (Keyboard::isKeyPressed(Keyboard::Right) && permittedMovementOptions[0] == 0) {
		state = RIGHT; speed = 0.1f;
		sprite->setTextureRect(IntRect(0, 52 * int(currentFrame), 40, 40));
		coordinatesGunTank = { int(coordinates.x) + w, int(coordinates.y) + h / 2 };
		speedBulles = { SPEED_BULLES, 0.0f };
	}
	else if (Keyboard::isKeyPressed(Keyboard::Up) && permittedMovementOptions[2] == 0) {
		state = UP; speed = 0.1f;
		sprite->setTextureRect(IntRect(100, 52 * int(currentFrame), 40, 40));
		coordinatesGunTank = { int(coordinates.x) + w / 2, int(coordinates.y) };
		speedBulles = { 0.0f, -SPEED_BULLES };
	}
	else if (Keyboard::isKeyPressed(Keyboard::Down) && permittedMovementOptions[3] == 0) {
		state = DOWN; speed = 0.1f;
		sprite->setTextureRect(IntRect(50, 52 * int(currentFrame), 40, 40));
		coordinatesGunTank = { int(coordinates.x) + w / 2, int(coordinates.y) + h };
		speedBulles = { 0.0f, SPEED_BULLES };
	}	
	else {
		state = STAY;
	}
}

void Player::checkCollisionWithMap(float Dx, float Dy, vector<Object> &obj) {
	if (!changeCourse) {
		for (size_t i = 0; i < obj.size(); ++i) {
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
}

void Player::update(float time, vector<Object> &obj, music *sound) {
	currentFrame += REDUCTION * time;
	control(sound);
	checkCollisionWithMap(diraction.x, diraction.y, obj);
	switch (state) {
	case RIGHT:
		if (sound->isMove.getStatus() != sound->isMove.Playing) {
			sound->isMove.play();
		}
		diraction.x = speed;
		diraction.y = 0.0f;
		break;
	case LEFT:
		if (sound->isMove.getStatus() != sound->isMove.Playing) {
			sound->isMove.play();
		}
		diraction.x = -speed;
		diraction.y = 0.0f;
		break;
	case UP:
		if (sound->isMove.getStatus() != sound->isMove.Playing) {
			sound->isMove.play();
		}
		diraction.x = 0.0f;
		diraction.y = -speed;
		break;
	case DOWN:
		if (sound->isMove.getStatus() != sound->isMove.Playing) {
			sound->isMove.play();
		}
		diraction.x = 0.0f;
		diraction.y = speed;
		break;
	case STAY: 
		diraction.x = 0.0f;
		diraction.y = 0.0f;
		sound->isMove.stop();
		break;
	}
	if (!isBlock) {
		coordinates.x += diraction.x * time;
		coordinates.y += diraction.y * time;
	}
	speed = 0;
	sprite->setPosition(coordinates.x, coordinates.y);
}