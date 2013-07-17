#ifndef _BURN_H_
#define JTAG_PROTOCOL			0x00
#define C2_PROTOCOL				0x01
struct data {
	const char* hex;
	int erase;
	int disableDialog;
	int lock;
	int goRun;
};
#endif