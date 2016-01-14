#include "boolean.h"


globalBool::globalBool() {
	g_explEnemyBulles = false;
	g_explPlayerBulles = false;
	g_explEnemy = false;
	g_explPlayer = false;
	g_appearanceEnemies = false;
	g_isHit = false;
	g_isExplPlayer = false;
	g_isBonus = false;
	g_playerLose = false;
	g_playerWin = false;
	g_playerWinTime = false;
	g_isPause = true;
	g_isMenu = true;
	g_isRestart = false;
	g_isStart = false;
}