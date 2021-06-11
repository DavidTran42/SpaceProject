/*
 * GameController.h
 *
 *  Created on: 11 Jun 2021
 *      Author: davidtran
 */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <stdint.h>

#endif /* GAMECONTROLLER_H_ */

typedef struct vector {
	uint16_t x, y;
};

typedef struct joystick {
	uint16_t left, right, up, down, center;
};

typedef struct clockTimer {
	volatile int8_t hour, min, sec, sec100;
};

void initGame(uint16_t borderWidth, uint16_t borderHeight, int gameMode);
void updateShipPos(char input, struct vector *shipptr, struct joystick controls, uint16_t borderWidth, uint16_t borderHeight);
void initializeShips(int gameMode, struct vector *shipptr, uint16_t borderWidth, uint16_t borderHeight);
struct joystick addJoystick();
void setUpTimer();
void enableTimer();
void disableTimer();
void makeAsteroid(uint8_t size, uint16_t borderWidth, uint16_t borderHeight);
