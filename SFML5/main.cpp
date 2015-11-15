#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "coordinate.h"
#include "level.h"
#include <vector>
#include <list>

; using namespace sf;
using namespace std;
struct cur {
	float CurrentFrame = 0;
};

class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed, moveTimer, CurrentFrame, speedX, speedY;
	int w, h, health;
	Vector2i coordinatesGunTank;
	bool life, isMove, onGround;
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

	virtual void update(float time, Object player) = 0;// , Object persona) = 0;

};

class Player : public Entity {
public:
	enum { LEFT, RIGHT, UP, DOWN, JUMP, STAY } state;
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

	void control(float time, Object player) {//, float &CurrentFrame) {
							  //cur cu;// todo Создать структуру для анимации, общую для всех объектов
							  //cout << "CurrentFrame = " << cu.CurrentFrame << endl;
		CurrentFrame += 0.005f * time;
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
				if ((obj[i].name == "solid"))// || (obj[i].name == "EasyEnemy"))//если встретили препятствие
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0;  onGround = true; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time, Object player) {//, float &CurrentFrame) {
							 //cur cu;
		control(time, player);// , cu.CurrentFrame);
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
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, Player p) :Entity(image, Name, X, Y, W, H, coordinatesGunTank) {
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(51, 0, w, h));
			dx = 0.1f;
		}
		if (name == "Bulles") {
			if ((p.speedX > 0) && (p.speedY == 0)) {
				dx = 0.15f;
				dy = 0;
				sprite.setTextureRect(IntRect(26, 0, w, h));
			}
			if ((p.speedX < 0) && (p.speedY == 0)) {
				dx = -0.15f;
				dy = 0;
				sprite.setTextureRect(IntRect(16, 0, w, h));
			}
			if ((p.speedX == 0) && (p.speedY < 0)) {
				dx = 0;
				dy = -0.15f;
				sprite.setTextureRect(IntRect(0, 0, w, h));
			}
			if ((p.speedX == 0) && (p.speedY > 0)) {
				dx = 0;
				dy = 0.15f;
				sprite.setTextureRect(IntRect(7, 2, w, h));
			}
		}
	}

	void choiceOfDirection(float &dx, float &dy) {
		int randomNumber = rand() % 3 + 1;
		if (dx > 0) {
			if (randomNumber == 2) {
				dx = -0.1f;
				dy = 0;
			}
			else if (randomNumber == 3) {
				dx = 0;
				dy = 0.1f;
			}
			else if (randomNumber == 4){
				dx = 0;
				dy = -0.1f;
			}
		}
		else if (dx < 0) {
			if (randomNumber == 1) {
				dx = 0.1f;
				dy = 0;
			}
			else if (randomNumber == 3) {
				dx = 0;
				dy = 0.1f;
			}
			else if (randomNumber == 4){
				dx = 0;
				dy = -0.1f;
			}
		}
		else if (dy < 0) {
			if (randomNumber == 1) {
				dx = 0.1f;
				dy = 0;
			}
			else if (randomNumber == 2) {
				dx = -0.1f;
				dy = 0;
			}
			else if (randomNumber == 4){
				dx = 0;
				dy = 0.1f;
			}
		}
		else if (dy > 0) {
			if (randomNumber == 1) {
				dx = 0.1f;
				dy = 0;
			}
			else if (randomNumber == 2) {
				dx = -0.1f;
				dy = 0;
			}
			else if (randomNumber == 3){
				dx = 0;
				dy = -0.1f;
			}
		}
		
	}

	void checkCollisionWithMap(float &Dx, float &Dy)
	{
		for (int i = 0; i < int(obj.size()); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid"){// || (obj[i-1].name == "solid")){
					//choiceOfDirection(dx, dy);
					if (name == "EasyEnemy") {
						Dx *= -1;
						Dy *= -1;
					}
					if (name == "Bulles") {
						life = false;
					}
				}
			}
	}

	void update(float time, Object enemy)
	{
		if (name == "EasyEnemy") {
			x += dx * time;
			y += dy * time;
			sprite.setPosition(x, y);
			checkCollisionWithMap(dx, dy);
			if (health <= 0) { life = false; }
		}
		if (name == "Bulles") {
			checkCollisionWithMap(dx, dy);
			x += dx * time;
			y += dy * time;
			sprite.setPosition(x, y);
		}
	}

	void parallelPlayerAndEnemy(float time, int flag) {

	}
};

void playerShotTank(int x, int y) {
	
}

int main(){
	sf::RenderWindow window(sf::VideoMode(800, 700), "SFML works!");
	view.reset(sf::FloatRect(0, 0, 800, 700));
	Clock clock;
	

	std::list<Entity*>  entities;//создаю список, сюда буду кидать объекты.например врагов.
	std::list<Entity*>::iterator it;//итератор чтобы проходить по эл-там списка
	std::list<Entity*>::iterator it2;//второй итератор.для взаимодействия между объектами списка
	
	std::list<Entity*> bullesPlayer;
	std::list<Entity*>::iterator iter;

	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/easyEnemy.png");
	easyEnemyImage.createMaskFromColor(Color(0, 128, 128));
	
	Image Bulles;
	Bulles.loadFromFile("images/Bulles.png");
	Bulles.createMaskFromColor(Color(0, 128, 128));
	
	Image hero;
	hero.loadFromFile("images/hero.png");
	hero.createMaskFromColor(Color::Black);

	
	Object player = lvl.GetObject("player");

	Player p(hero, "Player", lvl, player.rect.left, player.rect.top, 37.0f, 39.0f, player);

	std::vector<Object> e = lvl.GetObjects("EasyEnemy");

	for (int i = 0; i < int(e.size()); i++) {
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 45, 45, p));
		e[i].rect.left;//коорд Х
		e[i].rect.top;//коорд Y
	}
	
	float timeNoPressed = 2000;
	

	while (window.isOpen()) {
		float time = float(clock.getElapsedTime().asMicroseconds());
		clock.restart(); 
		time = time / 800;
		timeNoPressed += time;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if ((Keyboard::isKeyPressed(Keyboard::RControl)) && timeNoPressed > 2000){
			bullesPlayer.push_back(new Enemy(Bulles, "Bulles", lvl, p.coordinatesGunTank.x, p.coordinatesGunTank.y, 8.0f, 8.0f, p));
			timeNoPressed = 0;
		}

		
		p.update(time, player);
		for (iter = bullesPlayer.begin(); iter != bullesPlayer.end();) {
			cout << "sdasdassasssssssssssss" << endl;
			(*iter)->update(time, player);
			cout << "7777777777777777" << endl;
			if ((*iter)->life == false) {
				delete (*iter);
				iter = bullesPlayer.erase(iter); 
				 
			}
			else iter++;
		}

		for (it = entities.begin(); it != entities.end();){
			Entity *b = *it;
			b->update(time, player);
			if (b->life == false) { it = entities.erase(it); delete b; }
			else it++;
		}

		for (it = entities.begin(); it != entities.end(); it++) {
			if (int(p.sprite.getPosition().x) == int((*it)->sprite.getPosition().x)) {
				if ((p.sprite.getPosition().y - (*it)->sprite.getPosition().y) > 0) {
					(*it)->dx = 0;
					(*it)->dy = 0.1f;
				}
				else if ((p.sprite.getPosition().y - (*it)->sprite.getPosition().y) < 0) {
					(*it)->dx = 0;
					(*it)->dy = -0.1f;
				}
				(*it)->y += (*it)->dy * time;
				(*it)->x += (*it)->dx * time;
				(*it)->sprite.setPosition((*it)->x, (*it)->y);
			}
		}
		for (it = entities.begin(); it != entities.end(); it++){
			for (iter = bullesPlayer.begin(); iter != bullesPlayer.end(); iter++) {
				if ((*it)->getRect().intersects((*iter)->getRect())) {
					if ((*it)->name == "EasyEnemy") {
						cout << "cccccc" << endl;
						if ((*it)->dx > 0) {
							(*it)->life = false;
							(*iter)->life = false;
						}
						if ((*it)->dx < 0) {
							(*it)->life = false;
							(*iter)->life = false;
						}
						if ((*it)->dy < 0) {
							(*it)->life = false;
							(*iter)->life = false;
						}
						if ((*it)->dy > 0) {
							(*it)->life = false;
							(*iter)->life = false;
						}
					}
				}
			}
		}

		window.setView(view);
		window.clear();

		lvl.Draw(window);
		window.draw(p.sprite);
		for (iter = bullesPlayer.begin(); iter != bullesPlayer.end(); iter++) {
			window.draw((*iter)->sprite); //рисуем entities объекты (сейчас это только враги)
		}
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite); //рисуем entities объекты (сейчас это только враги)
		}
		
		window.display();
	}

	return 0;
}