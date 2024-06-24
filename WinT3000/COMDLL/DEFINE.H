#ifndef _DEFINET_H
#define _DEFINET_H

#define time_type	 unsigned long

#define DSignedChar   signed char
#define DByte         unsigned char
#define DInt          short int
#define DUint         unsigned short
#define DLong         long
#define DUlong        unsigned long


#define WM_PTP_CONNECTION      		 0x0590
#define WM_PTP_RECEPTION       		 0x0590+1
#define WM_PTP_TRANSMISSION    		 0x0590+2
#define WM_PTP_ESTABLISH_CONNECTION  0x0590+3
#define WM_REFRESHGRID               0x0590+4

#define SBProgress 0
#define SBText     1
#define SBCaps     2
#define SBTime     3

#define DIGITAL 0
#define ANALOG 1
#define BLANKCHAR  ""
#define  sizeof_mini_var  6
#define  sizeof_mini_con  18
#define  sizeof_mini_wr  4
#define  sizeof_mini_prg  1000
/* MAXIMUM NUMBER OF ITEMS ************/

#ifdef WORKSTATION

#define MAX_OUTPUT 			0
#define MAX_INPUT 			0
#define MAX_VARIABLE 		0
#define MAX_DMONITOR 		0
#define MAX_AMONITOR 		0
#define MAX_GROUP	 		   0
#define MAX_PROGRAM 		   0
#define MAX_TABLE 			0
#define MAX_CONTROLLER 		0
#define MAX_WEEKLY_ROUTINE 0
#define MAX_ANNUAL_ROUTINE	0
#define MAX_STATUS 			0
#define MAX_PASSWORD 		0
#define MAX_SYSTEM         0
#define MAX_UNIT           0
#define MAX_MONITOR        0

#else

#define MAX_OUTS		 128
#define MAX_INS		 128
#define MAX_VARS		 128
#define MAX_CONS		  48
#define MAX_WR			  16
#define MAX_AR			   8
#define MAX_PRGS		  32
#define MAX_TABS		   5
#define MAX_DIGM		   1
#define MAX_ANALM		  32
#define MAX_GRPS 		  32
#define MAX_ARRAYS	  16
#define MAX_INFO_TYPE  50
#define MAX_STATIONS	  32

#endif

#define MAX_TBL_BANK          15

#define MAX_ELEMENT_GROUP     48
#define MAX_POINTS_IN_MONITOR 14
#define MAX_ICON_NAME_TABLE   25

#define MAX_RANGE             99         /* number of range items */
#define MAX_FILE              43        /* maximum number of PRG files */
#define MAX_BUFFER_SIZE     2000        /* max size for one basic program */

#define SIZE_FULL_LABEL       21
#define SIZE_ABRV              9
#define SIZE_TEXT_RANGE       13         /* 12 characters for range text + null */
													  /* 10 characters for outputs though */
#define SIZE_TEXT_UNITS        6          /* 5 characeters plus a null   */


#define RELEASE_NUMBER		  343
#define MAX_STAND_GROUP_ELEM	64

#define MAX_RETRY		          1
#define YEAR_BYTES		      46

#define MAX_ALARMS            16
#define MAX_ALARMS_SET        16

#define MAX_UNITS              8
#define MAX_PASSW             10
#define MAX_MONSET             1
//#define MAX_USERS
//#define MAX_DST
#define PROGRAM_SIZE        2500
#define MAX_ELEM	            48

#define MAX_RECORD            50

#define MAX_ALPHA          	80
#define MAX_DES	         64000
#define MAX_VIEWS              3

#define T3000_OUT 		 0
#define T3000_IN  		 1
#define VAR 		 2
#define CON 		 3
#define WR 			 4
#define AR 			 5
#define PRG 		 6
#define TBL 	 	 7
#define DMON 		 8
#define AMON	    9
#define GRP 	   10
#define AY        11                  /* table to hold file names */

#define ALARMS    12
#define UNIT      13
#define USER_NAME 14
#define ALARMM    15
#define WR_TIME   16
#define AR_Y      17

#define PASS    USER_NAME

//#define STAT	11

//#define SYS 	13                    /* no table for sys (yet) */
#define MONSETUP    40

#define GRAPHIC_GRP 22
#define STATION     24
#define CSTATION    25
#define AYVALUE     29

#define ARRAY1      30
#define AMON_INPUTS 33

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
#define CNF_COMM			 0x02
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
		 READOUTPUT_T3000 = T3000_OUT+1, READINPUT_T3000 = T3000_IN+1, READVARIABLE_T3000 = VAR+1,
		 READCONTROLLER_T3000 = CON+1, READWEEKLYROUTINE_T3000 = WR+1,
		 READANNUALROUTINE_T3000 = AR+1, READPROGRAM_T3000 = PRG+1, READTABLE_T3000 = TBL+1,
		 READMONITOR_T3000 = AMON+1, READSCREEN_T3000 = GRP+1, READARRAY_T3000 = AY+1,
		 READPROGRAMCODE_T3000 = 16, READGROUPELEMENTS_T3000 = 19,
		 READPOINTINFOTABLE_T3000 = 24, UPDATEMEMMONITOR_T3000 = 23,
		 READMONITORDATA_T3000 = 22, READINDIVIDUALPOINT_T3000 = 20,
		 READPOINTINFO_T3000 = 25, TIME_COMMAND = 21, CLEARPANEL_T3000 = 28,

		 COMMAND_50 = 50, STATION_LIST_COMMAND = 21, DEFAULT_PRG_COMMAND = 32,
     ALARM_NOTIFY_COMMAND = 51,
		 TABLEPOINTS_COMMAND = 75,
		 PANEL_INFO1_COMMAND = 110, PANEL_INFO2_COMMAND = 111, MINICOMMINFO_COMMAND = 112,
		 PANELID_COMMAND = 113, ICON_NAME_TABLE_COMMAND = 114, WRITEDATAMINI_COMMAND = 116,
		 SENDCODEMINI_COMMAND = 117, SENDDATAMINI_COMMAND = 118, READFLASHSTATUS_COMMAND = 119,
     READSTATUSWRITEFLASH_COMMAND = 120, RESTARTMINI_COMMAND = 121, WRITEPRGFLASH_COMMAND = 122

	  } CommandRequest;


typedef enum { SERIAL_LINK=1, MODEM_LINK, RS485_LINK, NETBIOS } Media_type;
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


#define LOCAL_VAR 1
#define POINT_VAR 2
#define POINT_REMOTE_VAR 3
#define LABEL_VAR 4
                        
#endif

