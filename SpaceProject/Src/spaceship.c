#include <stdio.h>
#include "main.h"
#define ESC 0x1B
#include "Excellutex.h"
#include "charset.h"
#include "30010_io.h"
#include "ansi.h"
#include "spaceship.h"
#include "GameController.h"
/*
 * spacehip.c
 *
 *  Created on: 11. jun. 2021
 *      Author: Marius Mainz
 */

void print_ship1(struct vector position) {
	gotoxy(position.x-4, position.y - 2);
	fgcolor(6);
	printf("%c", 219);
	gotoxy(position.x-4, position.y - 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.x-4, position.y);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	gotoxy(position.x-4, position.y + 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.x-4, position.y + 2);
	printf("%c", 219);
	gotoxy(position.x - 5, position.y - 1);
	fgcolor(1);
	printf("%c", 219);
	gotoxy(position.x - 5, position.y + 1);
	printf("%c", 219);
	gotoxy(position.x - 6, position.y + 1);
	fgcolor(3);
	printf("%c", 219);
	gotoxy(position.x - 6, position.y - 1);
	printf("%c", 219);

}
void clear_ship1(struct vector position) {
	fgcolor(0);
	gotoxy(position.x-4, position.y - 2);
	printf("%c", 219);
	gotoxy(position.x-4, position.y - 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.x-4, position.y);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	gotoxy(position.x-4, position.y + 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.x-4, position.y + 2);
	printf("%c", 219);
	gotoxy(position.x - 5, position.y - 1);

	printf("%c", 219);
	gotoxy(position.x - 5, position.y + 1);
	printf("%c", 219);
	gotoxy(position.x - 6, position.y + 1);

	printf("%c", 219);
	gotoxy(position.x - 6, position.y - 1);
	printf("%c", 219);


}


void print_ship2(struct vector position) {
	printf("%c[?25l", ESC); //Hiding curser
	gotoxy(position.x-4, position.y - 2);
	fgcolor(2);
	printf(" ");
	gotoxy(position.x-4, position.y - 1);
	printf("   ");
	gotoxy(position.x-4, position.y);
	printf("     ");
	gotoxy(position.x-4, position.y + 1);
	printf("   ");
	gotoxy(position-4.x, position.y + 2);
	printf(" ");
	gotoxy(position.x - 5, position.y - 1);
	fgcolor(1);
	printf(" ");
	gotoxy(position.x - 5, position.y + 1);
	printf(" ");
	gotoxy(position.x - 6, position.y + 1);
	fgcolor(3);
	printf(" ");
	gotoxy(position.x - 6, position.y - 1);
	printf(" ");

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
