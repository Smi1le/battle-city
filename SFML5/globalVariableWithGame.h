#pragma once


struct globalVariable {
	int g_pointRessurection;
	float g_timer;
	float g_currentFrame; // todo Создать вектор для CurrentFrame и запихуть их туда все!
	float g_timerAppearenceEnemy;
	float g_curFrame;
	int g_count;
	int g_countBonus;
	int g_numberBonus;
	globalVariable() {
		g_pointRessurection = 0;
		g_timer = 0.0f;
		g_currentFrame = 0.0f; // todo Создать вектор для CurrentFrame и запихуть их туда все!
		g_timerAppearenceEnemy = 0.0f;
		g_curFrame = 0.0f;
		g_count = 0;
		g_countBonus = 0;
		g_numberBonus = 0;
	}
};