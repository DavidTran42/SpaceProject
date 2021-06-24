/*
 * mainMenu.h
 *
 *  Created on: 22 Jun 2021
 *      Author: davidtran
 */

#ifndef MAINMENU_H_
#define MAINMENU_H_


#endif /* MAINMENU_H_ */

#define ESC 0x1B
#include "ansi.h"
#include "30010_io.h"
#include "GameController.h"
#include "background.h"
#include "help.h"

void mainFrame(int16_t x1,int16_t y1,int16_t x2,int16_t y2);
void mainOptions();
void gameTitle();
void mainMenu();
void optionsSelect();
void guitGameMenu();
void singleplayerMenu();
void multiplayerMenu();
void helpMenu();



