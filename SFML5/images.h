#pragma once
#include "Entity.h"


struct images {
	Image easyEnemyImage;
	Image speedEnemyImage;
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
	Image fontImage;
	Image cursorImage;

	Texture explTexture;
	Texture enemiesTexture;
	Texture appTexture;
	Texture bigExplosionTexture;
	Texture fontTexture;
	Texture cursorTexture;

	Sprite spriteAppEnemies;
	Sprite bigExplosionSprite;
	Sprite fontSprite;
	Sprite cursorSprite;

	void create();
};