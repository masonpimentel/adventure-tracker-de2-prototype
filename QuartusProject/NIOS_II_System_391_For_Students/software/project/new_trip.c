/*
 * new_trip.c
 *
 *  Created on: 2016-02-22
 *      Author: Manolito
 */

#include "new_trip.h"
#include "graphics.h"
#include "menus.h"
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

void DrawGpsData(char* time, char* latitude, char* longitude, char* altitude, char* logname, int curMode, int *redrawButtons,
		int *initializing)
{
	int backgroundColor;
	DrawString(80, 300, time, strlen(time), BLACK, GRAY);
	DrawString(225, 300, latitude, strlen(latitude), BLACK, GRAY);
	DrawString(225, 395, longitude, strlen(longitude), BLACK, GRAY);
	DrawString(80, 395, altitude, strlen(altitude), BLACK, GRAY);
	DrawString(205, 110, logname, strlen(logname), BLACK, GRAY);

	if (*redrawButtons == 1) {
		//walking
		if (curMode == WALKING)
			backgroundColor = GREEN;
		else
			backgroundColor = BLUE;
		DrawButton(62,170,163,215, "Walking", sizeof("Walking"), WHITE, backgroundColor);
		//biking
		if (curMode == BIKING)
			backgroundColor = GREEN;
		else
			backgroundColor = BLUE;
		DrawButton(174,170,274,215, "Biking", sizeof("Biking"), WHITE, backgroundColor);
		//skiing
		if (curMode == SKIING)
			backgroundColor = GREEN;
		else
			backgroundColor = BLUE;
		DrawButton(287,170,386,215, "Skiing", sizeof("Skiing"), WHITE, backgroundColor);

		*redrawButtons = 0;
	}
	//cover up the "please insert sd card" if it's still there
	FilledRectangle(50,431,625,480, DARK_GREEN);

	//if no fix yet
	//printf("initializing is %lf, %d\n", *initializing, *initializing);
	if (*initializing == 1) {
		DrawString(80,235, "Attempting to get a fix...", sizeof("Attempting to get a fix..."), RED, GRAY);
		printf("here!\n");
	}
	else
	{
		//cover up the attempting box
		FilledRectangle(70,220,380,270, GRAY);
	}
}

void DrawGpsMenu(int *redraw)
{
	/* set the screen background */
	FilledRectangle(0,0,800,480, DARK_GREEN);
	DrawButton(0, 0, 800, 40, "Logging trip data...", sizeof ("Logging trip data..."), WHITE, BLACK);

	/* draw data screen and position screen */
	DrawButton(20, 50, 400, 430, "", 0, BLACK, GRAY);
	DrawButton(400, 50, 780, 430, "", 0, BLACK, SADDLE_BROWN);
	VLine(400, 50, 380, BLACK);
	Triangle(585, 235, 595, 235, 590, 245, BLACK);
	Fill(590, 240, BLACK, BLACK);

	/* Back to main menu button */
	DrawButton(650, 430, 800, 480, "End Trip", sizeof("End Trip")-1, BLACK, BURLY_WOOD);

	/* draw the GPS data labels*/
	DrawGpsLabels();
	*redraw = 0;
}



