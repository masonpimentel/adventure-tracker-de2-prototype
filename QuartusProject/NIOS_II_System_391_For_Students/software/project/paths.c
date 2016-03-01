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
#include "gps.h"

int RPIndex = 0;
RTPoint REAL_POINTS[MAX_REAL_POINTS];
/*
 * These values are chosen based off of displaying an area of approximately
 * 200mx200m on the 380pixel x 380pixel display window
 *
 * Calculated by: 380Pixels/(delta_long/lat)
 * in this case delta_long = 0.002 which corresponds with approximately
 * 200 meters.
 */
//int RTPixels_per_degree_x = 190000;
//int RTPixels_per_degree_y = 190000;

int RTPixels_per_degree_x = 1900000;
int RTPixels_per_degree_y = 1900000;


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
		float longitude = extractGpsLongitude(buf, NULL);
		float latitude = extractGpsLatitude(buf, NULL);
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
	pathInfo->pixels_per_division_x = 370/pathInfo->delta_long;
	pathInfo->pixels_per_division_y = 370/pathInfo->delta_lat;
	alt_up_sd_card_fclose(fd);

	return 0;
}



void GpsToPoint(char* gpsdat, path_info* pathInfo, XYPixel* current_point)
{
	float longitude = extractGpsLongitude(gpsdat, NULL);
	float latitude = extractGpsLatitude(gpsdat, NULL);

	int x = (longitude - pathInfo->min_long) * (pathInfo->pixels_per_division_x);
	int y = (latitude - pathInfo->min_lat) * (pathInfo->pixels_per_division_y);
	/* shhhhhh */
	x *= -1;
	y+=55;
	x+=775;

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
	DrawButton(current_point.x-4, current_point.y, current_point.x+3, current_point.y+14, "S", strlen("S"), WHITE, BLACK);
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
	DrawButton(current_point.x-4, current_point.y, current_point.x+3, current_point.y+14, "E", strlen("E"), WHITE, BLACK);
	alt_up_sd_card_fclose(fd);
	return 0;
}


void updateCenterPixel(float latitude, float longitude) {
	//Insert point into array
	RTPoint rtp = REAL_POINTS[RPIndex];
	rtp.latitude = latitude;
	rtp.longitude = longitude;
	// Most recent point is in the center of the screen
	rtp.pixel.x = 590;
	rtp.pixel.y = 240;
	REAL_POINTS[RPIndex] = rtp;
}

void updateOtherPixels()
{
	int i;
	RTPoint center = REAL_POINTS[RPIndex];
	// The delta values from the center pixel
	float d_lat, d_long;
	for(i = 0; i < MAX_REAL_POINTS; i++){
		RTPoint rtp = REAL_POINTS[i];
		if(i != RPIndex){
			d_long = center.longitude - rtp.longitude;
			d_lat = center.latitude - rtp.latitude;
			rtp.pixel.x = RTPixels_per_degree_x*d_long+center.pixel.x;
			rtp.pixel.y = center.pixel.y-RTPixels_per_degree_y*d_lat;

			REAL_POINTS[i] = rtp;
		}
	}
}

int isValidPixel(XYPixel p)
{
	return((p.x < 780 && p.x > 400) && (p.y > 50 && p.y < 430));
}
void InitRTPath()
{
	memset(REAL_POINTS, 0, sizeof(RTPoint) * MAX_REAL_POINTS);
}
void drawRTPath()
{
	int i = (RPIndex + 1)%MAX_REAL_POINTS;

	RTPoint rtpCurrent, rtpNext;
	/* clear the previous path */
	DrawButton(400, 50, 780, 430, "", 0, BLACK, SADDLE_BROWN);
	Triangle(585, 235, 595, 235, 590, 245, BLACK);
	Fill(590, 240, BLACK, BLACK);
	while(i!=RPIndex){
		rtpCurrent = REAL_POINTS[i];
		rtpNext = REAL_POINTS[(i+1)%MAX_REAL_POINTS];
		//if((rtpCurrent.pixel.x != rtpNext.pixel.x)&&(rtpCurrent.pixel.y != rtpNext.pixel.y)){
			if(isValidPixel(rtpCurrent.pixel)&&isValidPixel(rtpNext.pixel)){
				Line(rtpCurrent.pixel.x,
						rtpCurrent.pixel.y,
						rtpNext.pixel.x,
						rtpNext.pixel.y,
						BLACK);
			}

		//}
		// check that pixels will be different
		// if so, draw line
		// otherwise nothing

		i = (i+1)%MAX_REAL_POINTS;
	}
}

void updateRealTimePath(float latitude, float longitude)
{
	updateCenterPixel(latitude, longitude);
	updateOtherPixels();

	drawRTPath();

	RPIndex = (RPIndex + 1)%MAX_REAL_POINTS;
}





