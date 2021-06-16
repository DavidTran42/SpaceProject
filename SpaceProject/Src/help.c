/*
 * help.c
 *
 *  Created on: 16. jun. 2021
 *      Author: Marius Mainz
 */
#define ESC 0x1B
#include "main.h"
#include "ansi.h"
#include "30010_io.h"
#include "GameController.h"

void helpscreen(){
	newfgcolor(214);
	gotoxy(120, 10);
	printf("    )       (     (     ____");
	gotoxy(120, 11);
	printf(" ( /(       )\ )  )\ ) |   / ");
	gotoxy(120, 12);
	newfgcolor(202);
	printf(" )\()) (   (()/( (()/( |  /  ");
	gotoxy(120, 13);
	printf("((_)\  )\   /(_)) /(_))| /   ");
	gotoxy(120, 14);
	printf(" _((_)((_) (_))  (_))  |/    ");
	newfgcolor(9);
	gotoxy(120, 15);
	printf("| || || __|| |   | _ \(      ");
	gotoxy(120, 16);
	printf("| __ || _| | |__ |  _/)\     ");
	gotoxy(120, 17);
	printf("|_||_||___||____||_| ((_)    ");
}

