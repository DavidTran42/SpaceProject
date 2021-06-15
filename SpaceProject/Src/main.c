#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 	// Input/output library for this course
#include "ansi.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GameController.h"

#define ESC 0x1B

#define SINGLEPLAYER 1;
#define MULTIPLEAYER 2;
#define HELP 3;




int main(void) {
	uart_init(921600);
	clrscr();
	uint16_t buffer[512] = { 0 };

	//lcd_init();
	//test();

	//shiptest();
	//bosskey();

	//uint16_t borderWidth = 270, borderHeight = 75;
	//initGame(borderWidth, borderHeight, 1);

	//uint16_t borderWidth = 270, borderHeight = 75;
	//initGame(borderWidth, borderHeight, 1);


	mainMenu();


	/*background();
	struct asteroid asteroid1;			//Test asteroids
	asteroid1.pos.x = 20;
	asteroid1.pos.y = 20;

	struct asteroid asteroid2;
	asteroid2.pos.x = 40;
	asteroid2.pos.y = 20;

	struct asteroid asteroid3;
	asteroid3.pos.x = 60;
	asteroid3.pos.y = 20;

	small_asteroid(&asteroid1);
	medium_asteroid(&asteroid2);
	large_asteroid(&asteroid3);
	*/


	//lcd_update(buffer, 1);

	while (1) {
	}

}
