#include "images.h"

void images::create() {
	easyEnemyImage.loadFromFile("recources/images/easyEnemy.png");
	easyEnemyImage.createMaskFromColor(Color(0, 128, 128));

	speedEnemyImage.loadFromFile("recources/images/speed enemy.png");
	speedEnemyImage.createMaskFromColor(Color(0, 128, 128));

	brickImage.loadFromFile("recources/images/brick.png");

	Bulles.loadFromFile("recources/images/Bulles.png");
	Bulles.createMaskFromColor(Color(0, 128, 128));

	displaysEnemies.loadFromFile("recources/images/enemies.png");
	displaysEnemies.createMaskFromColor(Color(109, 109, 109));
	
	enemiesTexture.loadFromImage(displaysEnemies);

	eagle.loadFromFile("recources/images/eagle.png");
	eagle.createMaskFromColor(Color::Black);

	hero.loadFromFile("recources/images/hero.png");
	hero.createMaskFromColor(Color::Black);


	expl.loadFromFile("recources/images/explosionBulles.png");
	expl.createMaskFromColor(Color(0, 128, 128));
	explTexture.loadFromImage(expl);

	appearance.loadFromFile("recources/images/appearance.png");
	appearance.createMaskFromColor(Color(0, 128, 128));
	appTexture.loadFromImage(appearance);
	spriteAppEnemies.setTexture(appTexture);


	bigExplosion.loadFromFile("recources/images/BigExplosion.png");
	bigExplosion.createMaskFromColor(Color(0, 128, 128));
	
	bigExplosionTexture.loadFromImage(bigExplosion);
	bigExplosionSprite.setTexture(bigExplosionTexture);

	imageBonusBomb.loadFromFile("recources/images/bonus-bomb.png");
	imageBonusLife.loadFromFile("recources/images/bonus-life.png");
	imageBonusBomb.createMaskFromColor(Color(34, 177, 76));
	imageBonusLife.createMaskFromColor(Color(34, 177, 76));

	imagesBonus = { imageBonusBomb, imageBonusLife };

	fontImage.loadFromFile("recources/images/fontGameMenu.png");
	cursorImage.loadFromFile("recources/images/hero.png");
	cursorImage.createMaskFromColor(Color::Black);

	fontTexture.loadFromImage(fontImage);
	cursorTexture.loadFromImage(cursorImage);

	fontSprite.setTexture(fontTexture);
	cursorSprite.setTexture(cursorTexture);
	cursorSprite.setTextureRect(IntRect(0, 0, SIZE_PLAYER.x, SIZE_PLAYER.y));

	fontSprite.setOrigin(float(fontImage.getSize().x) / 2.0f, float(fontImage.getSize().y) / 2.0f);
	fontSprite.setScale(3.0f, 3.0f);
}