/*
 * main.c
 *
 *  Created on: Feb 3, 2016
 *      Author: mitchell
 */


#include "touch.h"
#include "menus.h"
#include "gps.h"
#include "sdcard.h"
#include <stdio.h>
#include "touch.h"
#include "bluetooth.h"

void boot(void)
{
	Init_Touch();
	Init_Gps();
	Init_SDCard();
	Init_Bluetooth();
}

int main()
{
	boot();
	char* msg = "test message";
	send_string_bluetooth(msg, strlen(msg));
	current_menu_func = &MainMenu;
	current_menu_func();
	return 0;
}
