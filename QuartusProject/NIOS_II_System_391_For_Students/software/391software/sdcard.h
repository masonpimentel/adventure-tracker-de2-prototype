/*
 * sdcard.h
 *
 *  Created on: 2016-02-03
 *      Author: Manolito
 */

#ifndef SDCARD_H_
#define SDCARD_H_

#include "Altera_UP_SD_Card_Avalon_Interface.h"

#define MAX_STRING_LENGTH		28

void writeToFile(void);
void readFromFile(void);
void sdcard(void);
int writeToSd(char* string, char log, int length);
void readFromSd(int log, int entry);
int moveToEnd(int length, char* logname);
void Init_SDCard(void);
int firstLog(void);
int lastLog(int first);
void firstLogEntry(int log, char* start, int part);
void lastLogEntry(int log, char* end, int part);
int numEntries(int log);



#endif /* SDCARD_H_ */
