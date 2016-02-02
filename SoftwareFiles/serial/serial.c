//Update January 21 12:26pm

#include <stdio.h>
#include <unistd.h>

#define RS232_Control (*(volatile unsigned char *)(0x84000200))
#define RS232_Status (*(volatile unsigned char *)(0x84000200))
#define RS232_TxData (*(volatile unsigned char *)(0x84000202))
#define RS232_RxData (*(volatile unsigned char *)(0x84000202))
#define RS232_Baud (*(volatile unsigned char *)(0x84000204))

/* Set up 6850 control register to utilise a divide by 16 clock,
 * RTS low, use 8 bits of data, no parity, 1 stop bit, transmitter
 * interrupt disabled.
 *
 * Counter Divide Select: 								CR1 = 0, CR0 = 1
 * Word Select Bits: 									CR4 = 1, CR3 = 0, CR2 = 1
 * Transmitter Control Bits: 							CR6 = 0, CR7 = 0
 * (don't know about the receive interrupt enable)		CR8 = 0
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

/* Initialises the RS232 port by writing to RS232_Control and RS232_Baud
 * registers.
 *
 * @param: none
 * @return: none
 */
void Init_RS232(void)
{
	RS232_Control = 0x15;
	RS232_Baud = 0x01;
}

/*
 * Finds the value of the transmit bit in 6850 status register.
 * Polls (every 500ms) until it becomes "1".
 * Writes "character" to the 6850 TxData register.
 *
 * @param: int character - integer to transmit
 * @return: none
 */
void putcharRS232(int character)
{
	int tx_bit = 0;
	int status = RS232_Status;

	printf("RS232 status = %d = ", status);
	print_binary(status);

	// poll transmit bit in 6850 status register. Wait for it to become '1'
	tx_bit = transmit_bit(status);
	printf("\nTx_bit = %d\n", tx_bit);
	while(tx_bit == 0)
	{
		// sleep(500); // for some reason can't link unistd.h
		int delay =0;
		while (delay < 200000)
		{
			delay++;
		}
		tx_bit = transmit_bit(RS232_Status);
		printf("\nTx_bit = %d\n", tx_bit);
	}

	// write character to the 6850 TxData register to output the character
	printf("Writing %d to TxData register.\n", character);
	RS232_TxData =  character;

	return;
}

/* NOT YET TESTED
 * Finds the value of the receive bit in 6850 status register.
 * Polls until it becomes "1".
 * Writes "character" to the 6850 TxData register.
 * @param: int character - integer to transmit
 * @return: none
 */
int getcharRS232(void)
{
	int character, rx_bit;
	int status = RS232_Status;

	printf("RS232 status = %d = ", status);
	print_binary(status);

	// poll Rx bit in 6850 status register. Wait for it to become '1'.
	rx_bit = RS232TestForReceivedData(RS232_Status);
	printf("\nRx_bit = %d\n", rx_bit);
	while(rx_bit == 0)
	{
		rx_bit = RS232TestForReceivedData(RS232_Status);
		printf("\nRx_bit = %d\n", rx_bit);
	}

	// read received character from 6850 RxData register.
	character = RS232_RxData;
	printf("Read %d from RxData register.\n", character);
	return character;
}

int main()
{
	char character = 0x34;

	Init_RS232();

	while(1) {
		int delay=0;
		while (delay < 200000)
		{
			delay++;
		}
		putcharRS232(character);
	}

	return 0;
}
