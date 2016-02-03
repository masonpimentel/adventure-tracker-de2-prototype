/*
 * main.c
 *
 *  Created on: Feb 3, 2016
 *      Author: mitchell
 */


#include "graphics.h"
#include "touch.h"
#include <stdio.h>



int main()
{
	Init_Touch();
	DrawMainMenu();
	Point p;
	while(1)
	{
		printf("RX = %x, TX = %x\n", Touch_RX, Touch_TX);
	}
	return 0;
}
