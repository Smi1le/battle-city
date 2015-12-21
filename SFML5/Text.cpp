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
}