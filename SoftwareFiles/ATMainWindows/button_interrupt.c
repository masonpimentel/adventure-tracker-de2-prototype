/*
 * button_interrupt.c
 *
 *  Created on: Feb 11, 2016
 *      Author: Mason
 */

#include <stdio.h>
#include "button_interrupt.h"

void interrupt_handler(void)
{
	int ipending;
	ipending = __builtin_rdctl(4); 	//Read the ipending register
	if ((ipending & 0x2) == 2) 		//If irq1 is high, run pushbutton_isr, otherwise return
	{
		pushbutton_isr();
	}
	return;
}

void pushbutton_isr(void)
{
	//TODO: go to newTrip
	printf("Nyess!!");

	return;
}

void Init_Button_ISR(void)
{

	volatile int * pushbuttons = (int *) PUSH_BUTTONS;
	*(pushbuttons + 2) = 0x8; //Enable KEY3 to enable interrupts
	__builtin_wrctl(3, 2); //Write 2 into ienable register
	__builtin_wrctl(0, 1); //Write 1 into status register

	printf("Waiting for interrupt\n");
	while(1) {
		printf("Waiting for interrupt\n");
		misc_sleep(1);
	}
	return;
}





