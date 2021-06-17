/*
 * help.c
 *
 *  Created on: 16. jun. 2021
 *      Author: Marius Mainz
 */
#include "main.h"
#include "ansi.h"
#include "help.h"

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
	uint32_t t = 0;
	newfgcolor(15);
	gotoxy(125, 70);
	printf("Developers:");
	gotoxy(120, 72);
	printf("David Tran");
	gotoxy(120, 73);
	printf("Silas Lohmann");
	gotoxy(120, 74);
	printf("Marius Mainz Elkjær");
	while (1) {
		t++;

		if (t == 2000) {
			t = 0;
			for (int i = 0; i < 75; i++) {
				gotoxy(120, i);
				printf("                  ");
				gotoxy(120, i - 4);
				printf("Developers:");
				gotoxy(120, i - 2);
				printf("David Tran");
				gotoxy(120, i - 1);
				printf("Silas Lohmann");
				gotoxy(120, i);
				printf("Marius Mainz Elkjær");
			}
		}
	}

}

