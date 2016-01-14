#pragma once

void transformationVariables(objectLevel *map, globalBool *id, globalVariable *var, Player *p) {
	Object player = map->lvl.GetObject("player");
	Vector2f coordPlayer{ float(player.rect.left), float(player.rect.top) };
	p->coordinates = { float(player.rect.left), float(player.rect.top) };
	id->g_playerLose = false;
	id->g_playerWin = false;
	id->g_playerWinTime = false;
	p->numberEnemyTank = 1;
	p->isBlock = false;
	p->health = 2;
	var->g_pointRessurection = 0;
}

void cleaningSets(objectLevel *map) {
	list<Entity*>::iterator it;
	if (map->entities.size() != 0) {
		it = map->entities.begin();
		while (map->entities.size() != 0) {
			delete *it;
			it = map->entities.erase(it);
		}
	}
	it = map->listPointsBonus.begin();
	if (map->listPointsBonus.size() != 0) {
		it = map->listPointsBonus.begin();
		while (map->listPointsBonus.size() != 0) {
			delete *it;
			it = map->listPointsBonus.erase(it);
		}
	}
	it = map->listBrick.begin();
	while (map->listBrick.size() != 0) {
		delete *it;
		it = map->listBrick.erase(it);
	}
	if (map->bullesEnemy.size() != 0) {
		it = map->bullesEnemy.begin();
		while (map->bullesEnemy.size() != 0) {
			delete *it;
			it = map->bullesEnemy.erase(it);
		}
	}
	if (map->bullesPlayer.size() != 0) {
		it = map->bullesPlayer.begin();
		while (map->bullesPlayer.size() != 0) {
			delete *it;
			it = map->bullesPlayer.erase(it);
		}
	}
}

void recreation(objectLevel *map, images *im) {
	for (int i = 0; i < int(map->enemies.size()); i++) {
		Vector2f coordIconEnemies = { float(map->enemies[i].rect.left), float(map->enemies[i].rect.top) };
		map->numberEnemies.push_back(new Enemy(im->displaysEnemies, "enemy", map->lvl, coordIconEnemies, SIZE_ICON_ENEMIES, nullptr, nullptr));
	}
	list<Entity*>::iterator it;
	it = map->listBrick.begin();
	while (map->listBrick.size() != 0) {
		delete *it;
		it = map->listBrick.erase(it);
	}
	for (int i = 0; i < int(map->brick.size()); i++) {
		map->listBrick.push_back(new Enemy(im->brickImage, "brick", map->lvl, { float(map->brick[i].rect.left), float(map->brick[i].rect.top) }, SIZE_BRICK, nullptr, nullptr));
	}
	if (map->bullesEnemy.size() != 0) {
		it = map->bullesEnemy.begin();
		while (map->bullesEnemy.size() != 0) {
			delete *it;
			it = map->bullesEnemy.erase(it);
		}
	}
}


void restart(Player *p, images *im, objectLevel *map, globalBool *id, globalVariable *var) {
	transformationVariables(map, id, var, p);
	cleaningSets(map);
	recreation(map, im);
}