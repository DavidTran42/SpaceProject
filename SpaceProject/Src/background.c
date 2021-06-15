/*
 * background.c
 *
 *  Created on: 11. jun. 2021
 *      Author: Marius Mainz
 */

#include <stdio.h>
#include "main.h"
#define ESC 0x1B
#include "Excellutex.h"
#include "charset.h"
#include "30010_io.h"
#include "ansi.h"
#include "GameController.h"

void newbgcolor(uint8_t background) {
	printf("\033[48;5;%dm", background);
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
	newbgcolor(252);
	gotoxy(asteroid1->pos.x - 1, asteroid1->pos.y - 1);
	printf("   ");
	gotoxy(asteroid1->pos.x - 2, asteroid1->pos.y);
	printf("     ");
	gotoxy(asteroid1->pos.x - 1, asteroid1->pos.y + 1);
	printf("   ");

	newbgcolor(240);
	gotoxy(asteroid1->pos.x - 1, asteroid1->pos.y);
	printf(" ");
	gotoxy(asteroid1->pos.x, asteroid1->pos.y - 1);
	printf(" ");

	newbgcolor(247);
	gotoxy(asteroid1->pos.x + 1, asteroid1->pos.y - 1);
	printf(" ");
	gotoxy(asteroid1->pos.x - 2, asteroid1->pos.y);
	printf(" ");
	gotoxy(asteroid1->pos.x, asteroid1->pos.y + 1);
	printf(" ");

	resetbgcolor();
}

void medium_asteroid(struct asteroid *asteroid2) {
	newbgcolor(240);
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y - 3);
	printf("     ");

	newbgcolor(247);
	gotoxy(asteroid2->pos.x - 3, asteroid2->pos.y - 2);
	printf("       ");
	newbgcolor(252);
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y - 1);
	printf("         ");
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y);
	printf("         ");
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y + 1);
	printf("         ");

	gotoxy(asteroid2->pos.x - 3, asteroid2->pos.y + 2);
	printf("       ");
	newbgcolor(247);
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y + 3);
	printf("     ");
	newbgcolor(240);
	gotoxy(asteroid2->pos.x - 3, asteroid2->pos.y - 2);
	printf("  ");
	newbgcolor(247);
	gotoxy(asteroid2->pos.x - 4, asteroid2->pos.y - 1);
	printf(" ");

	newbgcolor(8);
	gotoxy(asteroid2->pos.x + 1, asteroid2->pos.y);
	printf(" ");
	gotoxy(asteroid2->pos.x + 2, asteroid2->pos.y + 1);
	printf(" ");
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y + 2);
	printf("  ");

	newbgcolor(240);
	gotoxy(asteroid2->pos.x + 1, asteroid2->pos.y);
	printf(" ");
	gotoxy(asteroid2->pos.x + 2, asteroid2->pos.y + 1);
	printf(" ");
	gotoxy(asteroid2->pos.x - 2, asteroid2->pos.y + 2);
	printf("  ");

	newbgcolor(247);
	gotoxy(asteroid2->pos.x + 3, asteroid2->pos.y - 2);
	printf(" ");
	newbgcolor(250);
	gotoxy(asteroid2->pos.x + 3, asteroid2->pos.y - 1);
	printf("  ");

	resetbgcolor();
}

void large_asteroid(struct asteroid *asteroid3) {
	newbgcolor(240);
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y - 5);
	printf("     ");
	newbgcolor(247);
	gotoxy(asteroid3->pos.x - 5, asteroid3->pos.y - 4);
	printf("           ");
	newbgcolor(252);
	gotoxy(asteroid3->pos.x - 6, asteroid3->pos.y - 3);
	printf("             ");
	newbgcolor(250);
	gotoxy(asteroid3->pos.x - 7, asteroid3->pos.y - 2);
	printf("               ");
	newbgcolor(252);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y - 1);
	printf("                 ");
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y);
	printf("                 ");
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y + 1);
	printf("                 ");
	gotoxy(asteroid3->pos.x - 7, asteroid3->pos.y + 2);
	printf("               ");
	newbgcolor(250);
	gotoxy(asteroid3->pos.x - 6, asteroid3->pos.y + 3);
	printf("             ");
	gotoxy(asteroid3->pos.x - 5, asteroid3->pos.y + 4);
	printf("           ");
	newbgcolor(247);
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y + 5);
	printf("     ");

	newbgcolor(8);
	gotoxy(asteroid3->pos.x - 4, asteroid3->pos.y + 1);
	printf("  ");
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y + 2);
	printf(" ");
	gotoxy(asteroid3->pos.x, asteroid3->pos.y + 4);
	printf("  ");
	gotoxy(asteroid3->pos.x + 2, asteroid3->pos.y + 4);
	printf(" ");

	newbgcolor(248);
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y - 1);
	printf("   ");
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y);
	printf(" ");
	gotoxy(asteroid3->pos.x - 8, asteroid3->pos.y + 1);
	printf(" ");

	gotoxy(asteroid3->pos.x - 1, asteroid3->pos.y - 1);
	printf("  ");
	gotoxy(asteroid3->pos.x + 1, asteroid3->pos.y);
	printf(" ");
	gotoxy(asteroid3->pos.x + 1, asteroid3->pos.y + 1);
	printf(" ");

	newbgcolor(250);
	gotoxy(asteroid3->pos.x - 2, asteroid3->pos.y - 3);
	printf("  ");
	gotoxy(asteroid3->pos.x + 1, asteroid3->pos.y - 4);
	printf("   ");

	resetbgcolor();

}

