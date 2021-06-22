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
			5, settings.gameLoop = 1;
	struct powers powerups[3] = { 0 };
	struct ship ship1, ship2;
	ship1.bulletAmount = 5;
	ship1.hearts = 3;
	ship1.score = 0;
	ship1.bulletSpeed = 2;
	if (gameMode == 2) {
		ship2.bulletAmount = 5; // More ships due to power ups
		ship2.hearts = 3;
		ship2.score = 0;
		ship2.bulletSpeed = 2;
	}
	struct asteroid asteroid[5] = { 0 };
	struct bullet bullet1[10] = { 0 };
	struct bullet bullet2[10] = { 0 };
	struct joystick controls; // For joystick support
	uint8_t r = rand() % borderHeight, type = rand() % 3, buff = rand() % 3;
	uint16_t t, l, g, s, limiter;
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

	lcd_write_string(buffer, "SCORE: ", 3);
	lcd_write_string(buffer, "P1 HP: ***", 1);
	itoa(ship1.score, s_score, 10);
	lcd_write_string(buffer, s_score, 4);

	// Game loop
	while (settings.gameLoop) {
		level_led(settings.gameLevel);
		t++; // For every interupt, increment
		limiter++;

		controls.right = GPIOC->IDR & (0x0001 << 0);
		controls.up = GPIOA->IDR & (0x0001 << 4);
		controls.center = GPIOB->IDR & (0x0001 << 5);
		controls.left = GPIOC->IDR & (0x0001 << 1);
		controls.down = GPIOB->IDR & (0x0001 << 0);

		if (uart_get_count() > 0) {
			input = uart_get_char(); // Might need to put it outside the if statement
			uart_clear();
			if (input == 'm'){
				mainMenu();
			}

			// Check if boss key has been pressed
			bosskey(input);

			// Update ships from key press
			clear_ship1(ship1);
			updateShipPos(input, &ship1, borderWidth, borderHeight);

			stars_only(); //updating stars

			makeBullet1(input, &bullet1[0], ship1, ship1.bulletAmount);
		}

		// Update if multiplayer
		if (gameMode == 2 && limiter > 2000) {
			limiter = 0;
			if (controls.right || controls.left || controls.down || controls.up
					|| controls.center) {
				clear_ship1(ship2);
				updateShip2Pos(&ship2, controls, borderWidth, borderHeight);
				makeBullet2(controls, &bullet2[0], ship2, ship2.bulletAmount);
			}
		}

		// Update bullets and astroids
		if (t > 500) {
			l++;
			g++;
			s++;

			// Update ship with no joystick/keypress
			clear_ship1(ship1);
			updatingShip(&ship1, borderWidth, borderHeight, (1 << 9));
			print_ship1(ship1);
			update_pixels_ship(&ship1);

			if (gameMode == 2) {
				clear_ship1(ship2);
				updatingShip(&ship2, borderWidth, borderHeight, (1 << 9));
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

			// Make random powerup
			if (s > 5000) {
				s = 0;
				buff = rand() % 5;
				setRandomPowerUp(buff, &powerups[0], borderWidth, borderHeight);
			}

			t = 0;

			for (int k = 0; k < ship1.bulletAmount; k++) {
				if (bullet1[k].alive && s % ship1.bulletSpeed == 0) {

					gotoxy(bullet1[k].prev_pos.x >> 14,
							bullet1[k].prev_pos.y >> 14);
					printf(" ");
					gotoxy(bullet1[k].pos.x >> 14, bullet1[k].pos.y >> 14);
					update_bullet(bullet1[k].pos);
					printf("o");
					// For better collision detection
					bullet1[k].prev_pos.x = bullet1[k].pos.x;
					bullet1[k].prev_pos.y = bullet1[k].pos.y;

					// Move bullet & apply gravity
					gravity(1 << 14, 400 << 14, &bullet1[k], 125 << 14,
							35 << 14);
					bullet1[k].pos.x += bullet1[k].vel.x;
					bullet1[k].pos.y += bullet1[k].vel.y;

					if ((bullet1[k].pos.x >> 14) > borderWidth
							|| (bullet1[k].pos.x >> 14) < 0
							|| (bullet1[k].pos.y >> 14) > borderHeight
							|| (bullet1[k].pos.y >> 14) < 0) {
						gotoxy(bullet1[k].prev_pos.x >> 14,
								bullet1[k].prev_pos.y >> 14);

						bullet1[k].alive = 0;
						printf(" ");
					}
				}
			}

			// If multiplayer
			for (int k = 0; k < ship2.bulletAmount; k++) {
				if (bullet2[k].alive && s % ship2.bulletSpeed == 0) {

					gotoxy(bullet2[k].prev_pos.x >> 14,
							bullet2[k].prev_pos.y >> 14);
					printf(" ");
					gotoxy(bullet2[k].pos.x >> 14, bullet2[k].pos.y >> 14);
					update_bullet(bullet2[k].pos);
					printf("-");
					// For better collision detection
					bullet2[k].prev_pos.x = bullet2[k].pos.x;
					bullet2[k].prev_pos.y = bullet2[k].pos.y;

					// Move bullet & apply gravity
					gravity(1<<14,400<<14,&bullet2[k],125<<14,35<<14);
					bullet2[k].pos.x += bullet2[k].vel.x;
					bullet2[k].pos.y += bullet2[k].vel.y;

					if ((bullet2[k].pos.x >> 14) > borderWidth
							|| (bullet2[k].pos.x >> 14) < 0
							|| (bullet2[k].pos.y >> 14) > borderHeight
							|| (bullet2[k].pos.y >> 14) < 0) {
						gotoxy(bullet2[k].prev_pos.x >> 14,
								bullet2[k].prev_pos.y >> 14);

						bullet2[k].alive = 0;
						printf(" ");
					}
				}
			}

			// Updating asteroid
			for (int i = 0; i < settings.amountOfAsteroids; i++) {

				if (asteroid[i].alive) {
					gotoxy(asteroid[i].pos.x >> 14, asteroid[i].pos.y >> 14);

					// Check collision against ship
					if (abs(asteroid[i].pos.x - ship1.pos.x) < (9 << 14)
							&& abs(asteroid[i].pos.y - ship1.pos.y)
									< (8 << 14)) {
						if (checkCollisionWithAsteroid(ship1, asteroid[i])) {
							asteroid[i].alive = 0;
							//update lcd

							if (ship1.hearts == 3) {
								ship1.hearts--;
								lcd_write_string(buffer, "P1 HP: ** ", 1);
							} else if (ship1.hearts == 2) {
								ship1.hearts--;
								lcd_write_string(buffer, "P1 HP: *  ", 1);
							} else if (ship1.hearts == 1) {
								gotoxy(130, 40);
								printf("--- GAME OVER ---");

								ship1.hearts--;
								lcd_write_string(buffer,
										"GAME OVER!   GAME OVER!  ", 1);
								lcd_update(buffer, 1);

							}

						}
					}

					// Check for ship 2
					if (gameMode == 2) {

						lcd_write_string2(buffer, "SCORE: ", 3);
						if (ship2.hearts == 3) {
							lcd_write_string2(buffer, "P2 HP: ***", 1);
						}
						itoa(ship2.score, s_score2, 10);
						lcd_write_string2(buffer, s_score2, 4);

						if (abs(asteroid[i].pos.x - ship2.pos.x) < (9 << 14)
								&& abs(asteroid[i].pos.y - ship2.pos.y)
										< (8 << 14)) {

							if (checkCollisionWithAsteroid(ship2,
									asteroid[i])) {
								asteroid[i].alive = 0;

								if (ship2.hearts == 3) {
									ship2.hearts--;
									lcd_write_string2(buffer, "P2 HP: ** ", 1);
								} else if (ship2.hearts == 2) {
									ship2.hearts--;
									lcd_write_string2(buffer, "P2 HP: *  ", 1);
								} else if (ship2.hearts == 1) {
									gotoxy(130, 40);
									printf("--- GAME OVER ---");
									ship2.hearts--;
									lcd_write_string(buffer,
											"GAME OVER!   GAME OVER!  ", 1);
									lcd_update(buffer, 1);
								}
							}
						}

					}

					// Check collision against bullet
					for (int j = 0; j < ship1.bulletAmount; j++) {

						if (bullet1[j].alive) {
							if (abs(asteroid[i].pos.x - bullet1[j].pos.x)
									< (9 << 14)
									&& abs(asteroid[i].pos.y - bullet1[j].pos.y)
											< (6 << 14)) {
								if (checkHit(bullet1[j], asteroid[i])) {
									asteroid[i].alive = 0;
									bullet1[j].alive = 0;

									gotoxy(bullet1[j].prev_pos.x >> 14,
											bullet1[j].prev_pos.y >> 14);
									printf(" ");
									gotoxy(bullet1[j].pos.x >> 14,
											bullet1[j].pos.y >> 14);
									printf(" ");

									//Count and print score for p1

									if (asteroid[i].size == 2) {
										ship1.score += 1;
									} else if (asteroid[i].size == 4) {
										ship1.score += 2;

									} else {
										ship1.score += 4;
									}
									itoa(ship1.score, s_score, 10);
									lcd_write_string(buffer, s_score, 4);
								}

							}
						}

					}

					// If multiplayer
					if (gameMode == 2) {
						for (int j = 0; j < ship2.bulletAmount; j++) {
							if (bullet2[j].alive) {
								if (abs(asteroid[i].pos.x - bullet2[j].pos.x)
										< (9 << 14)
										&& abs(
												asteroid[i].pos.y
														- bullet2[j].pos.y)
												< (6 << 14)) {
									if (checkHit(bullet2[j], asteroid[i])) {
										asteroid[i].alive = 0;
										bullet2[j].alive = 0;

										gotoxy(bullet2[j].prev_pos.x >> 14,
												bullet2[j].prev_pos.y >> 14);
										printf(" ");
										gotoxy(bullet2[j].pos.x >> 14,
												bullet2[j].pos.y >> 14);
										printf(" ");

										//Count and print score for p2
										if (asteroid[i].size == 2) {
											ship2.score += 1;

										} else if (asteroid[i].size == 4) {
											ship2.score += 2;

										} else {
											ship2.score += 4;

										}
										itoa(ship2.score, s_score2, 10);
										lcd_write_string2(buffer, s_score2, 4);

										gotoxy(bullet2[j].prev_pos.x >> 14,
												bullet2[j].prev_pos.y >> 14);
										printf(" ");
										gotoxy(bullet2[j].pos.x >> 14,
												bullet2[j].pos.y >> 14);
										printf(" ");
									}

								}
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

					if (asteroid[i].alive) {

						// Higher number = slower asteroid.
						if (g > settings.asteroidSpeed) {
							move = true;
							asteroid[i].pos.x -= (1 << 14);
						}

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
						if (asteroid[i].pos.x <= (asteroid[i].size + 1) << 14) {
							if (asteroid[i].size == 2) {
								clear_small_asteroid(&asteroid[i]);
							} else if (asteroid[i].size == 4) {
								clear_medium_asteroid(&asteroid[i]);
							} else {
								clear_large_asteroid(&asteroid[i]);

							}
							asteroid[i].alive = 0;
							if (asteroid[i].pos.y > (63 << 14)) {
								game_background();
							}

						}

						// Higher speed -> slower asteroid
						if (g > settings.asteroidSpeed) {
							for (int j = 0; j < asteroid[i].amountOfPoints;
									j++) {
								asteroid[i].points[j].x -= (1 << 14);
							}
						}
					}
				}
			}

			// Check collision with ship 1
			for (int i = 0; i < 4; i++) {
				if (powerups[i].onField) {
					if (ship1.pos.x == powerups[i].pos.x
							&& ship1.pos.y == powerups[i].pos.y) {
						ship1.powered_up = true;
						if (powerups[i].doubleBullets) {
							ship1.bulletAmount *= 2;
							ship1.db_time = 30000;
							powerups[i].doubleBullets = false;
							ship1.doubleBullets = true;
						} else if (powerups[i].moreHearts) {
							ship1.hearts += 1;
							powerups[i].moreHearts = false;
						} else if (powerups[i].rapidFire) {
							ship1.bulletSpeed = 1;
							ship1.rf_time = 30000;
							powerups[i].rapidFire = false;
							ship1.rapidFire = true;
						}
						powerups[i].onField = false;
					}
				}
			}
			// Check collision with ship 2
			for (int i = 0; i < 4; i++) {
				if (powerups[i].onField) {
					if (ship2.pos.x == powerups[i].pos.x
							&& ship2.pos.y == powerups[i].pos.y) {
						ship2.powered_up = true;
						if (powerups[i].doubleBullets) {
							ship2.bulletAmount *= 2;
							ship2.db_time = 30000;
							powerups[i].doubleBullets = false;
							ship2.doubleBullets = true;
						} else if (powerups[i].moreHearts) {
							ship2.hearts += 1;
							powerups[i].moreHearts = false;
						} else if (powerups[i].rapidFire) {
							ship2.bulletSpeed = 1;
							ship2.rf_time = 30000;
							powerups[i].rapidFire = false;
							ship2.rapidFire = true;
						}
						powerups[i].onField = false;
					}
				}
			}

			// Controlling the asteroids speed
			if (move) {
				g = 0;
				move = false;
			}

			if (ship1.powered_up) {
				if (ship1.db_time > 0 && ship1.doubleBullets) {
					ship1.db_time--;
				} else if (ship1.doubleBullets) {
					ship1.bulletAmount /= 2;
					ship1.doubleBullets = false;
				}
				if (ship1.rf_time > 0 && ship1.rapidFire) {
					ship1.rf_time--;
				} else if (ship1.rapidFire) {
					ship1.bulletSpeed = 2;
					ship1.rapidFire = false;
				}
			}

			if (ship2.powered_up) {
				if (ship2.db_time > 0 && ship2.doubleBullets) {
					ship2.db_time--;
				} else if (ship2.doubleBullets) {
					ship2.bulletAmount /= 2;
					ship2.doubleBullets = false;
				}
				if (ship2.rf_time > 0 && ship2.rapidFire) {
					ship2.rf_time--;
				} else if (ship2.rapidFire) {
					ship2.bulletSpeed = 2;
					ship2.rapidFire = false;
				}
			}
			checkLevelGameUp(&settings);
		}

	}
}

void checkLevelGameUp(struct gameSettings *settings) {
	// Level 2
	if (settings->asteroidCount > 9) {
		settings->gameLevel = 2;
		settings->asteroidSpeed  = 8;
	// Level 3
	} else if (settings->asteroidCount > 24) {
		settings->gameLevel = 3;
		settings->asteroidSpeed  = 4;
	}
	// level 4
	else if (settings->asteroidCount > 49) {
		settings->gameLevel = 4;
		settings-> asteroidSpeed = 1;
	}
}

void setRandomPowerUp(uint8_t buff, struct powers *powerups,
		uint8_t borderWidth, uint8_t borderHeight) {
	uint8_t width = rand() % borderWidth - 1, height = rand() % borderHeight
			- 1;

	for (int i = 0; i < 3; i++, powerups++) {
		if (!powerups->onField) {
			powerups->pos.x = width << 14;
			powerups->pos.y = height << 14;

			if (buff == 2) {
				powerups->rapidFire = true;
			} else if (buff == 1) {
				powerups->moreHearts = true;
			} else {
				powerups->doubleBullets = true;
			}
		}
	}
}

bool checkCollisionWithAsteroid(struct ship ship, struct asteroid asteroid) {
	int wide = 7, height = 3;

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
	int16_t acc = 1 << 12;
// Player 1 controls
	if ((input == 'a') && shipptr->pos.x > 1 << 14) {
		shipptr->vel.x -= acc;
		if (shipptr->vel.x < -(2 << 14)) {
			shipptr->vel.x = -(2 << 14);
		}
		shipptr->pos.x += shipptr->vel.x;
	}
	if ((input == 'w') && shipptr->pos.y > 1 << 14) {
		shipptr->vel.y -= acc;
		if (shipptr->vel.y < -(2 << 14)) {
			shipptr->vel.y = -(2 << 14);
		}
		shipptr->pos.y += shipptr->vel.y;
	}
	if ((input == 'd') && shipptr->pos.x < borderWidth << 14) {
		shipptr->vel.x += acc;
		if (shipptr->vel.x > (2 << 14)) {
			shipptr->vel.x = (2 << 14);
		}
		shipptr->pos.x += shipptr->vel.x;
	}
	if ((input == 's') && shipptr->pos.y < borderHeight << 14) {
		shipptr->vel.y += acc;
		if (shipptr->vel.y > (2 << 14)) {
			shipptr->vel.y = (2 << 14);
		}
		shipptr->pos.y += shipptr->vel.y;
	}
}

void updateShip2Pos(struct ship *shipptr, struct joystick controls,
		uint16_t borderWidth, uint16_t borderHeight) {
	int16_t acc = 1 << 12;
// Player 1 controls
	if ((controls.left) && shipptr->pos.x > 1 << 14) {
		shipptr->vel.x -= acc;
		if (shipptr->vel.x < -(3 << 14)) {
			shipptr->vel.x = -(3 << 14);
		}
		shipptr->pos.x += shipptr->vel.x;
	}
	if ((controls.up) && shipptr->pos.y > 1 << 14) {
		shipptr->vel.y -= acc;
		if (shipptr->vel.y < -(3 << 14)) {
			shipptr->vel.y = -(3 << 14);
		}
		shipptr->pos.y += shipptr->vel.y;
	}
	if ((controls.right) && shipptr->pos.x < borderWidth << 14) {
		shipptr->vel.x += acc;
		if (shipptr->vel.x > (3 << 14)) {
			shipptr->vel.x = (3 << 14);
		}
		shipptr->pos.x += shipptr->vel.x;
	}
	if ((controls.down) && shipptr->pos.y < borderHeight << 14) {
		shipptr->vel.y += acc;
		if (shipptr->vel.y > (3 << 14)) {
			shipptr->vel.y = (3 << 14);
		}
		shipptr->pos.y += shipptr->vel.y;
	}
}

void updatingShip(struct ship *shipptr, uint16_t borderWidth,
		uint16_t borderHeight, int16_t acc) {

	shipptr->pos.x += shipptr->vel.x;
	shipptr->pos.y += shipptr->vel.y;

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
	if (shipptr->vel.x > 0) {
		shipptr->vel.x -= acc;
	}
	if (shipptr->vel.y > 0) {
		shipptr->vel.y -= acc;
	}
	if (shipptr->vel.x < 0) {
		shipptr->vel.x += acc;
	}
	if (shipptr->vel.y < 0) {
		shipptr->vel.y += acc;
	}
}

// 6 wide and 5 height
void initializeShips(int gameMode, struct ship *shipptr, struct ship *shipptr2, uint16_t borderWidth,
		uint16_t borderHeight) {
// Initialize the ships positions
	if (gameMode == 2) { // Multiplayer

		shipptr->pos.x = 10 << 14, shipptr->pos.y = ((borderHeight + 5) / 3)
				<< 14;
		shipptr2->pos.x = 10 << 14, shipptr->pos.y = (((borderHeight + 5) / 3)
				* 2) << 14;
	} else { // Singleplayer

		shipptr->pos.x = 10 << 14, shipptr->pos.y = ((borderHeight + 5) / 2)
				<< 14;

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
 void life_lcd(uint16_t buffer[512], uint8_t *hearts) {
 //uint8_t hearts = 3, score = 0;
 //char s_score[20] = "SCORE = ";

 //life remaining

 //		lcd_write_string(buffer, "SCORE: ", 3);

 //	lcd_write_string(buffer, "LIVES REMAINING: ***", 1);

 if (hearts == 3) {
 hearts--;
 lcd_write_string(&buffer, "LIVES REMAINING: ** ", 1);
 } else if (hearts == 2) {
 hearts--;
 lcd_write_string(&buffer, "LIVES REMAINING: *  ", 1);
 } else if (hearts == 1) {
 hearts--;
 lcd_write_string(&buffer, "GAME OVER!   GAME OVER!  ", 1);
 lcd_update(&buffer, 1);

 }
 }
 s
 if (gameMode == 2) {
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
 score1 += 1;
 itoa(score1, s_score, 10);
 lcd_write_string(buffer, s_score, 4);

 } else if (collision_asteroid_2) {
 score1 += 2;
 itoa(score1, s_score, 10);
 lcd_write_string(buffer, s_score, 4);
 } else if (collision_asteroid_3) {
 score1 += 4;
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
	/*
	 RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
	 enableTimer();
	 TIM2->ARR = 63999999; // Set reload value for 64x10^3 HZ - 1 (1/100 second)
	 TIM2->PSC = 0; // prescale value
	 TIM2->DIER |= 0x0001; // Enable timer 2 interrupts

	 NVIC_SetPriority(TIM2_IRQn, 0); // Can be from 0-15
	 NVIC_EnableIRQ(TIM2_IRQn);
	 */
	turnOff(GPIOA, 9);
	turnOff(GPIOC, 7);
	turnOn(GPIOB, 4);

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

	}
}

void level_led(uint8_t gameLevel) {
	//setUpTimer();
	if (gameLevel == 1) {
		turnOff(GPIOA, 9);
		turnOff(GPIOB, 4);
		turnOn(GPIOC, 7);
		/*
		 while (1) {
		 if (timer.sec100 == 1) {
		 turnOn(GPIOC, 7);
		 } else if (timer.sec100 == 50) {
		 turnOff(GPIOC, 7);
		 }

		 }
		 */
	} else if (gameLevel == 2) {
		turnOff(GPIOA, 9);
		turnOn(GPIOB, 4);
		turnOn(GPIOC, 7);
		/*
		 while (1) {
		 if (timer.sec100 == 1 || timer.sec100 == 50) {
		 turnOn(GPIOB, 4);
		 turnOn(GPIOC, 7);
		 } else if (timer.sec100 == 25 || timer.sec100 == 75) {
		 turnOff(GPIOB, 4);
		 turnOff(GPIOC, 7);
		 }
		 }
		 */
	} else if (gameLevel == 3) {
		turnOff(GPIOA, 9);
		turnOff(GPIOC, 7);
		turnOn(GPIOB, 4);
		/*
		 while (1) {
		 if (timer.sec100 == 1 || timer.sec100 == 25 || timer.sec100 == 50
		 || timer.sec100 == 75) {
		 turnOn(GPIOB, 4);
		 } else if (timer.sec100 == 13 || timer.sec100 == 37
		 || timer.sec100 == 62 || timer.sec100 == 88) {
		 turnOff(GPIOB, 4);
		 }
		 }*/
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

