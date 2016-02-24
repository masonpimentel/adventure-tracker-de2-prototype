/*
 * gps.h
 *
 *  Created on: Feb 3, 2016
 *      Author: Mason
 */

#ifndef GPS_H_
#define GPS_H_

char *getGpsData(int *reinit);
void Init_Gps(void);
void extractGpsTime(char* GpsData, char* ret);
void extractGpsLatitude(char* GpsData, char* ret);
void extractGpsLongitude(char* GpsData, char* ret);
void extractGpsAltitude(char* GpsData, char* ret);

#endif /* GPS_H_ */
