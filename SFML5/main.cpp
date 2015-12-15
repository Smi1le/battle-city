#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <random>
#include "coordinate.h"
#include "level.h"
#include <vector>
#include <list>
#include <ctime>
#include <crtdbg.h>

const Vector2i SIZE_WINDOW = { 832, 700 };

; using namespace sf;
using namespace std;

class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed, moveTimer, CurrentFrame, speedX, lastDirX = 0, lastDirY = 0, speedY, timer = 5000, delay;
	int w, h, health, individualNumber, numberEnemyTank = 1;
	enum { LEFT, RIGHT, UP, DOWN, STAY } state;
	Vector2i coordinatesGunTank;
	bool life, isMove, onGround, bulletReleased = false;
	Texture *texture;
	Sprite *sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H, Vector2i coordinatesGunTank) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture = new Texture;
		sprite = new Sprite;
		texture->loadFromImage(image);
		sprite->setTexture(*texture);
	}

	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, float(w), float(h));//эта ф-ция нужна для проверки столкновений 
	}

		// , Object persona) = 0;
	virtual void update(float time) = 0;

};

class Player : public Entity {
public:
	
	int playerScore;
	bool cheats = false;
	Player(Image &image, String name, Level &lev, float X, float Y, int W, int H, Object player) :Entity(image, name, X, Y, W, H, coordinatesGunTank) {
		obj = lev.GetAllObjects();
		playerScore = 0; state = STAY;
		health = 2;
		if (name == "Player") {
			sprite->setTextureRect(IntRect(0, 0, w, h));
			coordinatesGunTank.x = int(player.rect.left) + w;
			coordinatesGunTank.y = int(player.rect.top) + h / 2;
		}
	}

	void control(float time) {//, float &CurrentFrame) {
							  //cur cu;// todo Создать структуру для анимации, общую для всех объектов
							  //cout << "CurrentFrame = " << cu.CurrentFrame << endl;
		//CurrentFrame += 0.005f * time;
		if (CurrentFrame > 2) (CurrentFrame = 0);
		if (Keyboard::isKeyPressed) {//если нажата клавиша
			if (Keyboard::isKeyPressed(Keyboard::Left)) {//а именно левая
				state = LEFT; speed = 0.1f;
				sprite->setTextureRect(IntRect(145, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = x;
				coordinatesGunTank.y = y + h / 2;
				speedX = -0.15f;
				speedY = 0.0f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = RIGHT; speed = 0.1f;
				sprite->setTextureRect(IntRect(0, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = x + w;
				coordinatesGunTank.y = y + h / 2;
				speedX = 0.15f;
				speedY = 0.0f;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {//если нажата клавиша вверх и мы на земле, то можем прыгать
				state = UP; speed = 0.1f;// onGround = false;//увеличил высоту прыжка
				sprite->setTextureRect(IntRect(100, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = x + w / 2;
				coordinatesGunTank.y = y;
				speedX = 0.0f;
				speedY = -0.15f;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = DOWN; speed = 0.1f;
				sprite->setTextureRect(IntRect(50, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = x + w / 2;
				coordinatesGunTank.y = y + h;
				speedX = 0.0f;
				speedY = 0.15f;

			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = 0; i < int(obj.size()); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")// || (obj[i].name == "EasyEnemy"))//если встретили препятствие
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; }//  onGround = true;
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time) {//, float &CurrentFrame) {
		CurrentFrame += 0.005f * time;					 //cur cu;
		control(time);// , cu.CurrentFrame);
		switch (state) {
		case RIGHT:dx = speed; dy = 0; break;
		case LEFT:dx = -speed; dy = 0; break;
		case UP:dx = 0; dy = -speed; break;
		case DOWN:dx = 0; dy = speed; break;
		case STAY: break;
		}
		x += dx*time;
		checkCollisionWithMap(dx, 0);
		y += dy*time;
		checkCollisionWithMap(0, dy);
		speed = 0;
		sprite->setPosition(x, y);
	}
};

class Enemy :public Entity {
public:
	//enum { LEFT, RIGHT, UP, DOWN, JUMP, STAY } state;
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, Player p, list<Entity*>::iterator it) :Entity(image, Name, X, Y, W, H, coordinatesGunTank) {
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "EasyEnemy") {
			sprite->setTextureRect(IntRect(51, 0, w, h));
			state = DOWN; speed = 0.1f;
			delay = 2;
			//dx = 0.1f;
		}
		if (name == "enemy") {
			sprite->setPosition(x, y);
		}
		if (name == "Bulles") {
			if ((p.speedX > 0) && (p.speedY == 0)) {
				dx = 0.30f;
				dy = 0;
				sprite->setTextureRect(IntRect(26, 2, w, h));
			}
			if ((p.speedX < 0) && (p.speedY == 0)) {
				dx = -0.30f;
				dy = 0;
				sprite->setTextureRect(IntRect(16, 2, w, h));
			}
			if ((p.speedX == 0) && (p.speedY < 0)) {
				dx = 0;
				dy = -0.30f;
				sprite->setTextureRect(IntRect(0, 0, w, h));
			}
			if ((p.speedX == 0) && (p.speedY > 0)) {
				dx = 0;
				dy = 0.30f;
				sprite->setTextureRect(IntRect(7, 2, w, h));
			}
		}
		if (name == "Deagle"){
			sprite->setPosition(X, Y);
		}
		if (name == "brick") {
			sprite->setPosition(X, Y);
		}
		if (name == "bonus"){
			sprite->setPosition(x, y);
		}
		if (name == "bullesEnemy") {
			bulletReleased = true;

			if (((*it)->dx > 0) && ((*it)->dy == 0)) {
				dx = 0.30f;
				dy = 0;
				sprite->setTextureRect(IntRect(26, 2, w, h));
			}
			if (((*it)->dx < 0) && ((*it)->dy == 0)) {
				dx = -0.30f;
				dy = 0;
				sprite->setTextureRect(IntRect(16, 2, w, h));
			}
			if (((*it)->dx == 0) && ((*it)->dy < 0)) {
				dx = 0;
				dy = -0.30f;
				sprite->setTextureRect(IntRect(0, 0, w, h));
			}
			if (((*it)->dx == 0) && ((*it)->dy > 0)) {
				dx = 0;
				dy = 0.30f;
				sprite->setTextureRect(IntRect(7, 2, w, h));
			}
		}
	}

	
	void changeDirection(float Dx, float Dy) {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(1, 3);
		int randomNumber = dist(gen);
		if (Dx > 0) { //Right
			if (randomNumber == 1) {
				state = LEFT; speed = 0.1f;
				x -= 2;
			}
			if (randomNumber == 2) {
				state = DOWN; speed = 0.1f;
				x -= 2;
			}
			else if (randomNumber == 3) {
				state = UP; speed = 0.1f;
				x -= 2;
			}
		}
		if (Dx < 0) { //Left
			if (randomNumber == 1) {
				state = RIGHT; speed = 0.1f;
				x += 2;
			}
			if (randomNumber == 2) {
				state = DOWN; speed = 0.1f;
				x += 2;
			}
			else if (randomNumber == 3) {
				state = UP; speed = 0.1f;
				x += 2;
			}
		}
		if (Dy < 0) { //Up
			if (randomNumber == 1) {
				state = RIGHT; speed = 0.1f;
				y += 2;
			}
			else if (randomNumber == 2) {
				state = LEFT; speed = 0.1f;
				y += 2;
			}
			else if (randomNumber == 3) {
				state = DOWN; speed = 0.1f;
				y += 2;
			}
		}
		if (Dy > 0) { //Down
			if (randomNumber == 1) {
				state = RIGHT; speed = 0.1f;
				y -= 2;
			}
			else if (randomNumber == 2) {
				state = LEFT; speed = 0.1f;
				y -= 2;
			}
			else if (randomNumber == 3) {
				state = UP; speed = 0.1f;
				y -= 2;
			}
		}
	}
	

	void checkCollisionWithMap(float time)
	{
		for (int i = 0; i < int(obj.size()); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid") {// || (obj[i-1].name == "solid")){
					if (name == "EasyEnemy") {
						if (dy > 0) { y = obj[i].rect.top - h; }//  onGround = true;
						if (dy < 0) { y = obj[i].rect.top + obj[i].rect.height; }
						if (dx > 0) { x = obj[i].rect.left - w; }
						if (dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
						changeDirection(dx, dy);
						delay = 0;
					}
					if (name == "Bulles" || name == "bullesEnemy") {
						life = false;
						bulletReleased = false;
					}
				}
			}
	}	

	void animation(int X, int Y, float time, int w, int h, string dir) {
		//cout << "CurrentFrame = " << CurrentFrame << endl;
		if (CurrentFrame > 2) (CurrentFrame = 0);
		sprite->setTextureRect(IntRect(X, Y * int(CurrentFrame), w, h));
		if (dir == "RIGHT") {
			coordinatesGunTank.x = x + float(w);
			coordinatesGunTank.y = y + float(h) / 2.0f;
		}
		else if (dir == "LEFT") {
			coordinatesGunTank.x = x;
			coordinatesGunTank.y = y + float(h) / 2.0f;
		}
		else if (dir == "UP") {
			coordinatesGunTank.x = x + float(w) / 2.0f;
			coordinatesGunTank.y = y;
		}
		else if (dir == "DOWN") {
			coordinatesGunTank.x = x + float(w) / 2.0f;
			coordinatesGunTank.y = y + float(h);
		}
		/*speedX = 0.0f;
		speedY = -0.15f;*/

	}

	void update(float time)
	{
		if (life) {
			if (name == "EasyEnemy") {
				delay += 0.005f* time;

				if (delay > 1) {
					CurrentFrame += 0.005f * time;
					switch (state) {
					case RIGHT:dx = speed; dy = 0; animation(0, 51, time, 44, 44, "RIGHT"); break;
					case LEFT:dx = -speed; dy = 0; animation(141, 51, time, 44, 44, "LEFT"); break;
					case UP:dx = 0; dy = -speed; animation(92, 51, time, 44, 44, "UP"); break;
					case DOWN:dx = 0; dy = speed; animation(48, 51, time, 44, 44, "DOWN"); break;
					case STAY: break;
					}
					x += dx * time;
					y += dy * time;
					sprite->setPosition(x, y);
					checkCollisionWithMap(time);
				}
				if (health <= 0) { life = false; }
			}
			if (name == "Bulles" || name == "bullesEnemy") {
				checkCollisionWithMap(time);
				x += dx * time;
				y += dy * time;
				sprite->setPosition(x, y);
			}
		}
	}

	void parallelPlayerAndEnemy(float time, int flag) {

	}
};

void rangeValuesRandomly(int &number, int upperLimit) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, upperLimit);
	number = dist(gen);
}

void randomDirect(std::list<Entity*>::iterator it, float Dx, float Dy) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, 4);
	int randomNumber = dist(gen);
	if (randomNumber == 1) {
		(*it)->state = (*it)->RIGHT; (*it)->speed = 0.1f;
	}
	else if (randomNumber == 2) {
		(*it)->state = (*it)->LEFT; (*it)->speed = 0.1f;
	}
	else if (randomNumber == 3) {
		(*it)->state = (*it)->UP; (*it)->speed = 0.1f;
	}
	else if (randomNumber == 4) {
		(*it)->state = (*it)->DOWN; (*it)->speed = 0.1f;
	}
}

void changeCourseTank(int &randomNumber, int &timer, const float time, list<Entity*> &entities) {
	list<Entity*>::iterator it;
	if (randomNumber * 100 < timer) {
		for (it = entities.begin(); it != entities.end();) {
			if ((*it)->life) {
				randomDirect(it, (*it)->dx, (*it)->dy);
			}
		}
		timer = 0;
		rangeValuesRandomly(randomNumber, 50);
	}
}


void updateBrick(list<Entity*> &listBrick) {
	list<Entity*>::iterator itBrick;
	for (itBrick = listBrick.begin(); itBrick != listBrick.end();) {
		if ((*itBrick)->life == false) { delete (*itBrick); itBrick = listBrick.erase(itBrick); }
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
		if (!(*it)->life) {
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

void changeDirection(float Dx, float Dy, std::list<Entity*>::iterator it) {
	int randomNumber;
	rangeValuesRandomly(randomNumber, 4);
	if (Dx > 0) { //Right
		if (randomNumber == 2) {
			(*it)->state = (*it)->LEFT; (*it)->speed = 0.1f;
			(*it)->x -= 5;

		}
		if (randomNumber == 3) {
			(*it)->state = (*it)->DOWN; (*it)->speed = 0.1f;
			(*it)->x -= 5;
		}
		else if (randomNumber == 4) {
			(*it)->state = (*it)->UP; (*it)->speed = 0.1f;
			(*it)->x -= 5;
		}
	}
	if (Dx < 0) { //Left
		if (randomNumber == 1) {
			(*it)->state = (*it)->RIGHT; (*it)->speed = 0.1f;
			(*it)->x += 5;
		}
		if (randomNumber == 3) {
			(*it)->state = (*it)->DOWN; (*it)->speed = 0.1f;
			(*it)->x += 5;
		}
		else if (randomNumber == 4) {
			(*it)->state = (*it)->UP; (*it)->speed = 0.1f;
			(*it)->x += 5;
		}
	}
	if (Dy < 0) { //Up
		if (randomNumber == 1) {
			(*it)->state = (*it)->RIGHT; (*it)->speed = 0.1f;
			(*it)->y += 5;
		}
		else if (randomNumber == 2) {
			(*it)->state = (*it)->LEFT; (*it)->speed = 0.1f;
			(*it)->y += 5;
		}
		else if (randomNumber == 3) {
			(*it)->state = (*it)->DOWN; (*it)->speed = 0.1f;
			(*it)->y += 5;

		}
	}
	if (Dy > 0) { //Down
		if (randomNumber == 1) {
			(*it)->state = (*it)->RIGHT; (*it)->speed = 0.1f;
			(*it)->y -= 5;
		}
		else if (randomNumber == 2) {
			(*it)->state = (*it)->LEFT; (*it)->speed = 0.1f;
			(*it)->y -= 5;
		}
		else if (randomNumber == 4) {
			(*it)->state = (*it)->UP; (*it)->speed = 0.1f;
			(*it)->y -= 5;
		}
	}
}

void stoping(list<Entity*>::iterator &it, list<Entity*>::iterator &it2) {
	if ((*it)->dy > 0) { (*it)->y = (*it2)->sprite->getPosition().y - (*it)->h; }
	if ((*it)->dy < 0) { (*it)->y = (*it2)->sprite->getPosition().y + (*it2)->h; }
	if ((*it)->dx > 0) { (*it)->x = (*it2)->sprite->getPosition().x - (*it)->w; }
	if ((*it)->dx < 0) { (*it)->x = (*it2)->sprite->getPosition().x + (*it2)->w; }
}

void collisionWithBricks(list<Entity*>::iterator &it, list<Entity*> &listBrick) {
	list<Entity*>::iterator itBrick;
	for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
		if ((*it)->getRect().intersects((*itBrick)->getRect()) && (*it)->life) {
			stoping(it, itBrick);
			changeDirection((*it)->dx, (*it)->dy, it);
			(*it)->delay = 0;
		}
	}
}

void enemiesClash(list<Entity*> &entities, list<Entity*>::iterator &it) {
	list<Entity*>::iterator it2;
	for (it2 = entities.begin(); it2 != entities.end(); it2++) {
		if ((*it)->getRect() != (*it2)->getRect())//при этом это должны быть разные прямоугольники
			if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EasyEnemy") && ((*it2)->name == "EasyEnemy"))//если столкнулись два объекта и они враги
			{
				stoping(it, it2);
				if ((*it)->state == (*it)->LEFT) { (*it)->state = (*it)->RIGHT; }
				else if ((*it)->state == (*it)->RIGHT) { (*it)->state = (*it)->LEFT; }
				else if ((*it)->state == (*it)->DOWN) { (*it)->state = (*it)->UP; }
				else if ((*it)->state == (*it)->UP) { (*it)->state = (*it)->DOWN; }
				if ((*it2)->state == (*it2)->LEFT) { (*it2)->state = (*it2)->RIGHT; }
				else if ((*it2)->state == (*it2)->RIGHT) { (*it2)->state = (*it2)->LEFT; }
				else if ((*it2)->state == (*it2)->DOWN) { (*it2)->state = (*it2)->UP; }
				else if ((*it2)->state == (*it2)->UP) { (*it2)->state = (*it2)->DOWN; }
			}
	}
}



void checkCollisionWithMap(Player &p, std::list<Entity*>::iterator &it) {
	if (p.dy > 0) { p.y = (*it)->sprite->getPosition().y - p.h;  p.dy = 0; }
	if (p.dy < 0) { p.y = (*it)->sprite->getPosition().y + (*it)->h;   p.dy = 0; }
	if (p.dx > 0) { p.x = (*it)->sprite->getPosition().x - p.w; }
	if (p.dx < 0) { p.x = (*it)->sprite->getPosition().x + (*it)->w; }
}

void collisionWithBricksForPlayer(Player &p, list<Entity*> &listBrick) {
	list<Entity*>::iterator itBrick;
	for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
		if ((p.getRect().intersects((*itBrick)->getRect()))) {
			checkCollisionWithMap(p, itBrick);
		}
	}
}

void collisionWithEnemyForPlayer(Player &p, list<Entity*> &entities) {
	list<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++) {
		if ((*it)->getRect().intersects(p.getRect()) && (*it)->life) {
			checkCollisionWithMap(p, it);
			(*it)->speed = 0;
		}
		else {
			(*it)->speed = 0.1f;
		}
	}
}

void checkHitTheEnemy(list<Entity*>::iterator &iter, list<Entity*>  &entities) {
	list<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++) {
		if ((*it)->life && (*iter)->life) {
			if ((*it)->getRect().intersects((*iter)->getRect())) {
				//if ((*it)->name == "EasyEnemy") {
				(*it)->life = false;
				(*iter)->life = false;
				//}
			}
		}
	}
}

void checkHitTheBrick(list<Entity*>::iterator &iter, list<Entity*> &listBrick) {
	list<Entity*>::iterator itBrick;
	for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
		if ((*iter)->getRect().intersects((*itBrick)->getRect())) {
			(*itBrick)->life = false;
			(*iter)->life = false;
		}
	}
}

void checkHitTheBulletEnemy(list<Entity*>::iterator &iter, list<Entity*> listB) {
	list<Entity*>::iterator itBrick;
	for (itBrick = listB.begin(); itBrick != listB.end(); itBrick++) {
		if ((*iter)->getRect().intersects((*itBrick)->getRect())) {
			(*itBrick)->life = false;
			(*iter)->life = false;
		}
	}
}

void checkHitTheBaseForPlayer(RenderWindow &window, Enemy targetProtecting, list<Entity*>::iterator &iter, bool &lose) {
	if ((*iter)->getRect().intersects(targetProtecting.getRect())) {
		lose = true;
		(*iter)->life = false;
	}
}

void checkHitByPlayer(Player &p, list<Entity*>::iterator &itBulles, list<Entity*> entities, bool &isHit) {
	if ((*itBulles)->getRect().intersects(p.getRect())) {
		list<Entity*>::iterator it;
		for (it = entities.begin(); it != entities.end();) {
			if ((*it)->individualNumber == (*itBulles)->individualNumber) {
				(*itBulles)->life = false;
				(*itBulles)->bulletReleased = false;
				p.health--;
				isHit = true;
				it = entities.end();
			}
			else {
				if (entities.size() != 0) it++;
			}
		}
	}
}

void checkHitTheBaseForEnemy(RenderWindow &window, Enemy targetProtecting, list<Entity*>::iterator &itBulles, bool &lose) {
	if ((*itBulles)->getRect().intersects(targetProtecting.getRect())) {
		lose = true;
		(*itBulles)->life = false;
		(*itBulles)->bulletReleased = false;
	}
}

void checkHitTheBrickPlayer(list<Entity*> &listBrick, list<Entity*> &entities, list<Entity*>::iterator &itBulles) {
	list<Entity*>::iterator itBrick;
	for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
		if ((*itBulles)->getRect().intersects((*itBrick)->getRect())) {
			list<Entity*>::iterator it;
			for (it = entities.begin(); it != entities.end();) {
				if ((*it)->individualNumber == (*itBulles)->individualNumber) {
					(*itBulles)->bulletReleased = false;
					(*itBulles)->life = false;
					(*itBrick)->life = false;
					it = entities.end();
				}
				else {
					if (entities.size() != 0) it++;
				}
			}
		}
	}
}

void checkForBonus(Player &p, list<Entity*> &listPointsBonus, int numberBonus, list<Entity*> &entities) {
	for (list<Entity*>::iterator it = listPointsBonus.begin(); it != listPointsBonus.end(); it++) {
		if (p.getRect().intersects((*it)->getRect())) {
			if (numberBonus == 1) {
				for (list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++) {
					(*it2)->life = false;
				}
			}
			if (numberBonus == 0) {
				++p.health;
			}
			delete *it;
			it = listPointsBonus.erase(it);
			if (it == listPointsBonus.end()) break;
		}
	}
}

void drawing(RenderWindow &window, Player p, list<Entity*> bullesPlayer, list<Entity*> entities, list<Entity*> bullesEnemy, list<Entity*> listBrick, Level lvl, Enemy targetProtecting, list<Entity*> enemies, bool app, Sprite sprite, Text text, bool isHit, Sprite bigExplosion, bool lose, Text youLose, bool win, Text youWin, list<Entity*> imagesBonus) {
	
	window.clear();
	ostringstream playerHealthString;
	
	playerHealthString << p.health;
	text.setString("" + playerHealthString.str());//задает строку тексту
	//text.setPosition(view.getCenter().x, view.getCenter().y);//задаем позицию текста, центр камеры
	
	lvl.Draw(window);
	window.draw(text);//рисую этот текст

	
	list<Entity*>::iterator itBrick;
	for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
		window.draw(*(*itBrick)->sprite);
	}
	for (itBrick = imagesBonus.begin(); itBrick != imagesBonus.end(); itBrick++) {
		window.draw(*(*itBrick)->sprite);
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
	list<Entity*>::iterator iter;
	for (iter = bullesPlayer.begin(); iter != bullesPlayer.end(); iter++) {
		window.draw(*(*iter)->sprite);
	}
	list<Entity*>::iterator it;
	for (it = entities.begin(); it != entities.end(); it++) {
		window.draw(*(*it)->sprite);
	}
	list<Entity*>::iterator itBulles;
	for (itBulles = bullesEnemy.begin(); itBulles != bullesEnemy.end(); itBulles++) {
		window.draw(*(*itBulles)->sprite);
	}
	list<Entity*>::iterator itEnemies;
	for (itEnemies = enemies.begin(); itEnemies != enemies.end(); itEnemies++) {
		window.draw(*(*itEnemies)->sprite);
	}
	if (app) {
		window.draw(sprite);
	}
	
	window.display();
}

int main(){
	
	

	Clock clock;
	

	std::list<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	std::list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка
	std::list<Entity*>::iterator it2;//второй итератор.для взаимодействия между объектами списка

	
	std::list<Entity*> bullesEnemy;//создаю список, сюда буду кидать объекты.например врагов.
	std::list<Entity*>::iterator itBulles;
	
	std::list<Entity*>  listBrick;//создаю список, сюда буду кидать объекты.например врагов.
	std::list<Entity*>::iterator itBrick;
	

	std::list<Entity*> bullesPlayer;
	std::list<Entity*>::iterator iter;
	
	list<Entity*> numberEnemies;
	list<Entity*>::iterator itEnemies;
	
	list<Entity*> listPointsBonus;
	list<Entity*>::iterator itBonus;



	Level lvl;//создали экземпляр класса уровень
	
	lvl.LoadFromFile("map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.


	
	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/easyEnemy.png");
	easyEnemyImage.createMaskFromColor(Color(0, 128, 128));

	Image brickImage;
	brickImage.loadFromFile("images/brick.png");
	//brickImage.createMaskFromColor(Color(0, 128, 128));

	Image blockImage;
	blockImage.loadFromFile("images/blocks.png");
	//easyEnemyImage.createMaskFromColor(Color(0, 128, 128));

	Image borderImage;
	borderImage.loadFromFile("images/border.png");
	
	Image Bulles;
	Bulles.loadFromFile("images/Bulles.png");
	Bulles.createMaskFromColor(Color(0, 128, 128));

	Image displaysEnemies;
	displaysEnemies.loadFromFile("images/enemies.png");
	displaysEnemies.createMaskFromColor(Color(109, 109, 109));
	Texture enemiesTexture;
	enemiesTexture.loadFromImage(displaysEnemies);
	
	Image eagle;
	eagle.loadFromFile("images/eagle.png");
	eagle.createMaskFromColor(Color::Black);

	Image hero;
	hero.loadFromFile("images/hero.png");
	hero.createMaskFromColor(Color::Black);

	Image expl;
	expl.loadFromFile("images/explosionBulles.png");
	expl.createMaskFromColor(Color(0, 128, 128));
	Texture explTexture;
	explTexture.loadFromImage(expl);

	Image appearance;
	appearance.loadFromFile("images/appearance.png");
	appearance.createMaskFromColor(Color(0, 128, 128));
	Texture appTexture;
	appTexture.loadFromImage(appearance);
	Sprite spriteAppEnemies;
	spriteAppEnemies.setTexture(appTexture);

	Image bigExplosion;
	bigExplosion.loadFromFile("images/BigExplosion.png");
	bigExplosion.createMaskFromColor(Color(0, 128, 128));
	Texture bigExplosionTexture;
	bigExplosionTexture.loadFromImage(bigExplosion);
	Sprite bigExplosionSprite;
	bigExplosionSprite.setTexture(bigExplosionTexture);

	Image imageBonusBomb;
	Image imageBonusLife;
	imageBonusBomb.loadFromFile("images/bonus-bomb.png");
	imageBonusLife.loadFromFile("images/bonus-life.png");
	imageBonusBomb.createMaskFromColor(Color(34, 177, 76));
	imageBonusLife.createMaskFromColor(Color(34, 177, 76));
	Texture textureBonusBomb;
	Texture textureBonusLife;
	textureBonusBomb.loadFromImage(imageBonusBomb);
	textureBonusLife.loadFromImage(imageBonusLife);
	Sprite spriteBonusBomb;
	Sprite spriteBonusLife;
	spriteBonusBomb.setTexture(textureBonusBomb);
	spriteBonusLife.setTexture(textureBonusLife);
		
	Object health = lvl.GetObject("health");

	/*--------------------------------------------------------------------------------------------------------*/
	Font font;//шрифт 
	font.loadFromFile("Monoid.ttf");//передаем нашему шрифту файл шрифта

	Text text("", font, 26);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
	text.setColor(Color::Black);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	text.setStyle(sf::Text::Bold);
	text.setPosition(health.rect.left, health.rect.top);

	Text youLose(" YOU LOSE ", font, 50);
	youLose.setColor(Color::Red);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	youLose.setStyle(sf::Text::Bold);
	youLose.setPosition(200, SIZE_WINDOW.y / 2);

	Text youWin(" YOU WIN ", font, 50);
	youWin.setColor(Color::Blue);//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
	youWin.setStyle(sf::Text::Bold);
	youWin.setPosition(200, SIZE_WINDOW.y / 2);
	/*--------------------------------------------------------------------------------------------------------*/
	Object player = lvl.GetObject("player");

	Object targetDeagle = lvl.GetObject("Deagle");

	vector<Object> bonus = lvl.GetObjects("bonus");

	Player p(hero, "Player", lvl, player.rect.left, player.rect.top, 37.0f, 39.0f, player);

	Enemy targetProtecting(eagle, "Deagle", lvl, targetDeagle.rect.left, targetDeagle.rect.top, 50.0f, 50.0f, p, it);

	std::vector<Object> e = lvl.GetObjects("EasyEnemy");
	std::vector<Object> brick = lvl.GetObjects("destroyedSolid");
	vector<Object> enemies = lvl.GetObjects("enemy");
	vector<Image> imagesBonus = {imageBonusBomb,imageBonusLife };
	

	for (int i = 0; i < int(enemies.size()); i++) {
		numberEnemies.push_back(new Enemy(displaysEnemies, "enemy", lvl, enemies[i].rect.left, enemies[i].rect.top, 26, 26, p, it));
	}

	int pointRessurection = 0;
	for (it = entities.begin(); it != entities.end(); it++) {
		(*it)->individualNumber = p.numberEnemyTank;
		p.numberEnemyTank++;
	}
	for (int i = 0; i < int(brick.size()); i++) {
		listBrick.push_back(new Enemy(brickImage, "brick", lvl, brick[i].rect.left, brick[i].rect.top, 26, 25, p, it));
		brick[i].rect.left;//коорд Х
		brick[i].rect.top;//коорд Y
	
	}
	sf::RenderWindow window(sf::VideoMode(SIZE_WINDOW.x, SIZE_WINDOW.y), "Battle City");
	clock.restart();
	//float respawnTime = 0;
	
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, 50);
	int randomNumber = dist(gen);
	/*--------------------------------------------------*/
	int timer = 0;
	float currentFrame = 0;
	float frame = 0; // todo Создать вектор для CurrentFrame и запихуть их туда все!
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
		if ((Keyboard::isKeyPressed(Keyboard::RControl)) && bullesPlayer.size() == 0){
			bullesPlayer.push_back(new Enemy(Bulles, "Bulles", lvl, p.coordinatesGunTank.x, p.coordinatesGunTank.y, 8.0f, 8.0f, p, it));
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
				listPointsBonus.push_back(new Enemy(imagesBonus[numberBonus], "bonus", lvl, bonus[numberBonus].rect.left, bonus[numberBonus].rect.top, 45, 42, p, it));
				if (numberBonus == 0) numberBonus++;
				else numberBonus = 0;
				++countBonus;
			}
			if (appearanceEnemies) {
				bool flag = false;
				while (!flag) {
					int last = pointRessurection;
					if (p.sprite->getPosition().x + 22  > e[pointRessurection].rect.left && p.sprite->getPosition().x + 22 < e[pointRessurection].rect.left + 45 &&
						p.sprite->getPosition().y + 22  > e[pointRessurection].rect.top && p.sprite->getPosition().y + 22  < e[pointRessurection].rect.top + 45) {
						if (pointRessurection == 2) {
							pointRessurection = 0;
						}
						else {
							++pointRessurection;
						}
					}
					for (it = entities.begin(); it != entities.end(); it++) {
						if ((*it)->sprite->getPosition().x + 22 > e[pointRessurection].rect.left && (*it)->sprite->getPosition().x + 22  < e[pointRessurection].rect.left + 45 &&
							(*it)->sprite->getPosition().y + 22  > e[pointRessurection].rect.top && (*it)->sprite->getPosition().y + 22 < e[pointRessurection].rect.top + 45) {
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
					spriteAppEnemies.setTextureRect(IntRect(0, 0, 44, 45));
					spriteAppEnemies.setPosition(e[pointRessurection].rect.left, e[pointRessurection].rect.top);
				}
				else {
					spriteAppEnemies.setTextureRect(IntRect(44, 0, 46, 45));
					spriteAppEnemies.setPosition(e[pointRessurection].rect.left, e[pointRessurection].rect.top);
				}
				if (curFrame > 2) {
					count++;
					curFrame = 0;
				}
				curFrame += 0.005f * time;
				if (count == 2) {
					entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[pointRessurection].rect.left, e[pointRessurection].rect.top, 45, 45, p, it));
					timerAppearenceEnemy = 0;
					appearanceEnemies = false;
					count = 0;
					p.numberEnemyTank++;
					list<Entity*>::iterator iter;
					iter = numberEnemies.begin();
					delete (*iter);
					iter = numberEnemies.erase(iter);
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
					explosionsPlayer(time, frame, isExplPlayer, bigExplosionSprite, p);
				}
				if (!isExplPlayer) {
					isHit = false;
					p.sprite->setPosition(player.rect.left, player.rect.top);
					p.x = player.rect.left;
					p.y = player.rect.top;
				}

				if (p.health == -1) {
					p.life = false;
				}
			}
			else {
				if (p.life) p.update(time);
				else playerLose = true;
			}
			for (iter = bullesPlayer.begin(); iter != bullesPlayer.end();) {
				if (!(*iter)->life) {
					explPlayerBulles = true;
					if (explPlayerBulles) {
						explosion(time, iter, currentFrame, explPlayerBulles, explTexture, 2);
						iter++;
					}
					if (!explPlayerBulles) {
						iter--;
						delete (*iter);
						iter = bullesPlayer.erase(iter);
					}
				}
				else {
					(*iter)->update(time);
					if (bullesPlayer.size() != 0) iter++;
				}
			}
			for (itBulles = bullesEnemy.begin(); itBulles != bullesEnemy.end();) {
				if (!(*itBulles)->life) {
					for (it = entities.begin(); it != entities.end(); it++) {
						if ((*it)->individualNumber == (*itBulles)->individualNumber) {
							(*it)->bulletReleased = (*itBulles)->bulletReleased;
						}
					}
					explEnemyBulles = true;
					if (explEnemyBulles) {
						explosion(time, itBulles, currentFrame, explEnemyBulles, explTexture, 2);
						itBulles++;
					}
					if (!explEnemyBulles) {
						itBulles--;
						delete *itBulles;
						itBulles = bullesEnemy.erase(itBulles);
					}
				}
				else {
					(*itBulles)->update(time);
					if (bullesEnemy.size() != 0) itBulles++;
				}
			}
			if (randomNumber * 100 < timer) {
				for (it = entities.begin(); it != entities.end();) {
					if ((*it)->life) randomDirect(it, (*it)->dx, (*it)->dy);
					it++;
				}
				timer = 0;
				randomNumber = dist(gen);
			}
			changeCourseTank(randomNumber, timer, time, entities);
			updateBrick(listBrick);
			updateEnemy(time, entities, explEnemy, currentFrame, bigExplosionTexture, playerWinTime, playerWin);
			for (it = entities.begin(); it != entities.end(); it++) {
				int random;
				rangeValuesRandomly(random, 200);
				if ((*it)->life) {
					if (random == 1 && !(*it)->bulletReleased) {
						bullesEnemy.push_back(new Enemy(Bulles, "bullesEnemy", lvl, (*it)->coordinatesGunTank.x, (*it)->coordinatesGunTank.y, 8.0f, 8.0f, p, it));
						itBulles = bullesEnemy.end();
						--itBulles;
						(*itBulles)->individualNumber = (*it)->individualNumber;
						(*it)->bulletReleased = true;
					}
					enemiesClash(entities, it);
					collisionWithBricks(it, listBrick);
				}
			}
			checkForBonus(p, listPointsBonus, numberBonus, entities);
			collisionWithBricksForPlayer(p, listBrick);
			collisionWithEnemyForPlayer(p, entities);
			for (iter = bullesPlayer.begin(); iter != bullesPlayer.end(); iter++) {
				if ((*iter)->life) {
					checkHitTheEnemy(iter, entities);
				}
				if ((*iter)->life) {
					checkHitTheBrick(iter, listBrick);
				}
				if ((*iter)->life) {
					checkHitTheBaseForPlayer(window, targetProtecting, iter, playerLose);
				}
				if ((*iter)->life) {
					checkHitTheBulletEnemy(iter, bullesEnemy);
				}
			}
			for (itBulles = bullesEnemy.begin(); itBulles != bullesEnemy.end(); itBulles++) {
				if ((*itBulles)->life) {
					checkHitByPlayer(p, itBulles, entities, isHit);
				}
				if ((*itBulles)->life) {
					checkHitTheBaseForEnemy(window, targetProtecting, itBulles, playerLose);
				}
				if ((*itBulles)->life) {
					checkHitTheBrickPlayer(listBrick, entities, itBulles);
				}
			}
		}
		drawing(window, p, bullesPlayer, entities, bullesEnemy, listBrick, lvl, targetProtecting, numberEnemies, appearanceEnemies, spriteAppEnemies, text, isExplPlayer, bigExplosionSprite, playerLose, youLose, playerWin, youWin, listPointsBonus);
	}

	return 0;
}