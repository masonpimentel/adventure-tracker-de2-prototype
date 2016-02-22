#include <stdio.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "touch.h"
#include "sdcard.h"
#include <string.h>
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
		printf("File opened for scanning, filehandle = %d\n", myFileHandle);

		while (character != -1) {
			character = alt_up_sd_card_read(myFileHandle);
		}
		printf("Done scanning!!!\n");
		return myFileHandle;
	}
	else {
		printf("File not opened for scanning, creating\n");
		myFileHandle = alt_up_sd_card_fopen(logname, true);
		if (myFileHandle < 0) {
			Init_SDCard();
			moveToEnd(length, logname);
		}
	}
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

	printf("logname = %s\n", logname);

	myFileHandle = moveToEnd(length, logname);

	if(myFileHandle >= 0) {
		printf("File Opened for writing\n");

		strcpy(modified, string);
		strcat(modified, " x");
		printf("Writing '%s' to SD card\n", modified);
		length = strlen(modified);
		printf ("length of modified: %d\n", length);
		for(i = 0; i < length; i ++) {
			if(alt_up_sd_card_write(myFileHandle,modified[i]) == false){
				printf("Error writing to file...\n");
				return -1;
			}
		}
		printf("Done!!!\n");
		alt_up_sd_card_fclose(myFileHandle);
	}
	else {
		printf("File not opened for writing\n");
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

	//printf("logname in read = %s\n", logname);

	if((myFileHandle = alt_up_sd_card_fopen(logname, false)) != -1) {
		//printf("File Opened for reading\n");

		for (i=0; i<entry; i++) {
			while(character != 'x') {
				character = alt_up_sd_card_read(myFileHandle);
				//printf("ASCII character %i (decimal) read\n", character);
			}
			character = 'a';
		}
		while(character != 'x') {
			character = alt_up_sd_card_read(myFileHandle);
			printf("ASCII character %i (decimal) read\n", character);
		}
		//printf("Done!!!\n");

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
	//printf("logname in numEntries = %s\n", logname);

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
	printf("this many entries = %d\n", entries);
	return entries;

}



/*
 * Puts the last entry in a string
 *
 * @param: log - the logfile being read from
 * @param: end - the end string
 * @param: part - 1 for the first half of the string, 2 for the second half of the string
 */
void lastLogEntry(int log, char* end, int part)
{
	int toSeek = numEntries(log) - 1;

	printf("toSeek = %d\n", toSeek);

	short int myFileHandle;
	int i;
	char character = 'a';

	char logname[20];
	//char returnEntry[256] = "\0";

	sprintf(logname, "log%d.txt", log);
	//printf("logname in seekToLast = %s\n", logname);

	int toSkip = (part-1) * MAX_STRING_LENGTH;
	int divider = 0;

	if((myFileHandle = alt_up_sd_card_fopen(logname, false)) != -1) {

		//seek through all the 'x's
		for (i=0; i<toSeek; i++) {
			while(character != 'x') {
				character = alt_up_sd_card_read(myFileHandle);
			}
			//printf("incremented\n");
			character = 'a';
		}
		character = 'a';

		if (part == 1) {
			while (character != 'x' && divider < MAX_STRING_LENGTH){
				char charHolder[2] = "\0";
				character = alt_up_sd_card_read(myFileHandle);
				if (character != 'x') {
					charHolder[0] = character;
					//printf("(f)should be putting in %s\n", charHolder);
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
 * Puts the first entry in a string
 *
 * @param: log - the logfile being read from
 * @param: start - the start string
 * @param: part - 1 for the first half of the string, 2 for the second etc.
 */
void firstLogEntry(int log, char* start, int part) {
	short int myFileHandle;
	char character = 'a';

	char logname[20];

	sprintf(logname, "log%d.txt", log);

	int toSkip = (part-1) * MAX_STRING_LENGTH;
	int divider = 0;

	if((myFileHandle = alt_up_sd_card_fopen(logname, false)) != -1) {

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
