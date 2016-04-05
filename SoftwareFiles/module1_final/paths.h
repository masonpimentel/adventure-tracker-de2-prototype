/*
 * paths.h
 *
 *  Created on: Feb 20, 2016
 *      Author: mitchell
 */

#ifndef PATHS_H_
#define PATHS_H_

#include "Fill.h"

/* Struct for holding points to draw realtime */

typedef struct {
	float latitude;
	float longitude;
	int poi;
	XYPixel pixel;
} RTPoint;

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

#define MAX_REAL_POINTS 	64







/* top level path drawing function for displaying logged trip */
/* log is the number of the trip that we want to display */
int DrawPath(int log);
float ExtractAltitude(char* gpsdat);
void updateRealTimePath(float latitude, float longitude, int log_poi);
void InitRTPath(void);


#endif /* PATHS_H_ */
