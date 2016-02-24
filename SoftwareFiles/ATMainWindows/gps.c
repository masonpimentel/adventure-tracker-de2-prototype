/*
 * gps.c
 *
 *  Created on: Feb 3, 2016
 *      Author: Mason
 */





#define Gps_Control (*(volatile unsigned char *)(0x84000210))
#define Gps_Status (*(volatile unsigned char *)(0x84000210))
#define Gps_TxData (*(volatile unsigned char *)(0x84000212))
#define Gps_RxData (*(volatile unsigned char *)(0x84000212))
#define Gps_Baud (*(volatile unsigned char *)(0x84000214))


#include <stdio.h>
#include "touch.h"
#include <string.h>
#include <stdlib.h>

/* Returns the first bit (receive bit) from the status register
 *
 * @param: int reg - the value of the status register
 * @return: int bit - the first bit of the status register
 */

char getcharGps(void)
{
		int rx_bit = 0;
		char character;
		int status = Gps_Status;
		int i = 0;

		rx_bit = GpsTestForReceivedData(status);

		while(rx_bit == 0 && i < 10000 )
		{
			status = Gps_Status;
			rx_bit = GpsTestForReceivedData(status);
			i++;
		}

		if(rx_bit == 1){
			character =  Gps_RxData;
			//printf("%x\n", character);
		}

		return character;
}



int GpsTestForReceivedData(int reg)
{
	int bit = reg & 1;
	return bit;
}
void Init_Gps(void)
{
	Gps_Control = 0x15;
	Gps_Baud = 0x06;
}
/* Delay loops
 */
void smallDelay() {
	int delay = 0;
	while (delay < 2000000)
	{
		delay++;
	}
}
void longerDelay() {
	int delay = 0;
	while (delay < 10000000)
	{
		delay++;
	}
}
void evenLongerDelay() {
	int delay = 0;
	while (delay < 20000000)
	{
		delay++;
	}
}

void extractGpsTime(char* GpsData, char* ret)
{
	char* token;
	char* target = "Time:";

	token = strtok(GpsData, " ");

	while(strcmp(token, target) != 0)
	{
		token = strtok(NULL,  " ");
	}

	token = strtok(NULL, " ");
	strcpy(ret, token);

	//printf("%s\n", ret);
}

int extractTotalSeconds(char* time) {
	char* timeCopy1 = malloc(1+ strlen(time));
	strcpy(timeCopy1,time);
	char* timeCopy2 = malloc(1+ strlen(time));
	strcpy(timeCopy2,time);
	char* hours;
	char* minutes;
	char* seconds;
	int hoursInt, minutesInt, secondsInt;

	//get hours
	hours = strtok(time, ":");

	//get minutes
	minutes = strtok(timeCopy1, ":");
	minutes = strtok(NULL, ":");

	//get seconds
	seconds = strtok(timeCopy2, ":");
	seconds = strtok(NULL, ":");
	seconds = strtok(NULL, ":");

	hoursInt = 3600 * atoi(hours);
	minutesInt = 60 * atoi(minutes);
	secondsInt = atoi(seconds);

	return (secondsInt + minutesInt + hoursInt);
}

void secondsToTime(char* buf, int seconds) {
	int hours = 0;
	int minutes = 0;

	if (seconds >= 3600) {
		hours = seconds/3600;
		seconds = seconds - (hours*3600);
	}
	if (seconds >= 60) {
		minutes = seconds/60;
		seconds = seconds - (minutes*60);
	}

	sprintf(buf, "%02d:%02d:%02d", hours, minutes, seconds);
	printf("buf = %s\n", buf);
}

void extractGpsLatitude(char* GpsData, char* ret)
{
	char* token;
	char* target = "Latitude:";


	token = strtok(GpsData, " ");

	while(strcmp(token, target) != 0)
	{
		token = strtok(NULL,  " ");
		//printf("%s\n", token);
	}

	token = strtok(NULL, " ");

	char degree[3];
	memcpy(degree, token, 2);
	degree[2] = '\0';

	char minute[8];
	memcpy(minute, token+2, 7);
	minute[7] = '\0';

	float fdegree = atof(degree);
	float fminute = atof(minute);
	float latitude = fdegree + fminute/(60.0);
	char temp[64];
	snprintf(temp, 64, "%f", latitude);
	strcpy(ret, temp);
}

void extractGpsLongitude(char* GpsData, char* ret)
{
	char* token;
	char* target = "Longitude:";

	token = strtok(GpsData, " ");
	while(strcmp(token, target) != 0)
	{
		token = strtok(NULL,  " ");
	}

	token = strtok(NULL, " ");

	char degree[4];
	memcpy(degree, token, 3);
	degree[3] = '\0';

	char minute[8];
	memcpy(minute, token+3, 7);
	minute[7] = '\0';

	float fdegree = atof(degree);
	float fminute = atof(minute);

	float longitude = fdegree + fminute/(60.0);
	char temp[64];
	snprintf(temp, 64, "%f", longitude);
	strcpy(ret, temp);

}

void extractGpsAltitude(char* GpsData, char* ret)
{
	char* token;
	char* target = "Altitude:";
	token = strtok(GpsData, " ");
	while(strcmp(token, target) != 0)
	{
		token = strtok(NULL,  " ");
	}

	token = strtok(NULL, " ");
	strcpy(ret, token);

}

char *getGpsData(int *reinit){
	char time[256] = "Time: ";
	char lat[30] = "Latitude: ";
	char lon[30] = "Longitude: ";
	char alt[30] = "Altitude: ";
	char data[500];
	int j;
	int currentIndex = 0;
	int dataType = 1;
	for(j=0; j<500; j++){
		data[j]= '\0';
	}

	if (*reinit == 1) {
		Init_Gps();
		*reinit = 0;
		printf("in here\n");
	}

	char temp;
	temp = getcharGps();
	//smallDelay();

	while (1) {
		if(temp == '$'){
		//	smallDelay();
			if (getcharGps() == 'G'){
			//	smallDelay();
				if(getcharGps() == 'P'){
				//	smallDelay();
					if(getcharGps() == 'G'){
					//	smallDelay();
						if(getcharGps() == 'G'){
						//	smallDelay();
							if(getcharGps() == 'A'){
							//	smallDelay();
								temp = getcharGps();
								//smallDelay();
								while (temp != '\r'){
									data[currentIndex] = getcharGps();
									temp = data[currentIndex];
									//smallDelay();
									currentIndex++;
								}
								break;

							}
						}
					}
				}
			}
		}

		//smallDelay();
		temp = getcharGps();
	}

	const char comma[2] = ",";
	char *dataSplit;

	dataSplit = strtok(data, comma);
	while( dataSplit != NULL) {
		if (dataType == 1){
			snprintf(time, 256, "Time: %c%c:%c%c:%c%c", dataSplit[0], dataSplit[1],dataSplit[2], dataSplit[3],dataSplit[4], dataSplit[5]);
	    } else if (dataType == 2){
	    	strcat(lat, dataSplit);
	    } else if (dataType == 3){
	    	strcat(lat, dataSplit);
	    } else if (dataType == 4){
	    	strcat(lon, dataSplit);
	    } else if (dataType == 5){
	    	strcat(lon, dataSplit);
	    } else if (dataType == 9){
	    	strcat(alt, dataSplit);
	    } else if (dataType == 10){
	    	strcat(alt, dataSplit);
	    }
		dataSplit = strtok(NULL, comma);
		dataType++;
	}

	const char space[2] = " ";
	strcat(time, space );
	strcat(time, lat);
	strcat(time, space);
	strcat(time, lon);
	strcat(time, space);
	strcat(time, alt);

	return time;
}
