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

void newbgcolor(uint8_t background) {
	printf("\033[48;5;%dm", background);
}

void background() {

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

}
