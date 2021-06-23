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

void helpSelect() {
	RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
	enableTimer();
	TIM2->ARR = 639999; // Set reload value for 64x10^3 HZ - 1 (1/100 second)
	TIM2->PSC = 0; // prescale value
	TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	NVIC_EnableIRQ(TIM2_IRQn);

	uint8_t i = 1;
	char input;

	while (1) {

		while (i == 1) { // blinker controller menu

			if (timer.sec100 == 1) {
				inverse(1);
				controllerOption();
			} else if (timer.sec100 == 50) {
				inverse(0);
				controllerOption();
			}

			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					inverse(0);
					controllerOption();
					i--;
				} else if (input == 's') {
					inverse(0);
					controllerOption();
					i++;
				} else if (input == ' ') {
					inverse(0);
					clrscr();
					controllerMenu();

				}
				if (i > 4) {
					i = 1;
				}
				if (i < 1) {
					i = 4;
				}
			}
		}

		while (i == 2) { // blinker Help Option
			if (timer.sec100 == 1) {
				inverse(1);
				howToPlayOption();
			} else if (timer.sec100 == 50) {
				inverse(0);
				howToPlayOption();
			}

			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					inverse(0);
					howToPlayOption();
					i--;
				} else if (input == 's') {
					inverse(0);
					howToPlayOption();
					i++;
				} else if (input == ' ') {
					inverse(0);
					clrscr();
					howToPlayMenu();

				}
				if (i > 4) {
					i = 1;
				}
				if (i < 1) {
					i = 4;
				}
			}
		}
		while (i == 3) { // blinker about us
			if (timer.sec100 == 1) {
				inverse(1);
				aboutUsOption();
			} else if (timer.sec100 == 50) {
				inverse(0);
				aboutUsOption();
			}

			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					inverse(0);
					aboutUsOption();
					i--;
				} else if (input == 's') {
					inverse(0);
					aboutUsOption();
					i++;
				} else if (input == ' ') {
					inverse(0);
					clrscr();
					aboutUsMenu();

				}
				if (i > 4) {
					i = 1;
				}
				if (i < 1) {
					i = 4;
				}

			}
		}

		while (i == 4) { // blinker back
			if (timer.sec100 == 1) {
				inverse(1);
				backOption();
			} else if (timer.sec100 == 50) {
				inverse(0);
				backOption();
			}

			if (uart_get_count() > 0) {
				input = uart_get_char();
				uart_clear();
				if (input == 'w') {
					inverse(0);
					backOption();
					i--;
				} else if (input == 's') {
					inverse(0);
					backOption();
					i++;
				} else if (input == ' ') {
					inverse(0);
					disableTimer();
					clrscr();
					mainMenu();

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
void controllerMenu() {
	char input;
	// printer
	fgcolor(13);
	gotoxy(90, 5);
	printf(
			"  ______                         __                          __  __ ");
	gotoxy(90, 6);
	printf(
			" /      \\                       /  |                        /  |/  | ");
	gotoxy(90, 7);
	printf(
			"/$$$$$$  |  ______   _______   _$$ |_     ______    ______  $$ |$$ |  ______    ______ ");
	gotoxy(90, 8);
	printf(
			"$$ |  $$/  /      \\ /       \\ / $$   |   /      \\  /      \\ $$ |$$ | /      \\  /      \\");
	gotoxy(90, 9);
	printf(
			"$$ |      /$$$$$$  |$$$$$$$  |$$$$$$/   /$$$$$$  |/$$$$$$  |$$ |$$ |/$$$$$$  |/$$$$$$  |");
	gotoxy(90, 10);
	printf(
			"$$ |   __ $$ |  $$ |$$ |  $$ |  $$ | __ $$ |  $$/ $$ |  $$ |$$ |$$ |$$    $$ |$$ |  $$/");
	gotoxy(90, 11);
	printf(
			"$$ \\__/  |$$ \\__$$ |$$ |  $$ |  $$ |/  |$$ |      $$ \\__$$ |$$ |$$ |$$$$$$$$/ $$ |");
	gotoxy(90, 12);
	printf(
			"$$    $$/ $$    $$/ $$ |  $$ |  $$  $$/ $$ |      $$    $$/ $$ |$$ |$$       |$$ |");
	gotoxy(90, 13);
	printf(
			" $$$$$$/   $$$$$$/  $$/   $$/    $$$$/  $$/        $$$$$$/  $$/ $$/  $$$$$$$/ $$/");

	//keyboard box
	gotoxy(20, 22);
	printf("%c", 201);
	repeat(205, 84);
	printf("%c", 187);
	for (int i = 22 + 1; i < 61 - 1; i++) {
		gotoxy(20, i);
		printf("%c", 186);
		gotoxy(105, i);
		printf("%c\n", 186);
	}
	gotoxy(20, 60);
	printf("%c", 200);
	repeat(205, 84);
	printf("%c", 188);

	// text
	gotoxy(45, 23);
	printf(". . .-. . . .-. .-. .-. .-. .-.");
	gotoxy(45, 24);
	printf("|<  |-   |  |(  | | |-| |(  |  )");
	gotoxy(45, 25);
	printf("' ` `-'  `  `-' `-' ` ' ' ' `-'");
	gotoxy(20, 26);
	printf("%c", 204);
	repeat(205, 84);
	printf("%c", 186);

//W
	gotoxy(50, 30);
	printf("__      __");
	gotoxy(50, 31);
	printf("\\ \\    / /");
	gotoxy(50, 32);
	printf(" \\ \\/\\/ /  =  UP");
	gotoxy(50, 33);
	printf("  \\_/\\_/");

//A
	gotoxy(30, 37);
	printf("   _");
	gotoxy(30, 38);
	printf("  /_\\");
	gotoxy(30, 39);
	printf(" / _ \\  =  LEFT");
	gotoxy(30, 40);
	printf("/_/ \\_\\");

//S
	gotoxy(52, 37);
	printf(" ___ ");
	gotoxy(52, 38);
	printf("/ __|");
	gotoxy(52, 39);
	printf("\\__ \\  =  DOWN");
	gotoxy(52, 40);
	printf("|___/");

//D
	gotoxy(72, 37);
	printf(" ___ ");
	gotoxy(72, 38);
	printf("|   \\");
	gotoxy(72, 39);
	printf("| |) |  =  RIGHT");
	gotoxy(72, 40);
	printf("|___/");

//SPACE
	gotoxy(40, 45);
	printf("%c", 201);
	repeat(205, 30);
	printf("%c", 187);
	gotoxy(40, 46);
	printf("%c", 186);
	gotoxy(71, 46);
	printf("%c\n", 186);
	gotoxy(40, 47);
	printf("%c", 200);
	repeat(205, 30);
	printf("%c", 188);
	gotoxy(41, 46);
	printf("       SPACE  =  SHOOT    ");

	//A
	gotoxy(30, 51);
	printf(" __  __ ");
	gotoxy(30, 52);
	printf("|  \\/  |");
	gotoxy(30, 53);
	printf("| |\\/| |  =  Go to Main Menu");
	gotoxy(30, 54);
	printf("|_|  |_|");

	//B
	gotoxy(72, 51);
	printf(" ___ ");
	gotoxy(72, 52);
	printf("| _ )");
	gotoxy(72, 53);
	printf("| _ \\  =  Boss Key");
	gotoxy(72, 54);
	printf("|___/");

	//Joystick box
	gotoxy(165, 22);
	printf("%c", 201);
	repeat(205, 84);
	printf("%c", 187);
	for (int i = 22 + 1; i < 61 - 1; i++) {
		gotoxy(165, i);
		printf("%c", 186);
		gotoxy(250, i);
		printf("%c\n", 186);
	}
	gotoxy(165, 60);
	printf("%c", 200);
	repeat(205, 84);
	printf("%c", 188);

	// text
	gotoxy(195, 23);
	printf("  . .-. . . .-. .-. .-. .-. . .");
	gotoxy(195, 24);
	printf("  | | |  |  `-.  |   |  |   |< ");
	gotoxy(195, 25);
	printf("`-' `-'  `  `-'  '  `-' `-' ' `");
	gotoxy(165, 26);
	printf("%c", 204);
	repeat(205, 84);
	printf("%c", 186);

//Up
	gotoxy(202, 30);
	printf("  /\\");
	gotoxy(202, 31);
	printf(" /||\\");
	gotoxy(202, 32);
	printf("/ || \\  =  UP");
	gotoxy(202, 33);
	printf("  ||");

//Left
	gotoxy(180, 37);
	printf(" /");
	gotoxy(180, 38);
	printf("/-----");
	gotoxy(180, 39);
	printf("\\-----  =  LEFT");
	gotoxy(180, 40);
	printf(" \\");

//Center
	gotoxy(202, 37);
	printf("  ___");
	gotoxy(202, 38);
	printf(" / _ \\");
	gotoxy(202, 39);
	printf("| (_) |  =  SHOOT");
	gotoxy(202, 40);
	printf(" \\___/");

//Down
	gotoxy(202, 45);
	printf("  || ");
	gotoxy(202, 46);
	printf("\\ || /");
	gotoxy(202, 47);
	printf(" \\||/  =  DOWN");
	gotoxy(202, 48);
	printf("  \\/");

//RIGHT
	gotoxy(224, 37);
	printf("    \\");
	gotoxy(224, 38);
	printf("-----\\");
	gotoxy(224, 39);
	printf("-----/  =  RIGHT");
	gotoxy(224, 40);
	printf("    /");

	gotoxy(120, 65);
	printf("Press SPACE to go back");

	while (1) {

		if (uart_get_count() > 0) {
			input = uart_get_char();
			uart_clear();
			if (input == ' ') {
				inverse(0);
				clrscr();
				background();
				helpTitle();
				newfgcolor(226);
				controllerOption();
				howToPlayOption();
				aboutUsOption();
				backOption();
				break;
			}
		}

	}
}

//Menuerne!!

void howToPlayMenu() {
	char input;
// printer overskrift
	fgcolor(13);
	gotoxy(80, 5);
	printf(
			" __    __                                ________               _______   __                       ____ ");
	gotoxy(80, 6);
	printf(
			"/  |  /  |                              /        |             /       \\ /  |                     /    \\");
	gotoxy(80, 7);
	printf(
			"$$ |  $$ |  ______   __   __   __       $$$$$$$$/______        $$$$$$$  |$$ |  ______   __    __ /$$$$  |");
	gotoxy(80, 8);
	printf(
			"$$ |__$$ | /      \\ /  | /  | /  |         $$ | /      \\       $$ |__$$ |$$ | /      \\ /  |  /  |$$  $$ |");
	gotoxy(80, 9);
	printf(
			"$$    $$ |/$$$$$$  |$$ | $$ | $$ |         $$ |/$$$$$$  |      $$    $$/ $$ | $$$$$$  |$$ |  $$ |   /$$/ ");
	gotoxy(80, 10);
	printf(
			"$$$$$$$$ |$$ |  $$ |$$ | $$ | $$ |         $$ |$$ |  $$ |      $$$$$$$/  $$ | /    $$ |$$ |  $$ |  /$$/");
	gotoxy(80, 11);
	printf(
			"$$ |  $$ |$$ \\__$$ |$$ \\_$$ \\_$$ |         $$ |$$ \\__$$ |      $$ |      $$ |/$$$$$$$ |$$ \\__$$ |  $$/ ");
	gotoxy(80, 12);
	printf(
			"$$ |  $$ |$$    $$/ $$   $$   $$/          $$ |$$    $$/       $$ |      $$ |$$    $$ |$$    $$ |  /  |");
	gotoxy(80, 13);
	printf(
			"$$/   $$/  $$$$$$/   $$$$$/$$$$/           $$/  $$$$$$/        $$/       $$/  $$$$$$$/  $$$$$$$ |  $$/");
	gotoxy(80, 14);
	printf(
			"                                                                                       /  \\__$$ |");
	gotoxy(80, 15);
	printf(
			"                                                                                       $$    $$/ ");
	gotoxy(80, 16);
	printf(
			"                                                                                        $$$$$$/");

// Print box to
	gotoxy(70, 22);
	printf("%c", 201);
	repeat(205, 129);
	printf("%c", 187);
	for (int i = 22 + 1; i < 61 - 1; i++) {
		gotoxy(70, i);
		printf("%c", 186);
		gotoxy(200, i);
		printf("%c\n", 186);
	}
	gotoxy(70, 60);
	printf("%c", 200);
	repeat(205, 129);
	printf("%c", 188);

	gotoxy(90, 30);
	printf("The purpose of this game is to destroy all of the asteroids!");
	gotoxy(90, 31);
	printf(
			"You will control the spaceship with either the keyboard or the joystick.");
	gotoxy(90, 32);
	printf(
			"If You are playing multiplayer mode, player one is controlled by the keyboard");
	gotoxy(90, 33);
	printf("and player two is controlled by the joystick.");
	gotoxy(90, 35);
	printf(
			"The players will receive points by shooting the moving asteroids, but be careful");
	gotoxy(90, 36);
	printf("not to hit them with your vehicle!");
	gotoxy(90, 37);
	printf(
			"Both players depending on game-mode will start the game with 3 lives each,");
	gotoxy(90, 38);
	printf("one life is lost when a collision with an asteroid is detected.");
	gotoxy(90, 40);
	printf(
			"When a player have lost 3 lives, the player dies. If a player is remaining, they will continue.");
	gotoxy(90, 42);
	printf(
			"Watch out! A black hole has been spotted on the map, your bullets might be affected by a force.");
	gotoxy(90, 43);
	printf(
			"Look for power-ups on the map to increase your chances of winning!");
	gotoxy(90, 45);
	printf("GOOD LUCK!");

	gotoxy(120, 65);
	printf("Press SPACE to go back");

	while (1) {

		if (uart_get_count() > 0) {
			input = uart_get_char();
			uart_clear();
			if (input == ' ') {
				inverse(0);
				clrscr();
				background();
				helpTitle();
				newfgcolor(226);
				controllerOption();
				howToPlayOption();
				aboutUsOption();
				backOption();
				break;
			}
		}
	}
}
void aboutUsMenu() {
	char input;
	// printer overskrift
	fgcolor(13);
	gotoxy(95, 5);
	printf("  ______   __                              __            __    __");
	gotoxy(95, 6);
	printf(
			" /      \\ /  |                            /  |          /  |  /  | ");
	gotoxy(95, 7);
	printf(
			"/$$$$$$  |$$ |____    ______   __    __  _$$ |_         $$ |  $$ |  _______ ");
	gotoxy(95, 8);
	printf(
			"$$ |__$$ |$$      \\  /      \\ /  |  /  |/ $$   |        $$ |  $$ | /       |");
	gotoxy(95, 9);
	printf(
			"$$    $$ |$$$$$$$  |/$$$$$$  |$$ |  $$ |$$$$$$/         $$ |  $$ |/$$$$$$$/  ");
	gotoxy(95, 10);
	printf(
			"$$$$$$$$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |  $$ | __       $$ |  $$ |$$      \\");
	gotoxy(95, 11);
	printf(
			"$$ |  $$ |$$ |__$$ |$$ \\__$$ |$$ \\__$$ |  $$ |/  |      $$ \\__$$ | $$$$$$  | ");
	gotoxy(95, 12);
	printf(
			"$$ |  $$ |$$    $$/ $$    $$/ $$    $$/   $$  $$/       $$    $$/ /     $$/ ");
	gotoxy(95, 13);
	printf(
			"$$/   $$/ $$$$$$$/   $$$$$$/   $$$$$$/     $$$$/         $$$$$$/  $$$$$$$/");

	// Print box to
	gotoxy(70, 22);
	printf("%c", 201);
	repeat(205, 129);
	printf("%c", 187);
	for (int i = 22 + 1; i < 61 - 1; i++) {
		gotoxy(70, i);
		printf("%c", 186);
		gotoxy(200, i);
		printf("%c\n", 186);
	}
	gotoxy(70, 60);
	printf("%c", 200);
	repeat(205, 129);
	printf("%c", 188);

	gotoxy(120, 65);
	printf("Press SPACE to go back");
	while (1) {

		if (uart_get_count() > 0) {
			input = uart_get_char();
			uart_clear();
			if (input == ' ') {
				inverse(0);
				clrscr();
				background();
				helpTitle();
				newfgcolor(226);
				controllerOption();
				howToPlayOption();
				aboutUsOption();
				backOption();
				break;
			}
		}

	}
}

//Opstions

void controllerOption() {
	gotoxy(110, 22);
	printf("%c", 201);
	repeat(205, 50);
	printf("%c", 187);
	for (int i = 22 + 1; i < 27 - 1; i++) {
		gotoxy(110, i);
		printf("%c", 186);
		gotoxy(161, i);
		printf("%c\n", 186);
	}
	gotoxy(110, 26);
	printf("%c", 200);
	repeat(205, 50);
	printf("%c", 188);

	// text
	gotoxy(111, 23);
	printf("     .-. .-. . . .-. .-. .-. .   .   .-. .-.      ");
	gotoxy(111, 24);
	printf("     |   | | |\\|  |  |(  | | |   |   |-  |(       ");
	gotoxy(111, 25);
	printf("     `-' `-' ' `  '  ' ' `-' `-' `-' `-' ' '      ");

}
void howToPlayOption() {
	gotoxy(110, 29);
	printf("%c", 201);
	repeat(205, 50);
	printf("%c", 187);
	for (int i = 29 + 1; i < 34 - 1; i++) {
		gotoxy(110, i);
		printf("%c", 186);
		gotoxy(161, i);
		printf("%c\n", 186);
	}
	gotoxy(110, 33);
	printf("%c", 200);
	repeat(205, 50);
	printf("%c", 188);

	// text
	gotoxy(111, 30);
	printf("  . . .-. . . .   .-. .-.   .-. .   .-. . . .-.   ");
	gotoxy(111, 31);
	printf("  |-| | | | | |    |  | |   |-' |   |-|  |   .'   ");
	gotoxy(111, 32);
	printf("  ' ` `-' `.'.'    '  `-'   '   `-' ` '  `   .    ");

}
void aboutUsOption() {
	gotoxy(110, 36);
	printf("%c", 201);
	repeat(205, 50);
	printf("%c", 187);
	for (int i = 36 + 1; i < 41 - 1; i++) {
		gotoxy(110, i);
		printf("%c", 186);
		gotoxy(161, i);
		printf("%c\n", 186);
	}
	gotoxy(110, 40);
	printf("%c", 200);
	repeat(205, 50);
	printf("%c", 188);

	// text
	gotoxy(111, 37);
	printf("          .-. .-. .-. . . .-.   . . .-.           ");
	gotoxy(111, 38);
	printf("          |-| |(  | | | |  |    | | `-.           ");
	gotoxy(111, 39);
	printf("          ` ' `-' `-' `-'  '    `-' `-'           ");

}
void backOption() {
	gotoxy(110, 43);
	printf("%c", 201);
	repeat(205, 50);
	printf("%c", 187);
	for (int i = 43 + 1; i < 48 - 1; i++) {
		gotoxy(110, i);
		printf("%c", 186);
		gotoxy(161, i);
		printf("%c\n", 186);
	}
	gotoxy(110, 47);
	printf("%c", 200);
	repeat(205, 50);
	printf("%c", 188);

	// text
	gotoxy(111, 44);
	printf("                 .-. .-. .-. . .                  ");
	gotoxy(111, 45);
	printf("                 |(  |-| |   |<                   ");
	gotoxy(111, 46);
	printf("                 `-' ` ' `-' ' `                  ");

}

void helpTitle() {
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

}
void help_screen() {

	background();
	helpTitle();
	newfgcolor(226);
	controllerOption();
	howToPlayOption();
	aboutUsOption();
	backOption();

	helpSelect();

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
				printf(
						"                                                                          ");
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
				clrscr();
				background();
				mainFrame(1, 1, 270, 75);
				gameTitle();
				mainOptions();
				break;
			}
		}
	}
}

