#include "GameController.h"
#include <time.h>
#include "30010_io.h"
#define ESC 0x1B
#include "spaceship.h"
#include "ansi.h"
#include "background.h"

//////////////////////// CLOCK & TIMER //////////////////////////

struct clockTimer timer; // Global timer

void enableTimer() {
	TIM2->CR1 |= 0x0001;
}

void disableTimer() {
	TIM2->CR1 &= 0x0000;
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
	TIM2->PSC = 0; // prescale value
	TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	NVIC_EnableIRQ(TIM2_IRQn);
}
//////////////////////////////////////////////////////////////////

void initGame(uint16_t borderWidth, uint16_t borderHeight, int gameMode) {

	uint8_t bulletListSize = 20, asteroidListSize = 10, gameLevel = 1,
			gameloop = 1, hearts = 3, score = 0,
			r = rand() % borderHeight, type = rand() % 3;
	uint16_t t, l, g;
	struct vector ship[4] = { 0 }; // More ships due to power ups
	struct asteroid asteroid[10] = { 0 };
	struct bullet bullet1[20] = { 0 };
	struct bullet bullet2[20] = { 0 };
	struct joystick controls; // For joystick support

	char input;

	clrscr(); // clear screen

	srand(time(NULL)); // Initialization for randomizer. Only done once

	// Make game window
	game_background();

	// Initialize the ships positions
	initializeShips(gameMode, ship, borderWidth, borderHeight);

	// Draw the intial ships
	if (gameMode == 2) {
		print_ship2(ship[2]);

		// Add controls to ship 2
		controls = addJoystick();
	}
	print_ship1(ship[0]);

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
			updateShipPos(input, &ship[0], borderWidth, borderHeight);

			// print ship
			stars_only(); //updating stars
			print_ship1(ship[0]);

			update_pixels_ship(&ship[0]);

			// Update if multiplayer
			if (gameMode == 2) {
				clear_ship1(ship[2]);
				updateShip2Pos(&ship[2], controls, borderWidth,
						borderHeight);
				print_ship2(ship[2]);
				makeBullet2(controls, &bullet2[0],ship[2], bulletListSize);
			}

			// printf("shipx: %d, shipy: %d",ship->x, ship->y);

			makeBullet1(input, &bullet1[0], ship[0], bulletListSize);
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
						bullet1[k].alive = 0;
					}
				}
			}
		}

		if (g > 2000) {
			g = 0;

			for (int i = 0; i < asteroidListSize; i++) {

				if (asteroid[i].alive) {
					gotoxy(asteroid[i].pos.x, asteroid[i].pos.y);

					// Check collision against ship
					if (abs(asteroid[i].pos.x - ship[0].x) < 9
							&& abs(asteroid[i].pos.y - ship[0].y) < 6) {
						if (checkCollisionWithAsteroid(ship[0], asteroid[i])) {
							asteroid[i].alive = 0;
						}
					}

					// Check collision against bullet
					for(int j = 0; j < bulletListSize; j++) {
						if(abs(asteroid[i].pos.x - bullet1[j].pos.x) < 9
								&& abs(asteroid[i].pos.y - bullet1[j].pos.y) < 6) {
							if (checkHit(bullet1[j],asteroid[i])) {
								asteroid[i].alive = 0;
								bullet1[j].alive = 0;
							}
						}
					}

					if (asteroid[i].size == 2) {
						clear_small_asteroid(&asteroid[i]);
					} else if (asteroid[i].size == 4) {
						clear_medium_asteroid(&asteroid[i]);
					} else {
						clear_large_asteroid(&asteroid[i]);

					}

					resetbgcolor();

					if (asteroid[i].alive) {
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
						for (int j = 0; j < asteroid[i].amountOfPoints; j++) {
							asteroid[i].points[j].x -= 1;
						}
					}
				}
			}
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

bool checkCollisionWithAsteroid(struct vector ship, struct asteroid asteroid) {
	int wide = 7, height = 3;

	for (int i = 0; i < asteroid.amountOfPoints; i++) {
		if (ship.x + 1 > asteroid.points[i].x && ship.x - wide < asteroid.points[i].x
				&& ship.y + height > asteroid.points[i].y && ship.y - height < asteroid.points[i].y) {
			return 1;
		}
	}
	return 0;
}

bool checkHit(struct bullet bullet, struct asteroid asteroid) {
	for (int i = 0; i < asteroid.amountOfPoints; i++) {
		if (bullet.pos.x == asteroid.points[i].x
				&& bullet.pos.y == asteroid.points[i].y) {
			return 1;
		}
	}
	return 0;
}

void updateShipPos(char input, struct vector *shipptr,
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

void updateShip2Pos(struct vector *shipptr,
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

void makeBullet1(char input, struct bullet *bulletptr, struct vector ship,
		int bListSize) {
	if (input == ' ') {
// Function to shoot a bullet
		for (int i = 0; i < bListSize; i++, bulletptr++) {
			if (!(bulletptr->alive)) {
				bulletptr->pos.x = ship.x + 5;
				bulletptr->pos.y = ship.y;
				bulletptr->vel.x = 1;
				bulletptr->alive = 1;

				break;
			}
		}
	}
}

void makeBullet2(struct joystick controls, struct bullet *bulletptr, struct vector ship,
		int bListSize) {
	if (controls.center) {
// Function to shoot a bullet
		for (int i = 0; i < bListSize; i++, bulletptr++) {
			if (!(bulletptr->alive)) {
				bulletptr->pos.x = ship.x + 5;
				bulletptr->pos.y = ship.y;
				bulletptr->vel.x = 1;
				bulletptr->alive = 1;

				break;
			}
		}
	}
}

// Given the size of the asteroid, make a random asteroid
void makeAsteroid(struct asteroid *asteroidptr, uint16_t borderWidth,
		uint16_t borderHeight, uint8_t aListSize, uint8_t type, uint8_t r) {

	int16_t x;

	for (int i = 0; i < aListSize; i++, asteroidptr++) {
		if (!(asteroidptr->alive)) {

			// Give asteroid it's size and y-spawn
			if (type == 2) {
				asteroidptr->size = 8;
				if (r < 11) { // Ensures that the asteroid will be spawned correctly in y-axis
					r = 11;
				} else if (r > borderHeight - 11) {
					r = borderHeight - 11;
				}
				x = borderWidth - asteroidptr->size;
				asteroidptr->amountOfPoints = 36;

				// Assign each indivial contact points of asteroid
				asteroidptr->points[0].x = x + 8, asteroidptr->points[0].y = r;
				asteroidptr->points[1].x = x - 8, asteroidptr->points[1].y = r;
				asteroidptr->points[2].x = x + 8, asteroidptr->points[2].y = r
						+ 1;
				asteroidptr->points[3].x = x - 8, asteroidptr->points[3].y = r
						+ 1;
				asteroidptr->points[4].x = x + 8, asteroidptr->points[4].y = r
						- 1;
				asteroidptr->points[5].x = x - 8, asteroidptr->points[5].y = r
						- 1;

				asteroidptr->points[6].x = x - 2, asteroidptr->points[6].y = r
						+ 5;
				asteroidptr->points[7].x = x - 1, asteroidptr->points[7].y = r
						+ 5;
				asteroidptr->points[8].x = x, asteroidptr->points[8].y = r + 5;
				asteroidptr->points[9].x = x + 1, asteroidptr->points[9].y = r
						+ 5;
				asteroidptr->points[10].x = x + 2, asteroidptr->points[10].y = r
						+ 5;
				asteroidptr->points[11].x = x - 2, asteroidptr->points[11].y = r
						- 5;
				asteroidptr->points[12].x = x - 1, asteroidptr->points[12].y = r
						- 5;
				asteroidptr->points[13].x = x, asteroidptr->points[13].y = r - 5;
				asteroidptr->points[14].x = x + 1, asteroidptr->points[14].y = r
						- 5;
				asteroidptr->points[15].x = x + 2, asteroidptr->points[15].y = r
						- 5;

				asteroidptr->points[16].x = x - 5, asteroidptr->points[16].y = r
						+ 4;
				asteroidptr->points[17].x = x - 4, asteroidptr->points[17].y = r
						+ 4;
				asteroidptr->points[18].x = x - 3, asteroidptr->points[18].y = r
						+ 4;
				asteroidptr->points[19].x = x + 3, asteroidptr->points[19].y = r
						+ 4;
				asteroidptr->points[20].x = x + 4, asteroidptr->points[20].y = r
						+ 4;
				asteroidptr->points[21].x = x + 5, asteroidptr->points[21].y = r
						+ 4;
				asteroidptr->points[22].x = x - 5, asteroidptr->points[22].y = r
						- 4;
				asteroidptr->points[23].x = x - 4, asteroidptr->points[23].y = r
						- 4;
				asteroidptr->points[24].x = x - 3, asteroidptr->points[24].y = r
						- 4;
				asteroidptr->points[25].x = x + 3, asteroidptr->points[25].y = r
						- 4;
				asteroidptr->points[26].x = x + 4, asteroidptr->points[26].y = r
						- 4;
				asteroidptr->points[27].x = x + 5, asteroidptr->points[27].y = r
						- 4;

				asteroidptr->points[28].x = x + 7, asteroidptr->points[28].y = r
						+ 2;
				asteroidptr->points[29].x = x + 7, asteroidptr->points[29].y = r
						- 2;
				asteroidptr->points[30].x = x - 7, asteroidptr->points[30].y = r
						+ 2;
				asteroidptr->points[31].x = x - 7, asteroidptr->points[31].y = r
						- 2;
				asteroidptr->points[32].x = x + 6, asteroidptr->points[32].y = r
						+ 3;
				asteroidptr->points[33].x = x + 6, asteroidptr->points[33].y = r
						- 3;
				asteroidptr->points[34].x = x - 6, asteroidptr->points[34].y = r
						+ 3;
				asteroidptr->points[35].x = x - 6, asteroidptr->points[35].y = r
						- 3;
			} else if (type == 1) {
				asteroidptr->size = 4;
				if (r < 7) { // Ensures that the asteroid will be spawned correctly
					r = 7;
				} else if (r > borderHeight - 7) {
					r = borderHeight - 7;
				}
				x = borderWidth - asteroidptr->size;
				asteroidptr->amountOfPoints = 20;

				// Assign each indivial contact points of asteroid
				asteroidptr->points[0].x = x + 4, asteroidptr->points[0].y = r;
				asteroidptr->points[1].x = x - 4, asteroidptr->points[1].y = r;
				asteroidptr->points[2].x = x + 4, asteroidptr->points[2].y = r
						+ 1;
				asteroidptr->points[3].x = x - 4, asteroidptr->points[3].y = r
						+ 1;
				asteroidptr->points[4].x = x + 4, asteroidptr->points[4].y = r
						- 1;
				asteroidptr->points[5].x = x - 4, asteroidptr->points[5].y = r
						- 1;

				asteroidptr->points[6].x = x - 2, asteroidptr->points[6].y = r
						+ 3;
				asteroidptr->points[7].x = x - 1, asteroidptr->points[7].y = r
						+ 3;
				asteroidptr->points[8].x = x, asteroidptr->points[8].y = r + 3;
				asteroidptr->points[9].x = x + 1, asteroidptr->points[9].y = r
						+ 3;
				asteroidptr->points[10].x = x + 2, asteroidptr->points[10].y = r
						+ 3;
				asteroidptr->points[11].x = x - 2, asteroidptr->points[11].y = r
						- 3;
				asteroidptr->points[12].x = x - 1, asteroidptr->points[12].y = r
						- 3;
				asteroidptr->points[13].x = x, asteroidptr->points[13].y = r - 3;
				asteroidptr->points[14].x = x + 1, asteroidptr->points[14].y = r
						- 3;
				asteroidptr->points[15].x = x + 2, asteroidptr->points[15].y = r
						- 3;

				asteroidptr->points[16].x = x + 3, asteroidptr->points[16].y = r
						- 2;
				asteroidptr->points[17].x = x + 3, asteroidptr->points[17].y = r
						+ 2;
				asteroidptr->points[18].x = x - 3, asteroidptr->points[18].y = r
						- 2;
				asteroidptr->points[19].x = x - 3, asteroidptr->points[19].y = r
						+ 2;
			} else {
				asteroidptr->size = 2;
				if (r < 3) { // Ensures that the asteroid will be spawned correctly
					r = 3;
				} else if (r > borderHeight - 3) {
					r = borderHeight - 3;
				}

				x = borderWidth - asteroidptr->size;
				asteroidptr->amountOfPoints = 8;

				// Assign each indivial contact points of asteroid
				asteroidptr->points[0].x = x + 2, asteroidptr->points[0].y = r;
				asteroidptr->points[1].x = x - 2, asteroidptr->points[1].y = r;
				asteroidptr->points[2].x = x + 1, asteroidptr->points[2].y = r
						- 1;
				asteroidptr->points[3].x = x + 1, asteroidptr->points[3].y = r
						+ 1;
				asteroidptr->points[4].x = x - 1, asteroidptr->points[4].y = r
						- 1;
				asteroidptr->points[5].x = x - 1, asteroidptr->points[5].y = r
						+ 1;
				asteroidptr->points[6].x = x, asteroidptr->points[6].y = r - 1;
				asteroidptr->points[7].x = x, asteroidptr->points[7].y = r + 1;
			}

			asteroidptr->pos.x = x;
			asteroidptr->pos.y = r;
			asteroidptr->alive = 1;
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

		uint8_t j = 0, c = 0, t = 0, stop = 0;
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
	TIM2->PSC = 0; // prescale value
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

