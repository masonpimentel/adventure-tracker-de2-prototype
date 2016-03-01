/*
 * distance.h
 *
 *  Created on: 2016-02-23
 *      Author: Manolito
 */

#ifndef DISTANCE_H_
#define DISTANCE_H_

#define pi 3.14159265358979323846

double deg2rad(double);
double rad2deg(double);
double distance(double lat1, double lon1, double lat2, double lon2, char unit);

#endif /* DISTANCE_H_ */
