/*
 * past_trips.h
 *
 *  Created on: 2016-02-22
 *      Author: Manolito
 */

#ifndef PAST_TRIPS_H_
#define PAST_TRIPS_H_

void DrawTripLabels(void);
void DrawPastTrips(int *redraw, int CurLogNum);
double distance(double lat1, double lon1, double lat2, double lon2, char unit);

#endif /* PAST_TRIPS_H_ */
