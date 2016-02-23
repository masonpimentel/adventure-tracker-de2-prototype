/*
 * paths.c
 *
 *  Created on: Feb 20, 2016
 *      Author: mitchell
 */

#include "paths.h"
#include "graphics.h"
#include <stdlib.h>
#include "Fill.h"
#include <string.h>
#include "sdcard.h"
#include <stdio.h>

typedef struct {
	float delta_long;
	float delta_lat;
	float min_long;
	float min_lat;
	float max_long;
	float max_lat;
	int pixels_per_division_x;
	int pixels_per_division_y;
} path_info;

float ExtractLongitude(char* gpsdat)
{
	char* delim = " ";
	char temp[100];
	strcpy(temp, gpsdat);
	char* token = strtok(temp, delim);
	char* match = "Longitude:";
	while(strcmp(token, match) != 0)
	{
		token = strtok(0, delim);
	}
	token = strtok(0, delim);
	return atof(token);
}

float ExtractLatitude(char* gpsdat)
{
	char* delim = " ";
	char temp[100];
	strcpy(temp, gpsdat);
	char* token = strtok(temp, delim);
	char* match = "Latitude:";
	while(strcmp(token, match) != 0)
	{
		token = strtok(0, delim);
	}
	token = strtok(0, delim);
	return atof(token);
}

float ExtractAltitude(char* gpsdat)
{
	char* delim = " ";
	char temp[100];
	strcpy(temp,gpsdat);
	char* token = strtok(temp, delim);
	char* match = "Altitude:";
	while(strcmp(token, match) != 0)
	{
		token = strtok(0,delim);
	}
	token = strtok(0,delim);
	return atof(token);
}

int GetPathInfo(path_info* pathInfo, int log)
{
	char buf[100];
	char fname[64];

	pathInfo->min_lat = 9999.9;
	pathInfo->max_lat = 0.0;
	pathInfo->min_long = 9999.9;
	pathInfo->max_long = 0.0;


	snprintf(fname, 64, "log%d.txt", log);
	int fd = alt_up_sd_card_fopen(fname, 0);
	if(fd == -1)
	{
		return -1;
	}
	nextEntry(buf, fd);
	while(buf[0] != '\0')
	{
		float longitude = ExtractLongitude(buf);
		float latitude = ExtractLatitude(buf);
		if(latitude > pathInfo->max_lat)
		{
			pathInfo->max_lat = latitude;
		}
		if(longitude > pathInfo->max_long)
		{
			pathInfo->max_long = longitude;
		}
		if(latitude < pathInfo->min_lat)
		{
			pathInfo->min_lat = latitude;
		}
		if(longitude < pathInfo->min_long)
		{
			pathInfo->min_long = longitude;
		}
		nextEntry(buf, fd);
	}
	pathInfo->delta_long = pathInfo->max_long - pathInfo->min_long;
	pathInfo->delta_lat = pathInfo->max_lat - pathInfo->min_lat;
	pathInfo->pixels_per_division_x = 350/pathInfo->delta_long;
	pathInfo->pixels_per_division_y = 350/pathInfo->delta_lat;
	alt_up_sd_card_fclose(fd);

	return 0;
}



void GpsToPoint(char* gpsdat, path_info* pathInfo, XYPixel* current_point)
{
	float longitude = ExtractLongitude(gpsdat);
	float latitude = ExtractLatitude(gpsdat);

	int x = (longitude - pathInfo->min_long) * (pathInfo->pixels_per_division_x);
	int y = (latitude - pathInfo->min_lat) * (pathInfo->pixels_per_division_y);
	y+=50;
	x+=400;

	current_point->x = x;
	current_point->y = y;

}

int DrawPath(int log)
{
	char buf[100];
	char temp[100];
	char fname[64];

	XYPixel current_point;
	XYPixel previous_point;
	path_info pathInfo;

	GetPathInfo(&pathInfo, log);
	snprintf(fname, 64, "log%d.txt", log);
	int fd = alt_up_sd_card_fopen(fname, 0);
	if(fd == -1)
	{
		return -1;
	}
	nextEntry(buf, fd);
	GpsToPoint(buf, &pathInfo, &current_point);
	nextEntry(buf, fd);
	while(buf[0] != '\0')
	{
		previous_point.x = current_point.x;
		previous_point.y = current_point.y;
		GpsToPoint(buf, &pathInfo, &current_point);
		if((current_point.x == previous_point.x) && (current_point.y == previous_point.y))
		{
			WriteAPixel(current_point.x, current_point.y, BLACK);
		}
		else
		{
			Line(current_point.x, current_point.y, previous_point.x, previous_point.y, BLACK);
		}
		nextEntry(buf, fd);
	}
	alt_up_sd_card_fclose(fd);
	return 0;
}





