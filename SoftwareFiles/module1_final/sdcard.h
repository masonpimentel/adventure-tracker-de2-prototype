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

#include <stdio.h>
#include <string.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "touch.h"
#include "sdcard.h"
#include "misc.h"
#include "graphics.h"

#include <stdio.h>
#include <string.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "touch.h"
#include "sdcard.h"
#include "misc.h"
#include "graphics.h"

void nextEntry(char* buf, int fd);
int moveToEnd(int length, char* logname);
int writeToSd(char* string, char log, int length);
void readFromSd(int log, int entry);
int numEntries(int log);
void lastLogEntry(int log, char* end, int part);
int reOpenFile(int logNum, int fd);
void getLogEntry(int log, char* buf, int part, int entry, int alreadySeeked, int fd);
void firstLogEntry(int log, char* start, int part);
int lastLog(int first);
int firstLog(void);
int checkIfLog(void);
void Init_SDCard(void);
void sync_logs(void);
int set_dirty_flag(char* logname, int dirty);
int check_if_dirty(int fd);

#endif /* SDCARD_H_ */
