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
#include "30010_io.h"


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

int64_t expand(int64_t i);
int32_t calcSin(int32_t i);
int32_t calcCos(int32_t i);
void printFix(int32_t i);
int32_t expand2(int32_t i);

void turnOn(GPIO_TypeDef *pin, uint32_t pinnum);
void turnOff(GPIO_TypeDef *pin, uint32_t pinnum);

void lcd_write_string(uint8_t buffer[512], char *slice, uint8_t line);
void lcd_write_string2(uint8_t buffer[512], char *slice, uint8_t line);

