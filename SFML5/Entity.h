#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <random>
#include <vector>
#include <list>
#include <ctime>
#include <crtdbg.h>
#include "level.h"
#include "config.h"
#include "vld.h"
#include <memory>
#include "music.h"


//#include "levelObject.h"

class Entity {
public:
	//std::vector<Object> obj;
	float speed;
	float currentFrame;
	float delay;
	float frame;
	int w;
	int h;
	int health;
	int individualNumber;
	int individualNumberBonus;
	int numberEnemyTank;
	enum { LEFT, RIGHT, UP, DOWN, STAY } state;
	Vector2i coordinatesGunTank;
	bool alive;
	bool isMove;
	bool onGround;
	bool bulletReleased = false;
	bool isBlock;
	Vector2f coordinates;
	Vector2f diraction;
	Vector2f speedBulles;
	Vector2f lastCoordinates;
	Texture *texture;
	Sprite *sprite;
	String name;
	vector<int> permittedMovementOptions;

	Entity(Image &image, String Name, Vector2f position, Vector2i size, Vector2i coordinatesGunTank, Level &lvl);

	FloatRect getRect();
	
	virtual void update(float time, vector<Object> &obj, music *sound) = 0;
};