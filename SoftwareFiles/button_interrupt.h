/*
 * button_interrupt.h
 *
 *  Created on: Feb 11, 2016
 *      Author: Mason
 */

#ifndef BUTTON_INTERRUPT_H_
#define BUTTON_INTERRUPT_H_

#define PUSH_BUTTONS (*(volatile unsigned char *)(0x00001060))

void interrupt_handler(void);
void pushbutton_isr(void);
void Init_Button_ISR(void);


#endif /* BUTTON_INTERRUPT_H_ */
