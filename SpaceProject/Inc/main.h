/*
 * main.h
 *
 *  Created on: 11. jun. 2021
 *      Author: Marius Mainz
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#ifndef MAIN_H_
#define MAIN_H_
typedef struct vector_t v;
void test();

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "charset.h"
#include "30010_io.h"
#include "GameController.h"


#endif /* MAIN_H_ */

void mainFrame(int16_t x1,int16_t y1,int16_t x2,int16_t y2);
void mainOptions();
void gameTitle();
void mainMenu();
void newbgcolor(uint8_t background);




