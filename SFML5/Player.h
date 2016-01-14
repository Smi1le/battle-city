#pragma once
#include "Entity.h"
#include "images.h"



class Player : public Entity{
public:
	float frame;
	bool changeCourse;
	Player(Image &image, String name, Level &lvl, Vector2f position, Vector2i size, Object player) : Entity(image, name, position, size, coordinatesGunTank, lvl){
		state = STAY;
		health = 2;
		isBlock = false;
		if (name == PLAYER_NAME) {
			permittedMovementOptions = vector<int>(4, 0);
			frame = 0.0f;
			numberEnemyTank = 1;
			sprite->setTextureRect(IntRect(0, 0, w, h));
			coordinatesGunTank = { int(player.rect.left) + w, int(player.rect.top) + h / 2 };
			speedBulles = { 0.3f, 0.0f };
		}
	}


	void update(float time, vector<Object> &obj, music *sound);

private:

	void checkCollisionWithMap(float Dx, float Dy, vector<Object> &obj); 
	
	void control(music *sound);
};