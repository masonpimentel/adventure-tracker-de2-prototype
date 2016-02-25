/*
 * misc.c
 *
 *  Created on: Feb 11, 2016
 *      Author: Mason
 */


void misc_sleep(int seconds) {
	int i = 0;

	for (i=0; i<seconds; i++) {
		usleep(1000000);
	}
}


