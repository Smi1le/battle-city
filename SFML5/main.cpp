#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
//#pragma comment (lib, "tinyxml.lib")
//#include "level_1.h"
#include "coordinate.h"
#include "level.h"
#include <vector>
#include <list>
//#include "TinyXML/tinyxml.h"
//#include "TinyXML/tinyxml.cpp"

; using namespace sf;
using namespace std;

class Entity {
public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
	}

	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
};

class Player: public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;
	bool cheats = false;
	Player(Image &image, String name,Level &lev, float X, float Y, float W, float H):Entity(image,name,  X, Y, W, H) {
		obj = lev.GetAllObjects();
		playerScore = 0; state = stay;
		if (name == "Player") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
	}

	void control(float &CurrentFrame, float &time) {
		if (Keyboard::isKeyPressed) {//если нажата клавиша
			if (Keyboard::isKeyPressed(Keyboard::Left)) {//а именно левая
				state = left; speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 2) (CurrentFrame -= 2);
				sprite.setTextureRect(IntRect(145, 52 * int(CurrentFrame), 40, 40));
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 2) (CurrentFrame -= 2);
				sprite.setTextureRect(IntRect(0, 52 * int(CurrentFrame), 40, 40));
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {//если нажата клавиша вверх и мы на земле, то можем прыгать
				state = up; speed = 0.1;// onGround = false;//увеличил высоту прыжка
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 2) (CurrentFrame -= 2);
				sprite.setTextureRect(IntRect(100, 52 * int(CurrentFrame), 40, 40));
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = down; speed = 0.1;
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 2) (CurrentFrame -= 2);
				sprite.setTextureRect(IntRect(50, 52 * int(CurrentFrame), 40, 40));
			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if ((obj[i].name == "solid") || (obj[i].name == "easyEnemy"))//если встретили препятствие
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0;  onGround = true;}
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time, float &CurrentFrame) {
		control(CurrentFrame, time);
		switch (state) {
		case right:dx = speed; dy = 0; break;
		case left:dx = -speed; dy = 0; break;
		case up:dx = 0; dy = -speed; break;
		case down:dx = 0; dy = speed; break;
		case stay: break;
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
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(10, 10, w, h));
			//dx = 0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				//if (obj[i].name == "solid"){//если встретили препятствие (объект с именем solid)
				if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
				if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx>0) { x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
				if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
				//}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {
			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//меняет направление примерно каждые 3 сек
			x += dx*time;
			checkCollisionWithMap(dx, 0);
			cout << "x = " << x << endl;
			cout << "w = " << w << endl;
			cout << "y = " << y << endl;
			cout << "h = " << h << endl;
			sprite.setPosition(x, y);
			//sprite.setOrigin(0, 0);
			//if (health <= 0) { life = false; }
		}
	}
};

int main(){
	sf::RenderWindow window(sf::VideoMode(800, 700), "SFML works!");
	view.reset(sf::FloatRect(0, 0, 800, 700));
	Clock clock;
	float CurrentFrame = 0;

	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/easyEnemy.png");
	easyEnemyImage.createMaskFromColor(Color(255, 0, 0));

	
	Image hero;
	hero.loadFromFile("images/hero.png");
	hero.createMaskFromColor(Color::Black);

	
	Object player = lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Object easyEnemyObject = lvl.GetObject("easyEnemy");
	
	Player p(hero, "Player", lvl, player.rect.left, player.rect.top, 37, 39);//передаем координаты прямоугольника player из карты в координаты нашего игрока
	Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 37, 39); 
	
	//Player p( hero,"Player",lvl, 300, 300, 37, 39 );
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		if (Keyboard::isKeyPressed(Keyboard::F12)) {
			p.cheats = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::F11)) {
			p.cheats = false;
		}

		p.update(time, CurrentFrame);
		easyEnemy.update(time);
		window.setView(view);
		window.clear();

		
	
		window.draw(p.sprite);
		lvl.Draw(window);
		//window.draw(easyEnemy.sprite);
		/*for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++) {
			if (TileMap[i][j] == '0') (s_map.setTextureRect(IntRect(174, 0, 25, 25)));
			if (TileMap[i][j] == '1') (s_map.setTextureRect(IntRect(0, 0, 25, 25)));
			if (TileMap[i][j] == '2') (s_map.setTextureRect(IntRect(29, 0, 25, 25)));
			if (TileMap[i][j] == '3') (s_map.setTextureRect(IntRect(87, 29, 25, 25)));
			if (TileMap[i][j] == ' ') (s_map.setTextureRect(IntRect(145, 29, 25, 25)));
			s_map.setPosition(j * 25, i * 25);
			window.draw(s_map);
		}*/
		window.display();
	}

	return 0;
}