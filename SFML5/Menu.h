#pragma once

void gameMenu(RenderWindow &window, engine *en) {
	bool isMenu = true;
	en->im->fontSprite.setPosition(SIZE_WINDOW.x / 2.0f, SIZE_WINDOW.y / 2.0f);
	en->im->cursorSprite.setPosition(270, 380);
	int numberCursor = 0;
	window.setFramerateLimit(LIMIT_FRAME_IN_MENU);
	while (isMenu) {
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			en->sound->soundStepCursor.play();
			if (numberCursor == 0)
				numberCursor = 2;
			else
				numberCursor--;
			
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down)) {
			en->sound->soundStepCursor.play();
			if (numberCursor == 2)
				numberCursor = 0;
			else
				numberCursor++;
		}
		if (numberCursor == 0) {
			en->im->cursorSprite.setPosition(COORDINATES_CURSOR_MENU);
		}
		else if (numberCursor == 1) {
			en->im->cursorSprite.setPosition(COORDINATES_CURSOR_MENU.x, COORDINATES_CURSOR_MENU.y + 100.0f);
		}
		else {
			en->im->cursorSprite.setPosition(COORDINATES_CURSOR_MENU.x, COORDINATES_CURSOR_MENU.y + 200.0f);
		}
		if (Keyboard::isKeyPressed(Keyboard::RShift) && numberCursor == 0) {
			if (!en->id->g_isStart) {
				isMenu = false;
				en->id->g_isMenu = false;
				en->id->g_isPause = false;
				en->id->g_isStart = true;
			}
			else {
				isMenu = false;
				en->id->g_isMenu = false;
				en->id->g_isPause = false;
				en->id->g_isRestart = true;
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::RShift) && numberCursor == 1) {
			if (en->id->g_isStart) {
				isMenu = false;
				en->id->g_isMenu = false;
				en->id->g_isPause = false;
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::RShift) && numberCursor == 2) {
			isMenu = false;
			window.close();
		}

		if (en->id->g_playerLose || en->id->g_playerWin) {
			en->id->g_isRestart = true;
		}
		window.clear();
		window.draw(en->im->fontSprite);
		window.draw(en->im->cursorSprite);
		window.draw(en->txt->game);
		window.draw(en->txt->continues);
		window.draw(en->txt->exit);
		window.display();
	}
	window.setFramerateLimit(LIMIT_FRAME);
}