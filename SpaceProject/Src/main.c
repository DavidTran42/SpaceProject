#include "stm32f30x_conf.h" // STM32 config
#include "30010_io.h" 	// Input/output library for this course
#include "ansi.h"
#include "GameController.h"


int main(void) {

	uart_init(921600);

	mainMenu();

	while(1){}
}
