/*
 * spaceship.h
 *
 *  Created on: 14 Jun 2021
 *      Author: davidtran
 */

#ifndef SPACESHIP_H_
#define SPACESHIP_H_

struct vector{
	int16_t x,y;
};

void print_ship1(struct vector position);
void clear_ship1(struct vector position);
void print_ship2(struct vector position);

#endif /* SPACESHIP_H_ */
