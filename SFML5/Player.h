#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "level.h"
#include <vector>
#include <list>
#include "Entity.cpp"

using namespace sf;
using namespace std;

class Player : public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;
	bool cheats = false;
	Player(Image &image, String name, Level &lev, float X, float Y, float W, float H) :Entity(image, name, X, Y, W, H) {
		obj = lev.GetAllObjects();
		playerScore = 0; state = stay;
		if (name == "Player") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
			//sprite.setOrigin(w / 2, h / 2);
		}
	}

	void control(float time) {//, float &CurrentFrame) {
		//cur cu;// todo Создать структуру для анимации, общую для всех объектов
			   //cout << "CurrentFrame = " << cu.CurrentFrame << endl;
		float CurrentFrame;// += time;
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
				if ((obj[i].name == "solid"))// || (obj[i].name == "EasyEnemy"))//если встретили препятствие
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0;  onGround = true; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time) {//, float &CurrentFrame) {
		//cur cu;
		control(time);// , cu.CurrentFrame);
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