#include <stdio.h>
#include <unistd.h>

#define RS232_Control (*(volatile unsigned char *)(0x84000200))
#define RS232_Status (*(volatile unsigned char *)(0x84000200))
#define RS232_TxData (*(volatile unsigned char *)(0x84000202))
#define RS232_RxData (*(volatile unsigned char *)(0x84000202))
#define RS232_Baud (*(volatile unsigned char *)(0x84000204))

/* Subroutine to initialise the RS232 Port by writing some data
** to the internal registers.
** Call this function at the start of the program before you attempt
** to read or write to data via the RS232 port
**
** Refer to 6850 data sheet for details
***************************************************************************/
void print_binary(int dec)
{
	int c, k;

	for (c = 10; c >= 0; c--)
	{
		k = dec >> c;

		if (k & 1)
			printf("1");
		else
			printf("0");
	}
}

void Init_RS232(void)
{
	// set up 6850 Control Register to utilise a divide by 16 clock,
	// set RTS low, use 8 bits of data, no parity, 1 stop bit,
	// transmitter interrupt disabled
	RS232_Control = 0x15;
	/* Counter Divide Select: 								CR1 = 0, CR0 = 1
	 * Word Select Bits: 									CR4 = 1, CR3 = 0, CR2 = 1
	 * Transmitter Control Bits: 							CR6 = 0, CR7 = 0
	 * (don't know about the recieve interrupt enable)		CR8 = 0
	 * = 00010101 = 0x15
	 */

	// program baud rate generator to use 115k baud
	RS232_Baud = 0x01;
}
int putcharRS232(int c)
{
	int tx_bit = 0;

	int premask = RS232_Status;

	printf(premask);


	/*
	while(tx_bit == 0) {

		sleep(1000);
	}
	*/
	// poll Tx bit in 6850 status register. Wait for it to become '1'
	// write 'c' to the 6850 TxData register to output the character
	return c;
}
int getcharRS232( void )
{
// poll Rx bit in 6850 status register. Wait for it to become '1'
// read received character from 6850 RxData register.
}
// the following function polls the 6850 to determine if any character
// has been received. It doesn't wait for one, or read it, it simply tests
// to see if one is available to read
int RS232TestForReceivedData(void)
{
// Test Rx bit in 6850 serial comms chip status register
// if RX bit is set, return TRUE, otherwise return FALSE
}

int main()
{
	int character = 4;
	printf("Starting serial.c\n");

	//Init_RS232();

	//putcharRS232(character);
	/*
	while(1) {
		getcharRS232();
	}
	*/

	return 0;
}
