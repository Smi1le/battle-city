#include "collisions.h"



void checkCollisionWithMap(Player *p, Entity* brick, bool &changeCourse) {
	if (!changeCourse) {
		if (p->diraction.y > 0) {
			p->coordinates.y = brick->sprite->getPosition().y - p->h;
			p->diraction.y = 0;
		}
		else if (p->diraction.y < 0) {
			p->coordinates.y = brick->sprite->getPosition().y + brick->h;
			p->diraction.y = 0;
		}
		else if (p->diraction.x > 0) {
			p->coordinates.x = brick->sprite->getPosition().x - p->w;
		}
		else if (p->diraction.x < 0) {
			p->coordinates.x = brick->sprite->getPosition().x + brick->w;
		}
		changeCourse = true;
	}
	else {
		if (brick->lastCoordinates.y > 0) {
			p->coordinates.y = brick->sprite->getPosition().y - p->h;
		}
		else if (brick->lastCoordinates.y < 0) {
			p->coordinates.y = brick->sprite->getPosition().y + brick->h;
		}
		else if (brick->lastCoordinates.x > 0) {
			p->coordinates.x = brick->sprite->getPosition().x - p->w;
		}
		else if (brick->lastCoordinates.x < 0) {
			p->coordinates.x = brick->sprite->getPosition().x + brick->w;
		}
	}
}

void collisionWithBricksForPlayer(Player *p, list<Entity*> &listBrick, bool &changeCourse) {
	for (Entity* brick : listBrick) {
		if (p->getRect().intersects(brick->getRect())) {
			checkCollisionWithMap(p, brick, changeCourse);
			p->lastCoordinates = { p->diraction.x, p->diraction.y };

		}
	}
}

void collisionWithEnemyForPlayer(Player *p, list<Entity*> &entities, bool &changeCourse) {
	for (Entity* entity : entities) {
		if (entity->getRect().intersects(p->getRect()) && entity->alive) {
			checkCollisionWithMap(p, entity, changeCourse);
			entity->speed = 0;
		}
		else {
			entity->speed = 0.1f;
		}
	}
}