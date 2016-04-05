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
#include "past_trips.h"
#include "new_trip.h"
#include "sdcard.h"
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "bluetooth.h"

//insert dummy values, and randomly increase logNum
//#define DEBUG

char* bluetooth_name = "Adventure Tracker";
char bluetooth_pin[21];

int redraw = 1;
int redrawButtons = 1;
int logExists = 0;
int curMode = 0;
int reinit = 1;
int initializing = 1;
int log_poi = 0;

int CurLogNum = -1;

void DrawMainMenu()
{
	FilledRectangle(0, 0, 800, 480, DARK_GREEN);
	DrawButton(0, 0, 800, 40, "Adventure Tracker", sizeof ("Adventure Tracker"), WHITE, BLACK);
	DrawButton(50, 50, 375, 225, "Past Trips", sizeof("Past Trips")-1, BLACK, SADDLE_BROWN);
	DrawButton(50, 255, 375, 430, "Bluetooth Settings", sizeof("Bluetooth Settings")-1, BLACK, SADDLE_BROWN);
	DrawButton(400, 50, 750, 430, "New Trip", sizeof("New Trip")-1, BLACK, SADDLE_BROWN);
}

void DrawBluetoothSettings()
{
	DrawButton(0, 0, 800, 40, "Bluetooth Settings", sizeof ("Bluetooth Settings"), WHITE, BLACK);
	FilledRectangle(0,40, 800, 480, DARK_GREEN);
	DrawButton(250, 70, 550, 100, "Bluetooth ID", strlen("Bluetooth ID"), WHITE, BLACK);
	DrawButton(250, 100, 550, 170, bluetooth_name, strlen(bluetooth_name), BLACK, GRAY);
	DrawButton(250, 210, 550, 240, "Bluetooth PIN", strlen("Bluetooth PIN"), WHITE, BLACK);
	DrawButton(250, 240, 550, 310, bluetooth_pin, strlen(bluetooth_pin), BLACK, GRAY);
	DrawButton(250, 350, 550, 450, "Main Menu", strlen("Main Menu"), BLACK, GRAY);
}
////////////////////////////////////////////////////////////////////////////////////////
//CHANGING PAGES
////////////////////////////////////////////////////////////////////////////////////////
int GetNextMenu(Point p)
{
	switch (current_menu_val)
	{
		case (MAINMENU):
			if((p.x > NEWTRIP_X1) && (p.x < NEWTRIP_X2) &&
			   (p.y > NEWTRIP_Y1) && (p.y < NEWTRIP_Y2))
			{
				current_menu_func = &NewTrip;
				redraw = 1;
				return 0;
			}
			else if((p.x > 50) && (p.x < 375) &&
					   (p.y > 255) && (p.y < 430))
			{
				current_menu_func = &EasterEggs;
				redraw = 1;
				return 0;
			}
			else if ((p.x > 50) && (p.x < 375) &&
						(p.y > 50) && (p.y < 225))
			{
				current_menu_func = &PastTrips;
				redraw = 1;
				return 0;
			}
			break;

		case (NEWTRIP):
			if((p.x > NEWTRIP_BACK_X1) && (p.x < NEWTRIP_BACK_X2) &&
			   (p.y > NEWTRIP_BACK_Y1) && (p.y < NEWTRIP_BACK_Y2))
			{
				current_menu_func = &MainMenu;
				redraw = 1;
				return 0;
			}
			else if((p.x > 62) && (p.x < 162) &&
					(p.y > 80) && (p.y < 130))
			{
				log_poi = 1;
			}
			break;
		case (EASTEREGGS):
			if((p.x > 0) && (p.x < 800) &&
			   (p.y > 0) && (p.y < 480))
			{
				current_menu_func = &MainMenu;
				redraw = 1;
				return 0;
			}
			break;

		case (PASTTRIPS):
			if((p.x > PAST_TRIP_BACK_X1) && (p.x < PAST_TRIP_BACK_X2) &&
					(p.y > PAST_TRIP_BACK_Y1) && (p.y < PAST_TRIP_BACK_Y2))
			{
				current_menu_func = &MainMenu;
				redraw = 1;
				return 0;
			}
			break;

		default:
			break;
	}
	return -1;
}

//this is for switching logs in past trips
void PrevNext(Point p, int minLog, int maxLogs, int* logNum)
{
		//prev
		if((p.x > PREV_X1) && (p.x < PREV_X2) &&
				(p.y > PREV_Y1) && (p.y < PREV_Y2))
		{
			if (logNum <= 0) {
				*logNum = maxLogs-1;
			}
			else
				(*logNum)--;
			redraw = 1;
		}

		//next
		else if((p.x > NEXT_X1) && (p.x < NEXT_X2) &&
			(p.y > NEXT_Y1) && (p.y < NEXT_Y2))
		{
			if (*logNum >= maxLogs - 1) {
				*logNum = 0;
			}
			else
				(*logNum)++;
			redraw = 1;
		}
		else
			return;

}

//this is for changing speed in new trip
void ChangeSpeed(Point p, int *redrawButtons)
{
		//walking
		if((p.x > WALKING_X1) && (p.x < WALKING_X2) &&
				(p.y > WALKING_Y1) && (p.y < WALKING_Y2))
		{
			curMode = WALKING;
			redraw = 1;
			*redrawButtons = 1;
		}

		//biking
		else if((p.x > BIKING_X1) && (p.x < BIKING_X2) &&
			(p.y > BIKING_Y1) && (p.y < BIKING_Y2))
		{
			curMode = BIKING;
			redraw = 1;
			*redrawButtons = 1;
		}
		//skiing
		else if((p.x > SKIING_X1) && (p.x < SKIING_X2) &&
			(p.y > SKIING_Y1) && (p.y < SKIING_Y2))
		{
			curMode = SKIING;
			redraw = 1;
			*redrawButtons = 1;
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
	Init_SDCard();
	char cmd;
	while(1)
	{
		cmd = receive_command_bluetooth();
		execute_cmd_bluetooth(cmd);

		if(ScreenTouched())
		{
			break;
		}
	}

	int i;
	for(i=0; i<1000; i++);
	Point p = GetRelease();

	//this is here to make sure the SD card is inserted, otherwise program will crash

	GetNextMenu(p);
	current_menu_func();
}


void DrawKeypad(void)
{
	FilledRectangle(0,40, 800, 480, DARK_GREEN);
	DrawButton(0, 0, 800, 40, "Change PIN", sizeof ("Change PIN"), WHITE, BLACK);
	DrawButton(250, 100, 550, 150, bluetooth_pin, strlen(bluetooth_pin), BLACK, GRAY);
	DrawButton(250, 150, 350, 225, "1", strlen("1"), WHITE, BLACK);
	DrawButton(350, 150, 450, 225, "2", strlen("2"), WHITE, BLACK);
	DrawButton(450, 150, 550, 225, "3", strlen("3"), WHITE, BLACK);
	DrawButton(250, 225, 350, 300, "4", strlen("4"), WHITE, BLACK);
	DrawButton(350, 225, 450, 300, "5", strlen("5"), WHITE, BLACK);
	DrawButton(450, 225, 550, 300, "6", strlen("6"), WHITE, BLACK);
	DrawButton(250, 300, 350, 375, "7", strlen("7"), WHITE, BLACK);
	DrawButton(350, 300, 450, 375, "8", strlen("8"), WHITE, BLACK);
	DrawButton(450, 300, 550, 375, "9", strlen("9"), WHITE, BLACK);
	DrawButton(250, 375, 350, 450, "DEL", strlen("DEL"), WHITE, BLACK);
	DrawButton(350, 375, 450, 450, "0", strlen("0"), WHITE, BLACK);
	DrawButton(450, 375, 550, 450, "ENTER", strlen("ENTER"), WHITE, BLACK);
	VLine(350, 150, 300, WHITE);
	VLine(450, 150, 300, WHITE);
	HLine(250, 225, 300, WHITE);
	HLine(250, 300, 300, WHITE);
	HLine(250, 375, 300, WHITE);
}

void ChangeBluetoothPIN(void)
{

	DrawKeypad();
	int i;
	for(i=0; i<1000000; i++);
	Point p = GetRelease();
	int save = 0;
	while(!save)
	{
	if(p.x > 250 && p.x < 350)
	{
		if(p.y > 150 && p.y < 225)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '1';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
		else if(p.y > 225 && p.y < 300)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '4';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
		else if(p.y > 300 && p.y < 375)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '7';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
		else if(p.y > 375 && p.y < 450)
		{
			int len = strlen(bluetooth_pin);
			bluetooth_pin[len-1] = '\0';
		}
	}
	else if(p.x > 350 && p.x < 450)
	{
		if(p.y > 150 && p.y < 225)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '2';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
		else if(p.y > 225 && p.y < 300)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '5';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
		else if(p.y > 300 && p.y < 375)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '8';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
		else if(p.y > 375 && p.y < 450)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '0';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
	}
	if(p.x > 450 && p.x < 550)
	{
		if(p.y > 150 && p.y < 225)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '3';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
		else if(p.y > 225 && p.y < 300)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '6';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
		else if(p.y > 300 && p.y < 375)
		{
			int len = strlen(bluetooth_pin);
			if(len < 20)
			{
				bluetooth_pin[len] = '9';
				len++;
				bluetooth_pin[len] = '\0';
			}
		}
		else if(p.y > 375 && p.y < 450)
		{
			if(strlen(bluetooth_pin) > 0)
			{
				save = 1;
			}
		}
	}
	DrawButton(250, 100, 550, 150, bluetooth_pin, strlen(bluetooth_pin), BLACK, GRAY);
	for(i=0; i<1000000; i++);
	p = GetRelease();
	}

}
void EasterEggs()
{
	current_menu_val = EASTEREGGS;

	DrawBluetoothSettings();

	Point p = GetRelease();
	if(p.x > 250 && p.x < 550 && p.y > 350 && p.y < 450)
	{
		DrawButton(200, 190, 600, 330, "Applying Changes...", strlen("Applying Changes..."), BLACK, GRAY);
		changePIN(bluetooth_pin);
		changeName(bluetooth_name);
		BtReboot();
		GetNextMenu(p);
	}
	if(p.x > 250 && p.x < 550 && p.y > 210 && p.y < 310)
	{
		ChangeBluetoothPIN();
	}
	else
	{
		GetNextMenu(p);
	}
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

	//the first log will have to always be 0
	maxLogs = lastLog(0);

	if(CurLogNum == -1)
	{
		CurLogNum = maxLogs-1;
	}

	//first time getting here we should set log to the most recent one

	maxEntries = numEntries(CurLogNum);


	//cover up the "please insert sd card" if it's still there
	FilledRectangle(50,431,625,480, DARK_GREEN);

	int myFileHandle;
	char logname[20];

	snprintf(logname, 20, "log%d.txt", CurLogNum);
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
		DrawPastTrips(&redraw);
	}

	if(maxEntries >= 2)
	{
		DrawTripData(maxLogs, myFileHandle, maxEntries, CurLogNum, logExists);
		DrawPath(CurLogNum);
	}
	WaitForTouch();

	p = GetRelease();
	//this is here to make sure the SD card is inserted, otherwise program will crash
	Init_SDCard();
	PrevNext(p, 0, maxLogs, &CurLogNum);
	GetNextMenu(p);
	current_menu_func();
}

void NewTrip()
{
	int fileHandle;
	int timeDelay;
	int logNum;

	char time[100];
	char latitude[100];
	char longitude[100];
	char altitude[100];
	redrawButtons = 1;
	InitRTPath();
	//snprintf(time, 100,"4:20: ");
	//snprintf(latitude, 100, "S52* 26' 59.79\"");
	//snprintf(longitude, 100, "N13* 23' 36.11\"");
	//snprintf(altitude, 100, "1180.23m");

	Point p;
	/* update the menu state variable */
	current_menu_val = NEWTRIP;
	if(redraw)
		DrawGpsMenu(&redraw);

	logNum = lastLog(0);

	/* delay for debouncing screen */
	int i;
	for(i=0; i<10000; i++);

	char * gpsdat;

	char temp[256];
	char data[256];
	snprintf(data, 256, "%s", "DIRTY:1 ");

	#ifdef DEBUG
	int iteration = 0;
	#endif //DEBUG

	while(1)
	{
bad_touch:
		#ifdef DEBUG
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
		#endif

		#ifndef DEBUG
		gpsdat = getGpsData(&reinit);
		strcpy(temp, gpsdat);
		strcat(data, gpsdat);
		//printf("%s\n", gpsdat);
		#endif

		extractGpsTime(temp , time);
		strcpy(temp, gpsdat);
		float flatitude = extractGpsLatitude(temp, latitude);
		strcpy(temp, gpsdat);
		float flongitude = extractGpsLongitude(temp, longitude);
		strcpy(temp, gpsdat);
		extractGpsAltitude(temp,  altitude);
		strcpy(temp, gpsdat);

		char is_poi[20];
		snprintf(is_poi, 20, " POI:%d", (int)log_poi);
		strcat(data, is_poi);
		printf("%s", data);


		//check if gps is still trying to get a fix
		if (atoi(latitude) == 0) {
			printf("setting to 1\n");
			initializing = 1;
		}
		else
			initializing = 0;

		if (initializing == 0) {
			fileHandle = writeToSd(data,logNum,strlen(temp));
			while (fileHandle == -1) {
				//printf("Trying again, re-init sd-card\n");
				Init_SDCard();
				fileHandle = writeToSd(data,logNum,sizeof(temp));
			}
		}
		//else
			//printf("Not writing to SD!\n");

		char logname[20];
		sprintf(logname, "log%d", logNum);

		//printf("before init is %lf, %d\n", initializing, initializing);
		DrawGpsData(time, latitude, longitude, altitude, logname, curMode, &redrawButtons, &initializing);

		updateRealTimePath(flatitude, flongitude, log_poi);
		if(log_poi)
		{
			log_poi = false;
		}
		memset((void*)data, 0, 256);

		#ifdef DEBUG
		iteration++;
		#endif

		if (curMode == WALKING)
			timeDelay = WALKING_DELAY;
		else if (curMode == BIKING)
			timeDelay = BIKING_DELAY;
		else
			timeDelay = SKIING_DELAY;

		for(i=0; i<timeDelay; i++)
		{
			if(ScreenTouched())
			{
				goto touched;
			}
		}
	}
touched:

	p = GetRelease();
	ChangeSpeed(p, &redrawButtons);
	int _switch = GetNextMenu(p);
	if(_switch == -1)
	{
		goto bad_touch;
	}

	current_menu_func();
}


