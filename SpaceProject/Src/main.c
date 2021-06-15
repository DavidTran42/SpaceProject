#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 	// Input/output library for this course
#include "ansi.h"
#include "GameController.h"
#include "main.h"
#define ESC 0x1B

#define SINGLEPLAYER 1;
#define MULTIPLEAYER 2;
#define HELP 3;





int main(void) {
	uart_init(2000000);
	clrscr();
	uint16_t buffer[512] = { 0 };

	//lcd_init();
	//test();
	//background();
	//shiptest();
	//bosskey();

	//uint16_t borderWidth = 270, borderHeight = 75;
	//initGame(borderWidth, borderHeight, 1);


	mainMenu();
	while(1){}


}
