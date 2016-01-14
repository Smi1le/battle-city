#pragma once


struct engine {
	images *im = nullptr;
	globalBool *id = nullptr;
	objectLevel *map = nullptr;
	text *txt = nullptr;
	Player *p = nullptr;
	Enemy *targetProtecting = nullptr;
	globalVariable *var = nullptr;
	Object player;
	music *sound = nullptr;
	engine() {
		im = new images;
		im->create();
		id = new globalBool;
		map = new objectLevel(im);
		player = map->lvl.GetObject("player");
		txt = new text(map);
		p = new Player(im->hero, "Player", map->lvl, { float(player.rect.left), float(player.rect.top) }, SIZE_PLAYER, player);
		targetProtecting = new Enemy(im->eagle, "Deagle", map->lvl, { float(map->targetDeagle.rect.left), float(map->targetDeagle.rect.top) }, SIZE_EAGLE, nullptr, nullptr);
		var = new globalVariable;
		sound = new music;
	}
};