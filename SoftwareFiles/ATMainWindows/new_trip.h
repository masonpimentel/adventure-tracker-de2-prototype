/*
 * new_trip.h
 *
 *  Created on: 2016-02-22
 *      Author: Manolito
 */

#ifndef NEW_TRIP_H_
#define NEW_TRIP_H_

void DrawGpsLabels(void);
void DrawGpsData(char* time, char* latitude, char* longitude, char* altitude, char* logname);
void DrawGpsMenu(int *redraw);

#endif /* NEW_TRIP_H_ */
