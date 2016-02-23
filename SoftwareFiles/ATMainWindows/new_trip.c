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
	DrawString(60,65,"Time: ", sizeof("Time: ")-1, BLACK, GRAY);
	DrawString(60, 155, "Latitude: ", sizeof("Latitude: ")-1,BLACK, GRAY);
	DrawString(60, 250, "Longitude: ", sizeof("Longitude: ")-1, BLACK, GRAY);
	DrawString(60, 345, "Altitude: ", sizeof("Altitude: ")-1, BLACK, GRAY);
	DrawString(220, 160, "Current Trip: ", sizeof("Current Trip: ")-1, NAVY, GRAY);
}

void DrawGpsData(char* time, char* latitude, char* longitude, char* altitude, char* logname)
{
	DrawString(80, 85, time, strlen(time), BLACK, GRAY);
	DrawString(80, 175, latitude, strlen(latitude), BLACK, GRAY);
	DrawString(80, 270, longitude, strlen(longitude), BLACK, GRAY);
	DrawString(80, 365, altitude, strlen(altitude), BLACK, GRAY);
	DrawString(240, 180, logname, strlen(logname), BLACK, GRAY);

	//cover up the "please insert sd card" if it's still there
	FilledRectangle(50,431,625,480, DARK_GREEN);
}



