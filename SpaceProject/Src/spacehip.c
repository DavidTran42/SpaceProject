#include <stdio.h>
#include "main.h"
#define ESC 0x1B
#include "Excellutex.h"
#include "charset.h"
#include "30010_io.h"
/*
 * spacehip.c
 *
 *  Created on: 11. jun. 2021
 *      Author: Marius Mainz
 */
typedef struct vector_t {
	int32_t x, y;
} vector_t;

typedef struct ship {
	vector_t position;
} ship;

void print_ship(vector_t *position){
	gotoxy(position->x, position->y-2);
	color(0,14);
		printf(" \n");
		printf("    \n");
		printf("      \n");
		printf("    \n");
		printf(" \n");
	gotoxy(position->x-1, position->y-1);
		color(0,13);
		printf(" ");
		gotoxy(position->x-1, position->y+1);
		printf(" ");
}
