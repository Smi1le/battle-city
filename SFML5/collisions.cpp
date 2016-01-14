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
			if (p->speedBulles.x < 0.0f) {
				p->permittedMovementOptions[1] = 1;
				p->state = p->STAY;
				if (entity->speedBulles.x > 0.0f || entity->speedBulles.x < 0.0f) {
					p->isBlock = true;
					entity->speed = 0;
				}
			}
			else if  (p->speedBulles.x > 0.0f) {
				p->permittedMovementOptions[0] = 1;
				p->state = p->STAY;
				if (entity->speedBulles.x < 0.0f || entity->speedBulles.x > 0.0f) {
					p->isBlock = true;
					entity->speed = 0;
				}
			}
			else if (p->speedBulles.y < 0.0f) {
				p->permittedMovementOptions[2] = 1;
				p->state = p->STAY;
				if (entity->speedBulles.y > 0.0f || entity->speedBulles.y < 0.0f) {
					p->isBlock = true;
					entity->speed = 0;
				}
			}
			else if (p->speedBulles.y > 0.0f) {
				p->permittedMovementOptions[3] = 1;
				p->state = p->STAY;
				if (entity->speedBulles.y < 0.0f || entity->speedBulles.y > 0.0f) {
					p->isBlock = true;
					entity->speed = 0;
				}
			}
		}
		else {
			if (entity->name == EASY_ENEMY_NAME)
				entity->speed = SPEED_EASY_ENEMY;
			if (entity->name == SPEED_ENEMY_NAME)
				entity->speed = SPEED_SPEED_ENEMY;
		}
	}
}