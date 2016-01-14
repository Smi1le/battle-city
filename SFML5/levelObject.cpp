#include "levelObject.h"


objectLevel::objectLevel(images *im){
	lvl.LoadFromFile("recources/levels/map.tmx");
	health = lvl.GetObject("health");
	obj = lvl.GetObjects(SOLID_NAME);
	targetDeagle = lvl.GetObject("Deagle");
	bonus = lvl.GetObjects("bonus");
	e = lvl.GetObjects("EasyEnemy");
	brick = lvl.GetObjects("destroyedSolid");
	enemies = lvl.GetObjects("enemy");
	int number;
	if (enemies.size() >= NUMBER_ENEMYES) {
		number = NUMBER_ENEMYES;
	}
	else {
		number = enemies.size();
	}
	for (int i = 0; i < int(enemies.size()); i++) {
		Vector2f coordIconEnemies = { float(enemies[i].rect.left), float(enemies[i].rect.top) };
		numberEnemies.push_back(new Enemy(im->displaysEnemies, "enemy", lvl, coordIconEnemies, SIZE_ICON_ENEMIES, nullptr, nullptr));
	}
	for (int i = 0; i < int(brick.size()); i++) {
		Vector2f coordBrick = { float(brick[i].rect.left), float(brick[i].rect.top) };
		listBrick.push_back(new Enemy(im->brickImage, "brick", lvl, coordBrick, SIZE_BRICK, nullptr, nullptr));
	}
	window = make_unique<RenderWindow>(sf::VideoMode(SIZE_WINDOW.x, SIZE_WINDOW.y), WINDOW_NAME);
	window->setFramerateLimit(LIMIT_FRAME);
}

void objectLevel::createEnemy(Player *p) {
	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); it++) {
		(*it)->individualNumber = p->numberEnemyTank;
		p->numberEnemyTank++;
	}
}