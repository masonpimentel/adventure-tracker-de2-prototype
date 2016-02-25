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
	EASTEREGGS,
	PASTTRIPS,
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

/* past trips menu constants */

#define PAST_TRIP_BACK_X1	650
#define PAST_TRIP_BACK_Y1	380

#define PAST_TRIP_BACK_X2	800
#define PAST_TRIP_BACK_Y2	480

/* past trips prev and next buttons */

#define PREV_X1				75
#define PREV_Y1				75
#define PREV_X2				212
#define PREV_Y2				125

#define NEXT_X1				237
#define NEXT_Y1				75
#define NEXT_X2				375
#define NEXT_Y2				125

/* polling speed */

#define WALKING_X1			62
#define WALKING_Y1			180
#define WALKING_X2			163
#define WALKING_Y2			205

#define BIKING_X1			174
#define BIKING_Y1			180
#define BIKING_X2			274
#define BIKING_Y2			205

#define SKIING_X1			287
#define SKIING_Y1			180
#define SKIING_X2			386
#define SKIING_Y2			205

#define WALKING				0
#define BIKING				1
#define SKIING				2

#define WALKING_DELAY		3000000
#define BIKING_DELAY		2000000
#define SKIING_DELAY		1000000


int GetNextMenu(Point p);
void MainMenu(void);
void NewTrip(void);
void PastTrips(void);
void EasterEggs(void);
void DrawMainMenu();
void ChangeSpeed(Point p, int *redrawButtons);


#endif /* MENUS_H_ */


