#include "levelObject.h"


objectLevel::objectLevel(images *im){

	lvl.LoadFromFile("recources/levels/map.tmx");

	health = lvl.GetObject("health");

	obj = lvl.GetObjects(SOLID_NAME);

	targetDeagle = lvl.GetObject("Deagle");

	bonus = lvl.GetObjects("bonus");

	e = lvl.GetObjects("EasyEnemy");
	cout << "e.size() = " << e.size() << endl;
	brick = lvl.GetObjects("destroyedSolid");
	enemies = lvl.GetObjects("enemy");

	
	for (int i = 0; i < int(enemies.size()); i++) {
		Vector2f coordIconEnemies = { float(enemies[i].rect.left), float(enemies[i].rect.top) };
		numberEnemies.push_back(new Enemy(im->displaysEnemies, "enemy", lvl, coordIconEnemies, SIZE_ICON_ENEMIES, nullptr, nullptr));
	}

	
	for (int i = 0; i < int(brick.size()); i++) {
		Vector2f coordBrick = { float(brick[i].rect.left), float(brick[i].rect.top) };
		listBrick.push_back(new Enemy(im->brickImage, "brick", lvl, coordBrick, SIZE_BRICK, nullptr, nullptr));
	}
	window = new RenderWindow(sf::VideoMode(SIZE_WINDOW.x, SIZE_WINDOW.y), WINDOW_NAME);
}

void objectLevel::createEnemy(Player *p) {
	for (it = entities.begin(); it != entities.end(); it++) {
		(*it)->individualNumber = p->numberEnemyTank;
		p->numberEnemyTank++;
	}
}