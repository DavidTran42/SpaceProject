#include "ansi.h"
#include "spaceship.h"
/*
 * spacehip.c
 *
 *  Created on: 11. jun. 2021
 *      Author: Marius Mainz
 */

void print_ship1(struct ship position) {
	gotoxy(position.pos.x-4, position.pos.y - 2);
	fgcolor(6);
	printf("%c", 219);
	gotoxy(position.pos.x-4, position.pos.y - 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.pos.x-4, position.pos.y);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	gotoxy(position.pos.x-4, position.pos.y + 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.pos.x-4, position.pos.y + 2);
	printf("%c", 219);
	gotoxy(position.pos.x - 5, position.pos.y - 1);
	fgcolor(1);
	printf("%c", 219);
	gotoxy(position.pos.x - 5, position.pos.y + 1);
	printf("%c", 219);
	gotoxy(position.pos.x - 6, position.pos.y + 1);
	fgcolor(3);
	printf("%c", 219);
	gotoxy(position.pos.x - 6, position.pos.y - 1);
	printf("%c", 219);

}
void clear_ship1(struct ship position) {
	fgcolor(0);
	gotoxy(position.pos.x-4, position.pos.y - 2);
	printf("%c", 219);
	gotoxy(position.pos.x-4, position.pos.y - 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.pos.x-4, position.pos.y);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	gotoxy(position.pos.x-4, position.pos.y + 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(position.pos.x-4, position.pos.y + 2);
	printf("%c", 219);
	gotoxy(position.pos.x - 5, position.pos.y - 1);

	printf("%c", 219);
	gotoxy(position.pos.x - 5, position.pos.y + 1);
	printf("%c", 219);
	gotoxy(position.pos.x - 6, position.pos.y + 1);

	printf("%c", 219);
	gotoxy(position.pos.x - 6, position.pos.y - 1);
	printf("%c", 219);


}


void print_ship2(struct ship position) {
	// printf("%c[?25l", ESC); //Hiding curser
	gotoxy(position.pos.x-4, position.pos.y - 2);
	fgcolor(2);
	printf(" ");
	gotoxy(position.pos.x-4, position.pos.y - 1);
	printf("   ");
	gotoxy(position.pos.x-4, position.pos.y);
	printf("     ");
	gotoxy(position.pos.x-4, position.pos.y + 1);
	printf("   ");
	gotoxy(position.pos.x-4, position.pos.y + 2);
	printf(" ");
	gotoxy(position.pos.x - 5, position.pos.y - 1);
	fgcolor(1);
	printf(" ");
	gotoxy(position.pos.x - 5, position.pos.y + 1);
	printf(" ");
	gotoxy(position.pos.x - 6, position.pos.y + 1);
	fgcolor(3);
	printf(" ");
	gotoxy(position.pos.x - 6, position.pos.y - 1);
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
