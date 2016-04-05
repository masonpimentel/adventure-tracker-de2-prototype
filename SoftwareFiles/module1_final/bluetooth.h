void Init_Bluetooth(void);
void changePIN(char* pin);
void changeName(char* name);
void changePINoriginal(void);
void evenLongerDelay(void);
void enterCMD(void);
void exitCMD(void);
char getcharBluetooth(void);
void BtReboot(void);
void pollTxandWrite(int character);
void smallDelay(void);
int receive_command_bluetooth(void);
void send_string_bluetooth(char* msg, int len);
void execute_cmd_bluetooth(int cmd);

