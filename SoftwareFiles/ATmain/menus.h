/*
 * menus.h
 *
 *  Created on: Feb 3, 2016
 *      Author: mitchell
 */

#ifndef MENUS_H_
#define MENUS_H_

#include "touch.h"


/* pointer to current state function */
void (*current_menu_func)(void);
int current_menu_val;

typedef enum
{
	MAINMENU,
	NEWTRIP,
} Menus;


/* main menu constants */
#define NEWTRIP_X1	400
#define NEWTRIP_Y1	50

#define NEWTRIP_X2	750
#define NEWTRIP_Y2	430


#define PASTTRIP_X1	50
#define PASTTRIP_Y1	50

#define PASTTRIP_X2	375
#define PASTTRIP_Y2	225


#define EASTEREGGS_X1	50
#define EASTEREGGS_Y1	225

#define EASTEREGGS_X2	375
#define EASTEREGGS_Y2	430

#define MAIN_MENU_BUTTON_COLOR	BEIGE
#define MAIN_MENU_TEXT_COLOR	BLACK
#define MAIN_MENU_BACKGROUND_COLOR	GREY


/* new trip menu constants */

#define NEWTRIP_BACK_X1		650
#define NEWTRIP_BACK_Y1		380

#define NEWTRIP_BACK_X2		800
#define NEWTRIP_BACK_Y2		480


void GetNextMenu(Point p);
void MainMenu(void);
void NewTrip(void);
void PastTrips(void);
void EasterEggs(void);


#endif /* MENUS_H_ */


