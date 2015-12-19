#include "Entity.h"

Entity::Entity(Image &image, String Name, Vector2f position, Vector2i size, Vector2i coordinatesGunTank, Level &lvl) {
	coordinates.x = position.x; coordinates.y = position.y; w = size.x; h = size.y; name = Name;
	speed = 0; health = 100; diraction.x = 0; diraction.y = 0;
	alive = true; onGround = false; isMove = false;
	texture = new Texture;
	sprite = new Sprite;
	texture->loadFromImage(image);
	sprite->setTexture(*texture);
	obj = lvl.GetObjects(SOLID_NAME);
}

FloatRect Entity::getRect() {
	return FloatRect(coordinates.x, coordinates.y, float(w), float(h));
}