/*
 * help.c
 *
 *  Created on: 16. jun. 2021
 *      Author: Marius Mainz
 */
#include "main.h"
#include "ansi.h"
#include "30010_io.h"
#include "GameController.h"
#include "stdlib.h"
#include "help.h"

typedef struct time1 {
	volatile int8_t hour, min, sec, sec100;
} time1;

struct time1 timer; // Global timer

void help_screen() {
	char input;

	newfgcolor(214);
	gotoxy(120, 10);
	printf("    )       (     (     ____");
	gotoxy(120, 11);
	printf(" ( /(       )\\ )  )\\ ) |   / ");
	gotoxy(120, 12);
	newfgcolor(220);
	printf(" )\\()) (   (()/( (()/( |  /  ");
	gotoxy(120, 13);
	printf("((_)\\  )\\   /(_)) /(_))| /   ");
	gotoxy(120, 14);
	printf(" _((_)((_) (_))  (_))  |/    ");
	newfgcolor(226);
	gotoxy(120, 15);
	printf("| || || __|| |   | _ \(    ");
	gotoxy(120, 16);
	printf("| __ || _| | |__ |  _/)\\  ");
	gotoxy(120, 17);
	printf("|_||_||___||____||_| ((_)  ");
	while (1) {
		if (uart_get_count() > 0) {
			input = uart_get_char();
			uart_clear();
			if (input == ' ') {
				mainMenu();
			}
		}
	}
}

void quit_screen() {
	setUpTimer();
	char input;
	uint32_t t = 71;
	newfgcolor(15);
	while (1) {
		if (timer.sec100 == 1 || timer.sec100 == 50) {
			for (int i = 0; i < 75; i++) {
				gotoxy(100, i);
				printf("                                                                          ");
			}
			gotoxy(122, t - 6);
			printf("THANK YOU FOR PLAYING !");
			gotoxy(125, t - 4);
			printf("Developers:");
			gotoxy(125, t - 2);
			printf("David Tran");
			gotoxy(125, t - 1);
			printf("Silas Lohmann");
			gotoxy(125, t);
			printf("Marius Mainz Elkjaer");

			t--;
			if (t < 40) {
				gotoxy(115, 50);
				printf("(Press 'SPACE' to return to Main Menu)");
			}
		}

		if (uart_get_count() > 0) {
			input = uart_get_char();
			uart_clear();
			if (input == ' ') {
				mainMenu();
			}
		}
	}
}

