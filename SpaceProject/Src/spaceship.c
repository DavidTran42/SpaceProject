#include "ansi.h"
#include "spaceship.h"
#include "main.h"
/*
 * spacehip.c
 *
 *  Created on: 11. jun. 2021
 *      Author: Marius Mainz
 */

void print_ship1(struct ship position) {
	position.pos.x = position.pos.x >> 14;
	position.pos.y = position.pos.y >> 14;

	gotoxy(position.pos.x - 4, position.pos.y - 2);
	fgcolor(6);
	printf("%c", 219);
	gotoxy(position.pos.x - 4, position.pos.y - 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.pos.x - 4, position.pos.y);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	gotoxy(position.pos.x - 4, position.pos.y + 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.pos.x - 4, position.pos.y + 2);
	printf("%c", 219);
	gotoxy(position.pos.x - 5, position.pos.y - 1);
	fgcolor(1);
	printf("%c", 219);
	gotoxy(position.pos.x - 5, position.pos.y + 1);
	printf("%c", 219);

}
void clear_ship1(struct ship position) {
	position.pos.x = position.pos.x >> 14;
	position.pos.y = position.pos.y >> 14;

	fgcolor(0);
	gotoxy(position.pos.x - 4, position.pos.y + 2);
	printf(" ");
	gotoxy(position.pos.x - 6, position.pos.y + 1);
	printf("     ");
	gotoxy(position.pos.x - 4, position.pos.y);
	printf("     ");
	gotoxy(position.pos.x - 6, position.pos.y - 1);
	printf("     ");
	gotoxy(position.pos.x - 4, position.pos.y - 2);
	printf(" ");

	if (position.pos.y < 66) {
		gotoxy(position.pos.x - 7, position.pos.y - 2);
		printf("   ");
		gotoxy(position.pos.x - 7, position.pos.y);
		printf("   ");
		gotoxy(position.pos.x - 7, position.pos.y + 2);
		printf("   ");
		gotoxy(position.pos.x - 7, position.pos.y + 1);
		printf(" ");
		gotoxy(position.pos.x - 7, position.pos.y - 1);
		printf(" ");
	}
	if (position.pos.y < 70 && position.pos.y > 65) {
		newbgcolor(0);
		gotoxy(position.pos.x - 10, position.pos.y - 2);
		printf("      ");
	}
	if (position.pos.y == 65) {
		newbgcolor(0);
		gotoxy(position.pos.x - 10, position.pos.y + 2);
		printf("      ");
	}

	resetbgcolor();
}

void print_ship2(struct ship position) {
	// printf("%c[?25l", ESC); //Hiding curser
	position.pos.x = position.pos.x >> 14;
	position.pos.y = position.pos.y >> 14;
	gotoxy(position.pos.x - 4, position.pos.y - 2);
	fgcolor(2);
	printf("%c", 219);
	gotoxy(position.pos.x - 4, position.pos.y - 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.pos.x - 4, position.pos.y);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	gotoxy(position.pos.x - 4, position.pos.y + 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.pos.x - 4, position.pos.y + 2);
	printf("%c", 219);
	gotoxy(position.pos.x - 5, position.pos.y - 1);
	fgcolor(1);
	printf("%c", 219);
	gotoxy(position.pos.x - 5, position.pos.y + 1);
	printf("%c", 219);

}

void print_flames(struct ship *position) {
	int32_t x = (position->pos.x >> 14);
	int32_t y = (position->pos.y >> 14);
	gotoxy(x - 6, y + 1);
	fgcolor(3);
	printf("%c", 219);
	gotoxy(x - 6, y - 1);
	printf("%c", 219);
}

/*

 void updateShipPos(vector_t *pos) {
 >>>>>>> branch 'master' of https://github.com/DavidTran42/SpaceProject
 >>>>>>> branch 'master' of https://github.com/DavidTran42/SpaceProject

 void shiptest() {
 struct ship ship1;
 struct ship ship2;
 ship1.position.x = 5, ship1.position.y = 20;
 ship2.position.x = 5, ship2.position.y = 35;
 print_ship1(&ship1.position);
 print_ship2(&ship2.position);

 }
 */
