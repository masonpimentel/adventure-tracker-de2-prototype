#include <stdio.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"
#include "touch.h"
#include "sdcard.h"
#include <string.h>

int moveToEnd(int length) {
	short int myFileHandle;
	int i;
	int character = 1;

	if((myFileHandle = alt_up_sd_card_fopen("LOG.TXT", false)) != -1) {
		printf("File opened for scanning\n");

		while (character != -1) {
			character = alt_up_sd_card_read(myFileHandle);
			//printf("ASCII character %i (decimal) read\n", character);
		}
		printf("Done scanning!!!\n");
		return myFileHandle;
	}
	else {
		printf("File not opened for scanning\n");
		return -1;
	}
}

int writeToSd(char* string, char log, int length) {
	int i;
	int response;
	short int myFileHandle;
	char modified[100];

	myFileHandle = moveToEnd(length);

	if(myFileHandle >= 0) {
		printf("File Opened for writing\n");

		printf("Writing '%s' to SD card\n", string);
		//modified[0] = 0;
		//strcat(modified, string);
		//strcat(modified, "\r\n");
		//printf("Modified '%s'\n", modified);
		//length++;
		printf("length = %d\n", length);
		for(i = 0; i < length; i ++) {
			if(alt_up_sd_card_write(myFileHandle,*string) == false){
				printf("Error writing to file...\n");
				return -1;
			}
			string++;
		}
		printf("Done!!!\n");
		alt_up_sd_card_fclose(myFileHandle);
	}
	else
		printf("File not opened for writing\n");
	//alt_up_sd_card_fclose(myFileHandle);
	return myFileHandle;
}

void readFromSd(int length, int fileHandle) {
	short int myFileHandle;
	int i;
	int character;

	if((myFileHandle = alt_up_sd_card_fopen("LOG5.TXT", false)) != -1) {
		//printf("File Opened for reading\n");

		for(i = 0; i < length; i ++) {
			character = alt_up_sd_card_read(myFileHandle);
			printf("ASCII character %i (decimal) read\n", character);
		}
		printf("Done!!!\n");
		alt_up_sd_card_fclose(myFileHandle);
	}
	else
		printf("File not opened for reading\n");

	//printf("Please re-insert SD card to perform another operation.\n");
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
