#include <stdio.h>
#include <string.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "touch.h"
#include "sdcard.h"
#include "misc.h"
#include "graphics.h"

/*
 * Puts the next log entry into a buffer
 *
 * @param: buf - the string the character is being appended to
 * @param: fd - the file descriptor of the open file
 *
 */
void nextEntry(char* buf, int fd) {
	int i = 0;
	char character = 'a';

	while(character != 'x') {
		character = alt_up_sd_card_read(fd);
		if(character == -1)
			goto error;
		if (character != 'x') {
			buf[i] = character;
		}
    i++;
	}
	buf[i] = '\0';
	return;

error:
	buf[0] = '\0';

}

/*
 * Called by writeToSd to seek to the end of the log
 */
int moveToEnd(int length, char* logname) {
	short int myFileHandle;

	int character = 1;

	if((myFileHandle = alt_up_sd_card_fopen(logname, false)) != -1) {

		while (character != -1) {
			character = alt_up_sd_card_read(myFileHandle);
		}
		return myFileHandle;
	}
	else {
		myFileHandle = alt_up_sd_card_fopen(logname, true);
		if (myFileHandle < 0) {
			Init_SDCard();
			moveToEnd(length, logname);
			return -1;
		}
	}

	//shouldn't get here
	return -1;
}
/*
 * @param: string - pointer to the string to be added to the logfile
 * @param: log - the log number that goes after "log" and before ".txt"
 * @param: length - the length of the string that is being added
 *
 * @return: -1 on failure, file handle on success
 *
 * writeToSd needs to be called as follows:
 * 			fileHandle = writeToSd(string, log, length);
			if (fileHandle == -1) {
				printf("Trying again, re-init sd-card\n");
				//get string again
				length = strlen(string);
				Init_SDCard();
				fileHandle = writeToSd(string,log,length);
			}
 */
int writeToSd(char* string, char log, int length) {
	int i;

	short int myFileHandle;
	char modified[length+2];
	char logname[20];

	sprintf(logname, "log%d.txt", log);

	myFileHandle = moveToEnd(length, logname);

	if(myFileHandle >= 0) {

		strcpy(modified, string);
		strcat(modified, " x");
		//printf("Writing '%s' to SD card\n", modified);
		length = strlen(modified);
		for(i = 0; i < length; i ++) {
			if(alt_up_sd_card_write(myFileHandle,modified[i]) == false){
				//printf("Error writing to file...\n");
				return -1;
			}
		}
		//printf("Done!!!\n");
		alt_up_sd_card_fclose(myFileHandle);
	}
	else {
		//printf("File not opened for writing\n");
		Init_SDCard();
	}
	return myFileHandle;
}

/*
 * @param: log - the logfile being read from
 * @param: entry - which entry in the logfile to be read (starting at 0)
 *
 * Note: must be called after writeToSd
 */
void readFromSd(int log, int entry) {
	short int myFileHandle;
	int i;
	char character = 'a';

	char logname[20];

	sprintf(logname, "log%d.txt", log);


	if((myFileHandle = alt_up_sd_card_fopen(logname, false)) != -1) {

		for (i=0; i<entry; i++) {
			while(character != 'x') {
				character = alt_up_sd_card_read(myFileHandle);
			}
			character = 'a';
		}
		while(character != 'x') {
			character = alt_up_sd_card_read(myFileHandle);
			printf("ASCII character %i (decimal) read\n", character);
		}

	}
	else {
		printf("File not opened for reading\n");
		Init_SDCard();
	}
}

/*
 * Finds out the number of entries in a logfile
 *
 * @param: log - the logfile being read from
 * @return: entries - the number of entries in the logfile
 */
int numEntries(int log) {
	short int myFileHandle;

	char character = 'a';
	int entries = 0;

	char logname[20];

	sprintf(logname, "log%d.txt", log);

	if((myFileHandle = alt_up_sd_card_fopen(logname, false)) != -1) {

		while (character > 0) {
			while(character != 'x') {
				character = alt_up_sd_card_read(myFileHandle);
			}
			entries++;
			character = alt_up_sd_card_read(myFileHandle);
			if (character > 0) {
				//set it to anything but 'x'
				character = 'a';
			}
		}
		alt_up_sd_card_fclose(myFileHandle);

	}
	else {
		printf("File not opened for reading\n");
		Init_SDCard();
	}
	return entries;

}

/*
 * Puts the last entry in a string
 *
 * @param: log - the logfile being read from
 * @param: end - the end string
 * @param: part - 1 for the first half of the string, 2 for the second half of the string 0 for the whole thing
 */
void lastLogEntry(int log, char* end, int part)
{
	int toSeek = numEntries(log) - 1;

	short int myFileHandle;
	int i;
	char character = 'a';

	char logname[20];

	sprintf(logname, "log%d.txt", log);

	int toSkip = (part-1) * MAX_STRING_LENGTH;
	int divider = 0;

	if((myFileHandle = alt_up_sd_card_fopen(logname, false)) != -1) {

		//seek through all the 'x's
		for (i=0; i<toSeek; i++) {
			while(character != 'x') {
				character = alt_up_sd_card_read(myFileHandle);
			}
			character = 'a';
		}
		character = 'a';

		if (part == 0) {
			while (character != 'x'){
				char charHolder[2] = "\0";
				character = alt_up_sd_card_read(myFileHandle);
				if (character != 'x') {
					charHolder[0] = character;
					strcat(end,charHolder);
				}
			}
		}
		if (part == 1) {
			while (character != 'x' && divider < MAX_STRING_LENGTH){
				char charHolder[2] = "\0";
				character = alt_up_sd_card_read(myFileHandle);
				if (character != 'x') {
					charHolder[0] = character;
					strcat(end,charHolder);
				}
				divider++;
			}
		}
		else {
			int skip;
			//need to skip to where first half left off
			for (skip = 0; skip < toSkip; skip++) {
				character = alt_up_sd_card_read(myFileHandle);
			}
			while (character != 'x' && divider < MAX_STRING_LENGTH){
				char charHolder[2] = "\0";
				character = alt_up_sd_card_read(myFileHandle);
				if (character != 'x') {
					charHolder[0] = character;
					strcat(end,charHolder);
				}
				divider++;
			}
		}
		alt_up_sd_card_fclose(myFileHandle);
	}
	else {
		printf("File not opened for reading\n");
		Init_SDCard();
	}
	return;

}

/*
 * Re-opens the file to reset the seek position
 *
 * @param: logNum - the logfile being read from
 * @return: newLog - the new file descriptor
 */
int reOpenFile(int logNum, int fd) {
	char logname[20];
	int newLog;

	snprintf(logname, 20, "log%d.txt", logNum);

	if((newLog = alt_up_sd_card_fopen(logname, false)) != -1)
		printf("File re-opened, filehandle = %d\n", newLog);
	else {
		//handle SD card not inserted
		printf("File not re-opened\n");
		Init_SDCard();
		reOpenFile(logNum,fd);
	}

	return newLog;
}

/*
 * Puts the last entry in a string
 *
 * @param: log - the logfile being read from
 * @param: buf - the string the entry will be put in
 * @param: part - 1 for the first half of the string, 2 for the second half of the string 0 for the whole thing
 * @param: entry - the entry in the logfile to get (starting at 0)
 * @param: fd - the file descriptor
 * note that we need the fd now because we are doing multiple read operations - we cannot afford to be
 * constantly opening and closing the file
 */
void getLogEntry(int log, char* buf, int part, int entry, int alreadySeeked, int fd)
{

	int toSeek = entry-alreadySeeked;

	int i;
	char character = 'a';

	char logname[20];

	sprintf(logname, "log%d.txt", log);

	int toSkip = (part-1) * MAX_STRING_LENGTH;
	int divider = 0;

	//seek through all the 'x's
	for (i=0; i<toSeek; i++) {
		while(character != 'x') {
			character = alt_up_sd_card_read(fd);
		}
		character = 'a';
	}
	character = 'a';

	if (part == 0) {
		while (character != 'x'){
			char charHolder[2] = "\0";
			character = alt_up_sd_card_read(fd);
			if (character != 'x') {
				charHolder[0] = character;
				strcat(buf,charHolder);
			}
		}
	}
	if (part == 1) {
		while (character != 'x' && divider < MAX_STRING_LENGTH){
			char charHolder[2] = "\0";
			character = alt_up_sd_card_read(fd);
			if (character != 'x') {
				charHolder[0] = character;
				strcat(buf,charHolder);
			}
			divider++;
		}
	}
	else {
		int skip;
		//need to skip to where first half left off
		for (skip = 0; skip < toSkip; skip++) {
			character = alt_up_sd_card_read(fd);
		}
		while (character != 'x' && divider < MAX_STRING_LENGTH){
			char charHolder[2] = "\0";
			character = alt_up_sd_card_read(fd);
			if (character != 'x') {
				charHolder[0] = character;
				strcat(buf,charHolder);
			}
			divider++;
		}
	}

	return;
}

/*
 * Puts the first entry in a string
 *
 * @param: log - the logfile being read from
 * @param: start - the start string
 * @param: part - 1 for the first half of the string, 2 for the second etc. 0 for the whole thing
 */
void firstLogEntry(int log, char* start, int part) {
	short int myFileHandle;
	char character = 'a';

	char logname[20];

	sprintf(logname, "log%d.txt", log);

	int toSkip = (part-1) * MAX_STRING_LENGTH;
	int divider = 0;

	if((myFileHandle = alt_up_sd_card_fopen(logname, false)) != -1) {

		if (part == 0) {
			while (character != 'x'){
				char charHolder[2] = "\0";
				character = alt_up_sd_card_read(myFileHandle);
				if (character != 'x') {
					charHolder[0] = character;
					strcat(start,charHolder);
				}
			}
		}
		if (part == 1) {
			while (character != 'x' && divider < MAX_STRING_LENGTH){
				char charHolder[2] = "\0";
				character = alt_up_sd_card_read(myFileHandle);
				if (character != 'x') {
					charHolder[0] = character;
					strcat(start,charHolder);
				}
				divider++;
			}
		}
		else {
			int skip;
			//need to skip to where first half left off
			for (skip = 0; skip < toSkip; skip++) {
				character = alt_up_sd_card_read(myFileHandle);
			}
			while (character != 'x' && divider < MAX_STRING_LENGTH){
				char charHolder[2] = "\0";
				character = alt_up_sd_card_read(myFileHandle);
				if (character != 'x') {
					charHolder[0] = character;
					strcat(start,charHolder);
				}
				divider++;
			}
		}

		alt_up_sd_card_fclose(myFileHandle);
	}
	else {
		printf("File not opened for reading\n");
		Init_SDCard();
	}
	return;
}

/*
 * Finds out how many logs there are in the SD card
 *
 * @param: first - the first log in the SD card
 * @return: log - the last log in the SD card
 */
int lastLog(int first) {
	short int myFileHandle;
	int log = first;

	char logname[20];

	sprintf(logname, "log%d.txt", log);

	while((myFileHandle = alt_up_sd_card_fopen(logname, false)) >= 0) {
		log++;
		sprintf(logname, "log%d.txt", log);
		alt_up_sd_card_fclose(myFileHandle);
	}

	if (log==0) {
		printf("no files opened in lastLog!\n");
	}

	return log;
}

/*
 * Finds out the first log in the SD card
 *
 * @return: log - the first log in the SD card
 */
int firstLog(void) {
	short int myFileHandle;
	int log = 0;

	char logname[20];

	sprintf(logname, "log%d.txt", log);

	myFileHandle = alt_up_sd_card_fopen(logname, false);
	if (myFileHandle >= 0) {
		log++;
		alt_up_sd_card_fclose(myFileHandle);
		return log;
	}

	while((myFileHandle = alt_up_sd_card_fopen(logname, false)) < 0) {
		log++;
		sprintf(logname, "log%d.txt", log);
	}

	printf("returning this log %d\n", log);
	return log;
}

/*
 * Finds out if there are any logs in the SD card
 *
 * @return: 1 if true, 0 if false
 */
int checkIfLog(void) {
	int myFileHandle = alt_up_sd_card_fopen("log0.txt", false);

	if (myFileHandle >= 0) {
		alt_up_sd_card_fclose(myFileHandle);
		return 1;
	}
	else {
		return 0;
	}
}

void Init_SDCard(void) {
		int connected = 0;
		alt_up_sd_card_dev *device_reference = NULL;

		if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
		{
			printf("SDCard Open FAILED\n");
			return;
		}
		else
			printf("SDCard Open PASSED\n");

		if (device_reference != NULL ) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())){
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
				}
				else {
					printf("Unknown file system.\n");
				}
					connected = 1;
			}
			else{
				printf("No SD card detected\n");
				DrawString(280,450,"Please insert SD card", sizeof("Please insert SD card")-1, RED, GRAY);
				misc_sleep(2);
				Init_SDCard();
			}
		}
}
