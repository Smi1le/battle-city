#pragma once
#include "Entity.h"


struct images {
	Image easyEnemyImage;
	Image brickImage;
	Image Bulles;
	Image displaysEnemies;
	Image eagle;
	Image hero;
	Image expl;
	Image appearance;
	Image bigExplosion;
	Image imageBonusBomb;
	Image imageBonusLife;
	vector<Image> imagesBonus;

	Texture explTexture;
	Texture enemiesTexture;
	Texture appTexture;
	Texture bigExplosionTexture;

	Sprite spriteAppEnemies;
	Sprite bigExplosionSprite;

	void create();
};