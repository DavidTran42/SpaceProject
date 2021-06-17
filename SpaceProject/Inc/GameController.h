/*
 * GameController.h
 *
 *  Created on: 11 Jun 2021
 *      Author: davidtran
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <stdint.h>
#include <stdlib.h>
#include "spaceship.h"
#include <stdbool.h>

#endif /* GAMECONTROLLER_H_ */

typedef struct joystick {
	uint16_t left, right, up, down, center;
};

typedef struct clockTimer {
	volatile uint8_t hour, min, sec, sec100;
};

typedef struct asteroid {
	struct vector pos; // Due to the fact that u can see the asteroid outside boundaries
	uint8_t size, amountOfPoints;
	struct vector points[36];
	bool alive;
};

typedef struct bullet{
	struct vector pos, vel;
	bool alive;
};

void initGame(uint16_t borderWidth, uint16_t borderHeight, int gameMode);
void updateShipPos(char input, struct vector *shipptr, uint16_t borderWidth, uint16_t borderHeight);
void initializeShips(int gameMode, struct vector *shipptr, uint16_t borderWidth, uint16_t borderHeight);
struct joystick addJoystick();
void setUpTimer();
void enableTimer();
void disableTimer();
void rotateVector(struct vector *v, int32_t degree);
void makeAsteroid(struct asteroid *asteroidptr, uint16_t borderWidth, uint16_t borderHeight, uint8_t aListSize, uint8_t type, uint8_t r);
void makeBullet1(char input, struct bullet *bulletptr, struct vector ship,
		int bListSize);
void makeBullet2(struct joystick controls, struct bullet *bulletptr, struct vector ship,
		int bListSize);
void bosskey(char input);

void updateShip2Pos(struct vector *shipptr,struct joystick controls,
		uint16_t borderWidth, uint16_t borderHeight);
void init_pixels();
void update_pixels(struct vector *p);
void gravity(struct bullet *bulletptr, struct asteroid *asteroidptr,
		int bListSize, int aListSize, int i,int k);
void update_bullet(struct vector bullet);
bool checkCollisionWithAsteroid(struct vector ship, struct asteroid asteroid);
bool checkHit(struct bullet bullet, struct asteroid asteroid);
