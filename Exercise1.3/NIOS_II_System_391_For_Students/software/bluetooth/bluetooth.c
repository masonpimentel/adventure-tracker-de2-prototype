//Updated January 28, 2016

#include <stdio.h>

#define Bluetooth_Control (*(volatile unsigned char *)(0x84000220))
#define Bluetooth_Status (*(volatile unsigned char *)(0x84000220))
#define Bluetooth_TxData (*(volatile unsigned char *)(0x84000222))
#define Bluetooth_RxData (*(volatile unsigned char *)(0x84000222))
#define Bluetooth_Baud (*(volatile unsigned char *)(0x84000224))

/* Delay loops
 */
void smallDelay() {
	int delay = 0;
	while (delay < 2000000)
	{
		delay++;
	}
}

void longerDelay() {
	int delay = 0;
	while (delay < 10000000)
	{
		delay++;
	}
}

void evenLongerDelay() {
	int delay = 0;
	while (delay < 20000000)
	{
		delay++;
	}
}

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

/* Initialises the Bluetooth port by writing to Bluetooth_Control and Bluetooth_Baud
 * registers.
 *
 * @param: none
 * @return: none
 */
void Init_Bluetooth(void)
{
	Bluetooth_Control = 0x15;
	Bluetooth_Baud = 0x01;
}

/*
 * Finds the value of the transmit bit in 6850 status register.
 * Polls until it becomes "1".
 * Writes "character" to the 6850 TxData register.
 *
 * @param: int character - integer to transmit
 * @return: none
 */
void pollTxandWrite(int character) {

	int tx_bit = 0;
	int status = Bluetooth_Status;

	//printf("Bluetooth status = %d = ", status);
	print_binary(status);
	printf("\n");

	// poll transmit bit in 6850 status register. Wait for it to become '1'
	tx_bit = transmit_bit(status);
	//printf("\nTx_bit = %d\n", tx_bit);
	while(tx_bit == 0)
	{
		smallDelay();
		//tx_bit = transmit_bit(Bluetooth_Status);
		//printf("Tx_bit = %d\n", tx_bit);
	}
	printf("Writing %d to TxData register.\n", character);
	Bluetooth_TxData =  character;
}

/*
 * Asks to change name to either "LOL" or "LEL"
 * If "LOL" sends SN,LOL<CR>
 * If "LEL" sends SN,LEL<CR>
 *
 * @param: none
 * @return: none
 */
void changeName(void)
{
	int status = Bluetooth_Status;
	int res;
	int character;

	printf("\nChanging name now\n\n");

	printf("Enter 0 to change name to LOL or 1 to change to LEL:  ");
	scanf("%i", &res);
	printf("Entered: %i\n", res);
	if (res == 0) {
		// write S
		pollTxandWrite(83);
		smallDelay();
		// write N
		pollTxandWrite(78);
		smallDelay();
		// write ,
		pollTxandWrite(44);
		smallDelay();
		// write L
		pollTxandWrite(76);
		smallDelay();
		// write O
		pollTxandWrite(79);
		smallDelay();
		// write L
		pollTxandWrite(76);
		smallDelay();
		// write CR
		pollTxandWrite(13);
		smallDelay();
	}

	if (res == 1) {
		// write S
		pollTxandWrite(83);
		smallDelay();
		// write N
		pollTxandWrite(78);
		smallDelay();
		// write ,
		pollTxandWrite(44);
		smallDelay();
		// write L
		pollTxandWrite(76);
		smallDelay();
		// write E
		pollTxandWrite(69);
		smallDelay();
		// write L
		pollTxandWrite(76);
		smallDelay();
		// write CR
		pollTxandWrite(13);
		smallDelay();
	}

	return;
}

/*
 * Asks to change PIN to either "4321" or "1234"
 * If "4321" sends SP,4321<CR>
 * If "1234" sends SP,1234<CR>
 *
 * @param: none
 * @return: none
 */
void changePIN(void)
{
	int status = Bluetooth_Status;
	int res;
	int character;

	printf("\nChanging PIN now\n\n");

	printf("Enter 0 to change PIN to 4321 or 1 to change to 1234:  ");
	scanf("%i", &res);
	printf("Entered: %i\n", res);
	if (res == 0) {
		// write S
		pollTxandWrite(83);
		smallDelay();
		// write P
		pollTxandWrite(80);
		smallDelay();
		// write ,
		pollTxandWrite(44);
		smallDelay();
		// write 4
		pollTxandWrite(52);
		smallDelay();
		// write 3
		pollTxandWrite(51);
		smallDelay();
		// write 2
		pollTxandWrite(50);
		smallDelay();
		// write 1
		pollTxandWrite(49);
		smallDelay();
		// write CR
		pollTxandWrite(13);
		smallDelay();
	}

	if (res == 1) {
		// write S
		pollTxandWrite(83);
		smallDelay();
		// write P
		pollTxandWrite(80);
		smallDelay();
		// write ,
		pollTxandWrite(44);
		smallDelay();
		// write 1
		pollTxandWrite(49);
		smallDelay();
		// write 2
		pollTxandWrite(50);
		smallDelay();
		// write 3
		pollTxandWrite(51);
		smallDelay();
		// write 4
		pollTxandWrite(52);
		smallDelay();
		// write CR
		pollTxandWrite(13);
		smallDelay();
	}

	return;
}

/*
 * Sends "$$$" to enter CMD mode
 *
 * @param: none
 * @return: none
 */
void enterCMD(void)
{
	printf("Entering CMD mode now\n\n");
	// write -
	pollTxandWrite(36);
	smallDelay();
	// write -
	pollTxandWrite(36);
	smallDelay();
	// write -
	pollTxandWrite(36);
	smallDelay();
	return;
}

/*
 * Sends "---<CR>" to exit CMD mode
 *
 * @param: none
 * @return: none
 */
void exitCMD(void)
{
	printf("\nExiting CMD mode now\n\n");
	// write -
	pollTxandWrite(45);
	smallDelay();
	// write -
	pollTxandWrite(45);
	smallDelay();
	// write -
	pollTxandWrite(45);
	smallDelay();
	// write CR
	pollTxandWrite(13);
	smallDelay();
	return;
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
/*
 * Reads "character" from the 6850 TxData register.
 * Polls for a 1 in the 0th bit of status register first.
 *
 * @param: none
 * @return: character
 */
char getcharBluetooth(void)
{
	int rx_bit = 0;
	int character;
	int status = Bluetooth_Status;
	int i = 0;
	int res;

	printf("\nPolling for characters now\n\n");
	printf("Bluetooth status = %d = ", status);
	print_binary(status);

	// poll recieve bit in 6850 status register. Wait for it to become '1'
	rx_bit = BluetoothTestForReceivedData(status);
	printf("\nRx_bit before polling = %d\n", rx_bit);
	while(rx_bit == 0 && i<100)
	{
		status = Bluetooth_Status;
		rx_bit = BluetoothTestForReceivedData(status);
		i++;
	}
	printf("Keep checking for character? 1 for yes, 0 for no: ");
	scanf("%i", &res);
	printf("Entered: %i\n", res);
	if (res == 1) {
		getcharBluetooth();
	}
	// read character from the 6850 RxData register
	character =  Bluetooth_RxData;
	printf("Recieved '%x' from RxData register.\n", character);

	return character;
}

int main()
{
	Init_Bluetooth();
	evenLongerDelay();

	enterCMD();
	evenLongerDelay();

	getcharBluetooth();
	evenLongerDelay();

	changeName();
	evenLongerDelay();

	changePIN();
	evenLongerDelay();

	exitCMD();
	printf("\nDone");
	return 0;
}
