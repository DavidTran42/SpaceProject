#include "GameController.h"
#include <time.h>
#include <stdlib.h>
#include "30010_io.h"
#include "ansi.h"
#define ESC 0x1B
#include "spaceship.h"


//////////////////////// CLOCK & TIMER //////////////////////////

struct clockTimer timer; // Global timer

void enableTimer() {
	TIM2->CR1 |= 0x0001;
}

void disableTimer() {
	TIM2->CR1 &= 0x0000;
}

void setPrescaler(int32_t s) {
	TIM2->PSC = s;
}

void TIM2_IRQHandler(void) {

	timer.sec100++;

	if (timer.sec100 >= 100) {
		timer.sec++;
		timer.sec100 = 0;
	}

	if (timer.sec >= 60) {
		timer.min++;
		timer.sec = 0;
	}

	if (timer.min >= 60) {
		timer.hour++;
		timer.min = 0;
	}

	TIM2->SR &= ~0x0001;
}

void setUpTimer() {
	RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
	enableTimer();
	TIM2->ARR = 639999; // Set reload value for 64x10^3 HZ - 1 (1/100 second)
	setPrescaler(0); // prescale value
	TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	NVIC_EnableIRQ(TIM2_IRQn);
}
//////////////////////////////////////////////////////////////////

void initGame(uint16_t borderWidth, uint16_t borderHeight, int gameMode) {
	struct vector ship[4] = {0}; // More ships due to power ups
	struct asteroid asteroid[16] = {0};
	struct vector bullet1[20] = {0}; struct vector bullet2[20] = {0}; // More ships due to power ups
	uint8_t gameLevel = 1; // Starting level
	struct joystick controls; // For joystick support
	int gameloop = 1, hearts = 3, score = 0;
	int bulletListSize = sizeof(bullet1)/sizeof(bullet1[0]), shipListSize = sizeof(ship)/sizeof(ship[0]);
	char input;

	clrscr(); // clear screen

	background();

	srand(time(NULL)); // Initialization for randomizer. Only done once

	// Make game window

	// Initialize the ships positions
	initializeShips(gameMode, ship, borderWidth, borderHeight);

	// Add controls to ship
	controls = addJoystick();

	// Start timer
	setUpTimer();

	// Game loop
	while(gameloop) {
		controls.right = GPIOC->IDR & (0x0001 << 0);
		// printf("%d", controls.right);
		controls.up = GPIOA->IDR & (0x0001 << 4);
		controls.center = GPIOB->IDR & (0x0001 << 5);
		controls.left = GPIOC->IDR & (0x0001 << 1);
		controls.down = GPIOB->IDR & (0x0001 << 0);

		if (uart_get_count() > 0 || controls.right || controls.left || controls.down
				|| controls.up || controls.center) {
			input = uart_get_char();
			uart_clear(); // Might need to put it outside the if statement
			updateShipPos(input, &ship[0], controls, borderWidth, borderHeight);

			// print ship
			// print_ship1(&ship[0]);

			makeBullet(input, &bullet1[0], &ship[0], bulletListSize, controls);
		}

		if (timer.sec++ && timer.sec % 30 == 0) {
			makeAsteroid(&asteroid[0], borderWidth, borderHeight);
		}
		for(int i = 0; i < bulletListSize; i++) {
			if (bullet1[i].x != 0) {
				printf("i: %d",i);
				printf("bullet_x: %d, bullet_y: %d\n", bullet1[i].x, bullet1[i].y);
				bullet1[i].x += 1;
				if(bullet1[i].x == borderWidth) {
					bullet1[i].x = 0, bullet1[i].y = 0;
				}
			}
		}
	}
}

void checkCollisionWithAsteroid() {

}

void updateShipPos(char input, struct vector *shipptr, struct joystick controls, uint16_t borderWidth, uint16_t borderHeight) {
	// Player 1 controls
	if ((input == 'a' || controls.left) && shipptr->x > 1) {
		(shipptr->x)--;
	}
	if ((input == 'w' || controls.up) && shipptr->y > 1) {
		(shipptr->y)--;
	}
	if ((input == 'd' || controls.right) && shipptr->x < borderWidth) {
		(shipptr->x)++;
	}
	if ((input == 's' || controls.down) &&  shipptr->y < borderHeight) {
		(shipptr->y)++;
	}
}

void initializeShips(int gameMode, struct vector *shipptr, uint16_t borderWidth, uint16_t borderHeight) {
	// Initialize the ships positions
	if (gameMode == 2) { // Multiplayer
		shipptr->x = 0, shipptr->y = borderHeight/3;
		shipptr += 2;
		shipptr->x = 0, shipptr->y = (borderHeight/3)*2;


		// Draw the ship in the game window

	} else { // Singleplayer
		shipptr->x = 0, shipptr->y = borderHeight/2;
		// Draw the ships in the game window

	}
}

struct joystick addJoystick() {
	// Joystick
	RCC->AHBENR |= RCC_AHBPeriph_GPIOA; // Enable clock for GPIO Port A
	RCC->AHBENR |= RCC_AHBPeriph_GPIOB; // Enable clock for GPIO Port B
	RCC->AHBENR |= RCC_AHBPeriph_GPIOC; // Enable clock for GPIO Port C

	GPIOC->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 –
	GPIOC->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
	GPIOC->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 -
	uint16_t right = GPIOC->IDR & (0x0001 << 0); //Read from pin PC0

	GPIOA->MODER &= ~(0x00000003 << (4 * 2)); // Clear mode register
	GPIOA->MODER |= (0x00000000 << (4 * 2)); // Set mode register (0x00 –
	GPIOA->PUPDR &= ~(0x00000003 << (4 * 2)); // Clear push/pull register
	GPIOA->PUPDR |= (0x00000002 << (4 * 2)); // Set push/pull register (0x00 -
	uint16_t up = GPIOA->IDR & (0x0001 << 4); //Read from pin PA4

	GPIOB->MODER &= ~(0x00000003 << (5 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000000 << (5 * 2)); // Set mode register (0x00 –
	GPIOB->PUPDR &= ~(0x00000003 << (5 * 2)); // Clear push/pull register
	GPIOB->PUPDR |= (0x00000002 << (5 * 2)); // Set push/pull register (0x00 -
	uint16_t center = GPIOB->IDR & (0x0001 << 5); //Read from pin PB5

	GPIOC->MODER &= ~(0x00000003 << (1 * 2)); // Clear mode register
	GPIOC->MODER |= (0x00000000 << (1 * 2)); // Set mode register (0x00 –
	GPIOC->PUPDR &= ~(0x00000003 << (1 * 2)); // Clear push/pull register
	GPIOC->PUPDR |= (0x00000002 << (1 * 2)); // Set push/pull register (0x00 -
	uint16_t left = GPIOC->IDR & (0x0001 << 1); //Read from pin PC5

	GPIOB->MODER &= ~(0x00000003 << (0 * 2)); // Clear mode register
	GPIOB->MODER |= (0x00000000 << (0 * 2)); // Set mode register (0x00 –
	GPIOB->PUPDR &= ~(0x00000003 << (0 * 2)); // Clear push/pull register
	GPIOB->PUPDR |= (0x00000002 << (0 * 2)); // Set push/pull register (0x00 -
	uint16_t down = GPIOB->IDR & (0x0001 << 0); //Read from pin PA4

	struct joystick controls = { left, right, up, down, center };

	return controls;
}


void makeBullet(char input, struct vector *bulletptr, struct vector *ship, int bListSize, struct joystick controls) {
	if (input == ' ' || controls.center) {
		// Function to shoot a bullet
		for(int i = 0; i < bListSize; i++) {
			if (bulletptr->x == 0 && bulletptr->y == 0) {
				bulletptr->x = ship->x + 1;
				bulletptr->y = ship->y;
				break;
			}
		}
	}
}

// Given the size of the asteroid, make a random asteroid
void makeAsteroid(struct asteroid *asteroidptr, uint16_t borderWidth, uint16_t borderHeight) {
	uint8_t r = rand() % borderHeight;
	uint8_t size = rand() % 3;

	if (size == 2) {
		if(r < 3) { // Ensures that the asteroid will be spawned correctly
			r = 3;
		} else if (r > borderHeight - 3) {
			r = borderHeight - 3;
		}
		asteroidptr->x = borderWidth + 3;
		asteroidptr->y = borderHeight - 3;
	} else if (size == 1) {
		if(r < 2) { // Ensures that the asteroid will be spawned correctly
			r = 2;
		} else if (r > borderHeight - 2) {
			r = borderHeight - 2;
		}
		asteroidptr->x = borderWidth + 2;
		asteroidptr->y = borderHeight - 2;
	} else {
		if(r < 1) { // Ensures that the asteroid will be spawned correctly
			r = 1;
		} else if (r > borderHeight - 1) {
			r = borderHeight - 1;
		}
		asteroidptr->x = borderWidth + 1;
		asteroidptr->y = borderHeight - 1;
	}
}

void bosskey() {
	clrscr();

	/*RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
	enableTimer();
	TIM2->ARR = 639999; // Set reload value for 64x10^3 HZ - 1 (1/100 second)
	setPrescaler(0); // prescale value
	TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	NVIC_EnableIRQ(TIM2_IRQn);*/

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
			if (t == 800) {
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
/*
void life_score(uint8_t buffer[512]){
	uint32_t score = 0;
	uint8_t life = 3;

	//life remaining
	if (single_player){
	lcd_write_string(buffer, "LIVES REMAINING: ***", 1);

		if (hit && life == 3){
		life--;
		lcd_write_string(buffer, "LIVES REMAINING: ** ", 1);
		}
		else if (hit && life == 2){
		life--;
		lcd_write_string(buffer, "LIVES REMAINING: *  ", 1);
		}
		else if (hit && life == 1){
			life--;
			lcd_write_string(buffer, "GAME OVER!   GAME OVER!  ", 1);
			lcd_update(buffer, 1);
			blink(1);
			lcd_write_string(buffer, "SCORE: " + score, 3);
		}

		if (collision_asteroid_1){
			score += 100;
		}
		else if (collision_asteroid_2){
			score += 500;
		}
		else if (collision_asteroid_3){
			score += 1000;
		}
	}


	//score
}
*/


void lcd_update(uint8_t buffer[512], uint8_t line) {

	RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
	enableTimer();
	TIM2->ARR = 639999; // Set reload value for 64x10^3 HZ - 1 (1/100 second)
	setPrescaler(0); // prescale value
	TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	NVIC_EnableIRQ(TIM2_IRQn);

	uint32_t t = 0;
	while (1) {
		if (!timer.sec++) {
			t++;
		}
		if (t == 800) {
			for (int i = (line - 1) * 128; i <= line * 128 - 1; i++) {
				if (i == (line - 1) * 128) {
					buffer[line * 128 - 1] = buffer[(line - 1) * 128];
				}
				buffer[i] = buffer[i + 1];
			}
			lcd_push_buffer(buffer);
			t = 0;
		}
	}
}
