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

void repeat(int c, int count) {
	for (int i = 0; i < count; i++)
		printf("%c", c);
}

typedef struct time {
	volatile int8_t hour, min, sec, sec100;
} time;

struct time timer; // Global timer

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
	/*
	 fgcolor(1);
	 // Printer A
	 gotoxy(65, 5);
	 printf(
	 "__      _____________________________           ________________          _________________________________________               _________________");
	 gotoxy(64, 6);
	 fgcolor(2);
	 printf(
	 "/  \\    /  __________________________ \\  _______|_______________ \\         |  _____________________________________ \\  _______    / /| ____________ \\");
	 gotoxy(63, 7);
	 fgcolor(3);
	 printf(
	 "/ __ \\  /  /        | | | |     | |   \\ \\/ _____ \\   | |   | |   \\ \\        | |   \\ \\ | |    /  /         | | | |   \\ \\/ _____ \\  / / | |     | |   \\ \\");
	 gotoxy(62, 8);
	 fgcolor(4);
	 printf(
	 "/ /  \\ \\ \\  \\_____   | | | |___  | |___/ / /     \\ \\  | |   | |    \\ \\       | |    \\ \\| |____\\  \\______   | | | |___/ / /     \\ \\/ /  | |___  | |___/ /");
	 gotoxy(61, 9);
	 fgcolor(5);
	 printf(
	 "/ /____\\ \\ \\_____  \\  | | |  ___| | |\\   / /       \\ \\ | |   | |     ) )      | |     ) | |____|\\_______ \\  | | | |  / / /       \\ \\/   | |___| | |  / /");
	 gotoxy(60, 10);
	 fgcolor(6);
	 printf(
	 "/ ________ \\      \\  \\ | | | |     | | \\  \\ \\       / / | |   | |    / /       | |    / /| |             \\ \\ | | | |  \\ \\ \\       / /    | |     | |  \\ \\");
	 gotoxy(59, 11);
	 fgcolor(9);
	 printf(
	 "/ /        \\ \\_____/  / | | | |_____| |  \\  \\ \\_____/ /__| |___| |___/ /        | |___/ / | |____ ________/ / | | | |   \\ \\ \\_____/ /     | |_____| |   \\ \\");
	 gotoxy(58, 12);
	 fgcolor(11);
	 printf(
	 "/_/          \\_\\______/  |_| |_______|_|___\\__\\_______/_________|______/         |______/  |______|_________/  |_| |_|____\\_\\_______/      |_______|_|____\\_\\");
	 resetbgcolor();

	 fgcolor(1);
	 // Printer A
	 gotoxy(65, 5);
	 printf("");
	 gotoxy(64, 6);
	 fgcolor(2);
	 printf("");
	 gotoxy(63, 7);
	 fgcolor(3);
	 printf("");
	 gotoxy(62, 8);
	 fgcolor(4);
	 printf("");
	 gotoxy(61, 9);
	 fgcolor(5);
	 printf("");
	 gotoxy(60, 10);
	 fgcolor(6);
	 printf("");
	 gotoxy(59, 11);
	 fgcolor(9);
	 printf("");
	 gotoxy(58, 12);
	 fgcolor(11);
	 printf("");
	 resetbgcolor();
	 */

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
	// Singleplayer menu box
	color(6, 0);
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
	gotoxy(112, 33);
	printf(".-. .-. . . .-. .   .-. .-. .   .-. . . .-. .-.");
	gotoxy(112, 34);
	printf("`-.  |  |\\| |.. |   |-  |-' |   |-|  |  |-  |( ");
	gotoxy(112, 35);
	printf("`-' `-' ' ` `-' `-' `-' '   `-' ` '  `  `-' ' '");

	// Multiplayer menu box
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
	gotoxy(114, 40);
	printf(".  . . . .   .-. .-. .-. .   .-. . . .-. .-.");
	gotoxy(114, 41);
	printf("|\\/| | | |    |   |  |-' |   |-|  |  |-  |( ");
	gotoxy(114, 42);
	printf("'  ` `-' `-'  '  `-' '   `-' ` '  `  `-' ' '");

	// Help menu box
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
	gotoxy(130, 47);
	printf(". . .-. .   .-.");
	gotoxy(130, 48);
	printf("|-| |-  |   |-'");
	gotoxy(130, 49);
	printf("' ` `-' `-' '");

	// Quit Game menu box
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
	gotoxy(120, 54);
	printf(".-. . . .-. .-.   .-. .-. .  . .-.");
	gotoxy(120, 55);
	printf("|.| | |  |   |    |.. |-| |\\/| |- ");
	gotoxy(120, 56);
	printf("`-`.`-' `-'  '    `-' ` ' '  ` `-'");

}

void optionsSelect() {
	RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
	enableTimer();
	TIM2->ARR = 639999; // Set reload value for 64x10^3 HZ - 1 (1/100 second)
	setPrescaler(0); // prescale value
	TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	NVIC_EnableIRQ(TIM2_IRQn);

	uint8_t i = 0;
	uint8_t t = 0;
	color(6, 0);
	char input;

	while (1) {

		while (i == 0) { // blinker singleplayer menu

			if (!timer.sec++) {
				t++;

				if (t < 400 && t > 210) {
					inverse(1);
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
					printf(
							" .-. .-. . . .-. .   .-. .-. .   .-. . . .-. .-.  ");
					gotoxy(111, 34);
					printf(
							" `-.  |  |\\| |.. |   |-  |-' |   |-|  |  |-  |(   ");
					gotoxy(111, 35);
					printf(
							" `-' `-' ' ` `-' `-' `-' '   `-' ` '  `  `-' ' '  ");

				} else {
					inverse(0);
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
					printf(
							" .-. .-. . . .-. .   .-. .-. .   .-. . . .-. .-.  ");
					gotoxy(111, 34);
					printf(
							" `-.  |  |\\| |.. |   |-  |-' |   |-|  |  |-  |(   ");
					gotoxy(111, 35);
					printf(
							" `-' `-' ' ` `-' `-' `-' '   `-' ` '  `  `-' ' '  ");
				}

				if (t == 400) {
					t = 0;
				}
			}
			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					i--;
				} else if (input == 's') {
					i++;
				} else if (input == ' ') {
					clrscr();
				} else if (i > 3) {
					i = 0;
				} else if (i < 0) {
					i = 3;

				}
			}
		}
		while (i == 1) { // blinker multiplayermenu
			if (!timer.sec++) {
				t++;

				if (t < 400 && t > 210) {
					inverse(1);
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
					printf(
							"   .  . . . .   .-. .-. .-. .   .-. . . .-. .-.   ");
					gotoxy(111, 41);
					printf(
							"   |\\/| | | |    |   |  |-' |   |-|  |  |-  |(    ");
					gotoxy(111, 42);
					printf(
							"   '  ` `-' `-'  '  `-' '   `-' ` '  `  `-' ' '   ");

				} else {
					inverse(0);
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
					printf(
							"   .  . . . .   .-. .-. .-. .   .-. . . .-. .-.   ");
					gotoxy(111, 41);
					printf(
							"   |\\/| | | |    |   |  |-' |   |-|  |  |-  |(    ");
					gotoxy(111, 42);
					printf(
							"   '  ` `-' `-'  '  `-' '   `-' ` '  `  `-' ' '   ");
				}

				if (t == 400) {
					t = 0;
				}
			}
			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					i--;
				} else if (input == 's') {
					i++;
				} else if (input == ' ') {
					clrscr();
				} else if (i > 3) {
					i = 0;
				} else if (i < 0) {
					i = 3;

				}
			}

		}
		while (i == 2) { // blinker Help
			if (!timer.sec++) {
				t++;

				if (t < 400 && t > 210) {
					inverse(1);
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
					printf(
							"                   . . .-. .   .-.                ");
					gotoxy(111, 48);
					printf(
							"                   |-| |-  |   |-'                ");
					gotoxy(111, 49);
					printf(
							"                   ' ` `-' `-' '                  ");

				} else {
					inverse(0);
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
					printf(
							"                   . . .-. .   .-.                ");
					gotoxy(111, 48);
					printf(
							"                   |-| |-  |   |-'                ");
					gotoxy(111, 49);
					printf(
							"                   ' ` `-' `-' '                  ");
				}

				if (t == 400) {
					t = 0;
				}
			}
			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					i--;
				} else if (input == 's') {
					i++;
				} else if (input == ' ') {
					clrscr();
				} else if (i > 3) {
					i = 0;
				} else if (i < 0) {
					i = 3;

				}
			}

		}
		while (i == 3) { // blinker Quit game
			if (!timer.sec++) {
				t++;

				if (t < 400 && t > 210) {
					inverse(1);
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
					printf(
							"         .-. . . .-. .-.   .-. .-. .  . .-.       ");
					gotoxy(111, 55);
					printf(
							"         |.| | |  |   |    |.. |-| |\\/| |-        ");
					gotoxy(111, 56);
					printf(
							"         `-`.`-' `-'  '    `-' ` ' '  ` `-'       ");

				} else {
					inverse(0);
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
					printf(
							"         .-. . . .-. .-.   .-. .-. .  . .-.       ");
					gotoxy(111, 55);
					printf(
							"         |.| | |  |   |    |.. |-| |\\/| |-        ");
					gotoxy(111, 56);
					printf(
							"         `-`.`-' `-'  '    `-' ` ' '  ` `-'       ");
				}

				if (t == 400) {
					t = 0;
				}
			}
			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					i--;
				} else if (input == 's') {
					i++;
				} else if (input == ' ') {
					clrscr();
				}
				if (i > 3) {
					i = 0;
				}
				if (i < 0) {
					i = 3;

				}
			}

		}
	}

}

void mainMenu() {
	printf("%c[?25l", ESC); //hiding curser

	clrscr();
	resetbgcolor();
	int16_t box_h = 75, box_w = 270;
	background();
	mainFrame(1, 1, box_w, box_h);
	gameTitle();
	mainOptions();
	optionsSelect();
}

