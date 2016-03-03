/*
 * past_trips.c
 *
 *  Created on: 2016-02-22
 *      Author: Manolito
 */

#include "past_trips.h"
#include "graphics.h"
#include "gps.h"
#include "sdcard.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void DrawTripLabels()
{
	DrawString(60,205,"Total Time: ", sizeof("Total Time: ")-1, BLACK, GRAY);
	DrawString(60, 275, "Total Distance (km): ", sizeof("Total Distance: ")-1,BLACK, GRAY);
	DrawString(60, 345, "Total Change in Altitude (m): ", sizeof("Total Change in Altitude: ")-1,BLACK, GRAY);
}

void DrawPastTrips(int *redraw)
{
	/* set the screen background */
	FilledRectangle(0,0,800,480, DARK_GREEN);
	DrawButton(0, 0, 800, 40, "Past Trips", sizeof ("Past Trips"), WHITE, BLACK);

	/* draw data screen and position screen */
	DrawButton(20, 50, 400, 430, "", 0, BLACK, GRAY);
	DrawButton(400, 50, 780, 430, "", 0, BLACK, SADDLE_BROWN);
	VLine(400, 50, 380, BLACK);

	/* draw past log and next log buttons */
	DrawButton(75, 75, 212, 125, "Prev", sizeof("Past"), WHITE, BLUE);
	DrawButton(237, 75, 375, 125, "Next", sizeof("Next"), WHITE, BLUE);

	/* Back to main menu button */
	DrawButton(650, 430, 800, 480, "Main Menu", sizeof("Main Menu")-1, BLACK, BURLY_WOOD);

	DrawTripLabels();
	*redraw = 0;
}

/*
 * Draws the data in Past Trips, including total time, distance travelled and change in altitude
 * Total time works by taking the difference of the last time entry of a given log and the first time entry.
 * Total distance works by iterating through all the GPS coordinates in the logfile, finding the change in distance
 * between the current coordinates and the last coordinates and adding that difference in each iteration to
 * the previous total.
 * Total altitude works the same as total distance except there is no distance calulation necessary.
 *
 */
void DrawTripData(int maxLogs, int fd, int maxEntries, int logNum, int logExists)
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

	char tempStart[256] = "\0";
	char tempNext[256] = "\0";
	char tempEnd[256] = "\0";

	char startTime[30] = "\0";
	char endTime[30] = "\0";

	char startLat[30] = "\0";
	char nextLat[30] = "\0";
	char startLong[30] = "\0";
	char nextLong[30] = "\0";
	char startAlt[30] = "\0";
	char nextAlt[30] = "\0";
	char totalDistStr[30] = "\0";

	char diffTime[100] = "\0";
	char diffAltitude[100] = "\0";

	int startSeconds = 0;
	int endSeconds = 0;
	int diffSeconds = 0;

	int i = 0;

	float startAltitude = 0;
	float nextAltitude = 0;
	float diffAltitudeInt = 0;
	float totalAltitude = 0;

	double totalDistance = 0;
	double curDistance = 0;

	double startLatDbl = 0;
	double nextLatDbl = 0;
	double startLongDbl = 0;
	double nextLongDbl = 0;
	if(maxEntries < 2)
	{
		return;
	}
	getLogEntry(logNum, start, 0, 0, 0, fd);
	printf("start = %s\n", start);
	printf("max entries = %d\n", maxEntries);
	getLogEntry(logNum, end, 0, (maxEntries-1), 1, fd);
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

	//altitude
	fd = reOpenFile(logNum,fd);
	memset(start,0,sizeof(start));
	getLogEntry(logNum, start, 0, 0, 0, fd);
	strcpy(tempStart, start);
	extractGpsAltitude(tempStart, startAlt);
	startAltitude = atof(startAlt);

	for(i=1; i<maxEntries; i++) {
		memset(next,0,sizeof(next));
		getLogEntry(logNum, next, 0, 0, 0, fd);
		strcpy(tempNext, next);
		extractGpsAltitude(tempNext, nextAlt);
		nextAltitude = atof(nextAlt);

		diffAltitudeInt = nextAltitude - startAltitude;
		totalAltitude = totalAltitude + diffAltitudeInt;
		startAltitude = nextAltitude;
	}

	printf("total = %lf\n", totalAltitude);
	sprintf(diffAltitude, "%.0lf", totalAltitude);
	DrawString(80, 365, diffAltitude, strlen(diffAltitude), BLACK, GRAY);

	//distance
	fd = reOpenFile(logNum,fd);
	memset(start,0,sizeof(start));
	getLogEntry(logNum, start, 0, 0, 0, fd);
	printf("start = %s\n", start);
	strcpy(tempStart, start);
	extractGpsLatitude(tempStart,startLat);
	startLatDbl = atof(startLat);
	printf("start lat = %lf\n", startLatDbl);
	strcpy(tempStart, start);
	extractGpsLongitude(tempStart,startLong);
	startLongDbl = atof(startLong);
	printf("start long = %lf\n", startLongDbl);

	for(i=1; i<maxEntries; i++) {
		memset(next,0,sizeof(next));
		getLogEntry(logNum, next, 0, 0, 0, fd);
		strcpy(tempNext, next);
		nextLatDbl = extractGpsLatitude(tempNext,nextLat);

		strcpy(tempNext, next);
		nextLongDbl = extractGpsLongitude(tempNext,nextLong);

		curDistance = distance(startLatDbl,startLongDbl,nextLatDbl,nextLongDbl, 'K');

		startLatDbl = nextLatDbl;
		startLongDbl = nextLongDbl;

		totalDistance = totalDistance + curDistance;
	}

	printf("total distance = %lf\n", totalDistance);
	sprintf(totalDistStr, "%.4lf", totalDistance);
	DrawString(80, 295, totalDistStr, strlen(totalDistStr), BLACK, GRAY);

	alt_up_sd_card_fclose(fd);
}


