#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 	// Input/output library for this course
#include "main.h"

#define ESC 0x1B

#define SINGLEPLAYER 1;
#define MULTIPLEAYER 2;
#define HELP 3;

int main(void) {
	uart_init(2000000);
	clrscr();
	lcd_init();
	//test();

	mainMenu();


	//lcd_update(buffer, 1);
	while (1) {
	}

}
