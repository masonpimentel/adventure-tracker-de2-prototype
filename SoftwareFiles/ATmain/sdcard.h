/*
 * sdcard.h
 *
 *  Created on: 2016-02-03
 *      Author: Manolito
 */

#ifndef SDCARD_H_
#define SDCARD_H_

#include "Altera_UP_SD_Card_Avalon_Interface.h"

void writeToFile(void);
void readFromFile(void);
void sdcard(void);
int writeToSd(char* string, char log, int length);
void readFromSd(int length, int fileHandle);
int moveToEnd(int length);



#endif /* SDCARD_H_ */
