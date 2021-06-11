#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 	// Input/output library for this course
#include "ansi.h"
#include "GameController.h"
#include "main.h"
#define ESC 0x1B







int main(void) {
	uart_init(921600);
	clrscr();
	//lcd_init();
	//test();
	background();
	resetbgcolor();
	while (1) {
	}
}
