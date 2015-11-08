#include <SFML/Graphics.hpp>
#include <iostream>
#include "level_1.h"
#include "coordinate.h"

; using namespace sf;
using namespace std;




class Player {
private:float x, y = 0;
public:
	float w, h, dx, dy, speed = 0;
	int dir;
	int playerScore, health;
	bool life;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, float X, float Y, float W, float H) {
		dir = 0; speed = 0; playerScore = 0; health = 100;
		life = true;
		File = F;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		image.createMaskFromColor(Color(0, 0, 0));
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X, y = Y;
		sprite.setTextureRect(IntRect(0, 0, w, h));
	}

	void update(float time) {
		switch (dir) {
		case 0:dx = speed; dy = 0; break;
		case 1:dx = -speed; dy = 0; break;
		case 2:dx = 0; dy = speed; break;
		case 3:dx = 0; dy = -speed; break;
		}
		x += dx*time;
		y += dy*time;
		speed = 0;
		sprite.setPosition(x, y);
		interactionWithMap();
	}
	
	void interactionWithMap(){ //ф-ция взаимодействия с картой

		for (int i = y / 25; i < (y + h) / 25; i++)//проходимся по всей карте, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
			for (int j = x / 25; j<(x + w) / 25; j++){//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
				if ((TileMap[i][j] == '0') || (TileMap[i][j] == '1') || (TileMap[i][j] == '2'))//если наш квадратик соответствует символу 0 (стена), то проверяем "направление скорости" персонажа:
				{
					if (dy>0)//если мы шли вниз,
					{
						y = i * 25 - h;//то стопорим координату игрек персонажа. сначала получаем координату нашего квадратика на карте(стены) и затем вычитаем из высоты спрайта персонажа.
					}
					if (dy<0)
					{
						y = float(i) * 25.0f + 25.0f;//аналогично с ходьбой вверх. dy<0, значит мы идем вверх (вспоминаем координаты паинта)
					}
					if (dx>0)
					{
						x = j * 25 - w;//если идем вправо, то координата Х равна стена (символ 0) минус ширина персонажа
					}
					if (dx < 0)
					{
						x = j * 25 + 25;//аналогично идем влево
					}
				}

				/*if (TileMap[i][j] == '3' || (TileMap[i][j] == '1') || (TileMap[i][j] == '2')) {
					TileMap[i][j] = ' ';
				}*/
			}
	}

	/*float getplayercoordinateX() {
		return x;
	}
	float getplayercoordinateY() {
		return y;
	}*/

};

int main(){
	sf::RenderWindow window(sf::VideoMode(800, 700), "SFML works!");
	view.reset(sf::FloatRect(0, 0, 800, 700));
	Clock clock;
	float CurrentFrame = 0;
	
	Image map_image;
	map_image.loadFromFile("images/Sprite's.png");
	Texture map;
	map.loadFromImage(map_image);
	Sprite s_map;
	s_map.setTexture(map);


	Player p("hero.png", 300, 300, 37, 39);
	while (window.isOpen()) {
		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//movement_hero(herosprite, time, CurrentFrame);
		//std::cout << time << "\n";
		//drawing(window, herosprite, map_s);
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			p.dir = 1; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 2) (CurrentFrame -= 2);
			p.sprite.setTextureRect(IntRect(145, 52 * int(CurrentFrame), 40, 40));
			//getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right)) {
			p.dir = 0; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 2) (CurrentFrame -= 2);
			p.sprite.setTextureRect(IntRect(0, 52 * int(CurrentFrame), 40, 40));
			//getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up)) {
			p.dir = 3; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 2) (CurrentFrame -= 2);
			p.sprite.setTextureRect(IntRect(100, 52 * int(CurrentFrame), 40, 40));
			//getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down)) {
			p.dir = 2; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 2) (CurrentFrame -= 2);
			p.sprite.setTextureRect(IntRect(50, 52 * int(CurrentFrame), 40, 40));
			//getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}
		p.update(time);
		viewMap(time);
		window.setView(view);
		window.clear();

		window.draw(p.sprite);

		for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++) {
			/*cout << "i = " << i << endl;
			cout << "j = " << j << endl;
			cout << "TileMap[i][j] = " << TileMap[i][j] << endl;*/
			if (TileMap[i][j] == '0') (s_map.setTextureRect(IntRect(174, 0, 25, 25)));
			if (TileMap[i][j] == '1') (s_map.setTextureRect(IntRect(0, 0, 25, 25)));
			if (TileMap[i][j] == '2') (s_map.setTextureRect(IntRect(29, 0, 25, 25)));
			if (TileMap[i][j] == '3') (s_map.setTextureRect(IntRect(87, 29, 25, 25)));
			if (TileMap[i][j] == ' ') (s_map.setTextureRect(IntRect(145, 29, 25, 25)));
			s_map.setPosition(j * 25, i * 25);
			window.draw(s_map);
		}
		window.display();
	}

	return 0;
}