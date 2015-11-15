#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "level.h"
#include <vector>
#include <list>

using namespace sf;
using namespace std;

class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}

	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}

	virtual void update(float time) = 0;

};