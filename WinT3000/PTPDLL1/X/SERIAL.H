//---------------------------------------------------------------------------
#ifndef serialH
#define serialH
//---------------------------------------------------------------------------
#define DISCONNECT_TIME 33000
#define SERIAL_BUF_SIZE 21000

//typedef enum { BACKGND, FGND } Exec_mode;

//typedef enum { UNUSED_MODE, BLOCK, SIMPLE, SPECIAL_COMMAND } Handling_mode;

//typedef enum { MASTER, SLAVE, STOPED } Serial_mode;

//typedef enum { LOCAL_PORT, MODEM_PORT } Port_type;

//typedef enum { FREE, BUSY } Serial_activity;
#include "rs232.h"

#define PC8250_NEXT_FREE_ERROR   RS232_NEXT_FREE_ERROR
#define PC8250_NEXT_FREE_WARNING RS232_NEXT_FREE_WARNING

typedef enum { T3000_FAIL = PC8250_NEXT_FREE_ERROR, T3000_DESTINATION_ERROR,
			T3000_SOURCE_ERROR, T3000_COMMAND_ERROR, T3000_CHECKSUM_ERROR,
			T3000_SYNC_ERROR, T3000_LENGTH_ERROR, CRC_BAD, COMM_ERROR,
			UNRECOVERABLE_ERROR, USER_ABORT, T3000_NEXT_FREE_ERROR,
			T3000_NEXT_FREE_WARNING = PC8250_NEXT_FREE_WARNING }
			T3000_ERROR;

typedef struct {
	DUint position 	: 8;
	DUint no_points 	: 7;
	DUint bank_type	: 1;
	} Bank_Type;

#endif
