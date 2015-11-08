#include <SFML/Graphics.hpp>
#include <iostream>
#include "level_1.h"
#include "coordinate.h"

; using namespace sf;
using namespace std;


/*void movement_hero(Sprite &herosprite, float time, float &CurrentFrame){
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		CurrentFrame += 0.005*time;
		if (CurrentFrame > 2) (CurrentFrame -= 2);
		herosprite.setTextureRect(IntRect(201, 67 * int(CurrentFrame), 65, 65));
		herosprite.move(-0.1*time, 0);
	} 
	else if (Keyboard::isKeyPressed(Keyboard::Right)){
		CurrentFrame += 0.005*time;
		if (CurrentFrame > 2) (CurrentFrame -= 2);
		herosprite.setTextureRect(IntRect(0, 67 * int(CurrentFrame), 65, 65));
		herosprite.move(0.1 * time, 0); 
	} 
	else if (Keyboard::isKeyPressed(Keyboard::Up)) {
		CurrentFrame += 0.005*time;
		if (CurrentFrame > 2) (CurrentFrame -= 2);
		herosprite.setTextureRect(IntRect(134, 67 * int(CurrentFrame), 65, 65));
		herosprite.move(0, -0.1 * time);
	} 
	else if (Keyboard::isKeyPressed(Keyboard::Down)){ 
		CurrentFrame += 0.005*time;
		if (CurrentFrame > 2) (CurrentFrame -= 2);
		herosprite.setTextureRect(IntRect(67, 67 * int(CurrentFrame), 65, 65));
		herosprite.move(0, 0.1 * time); 
	} 
}*/

/*void drawing(RenderWindow &window, Sprite &herosprite, Sprite &tex) {
	window.clear();
	window.draw(tex);
	window.draw(herosprite);
	window.display();
}*/

class Player {
private:float x, y = 0;
public:
	float w, h, dx, dy, speed = 0;
	int dir = 0;
	String File;
	Image image;
	Texture texture;
	Sprite sprite;
	Player(String F, float X, float Y, float W, float H) {
		File = F;
		w = W; h = H;
		image.loadFromFile("images/" + File);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		x = X, y = Y;
		sprite.setTextureRect(IntRect(99, 99, w, h));
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


	Player p("hero.png", 134, 0, 47, 47);
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
			p.sprite.setTextureRect(IntRect(145, 45 * int(CurrentFrame), 47, 47));
			//getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right)) {
			p.dir = 0; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 2) (CurrentFrame -= 2);
			p.sprite.setTextureRect(IntRect(0, 45 * int(CurrentFrame), 47, 47));
			//getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up)) {
			p.dir = 3; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 2) (CurrentFrame -= 2);
			p.sprite.setTextureRect(IntRect(100, 45 * int(CurrentFrame), 47, 47));
			//getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down)) {
			p.dir = 2; p.speed = 0.1;
			CurrentFrame += 0.005*time;
			if (CurrentFrame > 2) (CurrentFrame -= 2);
			p.sprite.setTextureRect(IntRect(50, 45 * int(CurrentFrame), 47, 47));
			//getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
		}
		p.update(time);
		viewMap(time);
		window.setView(view);
		window.clear();

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
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}