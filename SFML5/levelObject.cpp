#include "levelObject.h"


objectLevel::objectLevel(images *im){

	lvl.LoadFromFile("recources/levels/map.tmx");

	obj = lvl.GetObjects(SOLID_NAME);

	targetDeagle = lvl.GetObject("Deagle");

	bonus = lvl.GetObjects("bonus");

	e = lvl.GetObjects("EasyEnemy");
	cout << "e.size() = " << e.size() << endl;
	brick = lvl.GetObjects("destroyedSolid");
	enemies = lvl.GetObjects("enemy");

	Vector2i sizeIconEnemies = { 26, 26 };
	for (int i = 0; i < int(enemies.size()); i++) {
		Vector2f coordIconEnemies = { float(enemies[i].rect.left), float(enemies[i].rect.top) };
		numberEnemies.push_back(new Enemy(im->displaysEnemies, "enemy", lvl, coordIconEnemies, sizeIconEnemies, nullptr, nullptr));
	}

	Vector2i sizeBrick = { 26, 25 };
	for (int i = 0; i < int(brick.size()); i++) {
		Vector2f coordBrick = { float(brick[i].rect.left), float(brick[i].rect.top) };
		listBrick.push_back(new Enemy(im->brickImage, "brick", lvl, coordBrick, sizeBrick, nullptr, nullptr));
		brick[i].rect.left;//����� �
		brick[i].rect.top;//����� Y

	}
}

void objectLevel::createEnemy(Player *p) {
	for (it = entities.begin(); it != entities.end(); it++) {
		(*it)->individualNumber = p->numberEnemyTank;
		p->numberEnemyTank++;
	}
}