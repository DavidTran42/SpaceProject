/*
 * spaceship.h
 *
 *  Created on: 14 Jun 2021
 *      Author: davidtran
 */

#ifndef SPACESHIP_H_
#define SPACESHIP_H_

typedef struct vector_t {
	int32_t x, y;
} vector_t;

typedef struct ship {
	vector_t position;
} ship;

void print_ship1(vector_t *position);
void print_ship2(vector_t *position);

#endif /* SPACESHIP_H_ */
