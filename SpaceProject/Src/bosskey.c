/*
 * bosskey.c
 *
 *  Created on: 13. jun. 2021
 *      Author: Marius Mainz
 */
#include <stdio.h>
#include "main.h"
#define ESC 0x1B
#include "Excellutex.h"
#include "charset.h"
#include "30010_io.h"
#include "ansi.h"
#include <time.h>
#include <string.h>
#include "GameController.h"
#include <stdlib.h>
/*
void bosskey() {
	clrscr();

	RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
	enableTimer();
	TIM2->ARR = 639999; // Set reload value for 64x10^3 HZ - 1 (1/100 second)
	setPrescaler(0); // prescale value
	TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	NVIC_EnableIRQ(TIM2_IRQn);

	for (int i = 1; i < 65; i++) {
		gotoxy(2, i);
		printf("%02d ", i);
	}
	gotoxy(6, 1);
	printf("#include <stdio.h>");
	gotoxy(6, 2);
	printf("#include <stdlib.h>");
	gotoxy(6, 3);
	printf("#include \"not_a_bosskey.h\"");

	int8_t count = 0;
	char char1[] =
			"//This following code is very important for this company, it will make us very rich and powerful!      ";

	uint8_t j = 0;
	uint8_t c = 0;
	uint32_t t = 0;
	uint8_t stop = 0;
	while (1) {
		if (!timer.sec++) {
			t++;
			if (t == 200) {
				gotoxy(j + 6, 5 + c);
				printf("%c", char1[j]);
				j++;

				if (j > 96) {
					c++;
					j = 0;
					stop++;
				}
				t = 0;
			}

		}
		if (stop == 60) {
			break;
		}
	}
}
*/
