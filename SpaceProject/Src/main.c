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

	//shiptest();
	//bosskey();

	//uint16_t borderWidth = 270, borderHeight = 75;
	//initGame(borderWidth, borderHeight, 1);

	mainMenu();

/*
	game_background();

	 struct asteroid asteroid1;			//Test asteroids
	 asteroid1.pos.x = 100;
	 asteroid1.pos.y = 66;

	 struct asteroid asteroid2;
	 asteroid2.pos.x = 120;
	 asteroid2.pos.y = 64;

	 struct asteroid asteroid3;
	 asteroid3.pos.x = 140;
	 asteroid3.pos.y = 62;

	 small_asteroid(&asteroid1);
	 medium_asteroid(&asteroid2);
	 large_asteroid(&asteroid3);
*/

	//lcd_update(buffer, 1);
	while (1) {
	}

}
