#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 	// Input/output library for this course
#include "ansi.h"
#include "GameController.h"

#include "main.h"
#define ESC 0x1B

#define SINGLEPLAYER 1;
#define MULTIPLEAYER 2;
#define HELP 3;

int main(void)
{
	//uint16_t borderWidth = 270, borderHeight = 75;
	uart_init(921600);
	//lcd_init();
	//initGame(borderWidth, borderHeight, 1);





int main(void) {
	uart_init(921600);
	clrscr();
	//lcd_init();
	//test();
	background();
	resetbgcolor();

	//mainMenu();
	while(1){}

}
