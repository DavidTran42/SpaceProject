/*
 * background.c
 *
 *  Created on: 11. jun. 2021
 *      Author: Marius Mainz
 */

#include <stdio.h>
#define ESC 0x1B
#include "charset.h"
#include "ansi.h"
#include "GameController.h"

void newbgcolor(uint8_t background) {
	printf("\033[48;5;%dm", background);
}
void newfgcolor(uint8_t forground) {
	printf("\033[38;5;%dm", forground);
}

void background() {

	//printing stars

	printf("\033[38;5;15m"); //fg color white
	for (int i = 1; i < 10; i++) {
		gotoxy(i * 24, 2);
		printf("*         *");
	}
	for (int i = 0; i <= 14; i++) {
		gotoxy(i * 18 + 5, 4);
		printf("*          *");

	}
	gotoxy(15, 7);
	printf(
			"*                                                                 *                                    * ");
	for (int i = 1; i < 6; i++) {
		gotoxy(i * 44, 8);
		printf("*        *");
	}
	for (int i = 0; i <= 9; i++) {
		gotoxy(i * 24 + 2, 10);
		printf("*       *");
	}
	for (int i = 1; i < 14; i++) {
		gotoxy(i * 18 + 5, 12);
		printf("*          *");

	}
	gotoxy(8, 13);
	printf(
			"*                                *                        *                                *");
	for (int i = 1; i < 6; i++) {
		gotoxy(i * 44, 15);
		printf("*            *");
	}

	for (int i = 0; i <= 10; i++) {
		gotoxy(i * 24 + 4, 18);
		printf("*         *");
	}
	for (int i = 1; i < 14; i++) {
		gotoxy(i * 18 + 5, 19);
		printf("*        *");

	}

	gotoxy(260, 30);
	printf("*");

	gotoxy(262, 26);
	printf("*");

	gotoxy(250, 37);
	printf("*");

	gotoxy(10, 35);
	printf("*");

	gotoxy(8, 26);
	printf("*");

	gotoxy(11, 40);
	printf("*");

	gotoxy(5, 20);
	printf(
			"*                                *                                    * ");
	for (int i = 1; i < 6; i++) {
		gotoxy(i * 44, 21);
		printf("*         *");
	}
	for (int i = 1; i < 9; i++) {
		gotoxy(i * 24, 23);
		printf("*        *");
	}
	for (int i = 1; i < 14; i++) {
		gotoxy(i * 18 + 5, 25);
		printf("*          *");

	}
	gotoxy(3, 26);
	printf(
			"*                                *                *                                *");
	for (int i = 1; i < 6; i++) {
		gotoxy(i * 44, 27);
		printf("*        *");
	}
	gotoxy(8, 29);
	printf(
			"*                                *                        *                                *");
	for (int i = 1; i < 6; i++) {
		gotoxy(i * 44, 31);
		printf("*                   *");
	}

	for (int i = 1; i < 10; i++) {
		gotoxy(i * 24, 33);
		printf("*      *");
	}
	for (int i = 1; i < 14; i++) {
		gotoxy(i * 18 + 5, 36);
		printf("*         *");
	}
	for (int i = 1; i < 9; i++) {
		gotoxy(i * 24, 38);
		printf("*        *");
	}
	for (int i = 1; i < 14; i++) {
		gotoxy(i * 18 + 5, 39);
		printf("*          *");

	}
	gotoxy(3, 41);
	printf(
			"*                                *                *                                *");
	for (int i = 1; i < 6; i++) {
		gotoxy(i * 44, 42);
		printf("*        *");
	}
	gotoxy(10, 43);
	printf(
			"*                                *                        *                                *");

	for (int i = 0; i <= 6; i++) {
		gotoxy(i * 44, 47);
		printf("*         *");
	}
	for (int i = 0; i <= 10; i++) {
		gotoxy(i * 24 + 2, 48);
		printf("*        *");
	}
	for (int i = 1; i < 14; i++) {
		gotoxy(i * 18 + 9, 50);
		printf("*          *");
	}

	//next

	for (int i = 0; i <= 9; i++) {
		gotoxy(i * 24 + 2, 53);
		printf("*       *");
	}
	for (int i = 1; i < 14; i++) {
		gotoxy(i * 18 + 5, 54);
		printf("*          *");

	}
	gotoxy(3, 55);
	printf(
			"*                                *                        *                                *");
	for (int i = 1; i < 6; i++) {
		gotoxy(i * 44, 57);
		printf("*            *");
	}

	for (int i = 0; i <= 10; i++) {
		gotoxy(i * 24 + 3, 59);
		printf("*         *");
	}
	for (int i = 1; i < 14; i++) {
		gotoxy(i * 18 + 6, 60);
		printf("*        *");
	}

	//Karlsvognen
	gotoxy(40, 30);
	printf("                                                       *");
	gotoxy(40, 31);
	printf("                                         *            ***");
	gotoxy(40, 32);
	printf(
			"                                        ***            *           *");
	gotoxy(40, 33);
	printf(
			"                                         *                        ***");
	gotoxy(40, 34);
	printf(
			"                                                                   *");
	gotoxy(40, 35);
	printf("    *                         *");
	gotoxy(40, 36);
	printf("   ***                       ***");
	gotoxy(40, 37);
	printf("    *                         *");
	gotoxy(40, 42);
	printf("        *                *");
	gotoxy(40, 43);
	printf("       ***              ***");

	gotoxy(40, 44);
	printf("        *                *");

	//atmosphere
	gotoxy(0, 59);
	newbgcolor(17);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 60);
	newbgcolor(18);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 61);
	newbgcolor(19);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 62);
	newbgcolor(20);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 63);
	newbgcolor(21);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}

	//Planet
	gotoxy(0, 64);
	newbgcolor(15);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 65);
	newbgcolor(15);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 66);
	newbgcolor(195);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 67);
	newbgcolor(159);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 68);
	newbgcolor(87);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 69);
	newbgcolor(51);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 70);
	newbgcolor(50);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 71);
	newbgcolor(45);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 72);
	newbgcolor(39);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 73);
	newbgcolor(33);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 74);
	newbgcolor(27);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}

	for (int i = 0; i < 5; i++) {
		gotoxy(i * 55, 66);
		newbgcolor(15);
		printf("            ");
	}
	for (int i = 1; i < 8; i++) {
		gotoxy(i * 33, 67);
		printf("          ");
	}
	for (int i = 1; i < 13; i++) {
		gotoxy(i * 20, 68);
		printf("      ");
	}
	for (int i = 0; i < 6; i++) {
		gotoxy(i * 44, 69);
		printf("      ");
	}
	for (int i = 1; i < 8; i++) {
		gotoxy(i * 33, 70);
		printf("       ");
	}
	for (int i = 0; i < 5; i++) {
		gotoxy(i * 55, 71);
		newbgcolor(15);
		printf("            ");
	}
	for (int i = 1; i < 8; i++) {
		gotoxy(i * 33 + 15, 72);
		printf("         ");
	}
	for (int i = 1; i < 13; i++) {
		gotoxy(i * 20 - 10, 73);
		printf("           ");
	}
	gotoxy(255, 70);
	printf("        ");
	for (int i = 1; i < 10; i++) {
		newbgcolor(20);
		gotoxy(i * 27 - 10, 73);
		printf("     ");
	}

	for (int i = 1; i < 7; i++) {
		newbgcolor(26);
		gotoxy(i * 39 - 10, 73);
		printf("          ");
	}
	for (int i = 1; i < 9; i++) {
		newbgcolor(105);
		gotoxy(i * 30 - 10, 72);
		printf("          ");
	}

	//litle planet

	newbgcolor(36);
	gotoxy(201, 20);
	printf("       ");
	newbgcolor(72);
	gotoxy(199, 21);
	printf("           ");
	newbgcolor(108);
	gotoxy(198, 22);
	printf("             ");
	newbgcolor(144);
	gotoxy(197, 23);
	printf("               ");
	newbgcolor(180);
	gotoxy(197, 24);
	printf("               ");
	newbgcolor(216);
	gotoxy(197, 25);
	printf("               ");
	newbgcolor(216);
	gotoxy(197, 26);
	printf("               ");
	newbgcolor(215);
	gotoxy(198, 27);
	printf("             ");
	newbgcolor(214);
	gotoxy(199, 28);
	printf("           ");
	newbgcolor(208);
	gotoxy(201, 29);
	printf("       ");

	newbgcolor(11);
	gotoxy(195, 23);
	printf("  ");
	gotoxy(212, 23);
	printf("  ");
	gotoxy(194, 24);
	printf(" ");
	gotoxy(214, 24);
	printf(" ");
	gotoxy(195, 25);
	printf("   ");
	gotoxy(211, 25);
	printf("   ");
	gotoxy(198, 26);
	printf("             ");

	newbgcolor(64);
	gotoxy(201, 21);
	printf(" ");
	gotoxy(199, 23);
	printf(" ");
	gotoxy(201, 23);
	printf(" ");
	gotoxy(202, 25);
	printf(" ");
	gotoxy(204, 25);
	printf(" ");

	printf("%c[?25l", ESC); //hiding curser

	resetbgcolor();
}
void small_asteroid(struct asteroid *asteroid1) {
	asteroid1->pos.x = asteroid1->pos.x >> 14;
	asteroid1->pos.y = asteroid1->pos.y >> 14;
	newfgcolor(252);
	gotoxy(asteroid1->pos.x - 1, asteroid1->pos.y - 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(asteroid1->pos.x - 2, asteroid1->pos.y);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	gotoxy(asteroid1->pos.x - 1, asteroid1->pos.y + 1);
	printf("%c%c%c", 219, 219, 219);

	newfgcolor(240);
	gotoxy(asteroid1->pos.x - 1, asteroid1->pos.y);
	printf("%c", 219);
	gotoxy(asteroid1->pos.x, asteroid1->pos.y - 1);
	printf("%c", 219);

	newfgcolor(247);
	gotoxy(asteroid1->pos.x + 1, asteroid1->pos.y - 1);
	printf("%c", 219);
	gotoxy(asteroid1->pos.x - 2, asteroid1->pos.y);
	printf("%c", 219);
	gotoxy(asteroid1->pos.x, asteroid1->pos.y + 1);
	printf("%c", 219);
	asteroid1->pos.x = asteroid1->pos.x << 14;
	asteroid1->pos.y = asteroid1->pos.y << 14;
}
void clear_small_asteroid(struct asteroid *asteroid1) {
	asteroid1->pos.x = asteroid1->pos.x >> 14;
	asteroid1->pos.y = asteroid1->pos.y >> 14;
	newfgcolor(0);
	gotoxy(asteroid1->pos.x - 1, asteroid1->pos.y - 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(asteroid1->pos.x - 2, asteroid1->pos.y);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	gotoxy(asteroid1->pos.x - 1, asteroid1->pos.y + 1);
	printf("%c%c%c", 219, 219, 219);
	asteroid1->pos.x = asteroid1->pos.x << 14;
	asteroid1->pos.y = asteroid1->pos.y << 14;
	resetbgcolor();
}

void medium_asteroid(struct asteroid *asteroid2) {
	asteroid2->pos.x = asteroid2->pos.x >> 14;
	asteroid2->pos.y = asteroid2->pos.y >> 14;
	newfgcolor(240);
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y - 3);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);

	newfgcolor(247);
	gotoxy(asteroid2->pos.x - 3, asteroid2->pos.y - 2);
	printf("%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219);
	newfgcolor(252);
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y - 1);
	printf("%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y);
	printf("%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y + 1);
	printf("%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);

	gotoxy(asteroid2->pos.x - 3, asteroid2->pos.y + 2);
	printf("%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219);
	newfgcolor(247);
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y + 3);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	newfgcolor(240);
	gotoxy(asteroid2->pos.x - 3, asteroid2->pos.y - 2);
	printf("%c%c", 219, 219);
	newfgcolor(247);
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y - 1);
	printf("%c", 219);

	newfgcolor(8);
	gotoxy(asteroid2->pos.x + 1, asteroid2->pos.y);
	printf("%c", 219);
	gotoxy(asteroid2->pos.x + 2, asteroid2->pos.y + 1);
	printf("%c", 219);
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y + 2);
	printf("%c%c", 219, 219);

	newfgcolor(240);
	gotoxy(asteroid2->pos.x + 1, asteroid2->pos.y);
	printf("%c", 219);
	gotoxy(asteroid2->pos.x + 2, asteroid2->pos.y + 1);
	printf("%c", 219);
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y + 2);
	printf("%c%c", 219, 219);

	newfgcolor(247);
	gotoxy(asteroid2->pos.x + 3, asteroid2->pos.y - 2);
	printf("%c", 219);
	newfgcolor(250);
	gotoxy(asteroid2->pos.x + 3, asteroid2->pos.y - 1);
	printf("%c%c", 219, 219);
	asteroid2->pos.x = asteroid2->pos.x << 14;
	asteroid2->pos.y = asteroid2->pos.y << 14;
}
void clear_medium_asteroid(struct asteroid *asteroid2) {
	asteroid2->pos.x = asteroid2->pos.x >> 14;
	asteroid2->pos.y = asteroid2->pos.y >> 14;
	newfgcolor(0);
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y - 3);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	gotoxy(asteroid2->pos.x - 3, asteroid2->pos.y - 2);
	printf("%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y - 1);
	printf("%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y);
	printf("%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y + 1);
	printf("%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid2->pos.x - 3, asteroid2->pos.y + 2);
	printf("%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y + 3);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	asteroid2->pos.x = asteroid2->pos.x << 14;
	asteroid2->pos.y = asteroid2->pos.y << 14;
}

void large_asteroid(struct asteroid *asteroid3) {
	asteroid3->pos.x = asteroid3->pos.x >> 14;
	asteroid3->pos.y = asteroid3->pos.y >> 14;
	newfgcolor(240);
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y - 5);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	newfgcolor(247);
	gotoxy(asteroid3->pos.x - 5, asteroid3->pos.y - 4);
	printf("%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219);
	newfgcolor(252);
	gotoxy(asteroid3->pos.x - 6, asteroid3->pos.y - 3);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219);
	newfgcolor(250);
	gotoxy(asteroid3->pos.x - 7, asteroid3->pos.y - 2);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219);
	newfgcolor(252);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y - 1);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y + 1);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 7, asteroid3->pos.y + 2);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219);
	newfgcolor(250);
	gotoxy(asteroid3->pos.x - 6, asteroid3->pos.y + 3);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 5, asteroid3->pos.y + 4);
	printf("%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219);
	newfgcolor(247);
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y + 5);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);

	newfgcolor(8);
	gotoxy(asteroid3->pos.x - 4, asteroid3->pos.y + 1);
	printf("%c%c", 219, 219);
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y + 2);
	printf("%c", 219);
	gotoxy(asteroid3->pos.x, asteroid3->pos.y + 4);
	printf("%c%c", 219, 219);
	gotoxy(asteroid3->pos.x + 2, asteroid3->pos.y + 4);
	printf("%c", 219);

	newfgcolor(248);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y - 1);
	printf("%c%c%c", 219, 219, 219);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y);
	printf("%c", 219);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y + 1);
	printf("%c", 219);

	gotoxy(asteroid3->pos.x - 1, asteroid3->pos.y - 1);
	printf("%c%c", 219, 219);
	gotoxy(asteroid3->pos.x + 1, asteroid3->pos.y);
	printf("%c", 219);
	gotoxy(asteroid3->pos.x + 1, asteroid3->pos.y + 1);
	printf("%c", 219);

	newfgcolor(250);
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y - 3);
	printf("%c%c", 219, 219);
	gotoxy(asteroid3->pos.x + 1, asteroid3->pos.y - 4);
	printf("%c%c%c", 219, 219, 219);
	asteroid3->pos.x = asteroid3->pos.x << 14;
	asteroid3->pos.y = asteroid3->pos.y << 14;
}

void clear_large_asteroid(struct asteroid *asteroid3) {
	asteroid3->pos.x = asteroid3->pos.x >> 14;
	asteroid3->pos.y = asteroid3->pos.y >> 14;
	newfgcolor(0);
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y - 5);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);

	gotoxy(asteroid3->pos.x - 5, asteroid3->pos.y - 4);
	printf("%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219);
	gotoxy(asteroid3->pos.x - 6, asteroid3->pos.y - 3);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 7, asteroid3->pos.y - 2);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y - 1);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y + 1);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 7, asteroid3->pos.y + 2);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219, 219, 219, 219);

	gotoxy(asteroid3->pos.x - 6, asteroid3->pos.y + 3);
	printf("%c%c%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219, 219, 219);
	gotoxy(asteroid3->pos.x - 5, asteroid3->pos.y + 4);
	printf("%c%c%c%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219,
			219, 219, 219);
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y + 5);
	printf("%c%c%c%c%c", 219, 219, 219, 219, 219);
	asteroid3->pos.x = asteroid3->pos.x << 14;
	asteroid3->pos.y = asteroid3->pos.y << 14;
}


void stars_only() {
	resetbgcolor();
	fgcolor(15);
	for (int i = 1; i < 5; i++) {
		gotoxy(54 * i, 2);
		printf("*");
	}
	for (int i = 1; i < 8; i++) {
		gotoxy(33 * i, 10);
		printf("*");
	}
	for (int i = 1; i < 4; i++) {
		gotoxy(67 * i, 15);
		printf("*");
	}
	for (int i = 1; i < 9; i++) {
		gotoxy(30 * i, 21);
		printf("*");
	}
	for (int i = 1; i < 6; i++) {
		gotoxy(45 * i, 29);
		printf("*");
	}
	for (int i = 1; i < 12; i++) {
		gotoxy(22 * i, 37);
		printf("*");
	}
	for (int i = 1; i < 5; i++) {
		gotoxy(54 * i, 44);
		printf("*");
	}
	for (int i = 1; i < 7; i++) {
		gotoxy(38 * i, 52);
		printf("*");
	}
	for (int i = 1; i < 8; i++) {
		gotoxy(33 * i, 60);
		printf("*");
	}

	blackHole(125,35);

}

void game_background() {
//atmosphere
	gotoxy(0, 68);
	newbgcolor(17);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 69);
	newbgcolor(18);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 70);
	newbgcolor(19);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 71);
	newbgcolor(20);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 72);
	newbgcolor(21);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}

//Planet
	gotoxy(0, 73);
	newbgcolor(15);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 74);
	newbgcolor(195);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}
	gotoxy(0, 75);
	newbgcolor(159);
	for (int i = 0; i < 269; i++) {
		printf(" ");
	}

	resetbgcolor();
	stars_only();

}

void update_pixels_ship(struct ship *p) {

	int32_t x = (p->pos.x >> 14);
	int32_t y =  (p->pos.y >> 14);
	if (y  == 65) {
		gotoxy(x  - 5, y  + 3);
		newbgcolor(17);
		printf("   ");

	} else if (y == 66) {
		gotoxy(x - 5, y + 3);
		newbgcolor(18);
		printf("   ");
		newbgcolor(17);
		gotoxy(x  - 7, y  + 2);
		printf("   ");
		gotoxy(x  - 3, y  + 2);
		printf("   ");

	} else if (y  == 67) {
		gotoxy(x  - 5, y + 3);
		newbgcolor(19);
		printf("   ");
		newbgcolor(18);
		gotoxy(x - 7, y  + 2);
		printf("   ");
		gotoxy(x  - 3, y + 2);
		printf("   ");
		newbgcolor(17);
		gotoxy(x  - 8, y  + 1);
		printf("  ");
		gotoxy(x - 1, y  + 1);
		printf("   ");

	} else if (y == 68) {
		gotoxy(x - 5, y  + 3);
		newbgcolor(20);
		printf("   ");
		newbgcolor(19);
		gotoxy(x  - 7, y  + 2);
		printf("   ");
		gotoxy(x  - 3, y  + 2);
		printf("   ");
		newbgcolor(18);
		gotoxy(x  - 8, y  + 1);
		printf("  ");
		gotoxy(x  - 1, y  + 1);
		printf("   ");
		newbgcolor(17);

		gotoxy(x - 8, y );
		printf("    ");
		gotoxy(x + 1, y);
		printf("  ");

	} else if (y == 69) {
		gotoxy(x - 5, y  + 3);
		newbgcolor(21);
		printf("   ");
		newbgcolor(20);
		gotoxy(x - 7, y  + 2);
		printf("   ");
		gotoxy(x - 3, y + 2);
		printf("   ");
		newbgcolor(19);
		gotoxy(x - 8, y  + 1);
		printf("  ");
		gotoxy(x  - 1, y + 1);
		printf("   ");
		newbgcolor(18);

		gotoxy(x - 8, y );
		printf("    ");
		gotoxy(x + 1, y);
		printf("  ");
		newbgcolor(17);
		gotoxy(x  - 8, y  - 1);
		printf("  ");
		gotoxy(x  - 1, y  - 1);
		printf("   ");

	} else if (y  == 70) {
		gotoxy(x - 5, y + 3);
		newbgcolor(15);
		printf("   ");
		newbgcolor(21);
		gotoxy(x - 7, y + 2);
		printf("   ");
		gotoxy(x - 3, y + 2);
		printf("   ");
		newbgcolor(20);
		gotoxy(x - 8, y + 1);
		printf("  ");
		gotoxy(x - 1, y + 1);
		printf("   ");
		newbgcolor(19);

		gotoxy(x - 8, y);
		printf("    ");
		gotoxy(x + 1, y);
		printf("  ");
		newbgcolor(18);
		gotoxy(x - 8, y - 1);
		printf("  ");
		gotoxy(x - 1, y - 1);
		printf("   ");
		newbgcolor(17);
		gotoxy(x - 7, y - 2);
		printf("   ");
		gotoxy(x - 3, y - 2);
		printf("   ");

	} else if (y == 71) {
		gotoxy(x - 5, y + 3);
		newbgcolor(195);
		printf("   ");
		newbgcolor(15);
		gotoxy(x - 7, y + 2);
		printf("   ");
		gotoxy(x - 3, y + 2);
		printf("   ");
		newbgcolor(21);
		gotoxy(x - 8, y + 1);
		printf("  ");
		gotoxy(x - 1, y + 1);
		printf("   ");
		newbgcolor(20);

		gotoxy(x - 8, y);
		printf("    ");
		gotoxy(x + 1, y);
		printf("  ");
		newbgcolor(19);
		gotoxy(x - 8, y - 1);
		printf("  ");
		gotoxy(x - 1, y - 1);
		printf("   ");
		newbgcolor(18);
		gotoxy(x - 7, y - 2);
		printf("   ");
		gotoxy(x - 3, y - 2);
		printf("   ");
		newbgcolor(17);
		gotoxy(x - 5, y - 3);
		printf("   ");

	} else if (y == 72) {
		gotoxy(x - 5, y + 3);
		newbgcolor(159);
		printf("   ");
		newbgcolor(195);
		gotoxy(x - 7, y + 2);
		printf("   ");
		gotoxy(x - 3, y + 2);
		printf("   ");
		newbgcolor(15);
		gotoxy(x - 8, y + 1);
		printf("  ");
		gotoxy(x - 1, y + 1);
		printf("   ");
		newbgcolor(21);
		gotoxy(x - 8, y);
		printf("    ");
		gotoxy(x + 1, y);
		printf("  ");
		newbgcolor(20);
		gotoxy(x - 8, y - 1);
		printf("  ");
		gotoxy(x - 1, y - 1);
		printf("   ");
		newbgcolor(19);
		gotoxy(x - 7, y - 2);
		printf("   ");
		gotoxy(x - 3, y - 2);
		printf("   ");
		newbgcolor(18);
		gotoxy(x - 5, y - 3);
		printf("   ");

	} else if (y == 73) {
		gotoxy(x - 5, y + 3);
		newbgcolor(159);
		gotoxy(x - 7, y + 2);
		printf("   ");
		gotoxy(x - 3, y + 2);
		printf("   ");
		newbgcolor(195);
		gotoxy(x - 8, y + 1);
		printf("  ");
		gotoxy(x - 1, y + 1);
		printf("   ");
		newbgcolor(15);
		gotoxy(x - 8, y);
		printf("    ");
		gotoxy(x + 1, y);
		printf("  ");
		newbgcolor(21);
		gotoxy(x - 8, y - 1);
		printf("  ");
		gotoxy(x - 1, y - 1);
		printf("   ");
		newbgcolor(20);
		gotoxy(x - 8, y - 2);
		printf("    ");
		gotoxy(x - 3, y - 2);
		printf("   ");
		newbgcolor(19);
		gotoxy(x - 5, y - 3);
		printf("   ");
	}
	resetbgcolor();
}

void update_small_background(struct vector *p) {
	p->x = p->x >> 14;
	p->y = p->y >> 14;
	if (p->y == 67) {
		newbgcolor(17);
		gotoxy(p->x + 2, p->y + 1);
		printf(" ");
	} else if (p->y == 68) {
		newbgcolor(18);
		gotoxy(p->x + 2, p->y + 1);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 3, p->y);
		printf(" ");
	} else if (p->y == 69) {
		newbgcolor(19);
		gotoxy(p->x + 2, p->y + 1);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 3, p->y);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 2, p->y - 1);
		printf(" ");
	} else if (p->y == 70) {
		newbgcolor(20);
		gotoxy(p->x + 2, p->y + 1);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 3, p->y);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 2, p->y - 1);
		printf(" ");
	} else if (p->y == 71) {
		newbgcolor(21);
		gotoxy(p->x + 2, p->y + 1);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 3, p->y);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 2, p->y - 1);
		printf(" ");
	} else if (p->y == 72) {
		newbgcolor(15);
		gotoxy(p->x + 2, p->y + 1);
		printf(" ");
		newbgcolor(21);
		gotoxy(p->x + 3, p->y);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 2, p->y - 1);
		printf(" ");
	} else if (p->y == 73) {
		newbgcolor(195);
		gotoxy(p->x + 2, p->y + 1);
		printf(" ");
		newbgcolor(15);
		gotoxy(p->x + 3, p->y);
		printf(" ");
		newbgcolor(21);
		gotoxy(p->x + 2, p->y - 1);
		printf(" ");
	} else if (p->y == 74) {
		newbgcolor(159);
		gotoxy(p->x + 2, p->y + 1);
		printf(" ");
		newbgcolor(195);
		gotoxy(p->x + 3, p->y);
		printf(" ");
		newbgcolor(15);
		gotoxy(p->x + 2, p->y - 1);
		printf(" ");
	}
	resetbgcolor();
	p->x = p->x << 14;
	p->y = p->y << 14;
}

void update_medium_background(struct vector *p) {
	p->x = p->x >> 14;
	p->y = p->y >> 14;
	if (p->y == 65) {
		newbgcolor(17);
		gotoxy(p->x + 3, p->y + 3);
		printf(" ");
	} else if (p->y == 66) {
		newbgcolor(18);
		gotoxy(p->x + 3, p->y + 3);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 4, p->y + 2);
		printf(" ");
	} else if (p->y == 67) {
		newbgcolor(19);
		gotoxy(p->x + 3, p->y + 3);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 4, p->y + 2);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 5, p->y + 1);
		printf(" ");
	} else if (p->y == 68) {
		newbgcolor(20);
		gotoxy(p->x + 3, p->y + 3);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 4, p->y + 2);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 5, p->y + 1);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 5, p->y);
		printf(" ");
	} else if (p->y == 69) {
		newbgcolor(21);
		gotoxy(p->x + 3, p->y + 3);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 4, p->y + 2);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 5, p->y + 1);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 5, p->y);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 5, p->y - 1);
		printf(" ");
	} else if (p->y == 70) {
		newbgcolor(15);
		gotoxy(p->x + 3, p->y + 3);
		printf(" ");
		newbgcolor(21);
		gotoxy(p->x + 4, p->y + 2);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 5, p->y + 1);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 5, p->y);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 5, p->y - 1);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 4, p->y - 2);
		printf(" ");
	} else if (p->y == 71) {
		newbgcolor(195);
		gotoxy(p->x + 3, p->y + 3);
		printf(" ");
		newbgcolor(15);
		gotoxy(p->x + 4, p->y + 2);
		printf(" ");
		newbgcolor(21);
		gotoxy(p->x + 5, p->y + 1);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 5, p->y);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 5, p->y - 1);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 4, p->y - 2);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 3, p->y - 3);
		printf(" ");
	} else if (p->y == 72) {
		newbgcolor(159);
		gotoxy(p->x + 3, p->y + 3);
		printf(" ");
		newbgcolor(195);
		gotoxy(p->x + 4, p->y + 2);
		printf(" ");
		newbgcolor(15);
		gotoxy(p->x + 5, p->y + 1);
		printf(" ");
		newbgcolor(21);
		gotoxy(p->x + 5, p->y);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 5, p->y - 1);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 4, p->y - 2);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 3, p->y - 3);
		printf(" ");
	}
	p->x = p->x << 14;
	p->y = p->y << 14;
}

void update_large_background(struct vector *p) {
	p->x = p->x >> 14;
	p->y = p->y >> 14;
	if (p->y == 63) {
		newbgcolor(17);
		gotoxy(p->x + 3, p->y + 5);
		printf(" ");
	} else if (p->y == 64) {
		newbgcolor(18);
		gotoxy(p->x + 3, p->y + 5);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 6, p->y + 4);
		printf(" ");
	} else if (p->y == 65) {
		newbgcolor(19);
		gotoxy(p->x + 3, p->y + 5);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 6, p->y + 4);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 7, p->y + 3);
		printf(" ");
	} else if (p->y == 66) {
		newbgcolor(20);
		gotoxy(p->x + 3, p->y + 5);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 6, p->y + 4);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 7, p->y + 3);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 8, p->y + 2);
		printf(" ");
	} else if (p->y == 67) {
		newbgcolor(21);
		gotoxy(p->x + 3, p->y + 5);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 6, p->y + 4);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 7, p->y + 3);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 8, p->y + 2);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 9, p->y + 1);
		printf(" ");
	} else if (p->y == 68) {
		newbgcolor(15);
		gotoxy(p->x + 3, p->y + 5);
		printf(" ");
		newbgcolor(21);
		gotoxy(p->x + 6, p->y + 4);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 7, p->y + 3);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 8, p->y + 2);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 9, p->y + 1);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 9, p->y);
		printf(" ");
	} else if (p->y == 69) {
		newbgcolor(195);
		gotoxy(p->x + 3, p->y + 5);
		printf(" ");
		newbgcolor(15);
		gotoxy(p->x + 6, p->y + 4);
		printf(" ");
		newbgcolor(21);
		gotoxy(p->x + 7, p->y + 3);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 8, p->y + 2);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 9, p->y + 1);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 9, p->y);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 9, p->y - 1);
		printf(" ");
	} else if (p->y == 70) {
		newbgcolor(159);
		gotoxy(p->x + 3, p->y + 5);
		printf(" ");
		newbgcolor(195);
		gotoxy(p->x + 6, p->y + 4);
		printf(" ");
		newbgcolor(15);
		gotoxy(p->x + 7, p->y + 3);
		printf(" ");
		newbgcolor(21);
		gotoxy(p->x + 8, p->y + 2);
		printf(" ");
		newbgcolor(20);
		gotoxy(p->x + 9, p->y + 1);
		printf(" ");
		newbgcolor(19);
		gotoxy(p->x + 9, p->y);
		printf(" ");
		newbgcolor(18);
		gotoxy(p->x + 9, p->y - 1);
		printf(" ");
		newbgcolor(17);
		gotoxy(p->x + 8, p->y - 2);
		printf(" ");
	}
	p->x = p->x << 14;
	p->y = p->y << 14;
}

void update_bullet(struct vector bullet) {
	bullet.y = bullet.y >> 14;
	if (bullet.y == 68) {
		newbgcolor(17);
	} else if (bullet.y == 69) {
		newbgcolor(18);
	} else if (bullet.y == 70) {
		newbgcolor(19);
	} else if (bullet.y == 71) {
		newbgcolor(20);
	} else if (bullet.y == 72) {
		newbgcolor(21);
	} else if (bullet.y == 73) {
		newbgcolor(15);
	} else if (bullet.y == 74) {
		newbgcolor(195);
	} else if (bullet.y == 75) {
		newbgcolor(159);
	} else {
		newbgcolor(0);
	}
}

/*
 void update_pix(struct pixel data[7], struct vector *p) {

 gotoxy(p->x - 7, p->y);
 newbgcolor(data[p->x - 7][p->y].bg);
 printf(" ");

 gotoxy(p->x - 6, p->y);
 newbgcolor(data[p->x - 6][p->y].bg);
 printf(" ");
 gotoxy(p->x - 5, p->y);
 newbgcolor(data[p->x - 5][p->y].bg);
 printf(" ");
 gotoxy(p->x + 1, p->y);
 newbgcolor(data[p->x + 1][p->y].bg);
 printf(" ");
 gotoxy(p->x - 7, p->y - 1);
 newbgcolor(data[p->x - 7][p->y - 1].bg);
 printf(" ");
 gotoxy(p->x - 1, p->y - 1);
 newbgcolor(data[p->x - 1][p->y - 1].bg);
 printf(" ");
 gotoxy(p->x, p->y - 1);
 newbgcolor(data[p->x][p->y - 1].bg);
 printf(" ");
 gotoxy(p->x - 6, p->y - 2);
 newbgcolor(data[p->x - 6][p->y - 2].bg);
 printf(" ");
 gotoxy(p->x - 5, p->y - 2);
 newbgcolor(data[p->x - 5][p->y - 2].bg);
 printf(" ");
 gotoxy(p->x - 3, p->y - 2);
 newbgcolor(data[p->x - 3][p->y - 2].bg);
 printf(" ");
 gotoxy(p->x - 2, p->y - 2);
 newbgcolor(data[p->x - 2][p->y - 2].bg);
 printf(" ");
 gotoxy(p->x - 4, p->y - 3);
 newbgcolor(data[p->x - 4][p->y - 3].bg);
 printf(" ");

 gotoxy(p->x - 7, p->y + 1);
 newbgcolor(data[p->x - 7][p->y + 1].bg);
 printf(" ");
 gotoxy(p->x - 1, p->y + 1);
 newbgcolor(data[p->x - 1][p->y + 1].bg);
 printf(" ");
 gotoxy(p->x, p->y + 1);
 newbgcolor(data[p->x][p->y + 1].bg);
 printf(" ");
 gotoxy(p->x - 6, p->y + 2);
 newbgcolor(data[p->x - 6][p->y + 2].bg);
 printf(" ");
 gotoxy(p->x - 5, p->y + 2);
 newbgcolor(data[p->x - 5][p->y + 2].bg);
 printf(" ");
 gotoxy(p->x - 3, p->y + 2);
 newbgcolor(data[p->x - 3][p->y + 2].bg);
 printf(" ");
 gotoxy(p->x - 2, p->y + 2);
 newbgcolor(data[p->x - 2][p->y + 2].bg);
 printf(" ");
 gotoxy(p->x - 4, p->y + 3);
 newbgcolor(data[p->x - 4][p->y + 3].bg);
 printf(" ");

 }


 void check_background(struct vector *p){
 //atmosphere
 if (p->y == 66){
 gotoxy(p->x-4,p->y+2);
 newbgcolor(17);
 printf(" ");
 }
 else if (p->y == 67){

 }
 gotoxy(0, 69);
 newbgcolor(18);
 for (int i = 0; i < 269; i++) {
 printf(" ");
 }
 gotoxy(0, 70);
 newbgcolor(19);
 for (int i = 0; i < 269; i++) {
 printf(" ");
 }
 gotoxy(0, 71);
 newbgcolor(20);
 for (int i = 0; i < 269; i++) {
 printf(" ");
 }
 gotoxy(0, 72);
 newbgcolor(21);
 for (int i = 0; i < 269; i++) {
 printf(" ");
 }

 //Planet
 gotoxy(0, 73);
 newbgcolor(15);
 for (int i = 0; i < 269; i++) {
 printf(" ");
 }
 gotoxy(0, 74);
 newbgcolor(195);
 for (int i = 0; i < 269; i++) {
 printf(" ");
 }
 gotoxy(0, 75);
 newbgcolor(159);
 for (int i = 0; i < 269; i++) {
 printf(" ");
 }
 }
 }
 */


void blackHole(int32_t a, int32_t b){
		newfgcolor(52);
		gotoxy(a-1,b);
		printf("   ");

		newbgcolor(88);
		gotoxy(a-2,b);
		printf(" ");
		gotoxy(a + 2 , b);
		printf(" ");
		gotoxy(a - 1, b -1);
		printf("   ");

		gotoxy(a - 1, b + 1);
		printf("   ");
		newbgcolor(0);
		gotoxy(a , b );
				printf(" ");
		resetbgcolor();

}

