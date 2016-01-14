#include "mainStruct.h"


engine::engine() {
	im = new images;
	im->create();
	id = new globalBool;

	map = new objectLevel(im);
	player = map->lvl.GetObject("player");
	Vector2f coordPlayer{ float(player.rect.left), float(player.rect.top) };

	p = new Player(im->hero, "Player", map->lvl, coordPlayer, SIZE_PLAYER, player);

	/*--------------------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------------------*/

	Vector2f coordEagle = { float(map->targetDeagle.rect.left), float(map->targetDeagle.rect.top) };

	targetProtecting = new Enemy(im->eagle, "Deagle", map->lvl, coordEagle, SIZE_EAGLE, nullptr, nullptr);
	
	window = new RenderWindow(sf::VideoMode(SIZE_WINDOW.x, SIZE_WINDOW.y), WINDOW_NAME);
}