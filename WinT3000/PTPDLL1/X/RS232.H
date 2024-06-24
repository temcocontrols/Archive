// ************************* START OF RS232.H *************************
//
//
//
// This header file contains the definitions for the base class RS232.
//
// ********************************************************************
#ifndef _RS232_DOT_H
#define _RS232_DOT_H
//#include <vcl\vcl.h>
#include "t3000def.h"

//
// These constants are used as parameters to RS232 member functions.
//

const DInt UNCHANGED = -1;
const DInt FOREVER = -1;
const DInt DISABLE = 0;
const DInt CLEAR = 0;
const DInt ENABLE = 1;
const DInt SET = 1;
const DInt RESET = 1;
const DInt REMOTE_CONTROL = -1;

//
// The Settings class provides a convenient mechanism for saving or
// assigning the state of a port.
//

typedef class
{
	 public :
			DLong BaudRate;
			DByte Parity;
			DByte WordLength;
			DByte StopBits;
			DByte Dtr;
			DByte Rts;
			DByte XonXoff;
			DByte RtsCts;
			DByte DtrDsr;
			void Adjust( DLong baud_rate,
							DByte parity,
							DByte word_length,
							DByte stop_bits,
							DByte dtr,
							DByte rts,
							DByte xon_xoff,
							DByte rts_cts,
							DByte dtr_dsr );
} SETTINGS;

// This structure defines the layout of the modem capability
// database elements.  At present, all of the definitions that
// can be used to define a particular brand or type of modem
// are stored in a static array in MODEM.CPP.  A commercial
// application might store these off line in a database.

struct ModemCapabilities {
		DByte name[41];
		DByte first_init_string[41];
		DByte second_init_string[41];
		DByte fail_strings[61];
		DByte compression_strings[41];
		DByte protocol_strings[41];
		DLong initial_baud_rate;
		DLong locked_baud_rate;
		DByte handshaking;
};

typedef struct {
	DByte             PortName;
	DByte	          IntName;
	DByte             Connection;
	SETTINGS          Settings;
	ModemCapabilities modemCapabilities;
	DByte             Baudrateindex;
	DByte             Modemtypeindex;
	HANDLE            handle;
}	COMMINFO;


//
// Class RS232 is the abstract base class used for any serial port
// class.  RS232 cannot be instantiated.  Only fully defined classes
// derived from RS232 can actually be created and used.
//

class RS232
{
	 protected :
		RS232PortName port_name;
		SETTINGS      saved_settings;
	 public:
		SETTINGS      settings;
	 protected :
		RS232Error    error_status;
//	   DInt debug_line_count;

// Mandatory protected functions

		virtual DInt read_buffer( char *buffer,
							DUint count ) = 0;
		virtual DInt write_buffer( char *buffer,
							 DUint count = -1, DInt type = 0 ) = 0;
		virtual DInt read_byte( void ) = 0;
		virtual DInt write_byte( DInt c ) = 0;

	 public :
		DUint  ByteCount;
		DLong ElapsedTime;

//  Mandatory functions.  All derived classes must define these.

		virtual RS232Error Set( DLong baud_rate = UNCHANGED,
						  DInt parity = UNCHANGED,
						  DInt word_length = UNCHANGED,
						  DInt stop_bits = UNCHANGED ) = 0;
		virtual DInt TXSpaceFree( void ) = 0;
		virtual DInt RXSpaceUsed( void ) = 0;
		virtual DInt Cd( void ) = 0;
		virtual DInt Ri( void ) = 0;
		virtual DInt Cts( void ) = 0;
		virtual DInt Dsr( void ) = 0;
		virtual DInt ParityError( DInt clear = UNCHANGED ) = 0;
		virtual DInt BreakDetect( DInt clear = UNCHANGED ) = 0;
		virtual DInt FramingError( DInt clear = UNCHANGED ) = 0;
		virtual DInt HardwareOverrunError( DInt clear = UNCHANGED ) = 0;

// Optional Functions.  Derived class are not required to support these.

		virtual ~RS232( void ){ ; }
		virtual DInt Break( DLong milliseconds = 300 );
		virtual DInt SoftwareOverrunError( DInt clear = UNCHANGED );
		virtual DInt XonXoffHandshaking( DInt setting = UNCHANGED );
		virtual DInt RtsCtsHandshaking( DInt setting = UNCHANGED );
		virtual DInt DtrDsrHandshaking( DInt setting = UNCHANGED );
		virtual DInt Dtr( DInt setting = UNCHANGED );
		virtual DInt Rts( DInt setting = UNCHANGED );
		virtual DInt Peek( void *buffer, DUint count );
		virtual DInt RXSpaceFree( void );
		virtual DInt TXSpaceUsed( void );
		virtual DInt FlushRXBuffer( void );
		virtual DInt FlushTXBuffer( void );
//		virtual char *ErrorName( DInt error );
		virtual DInt IdleFunction( void );
/*        virtual DInt FormatDebugOutput( char *buffer = 0,
								 DInt line_number = -1 );*/

// Non virtual functions.  These work the same for all classes.

		DInt Read( void *buffer,
				DUint  count,
				DLong milliseconds = 0 );
		DInt Read( void *buffer,
				DUint count,
				DLong milliseconds,
				char *terminator );
		DInt Write( void *buffer,
				 DUint count = 0,
				 DLong milliseconds = 0,
				 char *terminator = 0 );
		DInt Read( DLong milliseconds = 0 );
		DInt Write( DInt c, DLong milliseconds = 0 );
		DInt Peek( void );
		DInt ReadSettings( SETTINGS &copy ) { copy = settings;
									return RS232_SUCCESS; }
		DLong ReadBaudRate( void ) { return settings.BaudRate; }
		RS232Error ErrorStatus( void ) { return error_status; }
//	   DInt DebugLineCount( void ) { return debug_line_count; }
};


// A Miscellaneous support function.  This may be implemented
// differently by different environments.

DUlong ReadTime( void );

#endif  // #ifndef _RS232_DOT_H

// *************************** END OF RS232.H ***************************
