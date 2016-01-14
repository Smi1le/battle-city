#pragma once

using namespace std;
using namespace sf;





/*---------------------STRING----------------------*/
static const string WINDOW_NAME = "Battle City PC Version";
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
static const Vector2i SIZE_WINDOW = { 832, 700 };
static const Vector2i SIZE_ICON_ENEMIES = { 26, 26 };
static const Vector2i SIZE_BRICK = { 26, 25 };
static const Vector2i SIZE_PLAYER = { 37, 39 };
static const Vector2i SIZE_EAGLE = { 50, 50 };
static const Vector2i SIZE_BULLES_ENEMY = { 8, 8 };
static const Vector2i SIZE_BULLES_PLAYER = { 8, 8 };
/*-------------------------------------------------*/
static const Vector2f COORDINATES_FIRST_STRING_MENU = { 350.0f, 370.0f };
static const Vector2f COORDINATES_SECOND_STRING_MENU = { 300.0f, 470.0f };
static const Vector2f COORDINATES_THIRD_STRING_MENU = { 350.0f, 570.0f };
static const Vector2f COORDINATES_CURSOR_MENU = { 220.0f, 380.0f };
/*-------------------------------------------------*/
static const unsigned TIME_RESPAWN_ENEMY = 4000;
static const unsigned FREQUENCY_SHOOT_BULLES = 100;
static const unsigned DELAY = 1;
static const unsigned NUMBER_ENEMYES = 5;
static const unsigned FREQUENCY_RESPAWN_BONUS = 2;
static const unsigned LIMIT_FRAME = 60;
static const unsigned LIMIT_FRAME_IN_MENU = 9;
/*-------------------------------------------------*/
static const float VOLUME = 75;
static const float REDUCTION = 0.005f;