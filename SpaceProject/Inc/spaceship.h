/*
 * spaceship.h
 *
 *  Created on: 14 Jun 2021
 *      Author: davidtran
 */

#ifndef SPACESHIP_H_
#define SPACESHIP_H_

#include <stdbool.h>

struct vector{
	int32_t x,y;
};

struct ship {
	struct vector pos, vel;
	uint8_t hearts, score, bulletAmount, bulletSpeed, fuel;
	int16_t rf_time, db_time, ds_time, acc;
	bool powered_up, rapidFire, doubleBullets, alive;
};

void print_ship1(struct ship position);
void clear_ship1(struct ship position);
void print_ship2(struct ship position);
void print_flames(struct ship *position);

#endif /* SPACESHIP_H_ */
