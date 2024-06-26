// available 0x88-0x9B
//

#define NAME 100

#define MAX_OP				10
#define MAX_VAR_LINE    20

#define LOCAL_VARIABLE 0x82
#define FLOAT_TYPE     0x83
#define LONG_TYPE      0x84
#define INTEGER_TYPE   0x85
#define BYTE_TYPE      0x86
#define STRING_TYPE    0x87
#define FLOAT_TYPE_ARRAY     0x88
#define LONG_TYPE_ARRAY      0x89
#define INTEGER_TYPE_ARRAY   0x8A
#define BYTE_TYPE_ARRAY      0x8B
#define STRING_TYPE_ARRAY    0x8C

#define LENGTHSTRING 128 

#define LOCAL_VAR 1
#define POINT_VAR 2
#define POINT_REMOTE_VAR 3
#define LABEL_VAR 4

#define ATTRIB 5
#define MAX_ATTRIB 10
#define MAX_LINE  512

#define MAX_Var 50

#define MAX_ANNUAL_ARRAY 50

#define LOCAL_POINT_PRG  0x09C
#define CONST_VALUE_PRG  0x09D
#define REMOTE_POINT_PRG 0x09E

enum tok_types	{
					DELIMITER , NUMBER, IDENTIFIER , KEYWORD , TEMP, BLOCKX , TIMES, COLON, SEMICOLON,
					GE=0x60, LE=0x61, NL=0x62, MOD=0x66, MUL=0x67,  DIV=0x68,MINUSUNAR=0x69, PLUS=0x6B,
					MINUS=0x6C, LT=0x6D, GT=0x6E,
					EQ=0x71, NE=0x72 ,AND=0x74, OR=0x75, NOT=0x76, EOI=0x77, TTIME=0x78
					};
/* ltg                    

enum tokens {
				  BEEP=0x07, ASSIGNAR=0x08, ASSIGN=0x09, CLEARX=0x0A, FOR=0x0B, NEXT=0x0D, IF=0x0E,

				  ELSE=0x10, IFP=0x11, IFM=0x12, GOTO=0x13, GOSUB=0x14, RETURN=0x15, ENDPRG=0x17, PRINT=0x18,
				  REM=0x1A, PRINT_AT=0x1B, STARTPRG=0x1C, STOP=0x1D, WAIT=0x1E, HANGUP=0x1F,

				  PHONE=0x20, ALARM_AT=0x21, REMOTE_SET=0x22, RUN_MACRO=0x23, REMOTE_GET=0x24,
				  ENABLEX=0x25, DISABLEX=0x26, ON_ERROR=0x27, SET_PRINTER=0x28, ASSIGNARRAY_1=0x29, GOTOIF=0x2A,
				  ON_ALARM=0x2B, ASSIGNARRAY_2=0x2C, OPEN=0x2D, CLOSE=0x2E, CALLB=0x2F,

				  DECLARE=0x30, ASSIGNARRAY=0x31,

				  ON=0x40, Alarm=0x41, DALARM=0x43, USER_A=0x45, USER_B=0x46,

				  DIM=0x8A,

				  DATE=0x0A0, PTIME=0x0A1, SENSOR_ON=0x0A4, SENSOR_OFF, TO, STEP,	THEN,  LET, 	ALARM_AT_ALL,
				  FINISHED, PRINT_ALARM_AT, PRINT_ALARM_AT_ALL, ARG ,  DO , WHILE , SWITCH, EOL , STRING
				};
*/                


/* ltg
enum tok_func	{
					SUN, MON, TUE, WED, THU, FRI, SAT,
					ABS=0x32, AVG=0x33, DOY=0x34, DOW=0x35, INT=0x36, MAX=0x37, MIN=0x38, SQR=0x39, Tbl=0x3A,
					TIME=0x3B, TIME_ON=0x3C, TIME_OFF=0x3D, INTERVAL=0x3E, TIME_FORMAT=0x3F,
					WR_ON=0x40, WR_OFF=0x41, UNACK=0x42, Status=0x47, SCANS=0x4B, POWER_LOSS=0x4C, LN=0x4D, LN_1=0x4E,
					OUTPUTD=0x50, INKEYD=0x51, DOM=0x52, CONPROP=0x56, CONRATE=0x57, CONRESET=0x58, CLEARPORT=0x59

					};
*/                    

enum error_msg { SYNTAX , UNBAL_PARENS , NO_EXP , EQUALS_EXPECTED,
									NOT_VAR , PARAM_ERR , SEMI_EXPECTED , UNBAL_BRACES ,
									FUNC_UNDEF ,TYPE_EXPECTED , NEST_FUNC , RET_NOCALL ,
									PAREN_EXPECTED , WHILE_EXPECTED, QUOTE_EXPECTED ,
									NOT_TEMP , TOO_MANY_LVARS, NOT_LINE, GREATER_THEN_MAX,
									TOOBIG, TOOMANYLINES, TOOMANYVARS, TOOMANYGOTO } ;


struct variable_table {
				char name[12];
				char label[9];
				char type;
				char var_type;
				int  l;
				int  c;
				int  network;
				byte panel;
				byte point_type;
				byte num_point;
};

struct buf_str {
		 int v;
		 char var[12];
		 float fvar;
		 char op[10];
};

