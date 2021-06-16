/*
 * ansi.h
 *
 *  Created on: Jun 4, 2021
 *      Author: davidtran
 */

#ifndef ANSI_H_
#define ANSI_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>


#endif /* ANSI_H_ */


void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void resetbgcolor();
void color(uint8_t foreground, uint8_t background);
void clrscr();
void clreol();
void gotoxy(uint16_t x, uint16_t y);
void underline(uint8_t on);
void blink(uint8_t on);
void inverse(uint8_t on);
void repeat(int c, int count);

int32_t expand(int32_t i);
int32_t calcSin(int32_t i);
int32_t calcCos(int32_t i);



