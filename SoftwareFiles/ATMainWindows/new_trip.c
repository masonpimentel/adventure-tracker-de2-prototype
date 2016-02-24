/*
 * new_trip.c
 *
 *  Created on: 2016-02-22
 *      Author: Manolito
 */

#include "new_trip.h"
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void DrawGpsLabels()
{
	DrawString(80, 280,"Time: ", sizeof("Time: ")-1, BLACK, GRAY);
	DrawString(225, 280, "Latitude: ", sizeof("Latitude: ")-1,BLACK, GRAY);
	DrawString(225, 375, "Longitude: ", sizeof("Longitude: ")-1, BLACK, GRAY);
	DrawString(80, 375, "Altitude: ", sizeof("Altitude: ")-1, BLACK, GRAY);
	DrawString(160, 90, "Current Trip: ", sizeof("Current Trip: ")-1, NAVY, GRAY);
}

void DrawGpsData(char* time, char* latitude, char* longitude, char* altitude, char* logname)
{
	DrawString(80, 300, time, strlen(time), BLACK, GRAY);
	DrawString(225, 300, latitude, strlen(latitude), BLACK, GRAY);
	DrawString(225, 395, longitude, strlen(longitude), BLACK, GRAY);
	DrawString(80, 395, altitude, strlen(altitude), BLACK, GRAY);
	DrawString(205, 110, logname, strlen(logname), BLACK, GRAY);

	//walking
	DrawButton(62,180,163,205, "Walking", sizeof("Walking"), WHITE, GREEN);
	//biking
	DrawButton(174,180,274,205, "Biking", sizeof("Biking"), WHITE, BLUE);
	//skiing
	DrawButton(287,180,386,205, "Skiing", sizeof("Skiing"), WHITE, NAVY);
	//cover up the "please insert sd card" if it's still there
	FilledRectangle(50,431,625,480, DARK_GREEN);

	//if no fix yet
	//FilledRectangle
}

void DrawGpsMenu(int *redraw)
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
	DrawButton(650, 430, 800, 480, "End Trip", sizeof("End Trip")-1, BLACK, BURLY_WOOD);

	/* draw the GPS data labels*/
	DrawGpsLabels();
	*redraw = 0;
}



