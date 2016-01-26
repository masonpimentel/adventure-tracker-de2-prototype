#define Bluetooth_Control (*(volatile unsigned char *)(0x84000200))
#define Bluetooth_Status (*(volatile unsigned char *)(0x84000200))
#define Bluetooth_TxData (*(volatile unsigned char *)(0x84000222))
#define Bluetooth_RxData (*(volatile unsigned char *)(0x84000222))
#define Bluetooth_Baud (*(volatile unsigned char *)(0x84000204))

#include <stdio.h>

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
int BluetoothTestForReceivedData(int reg)
{
	int bit = reg & 1;
	return bit;
}

/*
 * Finds the value of the transmit bit in 6850 status register.
 * Polls (every 500ms) until it becomes "1".
 * Writes "character" to the 6850 TxData register.
 *
 * @param: int character - integer to transmit
 * @return: none
 */
void putcharBluetooth(int character)
{
	int tx_bit = 0;
	int status = Bluetooth_Status;

	printf("Bluetooth status = %d = ", status);
	print_binary(status);

	// poll transmit bit in 6850 status register. Wait for it to become '1'
	tx_bit = transmit_bit(status);
	printf("\nTx_bit = %d\n", tx_bit);
	while(tx_bit == 0)
	{
		// sleep(500); // for some reason can't link unistd.h
		int delay = 0;
		while (delay < 200000)
		{
			delay++;
		}
		tx_bit = transmit_bit(Bluetooth_Status);
		printf("\nTx_bit = %d\n", tx_bit);
	}

	// write character to the 6850 TxData register to output the character
	printf("Writing %d to TxData register.\n", character);
	Bluetooth_TxData =  character;

	return;
}

/* NOT YET TESTED
 * Finds the value of the receive bit in 6850 status register.
 * Polls until it becomes "1".
 * Writes "character" to the 6850 TxData register.
 * @param: int character - integer to transmit
 * @return: none
 */
int getcharBluetooth(void)
{
	int character, rx_bit;
	int status = Bluetooth_Status;

	printf("Bluetooth status = %d = ", status);
	print_binary(status);

	// poll Rx bit in 6850 status register. Wait for it to become '1'.
	rx_bit = BluetoothTestForReceivedData(Bluetooth_Status);
	printf("\nRx_bit = %d\n", rx_bit);
	while(rx_bit == 0)
	{
		rx_bit = BluetoothTestForReceivedData(Bluetooth_Status);
		printf("\nRx_bit = %d\n", rx_bit);
	}

	// read received character from 6850 RxData register.
	character = Bluetooth_RxData;
	printf("Read %d from RxData register.\n", character);
	return character;
}

void smallDelay() {
	int delay = 0;
	while (delay < 200000)
	{
		delay++;
	}
}

int main()
{
	char character = 0x24;
	while (1) {
		putcharBluetooth(character);
		smallDelay();
		putcharBluetooth(character);
		smallDelay();
		putcharBluetooth(character);
	}
	return 0;
}
