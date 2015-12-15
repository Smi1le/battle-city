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
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H, Vector2i coordinatesGunTank) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
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
		if (name == "Player") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
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
				sprite.setTextureRect(IntRect(145, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = x;
				coordinatesGunTank.y = y + h / 2;
				speedX = -0.15f;
				speedY = 0.0f;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = RIGHT; speed = 0.1f;
				sprite.setTextureRect(IntRect(0, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = x + w;
				coordinatesGunTank.y = y + h / 2;
				speedX = 0.15f;
				speedY = 0.0f;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {//если нажата клавиша вверх и мы на земле, то можем прыгать
				state = UP; speed = 0.1f;// onGround = false;//увеличил высоту прыжка
				sprite.setTextureRect(IntRect(100, 52 * int(CurrentFrame), 40, 40));
				coordinatesGunTank.x = x + w / 2;
				coordinatesGunTank.y = y;
				speedX = 0.0f;
				speedY = -0.15f;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = DOWN; speed = 0.1f;
				sprite.setTextureRect(IntRect(50, 52 * int(CurrentFrame), 40, 40));
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
		sprite.setPosition(x, y);
	}
};

class Enemy :public Entity {
public:
	//enum { LEFT, RIGHT, UP, DOWN, JUMP, STAY } state;
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, Player p, list<Entity*>::iterator it) :Entity(image, Name, X, Y, W, H, coordinatesGunTank) {
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(51, 0, w, h));
			state = DOWN; speed = 0.1f;
			delay = 2;
			//dx = 0.1f;
		}
		if (name == "Bulles") {
			if ((p.speedX > 0) && (p.speedY == 0)) {
				dx = 0.30f;
				dy = 0;
				sprite.setTextureRect(IntRect(26, 2, w, h));
			}
			if ((p.speedX < 0) && (p.speedY == 0)) {
				dx = -0.30f;
				dy = 0;
				sprite.setTextureRect(IntRect(16, 2, w, h));
			}
			if ((p.speedX == 0) && (p.speedY < 0)) {
				dx = 0;
				dy = -0.30f;
				sprite.setTextureRect(IntRect(0, 0, w, h));
			}
			if ((p.speedX == 0) && (p.speedY > 0)) {
				dx = 0;
				dy = 0.30f;
				sprite.setTextureRect(IntRect(7, 2, w, h));
			}
		}
		if (name == "Deagle"){
			sprite.setPosition(X, Y);
		}
		if (name == "brick") {
			sprite.setPosition(X, Y);
		}
		if (name == "bullesEnemy") {
			bulletReleased = true;
			cout << "(*it)->speedX = " << endl;
			cout << "(*it)->speedY" << endl;
			if (((*it)->dx > 0) && ((*it)->dy == 0)) {
				cout << "1111" << endl;
				dx = 0.30f;
				dy = 0;
				sprite.setTextureRect(IntRect(26, 2, w, h));
			}
			if (((*it)->dx < 0) && ((*it)->dy == 0)) {
				cout << "2222" << endl;
				dx = -0.30f;
				dy = 0;
				sprite.setTextureRect(IntRect(16, 2, w, h));
			}
			if (((*it)->dx == 0) && ((*it)->dy < 0)) {
				cout << "3333" << endl;
				dx = 0;
				dy = -0.30f;
				sprite.setTextureRect(IntRect(0, 0, w, h));
			}
			if (((*it)->dx == 0) && ((*it)->dy > 0)) {
				cout << "4444" << endl;
				dx = 0;
				dy = 0.30f;
				sprite.setTextureRect(IntRect(7, 2, w, h));
			}
		}
	}

	
	void changeDirection(float Dx, float Dy) {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(1, 3);
		int randomNumber = dist(gen);
		cout << "state = " << state << endl;
		
		
		if (Dx > 0) { //Right
			if (randomNumber == 1) {
				state = LEFT; speed = 0.1f;
				x -= 2;

			}
			if (randomNumber == 2) {
				state = DOWN; speed = 0.1f;
				//y -= 2;
				x -= 2;
			}
			else if (randomNumber == 3) {
				state = UP; speed = 0.1f;
				//y -= 2;
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
				//y -= 2;
			}
			else if (randomNumber == 3) {
				state = UP; speed = 0.1f;
				x += 2;
				//y -= 2;
			}
		}
		if (Dy < 0) { //Up
			if (randomNumber == 1) {
				state = RIGHT; speed = 0.1f;
				y += 2;
				//x -= 2;
			}
			else if (randomNumber == 2) {
				state = LEFT; speed = 0.1f;
				y += 2;
				//x -= 2;
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
				//x -= 2;
			}
			else if (randomNumber == 2) {
				state = LEFT; speed = 0.1f;
				y -= 2;
				//x -= 2;
			}
			else if (randomNumber == 3) {
				state = UP; speed = 0.1f;
				y -= 2;
			}
		}




		cout << "state = " << state << endl;

	}
	

	void checkCollisionWithMap(float time)
	{
		for (int i = 0; i < int(obj.size()); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid") {// || (obj[i-1].name == "solid")){
					if (name == "EasyEnemy") {
						cout << "88888888888" << endl;
						changeDirection(dx, dy);
						delay = 0;
					}
				}
			}
	}

	void checkCollisionWithMapForBulles() {
		for (int i = 0; i < int(obj.size()); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if ((obj[i].name == "solid")){// && (obj[i].type == "destroy")) {
					life = false;
					bulletReleased = false;
				}
			}
	}	

	void animation(int X, int Y, float time, int w, int h, string dir) {
		//cout << "CurrentFrame = " << CurrentFrame << endl;
		if (CurrentFrame > 2) (CurrentFrame = 0);
		sprite.setTextureRect(IntRect(X, Y * int(CurrentFrame), w, h));
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
				sprite.setPosition(x, y);
				checkCollisionWithMap(time);
			}
			if (health <= 0) { life = false; }
		}
		if (name == "Bulles" || name == "bullesEnemy") {
			checkCollisionWithMapForBulles();
			x += dx * time;
			y += dy * time;
			sprite.setPosition(x, y);
		}	
	}

	void parallelPlayerAndEnemy(float time, int flag) {

	}
};

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

void changeDirection(float Dx, float Dy, std::list<Entity*>::iterator it) {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, 4);
	int randomNumber = dist(gen);
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

void checkCollisionWithMap(float Dx, float Dy, Player &p, std::list<Entity*>::iterator it) {
	if (Dy > 0) { p.y = (*it)->sprite.getPosition().y - p.h;  p.dy = 0; }//  onGround = true;
	if (Dy < 0) { p.y = (*it)->sprite.getPosition().y + (*it)->h;   p.dy = 0; }
	if (Dx > 0) { p.x = (*it)->sprite.getPosition().x - p.w; }
	if (Dx < 0) { p.x = (*it)->sprite.getPosition().x + (*it)->w; }
	//system("pause");
}

void deadBullet(std::list<Entity*> &bullesEnemy, float time, RenderWindow &window) {
	float currentFrame = 0;
	Sprite sprite;
	Texture texture;
	Image image;
	image.loadFromFile("images/explosionBulles.png");
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	std::list<Entity*>::iterator itBulles = bullesEnemy.begin();
	
	cout << "(*itBulles)->sprite.getPosition().x = " << (*itBulles)->sprite.getPosition().x << endl;
	cout << "(*itBulles)->sprite.getPosition().yx = " << (*itBulles)->sprite.getPosition().y << endl;
	itBulles = bullesEnemy.begin();
	sprite.setPosition((*itBulles)->sprite.getPosition().x, (*itBulles)->sprite.getPosition().y);
	while (currentFrame < 10) {
		currentFrame += 0.005f * time;
		if (currentFrame < 6 ) {
			sprite.setTextureRect(IntRect(0, 0, 13, 12));
			window.draw(sprite);
			
		}
		else {
			sprite.setTextureRect(IntRect(14, 0, 14, 13));
			window.draw(sprite);
		}
	}
}

void  explosion(float time, RenderWindow &window, list<Entity*>::iterator &it, list<Entity*> &list) {

	float x = (*it)->sprite.getPosition().x;
	float y = (*it)->sprite.getPosition().y;
	delete (*it);
	it = list.erase(it);
	float currentFrame = 0;
	Texture *texture = new Texture;
	Sprite *sprite = new Sprite;
	texture->loadFromFile("images/explosionBulles.png");
	sprite->setTexture(*texture);

	do {
		if (currentFrame < 3) {
			sprite->setTextureRect(IntRect(0, 0, 13, 12));
			window.draw(*sprite);
		}
		else {
			sprite->setTextureRect(IntRect(14 , 0, 13, 12));
			window.draw(*sprite);
		}
		currentFrame += 0.005f * time;
		cout << currentFrame << endl;
		
	} while (currentFrame < 6);
	//system("pause");
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
	
	Image eagle;
	eagle.loadFromFile("images/eagle.png");
	eagle.createMaskFromColor(Color::Black);

	Image hero;
	hero.loadFromFile("images/hero.png");
	hero.createMaskFromColor(Color::Black);

	
	Object player = lvl.GetObject("player");

	Object targetDeagle = lvl.GetObject("Deagle");

	Player p(hero, "Player", lvl, player.rect.left, player.rect.top, 37.0f, 39.0f, player);

	Enemy targetProtecting(eagle, "Deagle", lvl, targetDeagle.rect.left, targetDeagle.rect.top, 50.0f, 50.0f, p, it);

	std::vector<Object> e = lvl.GetObjects("EasyEnemy");
	std::vector<Object> brick = lvl.GetObjects("destroyedSolid");

	for (int i = 0; i < int(e.size()); i++) {
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 45, 45, p, it));
		e[i].rect.left;//коорд Х
		e[i].rect.top;//коорд Y
	}
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
	float timerAppearenceEnemy = 0;
	int pointRessurection = 0;
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

		
		if (timerAppearenceEnemy > 8000 && p.numberEnemyTank <= 20) {
			entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[pointRessurection].rect.left, e[pointRessurection].rect.top, 45, 45, p, it));
			timerAppearenceEnemy = 0;
			p.numberEnemyTank++;
		}

		p.update(time);
		if (p.life == false) {
			window.close();
		}
		for (iter = bullesPlayer.begin(); iter != bullesPlayer.end();) {
			(*iter)->update(time);
			if ((*iter)->life == false) {
				//explosion(time, window, iter, bullesPlayer);
				delete (*iter);
				iter = bullesPlayer.erase(iter);
			}
			else iter++;
		}
		for (itBulles = bullesEnemy.begin(); itBulles != bullesEnemy.end();) {
			(*itBulles)->update(time);
			
			if ((*itBulles)->life == false) {
				for (it = entities.begin(); it != entities.end(); it++) {
					if ((*it)->individualNumber == (*itBulles)->individualNumber) {
						(*it)->bulletReleased = (*itBulles)->bulletReleased;
					}
				}
				delete *itBulles;
				itBulles = bullesEnemy.erase(itBulles);
				//currentFrame += 0.005f * time;
				//explosion(time, window, itBulles, bullesEnemy);
			}
			else itBulles++;
		}
		if (randomNumber * 100 < timer) {
			for (it = entities.begin(); it != entities.end();) {
				randomDirect(it, (*it)->dx, (*it)->dy);
				(*it)->update(time);
				if ((*it)->life == false) { it = entities.erase(it); delete (*it); }
				else it++;
			}
			timer = 0;
			randomNumber = dist(gen);
		}

		for (itBrick = listBrick.begin(); itBrick != listBrick.end();) {
			if ((*itBrick)->life == false) { delete (*itBrick); itBrick = listBrick.erase(itBrick);  }
			else itBrick++;
		}

		for (it = entities.begin(); it != entities.end();){
			Entity *b = *it;
			b->update(time);
			if (b->life == false) { it = entities.erase(it); delete b; }
			else it++;
		}

		for (it = entities.begin(); it != entities.end(); it++) {
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> dist(1, 200);
			int random = dist(gen);
			cout << random << endl;
			if (random == 1 && !(*it)->bulletReleased) {
				bullesEnemy.push_back(new Enemy(Bulles, "bullesEnemy", lvl, (*it)->coordinatesGunTank.x, (*it)->coordinatesGunTank.y, 8.0f, 8.0f, p, it));
				itBulles = bullesEnemy.end();
				--itBulles;
				(*itBulles)->individualNumber = (*it)->individualNumber;
				(*it)->bulletReleased = true;
			}
			for (it2 = entities.begin(); it2 != entities.end(); it2++) {
				if ((*it)->getRect() != (*it2)->getRect())//при этом это должны быть разные прямоугольники
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EasyEnemy") && ((*it2)->name == "EasyEnemy"))//если столкнулись два объекта и они враги
					{
						(*it)->speed *= -1;
						(*it2)->speed *= -1;
					}
			}
			for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
				if (((*it)->getRect().intersects((*itBrick)->getRect()))) {
					changeDirection((*it)->dx, (*it)->dy, it);
					(*it)->delay = 0;
				}
			}
		}


		for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
			if ((p.getRect().intersects((*itBrick)->getRect()))) {
				checkCollisionWithMap(p.dx, p.dy, p, itBrick);
			}
		}



		for (iter = bullesPlayer.begin(); iter != bullesPlayer.end(); iter++) {
			for (it = entities.begin(); it != entities.end(); it++) {
				if ((*it)->getRect().intersects((*iter)->getRect())) {
					if ((*it)->name == "EasyEnemy") {
						(*it)->life = false;
						(*iter)->life = false;
					}
				}
			}
			for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
				if ((*iter)->getRect().intersects((*itBrick)->getRect())) {
					(*itBrick)->life = false;
					(*iter)->life = false;
				}
			}
			if ((*iter)->getRect().intersects(targetProtecting.getRect())) {
				window.close();
				(*iter)->life = false;
			}
		}
		for (itBulles = bullesEnemy.begin(); itBulles != bullesEnemy.end(); itBulles++) {
			if ((*itBulles)->getRect().intersects(p.getRect())) {
				for (it = entities.begin(); it != entities.end();) {
					if ((*it)->individualNumber == (*itBulles)->individualNumber) {
						(*itBulles)->life = false;
						(*itBulles)->bulletReleased = false;
						p.life = false;
						it = entities.end();
					}
					else it++;
					
				}
			}
			if ((*itBulles)->getRect().intersects(targetProtecting.getRect())) {
				window.close();
				(*itBulles)->life = false;
				(*itBulles)->bulletReleased = false;
			}
			for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
				if ((*itBulles)->getRect().intersects((*itBrick)->getRect())) {
					for (it = entities.begin(); it != entities.end();) {
						if ((*it)->individualNumber == (*itBulles)->individualNumber) {
							(*itBulles)->bulletReleased = false;
							(*itBulles)->life = false;
							(*itBrick)->life = false;
							it = entities.end();
						}
						else (it++);
					}
				}
			}
		}
		window.clear();
		lvl.Draw(window);
		window.draw(p.sprite);
		window.draw(targetProtecting.sprite);
		for (iter = bullesPlayer.begin(); iter != bullesPlayer.end(); iter++) {
			window.draw((*iter)->sprite); 
		}
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite); 
		}
		for (itBulles = bullesEnemy.begin(); itBulles != bullesEnemy.end(); itBulles++) {
			window.draw((*itBulles)->sprite);
		}
		for (itBrick = listBrick.begin(); itBrick != listBrick.end(); itBrick++) {
			window.draw((*itBrick)->sprite);
		}
		window.display();
	}

	return 0;
}