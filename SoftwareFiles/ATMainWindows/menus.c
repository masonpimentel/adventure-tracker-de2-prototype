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
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "paths.h"
#include <math.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"

int redraw = 1;
int logNum = 0;
int initial = 1;

int logExists = 0;

////////////////////////////////////////////////////////////////////////////////////////
//DRAWING
////////////////////////////////////////////////////////////////////////////////////////
void DrawMainMenu()
{
	FilledRectangle(0, 0, 800, 480, DARK_GREEN);
	DrawButton(0, 0, 800, 40, "Adventure Tracker", sizeof ("Adventure Tracker"), WHITE, BLACK);
	DrawButton(50, 50, 375, 225, "Past Trips", sizeof("Past Trips")-1, BLACK, SADDLE_BROWN);
	DrawButton(50, 255, 375, 430, "Easter Eggs", sizeof("Easter Eggs")-1, BLACK, SADDLE_BROWN);
	DrawButton(400, 50, 750, 430, "New Trip", sizeof("New Trip")-1, BLACK, SADDLE_BROWN);
}

void DrawGpsLabels()
{
	DrawString(60,65,"Time: ", sizeof("Time: ")-1, BLACK, GRAY);
	DrawString(60, 155, "Latitude: ", sizeof("Latitude: ")-1,BLACK, GRAY);
	DrawString(60, 250, "Longitude: ", sizeof("Longitude: ")-1, BLACK, GRAY);
	DrawString(60, 345, "Altitude: ", sizeof("Altitude: ")-1, BLACK, GRAY);
	DrawString(220, 160, "Current Trip: ", sizeof("Current Trip: ")-1, NAVY, GRAY);
}

void DrawTripLabels()
{
	DrawString(60,205,"Total Time: ", sizeof("Total Time: ")-1, BLACK, GRAY);
	DrawString(60, 275, "Total Distance: ", sizeof("Total Distance: ")-1,BLACK, GRAY);
	DrawString(60, 345, "Total Change in Altitude: ", sizeof("Total Change in Altitude: ")-1,BLACK, GRAY);
}

void DrawPastTrips()
{
	/* set the screen background */
	FilledRectangle(0,0,800,480, DARK_GREEN);
	DrawButton(0, 0, 800, 40, "Past Trips", sizeof ("Past Trips"), WHITE, BLACK);

	/* draw data screen and position screen */
	DrawButton(50, 50, 400, 430, "", 0, BLACK, GRAY);
	DrawButton(400, 50, 750, 430, "", 0, BLACK, SADDLE_BROWN);
	VLine(400, 50, 380, BLACK);
	Triangle(565, 240, 585, 240, 575, 220, BLACK);
	Fill(575, 230, BLACK, BLACK);

	/* draw past log and next log buttons */
	DrawButton(75, 75, 212, 125, "Prev", sizeof("Past"), WHITE, BLUE);
	DrawButton(237, 75, 375, 125, "Next", sizeof("Next"), WHITE, BLUE);

	/* Back to main menu button */
	DrawButton(650, 430, 800, 480, "Main Menu", sizeof("Main Menu")-1, BLACK, BURLY_WOOD);

	DrawTripLabels();
	redraw = 0;
}

////////////////////////////////////////////////////////////////////////////////////////
//DATA
////////////////////////////////////////////////////////////////////////////////////////
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

void DrawTripData(int maxLogs, int fd, int maxEntries)
{
	char logname[20];
	snprintf(logname, 20, "log%d", logNum);

	//cover up the "please insert sd card" if it's still there
	FilledRectangle(50,431,625,480, DARK_GREEN);

	if (logExists == 1)
		DrawString(125, 155, logname, strlen(logname), BLUE, GRAY);
	else
		DrawString(125, 155, "No logs yet!", sizeof("No logs yet!"), RED, GRAY);

	char start[256] = "\0";
	char next[256] = "\0";
	char end[256] = "\0";

	char temp[256] = "\0";

	char tempStart[256] = "\0";
	char tempNext[256] = "\0";
	char tempEnd[256] = "\0";

	char startTime[30] = "\0";
	char endTime[30] = "\0";
	char startLat[30] = "\0";
	char endLat[30] = "\0";
	char startLong[30] = "\0";
	char endLong[30] = "\0";
	char startAlt[30] = "\0";
	char nextAlt[30] = "\0";

	char diffTime[100] = "\0";
	//char diffLocation[100] = "\0";
	char diffAltitude[100] = "\0";

	int startSeconds = 0;
	int endSeconds = 0;
	int diffSeconds = 0;

	int i = 0;

	float startAltitude = 0;
	float nextAltitude = 0;
	float diffAltitudeInt = 0;
	float totalAltitude = 0;

	getLogEntry(logNum, start, 0, 0, 0, 0, fd);
	printf("start = %s\n", start);
	printf("max entries = %d\n", maxEntries);
	getLogEntry(logNum, end, 0, (maxEntries-1), 1, 0, fd);
	printf("end = %s\n", end);

	//time
	strcpy(tempStart, start);
	strcpy(tempEnd, end);
	extractGpsTime(tempStart, startTime);
	extractGpsTime(tempEnd, endTime);
	startSeconds = extractTotalSeconds(startTime);
	endSeconds = extractTotalSeconds(endTime);
	diffSeconds = endSeconds - startSeconds;
	//check for roll-over
	if (diffSeconds < 0) {
		diffSeconds += (12*3600);
	}
	secondsToTime(diffTime,diffSeconds);
	DrawString(80, 225, diffTime, strlen(diffTime), BLACK, GRAY);



	fd = reOpenFile(logNum,fd);
	memset(start,0,sizeof(start));
	//printf("this should be empty %s\n", start);
	getLogEntry(logNum, start, 0, i, 0, 0, fd);
	strcpy(tempStart, start);
	printf("tempstart = %s\n", tempStart);
	extractGpsAltitude(tempStart, startAlt);
	printf("start altitude = %s\n", startAlt);
	startAltitude = atof(startAlt);

	//altitude
	for(i=1; i<maxEntries; i++) {
		//fd = reOpenFile(logNum,fd);
		memset(next,0,sizeof(next));
		getLogEntry(logNum, next, 0, 0, 0, 0, fd);
		strcpy(tempNext, next);
		printf("tempnext = %s\n", tempNext);
		extractGpsAltitude(tempNext, nextAlt);
		printf("next altitude = %s\n", nextAlt);
		nextAltitude = atof(nextAlt);

		printf("start alt f = %lf\n", startAltitude);
		printf("next alt f = %lf\n", nextAltitude);

		diffAltitudeInt = nextAltitude - startAltitude;

		totalAltitude = totalAltitude + diffAltitudeInt;

		startAltitude = nextAltitude;
	}

	printf("total = %lf\n", totalAltitude);
	sprintf(diffAltitude, "%.0lf", totalAltitude);
	DrawString(80, 365, diffAltitude, strlen(diffAltitude), BLACK, GRAY);

	//distance

	double val;
	double calc;

	val = 4.0;
	calc = sqrt(val);

	printf("Testing sqrt: %lf\n", calc);

	alt_up_sd_card_fclose(fd);
}

////////////////////////////////////////////////////////////////////////////////////////
//CHANGING PAGES
////////////////////////////////////////////////////////////////////////////////////////
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
			else if ((p.x > 50) && (p.x < 375) &&
						(p.y > 50) && (p.y < 225))
			{
				current_menu_func = &PastTrips;
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

		case (PASTTRIPS):
			if((p.x > PAST_TRIP_BACK_X1) && (p.x < PAST_TRIP_BACK_X2) &&
					(p.y > PAST_TRIP_BACK_Y1) && (p.y < PAST_TRIP_BACK_Y2))
			{
				current_menu_func = &MainMenu;
				redraw = 1;
			}
			break;

		default:
			break;
	}
}

//this is for switching logs in past trips
void PrevNext(Point p, int minLog, int maxLogs)
{
		initial = 0;
		//prev
		if((p.x > PREV_X1) && (p.x < PREV_X2) &&
				(p.y > PREV_Y1) && (p.y < PREV_Y2))
		{
			current_menu_func = &PastTrips;
			printf("log = %d, minlog = %d\n", logNum, minLog);
			if (logNum <= 0) {
				logNum = maxLogs-1;
			}
			else
				logNum--;
			redraw = 1;
		}

		//next
		else if((p.x > NEXT_X1) && (p.x < NEXT_X2) &&
			(p.y > NEXT_Y1) && (p.y < NEXT_Y2))
		{
			current_menu_func = &PastTrips;
			printf("log = %d, maxlog = %d\n", logNum, maxLogs);
			if (logNum >= maxLogs - 1) {
				logNum = 0;
			}
			else
				logNum++;
			redraw = 1;
		}
		else
			return;

}

////////////////////////////////////////////////////////////////////////////////////////
//PAGES
////////////////////////////////////////////////////////////////////////////////////////
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
	//this is here to make sure the SD card is inserted, otherwise program will crash
	Init_SDCard();
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

void PastTrips()
{
	current_menu_val = PASTTRIPS;
	int i;
	int maxLogs;
	int maxEntries;
	Point p;

	//check that there is a log0
	logExists = checkIfLog();

	maxEntries = numEntries(logNum);

	//the first log will have to always be 0
	maxLogs = lastLog(0);

	//first time getting here we should set log to the most recent one
	if (initial == 1)
		logNum = maxLogs-1;

	printf("number of logs = %d\n", maxLogs);

	//cover up the "please insert sd card" if it's still there
	FilledRectangle(50,431,625,480, DARK_GREEN);

	int myFileHandle;
	char logname[20];

	snprintf(logname, 20, "log%d.txt", logNum);
	printf("logname in pasttrips = %s\n", logname);

	//cannot afford to be constantly opening and closing, so open just once here
	if((myFileHandle = alt_up_sd_card_fopen(logname, false)) != -1)
		printf("File opened for getting next, filehandle = %d\n", myFileHandle);
	else {
		//handle SD card not inserted
		printf("File not opened for next\n");
		Init_SDCard();
		PastTrips();
	}

	/* delay for debouncing screen */
	for(i=0; i<1000; i++);

	if (redraw==1) {
		DrawPastTrips();
	}

	DrawTripData(maxLogs, myFileHandle, maxEntries);
	DrawPath(1);

	WaitForTouch();

	p = GetRelease();
	//this is here to make sure the SD card is inserted, otherwise program will crash
	Init_SDCard();
	PrevNext(p, 0, maxLogs);
	GetNextMenu(p);
	current_menu_func();
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

	//DEBUG
	int iteration = 0;
	int logNum = 0;
	int fileHandle;
	//DEBUG

	while(1)
	{
		//DEBUG
		printf("iteration = %d\n", iteration);
		int r2 = rand()%100;
		printf("r2 = %d\n", r2);
		int newlog = (iteration*2)/r2;

		if (newlog > 0) {
			logNum++;
			iteration = 0;
		}
		printf("log = %d\n", logNum);
		int r = rand() % 10;
		printf("random = %d\n", r);
		//DEBUG

		char temptime[256] = "Time: 3:3";
		char time[256];
		snprintf(time,256,"%s%d ",temptime,r);
		char templat[30] = "Latitude: 45.46";
		char lat[30];
		snprintf(lat,30,"%s%d ",templat,r);
		char templon[30] = "Longitude: 46.53";
		char lon[30];
		snprintf(lon,30,"%s%d ",templon,r);
		char tempalt[30] = "Altitude: 542";
		char alt[30];
		snprintf(alt,30,"%s%d ",tempalt,r);
		char gpsdat[500];
		snprintf(gpsdat,500,"%s%s%s%s",time,lat,lon,alt);

		strcpy(temp, gpsdat);
		printf("%s\n", temp);

		fileHandle = writeToSd(temp,logNum,sizeof(temp));
		while (fileHandle == -1) {
			printf("Trying again, re-init sd-card\n");
			Init_SDCard();
			strcpy(temp,gpsdat);
			fileHandle = writeToSd(temp,logNum,sizeof(temp));
		}

		/* DEBUG
		gpsdat = getGpsData();
		strcpy(temp, gpsdat);
		printf("%s\n", gpsdat);
		*/

		extractGpsTime(temp , time);
		//printf("%s\n", time);
		strcpy(temp, gpsdat);
		extractGpsLatitude(temp, latitude);
		//printf("%s\n", latitude);
		strcpy(temp, gpsdat);
		extractGpsLongitude(temp, longitude);
		//printf("%s\n", longitude);
		strcpy(temp, gpsdat);
		extractGpsAltitude(temp,  altitude);
		//printf("%s\n", altitude);

		char logname[20];
		sprintf(logname, "log%d", logNum);

		DrawGpsData(time, latitude, longitude, altitude, logname);

		iteration++;

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


