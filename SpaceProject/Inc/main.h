/*
 * main.h
 *
 *  Created on: 11. jun. 2021
 *      Author: Marius Mainz
 */


#ifndef MAIN_H_
#define MAIN_H_
void test();

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "30010_io.h"
#include "ansi.h"


#endif /* MAIN_H_ */

void mainFrame(int16_t x1,int16_t y1,int16_t x2,int16_t y2);
void mainOptions();
void gameTitle();
void mainMenu();
void optionsSelect();
void guitGameMenu();
void singleplayerMenu();
void multiplayerMenu();
void helpMenu();
void newfgcolor(uint8_t forground);
void newbgcolor(uint8_t background);
void blackHole(int32_t a, int32_t b);


//help menu:
void controllerMenu();
void howToPlayMenu();
void aboutUsMenu();
void backMenu();
void controllerOption();
void howToPlayOption();
void aboutUsOption();
void backOption();
void helpSelect();






