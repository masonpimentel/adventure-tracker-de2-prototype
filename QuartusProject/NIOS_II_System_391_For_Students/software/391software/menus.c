/*
 * menus.c
 *
 *  Created on: Feb 3, 2016
 *      Author: mitchell
 */


#include "menus.h"
#include "touch.h"
#include "graphics.h"
#include "Fill.h"
#include <string.h>
#include "gps.h"

int redraw = 1;


void DrawMainMenu()
{
	FilledRectangle(0, 0, 800, 480, DARK_GREEN);
	DrawButton(0, 0, 800, 40, "Adventure Tracker", sizeof ("Adventure Tracker"), WHITE, BLACK);
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
				redraw = 1;
			}
			else if((p.x > 50) && (p.x < 375) &&
					   (p.y > 255) && (p.y < 430))
			{
				current_menu_func = &EasterEggs;
				redraw = 1;
			}
			break;

		case (NEWTRIP):
			if((p.x > NEWTRIP_BACK_X1) && (p.x < NEWTRIP_BACK_X2) &&
			   (p.y > NEWTRIP_BACK_Y1) && (p.y < NEWTRIP_BACK_Y2))
			{
				current_menu_func = &MainMenu;
				redraw = 1;
			}
			break;
		case (EASTEREGGS):
			if((p.x > 0) && (p.x < 800) &&
			   (p.y > 0) && (p.y < 480))
			{
				current_menu_func = &MainMenu;
				redraw = 1;
			}
			break;

		default:
			break;
	}
}

void MainMenu()
{
	current_menu_val = MAINMENU;
	if(redraw)
	{
		DrawMainMenu();
		redraw = 0;
	}
	int i;
	for(i=0; i<1000; i++);
	Point p = GetRelease();
	GetNextMenu(p);
	current_menu_func();
}


void EasterEggs()
{
	current_menu_val = EASTEREGGS;
	int i;
	for(i=0; i<10; i++)
	{
		TriangleTest();
	}
	Point p = GetRelease();
	GetNextMenu(p);
	current_menu_func();
}

/* this is where we are going to poll the GPS and print the coordinates to screen */

void DrawGpsData(char* time, char* latitude, char* longitude, char* altitude)
{
	DrawString(80, 85, time, strlen(time), BLACK, GRAY);
	DrawString(80, 175, latitude, strlen(latitude), BLACK, GRAY);
	DrawString(80, 270, longitude, strlen(longitude), BLACK, GRAY);
	DrawString(80, 365, altitude, strlen(altitude), BLACK, GRAY);
}

void DrawGpsLabels()
{
	DrawString(60,65,"Time: ", sizeof("Time: ")-1, BLACK, GRAY);
	DrawString(60, 155, "Latitude: ", sizeof("Latitude: ")-1,BLACK, GRAY);
	DrawString(60, 250, "Longitude: ", sizeof("Longitude: ")-1, BLACK, GRAY);
	DrawString(60, 345, "Altitude: ", sizeof("Altitude: ")-1, BLACK, GRAY);
}

void NewTrip()
{
	char time[100];
	char latitude[100];
	char longitude[100];
	char altitude[100];
	//snprintf(time, 100,"4:20: ");
	//snprintf(latitude, 100, "S52* 26' 59.79\"");
	//snprintf(longitude, 100, "N13* 23' 36.11\"");
	//snprintf(altitude, 100, "1180.23m");


	Point p;
	/* update the menu state variable */
	current_menu_val = NEWTRIP;
	if(redraw)
	{

		/* set the screen background */
		FilledRectangle(0,0,800,480, DARK_GREEN);
		DrawButton(0, 0, 800, 40, "Logging trip data...", sizeof ("Logging trip data..."), WHITE, BLACK);

		/* draw data screen and position screen */
		DrawButton(50, 50, 400, 430, "", 0, BLACK, GRAY);
		DrawButton(400, 50, 750, 430, "", 0, BLACK, SADDLE_BROWN);
		VLine(400, 50, 380, BLACK);
		Triangle(565, 240, 585, 240, 575, 220, BLACK);
		Fill(575, 230, BLACK, BLACK);

		/* Back to main menu button */
		DrawButton(650, 430, 800, 480, "Main Menu", sizeof("Main Menu")-1, BLACK, BURLY_WOOD);

		/* draw the GPS data labels*/
		DrawGpsLabels();
		redraw = 0;
	}

	/* delay for debouncing screen */
	int i;
	for(i=0; i<10000; i++);

	char * gpsdat;

	char temp[256];
	while(1)
	{

		gpsdat = getGpsData();
		strcpy(temp, gpsdat);
		printf("%s\n", gpsdat);

		extractGpsTime(temp , time);
		printf("%s\n", time);
		strcpy(temp, gpsdat);
		extractGpsLatitude(temp, latitude);
		printf("%s\n", latitude);
		strcpy(temp, gpsdat);
		extractGpsLongitude(temp, longitude);
		printf("%s\n", longitude);
		strcpy(temp, gpsdat);
		extractGpsAltitude(temp,  altitude);
		printf("%s\n", altitude);
		DrawGpsData(time, latitude, longitude, altitude);

		for(i=0; i<1000000; i++)
		{
			if(ScreenTouched())
			{
				goto touched;
			}
		}
	}
touched:

	p = GetRelease();
	GetNextMenu(p);
	current_menu_func();
}


