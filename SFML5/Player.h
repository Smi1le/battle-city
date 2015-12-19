#pragma once
#include "Entity.h"
#include "images.h"



class Player : public Entity{
public:
	Player(Image &image, String name, Level &lev, Vector2f position, Vector2i size, Object player) : Entity(image, name, position, size, coordinatesGunTank, lev){
		state = STAY;
		health = 2;
		if (name == PLAYER_NAME) {
			numberEnemyTank = 1;
			sprite->setTextureRect(IntRect(0, 0, w, h));
			coordinatesGunTank.x = int(player.rect.left) + w;
			coordinatesGunTank.y = int(player.rect.top) + h / 2;
		}
	};

	void update(float time);

private:

	void checkCollisionWithMap(float Dx, float Dy);
	
	void control(float time);
};