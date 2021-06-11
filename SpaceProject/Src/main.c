#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 	// Input/output library for this course
#include "ansi.h"
#include "GameController.h"
#define SINGLEPLAYER 1;
#define MULTIPLEAYER 2;
#define HELP 3;


int main(void)
{
	//uint16_t borderWidth = 270, borderHeight = 75;
	uart_init(921600);
	//lcd_init();
	//initGame(borderWidth, borderHeight, 1);

	mainMenu();
	while(1){}
}
