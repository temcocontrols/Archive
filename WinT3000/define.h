#ifndef _DEFINET_H
#define _DEFINET_H

#define time_type	 unsigned long

#define DSignedChar   signed char
#define DChar         signed char
#define DByte         unsigned char
#define DInt          short int
//#define DInt          int 
#define DUint         unsigned short
#define DLong         long
#define DUlong        unsigned long


#define WM_PTP_CONNECTION      		 (WM_USER+400)
#define WM_PTP_RECEPTION       		 (WM_USER+401)
#define WM_PTP_TRANSMISSION    		 (WM_USER+402)
#define WM_PTP_ESTABLISH_CONNECTION  (WM_USER+403)
#define WM_REFRESHGRID               (WM_USER+405)

#define SBProgress 0
#define SBText     1
#define SBCaps     2
#define SBTime     3

#define DIGITAL 0
#define ANALOG 1
#define BLANKCHAR  ""

#define MAX_OUTS	         128        // maxim Output type object instances
#define MAX_INS		         128        // maxim Input  type object instances
#define MAX_VARS	    	 128        // maxim Variable type object instances
#define MAX_CONS		      48        // maxim Controller type object instances
#define MAX_WR		    	  16        // maxim Weekly routine type object instances
#define MAX_AR			       8        // maxim Annual routine type object instances
#define MAX_PRGS	    	  32        // maxim Program type object instances
#define MAX_TABS    		   5        // maxim Table  type object instances
                                        // ( analog user define tables )
#define MAX_DIGM		       1
#define MAX_ANALM	    	  32        // maxim Monitor type object instances
#define MAX_GRPS 	    	  32        // maxim Screen type object instances
#define MAX_SCREENS 	MAX_GRPS	    // alias for MAX_GRPS
#define MAX_ARRAYS	          16        // maxim Array type object instances
#define MAX_UNITS              8        // maxim Unit type object instances
                                        // ( digital user define units )
#define MAX_PASSW             10        // maxim Password type object instances
#define MAX_ALARMS            16
#define MAX_ALARMS_SET        16

#define MAX_ELEMENT_GROUP     48        // maxim Screen Element object instances
                                        // allowed per Screen object
#define MAX_ELEM   MAX_ELEMENT_GROUP    // alias for MAX_ELEMENT_GROUP
#define MAX_BUFFER_SIZE     2000        // max size for a Control Basic program code (binary code)
#define PROGRAM_SIZE        8000        // max size for a Control Basic source (ASCII)
#define MAX_POINTS_IN_MONITOR 14
#define MAX_ICON_NAME_TABLE   16       // 16 is the value for MINI's. If icrease
									   // the value for T3000, some code should be change
									   // in send_grp() in group.cpp , net_call with
									   // ICON_NAME_TABLE_COMMAND command

#define MAX_VIEWS              3

#define YEAR_BYTES		      46
#define MAX_INFO_TYPE         50
#define MAX_STATIONS	      32
#define MAX_DES	           64000

#define MAX_TBL_BANK          15

#define RELEASE_NUMBER		 343
#define MAX_STAND_GROUP_ELEM  64

#define MAX_RETRY		       1

#define MAX_MONSET             1
#define MAX_RECORD            50
#define MAX_ALPHA          	  80
#define SIZE_FULL_LABEL       21
#define SIZE_ABRV              9
#define SIZE_TEXT_RANGE       13         /* 12 characters for range text + null */
#define SIZE_TEXT_UNITS        6         /* 5 characeters plus a null   */

#define T3000_OUT      0         // Output type object
#define T3000_IN   	   1         // Input type object
#define VAR            2         // Variable type object
#define CON            3         // Controller type object
#define WR             4         // Weekly routine type object
#define AR 	           5         // Annual rouitne type object
#define PRG 	         6         // Program type object
#define TBL 	     	   7         // Table type object
#define DMON           8
#define AMON           9         // Monitor type object
#define GRP           10         // Screen type object
#define SCREEN	     GRP         // alias for GRP
#define AY            11         // Array type object

#define ALARMM        12         //
#define UNIT          13         // Unit type object
#define USER_NAME     14         // Password type object
#define PASS   USER_NAME         // alias for USER_NAME
#define ALARMS        15
#define WR_TIME       16         // Time Schedule type object
#define AR_Y          17         // Annual Schedule type object

#define SYSTEM_LIST   20         // System List type object
#define NETSTAT	      21
#define GRAPHIC_GRP   22
#define STATION       24
#define CSTATION      25

#define AYVALUE       29         // Array Element type object
#define ARRAY1        30
#define AMON_INPUTS   33

#define MONSETUP      40

#define  MAX_SAMPLE        200 //300
#define  MAX_INP            14
#define  MAX_SAMPLE_MEM     50 //100
#define MAX_MEM_DIG_BUF  10004    //6004;
#define MAX_HEADERS_AMON    16
#define MAX_HEADERS_DMON    17

#define SIZE_STR_OUT_T3000    40
#define SIZE_STR_IN_T3000     38
#define SIZE_STR_VAR_T3000    36
#define SIZE_STR_CON_T3000    24
#define SIZE_STR_WR_T3000     38
#define SIZE_STR_AR_T3000     32
#define SIZE_STR_PRG_T3000    34
#define SIZE_STR_TBL_T3000   105
#define SIZE_STR_AMON_T3000  147
#define SIZE_STR_SCREEN_T3000 46
#define SIZE_STR_AY_T3000     11   
#define SIZE_STR_SCREEN_ELEM_T3000  32
#define SIZE_STR_WR_ONE_DAY_T3000   16

#define DOUT 	0
#define DIN  	1
#define DVAR 	2
#define DWR 	3
#define DAR 	4
#define DPRG 	5
#define DAMON	6
#define DGRP 	7
#define DAY    8


#define MAN    1
#define AUTO   0
#define DIG1 100

#define YES    1
#define NO     0


#define TEMPERATURE   1
#define ALARM_VERYLOW 1
#define ALARM_LOW     2
#define ALARM_HI      3
#define ALARM_VERYHI  4

#define PGDN   81
#define PGUP   73
#define RIGHT  77
#define LEFT   75
#define HOME   71
#define ENDKEY 79
#define UP     72
#define DOWN   80
#define INSERT 82
#define DELETE_KEY 83
#define ENTER  13

#define F1KEY  59
#define F2KEY  60
#define F3KEY  61
#define F4KEY  62
#define F5KEY  63
#define F6KEY  64
#define F7KEY  65
#define F8KEY  66
#define F9KEY  67
#define F10KEY 68
#define F11KEY 69
#define F12KEY 70

#define CTRL   0x04
#define ALT    0x08

#define MODE_TEXT    1
#define MODE_GRAPHIC 0

#define ASCII_FONT 1
#define EGA_FONT   0

#define INDIVIDUAL -1

#define R_READ_ONLY   0x00
#define R_READ_WRITE  0x01
#define R_NA          0x02
#define R_IND         0x03

#define CNF_BOARD_INFO   0x01
#define CNF_COMM		 0x02
#define CNF_MODEM_INFO   0x04
#define CNF_PATH_INFO    0x08
#define CNF_CONSOLE_INFO 0x10
#define CNF_PASS_INFO    0x20
#define CNF_STATION_INFO 0x40
#define CNF_ALL          0x7F


#define MAXNETWORKPOINTS  30
#define MAXREMOTEPOINTS   30

#define NAME_SIZE  17
#define TIMEOUT_NETCALL 20

#define MAXDATA     2048
#define RXDELAYTIME  100
#define MAXCOMPORTS  4
typedef enum {

		 READOUTPUT_T3000          = T3000_OUT+1,  // read outputs
         READINPUT_T3000           = T3000_IN+1,   // read inputs
         READVARIABLE_T3000        = VAR+1,        // read variables
		 READCONTROLLER_T3000      = CON+1,        // read controllers
         READWEEKLYROUTINE_T3000   = WR+1,         // read weekly routines
		 READANNUALROUTINE_T3000   = AR+1,         // read annual routines
         READPROGRAM_T3000         = PRG+1,        // read programs
         READTABLE_T3000           = TBL+1,        // read tables
		 READMONITOR_T3000         = AMON+1,       // read monitors
         READSCREEN_T3000          = GRP+1,        // read screens
         READARRAY_T3000           = AY+1,         // read arrays
         READARRAYVALUE_T3000      = AYVALUE+1,    // read array elements
		 READTIMESCHEDULE_T3000    = WR_TIME+1,    // read time schedule
		 READANNUALSCHEDULE_T3000  = AR_Y+1,       // read annual schedule
		 READPROGRAMCODE_T3000     = 16,           // read program code
         READGROUPELEMENTS_T3000   = 19,           // read group elements
		 READPOINTINFOTABLE_T3000  = 24,           // read pointinfo table
         UPDATEMEMMONITOR_T3000    = 23,           // read monitor updates
		 READMONITORDATA_T3000     = 22,           // read monitor data
         READINDIVIDUALPOINT_T3000 = 20,           // read individual point
		 READPOINTINFO_T3000       = 25,           // read point info
         TIME_COMMAND              = 21,           // read time
         CLEARPANEL_T3000          = 28,           // clear panel

		 WRITEOUTPUT_T3000         = 100+T3000_OUT+1,  // write outputs
         WRITEINPUT_T3000          = 100+T3000_IN+1,   // write inputs
         WRITEVARIABLE_T3000       = 100+VAR+1,        // write variables
		 WRITECONTROLLER_T3000     = 100+CON+1,        // write controllers
         WRITEWEEKLYROUTINE_T3000  = 100+WR+1,         // write weekly routines
		 WRITEANNUALROUTINE_T3000  = 100+AR+1,         // write annual routines
         WRITEPROGRAM_T3000        = 100+PRG+1,        // write programs
         WRITETABLE_T3000          = 100+TBL+1,        // write tables
		 WRITEMONITOR_T3000        = 100+AMON+1,       // write monitors
         WRITESCREEN_T3000         = 100+GRP+1,        // write screens
         WRITEARRAY_T3000          = 100+AY+1,         // write arrays
		 WRITETIMESCHEDULE_T3000   = 100+WR_TIME+1,    // write time schedule
		 WRITEANNUALSCHEDULE_T3000 = 100+AR_Y+1,       // write annual schedule
		 WRITEPROGRAMCODE_T3000    = 100+16,           // write program code
         WRITEINDIVIDUALPOINT_T3000 = 100+READINDIVIDUALPOINT_T3000,  // write individual point

//       commands with COMMAND_50  
		 COMMAND_50                = 50,
         READ_COMMAND_50           = 50,
         WRITE_COMMAND_50          = COMMAND_50 + 100,
         STATION_LIST_COMMAND      = 21,
         SAVEPROGRAM_COMMAND       = 30,
         LOADPROGRAM_COMMAND       = 31,
         DEFAULT_PRG_COMMAND       = 32,

         READFILES_COMMAND         = 40,  // read the files of type define in
                                          // a previous SETFILETYPE_COMMAND command
                                          // from the current path.
                                          // The data returned are an array of type
                                          //  char [13];
         READDIRECTORIES_COMMAND   = 41,  // read the directories name from
                                          // the current path.
                                          // The data returned are of type
                                          //  (*char)[13];
         GETCURRENTPATH_COMMAND    = 42,  // get the current path
                                          // The command is a read command:
                                          //  command  = READ_COMMAND_50
                                          //  arg      = GETCURRENTPATH_COMMAND
         SETCURRENTPATH_COMMAND    = 43,  // set the current path for the subsequent
                                          // read directories and read files command
                                          // The command is a write command:
                                          //  command  = WRITE_COMMAND_50
                                          //  arg      = SETCURRENTPATH_COMMAND
         SETFILETYPE_COMMAND       = 44,  // set the file type (ex. "*.*", "*.prg").
                                          // the next read files command will
                                          // return only the files of type
                                          // set in a SETFILETYPE_COMMAND command.
                                          // The command is a write command:
                                          //  command  = WRITE_COMMAND_50
                                          //  arg      = SETFILETYPE_COMMAND
         ALARM_NOTIFY_COMMAND      = 51,

		 TABLEPOINTS_COMMAND       = 75,
		 PANEL_INFO1_COMMAND       = 110,
         PANEL_INFO2_COMMAND       = 111,
         MINICOMMINFO_COMMAND      = 112,
		 PANELID_COMMAND           = 113,
         ICON_NAME_TABLE_COMMAND   = 114,
         WRITEDATAMINI_COMMAND     = 116,
		 SENDCODEMINI_COMMAND      = 117,
         SENDDATAMINI_COMMAND      = 118,
         READFLASHSTATUS_COMMAND   = 119,
         READSTATUSWRITEFLASH_COMMAND = 120,
         RESTARTMINI_COMMAND       = 121,
         WRITEPRGFLASH_COMMAND     = 122

	  } CommandRequest;


typedef enum { SERIAL_LINK=1, MODEM_LINK, RS485_LINK, ASYNCRON_LINK, IPX_LINK,
			   TCPIP_LINK, NETBIOS_LINK, MOUSE_LINK } Media_type;
#define MOUSE_COMM 4

//typedef enum { SERIAL_LINK, MODEM_LINK, RS485_LINK, NETBIOS } Media_type;
//#define MOUSE_COMM RS485_LINK+1

typedef enum { INSTALLED, NOT_INSTALLED } Port_Status;

typedef enum { LOCAL_POINT, LOCAL_NETWORK_POINT, OTHER_NETWORK_POINT } Where_point;

typedef enum { RECEIVE, SEND, INDIRECT, READ_COMMAND, WRITE_COMMAND, TRANSFER_DATA,
					SEND_COMMAND, SEND_DATA, TRANSFER_COMMAND } Command_type;

typedef enum { A='A', B } Connection_port;

typedef enum { T3000, MINI_T3000, STANDARD, MINI_MAIN, SUB_MINI } PanelType;

typedef enum { POINTNET, POINTT3000, POINTLOCAL } DefinePointType;

typedef enum { NOTHING, IN16, OUT16, IN8_OUT8, DIGOUT } CardsType;

typedef enum { SUBA, SUBB } NetType;

typedef enum { OUTPUT=1, INPUT, VARIABLE, CONTROLLER, WEEKLY_ROUTINE,
	ANNUAL_ROUTINE, PROGRAM, TABLES, DIGITAL_MONITOR, ANALOG_MONITOR,
	CONTROL_GROUP, STATUS, ALARM, PASSWORD, CODE=16, WR_TIMES,
	AR_DATES, CG_DATA, AMON_DATA=22, DMON_DATA, MINI_DESCRIPTORS,
	UNITS=27, MONITOR=40, USERS=46, DESCRIPTORS, ARRAY
						} Point_type_equate;

typedef enum  {
	unused, degC, degF, FPM, Pa, KPa, psi, in_w, Watts, KW, KWH,
	Volts, KV, Amps, ma, CFM, Sec, Minute, Hours, Days, time_unit, ohms,
	procent, RH, ppm, counts,	Open, CFH, GPM, GPH, GAL, CF, BTU, CMH,
	custom1, custom2, custom3, custom4, custom5, custom6, custom7, custom8
	} Analog_units_equate;

typedef enum { UNUSED=100,
	OFF_ON, CLOSED_OPEN, STOP_START, DISABLED_ENABLED,
	NORMAL_ALARM, NORMAL_HIGH, NORMAL_LOW, NO_YES,
	COOL_HEAT, UNOCCUPIED_OCCUPIED, LOW_HIGH,
	ON_OFF , OPEN_CLOSED, START_STOP, ENABLED_DISABLED,
	ALARM_NORMAL, HIGH_NORMAL, LOW_NORMAL, YES_NO,
	HEAT_COOL, OCCUPIED_UNOCCUPIED, HIGH_LOW,
	custom_digital1, custom_digital2, custom_digital3, custom_digital4,
	custom_digital5, custom_digital6, custom_digital7, custom_digital8,
	} Digital_units_equate;

typedef enum { not_used_input, Y3K_40_150DegC, Y3K_40_300DegF, R10K_40_120DegC,
	R10K_40_250DegF, R3K_40_150DegC, R3K_40_300DegF, KM10K_40_120DegC,
	KM10K_40_250DegF, A10K_50_110DegC, A10K_60_200DegF, V0_5, I0_100Amps,
	I0_20ma, I0_20psi, N0_2_32counts, N0_3000FPM_0_5V, P0_100_0_5V,
	P0_100_4_20ma, P0_255p_min, table1, table2, table3, table4,
	table5 } Analog_input_range_equate;

/*
typedef enum {  off_on = 51, closed_open, stop_start, disabled_enabled,
	normal_alarm, normal_high, normal_low, no_yes, custom_digital_1,
	custom_digital_2, custom_digital_3, custom_digital_4, custom_digital_5,
	custom_digital_6, custom_digital_7, custom_digital_8, cool_heat,
	unoccupied_occupied, low_high, on_off, open_closed, start_stop,
	enabled_disabled, alarm_normal, high_normal, low_normal, yes_no,
	heat_cool, occupied_unoccupied, high_low
	} Digital_input_range_equate;
*/
typedef enum { not_used_output, V0_10, P0_100_Open, P0_20psi, P0_100, P0_100_Close, 
					I_0_20ma } 		Analog_output_range_equate;

typedef enum {
	local_modem_main, subA_modem_main, local_subA_main,
	local_modem_subA, subA_subB_main, subA_modem_subB, local_subA_subB
	} Port_configuration_equate;

typedef enum { SUCCESS, ERROR_UPDATE, ERROR_COMM, ERROR_NET } State;

/*===============================< Data types >=========================*/

typedef enum { COM0 = -1, COM1, COM2, COM3, COM4, BUS
						 } RS232PortName;

typedef enum  { RS232_SUCCESS                 = 0,
// Warning errors
							RS232_WARNING                 = -100,
							RS232_FUNCTION_NOT_SUPPORTED,
							RS232_TIMEOUT,
							RS232_ILLEGAL_BAUD_RATE,
							RS232_ILLEGAL_PARITY_SETTING,
							RS232_ILLEGAL_WORD_LENGTH,
							RS232_ILLEGAL_STOP_BITS,
							RS232_ILLEGAL_LINE_NUMBER,
							RS232_NO_MODEM_RESPONSE,
							RS232_NO_TERMINATOR,
							RS232_DTR_NOT_SUPPORTED,
							RS232_RTS_NOT_SUPPORTED,
							RS232_RTS_CTS_NOT_SUPPORTED,
							RS232_DTR_DSR_NOT_SUPPORTED,
							RS232_XON_XOFF_NOT_SUPPORTED,
							RS232_NEXT_FREE_WARNING,
// Fatal Errors
							RS232_ERROR                   = -200,
							RS232_IRQ_IN_USE,
							RS232_PORT_NOT_FOUND,
							RS232_PORT_IN_USE,
							RS232_ILLEGAL_IRQ,
							RS232_MEMORY_ALLOCATION_ERROR,
							RS232_PANEL_OFF,
							RS232_USER_ABORT,
							RS232_REJECT,
							RS232_NEXT_FREE_ERROR
						} RS232Error;

typedef enum { T3000_FAIL = RS232_NEXT_FREE_ERROR, T3000_DESTINATION_ERROR,
			T3000_SOURCE_ERROR, T3000_COMMAND_ERROR, T3000_CHECKSUM_ERROR,
			T3000_SYNC_ERROR, T3000_LENGTH_ERROR, CRC_BAD, COMM_ERROR,
			UNRECOVERABLE_ERROR, USER_ABORT, T3000_NEXT_FREE_ERROR,
			T3000_NEXT_FREE_WARNING = RS232_NEXT_FREE_WARNING }
			T3000_ERROR;

#define LOCAL_VAR 1
#define POINT_VAR 2
#define POINT_REMOTE_VAR 3
#define LABEL_VAR 4

typedef enum { RECEIVE_FRAME_IDLE, RECEIVE_FRAME_HEADER, RECEIVE_FRAME_DATA } ReceiveFrameStatusEnum;

typedef struct {
	unsigned position 	: 8;
	unsigned no_points 	: 7;
	unsigned bank_type	: 1;
	} Bank_Type;

typedef struct {
  DInt  point_type;
  char  *tableName;
} TABLENAME;

#endif

