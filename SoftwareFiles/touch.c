#include <stdio.h>
#include <unistd.h>
#include "touch.h"


#define RS232_Control (*(volatile unsigned char *)(0x84000200))
#define RS232_Status (*(volatile unsigned char *)(0x84000200))
#define RS232_TxData (*(volatile unsigned char *)(0x84000202))
#define RS232_RxData (*(volatile unsigned char *)(0x84000202))
#define RS232_Baud (*(volatile unsigned char *)(0x84000204))


/* Set up 6850 control register to utilise a divide by 16 clock,
 * RTS low, use 8 bits of data, no parity, 1 stop bit, transmitter
 * interrupt disabled.
 *
 * Counter Divide Select:                               CR1 = 0, CR0 = 1
 * Word Select Bits:                                    CR4 = 1, CR3 = 0, CR2 = 1
 * Transmitter Control Bits:                            CR6 = 0, CR7 = 0
 * (don't know about the receive interrupt enable)      CR8 = 0
 * = 00010101 = 0x15
 */
#define CONTROL 0x15

// Program baud rate generator to use 115k baud (taken from pdf)
#define BAUD 0x01



/* Prints a decimal in binary form
 *
 * @param: int dec - the decimal to display in binary
 * @return: none
 */
void print_binary(int dec)
{
    int i; //number of bits to display (minus 1)
    int k;

    for (i = 7; i >= 0; i--)
    {
        k = dec >> i;

        if (k & 1)
            printf("1");
        else
            printf("0");
    }
}

/* Returns the second bit (transmit bit) from the status register
 *
 * @param: int reg - the value of the status register
 * @return: int bit - the second bit of the status register
 */
int transmit_bit(int reg)
{
    int bit;
    // shift over 1
    bit = reg >> 1;
    // bitwise and
    bit = bit & 1;
    return bit;
}

/* Returns the first bit (receive bit) from the status register
 *
 * @param: int reg - the value of the status register
 * @return: int bit - the first bit of the status register
 */
int RS232TestForReceivedData(int reg)
{
    int bit = reg & 1;
    return bit;
}


/* Initializes the Touch Controller
 *
 * @return: none
 */
void Init_Touch(void)
{
    // From ex 1.3 documents
    Touch_Control = 0x15;
    Touch_Baud = 0x5;

    int k;
    for(k = 0; k<1000; k++);

    Touch_TX = 0x55;
    int i;
    for(i = 0; i<1000; i++);

    Touch_TX = 0x01;
    int t;
    for(t = 0; t<1000; t++);
    Touch_TX = 0x12;

}


// Tests to see if the screen is touched,
// Returns True if there was a touch even
// Returns False otherwise.
int ScreenTouched( void )
{
    if (Touch_RX == 0x80||Touch_RX == 0x81){
        return 1;
    } else {
        return 0;
    }
}

// Waits for the screen to be touched
void WaitForTouch()
{
    while(!ScreenTouched());

}

// Waits for a touch screen press and returns the point
Point GetPress(void)
{
    // p1 is a point to be returned, empty is a null point to be used in the case of an error
    Point p1, empty;

    int X_Byte1, X_Byte2, Y_Byte1, Y_Byte2;
    WaitForTouch();
    if (Touch_RX == 0x80) return empty;

    while(Touch_Status == 0 || Touch_RX == 0x80 || Touch_RX == 0x81);
    X_Byte1 = Touch_RX;

    while(Touch_Status == 0 || Touch_RX == 0x80 || Touch_RX == 0x81 || Touch_RX == X_Byte1);
    X_Byte2 = Touch_RX;

    while(Touch_Status == 0 || Touch_RX == 0x80 || Touch_RX == 0x81 || Touch_RX == X_Byte1 || Touch_RX == X_Byte2 );
    Y_Byte1 = Touch_RX;

    while(Touch_Status == 0 || Touch_RX == 0x80 || Touch_RX == 0x81 || Touch_RX == X_Byte1 || Touch_RX == X_Byte2 || Touch_RX == Y_Byte1 );
    Y_Byte2 = Touch_RX;


    X_Byte2 = X_Byte2 << 7;
    Y_Byte2 = Y_Byte2 << 7;

    int x = X_Byte2+X_Byte1;
    int y = Y_Byte2+Y_Byte1;

    x = x*800/4095;
    y = y*480/4095;



    if(x > 800 || y > 480){
        return empty;
    }


    printf("x: %d, y: %d\n", x, y);

    p1.x = x;
    p1.y = y;



    // wait for a pen down command then return the X,Y coord of the point
    // calibrated correctly so that it maps to a pixel on screen
    return p1;
}

// This waits for a touch screen release event, and returns a point
Point GetRelease()
{
    // p1 is a point to be returned, empty is a null point to be used in the case of an error
        Point p1, empty;

        int X_Byte1, X_Byte2, Y_Byte1, Y_Byte2;
        WaitForTouch();

        if (Touch_RX == 0x81) return empty;
        while(Touch_Status == 0 || Touch_RX == 0x80 || Touch_RX == 0x81){
            if (Touch_RX == 0x81) return empty;
        }
        X_Byte1 = Touch_RX;

        while(Touch_Status == 0 || Touch_RX == 0x80 || Touch_RX == 0x81 || Touch_RX == X_Byte1){
            if (Touch_RX == 0x81) return empty;
        }
        X_Byte2 = Touch_RX;

        while(Touch_Status == 0 || Touch_RX == 0x80 || Touch_RX == 0x81 || Touch_RX == X_Byte1 || Touch_RX == X_Byte2 ){
            if (Touch_RX == 0x81) return empty;
        }
        Y_Byte1 = Touch_RX;

        while(Touch_Status == 0 || Touch_RX == 0x80 || Touch_RX == 0x81 || Touch_RX == X_Byte1 || Touch_RX == X_Byte2 || Touch_RX == Y_Byte1 ){
            if (Touch_RX == 0x81) return empty;
        }
        Y_Byte2 = Touch_RX;


        X_Byte2 = X_Byte2 << 7;
        Y_Byte2 = Y_Byte2 << 7;

        int x = X_Byte2+X_Byte1;
        int y = Y_Byte2+Y_Byte1;

        x = x*800/4095;
        y = y*480/4095;
        if (Touch_RX == 0x81) return empty;

        if(x > 800 || y > 480){
            return empty;
        }

        //printf("x: %d, y: %d\n", x, y);

        p1.x = x;
        p1.y = y;

        // wait for a pen down command then return the X,Y coord of the point
        // calibrated correctly so that it maps to a pixel on screen
        return p1;
}


