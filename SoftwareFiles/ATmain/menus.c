/*
 * menus.c
 *
 *  Created on: Feb 3, 2016
 *      Author: mitchell
 */


#include "menus.h"
#include "touch.h"
#include "graphics.h"


void DrawMainMenu()
{
	FilledRectangle(0, 0, 800, 480, DARK_GREEN);
	FilledRectangle(0, 0, 800, 40, BLACK);
	DrawButton(50, 50, 375, 225, "Past Trips", sizeof("Past Trips")-1, BLACK, SADDLE_BROWN);
	DrawButton(50, 255, 375, 430, "Easter Eggs", sizeof("Easter Eggs")-1, BLACK, SADDLE_BROWN);
	DrawButton(400, 50, 750, 430, "New Trip", sizeof("New Trip")-1, BLACK, SADDLE_BROWN);

}

void GetNextMenu(Point p)
{
	switch (current_menu_val)
	{
		case (MAINMENU):
			if((p.x > NEWTRIP_X1) && (p.x < NEWTRIP_X2) &&
			   (p.y > NEWTRIP_Y1) && (p.y < NEWTRIP_Y2))
			{
				current_menu_func = &NewTrip;
			}
			break;

		case (NEWTRIP):
			if((p.x > NEWTRIP_BACK_X1) && (p.x < NEWTRIP_BACK_X2) &&
			   (p.y > NEWTRIP_BACK_Y1) && (p.y < NEWTRIP_BACK_Y2))
			{
				current_menu_func = &MainMenu;
			}
			break;

		default:
			break;
	}
}

void MainMenu()
{
	current_menu_val = MAINMENU;
	DrawMainMenu();
	Point p = GetRelease();
	GetNextMenu(p);
	current_menu_func();
}

/* this is where we are going to poll the GPS and print the coordinates to screen */

void NewTrip()
{
	/* update the menu state variable */
	current_menu_val = NEWTRIP;

	/* set the screen background */
	FilledRectangle(0,0,800,480, DARK_GREEN);
	/* draw a rectangle representing the users location */
	DrawButton(100, 100, 700, 380, "GPS Data Goes Here", sizeof("GPS Data Goes Here")-1, BLACK, SADDLE_BROWN);
	/* Back to main menu button */
	DrawButton(650, 380, 800, 480, "Main Menu", sizeof("Main Menu")-1, BLACK, BURLY_WOOD);

	Point p = GetRelease();
	GetNextMenu(p);
	current_menu_func();
}


