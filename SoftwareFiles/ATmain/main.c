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




int main()
{
	Init_Touch();
	Init_Gps();
	Init_SDCard();
	current_menu_func = &MainMenu;
	current_menu_func();
	return 0;
}
