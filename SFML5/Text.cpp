#include "Text.h"

text::text(objectLevel *lvl) {
	font.loadFromFile("recources/Font/Monoid.ttf");//�������� ������ ������ ���� ������

	healph = Text("", font, 26);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	healph.setColor(Color::Black);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	healph.setStyle(sf::Text::Bold);
	healph.setPosition(lvl->health.rect.left, lvl->health.rect.top);

	youLose = Text(" YOU LOSE ", font, 50);
	youLose.setColor(sf::Color::Red);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	youLose.setStyle(sf::Text::Bold);
	youLose.setPosition(200.0f, SIZE_WINDOW.y / 2.0f);

	youWin = Text(" YOU WIN ", font, 50);
	youWin.setColor(Color::Blue);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	youWin.setStyle(sf::Text::Bold);
	youWin.setPosition(200.0f, SIZE_WINDOW.y / 2.0f);

	game = Text("GAME", font, 50);
	//game.setColor();
	game.setStyle(sf::Text::Bold);
	game.setPosition(COORDINATES_FIRST_STRING_MENU);

	continues = Text("CONTINUE", font, 50);
	continues.setStyle(Text::Bold);
	continues.setPosition(COORDINATES_SECOND_STRING_MENU);

	exit = Text("EXIT", font, 50);
	exit.setStyle(Text::Bold);
	exit.setPosition(COORDINATES_THIRD_STRING_MENU);

	escape = Text("Please press ESC to exit the menu", font, 25);
	escape.setStyle(Text::Bold);
	escape.setPosition(Vector2f(100.0f, SIZE_WINDOW.y / 2.0f + 100.0f));
}