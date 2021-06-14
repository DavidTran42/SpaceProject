#include <stdio.h>
#include "main.h"
#define ESC 0x1B
#include "Excellutex.h"
#include "charset.h"
#include "30010_io.h"
#include "ansi.h"
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



void print_ship1(vector_t *position) {
	printf("%c[?25l", ESC); //Hiding curser
	gotoxy(position->x, position->y - 2);
	color(1, 6);
	printf(" ");
	gotoxy(position->x, position->y - 1);
	printf("   ");
	gotoxy(position->x, position->y);
	printf("     ");
	gotoxy(position->x, position->y + 1);
	printf("   ");
	gotoxy(position->x, position->y + 2);
	printf(" ");
	gotoxy(position->x - 1, position->y - 1);
	bgcolor(1);
	printf(" ");
	gotoxy(position->x - 1, position->y + 1);
	printf(" ");
	gotoxy(position->x - 2, position->y + 1);
	color(1, 3);
	printf("=");
	gotoxy(position->x - 2, position->y - 1);
	printf("=");


	resetbgcolor();
}

void print_ship2(vector_t *position) {
	printf("%c[?25l", ESC); //Hiding curser
	gotoxy(position->x, position->y - 2);
	color(1, 2);
	printf(" ");
	gotoxy(position->x, position->y - 1);
	printf("   ");
	gotoxy(position->x, position->y);
	printf("     ");
	gotoxy(position->x, position->y + 1);
	printf("   ");
	gotoxy(position->x, position->y + 2);
	printf(" ");
	gotoxy(position->x - 1, position->y - 1);
	bgcolor(1);
	printf(" ");
	gotoxy(position->x - 1, position->y + 1);
	printf(" ");
	gotoxy(position->x - 2, position->y + 1);
	color(1, 3);
	printf("=");
	gotoxy(position->x - 2, position->y - 1);
	printf("=");


	resetbgcolor();
}

/*void updateShipPos(vector_t *pos) {
>>>>>>> branch 'master' of https://github.com/DavidTran42/SpaceProject

void shiptest() {
	struct ship ship1;
	struct ship ship2;
	ship1.position.x = 5, ship1.position.y = 20;
	ship2.position.x = 5, ship2.position.y = 35;
	print_ship1(&ship1.position);
	print_ship2(&ship2.position);

}
