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

	struct vector ship[4] = { 0 }; // More ships due to power ups
	struct asteroid asteroid[10] = { 0 };
	struct vector bullet1[20] = { 0 };
	struct vector bullet2[20] = { 0 }; // More ships due to power ups

	uint8_t gameLevel = 1; // Starting level
	struct joystick controls; // For joystick support
	int gameloop = 1, hearts = 3, score = 0;
	uint8_t r = rand() % borderHeight;
	uint8_t type = rand() % 3;
	int bulletListSize = sizeof(bullet1) / sizeof(bullet1[0]), shipListSize =
			sizeof(ship) / sizeof(ship[0]);
	int asteroidListSize = sizeof(asteroid) / sizeof(asteroid);
	char input;

	clrscr(); // clear screen

	srand(time(NULL)); // Initialization for randomizer. Only done once

	// Make game window
	 background();

	// Initialize the ships positions
	initializeShips(gameMode, ship, borderWidth, borderHeight);

	// Draw the intial ships
	if (gameMode == 2) {
		// print_ship2(ship[2]);
	}
	// print_ship1(ship[0]);

	// Add controls to ship
	controls = addJoystick();

	// Start timer
	setUpTimer();

	// Game loop
	while (gameloop) {
		controls.right = GPIOC->IDR & (0x0001 << 0);
		// printf("%d", controls.right);
		controls.up = GPIOA->IDR & (0x0001 << 4);
		controls.center = GPIOB->IDR & (0x0001 << 5);
		controls.left = GPIOC->IDR & (0x0001 << 1);
		controls.down = GPIOB->IDR & (0x0001 << 0);

		if (uart_get_count() > 0 || controls.right || controls.left
				|| controls.down || controls.up || controls.center) {
			input = uart_get_char();
			uart_clear(); // Might need to put it outside the if statement

			// Check if boss key has been pressed
			bosskey(input);

			// Update ships from key press or joystick input
			updateShipPos(input, &ship[0], controls, borderWidth, borderHeight);

			 //print ship
			print_ship1(ship[0]);

			//printf("shipx: %d, shipy: %d",ship->x, ship->y);

			makeBullet(input, &bullet1[0], &ship[0], bulletListSize, controls);
		}

		if (timer.sec++ && timer.sec % 30 == 0) {
			makeAsteroid(&asteroid[0], borderWidth, borderHeight, asteroidListSize, type, r);
			r = rand() % borderHeight;
			type = rand() % 3;
		}

		// Update bullets and astroids
		for (int i = 0; i < bulletListSize; i++) {
			if (bullet1[i].x != 0) {
				/*printf("i: %d", i);
				printf("bullet_x: %d, bullet_y: %d\n", bullet1[i].x,
						bullet1[i].y);*/
				gotoxy(bullet1[i].x,bullet1[i].y);
				printf("-");
				bullet1[i].x += 1;
				if (bullet1[i].x == borderWidth) {
					bullet1[i].x = 0, bullet1[i].y = 0;
				}
			}
		}
		for (int i = 0; i < asteroidListSize; i++) {
			if (asteroid[i].pos.x != 0) {
				// gotoxy(asteroid[i].pos.x,asteroid[i].pos.y);
				//printf("asteroid%d_x = %d, asteroid%d_y = %d\n", i, asteroid[i].pos.x, i, asteroid[i].pos.y);
				asteroid[i].pos.x -= 1;
			}
			if (asteroid[i].pos.x <= 0 - asteroid[i].size) {
				asteroid[i].pos.x = 0, asteroid[i].pos.y = 0;
			}
		}
	}
}

int checkCollisionWithAsteroid(struct vector ship, struct asteroid *asteroid) {
	if (asteroid->size == 2) {

	} else if (asteroid->size == 1) {

	} else if (asteroid->size == 0) {

	} else {
		return 0;
	}
}

void updateShipPos(char input, struct vector *shipptr, struct joystick controls,
		uint16_t borderWidth, uint16_t borderHeight) {
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
	if ((input == 's' || controls.down) && shipptr->y < borderHeight) {
		(shipptr->y)++;
	}
}

// 6 wide and 5 height
void initializeShips(int gameMode, struct vector *shipptr, uint16_t borderWidth,
		uint16_t borderHeight) {
	// Initialize the ships positions
	if (gameMode == 2) { // Multiplayer

		shipptr->x = 6, shipptr->y = (borderHeight + 5) / 3;

		shipptr += 2;

		shipptr->x = 6, shipptr->y = ((borderHeight + 5) / 3) * 2;
	} else { // Singleplayer

		shipptr->x = 6, shipptr->y = (borderHeight + 5) / 2;

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

void makeBullet(char input, struct vector *bulletptr, struct vector *ship,
		int bListSize, struct joystick controls) {
	if (input == ' ' || controls.center) {
		// Function to shoot a bullet
		for (int i = 0; i < bListSize; i++) {
			if (bulletptr->x == 0 && bulletptr->y == 0) {
				bulletptr->x = ship->x + 5;
				bulletptr->y = ship->y;
				break;
			}
		}
	}
}

// Given the size of the asteroid, make a random asteroid
void makeAsteroid(struct asteroid *asteroidptr, uint16_t borderWidth,
		uint16_t borderHeight, uint8_t aListSize, uint8_t type, uint8_t r) {

	if (type == 2) {
		asteroidptr->size = 8;
		if (r < 11) { // Ensures that the asteroid will be spawned correctly in y-axis
			r = 11;
		} else if (r > borderHeight - 11) {
			r = borderHeight - 11;
		}
	} else if (type == 1) {
		asteroidptr->size = 4;
		if (r < 7) { // Ensures that the asteroid will be spawned correctly
			r = 7;
		} else if (r > borderHeight - 7) {
			r = borderHeight - 7;
		}
	} else {
		asteroidptr->size = 2;
		if (r < 3) { // Ensures that the asteroid will be spawned correctly
			r = 3;
		} else if (r > borderHeight - 3) {
			r = borderHeight - 3;
		}
	}
	for (int i = 0; i < aListSize; i++) {
		if (asteroidptr->pos.x == 0 && asteroidptr->pos.y == 0) {
			asteroidptr->pos.x = borderWidth + asteroidptr->size;
			asteroidptr->pos.y = r;
			break;
		}
	}
}

void bosskey(char input) {
	int pause = 0;
	if (input == 'b') {
		pause = 1;
	}
	if (pause) {
		clrscr();
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

		char char1[] =
				"//This following code is very important for this company, it will make us very rich and powerful!      ";

		uint8_t j = 0;
		uint8_t c = 0;
		uint32_t t = 0;
		uint8_t stop = 0;
		while (pause) {
			if (!timer.sec++) {
				t++;
				if (t == 100) {
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
			if (uart_get_count() > 0) {
				input = uart_get_char();
				if (input == 'b') {
					pause = 0;
					clrscr();
					break;
				}
			}
		}
	}

	/*RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
	 enableTimer();
	 TIM2->ARR = 639999; // Set reload value for 64x10^3 HZ - 1 (1/100 second)
	 setPrescaler(0); // prescale value
	 TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	 NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	 NVIC_EnableIRQ(TIM2_IRQn);*/
}
/*
void life_score(uint8_t buffer[512]) {
	uint32_t score1 = 0;
	uint32_t score2 = 0;
	uint8_t lives1 = 3;
	uint8_t lives2 = 3;
	char s_score[20] = "SCORE = ";

	//life remaining
	if (single_player) {
		lcd_write_string(buffer, "SCORE: ", 3);

		lcd_write_string(buffer, "LIVES REMAINING: ***", 1);

		if (p1_hit && life == 3) {
			lives1--;
			lcd_write_string(buffer, "LIVES REMAINING: ** ", 1);
		} else if (p1_hit && life == 2) {
			lives1--;
			lcd_write_string(buffer, "LIVES REMAINING: *  ", 1);
		} else if (p1_hit && life == 1) {
			lives1--;
			lcd_write_string(buffer, "GAME OVER!   GAME OVER!  ", 1);
			lcd_update(buffer, 1);

		}
	}

	if (multiplayer) {
		lcd_write_string(buffer, "P1 SCORE: ", 3);
		lcd_write_string2(buffer, "P2 SCORE: ", 3);

		lcd_write_string(buffer, "P1: ***", 1);
		lcd_write_string2(buffer, "P2: ***", 1);

		//Player one
		if (p1_hit && lives1 == 3) {
			life--;
			lcd_write_string(buffer, "P1: ** ", 1);
		} else if (p1_hit && lives1 == 2) {
			lives1--;
			lcd_write_string(buffer, "P1: *", 1);
		} else if (p1_hit && lives1 == 1) {
			lives1--;
			lcd_write_string(buffer, "GAME OVER!   GAME OVER!  ", 1);
			lcd_update(buffer, 1);

		}

		//player two
		if (p2_hit && lives2 == 3) {
			life--;
			lcd_write_string2(buffer, "P2: ** ", 1);
		} else if (p1_hit && lives2 == 2) {
			lives2--;
			lcd_write_string2(buffer, "P2: *", 1);
		} else if (p1_hit && lives2 == 1) {
			lives2--;
			lcd_write_string2(buffer, "GAME OVER!   GAME OVER!  ", 1);
			lcd_update(buffer, 1);
		}

	}
	//score player one
	if (p1_collision_asteroid_1) {
		score1 += 100;
		itoa(score1, s_score, 10);
		lcd_write_string(buffer, s_score, 4);

	} else if (collision_asteroid_2) {
		score1 += 200;
		itoa(score1, s_score, 10);
		lcd_write_string(buffer, s_score, 4);
	} else if (collision_asteroid_3) {
		score1 += 500;
		itoa(score1, s_score, 10);
		lcd_write_string(buffer, s_score, 4);
	}

	//Score player two
	if (p2_collision_asteroid_1) {
		score2 += 100;
		itoa(score2, s_score, 10);
		lcd_write_string2(buffer, s_score, 4);

	} else if (collision_asteroid_2) {
		score2 += 200;
		itoa(score2, s_score, 10);
		lcd_write_string2(buffer, s_score, 4);
	} else if (collision_asteroid_3) {
		score2 += 500;
		itoa(score2, s_score, 10);
		lcd_write_string2(buffer, s_score, 4);
	}
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




