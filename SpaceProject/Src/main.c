#include "mainMenu.h"
#include "main.h"

#define ESC 0x1B

#define SINGLEPLAYER 1;
#define MULTIPLEAYER 2;
#define HELP 3;

int main(void) {
	// Initialize UART and LCD
	uart_init(2000000);
	clrscr();
	lcd_init();

	// Show main menu
	mainMenu();

	while (1) {
	}
}
