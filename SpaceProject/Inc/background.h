/*
 * background.h
 *
 *  Created on: 17 Jun 2021
 *      Author: davidtran
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_



#endif /* BACKGROUND_H_ */

void background();
void game_background();
void stars_only();

void large_asteroid(struct asteroid *asteroid3);
void medium_asteroid(struct asteroid *asteroid2);
void small_asteroid(struct asteroid *asteroid1);
void clear_large_asteroid(struct asteroid *asteroid3);
void clear_medium_asteroid(struct asteroid *asteroid2);
void clear_small_asteroid(struct asteroid *asteroid1);
void newbgcolor(uint8_t background);
void newfgcolor(uint8_t forground);
void update_pixels_ship(struct ship *p);
void update_bullet(struct vector bullet);
void update_large_background(struct asteroid *p);
void update_medium_background(struct asteroid *p);
void update_small_background(struct asteroid *p);

