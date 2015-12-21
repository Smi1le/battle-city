#pragma once

using namespace std;
using namespace sf;





/*---------------------STRING----------------------*/
static const string WINDOW_NAME = "Battle City";
static const string EASY_ENEMY_NAME = "EasyEnemy";
static const string DISPLAYS_ENEMY_NAME = "enemy";
static const string PLAYER_BULLES_NAME = "Bulles";
static const string EAGLE_NAME = "Deagle";
static const string BRICK_NAME = "brick";
static const string BONUS_NAME = "bonus";
static const string ENEMY_BULLES_NAME = "bullesEnemy";	
static const string PLAYER_NAME = "Player";
static const string PLAYER_OBJECT_NAME = "player";
static const string SOLID_NAME = "solid";
static const string DESTROY_SOLID_NAME = "destroyedSolid";
static const string RIGHT_DIR_NAME = "RIGHT";
static const string LEFT_DIR_NAME = "LEFT";
static const string UP_DIR_NAME = "UP";
static const string DOWN_DIR_NAME = "DOWN";
/*-------------------------------------------------*/
static const Vector2f SIZE_WINDOW = { 832, 700 };
static const Vector2i SIZE_ICON_ENEMIES = { 26, 26 };
static const Vector2i SIZE_BRICK = { 26, 25 };
static const Vector2i SIZE_PLAYER = { 37, 39 };
static const Vector2i SIZE_EAGLE = { 50, 50 };

static unsigned int TIME_RESPAWN_ENEMY = 4000;