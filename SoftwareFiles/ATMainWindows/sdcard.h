/*
 * sdcard.h
 *
 *  Created on: 2016-02-03
 *      Author: Manolito
 */

#ifndef SDCARD_H_
#define SDCARD_H_


#define MAX_STRING_LENGTH		28
#include "Altera_UP_SD_Card_Avalon_Interface.h"

void writeToFile(void);
void readFromFile(void);
void sdcard(void);
int writeToSd(char* string, char log, int length);
void readFromSd(int log, int entry);
int moveToEnd(int length, char* logname);



#endif /* SDCARD_H_ */
