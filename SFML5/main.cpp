//#include <SFML/Graphics.hpp>
#include "Enemy.h"

using namespace sf;
using namespace std;

void rangeValuesRandomly(int &number, int upperLimit) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, upperLimit);
	number = dist(gen);
}

void randomDirect(Entity* &entity, float Dx, float Dy) {
	int randomNumber;
	rangeValuesRandomly(randomNumber, 4);
	if (randomNumber == 1) {
		entity->state = entity->RIGHT; entity->speed = 0.1f;
	}
	else if (randomNumber == 2) {
		entity->state = entity->LEFT; entity->speed = 0.1f;
	}
	else if (randomNumber == 3) {
		entity->state = entity->UP; entity->speed = 0.1f;
	}
	else if (randomNumber == 4) {
		entity->state = entity->DOWN; entity->speed = 0.1f;
	}
}

void changeCourseTank(int &randomNumber, int &timer, const float time, list<Entity*> &entities) {
	if (randomNumber * 100 < timer) {
		for (Entity* entity : entities) {
			if (entity->alive) randomDirect(entity, entity->diraction.x, entity->diraction.y);
		}
		timer = 0;
		rangeValuesRandomly(randomNumber, 50);
	}
}


void updateBrick(list<Entity*> &listBrick) {
	list<Entity*>::iterator itBrick;
	for (itBrick = listBrick.begin(); itBrick != listBrick.end();) {
		if ((*itBrick)->alive == false) { delete (*itBrick); itBrick = listBrick.erase(itBrick); }
		else itBrick++;
	}
}

void explosionsPlayer(float time, float &frame, bool &explosion, Sprite &sprite, Player p) {
	float x = p.sprite->getPosition().x;
	float y = p.sprite->getPosition().y;
	frame += 0.005f * time;
	sprite.setScale(3.0f, 3.0f);
	if (frame < 2) {
		sprite.setOrigin(13.0f / 2.0f, 12.0f / 2.0f);
		sprite.setTextureRect(IntRect(14 * int(frame), 0, 13, 12));
		sprite.setPosition(x, y);
	}
	else if (frame < 3) {
		sprite.setOrigin(31.0f / 2.0f, 28.0f / 2.0f);
		sprite.setTextureRect(IntRect(29, 0, 31, 28));
		sprite.setPosition(x, y);
	}
	else {
		frame = 0;
		explosion = false;
	}
}

void  explosion(float time, list<Entity*>::iterator it, float &currentFrame, bool &explosion, Texture &texture, int number) {
	float x = (*it)->sprite->getPosition().x;
	float y = (*it)->sprite->getPosition().y;
	delete (*it)->sprite;
	(*it)->sprite = new Sprite;
	(*it)->sprite->setTexture(texture);
	(*it)->sprite->setScale(3.0f, 3.0f);
	if (number == 2){
		if (currentFrame < number) {
			(*it)->sprite->setOrigin(13.0f / 2.0f, 12.0f / 2.0f);
			(*it)->sprite->setTextureRect(IntRect(14 * int(currentFrame), 0, 13, 12));
			(*it)->sprite->setPosition(x, y);
		}
		else {
			currentFrame = 0;
			explosion = false;
		}
	}
	else {
		if (currentFrame < 2) {
			(*it)->sprite->setOrigin(13.0f / 2.0f, 12.0f / 2.0f);
			(*it)->sprite->setTextureRect(IntRect(14 * int(currentFrame), 0, 13, 12));
			(*it)->sprite->setPosition(x, y);
		}
		else if (currentFrame < 3) {
			(*it)->sprite->setOrigin(31.0f / 2.0f, 28.0f / 2.0f);
			(*it)->sprite->setTextureRect(IntRect(29, 0, 31, 28));
			(*it)->sprite->setPosition(x, y);
		}
		else {
			currentFrame = 0;
			explosion = false;
		}
	}
	currentFrame += 0.005f * time;;
}

void updateEnemy(float time, list<Entity*> &entities, bool &explEnemy, float &currentFrame, Texture &explTexture, bool &playerWinTime, bool &playerWin) {
	list<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end();) {
		if (!(*it)->alive) {
			explEnemy = true;
			if (explEnemy) {
				explosion(time, it, currentFrame, explEnemy, explTexture, 3);
				it++;
			}
			if (!explEnemy) {
				it--;
				delete (*it);
				it = entities.erase(it);
			}
			if (!explEnemy && playerWinTime && entities.size() == 0) {
				playerWin = true;
			}
		}
		else {
				(*it)->update(time);
				it++;
		}
	}
}

void changeDirection(float Dx, float Dy, Entity* &it) {
	int randomNumber;
	rangeValuesRandomly(randomNumber, 4);
	if (Dx > 0) { //Right
		if (randomNumber == 2) {
			it->state = it->LEFT; it->speed = 0.1f;
			it->coordinates.x -= 5;

		}
		if (randomNumber == 3) {
			it->state = it->DOWN; it->speed = 0.1f;
			it->coordinates.x -= 5;
		}
		else if (randomNumber == 4) {
			it->state = it->UP; it->speed = 0.1f;
			it->coordinates.x -= 5;
		}
	}
	if (Dx < 0) { //Left
		if (randomNumber == 1) {
			it->state = it->RIGHT; it->speed = 0.1f;
			it->coordinates.x += 5;
		}
		if (randomNumber == 3) {
			it->state = it->DOWN; it->speed = 0.1f;
			it->coordinates.x += 5;
		}
		else if (randomNumber == 4) {
			it->state = it->UP; it->speed = 0.1f;
			it->coordinates.x += 5;
		}
	}
	if (Dy < 0) { //Up
		if (randomNumber == 1) {
			it->state = it->RIGHT; it->speed = 0.1f;
			it->coordinates.y += 5;
		}
		else if (randomNumber == 2) {
			it->state = it->LEFT; it->speed = 0.1f;
			it->coordinates.y += 5;
		}
		else if (randomNumber == 3) {
			it->state = it->DOWN; it->speed = 0.1f;
			it->coordinates.y += 5;

		}
	}
	if (Dy > 0) { //Down
		if (randomNumber == 1) {
			it->state = it->RIGHT; it->speed = 0.1f;
			it->coordinates.y -= 5;
		}
		else if (randomNumber == 2) {
			it->state = it->LEFT; it->speed = 0.1f;
			it->coordinates.y -= 5;
		}
		else if (randomNumber == 4) {
			it->state = it->UP; it->speed = 0.1f;
			it->coordinates.y -= 5;
		}
	}
}

void stoping(Entity* &it,Entity* &entity) {
	if (it->diraction.y > 0) { it->coordinates.y = entity->sprite->getPosition().y - it->h; }
	if (it->diraction.y < 0) { it->coordinates.y = entity->sprite->getPosition().y + entity->h; }
	if (it->diraction.x > 0) { it->coordinates.x = entity->sprite->getPosition().x - it->w; }
	if (it->diraction.x < 0) { it->coordinates.x = entity->sprite->getPosition().x + entity->w; }
}

void collisionWithBricks(Entity* &it, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (it->getRect().intersects(brick->getRect()) && it->alive) {
			stoping(it, brick);
			changeDirection(it->diraction.x, it->diraction.y, it);
			it->delay = 0;
		}
	}
}

void enemiesClash(list<Entity*> &entities, Entity* &it) {
	for (Entity* entity : entities) {
		if (entity->getRect().intersects(it->getRect())) {
			if (it->getRect() != entity->getRect()) {
				if ((it->getRect().intersects(entity->getRect())) && (it->name == EASY_ENEMY_NAME) && (entity->name == EASY_ENEMY_NAME)){
					stoping(it, entity);
					if (it->state == it->LEFT) { it->state = it->RIGHT; }
					else if (it->state == it->RIGHT) { it->state = it->LEFT; }
					else if (it->state == it->DOWN) { it->state = it->UP; }
					else if (it->state == it->UP) { it->state = it->DOWN; }
					if (entity->state == entity->LEFT) { entity->state = entity->RIGHT; }
					else if (entity->state == entity->RIGHT) { entity->state = entity->LEFT; }
					else if (entity->state == entity->DOWN) { entity->state = entity->UP; }
					else if (entity->state == entity->UP) { entity->state = entity->DOWN; }
				}
			}
		}
	}
}



void checkCollisionWithMap(Player &p, Entity* brick) {
	if (p.diraction.y > 0) {
		p.coordinates.y = brick->sprite->getPosition().y - p.h;
		p.diraction.y = 0;
	}
	else if (p.diraction.y < 0) {
		p.coordinates.y = brick->sprite->getPosition().y + brick->h;
		p.diraction.y = 0;
	}
	if (p.diraction.x > 0) {
		p.coordinates.x = brick->sprite->getPosition().x - p.w;
	}
	if (p.diraction.x < 0) {
		p.coordinates.x = brick->sprite->getPosition().x + brick->w;
	}
}

void collisionWithBricksForPlayer(Player &p, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (p.getRect().intersects(brick->getRect())) {
			checkCollisionWithMap(p, brick);
		}
	}
}

void collisionWithEnemyForPlayer(Player &p, list<Entity*> &entities) {
	for (Entity* entity : entities) {
		if (entity->getRect().intersects(p.getRect()) && entity->alive) {
			checkCollisionWithMap(p, entity);
			entity->speed = 0;
		}
		else {
			entity->speed = 0.1f;
		}
	}
}

void checkHitTheEnemy(Entity* &iter, list<Entity*>  &entities) {
	for (Entity* entity : entities) {
		if (entity->alive && iter->alive) {
			if (entity->getRect().intersects(iter->getRect())) {
				entity->alive = false;
				iter->alive = false;
			}
		}
	}
}

void checkHitTheBrick(Entity* &iter, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (iter->getRect().intersects(brick->getRect())) {
			brick->alive = false;
			iter->alive = false;
		}
	}
}

void checkHitTheBulletEnemy(Entity* &iter, list<Entity*> &listBrick) {
	for (Entity* brick : listBrick) {
		if (iter->getRect().intersects(brick->getRect())) {
			brick->alive = false;
			iter->alive = false;
		}
	}
}

void checkHitTheBaseForPlayer(RenderWindow &window, Enemy targetProtecting, Entity* &iter, bool &lose) {
	if (iter->getRect().intersects(targetProtecting.getRect())) {
		lose = true;
		iter->alive = false;
	}
}

void checkHitByPlayer(Player &p, Entity* &bulles, list<Entity*> &entities, bool &isHit) {
	if (bulles->getRect().intersects(p.getRect())) {
		for (Entity* entity : entities) {
			if (entity->individualNumber == bulles->individualNumber) {
				bulles->alive = false;
				bulles->bulletReleased = false;
				p.health--;
				isHit = true;
			}
		}
	}
}

void checkHitTheBaseForEnemy(RenderWindow &window, Enemy targetProtecting, Entity* &bulles, bool &lose) {
	if (bulles->getRect().intersects(targetProtecting.getRect())) {
		lose = true;
		bulles->alive = false;
		bulles->bulletReleased = false;
	}
}

void checkHitTheBrickPlayer(list<Entity*> &listBrick, list<Entity*> &entities, Entity* &bulles) {
	for (Entity* brick : listBrick) {
		if (bulles->getRect().intersects(brick->getRect())) {
			for (Entity* entity : entities) {
				if (entity->individualNumber == bulles->individualNumber) {
					bulles->bulletReleased = false;
					bulles->alive = false;
					brick->alive = false;
				}
			}
		}
	}
}

void checkForBonus(Player &p, list<Entity*> &listPointsBonus, list<Entity*> &entities) {
	for (list<Entity*>::iterator it = listPointsBonus.begin(); it != listPointsBonus.end(); it++) {
		if (p.getRect().intersects((*it)->getRect())) {
			if ((*it)->individualNumberBonus == 0) {
				for (list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++) {
					(*it2)->alive = false;
				}
			}
			else {
				++p.health;
			}
			delete *it;
			it = listPointsBonus.erase(it);
			if (it == listPointsBonus.end()) {
				break;
			}
		}
	}
}

void drawing(RenderWindow &window, Player p,  Level lvl,  bool app, Sprite sprite, Text text, bool isHit, Sprite bigExplosion, bool lose, Text youLose, bool win, Text youWin, objectLevel map, images *im, Enemy &targetProtecting) {
	window.clear();
	ostringstream playerHealthString;
	playerHealthString << p.health;
	text.setString("" + playerHealthString.str());
	lvl.Draw(window);
	window.draw(text);
	for (Entity* brick : map.listBrick) {
		window.draw(*brick->sprite);
	}
	for (Entity* bonus : map.listPointsBonus) {
		window.draw(*bonus->sprite);
	}
	if (!lose) {
		if (isHit) {
			window.draw(bigExplosion);
		}
		else {
			window.draw(*p.sprite);
		}
	}
	else {
		window.draw(youLose);
	}
	if (win) {
		window.draw(youWin);
	}
	window.draw(*targetProtecting.sprite);
	for (Entity* entity : map.bullesPlayer) {
		window.draw(*entity->sprite);
	}
	for (Entity* entity : map.entities) {
		window.draw(*entity->sprite);
	}
	for (Entity* bulles : map.bullesEnemy) {
		window.draw(*bulles->sprite);
	}
	for (Entity* enemy : map.numberEnemies) {
		window.draw(*enemy->sprite);
	}
	if (app) window.draw(sprite);
	window.display();
}

void cleaning() {

}

int main(){
	Clock clock;
	

	images * im = new images;
	im->create();


	Level lvl;
	
	lvl.LoadFromFile("recources/levels/map.tmx");


	
	Object health = lvl.GetObject("health");

	/*--------------------------------------------------------------------------------------------------------*/
	Font font;//����� 
	font.loadFromFile("recources/Font/Monoid.ttf");//�������� ������ ������ ���� ������

	Text text("", font, 26);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	text.setColor(Color::Black);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	text.setStyle(sf::Text::Bold);
	text.setPosition(health.rect.left, health.rect.top);

	Text youLose(" YOU LOSE ", font, 50);
	youLose.setColor(Color::Red);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	youLose.setStyle(sf::Text::Bold);
	youLose.setPosition(200, SIZE_WINDOW.y / 2);

	Text youWin(" YOU WIN ", font, 50);
	youWin.setColor(Color::Blue);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	youWin.setStyle(sf::Text::Bold);
	youWin.setPosition(200, SIZE_WINDOW.y / 2);
	/*--------------------------------------------------------------------------------------------------------*/
	Object player = lvl.GetObject("player");
	
	Vector2f coordPlayer{ float(player.rect.left), float(player.rect.top) };
	Vector2i sizePlayer = { 37, 39 };
	Player p(im->hero, "Player", lvl, coordPlayer, sizePlayer, player);

	objectLevel map(lvl, p, im);
	cout << "map->e.size() = " << map.e.size() << endl;
	cout << "map.entities.size() = " << map.entities.size() << endl;
	cout << "map.brick.size() = " << map.brick.size() << endl;
	cout << "map.listBrick.size() = " << map.listBrick.size() << endl;
	
	Vector2f coordEagle = { float(map.targetDeagle.rect.left), float(map.targetDeagle.rect.top) };
	Vector2i sizeEagle = { 50, 50 };
	Enemy targetProtecting(im->eagle, "Deagle", lvl, coordEagle, sizeEagle, nullptr, nullptr);


	sf::RenderWindow window(sf::VideoMode(SIZE_WINDOW.x, SIZE_WINDOW.y), WINDOW_NAME);
	clock.restart();
	
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, 50);
	int randomNumber;
	rangeValuesRandomly(randomNumber, 50);
	/*--------------------------------------------------*/
	int pointRessurection = 0;
	int timer = 0;
	float currentFrame = 0;
	float frame = 0; // todo ������� ������ ��� CurrentFrame � �������� �� ���� ���!
	float timerAppearenceEnemy = 0;
	bool explEnemyBulles = false;
	bool explPlayerBulles = false;
	bool explEnemy = false;
	bool explPlayer = false;
	bool appearanceEnemies = false;
	float curFrame = 0;
	bool isHit = false;
	bool isExplPlayer = false;
	int count = 0;
	int countBonus = 0;
	int numberBonus = 0;
	bool isBonus = false;
	bool playerLose = false;
	bool playerWin = false;
	bool playerWinTime = false;
	/*--------------------------------------------------*/
	while (window.isOpen()) {
		
		float time = float(clock.getElapsedTime().asMicroseconds());
		clock.restart();
		time = time / 800.0f;
		timer += time;
		timerAppearenceEnemy += time;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if ((Keyboard::isKeyPressed(Keyboard::RControl)) && map.bullesPlayer.size() == 0){
			Vector2f coordBullesPlayer = { float(p.coordinatesGunTank.x), float(p.coordinatesGunTank.y) };
			Vector2i sizeBullesPlayer = { 8, 8 };
			map.bullesPlayer.push_back(new Enemy(im->Bulles, "Bulles", lvl, coordBullesPlayer, sizeBullesPlayer, &p, nullptr));
		}
		if (!playerLose && !playerWin) {
			if (timerAppearenceEnemy > 4000 && p.numberEnemyTank <= 20) {
				appearanceEnemies = true;
			}
			if (p.numberEnemyTank > 20) {
				playerWinTime = true;
			}
			isBonus = false;
			if (p.numberEnemyTank % 9 == 0 && countBonus == 0) {
				isBonus = true;
			}
			if (p.numberEnemyTank % 9 != 0){
				countBonus = 0;
			}
			if (isBonus) {
				Vector2f coordIconBonus = { float(map.bonus[numberBonus].rect.left), float(map.bonus[numberBonus].rect.top) };
				Vector2i sizeIconBonus = { 45, 42 };
				map.listPointsBonus.push_back(new Enemy(im->imagesBonus[numberBonus], "bonus", lvl, coordIconBonus, sizeIconBonus, &p, nullptr));
				if (numberBonus == 0) {
					numberBonus++;
					p.individualNumberBonus++;
				}
				else {
					numberBonus = 0;
					p.individualNumberBonus = 0;
				}
				++countBonus;
			}
			if (appearanceEnemies) {
				bool flag = false;
				while (!flag) {
					int last = pointRessurection;
					
					if (p.sprite->getPosition().x + 22  > map.e[pointRessurection].rect.left && p.sprite->getPosition().x + 22 < map.e[pointRessurection].rect.left + 45 &&
						p.sprite->getPosition().y + 22  > map.e[pointRessurection].rect.top && p.sprite->getPosition().y + 22  < map.e[pointRessurection].rect.top + 45) {
						if (pointRessurection == 2) {
							pointRessurection = 0;
						}
						else {
							++pointRessurection;
						}
					}
					for (Entity* entity : map.entities) {
						if (entity->sprite->getPosition().x + 22 > map.e[pointRessurection].rect.left && entity->sprite->getPosition().x + 22 < map.e[pointRessurection].rect.left + 45 &&
							entity->sprite->getPosition().y + 22 > map.e[pointRessurection].rect.top && entity->sprite->getPosition().y + 22 < map.e[pointRessurection].rect.top + 45) {
							if (pointRessurection == 2) {
								pointRessurection = 0;
							}
							else {
								++pointRessurection;
							}
						}
					}
					if (last == pointRessurection) {
						flag = true;
					}
				}
				if (curFrame < 1) {
					im->spriteAppEnemies.setTextureRect(IntRect(0, 0, 44, 45));
					im->spriteAppEnemies.setPosition(map.e[pointRessurection].rect.left, map.e[pointRessurection].rect.top);
				}
				else {
					im->spriteAppEnemies.setTextureRect(IntRect(44, 0, 46, 45));
					im->spriteAppEnemies.setPosition(map.e[pointRessurection].rect.left, map.e[pointRessurection].rect.top);
				}
				if (curFrame > 2) {
					count++;
					curFrame = 0;
				}
				curFrame += 0.005f * time;
				if (count == 2) {
					Vector2f coordEnemy = { float(map.e[pointRessurection].rect.left), float(map.e[pointRessurection].rect.top) };
					Vector2i sizeEnemy = { 45, 45 };
					map.entities.push_back(new Enemy(im->easyEnemyImage, "EasyEnemy", lvl, coordEnemy, sizeEnemy,  &p, nullptr));
					timerAppearenceEnemy = 0;

					appearanceEnemies = false;
					count = 0;
					p.numberEnemyTank++;
					list<Entity*>::iterator iter;
					iter = map.numberEnemies.begin();
					delete (*iter);
					iter = map.numberEnemies.erase(iter);
					if (pointRessurection == 2) {
						pointRessurection = 0;
					}
					else {
						pointRessurection++;
					}
				}

			}
			if (isHit) {
				isExplPlayer = true;
				if (isExplPlayer) {
					explosionsPlayer(time, frame, isExplPlayer, im->bigExplosionSprite, p);
				}
				if (!isExplPlayer) {
					isHit = false;
					p.sprite->setPosition(map.player.rect.left, map.player.rect.top);
					p.coordinates.x = map.player.rect.left;
					p.coordinates.y = map.player.rect.top;
				}

				if (p.health == -1) {
					p.alive = false;
				}
			}
			else {
				if (p.alive) {
					p.update(time);
				}
				else {
					playerLose = true;
				}
			}
			for (list<Entity*>::iterator iter = map.bullesPlayer.begin(); iter != map.bullesPlayer.end();) {
				if (!(*iter)->alive) {
					explPlayerBulles = true;
					if (explPlayerBulles) {
						explosion(time, iter, currentFrame, explPlayerBulles, im->explTexture, 2);
						iter++;
					}
					if (!explPlayerBulles) {
						iter--;
						delete (*iter);
						iter = map.bullesPlayer.erase(iter);
					}
				}
				else {
					(*iter)->update(time);
					if (map.bullesPlayer.size() != 0) iter++;
				}
			}
			for (list<Entity*>::iterator itBulles = map.bullesEnemy.begin(); itBulles != map.bullesEnemy.end();) {
				if (!(*itBulles)->alive) {
					for (list<Entity*>::iterator it = map.entities.begin(); it != map.entities.end(); it++) {
						if ((*it)->individualNumber == (*itBulles)->individualNumber) {
							(*it)->bulletReleased = (*itBulles)->bulletReleased;
						}
					}
					explEnemyBulles = true;
					if (explEnemyBulles) {
						explosion(time, itBulles, currentFrame, explEnemyBulles, im->explTexture, 2);
						itBulles++;
					}
					if (!explEnemyBulles) {
						itBulles--;
						delete *itBulles;
						itBulles = map.bullesEnemy.erase(itBulles);
					}
				}
				else {
					(*itBulles)->update(time);
					if (map.bullesEnemy.size() != 0) {
						itBulles++;
					}
				}
			}
			changeCourseTank(randomNumber, timer, time, map.entities);
			updateBrick(map.listBrick);
			updateEnemy(time, map.entities, explEnemy, currentFrame, im->bigExplosionTexture, playerWinTime, playerWin);
			for (Entity* entity : map.entities) {
				int random;
				rangeValuesRandomly(random, 200);
				if (entity->alive) {
					if (random == 1 && !entity->bulletReleased) {
						Vector2f coordBullesEnemy = { float(entity->coordinatesGunTank.x), float(entity->coordinatesGunTank.y) };
						Vector2i sizeBullesEnemy = { 8, 8 };
						map.bullesEnemy.push_back(new Enemy(im->Bulles, "bullesEnemy", lvl, coordBullesEnemy, sizeBullesEnemy, nullptr, &entity));
						list<Entity*>::iterator itBulles = map.bullesEnemy.end();
						--itBulles;
						(*itBulles)->individualNumber = entity->individualNumber;
						entity->bulletReleased = true;
					}
					enemiesClash(map.entities, entity);
					collisionWithBricks(entity, map.listBrick);
				}
			}
			checkForBonus(p, map.listPointsBonus, map.entities);
			collisionWithBricksForPlayer(p, map.listBrick);
			collisionWithEnemyForPlayer(p, map.entities);
			for (Entity* bulles : map.bullesPlayer) {
				if (bulles->alive) {
					checkHitTheEnemy(bulles, map.entities);
				}
				if (bulles->alive) {
					checkHitTheBrick(bulles, map.listBrick);
				}
				if (bulles->alive) {
					checkHitTheBaseForPlayer(window, targetProtecting, bulles, playerLose);
				}
				if (bulles->alive) {
					checkHitTheBulletEnemy(bulles, map.bullesEnemy);
				}
			}
			for (Entity* bulles : map.bullesEnemy) {
				if (bulles->alive) {
					checkHitByPlayer(p, bulles, map.entities, isHit);
				}
				if (bulles->alive) {
					checkHitTheBaseForEnemy(window, targetProtecting, bulles, playerLose);
				}
				if (bulles->alive) {
					checkHitTheBrickPlayer(map.listBrick, map.entities, bulles);
				}
			}
		}
		drawing(window, p, lvl, appearanceEnemies, im->spriteAppEnemies, text, isExplPlayer, im->bigExplosionSprite, playerLose, youLose, playerWin, youWin, map, im, targetProtecting);
	}

	return 0;
}