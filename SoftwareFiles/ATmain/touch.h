/*
 * touch.h
 *
 *  Created on: Feb 3, 2016
 *      Author: mitchell
 */

#ifndef TOUCH_H_
#define TOUCH_H_

#define Touch_RX (*(volatile unsigned char *)(0x84000232))
#define Touch_TX (*(volatile unsigned char *)(0x84000232))
#define Touch_Control (*(volatile unsigned char *)(0x84000230))
#define Touch_Status (*(volatile unsigned char *)(0x84000230))
#define Touch_Baud (*(volatile unsigned char *)(0x84000234))

typedef struct { int x, y; } Point ;

void Init_Touch(void);

void WaitForTouch(void);

Point GetPress(void);

Point GetRelease(void);




#endif /* TOUCH_H_ */
