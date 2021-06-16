/*
 * GameController.h
 *
 *  Created on: 11 Jun 2021
 *      Author: davidtran
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <stdint.h>
#include "spaceship.h"

#endif /* GAMECONTROLLER_H_ */

typedef struct joystick {
	uint16_t left, right, up, down, center;
};

typedef struct clockTimer {
	volatile uint8_t hour, min, sec, sec100;
};

typedef struct asteroid {
	struct vector pos;
	uint8_t size;
};
typedef struct bullet{
	struct vector pos, vel;
};

void initGame(uint16_t borderWidth, uint16_t borderHeight, int gameMode);
void updateShipPos(char input, struct vector *shipptr, struct joystick controls, uint16_t borderWidth, uint16_t borderHeight);
void initializeShips(int gameMode, struct vector *shipptr, uint16_t borderWidth, uint16_t borderHeight);
struct joystick addJoystick();
void setUpTimer();
void enableTimer();
void disableTimer();
void rotateVector(struct vector *v, int32_t degree);
void gravity(struct bullet *bulletptr, struct asteroid *asteroidptr);
void makeAsteroid(struct asteroid *asteroidptr, uint16_t borderWidth, uint16_t borderHeight, uint8_t aListSize, uint8_t type, uint8_t r);
void makeBullet(char input, struct bullet *bulletptr, struct vector *ship,
		int bListSize, struct joystick controls);
void bosskey(char input);

void updateShip2Pos(char input2, struct vector *shipptr,struct joystick controls,
		uint16_t borderWidth, uint16_t borderHeight);
void init_pixels();
void update_pixels(struct vector *p);
void gravity2();
void update_bullet(struct vector bullet);

