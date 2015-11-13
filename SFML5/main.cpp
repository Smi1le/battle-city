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
	std::vector<Object> obj;//������ �������� �����
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

	FloatRect getRect() {//�-��� ��������� ��������������. ��� �����,������� (���,�����).
		return FloatRect(x, y, w, h);//��� �-��� ����� ��� �������� ������������ 
	}

	virtual void update(float time) = 0;

};

class Player: public Entity {
public:
	enum { left, right, up, down, jump, stay } state;
	int playerScore;
	bool cheats = false;
	Player(Image &image, String name,Level &lev, float X, float Y, float W, float H):Entity(image, name, X, Y, W, H) {
		obj = lev.GetAllObjects();
		playerScore = 0; state = stay;
		if (name == "Player") {
			sprite.setTextureRect(IntRect(0, 0, w, h));
		}
	}

	void control(float time){//, float &CurrentFrame) {
		cur cu;// todo ������� ��������� ��� ��������, ����� ��� ���� ��������
		//cout << "CurrentFrame = " << cu.CurrentFrame << endl;
		if (Keyboard::isKeyPressed) {//���� ������ �������
			if (Keyboard::isKeyPressed(Keyboard::Left)) {//� ������ �����
				state = left; speed = 0.1;
				cu.CurrentFrame += 0.005*time;
				if (cu.CurrentFrame > 2) (cu.CurrentFrame -= 2);
				sprite.setTextureRect(IntRect(145, 52 * int(cu.CurrentFrame), 40, 40));
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; speed = 0.1;
				cu.CurrentFrame += 0.005*time;
				if (cu.CurrentFrame > 2) (cu.CurrentFrame -= 2);
				sprite.setTextureRect(IntRect(0, 52 * int(cu.CurrentFrame), 40, 40));
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up))) {//���� ������ ������� ����� � �� �� �����, �� ����� �������
				state = up; speed = 0.1;// onGround = false;//�������� ������ ������
				cu.CurrentFrame += 0.005*time;
				if (cu.CurrentFrame > 2) (cu.CurrentFrame -= 2);
				sprite.setTextureRect(IntRect(100, 52 * int(cu.CurrentFrame), 40, 40));
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = down; speed = 0.1;
				cu.CurrentFrame += 0.005*time;
				if (cu.CurrentFrame > 2) (cu.CurrentFrame -= 2);
				sprite.setTextureRect(IntRect(50, 52 * int(cu.CurrentFrame), 40, 40));
			}
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = 0; i<obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if ((obj[i].name == "solid"))// || (obj[i].name == "EasyEnemy"))//���� ��������� �����������
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0;  onGround = true;}
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time){//, float &CurrentFrame) {
		cur cu;
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

class Enemy :public Entity {
public:
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H) {
		obj = lvl.GetObjects("solid");//��������������.�������� ������ ������� ��� �������������� ����� � ������
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(51, 0, w, h));
			int randomNumber = rand() % 4 + 1;
			//cout << "randomNumber = " << endl;
			if (randomNumber == 1) {
				dx = 0.1;
				dy = 0;
			}
			else if (randomNumber == 2){
				dx = -0.1;
				dy = 0;
			}
			else if (randomNumber == 3) {
				dx = 0;
				dy = 0.1;
			}
			else {
				dx = 0;
				dy = -0.1;
			}
		}
	}

	void choiceOfDirection(float &dx, float &dy) {
		int randomNumber = rand() % 3 + 1;
		//cout << "randomNumber = " << endl;
		//cout << dx;
		if (dx > 0) {
			/*if ((randomNumber == 1)) {
				dx = 0.1;
				dy = 0;
			}*/
			if (randomNumber == 2) {
				dx = -0.1;
				dy = 0;
			}
			else if (randomNumber == 3) {
				dx = 0;
				dy = 0.1;
			}
			else if (randomNumber == 4){
				dx = 0;
				dy = -0.1;
			}
		}
		else if (dx < 0) {
			if (randomNumber == 1) {
				dx = 0.1;
				dy = 0;
			}
			else if (randomNumber == 3) {
				dx = 0;
				dy = 0.1;
			}
			else if (randomNumber == 4){
				dx = 0;
				dy = -0.1;
			}
		}
		else if (dy < 0) {
			if (randomNumber == 1) {
				dx = 0.1;
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
				dx = 0.1;
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
		for (int i = 0; i<obj.size(); i++)//���������� �� ��������
			if (getRect().intersects(obj[i].rect))//��������� ����������� ������ � ��������
			{
				if (obj[i].name == "solid"){
					choiceOfDirection(dx, dy);
				}
			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {
			//moveTimer += time;if (moveTimer>3000){ dx *= -1; moveTimer = 0; }//������ ����������� �������� ������ 3 ���
			
			
			checkCollisionWithMap(dx, dy);
			x += dx * time;
			y += dy * time;
			sprite.setPosition(x, y);
			if (health <= 0) { life = false; }
		}
	}

	void parallelPlayerAndEnemy(float time, int flag) {

	}
};


int main(){
	sf::RenderWindow window(sf::VideoMode(800, 700), "SFML works!");
	view.reset(sf::FloatRect(0, 0, 800, 700));
	Clock clock;
	

	std::list<Entity*>  entities;//������ ������, ���� ���� ������ �������.�������� ������.
	std::list<Entity*>::iterator it;//�������� ����� ��������� �� ��-��� ������
	std::list<Entity*>::iterator it2;//������ ��������.��� �������������� ����� ��������� ������

	Level lvl;//������� ��������� ������ �������
	lvl.LoadFromFile("map.tmx");//��������� � ���� �����, ������ ������ � ������� ������� �� �� ����������.

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/easyEnemy.png");
	easyEnemyImage.createMaskFromColor(Color(0, 128, 128));
	
	/*Image bulles;
	easyEnemyImage.loadFromFile("images/Bulles.png");
	easyEnemyImage.createMaskFromColor(Color(0, 128, 128));*/
	
	Image hero;
	hero.loadFromFile("images/hero.png");
	hero.createMaskFromColor(Color::Black);

	
	Object player = lvl.GetObject("player");//������ ������ �� ����� �����.������ ���������� ������ � ������ ��� ������ ����
	//Object easyEnemyObject = lvl.GetObject("easyEnemy");

	std::vector<Object> e = lvl.GetObjects("EasyEnemy");//��� ������� ����� �� tmx ����� �������� � ���� �������

	for (int i = 0; i < e.size(); i++) {//���������� �� ��������� ����� �������(� ������ �� ������)
		entities.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 45, 45));//� ���������� � ������ ���� ����� ������ � �����
		e[i].rect.left;//����� �
		e[i].rect.top;//����� Y
	}
	Player p(hero, "Player", lvl, player.rect.left, player.rect.top, 37, 39);//�������� ���������� �������������� player �� ����� � ���������� ������ ������
	//Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 45, 45); 
	
	

	

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds(); //���� ��������� ����� � �������������
		clock.restart(); //������������� �����
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

		p.update(time);// , CurrentFrame);
		for (it = entities.begin(); it != entities.end();){
			Entity *b = *it;
			b->update(time);
			if (b->life == false) { it = entities.erase(it); delete b; }
			else it++;
		}
		for (it = entities.begin(); it != entities.end(); it++) {
			if (int(p.sprite.getPosition().x) == int((*it)->sprite.getPosition().x)) {
				if ((p.sprite.getPosition().y - (*it)->sprite.getPosition().y) > 0) {
					(*it)->dx = 0;
					(*it)->dy = 0.1;
				}
				else if ((p.sprite.getPosition().y - (*it)->sprite.getPosition().y) < 0) {
					(*it)->dx = 0;
					(*it)->dy = -0.1;
				}
				(*it)->y += (*it)->dy * time;
				(*it)->x += (*it)->dx * time;
				(*it)->sprite.setPosition((*it)->x, (*it)->y);
			}
		}
		for (it = entities.begin(); it != entities.end(); it++){
			if ((*it)->getRect().intersects(p.getRect())){
				if ((*it)->name == "EasyEnemy") {
					if ((*it)->dx>0){
						(*it)->life = false;
					}
					if ((*it)->dx < 0){
						(*it)->life = false;
					}
					if ((*it)->dy < 0) {
						(*it)->life = false;
					}
					if ((*it)->dy > 0) {
						(*it)->life = false;
					}
				}
			}
			for (it2 = entities.begin(); it2 != entities.end(); it2++) {
				if ((*it)->getRect() != (*it2)->getRect())
					if (((*it)->getRect().intersects((*it2)->getRect())) && ((*it)->name == "EasyEnemy") && ((*it2)->name == "EasyEnemy")){
						(*it)->dx *= -1;
						(*it)->sprite.scale(-1, 1);
					}
			}
		}
		
		window.setView(view);
		window.clear();

		
	
		window.draw(p.sprite);
		//window.draw(easyEnemy.sprite);
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite); //������ entities ������� (������ ��� ������ �����)
		}
		lvl.Draw(window);
		window.display();
	}

	return 0;
}