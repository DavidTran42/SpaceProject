#include "GameController.h"
#include <time.h>
#include <stdlib.h>
#include "30010_io.h"
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
	struct bullet bullet1[20] = { 0 };
	struct bullet bullet2[20] = { 0 };

	uint8_t gameLevel = 1; // Starting level
	struct joystick controls; // For joystick support
	int gameloop = 1, hearts = 3, score = 0;
	uint32_t t, s,l, g;
	uint8_t r = rand() % borderHeight;
	uint8_t type = rand() % 3;
	int bulletListSize = sizeof(bullet1) / sizeof(bullet1[0]), shipListSize =
			sizeof(ship) / sizeof(ship[0]);
	int asteroidListSize = sizeof(asteroid) / sizeof(asteroid[0]);
	char input, input2;

	clrscr(); // clear screen

	srand(time(NULL)); // Initialization for randomizer. Only done once

	// Make game window
	game_background();

	// Initialize the ships positions
	initializeShips(gameMode, ship, borderWidth, borderHeight);
	// Draw the intial ships
	if (gameMode == 2) {
		print_ship2(ship[2]);
	}
	print_ship1(ship[0]);

	// Add controls to ship
	controls = addJoystick();

	// Start timer
	setUpTimer();

	// Game loop
	while (gameloop) {
		t++; // For every interupt, increment
		l++;
		g++;

		controls.right = GPIOC->IDR & (0x0001 << 0);
		controls.up = GPIOA->IDR & (0x0001 << 4);
		controls.center = GPIOB->IDR & (0x0001 << 5);
		controls.left = GPIOC->IDR & (0x0001 << 1);
		controls.down = GPIOB->IDR & (0x0001 << 0);

		if (uart_get_count() > 0 || controls.right || controls.left
				|| controls.down || controls.up || controls.center) {
			input = uart_get_char(); // Might need to put it outside the if statement
			uart_clear();

			// Check if boss key has been pressed
			bosskey(input);

			// Update ships from key press or joystick input
			clear_ship1(ship[0]);
			updateShipPos(input, &ship[0], controls, borderWidth, borderHeight);

			// print ship
			stars_only(); //updating stars
			print_ship1(ship[0]);

			update_pixels_ship(&ship[0]);
			if (gameMode == 2) {
				updateShip2Pos(input2, &ship[2], controls, borderWidth,
						borderHeight);
				print_ship2(ship[2]);
			}

			// printf("shipx: %d, shipy: %d",ship->x, ship->y);

			makeBullet(input, &bullet1[0], &ship[0], bulletListSize, controls);
		}

		// Pause button for testing purposes
		/* if (input == 'p') {
		 disableTimer();
		 int p = 1;
		 while (p) {
		 input = uart_get_char();
		 if (input == 'p') {
		 enableTimer();
		 p = 0;
		 break;
		 }
		 }
		 }*/

		if (l > 10000) {
			l = 0;
			r = rand() % borderHeight;
			type = rand() % 3;
			makeAsteroid(&asteroid[0], borderWidth, borderHeight,
					asteroidListSize, type, r);
		}
		/*
		 if (timer.sec++) {
		 // printf("%d",timer.sec);
		 makeAsteroid(&asteroid[0], borderWidth, borderHeight,
		 asteroidListSize, type, r);
		 r = rand() % borderHeight;
		 type = rand() % 3;
		 }
		 */

		// Update bullets and astroids
		if (t > 2000) {
			t = 0;
			for (int k = 0; k < bulletListSize; k++) {
				if (bullet1[k].pos.x != 0) {
					gotoxy(bullet1[k].pos.x, bullet1[k].pos.y);
					update_bullet(bullet1[k].pos);
					printf(" o");
					bullet1[k].pos.x += bullet1[k].vel.x;
					bullet1[k].pos.y += bullet1[k].vel.y;
					if (bullet1[k].pos.x == borderWidth) {
						bullet1[k].pos.x = 0, bullet1[k].pos.y = 0;
					}
				}
			}
		}

		if (g > 5000) {
			g = 0;

			for (int i = 0; i < asteroidListSize; i++) {

				if (asteroid[i].alive) {
					gotoxy(asteroid[i].pos.x, asteroid[i].pos.y);
					if (asteroid[i].size == 2) {
						clear_small_asteroid(&asteroid[i]);
					} else if (asteroid[i].size == 4) {
						clear_medium_asteroid(&asteroid[i]);
					} else {
						clear_large_asteroid(&asteroid[i]);

					}

					resetbgcolor();

					asteroid[i].pos.x -= 1;

					if (asteroid[i].size == 2) {
						small_asteroid(&asteroid[i]);

						update_small_background(&asteroid[i]);

					} else if (asteroid[i].size == 4) {
						medium_asteroid(&asteroid[i]);
						update_medium_background(&asteroid[i]);
					} else {
						large_asteroid(&asteroid[i]);
						update_large_background(&asteroid[i]);
					}

					// Reset asteroid, so it can be used to make a new one
					if (asteroid[i].pos.x <= asteroid[i].size + 1) {
						if (asteroid[i].size == 2) {
							clear_small_asteroid(&asteroid[i]);
						} else if (asteroid[i].size == 4) {
							clear_medium_asteroid(&asteroid[i]);
						} else {
							clear_large_asteroid(&asteroid[i]);

						}
						asteroid[i].alive = 0;
						if (asteroid[i].pos.y > 63) {
							game_background();
						}

					}
					/*
					 if (checkCollisionWithAsteroid(ship[0], asteroid[i]) > 0) {
					 printf("%d",t);
					 }*/
				}
			}

			// printf("asteroid%d_x = %d, asteroid%d_y = %d\n", i, asteroid[i].pos.x, i, asteroid[i].pos.y);
		}
	}
}

void gravity(struct bullet *bulletptr, struct asteroid *asteroidptr,
		int bListSize, int aListSize, int i, int k) {
	int64_t fx = 0, fy = 0;
	/*
	 for (int e = 0; e < aListSize; e++) {
	 for (int k = 0; k < bListSize; k++) {
	 if (bulletptr[k].pos.x != 0) {
	 gotoxy(bulletptr[k].pos.x, bulletptr[k].pos.y);
	 update_bullet(bulletptr[k].pos);
	 printf(" o");
	 bulletptr[k].pos.x += bulletptr[k].vel.x;
	 bulletptr[k].pos.y += bulletptr[k].vel.y;
	 if (asteroidptr[e].pos.y != 0
	 && asteroidptr[e].pos.x > 0 - asteroidptr[e].size) {
	 if (asteroidptr[e].pos.x <= (0 - asteroidptr[e].size)) {
	 asteroidptr[e].pos.x = 0, asteroidptr[e].pos.y = 0;
	 }

	 <<<<<<< HEAD
	 if (bulletptr[k].pos.x == 270) {
	 bulletptr[k].pos.x = 0, bulletptr[k].pos.y = 0;
	 }*/

	if (asteroidptr->size == 2) {
		fx =
				expand(
						(50000
								/ ((bulletptr[k].pos.x - asteroidptr[i].pos.x)
										* (bulletptr[k].pos.x
												- asteroidptr[i].pos.x))));
		fy =
				expand(
						(50000
								/ ((bulletptr[k].pos.y - asteroidptr[i].pos.y)
										* (bulletptr[k].pos.y
												- asteroidptr[i].pos.y))));

		bulletptr[k].vel.x *= fx;
		bulletptr[k].vel.y *= fy;

	}
	if (asteroidptr->size == 4) {
		fx =
				expand(
						(50000
								/ ((bulletptr[k].pos.x - asteroidptr[i].pos.x)
										* (bulletptr[k].pos.x
												- asteroidptr[i].pos.x))));
		fy =
				expand(
						(50000
								/ ((bulletptr[k].pos.y - asteroidptr[i].pos.y)
										* (bulletptr[k].pos.y
												- asteroidptr[i].pos.y))));

		bulletptr[k].vel.x *= fx;
		bulletptr[k].vel.y *= fy;

	}
	if (asteroidptr->size == 8) {
		fx =
				expand(
						(50000
								/ ((bulletptr[k].pos.x - asteroidptr[i].pos.x)
										* (bulletptr[k].pos.x
												- asteroidptr[i].pos.x))));
		fy =
				expand(
						(50000
								/ ((bulletptr[k].pos.y - asteroidptr[i].pos.y)
										* (bulletptr[k].pos.y
												- asteroidptr[i].pos.y))));

		bulletptr[k].vel.x *= fx;
		bulletptr[k].vel.y *= fy;

	}
	gotoxy(10, 10);
	printf("%04ld,%04ld\n", fx, fy);
	printFix(fx);
	printf(" , ");
	printFix(fy);
}

int checkCollisionWithAsteroid(struct vector ship, struct asteroid asteroid) {
// 0 = false, 1 = size 0, 2 = size 1, 3 = size 2
	int wide = 6, height = 2, wing = 4;
	if (ship.x - wide >= asteroid.pos.x - 11
			&& ship.y - height >= asteroid.pos.y - 8
			&& ship.x <= asteroid.pos.x + 11
			&& ship.y + height <= asteroid.pos.y + 8) {
		if (asteroid.size == 8) {
			// Split the asteroid up to three squares
			if ((ship.x - wide >= asteroid.pos.x - 8
					&& ship.x <= asteroid.pos.x + 8
					&& ship.y - height >= asteroid.pos.y - 1
					&& ship.y + height <= asteroid.pos.y + 1)
					|| (ship.x - wide >= asteroid.pos.x - 5
							&& ship.x <= asteroid.pos.x + 5
							&& ship.y - height >= asteroid.pos.y - 4
							&& ship.y + height <= asteroid.pos.y + 4)
					|| (ship.x - wide >= asteroid.pos.x - 2
							&& ship.x <= asteroid.pos.x + 2
							&& ship.y - height >= asteroid.pos.y - 5
							&& ship.y + height <= asteroid.pos.y + 5)

					// Now check the contact points points
					|| (ship.x == asteroid.pos.x + 7
							&& ship.y == asteroid.pos.y + 2)
					|| (ship.x == asteroid.pos.x + 7
							&& ship.y == asteroid.pos.y - 2)
					|| (ship.x == asteroid.pos.x - 7
							&& ship.y == asteroid.pos.y + 2)
					|| (ship.x == asteroid.pos.x - 7
							&& ship.y == asteroid.pos.y - 2)
					|| (ship.x == asteroid.pos.x + 6
							&& ship.y == asteroid.pos.y + 3)
					|| (ship.x == asteroid.pos.x + 6
							&& ship.y == asteroid.pos.y - 3)
					|| (ship.x == asteroid.pos.x - 6
							&& ship.y == asteroid.pos.y + 3)
					|| (ship.x == asteroid.pos.x - 6
							&& ship.y == asteroid.pos.y - 3)
					|| (ship.x - wide == asteroid.pos.x + 7
							&& ship.y == asteroid.pos.y + 2)
					|| (ship.x - wide == asteroid.pos.x + 7
							&& ship.y == asteroid.pos.y - 2)
					|| (ship.x - wide == asteroid.pos.x - 7
							&& ship.y == asteroid.pos.y + 2)
					|| (ship.x - wide == asteroid.pos.x - 7
							&& ship.y == asteroid.pos.y - 2)
					|| (ship.x - wide == asteroid.pos.x + 6
							&& ship.y == asteroid.pos.y + 3)
					|| (ship.x - wide == asteroid.pos.x + 6
							&& ship.y == asteroid.pos.y - 3)
					|| (ship.x - wide == asteroid.pos.x - 6
							&& ship.y == asteroid.pos.y + 3)
					|| (ship.x - wide == asteroid.pos.x - 6
							&& ship.y == asteroid.pos.y - 3)
					|| (ship.x - wing == asteroid.pos.x + 7
							&& ship.y + height == asteroid.pos.y + 2)
					|| (ship.x - wing == asteroid.pos.x + 7
							&& ship.y + height == asteroid.pos.y - 2)
					|| (ship.x - wing == asteroid.pos.x - 7
							&& ship.y + height == asteroid.pos.y + 2)
					|| (ship.x - wing == asteroid.pos.x - 7
							&& ship.y + height == asteroid.pos.y - 2)
					|| (ship.x - wing == asteroid.pos.x + 6
							&& ship.y + height == asteroid.pos.y + 3)
					|| (ship.x - wing == asteroid.pos.x + 6
							&& ship.y + height == asteroid.pos.y - 3)
					|| (ship.x - wing == asteroid.pos.x - 6
							&& ship.y + height == asteroid.pos.y + 3)
					|| (ship.x - wing == asteroid.pos.x - 6
							&& ship.y + height == asteroid.pos.y - 3)
					|| (ship.x - wing == asteroid.pos.x + 7
							&& ship.y - height == asteroid.pos.y + 2)
					|| (ship.x - wing == asteroid.pos.x + 7
							&& ship.y - height == asteroid.pos.y - 2)
					|| (ship.x - wing == asteroid.pos.x - 7
							&& ship.y - height == asteroid.pos.y + 2)
					|| (ship.x - wing == asteroid.pos.x - 7
							&& ship.y - height == asteroid.pos.y - 2)
					|| (ship.x - wing == asteroid.pos.x + 6
							&& ship.y - height == asteroid.pos.y + 3)
					|| (ship.x - wing == asteroid.pos.x + 6
							&& ship.y - height == asteroid.pos.y - 3)
					|| (ship.x - wing == asteroid.pos.x - 6
							&& ship.y - height == asteroid.pos.y + 3)
					|| (ship.x - wing == asteroid.pos.x - 6
							&& ship.y - height == asteroid.pos.y - 3)) {
				return 3;
			}
		} else if (asteroid.size == 4) {
			// Split the asteroid up to two squares and four points
			if ((ship.x - wide >= asteroid.pos.x - 4
					&& ship.x <= asteroid.pos.x + 4
					&& ship.y - height >= asteroid.pos.y - 1
					&& ship.y + height <= asteroid.pos.y + 1)
					|| (ship.x - wide >= asteroid.pos.x - 2
							&& ship.x <= asteroid.pos.x + 2
							&& ship.y - height >= asteroid.pos.y - 3
							&& ship.y + height <= asteroid.pos.y + 3)
					// Now check the contact points
					|| (ship.x == asteroid.pos.x + 3
							&& ship.y == asteroid.pos.y + 2)
					|| (ship.x == asteroid.pos.x + 3
							&& ship.y == asteroid.pos.y - 2)
					|| (ship.x == asteroid.pos.x - 3
							&& ship.y == asteroid.pos.y + 2)
					|| (ship.x == asteroid.pos.x - 3
							&& ship.y == asteroid.pos.y - 2)
					|| (ship.x - wide == asteroid.pos.x + 3
							&& ship.y == asteroid.pos.y + 2)
					|| (ship.x - wide == asteroid.pos.x + 3
							&& ship.y == asteroid.pos.y - 2)
					|| (ship.x - wide == asteroid.pos.x - 3
							&& ship.y == asteroid.pos.y + 2)
					|| (ship.x - wide == asteroid.pos.x - 3
							&& ship.y == asteroid.pos.y - 2)
					|| (ship.x - wing == asteroid.pos.x + 3
							&& ship.y - height == asteroid.pos.y + 2)
					|| (ship.x - wing == asteroid.pos.x + 3
							&& ship.y - height == asteroid.pos.y - 2)
					|| (ship.x - wing == asteroid.pos.x - 3
							&& ship.y - height == asteroid.pos.y + 2)
					|| (ship.x - wing == asteroid.pos.x - 3
							&& ship.y - height == asteroid.pos.y - 2)
					|| (ship.x - wing == asteroid.pos.x + 3
							&& ship.y + height == asteroid.pos.y + 2)
					|| (ship.x - wing == asteroid.pos.x + 3
							&& ship.y + height == asteroid.pos.y - 2)
					|| (ship.x - wing == asteroid.pos.x - 3
							&& ship.y + height == asteroid.pos.y + 2)
					|| (ship.x - wing == asteroid.pos.x - 3
							&& ship.y + height == asteroid.pos.y - 2)) {
				return 2;
			}
		} else if (asteroid.size == 2) {
			if ((ship.x - wide >= asteroid.pos.x - 2
					&& ship.x <= asteroid.pos.x + 2
					&& ship.y - height >= asteroid.pos.y
					&& ship.y + height <= asteroid.pos.y)
					|| (ship.x - wide >= asteroid.pos.x - 1
							&& ship.x <= asteroid.pos.x + 1
							&& ship.y - height >= asteroid.pos.y - 1
							&& ship.y + height <= asteroid.pos.y + 1)) {
				return 1;
			}
		}
	}
	return 0;
}

int checkHit(struct vector bullet, struct asteroid asteroid) {

}

void updateShipPos(char input, struct vector *shipptr, struct joystick controls,
		uint16_t borderWidth, uint16_t borderHeight) {
// Player 1 controls
	if ((input == 'a') && shipptr->x > 1) {
		(shipptr->x)--;
	}
	if ((input == 'w') && shipptr->y > 1) {
		(shipptr->y)--;
	}
	if ((input == 'd') && shipptr->x < borderWidth) {
		(shipptr->x)++;
	}
	if ((input == 's') && shipptr->y < borderHeight) {
		(shipptr->y)++;
	}
}

void updateShip2Pos(char input2, struct vector *shipptr,
		struct joystick controls, uint16_t borderWidth, uint16_t borderHeight) {
// Player 1 controls
	if ((controls.left) && shipptr->x > 1) {
		(shipptr->x)--;
	}
	if ((controls.up) && shipptr->y > 1) {
		(shipptr->y)--;
	}
	if ((controls.right) && shipptr->x < borderWidth) {
		(shipptr->x)++;
	}
	if ((controls.down) && shipptr->y < borderHeight) {
		(shipptr->y)++;
	}
}

// 6 wide and 5 height
void initializeShips(int gameMode, struct vector *shipptr, uint16_t borderWidth,
		uint16_t borderHeight) {
// Initialize the ships positions
	if (gameMode == 2) { // Multiplayer

		shipptr->x = 10, shipptr->y = (borderHeight + 5) / 3;

		shipptr += 2;

		shipptr->x = 10, shipptr->y = ((borderHeight + 5) / 3) * 2;
	} else { // Singleplayer

		shipptr->x = 10, shipptr->y = (borderHeight + 5) / 2;

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

void makeBullet(char input, struct bullet *bulletptr, struct vector *ship,
		int bListSize, struct joystick controls) {
	if (input == ' ' || controls.center) {
// Function to shoot a bullet
		for (int i = 0; i < bListSize; i++) {
			if (!(bulletptr->alive)) {
				bulletptr->pos.x = ship->x + 5;
				bulletptr->pos.y = ship->y;
				bulletptr->vel.x = 1;
				bulletptr->alive = 1;

				break;
			}
			bulletptr++;
		}
	}
}

// Given the size of the asteroid, make a random asteroid
void makeAsteroid(struct asteroid *asteroidptr, uint16_t borderWidth,
		uint16_t borderHeight, uint8_t aListSize, uint8_t type, uint8_t r) {
	for (int i = 0; i < aListSize; i++) {
		if (!(asteroidptr->alive)) {

			// Give asteroid it's size and y-spawn
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

			asteroidptr->pos.x = borderWidth - asteroidptr->size;
			asteroidptr->pos.y = r;
			asteroidptr->alive = 1;

			break;
		}
		asteroidptr++;
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
					game_background();
					break;
				}
			}
		}
	}

}
/*
 void life_score(uint8_t buffer[512]) {
 uint32_t score1 = 0;
 uint32_t score2 = 0;
 uint8_t lives1 = 3;
 uint8_t lives2 = 3;
 char s_score[20] = "SCORE = ";

 //life remaining
 <<<<<<< HEAD
 if (single_player) {
 =======
 if (gameMode == 1) {
 >>>>>>> branch 'master' of https://github.com/DavidTran42/SpaceProject
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

 <<<<<<< HEAD
 if (multiplayer) {
 =======
 if (gameMode == 2) {
 >>>>>>> branch 'master' of https://github.com/DavidTran42/SpaceProject
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

void level_led(uint8_t gameLevel) {
	setUpTimer();
	if (gameLevel == 1) {
		turnOff(GPIOA, 9);
		turnOff(GPIOB, 4);

		while (1) {
			if (timer.sec100 == 1) {
				turnOn(GPIOC, 7);
			} else if (timer.sec100 == 50) {
				turnOff(GPIOC, 7);
			}
		}
	} else if (gameLevel == 2) {
		turnOff(GPIOA, 9);
		while (1) {
			if (timer.sec100 == 1 || timer.sec100 == 50) {
				turnOn(GPIOB, 4);
				turnOn(GPIOC, 7);
			} else if (timer.sec100 == 25 || timer.sec100 == 75) {
				turnOff(GPIOB, 4);
				turnOff(GPIOC, 7);
			}
		}
	} else if (gameLevel == 3) {
		turnOff(GPIOA, 9);
		turnOff(GPIOC, 7);
		while (1) {
			if (timer.sec100 == 1 || timer.sec100 == 25 || timer.sec100 == 50
					|| timer.sec100 == 75) {
				turnOn(GPIOB, 4);
			} else if (timer.sec100 == 13 || timer.sec100 == 37
					|| timer.sec100 == 62 || timer.sec100 == 88) {
				turnOff(GPIOB, 4);
			}
		}
	}
}

