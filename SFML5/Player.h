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
		if (name == PLAYER_NAME) {
			frame = 0.0f;
			numberEnemyTank = 1;
			sprite->setTextureRect(IntRect(0, 0, w, h));
			coordinatesGunTank.x = int(player.rect.left) + w;
			coordinatesGunTank.y = int(player.rect.top) + h / 2;
		}
	};

	void update(float time, vector<Object> &obj);

private:

	void checkCollisionWithMap(float Dx, float Dy, vector<Object> &obj);
	
	void control(float time);
};