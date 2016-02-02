#include <stdio.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"

void writeToFile(void) {
	short int myFileHandle;
	int i;

	if((myFileHandle = alt_up_sd_card_fopen("text1.txt", true)) != -1) {
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

int main(void){
	alt_up_sd_card_dev *device_reference = NULL;
	int connected = 0;
	int response;

	printf("Opening SDCard\n");
	if((device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0")) == NULL)
	{
		printf("SDCard Open FAILED\n");
		return 0 ;
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

	return 0;
}
