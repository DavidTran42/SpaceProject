/*
 * mainMenu.c
 *
 *  Created on: 11 Jun 2021
 *      Author: silas
 */
#define ESC 0x1B
#include "main.h"
#include "ansi.h"
#include "30010_io.h"
#include "GameController.h"

void repeat(int c, int count) {
	for (int i = 0; i < count; i++)
		printf("%c", c);
}

typedef struct time {
	volatile int8_t hour, min, sec, sec100;
} time;

struct time timer; // Global timer

uint16_t borderWidth = 270, borderHeight = 75;

void mainFrame(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	color(6, 0);
	gotoxy(x1, y1);
	printf("%c", 201);
	repeat(205, (x2 - x1 - 2));
	printf("%c", 187);
	for (int i = y1 + 1; i < y2 - 1; i++) {
		gotoxy(x1, i);
		printf("%c", 186);
		gotoxy(x2 - x1, i);
		printf("%c\n", 186);
	}
	printf("%c", 200);
	repeat(205, (x2 - x1 - 2));
	printf("%c", 188);
}
void gameTitle() {
	fgcolor(1);
	// Printer A
	gotoxy(20, 5);
	printf(
			"     ___           ___                       ___           ___           ___                      _____                  _____          ___           ___                       ___           ___                       ___           ___");
	gotoxy(20, 6);
	fgcolor(2);
	printf(
			"    /  /\\         /  /\\          ___        /  /\\         /  /\\         /  /\\        ___         /  /::\\                /  /::\\        /  /\\         /  /\\          ___        /  /\\         /  /\\          ___        /  /\\         /  /\\");
	gotoxy(20, 7);
	fgcolor(3);
	printf(
			"   /  /::\\       /  /:/_        /  /\\      /  /:/_       /  /::\\       /  /::\\      /  /\\       /  /:/\\:\\              /  /:/\\:\\      /  /:/_       /  /:/_        /  /\\      /  /::\\       /  /::\\        /__/|      /  /:/_       /  /::\\   ");
	gotoxy(20, 8);
	fgcolor(4);
	printf(
			"  /  /:/\\:\\     /  /:/ /\\      /  /:/     /  /:/ /\\     /  /:/\\:\\     /  /:/\\:\\    /  /:/      /  /:/  \\:\\            /  /:/  \\:\\    /  /:/ /\\     /  /:/ /\\      /  /:/     /  /:/\\:\\     /  /:/\\:\\      |  |:|     /  /:/ /\\     /  /:/\\:\\  ");
	gotoxy(20, 9);
	fgcolor(5);
	printf(
			" /  /:/~/::\\   /  /:/ /::\\    /  /:/     /  /:/ /:/_   /  /:/~/:/    /  /:/  \\:\\  /__/::\\     /__/:/ \\__\\:|          /__/:/ \\__\\:|  /  /:/ /:/_   /  /:/ /::\\    /  /:/     /  /:/~/:/    /  /:/  \\:\\     |  |:|    /  /:/ /:/_   /  /:/~/:/ ");
	gotoxy(20, 10);
	fgcolor(6);
	printf(
			"/__/:/ /:/\\:\\ /__/:/ /:/\\:\\  /  /::\\    /__/:/ /:/ /\\ /__/:/ /:/___ /__/:/ \\__\\:\\ \\__\\/\\:\\__  \\  \\:\\ /  /:/          \\  \\:\\ /  /:/ /__/:/ /:/ /\\ /__/:/ /:/\\:\\  /  /::\\    /__/:/ /:/___ /__/:/ \\__\\:\\  __|__|:|   /__/:/ /:/ /\\ /__/:/ /:/___");
	gotoxy(20, 11);
	fgcolor(9);
	printf(
			"\\  \\:\\/:/__\\/ \\  \\:\\/:/~/:/ /__/:/\\:\\   \\  \\:\\/:/ /:/ \\  \\:\\/:::::/ \\  \\:\\ /  /:/    \\  \\:\\/\\  \\  \\:\\  /:/            \\  \\:\\  /:/  \\  \\:\\/:/ /:/ \\  \\:\\/:/~/:/ /__/:/\\:\\   \\  \\:\\/:::::/ \\  \\:\\ /  /:/ /__/::::\\   \\  \\:\\/:/ /:/ \\  \\:\\/:::::/");
	gotoxy(20, 12);
	fgcolor(11);
	printf(
			" \\  \\::/       \\  \\::/ /:/  \\__\\/  \\:\\   \\  \\::/ /:/   \\  \\::/~~~~   \\  \\:\\  /:/      \\__\\::/   \\  \\:\\/:/              \\  \\:\\/:/    \\  \\::/ /:/   \\  \\::/ /:/  \\__\\/  \\:\\   \\  \\::/~~~~   \\  \\:\\  /:/     ~\\~~\\:\\   \\  \\::/ /:/   \\  \\::/~~~~ ");
	gotoxy(20, 13);
	fgcolor(6);
	printf(
			"  \\  \\:\\        \\__\\/ /:/        \\  \\:\\   \\  \\:\\/:/     \\  \\:\\        \\  \\:\\/:/       /__/:/     \\  \\::/                \\  \\::/      \\  \\:\\/:/     \\__\\/ /:/        \\  \\:\\   \\  \\:\\        \\  \\:\\/:/        \\  \\:\\   \\  \\:\\/:/     \\  \\:\\");
	gotoxy(20, 14);
	fgcolor(9);
	printf(
			"   \\  \\:\\         /__/:/          \\__\\/    \\  \\::/       \\  \\:\\        \\  \\::/        \\__\\/       \\__\\/                  \\__\\/        \\  \\::/        /__/:/          \\__\\/    \\  \\:\\        \\  \\::/          \\__\\/    \\  \\::/       \\  \\:\\");
	gotoxy(20, 15);
	fgcolor(11);
	printf(
			"    \\__\\/         \\__\\/                     \\__\\/         \\__\\/         \\__\\/                                                          \\__\\/         \\__\\/                     \\__\\/         \\__\\/                     \\__\\/         \\__\\/");

	resetbgcolor();

	// main menu text

	fgcolor(13);
	gotoxy(90, 21);
	printf(" __       __            __");
	gotoxy(90, 22);
	printf("|  \\     /  \\          |  \\");
	gotoxy(90, 23);
	printf(
			"| $$\\   /  $$  ______   \\$$ _______         ______ ____    ______   _______   __    __");
	gotoxy(90, 24);
	printf(
			"| $$$\\ /  $$$ |      \\ |  \\|       \\       |      \\    \\  /      \\ |       \\ |  \\  |  \\");
	gotoxy(90, 25);
	printf(
			"| $$$$\\  $$$$  \\$$$$$$\\| $$| $$$$$$$\\      | $$$$$$\\$$$$\\|  $$$$$$\\| $$$$$$$\\| $$  | $$");
	gotoxy(90, 26);
	printf(
			"| $$\\$$ $$ $$ /      $$| $$| $$  | $$      | $$ | $$ | $$| $$    $$| $$  | $$| $$  | $$");
	gotoxy(90, 27);
	printf(
			"| $$ \\$$$| $$|  $$$$$$$| $$| $$  | $$      | $$ | $$ | $$| $$$$$$$$| $$  | $$| $$__/ $$");
	gotoxy(90, 28);
	printf(
			"| $$  \\$ | $$ \\$$    $$| $$| $$  | $$      | $$ | $$ | $$ \\$$     \\| $$  | $$ \\$$    $$");
	gotoxy(90, 29);
	printf(
			" \\$$      \\$$  \\$$$$$$$ \\$$ \\$$   \\$$       \\$$  \\$$  \\$$  \\$$$$$$$ \\$$   \\$$  \\$$$$$$");
	resetbgcolor();
}

void mainOptions() {
	color(6, 0);
	singleplayerMenu();
	multiplayerMenu();
	helpMenu();
	guitGameMenu();

}

void singleplayerMenu() {
	gotoxy(110, 32);
	printf("%c", 201);
	repeat(205, 50);
	printf("%c", 187);
	for (int i = 32 + 1; i < 37 - 1; i++) {
		gotoxy(110, i);
		printf("%c", 186);
		gotoxy(161, i);
		printf("%c\n", 186);
	}
	gotoxy(110, 36);
	printf("%c", 200);
	repeat(205, 50);
	printf("%c", 188);

	// text
	gotoxy(111, 33);
	printf(" .-. .-. . . .-. .   .-. .-. .   .-. . . .-. .-.  ");
	gotoxy(111, 34);
	printf(" `-.  |  |\\| |.. |   |-  |-' |   |-|  |  |-  |(   ");
	gotoxy(111, 35);
	printf(" `-' `-' ' ` `-' `-' `-' '   `-' ` '  `  `-' ' '  ");
}
void multiplayerMenu() {
	gotoxy(110, 39);
	printf("%c", 201);
	repeat(205, 50);
	printf("%c", 187);
	for (int i = 39 + 1; i < 44 - 1; i++) {
		gotoxy(110, i);
		printf("%c", 186);
		gotoxy(161, i);
		printf("%c\n", 186);
	}
	gotoxy(110, 43);
	printf("%c", 200);
	repeat(205, 50);
	printf("%c", 188);

	// text
	gotoxy(111, 40);
	printf("   .  . . . .   .-. .-. .-. .   .-. . . .-. .-.   ");
	gotoxy(111, 41);
	printf("   |\\/| | | |    |   |  |-' |   |-|  |  |-  |(    ");
	gotoxy(111, 42);
	printf("   '  ` `-' `-'  '  `-' '   `-' ` '  `  `-' ' '   ");
}
void helpMenu() {
	gotoxy(110, 46);
	printf("%c", 201);
	repeat(205, 50);
	printf("%c", 187);
	for (int i = 46 + 1; i < 51 - 1; i++) {
		gotoxy(110, i);
		printf("%c", 186);
		gotoxy(161, i);
		printf("%c\n", 186);
	}
	gotoxy(110, 50);
	printf("%c", 200);
	repeat(205, 50);
	printf("%c", 188);

	// text
	gotoxy(111, 47);
	printf("                   . . .-. .   .-.                ");
	gotoxy(111, 48);
	printf("                   |-| |-  |   |-'                ");
	gotoxy(111, 49);
	printf("                   ' ` `-' `-' '                  ");
}

void guitGameMenu() {
	gotoxy(110, 53);
	printf("%c", 201);
	repeat(205, 50);
	printf("%c", 187);
	for (int i = 53 + 1; i < 58 - 1; i++) {
		gotoxy(110, i);
		printf("%c", 186);
		gotoxy(161, i);
		printf("%c\n", 186);
	}
	gotoxy(110, 57);
	printf("%c", 200);
	repeat(205, 50);
	printf("%c", 188);

	// text
	gotoxy(111, 54);
	printf("         .-. . . .-. .-.   .-. .-. .  . .-.       ");
	gotoxy(111, 55);
	printf("         |.| | |  |   |    |.. |-| |\\/| |-        ");
	gotoxy(111, 56);
	printf("         `-`.`-' `-'  '    `-' ` ' '  ` `-'       ");
}

void optionsSelect() {
	RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
	enableTimer();
	TIM2->ARR = 639999; // Set reload value for 64x10^3 HZ - 1 (1/100 second)
	setPrescaler(0); // prescale value
	TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	NVIC_EnableIRQ(TIM2_IRQn);

	uint8_t i = 1;
	uint8_t t = 0;
	color(6, 0);
	char input;

	while (1) {

		while (i == 1) { // blinker singleplayer menu

			if (!timer.sec++) {
				t++;

				if (t < 200 && t > 100) {
					inverse(1);
					singleplayerMenu();
				} else {
					inverse(0);
					singleplayerMenu();
				}
				if (t == 200) {
					t = 0;
				}
			}
			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					inverse(0);
					singleplayerMenu();
					i--;
				} else if (input == 's') {
					inverse(0);
					singleplayerMenu();
					i++;
				} else if (input == ' ') {
					inverse(0);
					disableTimer();
					clrscr();
					initGame(borderWidth, borderHeight, 1);

				}
				if (i > 4) {
					i = 1;
				}
				if (i < 1) {
					i = 4;
				}
			}
		}
		while (i == 2) { // blinker multiplayermenu
			if (!timer.sec++) {
				t++;

				if (t < 200 && t > 100) {
					inverse(1);
					multiplayerMenu();
				} else {
					inverse(0);
					multiplayerMenu();
				}

				if (t == 200) {
					t = 0;
				}
			}
			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					inverse(0);
					multiplayerMenu();
					i--;
				} else if (input == 's') {
					inverse(0);
					multiplayerMenu();
					i++;
				} else if (input == ' ') {
					inverse(0);
					disableTimer();
					clrscr();
					initGame(borderWidth, borderHeight, 2);

				}
				if (i > 4) {
					i = 1;
				}
				if (i < 1) {
					i = 4;
				}
			}

		}
		while (i == 3) { // blinker Help
			if (!timer.sec++) {
				t++;

				if (t < 200 && t > 100) {
					inverse(1);
					helpMenu();
				} else {
					inverse(0);
					helpMenu();
				}

				if (t == 200) {
					t = 0;
				}
			}
			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					inverse(0);
					helpMenu();
					i--;
				} else if (input == 's') {
					inverse(0);
					helpMenu();
					i++;
				} else if (input == ' ') {
					inverse(0);
					disableTimer();
					clrscr();

				}
				if (i > 4) {
					i = 1;
				}
				if (i < 1) {
					i = 4;
				}

			}

		}
		while (i == 4) { // blinker Quit game
			if (!timer.sec++) {
				t++;

				if (t < 200 && t > 100) {
					inverse(1);
					guitGameMenu();

				} else {
					inverse(0);
					guitGameMenu();
				}

				if (t == 200) {
					t = 0;
				}
			}
			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					inverse(0);
					guitGameMenu();
					i--;
				} else if (input == 's') {
					inverse(0);
					guitGameMenu();
					i++;
				} else if (input == ' ') {
					inverse(0);
					disableTimer();
					clrscr();

				}
				if (i > 4) {
					i = 1;
				}
				if (i < 1) {
					i = 4;
				}
			}

		}
	}

}

void mainMenu() {
	resetbgcolor();
	printf("%c[?25l", ESC); //hiding curser

	clrscr();

	int16_t box_h = 75, box_w = 270;
	background();
	mainFrame(1, 1, box_w, box_h);
	gameTitle();
	mainOptions();
	optionsSelect();
	resetbgcolor();
}

