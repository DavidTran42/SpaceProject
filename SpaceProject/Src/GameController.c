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
	uint8_t buffer[512] = { 0 };
	char s_score[10] = "0", s_score2[10] = "0";
	struct gameSettings settings;
	settings.gameLevel = 1, settings.asteroidSpeed = 16, settings.amountOfAsteroids =
			5, settings.gameLoop = 1, settings.clock = 90;
	struct powers powerups[3] = { 0 };
	struct ship ship1 = { 0 }, ship2 = { 0 };
	ship1.bulletAmount = 5;
	ship1.hearts = 3;
	ship1.fuel = 255;
	ship1.alive = true;
	ship1.bulletSpeed = 2;
	if (gameMode == 2) {
		ship2.bulletAmount = 5; // More ships due to power ups
		ship2.hearts = 3;
		ship2.fuel = 255;
		ship2.alive = true;
		ship2.bulletSpeed = 2;
	}
	struct asteroid asteroid[10] = { 0 };
	struct bullet bullet1[10] = { 0 }, bullet2[10] = { 0 };
	struct joystick controls; // For joystick support
	uint8_t r, type;
	uint16_t t, l, g, s, limiter, deacceleration = (1 << 10);
	bool move = false;
	char input;

	clrscr(); // clear screen

	srand(time(NULL)); // Initialization for randomizer. Only done once

	// Make game window
	game_background();

	// Initialize the ships positions
	initializeShips(gameMode, &ship1, &ship2, borderWidth, borderHeight);

	// Draw the intial ships
	if (gameMode == 2) {
		print_ship2(ship2);

		// Add controls to ship 2
		controls = addJoystick();
	}
	print_ship1(ship1);

	// Start timer
	setUpTimer();

	// Setting up displaying score and health on LCD
	lcd_write_string(buffer, "SCORE: ", 3);
	lcd_write_string(buffer, "P1 HP: ***", 1);
	itoa(ship1.score, s_score, 10);
	lcd_write_string(buffer, s_score, 4);

	if (gameMode == 2) {
		lcd_write_string2(buffer, "SCORE: ", 3);
		lcd_write_string2(buffer, "P2 HP: ***", 1);
		itoa(ship2.score, s_score2, 10);
		lcd_write_string2(buffer, s_score2, 4);
	}

	// Game loop
	while (settings.gameLoop) {
		level_led(settings.gameLevel);
		t++; // For every interupt, increment

		controls.right = GPIOC->IDR & (0x0001 << 0);
		controls.up = GPIOA->IDR & (0x0001 << 4);
		controls.center = GPIOB->IDR & (0x0001 << 5);
		controls.left = GPIOC->IDR & (0x0001 << 1);
		controls.down = GPIOB->IDR & (0x0001 << 0);

		if (uart_get_count() > 0) {
			input = uart_get_char(); // Might need to put it outside the if statement
			uart_clear();
			if (input == 'm') {
				clrscr();
				background();
				mainFrame(1, 1, 270, 75);
				gameTitle();
				mainOptions();
				break;

			}

			// Check if boss key has been pressed
			bosskey(input);

			// Update ships from key press
			if (ship1.alive && ship1.fuel > 0) {
				clear_ship1(ship1);
				updateShipPos(input, &ship1, borderWidth, borderHeight);
				makeBullet1(input, &bullet1[0], ship1, ship1.bulletAmount);
			}
			print_ship1(ship1);
			stars_only(); //updating stars
		}

		// Update if multiplayer
		if (gameMode == 2 && limiter > 4) {
			limiter = 0;
			if (controls.right || controls.left || controls.down || controls.up
					|| controls.center) {

				if (ship2.alive && ship2.fuel > 0) {
					clear_ship1(ship2);
					updateShip2Pos(&ship2, controls, borderWidth, borderHeight);
					makeBullet2(controls, &bullet2[0], ship2,
							ship2.bulletAmount);

				}
				print_ship2(ship2);
			}
		}

		// Update bullets and astroids
		if (t > 500) {
			l++;
			limiter++;
			g++;
			s++;

			// Countdown for level change
			if (s > 40 && settings.clock > 0) {
				s = 0;
				settings.clock--;
			}

			// Display information on top left of the game screen
			fgcolor(15);
			gotoxy(1, 1);
			printf("Current level: %d", settings.gameLevel);
			gotoxy(1, 2);
			printf("Countdown to next level: %d  ", settings.clock);
			gotoxy(1, 3);
			printf(
					"Ship 1 (DoubleBullets): %04d, (RapidFire): %04d, (Fuel): %d  ",
					ship1.db_time, ship1.rf_time, ship1.fuel);
			if (gameMode == 2) {
				gotoxy(1, 4);
				printf(
						"Ship 2 (DoubleBullets): %04d, (RapidFire): %04d, (Fuel): %d  ",
						ship2.db_time, ship2.rf_time, ship2.fuel);
			}

			// Update ship with no joystick/keypress
			if (ship1.alive && (abs(ship1.vel.x) > 0 || abs(ship1.vel.y) > 0)) {
				clear_ship1(ship1);
				updatingShip(&ship1, borderWidth, borderHeight, deacceleration);
				print_ship1(ship1);
				update_pixels_ship(&ship1);
			}

			if (gameMode == 2 && ship2.alive
					&& (abs(ship2.vel.x) > deacceleration
							|| abs(ship2.vel.y) > deacceleration)) {
				clear_ship1(ship2);
				updatingShip(&ship2, borderWidth, borderHeight, deacceleration);
				print_ship2(ship2);
				update_pixels_ship(&ship2);
			}

			// Make random asteroid
			if (l > 500) {
				l = 0;
				r = rand() % borderHeight;
				type = rand() % 3;
				makeAsteroid(&asteroid[0], borderWidth, borderHeight,
						settings.amountOfAsteroids, type, r);
			}

			t = 0;

			drawBullets(ship1, &bullet1[0], borderWidth, borderHeight, s, '1');

			if (gameMode == 2) {
				drawBullets(ship2, &bullet2[0], borderWidth, borderHeight, s,
						'2');
			}

			// Draw powerups
			drawPowerUps(&powerups[0]);

			// Updating asteroid
			for (int i = 0; i < settings.amountOfAsteroids; i++) {

				if (asteroid[i].alive) {

					// Check collision against ship
					if (ship1.alive) {
						if (abs(asteroid[i].pos.x - ship1.pos.x) < (9 << 14)
								&& abs(asteroid[i].pos.y - ship1.pos.y)
										< (8 << 14)) {
							if (checkCollisionWithAsteroid(ship1,
									asteroid[i])) {
								asteroid[i].alive = 0;
								//update lcd

								checkLives(&ship1, &ship2, buffer, '1',
										borderWidth, borderHeight, gameMode,
										&settings);

								// Clear asteroid
								clearAsteroid(&asteroid[i]);
							}
						}
					}

					// Check for ship 2
					if (gameMode == 2 && ship2.alive) {

						if (abs(asteroid[i].pos.x - ship2.pos.x) < (9 << 14)
								&& abs(asteroid[i].pos.y - ship2.pos.y)
										< (8 << 14)) {

							if (checkCollisionWithAsteroid(ship2,
									asteroid[i])) {
								asteroid[i].alive = 0;

								checkLives(&ship2, &ship1, buffer, '2',
										borderWidth, borderHeight, gameMode,
										&settings);

								// Clear asteroid
								clearAsteroid(&asteroid[i]);
							}
						}

					}

					checkCollisionWithBullet(&bullet1[0], &asteroid[i], &ship1,
							s_score, buffer, &settings, &powerups[0]);
					// Display score if hit
					lcd_write_string(buffer, s_score, 4);

					// If multiplayer, check collision with bullet
					if (gameMode == 2) {
						checkCollisionWithBullet(&bullet2[0], &asteroid[i],
								&ship2, s_score2, buffer, &settings,
								&powerups[0]);
						lcd_write_string2(buffer, s_score2, 4);
					}

					if (asteroid[i].alive) {

						// Higher number = slower asteroid.
						if (g > settings.asteroidSpeed) {
							clearAsteroid(&asteroid[i]);
							move = true;
							asteroid[i].pos.x -= (1 << 14);

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
							if (asteroid[i].pos.x
									<= (asteroid[i].size + 1) << 14) {
								clearAsteroid(&asteroid[i]);
								asteroid[i].alive = 0;
								if (asteroid[i].pos.y > (63 << 14)) {
									game_background();
								}

							}

							for (int j = 0; j < asteroid[i].amountOfPoints;
									j++) {
								asteroid[i].points[j].x -= (1 << 14);
							}
						}
					}
				}
			}

			// Check collision with ship 1
			if (ship1.alive) {
				checkCollisionWithPowerUp(&ship1, &powerups[0]);
				checkActivePowerUp(&ship1);
			}

			// Check collision with ship 2
			if (gameMode == 2 && ship2.alive) {
				checkCollisionWithPowerUp(&ship2, &powerups[0]);
				checkActivePowerUp(&ship2);
			}

			// Controlling the asteroids speed
			if (move) {
				g = 0;
				move = false;
			}

			// If enough asteroids are destroyed. Level game up
			checkLevelGameUp(&settings);
		}
	}

}

void checkLives(struct ship *shipptr, struct ship *shipptr2,
		uint8_t buffer[512], char playerNumber, uint16_t borderWidth,
		uint16_t borderHeight, uint8_t gameMode, struct gameSettings *p) {
	if (shipptr->hearts == 3) {
		shipptr->hearts--;
		if (playerNumber == '1') {
			lcd_write_string(buffer, "P1 HP: ** ", 1);
		} else {
			lcd_write_string2(buffer, "P2 HP: ** ", 1);
		}
	} else if (shipptr->hearts == 2) {
		shipptr->hearts--;
		if (playerNumber == '1') {
			lcd_write_string(buffer, "P1 HP: *  ", 1);
		} else {
			lcd_write_string2(buffer, "P2 HP: *  ", 1);
		}
	} else if (shipptr->hearts == 1) {
		if (playerNumber == '1') {
			lcd_write_string(buffer, "P1 HP:    ", 1);
			shipptr->alive = false;
			if (gameMode == 1) {
				makeGameOverScreen(buffer, borderWidth, borderHeight, gameMode,
						p);

			}
		} else {
			lcd_write_string2(buffer, "P2 HP:    ", 1);
			shipptr->alive = false;
		}
		// Clear ship cause there hearts are now 0
		gotoxy(shipptr->pos.x >> 14, shipptr->pos.y >> 14);
		clear_ship1(*shipptr);
		if (gameMode == 2) {
			if (!shipptr->alive && !shipptr2->alive) {
				makeGameOverScreen(buffer, borderWidth, borderHeight, gameMode,
						p);

			}
		}
	}
}

void makeGameOverScreen(uint8_t buffer[512], uint16_t borderWidth,
		uint16_t borderHeight, uint8_t gameMode, struct gameSettings *p) {
	gotoxy(130, 40);
	printf("--- GAME OVER ---");
	gotoxy(122, 48);
	printf("Press 'm' or 'SPACE' for Main Menu");
	lcd_write_string(buffer, "GAME OVER!   GAME OVER!  ", 1);
	lcd_update(buffer, 1, borderWidth, borderHeight, gameMode, p);

}

void drawPowerUps(struct powers *powerup) {
	for (int i = 0; i < 3; i++, powerup++) {
		if (powerup->onField) {
			// Draw powerup
			bgcolor(2);
			gotoxy(powerup->pos.x >> 14, powerup->pos.y >> 14);
			printf("?");
			resetbgcolor();
		}
	}
}

void drawBullets(struct ship ship, struct bullet *bulletptr,
		uint16_t borderWidth, uint16_t borderHeight, uint16_t s,
		char playerNumber) {
	for (int k = 0; k < ship.bulletAmount; k++, bulletptr++) {
		if (bulletptr->alive && s % ship.bulletSpeed == 0) {

			gotoxy(bulletptr->prev_pos.x >> 14, bulletptr->prev_pos.y >> 14);
			update_bullet(bulletptr->prev_pos);
			printf(" ");
			gotoxy(bulletptr->pos.x >> 14, bulletptr->pos.y >> 14);
			update_bullet(bulletptr->pos);
			if (playerNumber == '1') {
				printf("o");
			} else {
				printf("-");
			}
			resetbgcolor();

			// For better collision detection
			bulletptr->prev_pos.x = bulletptr->pos.x;
			bulletptr->prev_pos.y = bulletptr->pos.y;

			// Move bullet & apply gravity
			gravity(1 << 14, 400 << 14, bulletptr, 125 << 14, 35 << 14);
			bulletptr->pos.x += bulletptr->vel.x;
			bulletptr->pos.y += bulletptr->vel.y;

			if ((bulletptr->pos.x >> 14) > borderWidth
					|| (bulletptr->pos.x >> 14) < 0
					|| (bulletptr->pos.y >> 14) > borderHeight
					|| (bulletptr->pos.y >> 14) < 0) {
				gotoxy(bulletptr->prev_pos.x >> 14,
						bulletptr->prev_pos.y >> 14);

				bulletptr->alive = 0;
				printf(" ");
			}
		}
	}
}

void clearAsteroid(struct asteroid *asteroidptr) {
	if (asteroidptr->size == 2) {
		clear_small_asteroid(asteroidptr);
	} else if (asteroidptr->size == 4) {
		clear_medium_asteroid(asteroidptr);
	} else {
		clear_large_asteroid(asteroidptr);
	}
}

void checkActivePowerUp(struct ship *shipptr) {
	if (shipptr->powered_up) {
		if (shipptr->db_time > 0 && shipptr->doubleBullets) {
			shipptr->db_time--;
		} else if (shipptr->doubleBullets) {
			shipptr->bulletAmount = 5;
			shipptr->doubleBullets = false;
		}
		if (shipptr->rf_time > 0 && shipptr->rapidFire) {
			shipptr->rf_time--;
		} else if (shipptr->rapidFire) {
			shipptr->bulletSpeed = 2;
			shipptr->rapidFire = false;
		}

		if (!(shipptr->rapidFire) && !(shipptr->doubleBullets)) {
			shipptr->powered_up = false;
		}
	}
}

void checkCollisionWithPowerUp(struct ship *shipptr, struct powers *powerptr) {
	uint8_t wide = 7, height = 3;

	for (int i = 0; i < 4; i++, powerptr++) {
		if (powerptr->onField) {
			if (shipptr->pos.x + (1 >> 14) > powerptr->pos.x
					&& shipptr->pos.x - (wide << 14) < powerptr->pos.x
					&& shipptr->pos.y + (height << 14) > powerptr->pos.y
					&& shipptr->pos.y - (height << 14) < powerptr->pos.y) {
				if (powerptr->doubleBullets) {
					shipptr->bulletAmount = 10;
					shipptr->db_time = 3000;
					powerptr->doubleBullets = false;
					shipptr->doubleBullets = true;
				} else if (powerptr->moreHearts) {
					// No more hearts if higher or equal 3 hearts
					if (shipptr->hearts < 3) {
						shipptr->hearts += 1;
					}
					powerptr->moreHearts = false;
				} else if (powerptr->rapidFire) {
					shipptr->bulletSpeed = 1;
					shipptr->rf_time = 3000;
					powerptr->rapidFire = false;
					shipptr->rapidFire = true;
				} else if (powerptr->refill) {
					shipptr->fuel = 255;
					powerptr->refill = false;
				}

				// If the hearts doesn't get collected. Let the powerup stay
				powerptr->onField = false;
			}
		}
	}
}

void checkLevelGameUp(struct gameSettings *settings) {
	// Level 2
	if (settings->clock == 0 && settings->gameLevel == 1) {
		settings->clock = 120;
		settings->gameLevel = 2;
		settings->asteroidSpeed = 8;
		// Level 3
	} else if (settings->clock == 0 && settings->gameLevel == 2) {
		settings->clock = 180;
		settings->gameLevel = 3;
		settings->asteroidSpeed = 4;
	}
	// level 4
	else if (settings->clock == 0 && settings->gameLevel == 3) {
		timer.min = 0;
		settings->gameLevel = 4;
		settings->asteroidSpeed = 1;
	}
}

void setPowerUp(uint8_t buff, struct powers *powerups, struct asteroid asteroid) {

	for (int i = 0; i < 4; i++, powerups++) {
		if (!powerups->onField) {
			powerups->pos.x = asteroid.pos.x;
			powerups->pos.y = asteroid.pos.y;
			powerups->onField = true;
			if (buff == 3) {
				powerups->doubleBullets = true;
			} else if (buff == 2) {
				powerups->rapidFire = true;
			} else if (buff == 1) {
				powerups->moreHearts = true;
			} else {
				powerups->refill = true;
			}
			break;
		}
	}
}

void checkCollisionWithBullet(struct bullet *bulletptr,
		struct asteroid *asteroidptr, struct ship *shipptr, char s_score[],
		uint8_t buffer[512], struct gameSettings *settings,
		struct powers *powerptr) {
	for (int j = 0; j < shipptr->bulletAmount; j++, bulletptr++) {

		if (bulletptr->alive) {
			if (abs(asteroidptr->pos.x - bulletptr->pos.x) < (9 << 14)
					&& abs(asteroidptr->pos.y - bulletptr->pos.y) < (6 << 14)) {
				if (checkHit(*bulletptr, *asteroidptr)) {
					asteroidptr->alive = 0;
					bulletptr->alive = 0;
					settings->asteroidCount += 1;

					// Clear asteroid
					clearAsteroid(asteroidptr);

					// Every fifth asteroid destroyed, place powerup
					if (settings->asteroidCount % 5 == 0) {
						uint8_t buff = rand() % 4;
						setPowerUp(buff, powerptr, *asteroidptr);
					}

					gotoxy(bulletptr->prev_pos.x >> 14,
							bulletptr->prev_pos.y >> 14);
					printf(" ");
					gotoxy(bulletptr->pos.x >> 14, bulletptr->pos.y >> 14);
					printf(" ");

					//Count and print score for p1

					if (asteroidptr->size == 2) {
						shipptr->score += 1;
					} else if (asteroidptr->size == 4) {
						shipptr->score += 2;

					} else {
						shipptr->score += 4;
					}
					// For displaying score
					itoa(shipptr->score, s_score, 10);
				}

			}
		}
	}
}

bool checkCollisionWithAsteroid(struct ship ship, struct asteroid asteroid) {
	uint8_t wide = 7, height = 3;

	for (int i = 0; i < asteroid.amountOfPoints; i++) {
		if (ship.pos.x + (1 << 14) > asteroid.points[i].x
				&& ship.pos.x - (wide << 14) < asteroid.points[i].x
				&& ship.pos.y + (height << 14) > asteroid.points[i].y
				&& ship.pos.y - (height << 14) < asteroid.points[i].y) {
			return 1;
		}
	}
	return 0;
}

bool checkHit(struct bullet bullet, struct asteroid asteroid) {
	for (int i = 0; i < asteroid.amountOfPoints; i++) {
		if ((bullet.pos.x >> 14) == (asteroid.points[i].x >> 14)
				&& (bullet.pos.y >> 14) == (asteroid.points[i].y >> 14)) {
			return 1;
		}
		if ((bullet.prev_pos.x >> 14) == (asteroid.points[i].x >> 14)
				&& (bullet.prev_pos.y >> 14) == (asteroid.points[i].y >> 14)) {
			return 1;
		}
	}
	return 0;
}

void updateShipPos(char input, struct ship *shipptr, uint16_t borderWidth,
		uint16_t borderHeight) {
// Player 1 controls
	if ((input == 'a') && shipptr->pos.x > 1 << 14) {
		shipptr->vel.x = -(1 << 14);
		shipptr->fuel--;
	}
	if ((input == 'w') && shipptr->pos.y > 1 << 14) {
		shipptr->vel.y = -(1 << 14);
		shipptr->fuel--;
	}
	if ((input == 'd') && shipptr->pos.x < borderWidth << 14) {
		shipptr->vel.x = +(1 << 14);
		shipptr->fuel--;
	}
	if ((input == 's') && shipptr->pos.y < borderHeight << 14) {
		shipptr->vel.y = +(1 << 14);
		shipptr->fuel--;
	}
}

void updateShip2Pos(struct ship *shipptr, struct joystick controls,
		uint16_t borderWidth, uint16_t borderHeight) {
// Player 2 controls
	if (controls.left && shipptr->pos.x > 1 << 14) {
		shipptr->vel.x = -(1 << 14);
		shipptr->fuel--;
	}
	if (controls.up && shipptr->pos.y > 1 << 14) {
		shipptr->vel.y = -(1 << 14);
		shipptr->fuel--;
	}
	if (controls.right && shipptr->pos.x < borderWidth << 14) {
		shipptr->vel.x = +(1 << 14);
		shipptr->fuel--;
	}
	if (controls.down && shipptr->pos.y < borderHeight << 14) {
		shipptr->vel.y = +(1 << 14);
		shipptr->fuel--;
	}
}

void updatingShip(struct ship *shipptr, uint16_t borderWidth,
		uint16_t borderHeight, int32_t acc) {

	if (shipptr->pos.x < (8 << 14)) {
		shipptr->pos.x = (7 << 14);
	}
	if (shipptr->pos.x > (borderWidth - 2) << 14) {
		shipptr->pos.x = (borderWidth - 1) << 14;
	}

	if (shipptr->pos.y < (4 << 14)) {
		shipptr->pos.y = (3 << 14);
	}
	if (shipptr->pos.y > (borderHeight - 3) << 14) {
		shipptr->pos.y = (borderHeight - 2) << 14;
	}

	// Deacceleration
	if (shipptr->vel.x < 0) {
		shipptr->pos.x += shipptr->vel.x;
		shipptr->vel.x += acc;
		if (shipptr->vel.x > 0) {
			shipptr->vel.x = 0;
		}
	} else if (shipptr->vel.x > 0) {
		shipptr->pos.x += shipptr->vel.x;
		shipptr->vel.x -= acc;
		if (shipptr->vel.x < 0) {
			shipptr->vel.x = 0;
		}
	}
	if (shipptr->vel.y < 0) {
		shipptr->pos.y += shipptr->vel.y;
		shipptr->vel.y += acc;
		if (shipptr->vel.y > 0) {
			shipptr->vel.y = 0;
		}
	} else if (shipptr->vel.y > 0) {
		shipptr->pos.y += shipptr->vel.y;
		shipptr->vel.y -= acc;
		if (shipptr->vel.y < 0) {
			shipptr->vel.y = 0;
		}
	}

	if (abs(shipptr->vel.x) > acc || abs(shipptr->vel.y > acc)) {
		print_flames(shipptr);
	}
}

// 6 wide and 5 height
void initializeShips(int gameMode, struct ship *shipptr, struct ship *shipptr2,
		uint16_t borderWidth, uint16_t borderHeight) {
	shipptr->vel.x = 0;
	shipptr->vel.y = 0;
	shipptr2->vel.x = 0;
	shipptr2->vel.y = 0;
// Initialize the ships positions
	if (gameMode == 2) { // Multiplayer

		shipptr->pos.x = 10 << 14, shipptr->pos.y = (borderHeight / 3) << 14;
		shipptr2->pos.x = 10 << 14, shipptr2->pos.y = shipptr->pos.y * 2;
	} else { // Singleplayer

		shipptr->pos.x = 10 << 14, shipptr->pos.y = (borderHeight / 2) << 14;

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

void makeBullet1(char input, struct bullet *bulletptr, struct ship ship,
		int bListSize) {
	if (input == ' ') {
// Function to shoot a bullet
		for (int i = 0; i < bListSize; i++, bulletptr++) {
			if (!(bulletptr->alive)) {
				bulletptr->pos.x = ship.pos.x + (1 << 14);
				bulletptr->pos.y = ship.pos.y;
				bulletptr->vel.x = (1 << 14);
				bulletptr->vel.y = 0;
				bulletptr->alive = 1;
				break;
			}
		}
	}
}

void makeBullet2(struct joystick controls, struct bullet *bulletptr,
		struct ship ship, int bListSize) {
	if (controls.center) {
// Function to shoot a bullet
		for (int i = 0; i < bListSize; i++, bulletptr++) {
			if (!(bulletptr->alive)) {
				bulletptr->pos.x = ship.pos.x + (1 << 14);
				bulletptr->pos.y = ship.pos.y;
				bulletptr->vel.x = (1 << 14);
				bulletptr->vel.y = 0;
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
				asteroidptr->points[0].x = (x + 8) << 14, asteroidptr->points[0].y =
						r << 14;
				asteroidptr->points[1].x = (x - 8) << 14, asteroidptr->points[1].y =
						r << 14;
				asteroidptr->points[2].x = (x + 8) << 14, asteroidptr->points[2].y =
						(r + 1) << 14;
				asteroidptr->points[3].x = (x - 8) << 14, asteroidptr->points[3].y =
						(r + 1) << 14;
				asteroidptr->points[4].x = (x + 8) << 14, asteroidptr->points[4].y =
						(r - 1) << 14;
				asteroidptr->points[5].x = (x - 8) << 14, asteroidptr->points[5].y =
						(r - 1) << 14;
				asteroidptr->points[6].x = (x - 2) << 14, asteroidptr->points[6].y =
						(r + 5) << 14;
				asteroidptr->points[7].x = (x - 1) << 14, asteroidptr->points[7].y =
						(r + 5) << 14;
				asteroidptr->points[8].x = x << 14, asteroidptr->points[8].y =
						(r + 5) << 14;
				asteroidptr->points[9].x = (x + 1) << 14, asteroidptr->points[9].y =
						(r + 5) << 14;
				asteroidptr->points[10].x = (x + 2) << 14, asteroidptr->points[10].y =
						(r + 5) << 14;
				asteroidptr->points[11].x = (x - 2) << 14, asteroidptr->points[11].y =
						(r - 5) << 14;
				asteroidptr->points[12].x = (x - 1) << 14, asteroidptr->points[12].y =
						(r - 5) << 14;
				asteroidptr->points[13].x = x << 14, asteroidptr->points[13].y =
						(r - 5) << 14;
				asteroidptr->points[14].x = (x + 1) << 14, asteroidptr->points[14].y =
						(r - 5) << 14;
				asteroidptr->points[15].x = (x + 2) << 14, asteroidptr->points[15].y =
						(r - 5) << 14;
				asteroidptr->points[16].x = (x - 5) << 14, asteroidptr->points[16].y =
						(r + 4) << 14;
				asteroidptr->points[17].x = (x - 4) << 14, asteroidptr->points[17].y =
						(r + 4) << 14;
				asteroidptr->points[18].x = (x - 3) << 14, asteroidptr->points[18].y =
						(r + 4) << 14;
				asteroidptr->points[19].x = (x + 3) << 14, asteroidptr->points[19].y =
						(r + 4) << 14;
				asteroidptr->points[20].x = (x + 4) << 14, asteroidptr->points[20].y =
						(r + 4) << 14;
				asteroidptr->points[21].x = (x + 5) << 14, asteroidptr->points[21].y =
						(r + 4) << 14;
				asteroidptr->points[22].x = (x - 5) << 14, asteroidptr->points[22].y =
						(r - 4) << 14;
				asteroidptr->points[23].x = (x - 4) << 14, asteroidptr->points[23].y =
						(r - 4) << 14;
				asteroidptr->points[24].x = (x - 3) << 14, asteroidptr->points[24].y =
						(r - 4) << 14;
				asteroidptr->points[25].x = (x + 3) << 14, asteroidptr->points[25].y =
						(r - 4) << 14;
				asteroidptr->points[26].x = (x + 4) << 14, asteroidptr->points[26].y =
						(r - 4) << 14;
				asteroidptr->points[27].x = (x + 5) << 14, asteroidptr->points[27].y =
						(r - 4) << 14;
				asteroidptr->points[28].x = (x + 7) << 14, asteroidptr->points[28].y =
						(r + 2) << 14;
				asteroidptr->points[29].x = (x + 7) << 14, asteroidptr->points[29].y =
						(r - 2) << 14;
				asteroidptr->points[30].x = (x - 7) << 14, asteroidptr->points[30].y =
						(r + 2) << 14;
				asteroidptr->points[31].x = (x - 7) << 14, asteroidptr->points[31].y =
						(r - 2) << 14;
				asteroidptr->points[32].x = (x + 6) << 14, asteroidptr->points[32].y =
						(r + 3) << 14;
				asteroidptr->points[33].x = (x + 6) << 14, asteroidptr->points[33].y =
						(r - 3) << 14;
				asteroidptr->points[34].x = (x - 6) << 14, asteroidptr->points[34].y =
						(r + 3) << 14;
				asteroidptr->points[35].x = (x - 6) << 14, asteroidptr->points[35].y =
						(r - 3) << 14;
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
				asteroidptr->points[0].x = (x + 4) << 14, asteroidptr->points[0].y =
						r << 14;
				asteroidptr->points[1].x = (x - 4) << 14, asteroidptr->points[1].y =
						r << 14;
				asteroidptr->points[2].x = (x + 4) << 14, asteroidptr->points[2].y =
						(r + 1) << 14;
				asteroidptr->points[3].x = (x - 4) << 14, asteroidptr->points[3].y =
						(r + 1) << 14;
				asteroidptr->points[4].x = (x + 4) << 14, asteroidptr->points[4].y =
						(r - 1) << 14;
				asteroidptr->points[5].x = (x - 4) << 14, asteroidptr->points[5].y =
						(r - 1) << 14;
				asteroidptr->points[6].x = (x - 2) << 14, asteroidptr->points[6].y =
						(r + 3) << 14;
				asteroidptr->points[7].x = (x - 1) << 14, asteroidptr->points[7].y =
						(r + 3) << 14;
				asteroidptr->points[8].x = x << 14, asteroidptr->points[8].y =
						(r + 3) << 14;
				asteroidptr->points[9].x = (x + 1) << 14, asteroidptr->points[9].y =
						(r + 3) << 14;
				asteroidptr->points[10].x = (x + 2) << 14, asteroidptr->points[10].y =
						(r + 3) << 14;
				asteroidptr->points[11].x = (x - 2), asteroidptr->points[11].y =
						(r - 3) << 14;
				asteroidptr->points[12].x = (x - 1), asteroidptr->points[12].y =
						(r - 3) << 14;
				asteroidptr->points[13].x = x << 14, asteroidptr->points[13].y =
						(r - 3) << 14;
				asteroidptr->points[14].x = (x + 1) << 14, asteroidptr->points[14].y =
						(r - 3) << 14;
				asteroidptr->points[15].x = (x + 2) << 14, asteroidptr->points[15].y =
						(r - 3) << 14;
				asteroidptr->points[16].x = (x + 3) << 14, asteroidptr->points[16].y =
						(r - 2) << 14;
				asteroidptr->points[17].x = (x + 3) << 14, asteroidptr->points[17].y =
						(r + 2) << 14;
				asteroidptr->points[18].x = (x - 3) << 14, asteroidptr->points[18].y =
						(r - 2) << 14;
				asteroidptr->points[19].x = (x - 3) << 14, asteroidptr->points[19].y =
						(r + 2) << 14;
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
				asteroidptr->points[0].x = (x + 2) << 14, asteroidptr->points[0].y =
						r << 14;
				asteroidptr->points[1].x = (x - 2) << 14, asteroidptr->points[1].y =
						r << 14;
				asteroidptr->points[2].x = (x + 1) << 14, asteroidptr->points[2].y =
						(r - 1) << 14;
				asteroidptr->points[3].x = (x + 1) << 14, asteroidptr->points[3].y =
						(r + 1) << 14;
				asteroidptr->points[4].x = (x - 1) << 14, asteroidptr->points[4].y =
						(r - 1) << 14;
				asteroidptr->points[5].x = (x - 1) << 14, asteroidptr->points[5].y =
						(r + 1) << 14;
				asteroidptr->points[6].x = x << 14, asteroidptr->points[6].y =
						(r - 1) << 14;
				asteroidptr->points[7].x = x << 14, asteroidptr->points[7].y =
						(r + 1) << 14;
			}

			asteroidptr->pos.x = x << 14;
			asteroidptr->pos.y = r << 14;
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
			printf("%2d ", i);
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
			if (!timer.sec++ && c < 50) {
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
				clrscr();
				c = 0;
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

void lcd_update(uint8_t buffer[512], uint8_t line, uint16_t borderWidth,
		uint16_t borderHeight, int gameMode, struct gameSettings *p) {
	char input;

	turnOff(GPIOA, 9);
	turnOff(GPIOC, 7);
	turnOff(GPIOB, 4);

	while (1) {
		if (timer.sec100 == 1 || timer.sec100 == 25 || timer.sec100 == 50
				|| timer.sec100 == 75) {
			// Scrolling text
			for (int i = (line - 1) * 128; i <= line * 128 - 1; i++) {
				if (i == (line - 1) * 128) {
					buffer[line * 128 - 1] = buffer[(line - 1) * 128];
				}
				buffer[i] = buffer[i + 1];
			}
			lcd_push_buffer(buffer);
		}

		//blinking led
		if (timer.sec100 == 1 || timer.sec100 == 25 || timer.sec100 == 50
				|| timer.sec100 == 75) {
			turnOn(GPIOB, 4);
		} else if (timer.sec100 == 13 || timer.sec100 == 37
				|| timer.sec100 == 62 || timer.sec100 == 88) {
			GPIOB->ODR |= (0x0001 << 4); //Set pin to high (turned off)
		}

		//Bryder ud af game over skærm
		if (uart_get_count() > 0) {
			input = uart_get_char();
			uart_clear();
			if (input == 'm' || input == ' ') {
				mainMenu();
			}

		}

	}
}

void level_led(uint8_t gameLevel) {
	//setUpTimer();
	if (gameLevel == 1) {  //difficulty 1, color green.
		turnOff(GPIOA, 9);
		turnOff(GPIOB, 4);
		turnOn(GPIOC, 7);

	} else if (gameLevel == 2) { //difficulty 2, color yellow.
		turnOff(GPIOA, 9);
		turnOn(GPIOB, 4);
		turnOn(GPIOC, 7);

	} else if (gameLevel == 3) { //difficulty 3, color red
		turnOff(GPIOA, 9);
		turnOff(GPIOC, 7);
		turnOn(GPIOB, 4);
	} else if (gameLevel == 4) { //difficulty 4, color purple
		turnOn(GPIOA, 9);
		turnOff(GPIOC, 7);
		turnOn(GPIOB, 4);
	}

}

void gameover_led() {

	turnOff(GPIOA, 9);
	turnOff(GPIOC, 7);
	turnOn(GPIOB, 4);

	while (1) {
		if (timer.sec100 == 1 || timer.sec100 == 25 || timer.sec100 == 50
				|| timer.sec100 == 75) {
			turnOn(GPIOB, 4);
		} else if (timer.sec100 == 13 || timer.sec100 == 37
				|| timer.sec100 == 62 || timer.sec100 == 88) {
			turnOff(GPIOB, 4);
		}
		if (uart_get_count() > 0) {
			if (uart_get_char() == ' ') {
				mainMenu();
			}
		}
	}
}

