#include <stdio.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "touch.h"
#include "sdcard.h"
#include <string.h>

/*
 * Called by writeToSd to seek to the end of the log
 */
int moveToEnd(int length, char* logname) {
	short int myFileHandle;
	int i;
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
		alt_up_sd_card_fclose(myFileHandle);
		return -1;
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
	int response;
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
	else
		printf("File not opened for writing\n");
	//alt_up_sd_card_fclose(myFileHandle);
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

	printf("logname in read = %s\n", logname);

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
		alt_up_sd_card_fclose(myFileHandle);

	}
	else
		printf("File not opened for reading\n");

}

void Init_SDCard(void) {
		int i;
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
		}
}

//for reference

/*
void writeToFile(void) {
	short int myFileHandle;
	int i;

	if((myFileHandle = alt_up_sd_card_fopen("text8.txt", true)) != -1) {
		printf("File Opened\n");

		for(i = 0; i < 5; i ++) {
			if(alt_up_sd_card_write(myFileHandle,'A') == false){
				printf("Error writing to file...\n");
				return;
			}
		}
		printf("Done!!!\n");
		alt_up_sd_card_fclose(myFileHandle);
	}
	else
		printf("File NOT Opened\n");

	//printf("Please re-insert SD card to perform another operation.\n");
}

void readFromFile(void) {
	short int myFileHandle;
	int i;
	int character;

	if((myFileHandle = alt_up_sd_card_fopen("text2.txt", false)) != -1) {
		printf("File Opened\n");

		for(i = 0; i < 5; i ++) {
			character = alt_up_sd_card_read(myFileHandle);
			printf("ASCII character %i (decimal) read\n", character);
		}
		printf("72 = H, 69 = E, 76 = L, 79 = O\n");
		printf("Done!!!\n");
		alt_up_sd_card_fclose(myFileHandle);
	}
	else
		printf("File NOT Opened\n");

	//printf("Please re-insert SD card to perform another operation.\n");
}


void sdcard(void){
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	int response;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return;
	}
	else
		printf("SDCard Open PASSED\n");


	if (device_reference != NULL ) {
		while(1) {
			if ((connected == 0) && (alt_up_sd_card_is_Present())){
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");

					while(1) {
						printf("Enter 0 to write to text1.txt or 1 to read from text2.txt: ");
						scanf("%d", &response);

						if (response == 0) {
							writeToFile();
						}

						if (response == 1) {
							readFromFile();
						}

						if (alt_up_sd_card_is_Present() == false) {
							break;
						}
					}
				}
				else {
					printf("Unknown file system.\n");
				}
				connected = 1;
			} else if((connected == 1) && (alt_up_sd_card_is_Present() == false)){
				printf("Card disconnected.\n");
				connected =0;
			}
		}
	}
	else
		printf("Can't open device\n");

	return;
}
*/
