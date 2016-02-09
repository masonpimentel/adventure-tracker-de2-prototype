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

	printf("%s\n", ret);
}

void extractGpsLatitude(char* GpsData, char* ret)
{
	char* token;
	char* target = "Latitude:";


	token = strtok(GpsData, " ");

	while(strcmp(token, target) != 0)
	{
		token = strtok(NULL,  " ");
		printf("%s\n", token);
	}

	token = strtok(NULL, " ");
	strcpy(ret, token);
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
	strcpy(ret, token);

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

char *getGpsData(){
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

	Init_Gps();

	char temp;
	char temp1;
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
