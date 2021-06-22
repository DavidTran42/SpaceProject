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

#endif /* GAMECONTROLLER_H_ */

struct joystick {
	uint16_t left, right, up, down, center;
} controls;

struct clockTimer {
	volatile uint8_t hour, min, sec, sec100;
};

struct asteroid {
	struct vector pos; // Due to the fact that u can see the asteroid outside boundaries
	uint8_t size, amountOfPoints;
	struct vector points[36];
	bool alive;
};

struct bullet{
	struct vector pos, vel, prev_pos;
	bool alive;
};

struct powers {
	struct vector pos;
	bool rapidFire, onField, doubleBullets, moreHearts;
};

struct gameSettings {
	uint8_t gameLevel, amountOfAsteroids, asteroidSpeed, gameLoop, asteroidCount;
};


void initGame(uint16_t borderWidth, uint16_t borderHeight, int gameMode);
void updateShipPos(char input, struct ship *shipptr, uint16_t borderWidth, uint16_t borderHeight);
void initializeShips(int gameMode, struct ship *shipptr, struct ship *shipptr2, uint16_t borderWidth, uint16_t borderHeight);
struct joystick addJoystick();
void setUpTimer();
void enableTimer();
void disableTimer();
void level_led(uint8_t gameLevel);
void rotateVector(struct vector *v, int32_t degree);
void makeAsteroid(struct asteroid *asteroidptr, uint16_t borderWidth, uint16_t borderHeight, uint8_t aListSize, uint8_t type, uint8_t r);
void makeBullet1(char input, struct bullet *bulletptr, struct ship ship,
		int bListSize);
void makeBullet2(struct joystick controls, struct bullet *bulletptr, struct ship ship,
		int bListSize);
void bosskey(char input);

void updateShip2Pos(struct ship *shipptr,struct joystick controls,
		uint16_t borderWidth, uint16_t borderHeight);
void init_pixels();
void lcd_update(uint8_t buffer[512], uint8_t line,uint16_t borderWidth, uint16_t borderHeight, int gameMode, struct gameSettings *p);
void update_pixels(struct vector *p);
void gravity(uint32_t m1, uint32_t m2, struct bullet *bulletptr, int64_t a, int64_t b);
void update_bullet(struct vector bullet);
bool checkCollisionWithAsteroid(struct ship ship, struct asteroid asteroid);
bool checkHit(struct bullet bullet, struct asteroid asteroid);
void updatingShip(struct ship *shipptr, uint16_t borderWidth, uint16_t borderHeight, int32_t acc);
void setRandomPowerUp(uint8_t buff, struct powers *powerups, uint16_t width, uint16_t height);
void blackHole(int32_t a, int32_t b);
void checkCollisionWithPowerUp(struct ship *shipptr, struct powers *powerptr);
void clearAsteroid(struct asteroid *asteroidptr);
void checkLevelGameUp(struct gameSettings *settings);
void checkCollisionWithBullet(struct bullet *bulletptr, struct asteroid *asteroid, struct ship *shipptr, char s_score[], uint8_t buffer[512]);
void checkActivePowerUp(struct ship *shipptr);
void drawBullets(struct ship ship, struct bullet *bulletptr, uint16_t borderWidth, uint16_t borderHeight, uint16_t s, char playerNumber);
void checkLives(struct ship *shipptr, struct ship *shipptr2, uint8_t buffer[512], char playerNumber, uint16_t borderWidth, uint16_t borderHeight, uint8_t gameMode, struct gameSettings *p);
void makeGameOverScreen(uint8_t buffer[512], uint16_t borderWidth, uint16_t borderHeight, uint8_t gameMode, struct gameSettings *p);
