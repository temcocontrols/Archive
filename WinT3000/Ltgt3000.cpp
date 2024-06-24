//---------------------------------------------------------------------------
#define MAIN
#define MOUSE_TEXT

#include <vcl\vcl.h>
#pragma hdrstop
#include <conio.h>
#include <process.h>
#include <dir.h>
#include <dos.h>
#include <sys\stat.h>
#include <math.h>
#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "define.h"
#include "wint3.h"
#include <time.h>
#include "baseclas.h"
#include "LTGLoginInit.h"
#include "LTGUserEnsur.h"
#include "LTGdef.h"
#include "parser.h"
#include "aio.h"
#include "xms.h"
#include "ptpdll.h"
#include <nb30.h>

void setlocalpanel(void);
void RestoreOldPrg(char *);


#define min(a,b)    ( ((a) < (b)) ? (a) : (b))

#define ESC 27

#define M0 0
#define M1 10
#define M2 20
#define M3 30
#define M4 40
#define M5 50
#define M6 60

//ltg  config file format.
#define CNF_VER             0
#define CNF_RESERVED        CNF_VER+2
#define CNF_STATION_NUM     CNF_RESERVED+10
#define CNF_STATION_NAME    CNF_STATION_NUM+2
#define CNF_NETWORK_NUMBER  CNF_STATION_NAME+16
#define CNF_NR_BOARDS       CNF_NETWORK_NUMBER+2+16
#define CNF_SIZE_BOARD_INFO CNF_NR_BOARDS+2
#define CNF_BOARD           CNF_SIZE_BOARD_INFO+2
#define CNF_SIZE_COMM_INFO  CNF_BOARD+MAX_BOARDS*sizeof(Board_info)
#define CNF_COMM_INFO       CNF_SIZE_COMM_INFO+2
#define CNF_MODEM         CNF_COMM_INFO+MAX_COMM_INFO*sizeof(CommINFO)
#define CNF_PROGRAM_PATH  CNF_MODEM+sizeof(ModemCapabilities)
#define CNF_PROGRAM_DES   CNF_PROGRAM_PATH+64
#define CNF_PROGRAM_BASIC CNF_PROGRAM_DES+64
#define CNF_PROGRAM_MONITOR  CNF_PROGRAM_BASIC+64
#define CNF_PROGRAM_GRAPHICS CNF_PROGRAM_MONITOR+64
#define CNF_DEFAULT_PROGRAM  CNF_PROGRAM_GRAPHICS+64
#define CNF_CONSOLE          CNF_DEFAULT_PROGRAM+13
#define CNF_RINGS            CNF_CONSOLE+1
#define CNF_STATION_LIST     CNF_RINGS+1
#define CNF_IND_PASS         CNF_STATION_LIST+MAX_STATIONS*sizeof(Station_point)

#define MAX_INPUT_BUFFER 2000

static FILE *h;

extern void ProceXitClick(void);


extern unsigned long ind_heap_buf;
extern DUint first_free;
extern int nr_boards;
extern int height_char_size(void);
extern int width_char_size(void);
extern int checkport(int com, int t=0);
extern int readdes_serial(void);
extern void msettextstyle(int font, int dir, int chars);
extern void result_ser(RS232Error result, int rs485=0);
extern int netcall(BACnetPDU pdutype, int service, int network, int dest, char *data, int *length , int command, int arg,  int sign=0);
extern struct remote_local_list  remote_local_list[MAXREMOTEPOINTS];
extern int ind_remote_local_list;
extern DUlong timestart;    // sec de la inceputul programului
extern DInt networkaddress;    // sec de la inceputul programului
extern Panel *ptr_panel;
extern DInt local_panel;
extern DInt  station_num;
extern Byte serial_access;
extern long timest;
extern DInt timewait(DWORD timesleep, DUint msg, MSG& myMsg);
extern void align(DInt type, DInt point_type, DByte *buf, DInt start, DInt nr);
extern DByte *Heap_alloc(DInt bytes);
extern DInt Heap_dealloc(DByte **old);
extern DInt Heap_move(DByte *old, DByte *newp, DInt ind);
extern DByte *Heap_grp_alloc(DInt bytes);
extern DInt Heap_grp_dealloc(DByte **old, DInt bytes_old);
extern DInt Heap_grp_resizedown(DByte **old, DInt width, DInt bytes);

struct input_buffer {
		unsigned time;
		union {
			int   i;
			char  key[2];
			} key;
		};

extern struct input_buffer inp_buffer[11];
extern Panel *ptr_panel;

struct input_buffer *ptr_input_buffer;
icon *points_icon;

int pixvar;

int Black=BLACK;                 //0
int Blue=BLUE;                   //1
int Green=LIGHTBLUE;                 //2
int Cyan=CYAN;                   //3
int Red=RED;                     //4
int Green1=WHITE; 						 //5
int Blue1=DARKGRAY;                 //6
int Lightgray=LIGHTGRAY;         //7

int Darkgray=BROWN;           //8
int Lightblue=GREEN;         //9
int Lightgreen=LIGHTGREEN;       //10
int Lightcyan=LIGHTCYAN;         //11
int Lightred=LIGHTRED;           //12
int Lightmagenta=LIGHTMAGENTA;   //13
int Yellow=YELLOW;               //14
int White=MAGENTA;                 //15
int Brown;
int Magenta;

int Overscan=Black;

void changechar(int arg=0);
void restorechar(int arg=0);

extern int i_files;
extern char Icon_name_table[MAX_ICON_NAME_TABLE][14];

#ifdef NETWORK_COMM
extern byte panels_number;
extern NETBIOS_Presence	Netbios_state;
extern USERS_RECORD	user;
extern DIRECT_POINT_RECORD	direct_point;
extern Netbios *pnet;
extern int NET_EXIT;
#endif

extern int time_run_MSTP;
extern int mouse_inst;
extern char display_amons;
extern int setvectint8;
extern int communication;
extern int dontch;
extern int prg_active;
extern char test_tiff;
extern unsigned long  ora_current_sec; // ora curenta exprimata in secunde

extern int ISA_uarts[4];
extern ModemCapabilities ModemDataBase[1];
extern int SERIAL_EXIT;
extern Dial_list_point dial_list_data;
extern  int error_net;
extern byte 	number_of_sessions;
extern char null_buf[10];
extern char *default_string;       // In LTGmodem.cpp
extern int network_select, panel_select, network_sel_tmp, panel_sel_tmp;
extern Station_point *pstation_list_sel;
extern long galarm_read_time;

extern unsigned int MAX_HEAP_BUF, MAX_HEAP_GRP, MAX_HEAP_AMON, MAX_HEAP_ARRAY;
extern unsigned int MAX_HEAP_DIGM;
extern long *heap_array;
extern char *heap_buf;
extern char *heap_grp;
extern char *heap_amon;
extern Heap_dmon *heap_dmon;
extern DUlong ind_heap_grp;

char tempbuf[65];
char control;          //?????

byte ser_sessions[2] = { 0, 0};
char com_buf[10][5] = {"N/A ", "com1", "com2", "com3", "com4",
										 "com5", "com6", "com7", "com8",
										 "bus"};
char connection_type_text[9][14] = {"Not Installed",
													  "SerialRS232  ",
													  "Modem        ",
													  "RS485        ",
													  "Asyncron mode",
													  "Ethernet     ",
													  "Netbios      ",
													  "TCP/IP       ",
													  "Mouse        "};
char tables_text[5][8]={{"Table 1"},{"Table 2"},{"Table 3"},{"Table 4"},{"Table 5"}};
char *autodetect="Auto Detect";
char *ctrlbreak_text="    CTRL/Break - Interrupt";
char *ok_button    =" < OK > ";
char *cancel_button="<Cancel>";
char *exit_button    =" <eXit> ";
char *noexit_button  ="<No exit>";
extern char *version;
extern DInt Version;    //1.3
short int cnf_version;

char *loadpanel_text ="Load panel ";
char *savepanel_text ="Save panel ";
char *renamefile_text ="Rename file ";
char *deletefile_text ="Delete file ";

char *signature = "!@#$";

char Password[9];
char User_name[16];
extern char password[9]; // In Date.cpp.  user password when user login.
char user_name[16];      // user name when user login.
char access_level;
unsigned long rights_access;
char default_group;
char screen_right[2*((MAX_GRPS%8)?MAX_GRPS/8+1:MAX_GRPS/8)];
char program_right[2*((MAX_PRGS%8)?MAX_PRGS/8+1:MAX_PRGS/8)];
char right_access;
char remote_path[65];
char array_sel_path[][11]={{"Common    "},{"Separate  "}};

byte modem_present = 0;
byte modem_active = 0;
byte console_mode;
byte number_rings=1;
char keyboard_data;

extern DInt ind_station;                      // In date.cpp
extern DByte station_name[NAME_SIZE];         // In Date.cpp

extern DUlong Active_Panels;
extern char Panel_Type;          // In Date.cpp  = T3000;

int simulator;
int start_apl=0;
int nnr_boards;
int grid_record_number;
int entry_selected;
char main_screen;

int extended_memory;
int maxx;
int maxy;

Str_array_point arights[14] =
					 {{"Outs",R_READ_ONLY},{"Ins",R_READ_ONLY},{"Vars",R_READ_ONLY},{"Cons",R_READ_ONLY},
						{"Wrs",R_READ_ONLY},{"Ars",R_READ_ONLY},{"Prgs",R_READ_ONLY},{"Tables",R_READ_ONLY},
						{"Dmons",R_READ_ONLY},{"Amons",R_READ_ONLY},{"Screens",R_READ_ONLY},{"Arrays",R_READ_ONLY},
						{"Config",R_READ_ONLY},{"File op",R_READ_ONLY}};
char arights_grid;

char GAlarmold;
int alarm_time;
int alarm_col;
char alarm_beep;
int exit_apl = 0;
int logout;
int qtt;
char supervisor[]="TRAVEL";

int ex_apl=1;
char LOADPRG;

int ind_nested_grp;
int current_grp;

extern Board_info	boards[MAX_BOARDS]; // In Date.cpp
             // ={256,1,7372,200, 250, 0,0,0,0,0,0,0};

union seg_sel {
 unsigned long l;
 struct {
	unsigned sel;
	unsigned seg;
 } seg_sel;
} seg_sel;
unsigned ega_selector;

char EGA_data_block[30];
char *SAVE_PTR;
char text_auxiliary[13];

char characters_font[] =
			{
			 0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,     // ³ left
			 0x80,0x80,0x80,0x80,0x80,0x80,

			 0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,     // ³ left-1
			 0x60,0x60,0x60,0x60,0x60,0x60,

			 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,    // ³ right
			 0x01,0x01,0x01,0x01,0x01,0x01,

			 0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,    // ³ right-1
			 0x02,0x02,0x02,0x02,0x02,0x02,

			 0x0ff,0,0,0,0,0,0,0,0,0,                        // Ä  up
			 0,0,0,0,0,0,

			 0,0x0ff,0,0,0,0,0,0,0,0,                        // Ä  up-1
			 0,0,0,0,0,0,

			 0,0,0,0,0,0,0,0,0,0,                        // Ä dn
			 0,0,0,0,0,0x0ff,


			 0,0,0,0,0,0,0,0,0,0,                        // Ä dn-1
			 0,0,0,0,0x0ff,0,

			 0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,       //  laterale
			 0x81,0x81,0x81,0x81,0x81,0x81,

			 0x0,0,0xff,0,0,0,0,0,0,0,                                 // sus-jos
			 0,0,0,0,0,0xff,

			 0xff,0x80,0x80,0x80,0x80,0x80,0x9f,0x9f,0x9f,0x80,       //  exit
			 0x80,0x80,0x80,0x80,0x80,0xff,

			 0xff,0x01,0x01,0x01,0x01,0x01,0xf9,0xf9,0xf9,0x01,       //  exit
			 0x01,0x01,0x01,0x01,0x01,0xff,


			0,0,0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfa,0x98,
			0x18,0x0c,0x0c,0x0,0x0,0,

			 0xff,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,     // ³ left top
			 0x80,0x80,0x80,0x80,0x80,0x80,

			 0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,     // ³ left bottom
			 0x80,0x80,0x80,0x80,0x80,0xff,

			 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,    // ³ right bottom
			 0x01,0x01,0x01,0x01,0x01,0xff,

			 0xff,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,    // ³ right bottom
			 0x01,0x01,0x01,0x01,0x01,0x01,

			 0x18,0x18,0x18,0x18,0x18,0x18,0x18,0xf8,0x0,0x0,      // Ù;
			 0x0,0x0,0x0,0x0,0x0,0x0,

			 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1f,0x18,0x18,0x18,      // Ú
			 0x18,0x18,0x18,0x18,0x18,

			 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,     //Û
			 0xff,0xff,0xff,0xff,0xff,0xff,

			 0x0,0x7f,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,     // ³ left-1 top
			 0x60,0x60,0x60,0x60,0x60,0x60,

			 0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,     // ³ left-1 bottom
			 0x60,0x60,0x60,0x60,0x7f,0x0,

			 0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,    // ³ right-1 bottom
			 0x06,0x06,0x06,0x06,0xfe,0x0,

			 0x0,0xfe,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,    // ³ right-1 top
			 0x06,0x06,0x06,0x06,0x06,0x06,

			 0xff,0x81,0x81,0x81,0x99,0xbd,0x7e,0xff,0x99,0x99,            // arrow up
			 0x99,0x99,0x81,0x81,0x81,0xff,

			 0xff,0x81,0x81,0x81,0x99,0x99,0x99,0x99,0xff,0xbe,            // arrow down
			 0x9d,0x89,0x081,0x81,0x81,0xff,

			 0x0,0x0,0xff,0x80,0x82,0x86,0x8e,0x9e,0xbf,0xbf,0x9e,            // arrow left1
			 0x8e,0x86,0x82,0x80,0xff,

			 0x0,0x0,0xff,0x01,0x01,0x01,0x01,0x01,0xf1,0xf1,0x01,            // arrow left2
			 0x01,0x01,0x01,0x01,0xff,

			 0x0,0x0,0xff,0x80,0x80,0x80,0x80,0x80,0x8f,0x8f,0x80,            // arrow left2
			 0x80,0x80,0x80,0x80,0xff,

			 0x0,0x0,0xff,0x01,0x41,0x61,0x71,0x79,0xfd,0xfd,0x79,            // arrow down2
			 0x71,0x61,0x41,0x01,0xff,

			 80,0,0,0,0,0,0,0,0,0,                        // . left-top
			 0,0,0,0,0,0,

			 0,0,0,0,0,0,0,0,0,0,                        // . left-bottom
			 0,0,0,0,0,80,

			 0,0,0,0,0,0,0,0,0,0,                        // . right-bottom
			 0,0,0,0,0,1,

			 1,0,0,0,0,0,0,0,0,0,                        // . right-top
			 0,0,0,0,0,0,

			};


#define MODE_TEXT 1
#define MODE_GRAPHIC 0
#define TEXT_FOND_COLOR White

char *lin_read ="±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±";
char *lin_text  ="                                                                                 ";
char *prg_text  ="*.PRG";

extern signed char point_des[AY+1];

extern char *blank;
extern char *na;
extern char *on;
extern char *off;

extern int maxx;
extern int maxy;
extern char text;
extern int mode_graph;
int fond_color;

extern int font_type,charsize;
extern char blocked1;

union key {
		int i;
		char ch[2];
		} ;

extern char *estab_text;

extern int nsample_mem_dig;

//ltg extern char just_load;
//ltg extern char onemin;

extern char *lin_menu;
extern char *lin_hor;
extern char *lin_ver;

extern int dxscreen, dyscreen;

extern char point_cod[AY+1][5];

extern Str_tbl_point custom_tab[MAX_TABS];

extern dig_range_form dig_range_array[];

extern struct {
	char name[5];
	int cod_point;
	int point_type;
	int size;
	} points_cod[12];


byte cod_command[AY+1]={23,22,24,25,26,27,21,0,0,0,20,28};

char interface_mode;
char oldpath[65];
int oldrive;
int refresh_time_init=150;
int refresh;
long t;
int refresh_time = 150;
unsigned long grp_time;
unsigned long display_grp_time;
char present_analog_monitor, save_prg_flag;

extern DUlong timesec1970;  // sec la inceputul programului
                            // In Date.cpp.

int programs_size;

int exec_program(char *prog);
void About(void);

extern void mini_operations(void);
extern unsigned int countlength(int command, int arg);
extern void infoscreen(void);
extern void resetop(void);
extern void init_boards_struct( void );
void  getkey(void);
extern int	repeat_grp(void);
extern void adjust_readtime( Time_block *time_buf );
extern void adjust_writetime(Time_block *time_buf, Time_block_mini *timemini);
extern void memsize(void);
extern char *lalign(char *buf, int n);
extern void ftoa(float f, int length, int ndec, char *buf);
//ltg extern void initanalogmon(void);
extern int print_alarm_word(void);
extern void Border(int ltopx, int ltopy, int rbottomx, int rbottomy, int colour,int bcolour);

char *rtrim(char *text);              // LTGGgrid.cpp
void DisplayError(int lx, int ly, int rx, int ry, char *message,int bkgnd, int frgnd, int tbkgnd, int fcolor=Black);
int ReadYesNoButton(int x1, int y1, int x2, int y2, int bkgnd, int tcolor, int frgnd,
							int bkgndrreadcolor, int readcolor, char *text, char *yes, char *no,
							char okchar='O', char cancelchar='C', char xok=3, char xcancel=1);
int	saveloadpoint(int type, int point_type, int n_points, int size, int panel,FILE *h, char *buf);
void pfarfree(HANDLE handle);
void clear_color(int lx, int length, int ly, int ry, char *lin_text, int bkgnd, int frgnd);
void terminal_mode(void);

//ltg 
int check_access(int ptype, int num=-1, int mes=R_NA);
void save_config(int what);

extern void pdate(GWDialog *D);
extern void ptime(GWDialog *D);

void select_panel(int type, int net_no, int where=0);
void filetransfer_routine(void);
void wait(long sec);
int get_access(int ex=0);
void disconnect_serial(int d=0);

void PutCheckBox(int lx, int ly, char *string, int fondcolor, int bkgnd, int frgnd);
void CheckBox(int lx, int ly, int bkgnd, int frgnd);
void UnCheckBox(int lx, int ly, int bkgnd, int frgnd);
extern void miniconfig(void);
void mouseport(GWDialog *D);
void mport(GWDialog *D);
void sport(GWDialog *D);
void mouseirq(GWDialog *D);
void mirq(GWDialog *D);
void sirq(GWDialog *D);
void mouserate(GWDialog *D);
void mrate(GWDialog *D);
void srate(GWDialog *D);
void adetect(GWDialog *D);
void portconfig(void);
void pmodemconfigdefault(GWDialog *D);

void setup(void);
void hardware(void);

GWDialog *pm;

			 char array_access_level[] = {
									0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
									0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
									0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
									0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
									0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
									0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,
									0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F
									};

			 int  listonscreen[10] = {0x10,0x01,0,0,0x05};
			 char *graphic_grp_message[]={{"~Group Elements"},{"~Refresh time"},{"e~Xit"},0};
			 char *grp_message[]={{"~Load"},{"~Send"},{"~Edit"},{"e~Xit"},0};
			 char *panel_message[]={{"~Load"},{"~Save"},{"~Rename"},{"~Erase"},{"E~xit"},0};
			 char *message[]={{"  ~Miscellaneous   "},{"  ~Panel  "},{"  ~Control  "},{"  ~Data   "},{"  Con~Figuration  "},0};
			 char *list1[]={
								{"~Connect"},
								{"~Disconnect"},
								{"~About      "},
								{"~Info screen "},
								{"~File Transfer"},
								{"l~Oad descriptors"},
								{"~Update mini"},
								{"e~Xit.......Alt-X"},
								{"~Logout.....Alt-L"},0};
			 char *list2[]={
								{"~File"},
								{"~Select Panel"},
								{"~Print Panel"},
								{"~Clear Panel"},0};

			 char *list3[]={
								{"~Screens..........Alt-S"},
								{"~Programs.........Alt-P"},
								{"~Inputs...........Alt-I"},
								{"~Outputs..........Alt-O"},
								{"~Variables........Alt-V"},
								{"~Controllers......Alt-C"},
								{"~Weekly routines..Alt-W"},
								{"~Annual routines..Alt-A"},
								{"Arra~Ys...........Alt-Y"},0};
			char *list4[]={
								{"~Monitors.......Alt-M"},
								{"~Tables.........Alt-T"},
								{"Custom ~units        "},
								{"~Alarm Log           "},0};
			 char *list6[]={
								{"Set~Up........Alt-U"},
								{"U~ser List         "},
								{"~Hardware          "},0};
			 char *list7[]={
								{"~Default program"},
								{"File ~Paths"},
								{"~Time and Date"},
								{"~Operation mode"},
								{"~ID panel"},0};

			 char *list8[]={
								{"~COM ports          "},
								{"~Mini configuration"},
								{"~T3000 configuration"},0};

			 char *list9[]={
								{"  ~Load    "},
								{"  ~Save    "},
								{"  ~Prg->Flash"},0};
			 char *list10[]={
								{"~Read File "},
								{"~Send File "},
								{"~Delete File"},
								{"~Rename File"},
								{"~Update network"},0};
			 char *list11[]={
								{"Send ~Data "},
								{"Send ~Code"},
								{"~Read flash"},
								{"~Erase sector"},0};

	char **message_list[7]={list1,list2,list3,list4,list6};

unsigned high_mem_block;

#define TOPPANELX 10
#define TOPPANELY 3
#define BOTPANELX 65
#define BOTPANELY 10


extern void alignPanelInfo(int t, DByte *p_info1, Panel_info1 *panel_info1);
extern void upcustomunit(Units_element	*punits);
extern void StatusBarMessage(int i, AnsiString text);
extern void runImage(void);

extern dig_range_form dig_range_array[];

extern DUlong   ora_current_sec; // ora curenta exprimata in secunde
extern XmsDes *Des;
extern DSignedChar point_des[AY+1];
extern AnsiString readingdestext;
extern DByte *points_text[];

extern DInt  station_num;
extern DByte tbl_bank[MAX_TBL_BANK];
extern DByte default_prg[13];
extern Panel_info1 Panel_Info1, panel_info1;
extern DByte GAlarm, gAlarm;
extern long galarm_read_time;
extern Password_struct passwords;
extern Station_point station_list[MAX_STATIONS];
extern DInt comPort;

void adjust_readtime( Time_block *time_buf );  // mini->T3000
void adjust_writetime(Time_block *time_buf, Time_block_mini *timemini);

//#include <LTGT3000APP.cpp>
/**********************************************************************/
/* LTG:  July 23, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Initialization of system.                                        */
/*   1. read config;                                                  */
/*   2. login;                                                        */
/*--------------------------------------------------------------------*/
/*   Calling:  WinMain() In WinT3000.cpp                              */
/*--------------------------------------------------------------------*/
/* Updating: Aug. 12, 1997                                            */
/**********************************************************************/
int InitializeSystem()
{
//  char *ontext;
  DInt j,n;
  DUint i;

  Station_NUM = 1;

//B******************    Adi    ******************************************
 heap_buf = new char [(long)MAX_HEAP_BUF+(long)MAX_HEAP_GRP];
 heap_grp = ((char *)heap_buf)+MAX_HEAP_BUF;

 heap_amon = new char [(long)MAX_HEAP_AMON+(long)MAX_HEAP_DIGM*sizeof(Heap_dmon)+(long)MAX_HEAP_ARRAY*4L];
 heap_dmon = (Heap_dmon *)(((char *)heap_amon)+(long)MAX_HEAP_AMON);
 heap_array = (long *)(((char *)heap_dmon)+(long)MAX_HEAP_DIGM*sizeof(Heap_dmon));

 points_icon = new icon[AY+1];
 strcpy(points_icon[AMON].name,"monitor.bmp");
 strcpy(points_icon[PRG].name,"program.bmp");
 strcpy(points_icon[WR].name,"wr.bmp");

//E******************    Adi    ******************************************


/**********************************************************************/
/* LTG:  July 23, 1997.                                               */
/*       Read config file.                                            */
/**********************************************************************/
#ifndef WORKSTATION
  FILE *fhandle;
  
  if( (fhandle=fopen("config.cnf","rb+"))!=NULL)
  {
	fseek(fhandle,CNF_VER,SEEK_SET);       // from 0 (CNF_VER).
	fread(&cnf_version, 2, 1, fhandle);    //config.cnf version

	if(cnf_version >= 212)
	{
	  fseek(fhandle,CNF_STATION_NUM,SEEK_SET);
	  fread(&Station_NUM, 1, 2, fhandle);
 	  fread(Station_NAME, 16, 1, fhandle);

	  fread(&NetworkAddress, 1, 2, fhandle);
//ltg** next statement only for dddeleting previous
//ltg** statement:   fread(&NetworkAddress, 1, sizeof(DInt), fhandle);
//      fread(NetworkName, 1, 2, fhandle);

//ltg 	  fread(&NetworkName, 16, 1, fhandle);
      fread(NetworkName, 16, 1, fhandle);

	  fread(&nr_boards, 2, 1, fhandle);
	  fread(&n, 2, 1, fhandle);    //size
	  for(i=0;i<MAX_BOARDS;i++)
	  {
	    if(sizeof(Board_info)>=n)
		  fread(&boards[i], n, 1, fhandle);
		else
		{
		  fread(&boards[i], sizeof(Board_info), 1, fhandle);
		  fseek(fhandle,n-sizeof(Board_info),SEEK_CUR);
		}
	  }

	  fread(&n, 2, 1, fhandle);   //size CommInfo
	  j=MAX_COMM_INFO;
	  for(i=0;i<j;i++)
	  {
// ltg 			if(sizeof(CommINFO)>=n)
// ltg 			 fread(&CommInfo[i], n, 1, fhandle);
// ltg 			else
			{
// ltg 			 fread(&CommInfo[i], sizeof(CommINFO), 1, fhandle);
// ltg 			 fseek(fhandle,n-sizeof(CommINFO),SEEK_CUR);
			}
	  }
	  fclose(fhandle);
	}  // end of " if(cnf_version >= 212) "
	else
	{
	  fclose(fhandle);
      MessageBox(GetFocus(),
        " Old configuration file! Created a new one.", "", MB_OK );

	  init_boards_struct();
	}
  }   // end of open file successfully.
  else
  {
	//  fail to open config file.
    init_boards_struct();
  }

  Station_NUM = 1;

  strcpy((char *)station_name, (char *)Station_NAME);
  station_num=Station_NUM;
  local_panel=1;
  networkaddress = NetworkAddress;
  Panel_Info1.panel_type   = Panel_Type;
  Panel_Info1.panel_number = Station_NUM;
  Panel_Info1.network      = NetworkAddress;
  Panel_Info1.version      = Version;
  memcpy(Panel_Info1.network_name, NetworkName, sizeof(Panel_Info1.network_name));
  memcpy(Panel_Info1.panel_name, Station_NAME, sizeof(Panel_Info1.panel_name));
  Panel_Info1.active_panels = (1L<<(Station_NUM-1));
  panel_info1 = Panel_Info1;

#endif

#ifdef WORKSTATION
  strcpy(station_name, "Station 1       ");
  station_num=1;
  local_panel=0;
#endif

  memset(&passwords,'\0',sizeof(passwords));
  access_level=0;

  if( (fhandle=fopen("config.cnf","rb+"))!=NULL)
  {
    long temp;

    temp = CNF_BOARD;
    temp = CNF_SIZE_COMM_INFO;
    temp = CNF_COMM_INFO;
    temp = CNF_MODEM;

 	fseek(fhandle,temp,SEEK_SET);
    fseek(fhandle,CNF_STATION_LIST,SEEK_SET);
    fread(station_list,sizeof(Station_point),MAX_STATIONS,fhandle);

    fseek(fhandle,CNF_STATION_LIST+MAX_STATIONS*sizeof(Station_point),
      			  SEEK_SET);
	fread(&passwords.ind_passwords,2,1,fhandle);
	fread(passwords.passwords,sizeof(Password_point),
    						  passwords.ind_passwords,fhandle);

	fclose(fhandle);
  }

strcpy((char *)station_list[Station_NUM-1].name,(char *)Station_NAME);

//ltg 	hide_pointer();

	for(i=0; i<4; i++)
	{
// ltg 	 if( CommInfo[i].connection == MOUSE_LINK )
	 {
// ltg 		 CommInfo[i].connection = 0;
// ltg 		 CommInfo[i].baudrate = 38400L;
	 }
	}
//ltg 	mouseint();

/**********************************************************************/
/* LTG:  July 25, 1997.                                               */
/*       Read ????? file.                                            */
/**********************************************************************/
  {

	oldrive=getdisk();
	getcwd(oldpath,60);
	if(oldpath[strlen(oldpath)-1]=='\\') oldpath[strlen(oldpath)-1]=0;
	if(!ptr_panel)
	{
      ptr_panel = new Panel();
	  if(!ptr_panel)
	  {
        MessageBox(GetFocus(),"Not enough memory!", "", MB_OK );
		exit(0);
	  }

	  if(cnf_version < 140)
	  {
	    save_config(CNF_ALL);
		cnf_version = Version;
	  }

#ifndef WORKSTATION
	  if(!Station_NUM)
	  {
 	    strcpy((char *)Station_NAME, (char *)"PANEL1");
	    Station_NUM=1;
	    if( control )
        {
// ltg 		boardconfig();
	    }
	    logout=1;
 	    strcpy((char *)station_list[Station_NUM-1].name,(char *)Station_NAME);
 	    strcpy((char *)station_name, (char *)Station_NAME);
	    station_num=Station_NUM;
	    local_panel=1;
	  }
#endif

	  ptr_panel->GlPanel=station_num;
//ltg 	ptr_panel->GlNetwork = networkaddress;

#ifdef NETWORK_COMM
//ltg 	  operator_list.state = IDLE;
	  direct_point.state = DIRECT_IDLE;
#endif

	  Station_point *stp;

	  for(i=0,stp=station_list; i < MAX_STATIONS; stp++,i++)
	  {
		 if(i+1!=Station_NUM)
			 stp->state = 0;
		 else
		  {
			 stp->state = 1;
 			 strcpy( (char *)stp->name, (char *)Station_NAME );
		  }
		 stp->des_length = 0;
	  }


/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   ??? adr of  TASK InitializeSystem.                               */
/**********************************************************************/

	  if(logout)
	  {
		 save_config(CNF_ALL);
		 logout = 0;
	  }

#ifdef NETWORK_COMM
	  user.state = REQUEST_HOSTS_NAMES;
	  if(tasks[NETWORK].status!=DEAD)
	  {
        MessageBox(GetFocus()," Connecting to network... ", "", MB_OK );
//ltg 			resume(NETWORK);
//ltg 			suspend(PROJ);
	  }

#endif
	}  // end of "if(!ptr_panel)".

#ifdef NETWORK_COMM
	if( Netbios_state == NB_INSTALLED && panels_number )
	{
	 ind_station = (int)panels_number;
//	 if(extended_memory)
	 {
//		DisplayMessage(20, 7, 60, 11, " Reading descriptors... ",&D);
		DisplayMessageT(20, 7, 60, 11, ctrlbreak_text, " Reading descriptors... ", NULL, Black, &D);
		int net_length=0;
		for(i=0,Station_point *p=station_list; p < &station_list[MAX_STATIONS]; p++,i++)
//		for(i=0;i<32;i++)
		 if(p.state && Station_NUM!=i+1)
			 net_length += p.des_length;
		Des = new XmsDes(net_length);
		for(i=0;i<MAX_STATIONS;i++)
		if(Station_NUM!=i+1)
		 if(station_list[i].state)
			{
				for(int j=0;j<=AY;j++)
				 if(point_des[j]>=0)
					{
 					 t3000Request(47, j+1, Des->buffer, 0, i+1);
					 int l;

//ltg 					 Des->put(i+1,j,operator_list.length);
		}

	  }
    }
#endif
  }

/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   If load default prg ?                                            */
/**********************************************************************/
/*
     NCB ncb;
     DByte ncb_lana_num;
     LANA_ENUM le,*ple;
     memset(&le,0,sizeof(LANA_ENUM));
     memset(&ncb,0,sizeof(NCB));
     ncb.ncb_command=NCBENUM	;           //0x7F;
     ncb.ncb_buffer=(unsigned char *)&le;
     ncb.ncb_length=2000;
     Netbios(&ncb);
     ncb_lana_num=le.lana[0];

     memset(&ncb,0,sizeof(NCB));
     ncb.ncb_command=NCBRESET;
     ncb.ncb_lana_num=ncb_lana_num;
     ncb.ncb_lsn=3;
     ncb.ncb_num=3;
     Netbios(&ncb);

     char buf[sizeof(ADAPTER_STATUS)+sizeof(NAME_BUFFER)+100];
     ADAPTER_STATUS *pas;
     NAME_BUFFER *pnb;
     memset(&ncb,0,sizeof(NCB));
     memset(buf,0,sizeof(ADAPTER_STATUS)+sizeof(NAME_BUFFER));
     ncb.ncb_command=NCBASTAT	;           //0x7F;
     ncb.ncb_lana_num=ncb_lana_num;           //0x7F;
     ncb.ncb_buffer=(unsigned char *)buf;
     ncb.ncb_length=sizeof(ADAPTER_STATUS)+sizeof(NAME_BUFFER)+100;
     Netbios(&ncb);
     pas = (ADAPTER_STATUS *)ncb.ncb_buffer;
     pnb = (NAME_BUFFER *)(pas+1);

     memset(&ncb,0,sizeof(NCB));
     ncb.ncb_command=0x7F;
     ncb.ncb_lana_num=ncb_lana_num;           //0x7F;
     Netbios(&ncb);
*/
	 if(!console_mode)
	 if(!LOADPRG)
	 {
		tempbuf[0] = 0;
		strcpy(tempbuf, ptr_panel->Program_Path);
		if(strlen(tempbuf)) strcat(tempbuf,"\\");
		strcat(tempbuf, ptr_panel->Default_Program);
//        if(strlen(tempbuf))
//			ptr_panel->loadprg(tempbuf);
			ptr_panel->loadprg("demo.prg");
		LOADPRG=1;
	 }


//ltg 	G.GShowMenuBar(message,listonscreen);
//ltg 	G.ShowunderMBar();
//ltg 	install_port();

	main_screen = 0x02;

/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   Main LOOP of T3000.                                              */
/*   1. Start T3000.                                                  */
/*   2. Get password.                                                 */
/*   3. Start default panel (PRG).                                    */
/*   4. Display main menu.                                            */
/*   5. get user access.                                              */
/**********************************************************************/
//  int x;

/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   Display first graph of T3000.                                    */
/*   TITLE: THIS IS OUR T3000.                                        */
/**********************************************************************/
  if( (h=fopen("T3000.TIF","r")) != NULL )
  {
    fclose(h);

//ltg 		  hide_pointer();

// ltg 	rgb pal1[16];
    
//ltg 		  grp_current_pal=(rgb *)pal1;
//ltg 		  tif("T3000.TIF",0,0,maxx,maxy);
//ltg 		  Darkgray=closest_rgb( init_pal[Darkgray], grp_current_pal, 16);
//		set16( init_pal, 0 );
//ltg 		  display_pointer();
  }
  else              // open t3000.tif successfullly.
  {
//ltg 		  Bordergr(1, 1, 80, 25, Blue1, White, Black, Blue1, 3, 0);
  }

  ex_apl = 0;       // not exit.

/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   Check user name and password.                                    */
/**********************************************************************/

  MSG myMsg;
  if( time(NULL) < (DLong)(timesec1970+2) )
  {
    timewait( (timesec1970+2-time(NULL))*1000L+500, 0, myMsg);
  }
  MainScreen->LogoPanel->Visible = false;
  if( get_access() )
  {
   memcpy(Password, password, sizeof(Password));
   memcpy(User_name, user_name, sizeof(User_name));

   if ( default_group )
   {
		  itoa(default_group, tempbuf, 10);
		  for(i=0;i<strlen(tempbuf); i++)
			{
//ltg 			  input_buffer[i+4].key.i=tempbuf[i];
			}
//ltg 		  input_buffer[i+4].key.i=13;
//ltg 		  simulate=1;
//ltg 		  ind_input_buffer=0;
   }
   return 1;
  }
  else
  {
    ProceXitClick();
    return 0;
  }
}
//---------------------------------------------------------------------------

/**********************************************************************/
/* LTG:  JuLY 28, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Saving config file.                                              */
/*--------------------------------------------------------------------*/
/*	2 bytes  : version                                                */
/*	10 bytes : reserved                                               */
/*	2 bytes  : Station_NUM                                            */
/*	16 bytes : Station_NAME                                           */
/*	2 bytes  : nr_boards                                              */
/*	2 bytes  : sizeof(Board_info);                                    */
/*	MAX_BOARDS*sizeof(Board_info) bytes  : boards                     */
/*	2 bytes  : sizeof(CommINFO)                                      */
/*	MAX_COMM_INFO * sizeof(CommINFO) bytes : CommInfo               */
/*--------------------------------------------------------------------*/
/* Updating:  July 28, 1997                                           */
/**********************************************************************/
void save_config(int what)
{
  char buf[13]={0,0,0,0,0};
  char fname[65];
  int n;
  FILE *h;

  strcpy(fname, oldpath);
  strcat(fname,"\\config.cnf");

  if( (h=fopen(fname,"rb+"))==NULL)
    if( (h=fopen(fname,"wb+"))==NULL)
	{
	  return;
	}

  fseek(h,CNF_VER ,SEEK_SET);
  fwrite(&Version, 2, 1, h);
  fwrite(buf, 10, 1, h);
  if( what & CNF_BOARD_INFO )
  {
    fseek(h,CNF_STATION_NUM,SEEK_SET);
	fwrite(&Station_NUM, 2, 1, h);
	fwrite(Station_NAME, 16, 1, h);
	fwrite(&NetworkAddress, 1, 2, h);
	fwrite(NetworkName, 16, 1, h);

	fwrite(&nr_boards, 2, 1, h);
	n=sizeof(Board_info);
	fwrite(&n, 2, 1, h);
	fwrite(boards, MAX_BOARDS*sizeof(Board_info), 1, h);
  }

  if( what & CNF_COMM )
  {
    fseek(h,CNF_SIZE_COMM_INFO, SEEK_SET);
 	n=sizeof(CommINFO);
	fwrite(&n, 2, 1, h);
 	fwrite(CommInfo, MAX_COMM_INFO * sizeof(CommINFO), 1, h);
  }

  if( what & CNF_PATH_INFO )
  {
 	fseek(h,CNF_PROGRAM_PATH,SEEK_SET);
	fwrite(ptr_panel->Program_Path, 64, 1, h);

	fwrite(ptr_panel->Descriptor_Path, 64, 1, h);
	fwrite(ptr_panel->Basic_Path, 64, 1, h);
	fwrite(ptr_panel->Monitor_Path, 64, 1, h);
	fwrite(ptr_panel->Graphics_Path, 64, 1, h);
	fwrite(ptr_panel->Default_Program, 13, 1, h);
  }

  if( what & CNF_CONSOLE_INFO )
  {
    fseek(h,CNF_CONSOLE, SEEK_SET);
    fwrite(&console_mode, 1, 1, h);
  }

  if( what & CNF_STATION_INFO )
  {
    fseek(h,CNF_STATION_LIST,SEEK_SET);
	fwrite(station_list,sizeof(Station_point),MAX_STATIONS,h);
  }

  if( what & CNF_PASS_INFO )
  {
    fseek(h,CNF_IND_PASS,SEEK_SET);
	fwrite(&passwords.ind_passwords,2,1,h);
	fwrite(passwords.passwords,sizeof(Password_point),passwords.ind_passwords,h);
  }

  fclose(h);
}
//----------------------------------------------------------------------------
//  ***********************************************************
//  Mao Hui Aug 14,1997
//  Panel::Panel(void) : Aio(nr_boards)
//	begin
//  ***********************************************************
Panel::Panel(void) : Aio(nr_boards)
{
//	start_apl=1;
//	ptr_panel = this;
//	strcpy(Program_Path, oldpath);
//	strcpy(Descriptor_Path, oldpath);
//	strcpy(Basic_Path, oldpath);
//	strcpy(Monitor_Path, oldpath);
//	strcpy(Graphics_Path, oldpath);
//	strcpy(DataBase_Path, oldpath);
    Default_Program[0] = 0;
	if( (h=fopen("config.cnf","rb+"))!=NULL)
	{
		fseek(h,20+MAX_BOARDS*sizeof(Board_info)+3*sizeof(CommInfo)+sizeof(ModemCapabilities),SEEK_SET);
        fseek(h,CNF_PROGRAM_PATH,SEEK_SET);
        fread(Program_Path, 64, 1, h);
        fread(Descriptor_Path, 64, 1, h);
        fread(Basic_Path, 64, 1, h);
        fread(Monitor_Path, 64, 1, h);
        fread(Graphics_Path, 64, 1, h);
        fread(Default_Program, 13, 1, h);
        fread(&console_mode, 1, 1, h);
        fread(&number_rings, 1, 1, h);
        memcpy(default_prg, Default_Program, 13);
        fclose(h);
	}
	GlPanel=station_num;
	GlNetwork=networkaddress;
	GlPanel_type = T3000;
}
//  ***********************************************************
//  Mao Hui Aug 14,1997
//  Panel::Panel(void) : Aio(nr_boards)
//	end
//  ***********************************************************

/* ltg
void Panel::operator=(const Panel &copy)
{
	if(this == &copy) return;
	Aio( copy.number_of_boards);
	strcpy(Program_Path, copy.Program_Path);
	strcpy(Descriptor_Path, copy.Descriptor_Path);
	strcpy(Basic_Path, copy.Basic_Path);
	strcpy(Monitor_Path, copy.Monitor_Path);
	strcpy(Graphics_Path, copy.Graphics_Path);
	strcpy(DataBase_Path, copy.DataBase_Path);
	strcpy(Default_Program, copy.Default_Program);

	GlPanel=station_num;
	GlPanel_type = T3000;
//ltg    GlNetwork = networkaddress;
}
*/

/* ltg
Panel::Panel(void) : Aio(nr_boards)
{
    Default_Program[0] = 0;

    	if( (h=fopen("config.cnf","rb+"))!=NULL)
	 {

		 fseek(h,CNF_PROGRAM_PATH,SEEK_SET);
		 fread(Program_Path, 64, 1, h);
		 fread(Descriptor_Path, 64, 1, h);
		 fread(Basic_Path, 64, 1, h);
		 fread(Monitor_Path, 64, 1, h);
		 fread(Graphics_Path, 64, 1, h);
		 fread(Default_Program, 13, 1, h);
//ltg 		 fread(&console_mode, 1, 1, h);
//ltg 		 fread(&number_rings, 1, 1, h);


		 fclose(h);
	 }

	GlPanel=station_num;
//ltg 	GlNetwork=networkaddress;
	GlPanel_type = T3000;
}
*/

/* ltg
DInt  Panel::connect_panel_proc(DInt t)
{
		 DByte tbl_bank1[MAX_TBL_BANK];
		 Panel_info1 panel_info,p_info1;
		 DByte default_prg1[13];
		 RS232Error result;
		 Time_block time_buf;
		 unsigned int l1;

		 memcpy( tbl_bank1, tbl_bank, sizeof(tbl_bank1));
		 memcpy( default_prg1, default_prg, sizeof(default_prg));
		 panel_info = panel_info1;
         gAlarm = 0;

//ltg 			result = t3000Request(COMMAND_50, PANEL_INFO1_COMMAND, (DByte*)&p_info1, 0, station_num, networkaddress);
            alignPanelInfo(1, (DByte *)&p_info1, &panel_info1);
			if( result == RS232_SUCCESS )
			{
				l1 = sizeof(tbl_bank);
//ltg 				result=t3000Request(COMMAND_50, TABLEPOINTS_COMMAND, (DByte *)tbl_bank, &l1, station_num, networkaddress);
			}
			if( result == RS232_SUCCESS )
			{
//ltg 					 result = t3000Request(COMMAND_50, DEFAULT_PRG_COMMAND, default_prg, 0, station_num, networkaddress);
			}
			if( result == RS232_SUCCESS )
			{
				Units_element	*punits;
				punits = new Units_element[MAX_UNITS];
				memset(punits,0,sizeof(Units_element)*MAX_UNITS);
//ltg 				if( t3000Request(UNIT+1, 0, (DByte *)punits, 0, station_num, networkaddress)==SUCCESS)
				{
				 upcustomunit(punits);
				}
				delete punits;
			}
			if( (panel_info1.panel_type == T3000 && panel_info1.version >= 216) ||
					(panel_info1.panel_type == MINI_T3000 && panel_info1.version >= 107) )
				if( result == RS232_SUCCESS )
				{
//ltg 				 result = t3000Request(COMMAND_50, ALARM_NOTIFY_COMMAND, &gAlarm, 0, station_num, networkaddress);
						 galarm_read_time=timestart+60;
				}
		 if(t)
		 {
		   if( result == RS232_SUCCESS )
		   {
			if( panel_info1.panel_type == T3000 )
			{
//ltg              result = t3000Request(COMMAND_50, STATION_LIST_COMMAND, (DByte*)station_list, 0, station_num, networkaddress);
		    }
			else
			{
			 for( int i=0; i<32; i++ )
			 {
				 station_list[i].state = 0;
				 station_list[i].des_length = 0;
			 }
			 station_list[station_num-1].state = 1;
			 station_list[station_num-1].des_length = panel_info1.des_length;
			 memcpy((DByte*)station_list[station_num-1].name, panel_info1.panel_name, NAME_SIZE);
		    }
		   }
		   if( result == RS232_SUCCESS )
		   {
//ltg 			 result = t3000Request(PASS+1, 0, (DByte *)&::passwords, 0, station_num, networkaddress, 0, 30);
		   }
		   if( result == RS232_SUCCESS )
		   {
//ltg 				 result = t3000Request(TIME_COMMAND, 0, (DByte *)&time_buf, 0, station_num, networkaddress, 0, TIMEOUT_NETCALL);
		   }
		   if( result == RS232_SUCCESS )
		   {
				adjust_readtime(&time_buf);
				t = mktime((struct tm *)&time_buf);
				disable();
				stime((time_t *)&t);
				memcpy(&ora_current, &time_buf, sizeof(Time_block));
				enable();
		   }
		 }
		 if( result!=SUCCESS )
		 {
			memcpy( tbl_bank, tbl_bank1, sizeof(tbl_bank));
			memcpy( default_prg, default_prg1, 13);
			panel_info1 = panel_info;
		 }
		 return result;
}
*/


/* ltg
void adjust_readtime( Time_block *time_buf )  // mini->T3000
{
				Time_block_mini timemini;
				if( Panel_Info1.panel_type == MINI_T3000 )
				{
				 memcpy(&timemini, time_buf, sizeof(Time_block_mini) );
				 time_buf->ti_sec=timemini.ti_sec;         // 0-59
				 time_buf->ti_min=timemini.ti_min;         // 0-59
				 time_buf->ti_hour=timemini.ti_hour;           // 0-23
				 time_buf->dayofmonth=timemini.dayofmonth;   // 1-31
				 time_buf->month=timemini.month;          // 0-11
				 time_buf->year=timemini.year;           // year - 1900
				 time_buf->dayofweek=timemini.dayofweek;        // 0-6 ; 0=sunday
				 time_buf->dayofyear=timemini.dayofyear;    // 0-365 gmtime
				 time_buf->isdst=timemini.isdst;
				}
}
*/

/* ltg
void adjust_writetime(Time_block *time_buf, Time_block_mini *timemini)
{
				if( Panel_Info1.panel_type == MINI_T3000 )
				{
				  timemini->ti_sec=time_buf->ti_sec;         // 0-59
				  timemini->ti_min=time_buf->ti_min;         // 0-59
				  timemini->ti_hour=time_buf->ti_hour;           // 0-23
				  timemini->dayofmonth=time_buf->dayofmonth;   // 1-31
				  timemini->month=time_buf->month;          // 0-11
				  timemini->year=time_buf->year;           // year - 1900
				  timemini->dayofweek=time_buf->dayofweek;        // 0-6 ; 0=sunday
				  timemini->dayofyear=time_buf->dayofyear;    // 0-365 gmtime
				  timemini->isdst=time_buf->isdst;
				}
}
*/

/* ltg
void upcustomunit(Units_element	*punits)
{
 for(DInt i=0;i<MAX_UNITS;i++)
 {
	DInt x,y, z;
	z = custom_digital1 - DIG1;

    x =  min(5,strlen(punits[i].digital_units_off));
	memcpy(dig_range_array[z+i].range_text, punits[i].digital_units_off,x);
	dig_range_array[z+i].range_text[x]='/';
	y =  min(5,strlen(punits[i].digital_units_on));
	memcpy(&dig_range_array[z+i].range_text[x+1], punits[i].digital_units_on,x);
	dig_range_array[z+i].range_text[x+1+y]=0;
	strcpy(dig_range_array[z+i].dunits[1], punits[i].digital_units_on);
	strcpy(dig_range_array[z+i].dunits[0], punits[i].digital_units_off);

   if(5 < strlen(punits[i].digital_units_off))
   {
	   x = 5;
   }
   else
   {
   	x =  strlen(punits[i].digital_units_off);
   }
	memcpy(dig_range_array[z+i].range_text, punits[i].digital_units_off,x);
	dig_range_array[z+i].range_text[x]='/';
   if( 5<strlen(punits[i].digital_units_on) )
   {
	 y =  5;
   }
   else
	 y =  strlen(punits[i].digital_units_on);
	memcpy(&dig_range_array[z+i].range_text[x+1], punits[i].digital_units_on,x);
	dig_range_array[z+i].range_text[x+1+y]=0;
	strcpy(&dig_range_array[z+i].dunits[1][0], punits[i].digital_units_on);
	strcpy(&dig_range_array[z+i].dunits[0][0], punits[i].digital_units_off);
 }
}
*/

/**********************************************************************/
/* LTG:  July 30, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Set local panel: not connecting mini_panel.                      */
/*--------------------------------------------------------------------*/
/*   Calling:  In this program.                                       */
/*--------------------------------------------------------------------*/
/* Updating: July 30, 1997                                            */
/**********************************************************************/
/* ltg
void setlocalpanel(void)
{
  local_panel = 1;
  station_num = Station_NUM;            // ltg: local station NO.
  networkaddress = NetworkAddress;      // ltg: local network adr.
  panel_info1 = Panel_Info1;
  memcpy( tbl_bank, ptr_panel->table_bank, sizeof(tbl_bank));
  memcpy( default_prg, ptr_panel->Default_Program, 13);
  upcustomunit(ptr_panel->units);
  ptr_panel->GlPanel = station_num;
  ptr_panel->GlNetwork = networkaddress;

  return;
}
*/

/**********************************************************************/
/* LTG:  July 30, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   DisConnect serial port.                                          */
/*--------------------------------------------------------------------*/
/*   Calling:  In wint300Proc.cpp                                     */
/*--------------------------------------------------------------------*/
/* Updating: July 30, 1997                                            */
/**********************************************************************/
/* ltg
void disconnect_serial(void)
{
  setlocalpanel();
  serial_access = 0;    //ltg NOT connectting serial port.
  desinstallPort(comPort);
}
*/

/* ltg
void reset_serial(void)
{
	serial_access = 0;
	local_panel = 1;
//ltg 	ser_ptr = NULL;
}
*/

DInt pwait(DWORD timesleep, unsigned int msg, MSG& myMsg, DInt breakcond)
{
   DWORD            dwStopTime, timeVar;
   /*
	The following loop determines the status of the serial
	interface, which specifies whether the application will act
	as sender or receiver.

	This loop gets the system time and waits for about 4 timer
	clicks. During this time, the application releases the CPU for
	message processing, and for getting important WM_TIMER
	messages for PollRxQueue.
   */
   dwStopTime  = GetTickCount() + timesleep;
   timeVar = GetTickCount() + 800;
   while(GetTickCount() < dwStopTime)
   {
      if(GetTickCount() > timeVar)
      {
        timeVar = GetTickCount() + 800;
        runImage();
      }
	  if(PeekMessage(&myMsg, NULL, NULL, NULL, PM_REMOVE))
	  {
       if( myMsg.message == msg )
         return FALSE;
       if( breakcond )
        if( myMsg.message == WM_KEYDOWN && myMsg.wParam == VK_CANCEL )
	    {
         return TRUE;
	    }
	   TranslateMessage(&myMsg);
	   DispatchMessage (&myMsg);
	  }
   }
   return TRUE;
}

int readdes_serial(void)
{
/*
		unsigned int l1, net_length=0;
		int result=ERROR_COMM;
		GWindow *D=NULL;

//		if(station_list[station_num-1].state)
		net_length = station_list[station_num-1].des_length;

		if( net_length )
		{
		 DisplayMessage(19, 7, 59, 11, ctrlbreak_text, "Reading descriptors        ", NULL, Blue, D);


		 if(Des)
		 {
				delete Des;
				Des = 0;
		 }
		 Des = new XmsDes(net_length);
//		for(i=0;i<32;i++)
//		if(Station_NUM!=i+1)
//		 if(station_list[station_num-1].state)
		 {
					char r,b[3];
					b[0] = '.'; b[1] = 0;
					int i=0;
					for(int j=0;j<=AY;j++)
					 if(point_des[j]>=0)
					 {
						 Delay(100);
									 l1=0;
						 if( (result=t3000Request(47, j+1, Des->buffer, &l1, station_num, networkaddress, 0, TIMEOUT_NETCALL)) )
						 {
							Delay(1000);
										 l1=0;
							if( (result=t3000Request(47, j+1, Des->buffer, &l1, station_num, networkaddress, 0, TIMEOUT_NETCALL))!=SUCCESS)
							 break;
						 }
						 if(result==SUCCESS)
						 {
							Des->put(station_num,j,l1);
							D->GWPuts(1, 26 + i++, b, Lightblue, Black);
						 }
					 }
		 }
			}
		if(D) {DeleteMessage(D); D=NULL;}
		return result;
*/
		unsigned long activepanels;
		unsigned int l, l1, net_length=0;
		int i,result=ERROR_COMM;
		Panel_info1 panel_info;
// ltg  		GWindow *D=NULL;

		activepanels = panel_info1.active_panels;
		activepanels = activepanels | (1L<<(station_num-1));
		for(i=0;i<MAX_STATIONS;i++)
		 if( (activepanels&(1L<<i)) )
		 {
//ltg 				if( (result=t3000Request(COMMAND_50, PANEL_INFO1_COMMAND, (char*)&panel_info, 0, i+1, networkaddress))!=SUCCESS )
//ltg 				 result = t3000Request(COMMAND_50, PANEL_INFO1_COMMAND, (char*)&panel_info, 0, i+1, networkaddress);
				if(result==SUCCESS)
				{
					station_list[i].state = 1;
					memcpy(station_list[i].name, panel_info.panel_name, sizeof(Station_NAME));
					station_list[i].des_length=panel_info.des_length;
				}
		 }
		l=net_length=0;
		for(i=0;i<MAX_STATIONS;i++)
		 if( (activepanels&(1L<<i)) )
		 {
			 net_length += station_list[i].des_length;
		 }
		if(Des)
		{
			delete Des;
			Des = 0;
		}
		if(net_length)
			Des = new XmsDes(net_length);

		if(net_length)
		{
//ltg 		 DisplayMessageT(19, 7, 59, 11, ctrlbreak_text, "Reading descriptors        ", NULL, Blue, D);
		 for(i=0;i<MAX_STATIONS;i++)
			if( (activepanels&(1L<<i)) )
			{
				char b[3];
//				int k=0;
				b[0] = '.'; b[1] = 0;
// ltg 				if(D)
				{
//ltg 				 D->GWPuts(1, 26, "           ", Lightblue, Black);
				 for(int j=0;j<=AY;j++)
				 if(point_des[j]>=0)
				 {
//ltg 					 Delay(100);
					 l1=0;
//ltg 					 if( (result=t3000Request(47, j+1, Des->buffer, &l1, i+1, networkaddress))!=SUCCESS )
//ltg 						result=t3000Request(47, j+1, Des->buffer, &l1, i+1, networkaddress);
					 if( result==SUCCESS )
					 {
						if(l+l1>net_length) break;
						l += l1;
						Des->put(i+1,j,l1);
//ltg 						D->GWPuts(1, 26 + k++, b, Lightblue, Black);
					 }
					 else
						 break;
				 }
				}
			}
		}
//ltg 		if(D) {DeleteMessage(D); D=NULL;}
		return result;
}

void countdeslength(void)
{
//	int j;
//ltg 	Point point;
//	unsigned int length;

//ltg 	length = search_point( point, NULL, NULL, 0, LENGTH );

/*ltg	Panel_Info1.des_length = length;
	for(j=0; j<MAX_Routing_table ; j++)
	{
	  if( (Routing_table[j].status&RS485_ACTIVE)==RS485_ACTIVE )
		{
		  ((class MSTP *)Routing_table[j].ptr)->station_list[Station_NUM-1].des_length=length;
			((class MSTP *)Routing_table[j].ptr)->panel_info1.des_length=length;
	  }
	}
*/
}

int read_descriptors(void)
{
		unsigned int l1;
		int i,result=ERROR_COMM;
		Panel_info1 panel_info;
		Station_point *station_list;
// ltg  		GWindow *D=NULL;
		unsigned int l,net_length;
		unsigned long activepanels;

// ltg 		for(l1=0; l1<MAX_Routing_table ; l1++)
		{
/* ltg
		 if( (Routing_table[l1].status&RS485_ACTIVE)==RS485_ACTIVE )
		 {
			if( Routing_table[l1].Port.network == networkaddress ) break;
		 }
*/
		}
// ltg 	  if( l1<MAX_Routing_table )
	  {
//ltg 		if( (ready_for_descriptors&0x08) || (main_screen&0x02) )
//ltg 			Delay(4000);
//		station_list = ((class MSTP *)Routing_table[l1].ptr)->station_list;
//ltg 		activepanels = ((class MSTP *)Routing_table[l1].ptr)->panel_info1.active_panels;

		for(i=0;i<MAX_STATIONS;i++)
		 if( (activepanels&(1L<<i)) && Station_NUM!=i+1 )
		 {
//			if( !(((class MSTP *)Routing_table[l1].ptr)->need_info&(1l<<i)) )
			{
//ltg 				if( (result=t3000Request(COMMAND_50, PANEL_INFO1_COMMAND, (char*)&panel_info, 0, i+1, networkaddress))!=SUCCESS )
//ltg 				 result = t3000Request(COMMAND_50, PANEL_INFO1_COMMAND, (char*)&panel_info, 0, i+1, networkaddress);
				if(result==SUCCESS)
				{
//ltg 					((class MSTP *)Routing_table[l1].ptr)->station_list[i].state = 1;
//ltg 					 memcpy( ((class MSTP *)Routing_table[l1].ptr)->station_list[i].name, panel_info.panel_name, sizeof(Station_NAME));
//ltg 					 ((class MSTP *)Routing_table[l1].ptr)->station_list[i].des_length=panel_info.des_length;
				}
			}
		 }

//ltg 		station_list = ((class MSTP *)Routing_table[l1].ptr)->station_list;
		l=net_length=0;
		for(i=0;i<MAX_STATIONS;i++)
		 if( (activepanels&(1L<<i)) && Station_NUM!=i+1 )
		 {
			 net_length += station_list[i].des_length;
		 }

		if(Des)
		{
			delete Des;
			Des = 0;
		}
		if(net_length)
			Des = new XmsDes(net_length);

		if(net_length)
		{
//ltg 		 DisplayMessageT(18, 7, 61, 11, ctrlbreak_text, "Reading descriptors        ", NULL, Blue, D);
		 for(i=0;i<MAX_STATIONS;i++)
			if(Station_NUM!=i+1)
			if( (activepanels&(1L<<i)) )
			{
				char b[3];
				int k=0;
				b[0] = '.'; b[1] = 0;
//				station_list[i].state &= ~0x02;
// ltg 				if(D)
				{
//ltg 				 D->GWPuts(1, 26, "           ", Lightblue, Black);
				 for(int j=0;j<=AY;j++)
				 if(point_des[j]>=0)
				 {
//ltg 					 Delay(100);
					 l1=0;
//ltg 					 if( (result=t3000Request(47, j+1, Des->buffer, &l1, i+1, networkaddress))!=SUCCESS )
//ltg 					  result=t3000Request(47, j+1, Des->buffer, &l1, i+1, networkaddress);
					 if( result==SUCCESS )
					 {
						if(l+l1>net_length) break;
						l += l1;
						Des->put(i+1,j,l1);
//ltg 						D->GWPuts(1, 26 + k++, b, Lightblue, Black);
					 }
					 else
						 break;
				 }
				}
			}
		}
//ltg 		if(D) {DeleteMessage(D); D=NULL;}
		}
	  return result;
}

/* ltg
void repeat(void)
{
 int man;

	G.GShowMenuBar(message,listonscreen);

	qtt=0;
	logout=0;

	do {

		 if(ready_for_descriptors&0x08)
		 {
			read_descriptors();
			ready_for_descriptors &= ~0x08;
		 }
		 display_pointer();
		 if(!timesave)
		 {
		  if( save_prg_flag )
		  {
			ptr_panel->savefile(ptr_panel->Default_Program, 1);
			save_prg_flag = 0;
		  }
		  timesave=300000;
		 }
		 main_screen=1;
		 man=G.GReturn();
		 main_screen=0;
		 if(man == 68)
		 {
			memsize();
			continue;
		 }
		 if(man == 0x70)
		 {
		  if(!local_panel)
		  {
			resetop();
		  }
		  continue;
		 }
		 if(man == 0x71)
		 {
			if(local_panel)
			{
				{
					 ptr_panel->go_text();
					 interface_mode = MODE_TEXT;
				}
			}
			continue;
		 }

// **********************************************************************
// LTG:  June 12, 1997.
//   LOAD DEFAULT PANEL: *.PRG
// **********************************************************************
		 if(man == -32)
		 {
			man = comman_line(2, 21, 79, 23, fond_color);
		 }
// **********************************************************************
// LTG:  June 12, 1997.
//   processing main menu: user input...
// **********************************************************************
		 ptr_panel->Select(man);


		 print_alarm_word();


	} while(!qtt && !logout);

 }
*/ 



extern char *ralign(char *,int );
extern int savedes();
extern int loaddes();

// ltg extern GWControl Control;
extern edit(char *);
//char ptr[60];
unsigned char *mesaj;
void ChangeDir();
void About();
void ChangeFile(char *ptr);
//unsigned _ovrbuffer = 0x500;

extern char  des[MAX_DES];
int error_net=0;

char subpbuf[5]; //for read sub_panel


//output_form output[128];


/*ltg
void showpoints(int man, int Panel_type, int Panel, int Network, int arg=0)
{
				int ptype,mrow,rx,lx,stk=GRIDINS,ttype=0,ly,ry;
				char ca=0, *ptitle;
//				mrow=8;
				mrow   = 16;
				ptitle = NULL;
				lx = 2;
				ly = 3;
				rx = 79;
				ry = 21;
				switch (man) {
				 case M0+0:
//ltg 							ptype=CONNECT_COM;
							lx=19; rx=62;
							ly = 7;
							stk = GRID;
							mrow=4;
							ca=1;
							break;
				 case M1+1:
//ltg 							ptype=NET_SEL;
							lx=5; rx=46;
							ly = 5;
							stk = GRID;
							ca=1;
							ptitle = "Network List";
							break;
				 case M2+0:
							ptype=GRP;
							lx=10; rx=71;
							break;
				 case M2+1:
							ptype=PRG;
							lx=10; rx=72;
//							mrow=5;
							break;
				 case M2+3:
							ptype=OUT;
							stk = GRID;
							break;
				 case M2+2:
//ltg 							ptype=IN;
							rx=80;
							stk = GRID;
							break;
				 case M2+4:
							ptype=VAR;
							stk = GRID;
							lx=9; rx=72;
//							mrow=16;
							break;
				 case M2+5:
							ptype=CON;
//							mrow=8;
							break;
				 case M2+6:
							ptype=WR;
//							mrow=4;
							break;
				 case M2+7:
							ptype=AR;
							lx=18; rx=63;
				         ly = 6;
//							mrow=2;
							mrow=8;
							break;
				 case M2+8:
							ptype=AY;
							lx=30; rx=50;
							break;
				 case M3+0:
							ptype=AMON;
							lx=6;rx=59;
							break;
				 case M3+1:
							ptype=TBL;
							ttype=INDIVIDUAL;
							lx=30;rx=50;
							ca=1;
							break;
				 case M3+2:
							ptype=UNIT;
							mrow=8;
							lx=15;rx=65;
							ca=1;
							break;
				 case M3+3:
							rx=80;lx=2;
							ptype=ALARMM;
							ca=1;
							break;
				 }

				if( (ca) || (check_access(ptype)!=R_NA) )
				{
//ltg 				 GGrid *wgrid;
				 int row;
				 row=mrow;
				 switch(ptype){
				 case GRP:
				 case PRG:
				 case OUT:
				 case IN:
				 case VAR:
				 case CON:
				 case WR:
				 case AR:
				 case AY:
				 case AMON:
				 case UNIT:
					 row=min(mrow,tbl_bank[ptype]);
					 break;
				 }
				 error_net=0;
				 ry = ly+row+2;
				 if(!mode_text)
				 {
					lx--;
					lx = lx*8;
					ly = ly*8;
					rx = rx*8-3;
					ry = ly + row*20 + 24 + 24 + 17;
					switch(ptype){
						 case IN:
							 rx -= 10;   //6;
							 lx += 4;   //6;
							 break;
						 case WR:
							 rx -= 4;   //6;
							 lx += 4;
							 break;
						 case AR:
						 case PRG:
							 rx += 16;
							 break;
						 case VAR:
							 rx += 8;
							 break;
						 case AY:
							 rx += 18;
							 break;
					}
				 }
//				 wgrid=new GGrid(lx,ly,rx,ly+row+2,stk,NULL,ptype,Panel_type,Panel,Network,ttype,arg);
				 wgrid=new GGrid( lx, ly, rx, ry, ptitle, Lightgray, Darkgray, Black,
													stk, NULL, ptype, Panel_type, Panel, Network, ttype, arg,SMALL_FONT,5,1);
				 if(!wgrid->grid_error)
				 {
					if (!error_net)
					{
					if(!mode_text)
					{
//						wgrid->save = 0;   //NO_SAVE
					}
//					wgrid->GWSet(ptitle,Lightgray,Darkgray);
					display_amons = 1;
					wgrid->GShowGrid();
					display_amons = 0;
					refresh = 1;
					refresh_time = refresh_time_init;
					while (!wgrid->HandleEvent())
					{
					 unsigned long l;
					 if(!refresh_time && ptype!=ALARMM && ptype!=AMON && ptype!=AMON_INPUTS
						 && ptype!=ALARMS && ptype!=TBL && ptype!=UNIT && ptype!=CONNECT_COM
						 && ptype!=NET_SEL )
					 {
						 if(wgrid->modify)
						 {
							 wgrid->GSend(SENDPOINTS);
							 wgrid->modify = 0;
						 }
						 else
							 wgrid->GRead(SMALLBANK);
						 int w_orecord = wgrid->w_record;
						 int w_ofield = wgrid->w_field;

						 wgrid->w_nfields = 0;
						 wgrid->t_fields[wgrid->w_lfield-1]=1;
						 wgrid->w_field = 1;
						 wgrid->GGDrawGrid(Black);
						 wgrid->GGHideCur(Black);
						 wgrid->w_record = w_orecord;
						 wgrid->w_field = w_ofield;
						 wgrid->GGShowCur(Black);

						 refresh_time = refresh_time_init;
					 }
					 print_alarm_word();
					};
					}
				 }
				 delete wgrid;
				 refresh = 0;
//		 set_pointer_shape(ARROW);
				 display_pointer();
				}

}
*/

void getpassword(char *pass, char *text)
{
/*
 int l = (strlen(text)+14)/2;
		 hide_pointer();
		 Bordergr(40-l, 9, 40+l, 13, Black, Lightgray, White, White, 1, 0);
		 mxyputs( 42-l, 11,  text, Lightgray, Black);
		 chbkg(28+l,11, 8, Darkgray, White);
		 mgotoxy(28+l,11);
		 mgets(pass,0,Darkgray, White);
		 clear_color(39-l, 42+l, 9, 13, lin_text, fond_color, Black);
		 display_pointer();
*/
//ltg 		 rtrim(pass);
}

/* ltg
int  Panel::connect_panel_proc(int t)
{
		 unsigned char tbl_bank1[MAX_TBL_BANK];
		 Panel_info1 panel_info;
		 char default_prg1[13];
		 RS232Error result;
		 Time_block time_buf;
		 unsigned int l1;

		  memcpy( tbl_bank1, tbl_bank, sizeof(tbl_bank1));
		  memcpy( default_prg1, default_prg, 13);
		  panel_info = panel_info1;
      gAlarm = 0;
		  result = t3000Request(COMMAND_50, PANEL_INFO1_COMMAND, (char*)&panel_info1, 0, station_num, networkaddress);
		  if( result == RS232_SUCCESS )
			{
//				Delay(100);
				l1 = sizeof(tbl_bank);
				result=t3000Request(COMMAND_50, TABLEPOINTS_COMMAND, (char *)tbl_bank, &l1, station_num, networkaddress);

			}
			if( result == RS232_SUCCESS )
			{
//					if( panel_info1.panel_type == T3000 )
					{
					 result = t3000Request(COMMAND_50, DEFAULT_PRG_COMMAND, default_prg, 0, station_num, networkaddress);
					}
			}
			if( result == RS232_SUCCESS )
			{
				Units_element	*punits;
				punits = new Units_element[MAX_UNITS];
				memset(punits,0,sizeof(Units_element)*MAX_UNITS);
				if( t3000Request(UNIT+1, 0, (char *)punits, 0, station_num, networkaddress)==SUCCESS)
				{
				 upcustomunit(punits);
				}
			}
			if( (panel_info1.panel_type == T3000 && panel_info1.version >= 216) ||
					(panel_info1.panel_type == MINI_T3000 && panel_info1.version >= 107) )
				if( result == RS232_SUCCESS )
				{
				 result = t3000Request(COMMAND_50, ALARM_NOTIFY_COMMAND, &gAlarm, 0, station_num, networkaddress);
         galarm_read_time=timestart+60;
				}
		 if(t)
		 {
			if( result == RS232_SUCCESS )
			{
			if( panel_info1.panel_type == T3000 )
			{
			 result = t3000Request(COMMAND_50, STATION_LIST_COMMAND, (char*)station_list, 0, station_num, networkaddress);
			}
			else
			{
			 for( int i=0; i<32; i++ )
			 {
				 station_list[i].state = 0;
				 station_list[i].des_length = 0;
			 }
			 station_list[station_num-1].state = 1;
			 station_list[station_num-1].des_length = panel_info1.des_length;
			 memcpy((char*)station_list[station_num-1].name, panel_info1.panel_name, NAME_SIZE);
			}
			}
			if( result == RS232_SUCCESS )
			{
//			 Delay(100);
			 result = t3000Request(PASS+1, 0, (char *)&::passwords, 0, station_num, networkaddress, 0, 30);
			}
			if( result == RS232_SUCCESS )
			{
//				 Delay(100);
				 result = t3000Request(TIME_COMMAND, 0, (char *)&time_buf, 0, station_num, networkaddress, 0, TIMEOUT_NETCALL);
			}
			if( result == RS232_SUCCESS )
			{
				adjust_readtime(&time_buf);
				t = mktime((struct tm *)&time_buf);
				disable();
				stime((time_t *)&t);
				memcpy(&ora_current, &time_buf, sizeof(Time_block));
				enable();
//				memcpy( (char*)station_list, (char*)ser_ptr->remote_list,
//																								32*sizeof( Station_point ) );
			}
		 }
		 if( result!=SUCCESS )
		 {
			memcpy( tbl_bank, tbl_bank1, sizeof(tbl_bank));
			memcpy( default_prg, default_prg1, 13);
			panel_info1 = panel_info;
		 }
		 return result;
}
*/

/**********************************************************************/
/* LTG:  June 12, 1997.                                               */
/*   Main menu processing.                                            */
/*   Get users input, then do what user want to do.                   */
/**********************************************************************/
/* ltg
void Panel::Select(int man)
{
// char *message[]={{" ~Convert"},{"~Edit"},{"~Miscellaneous"},{" E~xit"},0};
  int i,j,one_net=-1;

  switch(man)
	{

// **********************************************************************
// LTG:  June 13, 1997.
//   select panel.
// *********************************************************************
	  case M1+1:
		 for(j=0,i=0; i<MAX_Routing_table ; i++)
		  {
			 if( (Routing_table[i].status&PORT_ACTIVE)==PORT_ACTIVE )
			  {
				 j++;
				 one_net=i;
			  }
		  }
		 if ( j != 1 )
			one_net = -1;

	  case M0+0:
	  case M2+0:
	  case M2+1:
	  case M2+2:
	  case M2+3:
	  case M2+4:
	  case M2+5:
	  case M2+6:
	  case M2+7:
	  case M2+8:
	  case M3+0:
	  case M3+2:
	  case M3+3: //		if(panel_info1.panel_type == MINI_T3000) break;
	 if(one_net<0)
	 {
		entry_selected = 0;
		G.ShowunderMBar();
		showpoints(man, GlPanel_type, GlPanel, GlNetwork);
//		showpoints(man);
		G.GShowMenuBar(message);
//				 ChangeDir();
	 }
	 else
	 {
	  network_sel_tmp = Routing_table[one_net].Port.network;
	  panel_sel_tmp = station_num;

	  if( (Routing_table[one_net].status&RS485_ACTIVE)==RS485_ACTIVE )
	  {
		pactivepanels = &((class MSTP *)Routing_table[one_net].ptr)->panel_info1.active_panels;
		pstation_list_sel = &((class MSTP *)Routing_table[one_net].ptr)->station_list[0];
	  }
	  else
	  {
		pactivepanels = &panel_info1.active_panels;
		pstation_list_sel = &station_list[0];
	  }
		::select_panel(CSTATION, Routing_table[one_net].Port.network, 1);
		network_select = network_sel_tmp;
		panel_select   = panel_sel_tmp;
	 }

// **********************************************************************
// LTG:  June 13, 1997.
//   Connect a mini-panel through a serial port.
// **********************************************************************
	 if(man==(M0+0))
	 {
	  int com_select,system_entry,l;

	  com_select = entry_selected-1;
	  if( com_select >= 0)
		if( comm_info[com_select].connection )
	  {
		if( comm_info[com_select].connection == SERIAL_LINK || comm_info[com_select].connection == MODEM_LINK )
		{
		  if( comm_info[com_select].connection!=MOUSE_LINK )
		  {
			if ( (l=checkport(com_select)) >= 0 )
			{
			if( (Routing_table[l].status&PTP_ACTIVE)!=PTP_ACTIVE )
			{
			  system_entry = -1;
			  if( comm_info[com_select].connection==MODEM_LINK )
				{
				 entry_selected = 0;
				 if( FXOpen("syslist.dbf", &syslist_handle, READWRITE ) == FX_OPEN_ERROR )
					 if(FXCreate( "syslist.dbf", 5, dial_struct, &syslist_handle )!=FX_OPEN_ERROR)
						 for(int i=0; i<32;i++) FXPutRec(syslist_handle);
				 FXClose( syslist_handle );

				 GGrid *system_list;
//				 system_list = new GGrid(3,3,76,21,GRID,"syslist.dbf",0,0,0,0,0,0);
				 system_list = new GGrid(3,3,76,21,"System List",Lightgray,Darkgray,Black,
																 GRID,"syslist.dbf",0,0,0,0,0,0);
//				 system_list->GWSet("System List", Lightgray,Darkgray);
				 system_list->GShowGrid();
				 while (!system_list->HandleEvent());
				 delete system_list;
				 if( !entry_selected ) return;
				 system_entry = entry_selected-1;
			  }
			  if( serial_access ) disconnect_serial(1);
			  GWindow *D=NULL;
			  serial_access = 1;
				local_panel = 0;

			  if( ((class PTP *)Routing_table[l].ptr)->activity == BUSY )
			  { serial_access = 0; return; }
			  ser_ptr = (class PTP *)Routing_table[l].ptr;
			  ser_ptr->Set_mode( MASTER );
//			  DisplayMessage(mode_text?19:200, mode_text?7:150, mode_text?61:500, mode_text?11:200, ctrlbreak_text, "  Connecting to serial...  ", &D, Blue);
			  RS232Error result;
			  if( system_entry>=0 )
			  {
						 if( ser_ptr->modem_active )
						 {
							 if( ( ser_ptr->modem_status = ser_ptr->modem_obj->Initialize() ) != MODEM_SUCCESS )
							 {
								 reset_serial();
								 return;
							 }
							 if( ser_ptr->modem_status = ser_ptr->modem_obj->Dial( dial_list_data.phone_number ) != MODEM_SUCCESS )
							 {
								 if(D) DeleteMessage(D);
								 ser_ptr->modem_obj->Disconnect();
								 reset_serial();
								 return;
							 }
                      Delay(1000);
							 if(D) {DeleteMessage(D);D=NULL;};
							 Routing_table[ser_ptr->port_number].port_status_vars.connection = PTP_DISCONNECTED;
							 ((class PTP *)Routing_table[ser_ptr->port_number].ptr)->connection_state = PTP_DISCONNECTED;
						 }
						 else
						 {
//								if(D) DeleteMessage(D);
//ltg 								DisplayMessageT(20, 7, 60, 11, NULL, " Modem port not installed ", NULL, Blue, NULL, 1500);
								reset_serial();
								return;
						 }
				 }
				if(ser_ptr)
				{
				 ser_ptr->activity = BUSY;
				 refresh_time_init = 200;
				 disconnect_time = DISCONNECT_TIME;
				 if( ser_ptr->ReadBaudRate()<14400 ) refresh_time_init = 400;
				 memcpy( (char*)ptr_panel->local_stations, (char*)station_list,
							 MAX_STATIONS*sizeof( Station_point ) );
				 memcpy( (char*)&passwords, (char*)&::passwords,sizeof( passwords ) );
				 memset( (char*)station_list, 0, MAX_STATIONS*sizeof( Station_point ) );
				 if( ser_ptr->modem_active )
				 {
						Delay(1000);
				 }
				 ser_ptr->port->FlushRXBuffer();
				 ser_ptr->port->FlushTXBuffer();
				 estab_text[strlen(estab_text)-5]=com_select+0x31;
				 DisplayMessage(mode_text?15:200, mode_text?7:150, mode_text?65:500, mode_text?11:200, ctrlbreak_text, estab_text, &D, Blue);
				 if ( (result=t3000Request(50, ESTABLISH_CONNECTION_PORT, NULL, 0, com_select, 0xFFFF, 0, 50) ) == SUCCESS )
				 {
				  if( (result=connect_panel_proc(1))==RS232_SUCCESS )
				  {
					if(D) {DeleteMessage(D);D=NULL;};
//					D.GReleaseWindow();
//					clear_color(1, 78, 5, 20, lin_text, fond_color, Black);
					ptr_panel->GlPanel = station_num;
					ptr_panel->GlNetwork = networkaddress;
					char b[80];
					strcpy(b,"  Connected to panel ");
					strcat(b,station_list[station_num-1].name);
//ltg 					DisplayMessageT(20, 7, 60, 11, ctrlbreak_text, b, &D, Blue);
				  }
				  else
					{
					 if(D) {DeleteMessage(D);D=NULL;};
					 disconnect_serial();
//ltg 					 DisplayMessageT(20, 7, 60, 11, NULL, "  Communication error !  ", &D, Blue);
				  }
				 }
				 else
				 {
					if(D) {DeleteMessage(D);D=NULL;};
					if(result==ERROR_NET)
//ltg 					 DisplayMessageT(20, 7, 60, 11, NULL, "           Connection failed.              Change the local network number.", &D, Blue);
					 Delay(7000);
				 }
				 entry_selected = 0;
				 Delay(1000);
//				 D.GReleaseWindow();
				 if(D) DeleteMessage(D);
#ifdef SERIAL_COMM
				 if( result==SUCCESS )
					if( serial_access )
					{
						readdes_serial();
					}


				 if( result!=SUCCESS )
				 {
				  disconnect_serial(1);
				 }
				 else
				 {
				  if(!get_access(ESC))   //ltg : ESC ????
					disconnect_serial(1);
				 }
				 if( result==SUCCESS )
				 {
				  strcpy(remote_path, oldpath);
				  rtrim(dial_list_data.des_file);
				  if( strlen(dial_list_data.des_file) )
				  {
					strcat(remote_path,"\\");
					strcat(remote_path,dial_list_data.des_file);
					if( chdir(remote_path) )
					{
					 mkdir( dial_list_data.des_file );
					 chdir(remote_path);
					}
				  }
				 }
#endif
				}
			}
			else
			{
//ltg 			  DisplayMessageT(20, 7, 60, 11, NULL, " Connection already established! ", NULL, Blue, NULL, 2000);
			}
			}
		  }
			else
		  {
//ltg 			DisplayMessageT(20, 7, 60, 11, NULL, " This is a mouse port! ", NULL, Blue, NULL, 2000);
		  }
		}
		else
		{
//ltg 		  if( comm_info[com_select].connection == RS485_LINK )
//ltg 			  DisplayMessageT(20, 7, 60, 11, NULL, " RS485 network installed! ", NULL, Blue, NULL, 2000);
//ltg 		  else
//ltg 			  DisplayMessageT(20, 7, 60, 11, NULL, " Ayncron Mode installed! ", NULL, Blue, NULL, 2000);
		}
	  }
	  else
	  {
//ltg 			DisplayMessageT(20, 7, 60, 11, NULL, " COM port not installed! ", NULL, Blue, NULL, 2000);
	  }
	 }

// **********************************************************************
// LTG:  June 13, 1997.
//   Select a panel.
// *********************************************************************
	 if ( man == (M1+1) )
	 {
		RS232Error error;
		int l1, st, net;

		if( station_num!=panel_select || networkaddress!=network_select)
		{
		 st = station_num;
		 net= networkaddress;
		 l1 = local_panel;
		 station_num=panel_select;
		 networkaddress=network_select;
		 if(station_num==Station_NUM && networkaddress==NetworkAddress) local_panel=1;
		 else local_panel=0;
//		 memcpy( tbl_bank1, tbl_bank, sizeof(tbl_bank1));
//		 memcpy( default_prg1, default_prg, 13);
//		 panel_info = panel_info1;
		 if(!local_panel)
		 {
//  memsize();
			if( (error=ptr_panel->connect_panel_proc())!=SUCCESS )
		  {
			 station_num = st;
			 networkaddress = net;
			 local_panel=l1;
			}
		 }
		 else
		 {
			 setlocalpanel();
		 }
		 ptr_panel->GlPanel = station_num;
		 ptr_panel->GlNetwork = networkaddress;

		 display_pointer();

		}
		}
		break;


//				 ChangeDir();

// **********************************************************************
// LTG:  June 13, 1997.
//   Disconnect a mini-panel.
// *********************************************************************
	  case M0+1:
			 disconnect_serial(1);
			 break;

// **********************************************************************
// LTG:  June 12, 1997.
//   About T3000.
// **********************************************************************
	  case M0+2:
			 About();
//	exec_program(ptr_panel->program_codes[0]);
			 break;

// **********************************************************************
// LTG:  June 13, 1997.
//   System information.
// **********************************************************************
	  case M0+3:
		 infoscreen();
		 break;

// **********************************************************************
// LTG:  June 13, 1997.
//   Transfer file.
// **********************************************************************
	  case M0+4:
		 if( !local_panel && panel_info1.panel_type == T3000 )
			filetransfer_routine();
		 else
			G.pGPopUpTable[0].ReleasePopUp(-1);
		 break;

// **********************************************************************
// LTG:  June 13, 1997.
//   LOAD descriptors.
// **********************************************************************
	  case M0+5:
		 if( serial_access )
			readdes_serial();
		 else
			read_descriptors();
		 break;

// **********************************************************************
// LTG:  June 13, 1997.
//   Update mini-panel.
// **********************************************************************
	  case M0+6:
		 mini_operations();
		 break;

	  //ltg: exit.
	  case M0+7:
//		   savedes();
		 if ( ReadYesNoButton(24, 8, 55, 14, Lightgray, Darkgray, Black,
									 Cyan, White, "   Exit application ?",
									 exit_button, noexit_button, 'X', 'N') )
		  {
			 char pass[15]={0};

			 getpassword(pass, " Password : ");
			 if ( !strcmp(pass,password) || !strcmp(pass,supervisor) )
			  {
				 if (serial_access)
					disconnect_serial(1);
				 qtt=1;
				 exit_apl=1;
			  }
		  }
		 break;

	  // ltg: logout.
	  case M0+8:
		 if(serial_access) disconnect_serial(1);
		 logout=1;
		 break;

	  // ltg: DATA/TABLE ???
	  case M3+1:
		 {
			int i;

			i = dialog_select( min(tbl_bank[TBL],5), (char *)tables_text, 0,
									 32, 6, Cyan,Blue);           // 0: don't save.

			if ( i>=0 )
			 {
				G.ShowunderMBar();
				showpoints(man,GlPanel_type, GlPanel, GlNetwork,i+1);
				G.GShowMenuBar(message);
			 }
		 }
		 break;

	  // LTG  CONFIG / SETUP
	  case M4+0:
		 if( check_access(12)!=R_NA )
			setup();
		 break;

     // LTG SONFIG / USERLIST
	  case M4+1:
		{
		 if( check_access(12)!=R_NA )
		 {
			GGrid *wgrid;
			GWindow *wwin;
			char *p;
			hide_pointer();
			p = "      ENTER - select     INS - add      DEL - delete       ";
		wwin=NULL;
			if(mode_text)
			{
			 Bordergr(10, 4, 70, 22, Blue, Lightblue, White, White, 1, 0);
//		Border(10,4,70,22, Lightgreen, Green );
//		mtextcolor(Black) ;
			 mxyputs(11,21,p,Lightgray,Black);
			}
			else
			{
				wwin=new GWindow(78,60,562,430,"Password system",Lightgray,Darkgray,Black,NO_STACK,0);
				wwin->GShowWindow(WIN_ERROR);
				wwin->GWPuts(15,1,p,Lightgray,Black);
			}
			wgrid=new GGrid(mode_text?13:100,mode_text?7:110,mode_text?29:250,mode_text?19:360,GRID,NULL,USER_NAME,GlPanel_type,GlPanel,GlNetwork);
			wgrid->GWSet(NULL,Lightgray,Darkgray);
			wgrid->GShowGrid();
			while (!wgrid->HandleEvent());
			delete wgrid;
			if(mode_text)
			{
			 clear_color(9, 64, 4, 22, lin_text, fond_color, Black);
			}
			else
			 if(wwin)
			 {
         wwin->GReleaseWindow();
				 delete wwin;
			 }
		 }
		}
				 break;

//	case M4+3:
//				 boardconfig();
//				 strcpy(station_list[Station_NUM-1].name,Station_NAME);
//				 break;

	  // LTG CONFIG / HARDWARE.
	  case M4+2:
			hardware();
//			portconfig();
			break;


// **********************************************************************
// LTG:  June 12, 1997.
//   read panel file.
// **********************************************************************
	  case M1+0:
		 file();                      // In this program, LINE 5998.
		 break;


// **********************************************************************
// LTG:  June 12, 1997.
//   PRINT panel file.
// **********************************************************************
	  case M1+2:
//		 select_subpanel();
		 break;

// **********************************************************************
// LTG:  June 12, 1997.
//   clear selected panel.
// **********************************************************************

	case M1+3:
#ifndef WORKSTATION
					 suspend(VIRTUAL);
#endif
					 if(ReadYesNo(25, 9, 58, 13, Lightgray, Darkgray, Black,
													Cyan, White, "Clear panel? (Y/N): "))
					 {
						 if( local_panel )
							 clearpanel();
						 else
						 {
							 if( t3000Request(CLEARPANEL_T3000, 0, NULL, 0 ,station_num, networkaddress)!=SUCCESS )
							 {
									t3000Request(CLEARPANEL_T3000, 0, NULL, 0 ,station_num, networkaddress);
							 }
//ltg 							 DisplayMessageT(20, 7, 60, 11, NULL, " Wait for panel initialization... ", NULL, Blue, NULL, 5000);
				 }
					 }
#ifndef WORKSTATION
					 resume(VIRTUAL);
#endif
					break;
	case M1+4:
//	Select SERIAL CONNECTION
//			ser_p->Set_working_port( LOCAL_PORT );
			break;
	case M1+5:
//	Select MODEM CONNECTION
//			ser_p->Set_working_port( MODEM_PORT );
			break;
	default:break;
			}
};

*/

void Panel::Path()
{
 int i = 1;  //ltg add "=1".
 
//ltg  i=dialog_select( 2, (char *)array_sel_path, 0, 32, 7, Lightgray, Darkgray, "Path");  // 0=don't save

 if(Program_Path[strlen(Program_Path)-1] == '\\')  Program_Path[strlen(Program_Path)-1]=0;
 if(Descriptor_Path[strlen(Descriptor_Path)-1] == '\\')  Descriptor_Path[strlen(Descriptor_Path)-1]=0;
 if(Basic_Path[strlen(Basic_Path)-1] == '\\')  Basic_Path[strlen(Basic_Path)-1]=0;
 if(Monitor_Path[strlen(Monitor_Path)-1] == '\\')  Monitor_Path[strlen(Monitor_Path)-1]=0;
 if(Graphics_Path[strlen(Graphics_Path)-1] == '\\')  Graphics_Path[strlen(Graphics_Path)-1]=0;
}

void select_panel(int type, int net_no, int where)
{
 //ltg		RS232Error error;
//ltg		int l1, st, net;
		unsigned char tbl_bank1[MAX_TBL_BANK];
		Panel_info1 panel_info;
		char default_prg1[13];
//ltg 		GGrid *wgrid;

//ltg 		hide_pointer();

//ltg 		wgrid->GWSet("Select Panel",Lightgray,Darkgray);

//ltg 		wgrid->GShowGrid();

//ltg 		while (!wgrid->HandleEvent());

//ltg 		delete wgrid;

//ltg 		if(station_num==Station_NUM && networkaddress==NetworkAddress) local_panel=1;
//ltg 		else local_panel=0;

}


/*
void pselect(GWDialog *D)
{
 D->GDGets(&ptr_panel->panel_name[D->index_cur][0],(D->index_cur)%16,4+((D->index_cur)/16)*24,20,Blue,White);
}
*/
/*
void Panel::select_subpanel()
{
 char xbuf[5];
 select_panel(0);
 GWDialog D(12,6,63,11,NO_STACK,0);

 D.GWSet("Select Sub Panel ",Lightgray,Darkgray);

 D.GShowWindow(DIALOG);

 D.GWPuts(1,2,"Select Sub Panel ( A1-A124 or B1-B124 ) : ",Lightgray,Black);

 mtextbackground(Cyan);
 mtextcolor(Black);

// D.GDDef_object(subselect,44,1,4,Cyan,Black);
// while(!D.HandleEvent());
	 D.GDPuts(57,9,"    ",4,Cyan,Black);
	 strcpy(xbuf,"    ");
	 mgotoxy(57,9);
//	 D.wingets(xbuf, 1, 44, 4, Lightcyan, Black, STRING);
	 wingets(xbuf, 1, 44, 4, Lightcyan, Black, STRING);
	 D.GReleaseWindow();

 if (atoi(&xbuf[1])>=1 && atoi(&xbuf[1])<=124)
	{
	 strupr(xbuf);
	 if (xbuf[0]=='A' )
		 GlNet_type=SUBA;
	 else
		 if (xbuf[0]=='B')
				 GlNet_type=SUBB;
		 else
				 return;

	 GlPanel_type=SUB_MINI;
	 GlSub_panel = atoi(&xbuf[1]);
	 mgotoxy(14,25);
	 mtextbackground(Lightgray);
	 mtextcolor(White);
	 mcputs("  Sub");
	 mcputs(xbuf);
	}
};
*/

int program_routine(int num_point,int point_type,int num_panel,int num_net)
{
//	int i;
/* ltg
	asm push es;
*/
//ltg 			if(!mode_text)
			{
//ltg 				lat_char_n = 1;
//ltg 				lung_char_n = 1;
				Green=LIGHTBLUE;                 //2
				Green1=WHITE; 						 //5
				Blue1=DARKGRAY;                 //6
				Darkgray=BROWN;           //8
				Lightblue=GREEN;         //9
				White=MAGENTA;                 //15
				fond_color=TEXT_FOND_COLOR;
//ltg 				mode_text = MODE_TEXT;
//ltg 				hide_pointer();
//ltg               cleardevice();
//ltg 				restorecrtmode();
				changechar();
/* ltg
				asm {
					mov ax,21h
					int 33h
					}
*/                    
//ltg 				move_mouse(30,20);
//ltg 				display_pointer();
			}

		Str_program_point block;
// ltg 		if (!local_request(num_panel))
		{
//ltg 			Delay(100);
//			i = countlength(20, ((point_type+1)<<8) + num_point-1);
//ltg 			if( t3000Request(20, ((point_type+1)<<8)+ num_point-1, (char *)&block, 0 ,num_panel, num_net) )
			{
//ltg 				asm pop es;
				return 1;
			}
		}
// ltg 		else
			memmove(&block,&ptr_panel->programs[num_point-1],sizeof(Str_program_point));

//ltg 		if( edit_program(&block, num_point-1, num_panel, num_net) )
		{
//ltg 		 asm pop es;
		 return 1;
		}

// ltg 		if (!local_request(num_panel))
		{
//ltg 			Delay(100);
//ltg 			t3000Request(20+100, ((point_type+1)<<8)+ num_point-1, (char *)&block, 0, num_panel, num_net);
		}
// ltg 		else
			memmove(&ptr_panel->programs[num_point-1], &block, sizeof(Str_program_point));
//ltg  asm pop es;
}

int icommand(char *buf, int from, byte panel, int network)
{
 char *ptitle;
 ptitle = NULL;
//ltg  GGrid *wgrid;
 byte num_point,var_type,point_type,num_panel;
 int  num_net;
 int n=0;

//ltg  if( ispoint(buf,&num_point,&var_type,&point_type,&num_panel,&num_net,network,panel) )
 {
	n=cod_command[point_type];
	if(check_access(point_type, num_point-1)!=R_NA)
	{
	if( (from && (point_type==T3000_OUT  ||  point_type == T3000_IN  || point_type==VAR
							|| point_type==CON)) ||
			(!from && (point_type==T3000_OUT || point_type==T3000_IN || point_type==VAR
							|| point_type==CON || point_type==PRG || point_type==WR
							|| point_type==AR || point_type==GRP)) )
				{
				 int row;
				 row=1;
				 error_net=0;
				int rx=79,lx=2,stk; // ltg  =GRIDINS;
				switch (point_type) {
				 case GRP:
							lx=13; rx=68;
							break;
				 case PRG:
				 case T3000_OUT:
				 case T3000_IN:
// ltg 							stk = GRID;
							break;
				 case CON:
				 case WR:
							break;
				 case VAR:
							lx=9; rx=72;
							break;
				 case AR:
							lx=12; rx=69;
							break;
				 case AMON:
							lx=8;rx=53;
							break;
				 case TBL:
							lx=5;rx=75;
							break;
				 }
//ltg 				 wgrid=new GGrid(lx,6,rx,6+row+2,stk,NULL,point_type,T3000,num_panel,num_net,INDIVIDUAL,num_point,SMALL_FONT,5,1);
/* ltg
				 if(!wgrid->grid_error)
				 {
					if (!error_net)
					{
					if(!mode_text)
					{
						wgrid->ltop.x=lx*width_char_size();wgrid->ltop.y=6*height_char_size();
						wgrid->rbottom.x=rx*width_char_size()-1;wgrid->rbottom.y=(6+row)*height_char_size()+32;
						wgrid->save = 0;   //NO_SAVE
					}
//					wgrid->GWSet(ptitle[point_type],Lightgray,Darkgray);
					wgrid->GWSet(ptitle,Lightgray,Darkgray);
					wgrid->GShowGrid();
					refresh = 1;
//				 refresh_time = ReadTime();
					refresh_time = refresh_time_init;
//				 time_t t;
//				 t = mtime(NULL);
					while (!wgrid->HandleEvent())
					{
							if(!refresh_time)
							{
								 if(wgrid->modify)
										wgrid->GSend();
								 else
										wgrid->GRead();
								 int w_orecord = wgrid->w_record;
								 int w_ofield = wgrid->w_field;

								wgrid->w_nfields = 0;
								wgrid->t_fields[wgrid->w_lfield-1]=1;
								wgrid->w_field = 1;
								wgrid->GGDrawGrid(Black);
								wgrid->GGHideCur(Black);
								wgrid->w_record = w_orecord;
								wgrid->w_field = w_ofield;
								wgrid->GGShowCur(Black);

								refresh_time = refresh_time_init;
							}
					}
				  }
				 }

				 delete wgrid;
				 refresh = 0;
				 display_pointer();
				 n=-1;
				 return n;
*/
				}


		 if(from && point_type==PRG)
			 program_routine(num_point,point_type,num_panel,num_net);

/* ltg
		 if(from && point_type==WR)
			 weekly_routine(num_point,num_panel,num_net,INDIVIDUAL);


		 if(from && point_type==AR)
			 annual_routine(num_point,num_panel,num_net,INDIVIDUAL);
*/             

		 if(from && point_type==AMON)
		 {

//ltg 		 amon_routine(num_point,num_panel,num_net,INDIVIDUAL,0,NULL);
/* ltg
		 if(mode_text)
			 {
				G.save=0;
				G.GShowMenuBar(message);
				G.save=1;
				clear_color(1, 80, 2, 25, lin_text, fond_color, Black);
			 }
*/
		 }

		if(from && point_type==GRP)
		{
		 ind_nested_grp=0;
//ltg 		 memset(&openscreen, 0, 2);
//ltg 		 if(grp_proc(num_point, num_panel, networkaddress, INDIVIDUAL)==27) n=27;
		 if(default_group) n=27;
		}
	 return n;
	}
 }
}


int comman_line(int lx, int ly, int rx, int ry, int fond_color)
              // ltg , GEdit *edit)
{
/* ltg
 char buf[21];
 int n,j,num_panel,num_net;
 char pbuf[7];
 char pc[11],*p,*q;
 while(1)
 {
 memset(buf,0,21);
 Bordergr(lx, ly, rx, ry, Black ,Lightgray,fond_color,fond_color,1,0);
 mxyputs(lx+1, (ly+ry)/2," >                                                     ",Lightgray,Black);
 mgotoxy(lx+3,22);
 n = (int)wingets(buf, (ly+ry)/2, lx+3, 20, Lightgray, Black);
 for( int y=ly-1;y<=24;y++)
 {
		mxyputs(1,y,lin_text,fond_color,Black);
 }

 if(n==27)
 {
		 n = -1;
		 break;
 }
 for(int i=0; i<=AY;i++)
 {
	 p = buf;
	 num_net=0;
	 num_panel=0;
	 if ( (q=strchr(buf,'_'))!=NULL )
	 {
		memcpy(pc,buf,min( (int)(q-buf),10));
		pc[min((int)(q-buf),10)]=0;
		q++;
		num_net = atoi(pc);
		p = q;
      if(!num_net) continue; 
	 }
	 num_panel = atoi(p);
	 if(num_panel)
	 {
		 j=0;
		 while(p[j])
		 {
		  if(!isdigit(p[j])) break;
		  j++;
		 }
		 p = &p[j];
	 }
	 else
		if(num_net) continue;
	 strcpy(pbuf,point_cod[i]);
	 strcat(pbuf,"S");
	 if(!strcmp(pbuf,p))
		 break;
 }
 if(i<=AY)
 {
	 n=cod_command[i];
//	 ptr_panel->showpoints(n, T3000, station_num, networkaddress);
    if(!num_net) num_net=networkaddress; 
    if(!num_panel) num_panel=station_num; 
	 showpoints(n, T3000, num_panel, num_net);
 }
 else
	 n=icommand(buf, 1, station_num, networkaddress);

	if(edit) edit->showedit(edit->beginscreen);
	if(n==27) {n=-1;break;}
 }
 hidecur();
 return n;
 */
}


/*
static unsigned int m[100];


unsigned int i=0,j=0;
asm push es

_ES=FP_SEG(&m[0]);
_SI=FP_OFF(&m[0]);
_CX=0;
asm {
	 xor dx,dx
		}
memloop: asm  mov ah,48h
asm {
		 mov bx,0ffffh
		 int 21h
		 or bx,bx
		 jz done
		 add dx,bx
		 mov ah,48h
		 int 21h
		 mov es:[si],ax
		 inc si
		 inc si
		 inc cx
		}
//		m[i++]=(char *)MK_FP(j,0);
asm		jmp memloop
done:
asm mov i,cx
	nr=(unsigned long)_DX*16;
	for(j=0;j<i;j++)
	{
	 _ES=m[j];
	asm  mov ah,49h
	asm 	 int 21h
	}
asm pop es
	 mcprintf("total mem 21h : %s \n\r",ltoa(nr,xbuf,10));
*/

/**********************************************************************/
/* LTG:  Aug.  7, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Save load point ????                                             */
/*--------------------------------------------------------------------*/
/*                                                                    */
/*--------------------------------------------------------------------*/
/* Calling:  In this program.                                         */
/*--------------------------------------------------------------------*/
/* Updating: Aug.  7, 1997                                            */
/**********************************************************************/
int	saveloadpoint(int type, int point_type, int n_points, int size,
                  int panel, FILE *h, char *buf)
{
  int r;
  DUint length;
  int n=3;
  length = n_points*size;

  while(n--)
  {
    // read all output objects.s
    if( (r=t3000Request( type + point_type+1 , 0, buf, &length,
                         station_num, networkaddress)) == SUCCESS )
      break;
  }

  if( r==SUCCESS )
  {
    if(!type)
    {
	  n = length / size;
	  if( panel_info1.panel_type==MINI_T3000 )
	  {
	    if( point_type==T3000_OUT )
		  n = MAX_OUTS_MINI;

	    if( point_type==T3000_IN )
		  n = MAX_INS_MINI;
	  }

	  fwrite(	&n, 2, 1, h);
	  fwrite(	&size, 2, 1, h);
	  fwrite(	buf, size, n, h);
    }
  }
  return r;
}


void changechar(int arg)
{
/* ltg
char **p1,*p2,*p3, *p4, *ptext_auxiliary;
int n = 34;
int o=200;

	 if( text == ASCII_FONT) return;
	 if(arg)
	 {

		if((seg_sel.l=GlobalDosAlloc(1000))==0)
		{
		 text=ASCII_FONT;
		 return;
		}

	 }

	 p1 = (char **)MK_FP(__Seg0040,0xa8);
	 SAVE_PTR = *p1;
	 if(arg)
	 {
		_BX = FP_SEG(*p1);
		asm {
		 mov ax,0002h
		 int 31h
		 mov WORD PTR ega_selector, ax
		}
	 }

	 p4 = (char *)MK_FP(ega_selector, FP_OFF(*p1));
	 p3 = (char *)MK_FP(seg_sel.seg_sel.sel, 0);
//	 memcpy(EGA_data_block, p4, 28);
	 memcpy(p3, p4, 32);
	 if(arg)
	 {
		memcpy(p3 + 47, characters_font, sizeof(characters_font));
	 }
//	 *p1 = (char *)EGA_data_block;
//	 p2 = (char *)&EGA_data_block[0] + 8;
	 p2 = p3 + 8;
//	 p3 = (char *)text_auxiliary;
//	 memcpy(p2,&p3,4);
	 p4 = (char *)MK_FP(seg_sel.seg_sel.seg, 34);
	 memcpy(p2,&p4,4);
	 if(arg)
	 {
		ptext_auxiliary = p3 + 34;
		ptext_auxiliary[0] = 16;
		ptext_auxiliary[1] = 0;
		memcpy(&ptext_auxiliary[2],&n,2);
		memcpy(&ptext_auxiliary[4],&o,2);
//	 p3 = characters_font;
		p4 += 13;
//	 memcpy(&text_auxiliary[6],&p3,4);
		memcpy(&ptext_auxiliary[6],&p4,4);
		ptext_auxiliary[10] = 0x0ff;
		ptext_auxiliary[11] = 3;
		ptext_auxiliary[12] = 0x0ff;
	 }
	 *p1 = (char *)MK_FP(seg_sel.seg_sel.seg,0);

	asm {
	 mov ah,0
	 mov al,83h
	 int 10h
	}
*/
}
void restorechar(int arg)
{
/* ltg
char **p1;
	 if( text == ASCII_FONT) return;

	 p1 = (char **)MK_FP(__Seg0040,0xa8);
	 *p1=SAVE_PTR;
	 if(arg)
	 {
		GlobalDosFree(seg_sel.seg_sel.sel);
		asm {
			mov ah,0
			mov al,83h
			int 10h
		}
	 }
*/
}

int updateremotepoint(Point_Net point)
{
  int j;

  if( point.zero() ) return -1;
  if( point.point_type-1 == GRP || point.point_type-1 == AMON ) return -1;
  if( point.network == 0xFFFF ) point.network = NetworkAddress;
  for( j=0; j < MAXREMOTEPOINTS; j++)
  {
//ltg 	 if( remote_points_list[j].info.point == point )
	 {
//ltg 	  break;
	 }
  }
  if(j==MAXREMOTEPOINTS)
  {
	for(j=0;j<MAXREMOTEPOINTS;j++)
	{
//ltg 	 if( remote_points_list[j].info.point.zero() )
	 {
//ltg 		memset(&remote_points_list[j].info, 0, sizeof(Point_info));
//ltg 		remote_points_list[j].info.point=point;
//ltg 		remote_points_list[j].state = OK;
//ltg 		remote_points_list[j].read = 1;
		break;
	 }
	}
  }
  if(j==MAXREMOTEPOINTS) j=0;
//ltg   remote_points_list[j].count++;
//  want_points_list[j].point=point;
//  want_points_list[j].point.network = NetworkAddress;
  return j;
}


void deleteremotepoint(Point_Net point)
{
  int j;

  if( point.network == 0xFFFF ) point.network = NetworkAddress;
  for( j=0; j < MAXREMOTEPOINTS; j++)
  {
//ltg 	 if(remote_points_list[j].info.point==point)
	 {
//ltg 	  break;
	 }
  }
  if(j<MAXREMOTEPOINTS)
  {
//ltg 	 if( !(--remote_points_list[j].count) )
	 {
//ltg 		 memset(&remote_points_list[j],0,sizeof(REMOTE_POINTS));
//		 remote_points_list[j].info.point.setzero();
//		 want_points_list[j].point.setzero();
//		 want_points_list[j].destination = 255;
	 }
  }
}

void updateremotepoints(int ind_remote_local_list, struct remote_local_list *remote_local_list)
{
 int j;
 for(int i=0;i<ind_remote_local_list;i++)
 {
  if( (j=updateremotepoint(remote_local_list[i].point)) >= 0 )
	  remote_local_list[i].index=j;
 }
}

void grp_updateremotepoint(Control_group_elements cgrp)
{
  Str_grp_element *p;
  p = cgrp.ptrgrp;
  for(int i=0;i<cgrp.nr_elements;i++,p++)
	if(p->point_info.point.panel+1!=Station_NUM)
	{
	 updateremotepoint(p->point_info.point);
	}
}

void mini_operations(void)
{
 char buf[13], cont;
 char *p, *q;
 int n,crc,result,crc_file;
// ltg  GWindow *D=NULL;
 long j, addr, length, l, buffer_length, l1;
 unsigned i;

 if( panel_info1.panel_type == MINI_T3000 )
  if(!local_panel)
  {
//ltg 	if(ReadYesNo(20, 9, 65, 13, Lightgray, Darkgray, Black, Cyan, White, "Update mini firmware ? (Y/N): "))
	{
	  buf[0]=0;
//ltg 	  ReadWindow(24, 7, 56, 11, Lightgray, Darkgray, Black,
//ltg 							 Cyan, White, "File name : ", buf, 12);
	  if( strlen(buf) )
	  {
// send data
		  if ((h=fopen(buf,"rb"))!=NULL)
		  {
			 SectorInfo si[8];  // 7+buffer_length
			 length =  sizeof(SectorInfo);
//  read flash status
          i=0;
//ltg 			 if( ( result = t3000Request(COMMAND_50, READFLASHSTATUS_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS)
//ltg 				  return;
			 memcpy(&buffer_length, si, 4);   // buffer_available
			 if( buffer_length > 65535 ) buffer_length = 65535;
			 p = new char[buffer_length];
			 q = p;
			 if(!p)
			 {
//ltg 				if(D) DeleteMessage(D);
//ltg 				DisplayMessageT(20, 7, 60, 11, NULL, " Enough memory ! ", NULL, Blue, NULL, 10000);
				return;
			 }
//ltg 			 if(ReadYesNo(20, 9, 65, 13, Lightgray, Darkgray, Black, Cyan, White, "Proceed with update ? (Y/N): "))
			 {
// check file format
			  fread(&crc_file,1,2,h);
			  fread(&addr,1,4,h);
			  fread(&l,1,4,h);
			  while(!feof(h))
			  {
				 if(l>buffer_length-8)
				 {
//ltg 				  DisplayMessageT(20, 7, 60, 11, NULL, " Error buffer length! ", NULL, Blue, NULL, 10000);
				  result=ERROR_COMM;
				  break;
				 }
				 fread(p,l,1,h);
				 crc = 0;
//ltg 				 for(j=0; j<l; j++)
//ltg 					crc=CalcDataCRC(p[j], crc);
				 if(crc!=crc_file)
				 {
//ltg 				  DisplayMessageT(20, 7, 60, 11, NULL, " Wrong CRC ! ", NULL, Blue, NULL, 10000);
				  result=ERROR_COMM;
				  break;
				 }
				 fread(&crc_file,1,2,h);
				 fread(&addr,1,4,h);
				 fread(&l,1,4,h);
			  }
			  fseek(h,0,SEEK_SET);
//
			 if( result == SUCCESS )
			 {
//ltg 			  DisplayMessage(mode_text?19:200, mode_text?7:150, mode_text?61:500, mode_text?11:200, ctrlbreak_text, "The update will take about four minutes!", NULL, Blue, NULL, 3000);
			  fread(&crc_file,1,2,h);
			  fread(&addr,1,4,h);
			  fread(&l,1,4,h);
			  while(!feof(h))
			  {
				cont = 1;
				length = 0;
				p = q;
				crc = 0;
				if(l>buffer_length-8)
				{
//ltg 				 DisplayMessageT(20, 7, 60, 11, NULL, " Error buffer length! ", NULL, Blue, NULL, 10000);
				 break;
				}
				while(!feof(h) && length + l + 8 <= buffer_length )
				{
				 memcpy(p+length, &addr, 4);
				 memcpy(p+length+4, &l, 4);
				 fread(p+length+8,l,1,h);

				 length += 8;
//ltg 				 for(j=0; j<l; j++)
//ltg 					crc=CalcDataCRC(p[length+j], crc);
				 length += l;
			    fread(&crc_file,1,2,h);
				 fread(&addr,1,4,h);
				 fread(&l,1,4,h);
				}
				if( length > 65535 )
				{
				  i = 65535;
				}
				else
				{
				  i = length;
				}
				strcpy(tempbuf,"Sending data to ");
				memcpy(&tempbuf[16], panel_info1.panel_name, NAME_SIZE);
				tempbuf[16+NAME_SIZE]=0;
//ltg 				rtrim(tempbuf);
				strcat(tempbuf,"...   ");
//ltg 				DisplayMessageT(18, 7, 62, 11, NULL, tempbuf, &D);
				while(i)
				{
//  how to continue:  0 - continue  1 - reset
//ltg 				 if( ( result = t3000Request(COMMAND_50+100, ((unsigned)(cont<<15))+(unsigned)SENDDATAMINI_COMMAND, p, &i, station_num, networkaddress, NETCALL_SIGN))!=SUCCESS)
				 {
//ltg 				  if( ( result = t3000Request(COMMAND_50+100, ((unsigned)(cont<<15))+(unsigned)SENDDATAMINI_COMMAND, p, &i, station_num, networkaddress, NETCALL_SIGN))!=SUCCESS)
//ltg 					if( ( result = t3000Request(COMMAND_50+100, ((unsigned)(cont<<15))+(unsigned)SENDDATAMINI_COMMAND, p, &i, station_num, networkaddress, NETCALL_SIGN))!=SUCCESS)
					{
//ltg 						DisplayMessageT(20, 7, 60, 11, NULL, " Error send data! ", NULL, Blue, NULL, 10000);
//ltg						break;
					}
				 }
				 p += i;
				 i = length - i;
				 cont = 0;
				}
//ltg 				if(D) {DeleteMessage(D); D=NULL;}
				if( result == SUCCESS )
				{
// write command
				 i=0;
//ltg 				 Delay(500);
//ltg 				 if( (result = t3000Request(COMMAND_50, WRITEDATAMINI_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS )
//					if( (result = t3000Request(COMMAND_50, WRITEDATAMINI_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS )
//						if( (result = t3000Request(COMMAND_50, WRITEDATAMINI_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS )
						{
//ltg 						 DisplayMessageT(20, 7, 60, 11, NULL, " Error send command! ", NULL, Blue, NULL, 10000);
						 break;
						}
// wait for mini
				 if(serial_access)      // do not disconnect
				 {
/*
				  if( (n = findroutingentry(-1, network, j)) >= 0)
*/
//ltg 				  T_Inactivity = 2100;
				 }
//ltg 				 Delay(1000);
//ltg 				 DisplayMessageT(19, 7, 59, 11, ctrlbreak_text, "Waiting for mini to write data...", &D);
				 result=ERROR_COMM;
				 l1 = timestart;
//ltg 				 Delay(15000);
				 while( timestart<l1+180 )
				 {
				  i=0;
//ltg 				  if( (result = t3000Request(COMMAND_50, READSTATUSWRITEFLASH_COMMAND, (char *)&si, &i, station_num, networkaddress, NETCALL_NOTTIMEOUT))==SUCCESS )
				  {
					if( *(((char *)&si)+1) )
					{
					 if ( crc !=  *(((int *)&si)+1) ) result=ERROR_COMM;
//ltg 					 Delay(1000);
					 break;
					}
					result=ERROR_COMM;
				  }
				 }
				 if(serial_access)
				 {
//ltg 				  T_Inactivity = T_INACTIVITY;
				 }
//ltg 				 Delay(5000);
//ltg 				 if(D) {DeleteMessage(D); D=NULL;}
				 if( result==ERROR_COMM )
				 {
//ltg 					DisplayMessageT(20, 7, 60, 11, NULL, " Error write data! ", NULL, Blue, NULL, 10000);
					break;
				 }
				}
				else
				 break;

				}
//ltg 				Delay(1000);
				if( result==SUCCESS )
				{
				 i=0;
/*ltg				 if( (result=t3000Request(COMMAND_50, RESTARTMINI_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS )
					 if( (result=t3000Request(COMMAND_50, RESTARTMINI_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS )
						 DisplayMessageT(20, 7, 60, 11, NULL, " Error send command! ", NULL, Blue, NULL, 10000);
*/
				 if(serial_access) disconnect_serial(1);
				 else
				 {
					setlocalpanel();
				 }
				 if( result==SUCCESS )
				 {
//ltg 					DisplayMessageT(16, 7, 64, 11, NULL,   "                Update O.K.!                    The panel will reboot in about 20 seconds. ", NULL, Blue, NULL, 20000);
//ltg 					DisplayMessageT(20, 7, 60, 11, NULL, " You have to reconnect...! ",NULL, Black, NULL, 2000);
				 }
				}
//			  if(D) DeleteMessage(D);
			  }
			 }
			 delete q;
			 fclose(h);
		  }
//ltg 		  else
	  }
	}
  }
/*
	GPopUp *popup;
	popup = new GPopUp(1,9,(char **)&list11,0,4);
	int man,i,j,n,crc,result;
	man = popup->GReturn();
	G.pGPopUpTable[0].ReleasePopUp(-1);
	delete popup;
	char buf[13];
	char *p, *q;
	GWindow *D=NULL;
	long addr, length, l, buffer_length, l1;

	if(!local_panel)
	{
	 if(man==0 || man==1)
	 {
     buf[0]=0;
//ltg 	  ReadWindow(20, 7, 60, 11, Lightgray, Darkgray, Black,
//ltg 							 Cyan, White, "File name : ", buf, 12);
	  if( strlen(buf) )
	  {
		if(man==0)
		{        // send data
		  if ((h=fopen(buf,"rb"))!=NULL)
		  {
			  SectorInfo si[8];  // 7+buffer_length
			  DisplayMessage(mode_text?19:200, mode_text?7:150, mode_text?61:500, mode_text?11:200, ctrlbreak_text, "Please do not shut down the computer!", &D, Blue);
			  length =  sizeof(SectorInfo);
//  read flash data
			  if( ( result = t3000Request(COMMAND_50, READFLASHSTATUS_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS)
				  return;
			  memcpy(&buffer_length, si, 4);
			  p = new char[buffer_length];
           q = p;
			  if(!p)
			  {
				if(D) DeleteMessage(D);
				DisplayMessage(20, 7, 60, 11, NULL, " Error memory alloc! ", NULL, Blue, NULL, 10000);
				return;
			  }
			  crc = 0;
			  fread(&addr,1,4,h);
			  fread(&l,1,4,h);
			  while(!feof(h))
			  {
				length = 0;
				if(l>buffer_length-8)
				{
				 DisplayMessage(20, 7, 60, 11, NULL, " Error buffer length! ", NULL, Blue, NULL, 10000);
				 break;
				}
				while(!feof(h) && length + l + 8 <= buffer_length )
				{
				 memcpy(p+length, &addr, 4);
				 memcpy(p+length+4, &l, 4);
				 fread(p+length+8,1,l,h);
				 length += 8;
				 for(i=0; i<l; i++)
					crc=CalcDataCRC(p[length+i], crc);
//				 crc = ~crc;
				 length += l;
				 fread(&addr,1,4,h);
				 fread(&l,1,4,h);
				}
				if( length > 65535 )
				{
				  j = 65535;
				}
				else
				{
				  j = length;
				}
				while(j)
				{
//  how to continue:  0 - continue  1 - reset
				 if( ( result = t3000Request(COMMAND_50+100, SENDDATAMINI_COMMAND, p, &j, station_num, networkaddress))!=SUCCESS)
				 {
				  if( ( result = t3000Request(COMMAND_50+100, SENDDATAMINI_COMMAND, p, &j, station_num, networkaddress))!=SUCCESS)
					if( ( result = t3000Request(COMMAND_50+100, SENDDATAMINI_COMMAND, p, &j, station_num, networkaddress))!=SUCCESS)
					{
						DisplayMessage(20, 7, 60, 11, NULL, " Error send data! ", NULL, Blue, NULL, 10000);
						break;
					}
				 }
				 p += j;
				 j = length - j;
				}
				if( result == SUCCESS )
				{
// write command
				 i=0;
				 if( (result = t3000Request(COMMAND_50, WRITEDATAMINI_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS )
					if( (result = t3000Request(COMMAND_50, WRITEDATAMINI_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS )
						if( (result = t3000Request(COMMAND_50, WRITEDATAMINI_COMMAND, (char *)&si, &i, station_num, networkaddress))!=SUCCESS )
						{
						 DisplayMessage(20, 7, 60, 11, NULL, " Error send command! ", NULL, Blue, NULL, 10000);
						 break;
						}
// wait for mini

				 if(serial_access)      // do not disconnect
				 {
//				  if( (n = findroutingentry(-1, network, j)) >= 0)
				  T_Inactivity = 5500;
				 }
				 result=ERROR_COMM;
				 l1 = timestart;
				 while( timestart<l1+300 )
				 {
				  i=0;
				  if( (result = t3000Request(COMMAND_50, READSTATUSWRITEFLASH_COMMAND, (char *)&si, &i, station_num, networkaddress))==SUCCESS )
				  {
					if( *(((char *)&si)+1) )
					{
					 if ( crc !=  *(((int *)&si)+1) ) result=ERROR_COMM;
					 break;
					}
					result=ERROR_COMM;
				  }
				 }
				 if(serial_access)
				 {
				  T_Inactivity = T_INACTIVITY;
				 }
				 if( result==ERROR_COMM )
				 {
					DisplayMessage(20, 7, 60, 11, NULL, " Error write data! ", NULL, Blue, NULL, 10000);
					break;
				 }
				}
				else
				 break;

			  }
			  if( result==SUCCESS )
			  {
				 i=0;
				 if( t3000Request(COMMAND_50, RESTARTMINI_COMMAND, (char *)&si, &i, station_num, networkaddress)!=SUCCESS )
					 if( t3000Request(COMMAND_50, RESTARTMINI_COMMAND, (char *)&si, &i, station_num, networkaddress)!=SUCCESS )
						 DisplayMessage(20, 7, 60, 11, NULL, " Error send command! ", NULL, Blue, NULL, 10000);
			  }
			  if(D) DeleteMessage(D);
			  delete q;
			  fclose(h);
		  }
		}
		if(man==1)
		{        // send code
		  if ((h=fopen(buf,"rb"))!=NULL)
		  {
			  p = new char[65536];
			  if(!p) return;
			  DisplayMessage(mode_text?19:200, mode_text?7:150, mode_text?61:500, mode_text?11:200, ctrlbreak_text, "Please do not shut down the computer!", &D, Blue);
			  fread(p,1,4,h);
			  fread(p+4,1,2,h);
			  fread(p+6,1,2,h);

				memcpy(&n, p+4, 2);
				fread(p+8,1,n,h);
				n += 8;
				if( ( result = t3000Request(COMMAND_50+100, SENDCODEMINI_COMMAND, p, &n, station_num, networkaddress))!=SUCCESS)
				{
				 if( ( result = t3000Request(COMMAND_50+100, SENDCODEMINI_COMMAND, p, &n, station_num, networkaddress))!=SUCCESS)
					if( ( result = t3000Request(COMMAND_50+100, SENDCODEMINI_COMMAND, p, &n, station_num, networkaddress))!=SUCCESS)
					{
						DisplayMessage(20, 7, 60, 11, NULL, " Error send code! ", NULL, Blue, NULL, 10000);
					}
				}
				if(D) DeleteMessage(D);
				delete p;
			   fclose(h);
		  }
		}

	  }
	}
  }
*/
}

void setup(void)
{
	unsigned int i;
    
/* ltg
	GPopUp *popup;
 	popup = new GPopUp( mode_text?50:400,mode_text?3:56,(char **)&list7,0,5, SMALL_FONT, 5, 1);

	i=popup->GReturn();
	G.pGPopUpTable[4].ReleasePopUp(-1);
	delete popup;
*/

	switch (i) {
	 case 0:
				{
				 char *p,*q;
				 if(local_panel) q=ptr_panel->Default_Program;
				 else q=(char *)default_prg;

				 if((p=strrchr(q,'.'))!=NULL)
				 {
					*p=0;
				 }
				 keyboard_data=0;
//ltg 				 ReadWindow(mode_text?20:140, mode_text?7:140, mode_text?60:450, mode_text?11:200, Lightgray, Darkgray, Black,
//ltg 							 Cyan, White, "Default panel program : ", q, 8);
				 if(keyboard_data)
				  if(!strchr(q,'.'))
				  {
					if(strlen(q) > 8)
						strcpy(&q[8],".PRG");
					else
						strcat(q,".PRG");

					if(local_panel)
						save_config(CNF_PATH_INFO);
					else
					{
					  i = 13;
//ltg 					  t3000Request(50+100, 33, default_prg, &i, station_num, networkaddress);
					}
				  }
				  break;
				}
	 case 1:
//				 G.ShowunderMBar();
				 ptr_panel->Path();
					save_config(CNF_PATH_INFO);
//				 savepath();
//				 G.GShowMenuBar(message);
				 break;
	 case 2:
				 break;
	 case 3:
				 if(!control)
				 {
//ltg 					if(ReadYesNo(20, 9, 60, 13, Lightgray, Darkgray, Black,	Cyan, White,
//ltg 								 "  Console mode?(Y/N): "))
					{
					 console_mode=1;
#ifndef WORKSTATION
//ltg 					 suspend(VIRTUAL);
#endif
					}
//ltg 					else
					{
					 console_mode=0;
#ifndef WORKSTATION
//ltg 					 resume(VIRTUAL);
#endif
					}
					save_config(CNF_CONSOLE_INFO);
				 }
				 break;
	 case 4:
				 break;
	}

}

void hardware(void)
{
// ltg 	GPopUp *popup;
	int i=-1;
/* ltg
	if( check_access(12)!=R_NA )
	{
	 popup= new GPopUp(mode_text?47:386,mode_text?5:87,(char **)&list8,0,3,SMALL_FONT,5,1);
	 i=popup->GReturn();
	 delete popup;
	}
	G.pGPopUpTable[4].ReleasePopUp(-1);

	switch (i) {
	 case 0:
				 if(serial_access) disconnect_serial(1);
				 portconfig();
				 break;
//	 case 1:
//				 modemconfig();
//				 break;
	 case 1:
				 miniconfig();
//				 boardconfig();
				 break;
	 case 2:
				 boardconfig();
//				 terminal_mode();
				 break;

	}

*/
}

/****************************************************************/
/*   Adrian Boici July 7                                        */
/****************************************************************/

//int Panel::loadprg(char *file, Panel_info1 *panel_info, unsigned char *tblbank, int local, int contr)
DInt Panel::loadprg(char *file)
{
	char *buf,btext[8];
	DLong length;   //, coef;
	DInt i,k,m,l1,c1,nitem, ver, vermini, ret, st;
	DUint n,j, l;
	Str_program_point *pprg;

    DInt local = 1;
    DInt contr = 0;

    Panel_info1 *panel_info, pinfo;
    pinfo = panel_info1;
    panel_info = &pinfo;

    DByte tblbank[MAX_TBL_BANK];
    memcpy( tblbank, tbl_bank, sizeof(tbl_bank));

							 buf=new char[7500];   //7000

							 ret = SUCCESS; //ERROR_COMM;
							 st = panel_info->panel_number;
							 if ((h=fopen(file,"rb+"))==NULL)
							 {
//								 if(contr)
//									 DisplayMessageT(20, 7, 60, 11, NULL, " File not loaded! ",NULL, Black, NULL, 1000);
//								 onemin = 1;
							 }
							 else
							 {
								 m = 1;
								 fread( buf, 26, 1, h); //time and date
								 fread( buf, 4, 1, h);  //signature
								 if ( !memcmp(buf, signature, 4) )
								 {
                                  fread(	&n, 2, 1, h);   //panel number
								  fread(	&m, 2, 1, h);   //network number
								  fread(	&ver, 2, 1, h);       //ver T3000
								  fread(	&vermini, 2, 1, h);   //ver mini
								  fread(	buf, 1, 32, h); // reserved bytes
								  if ( ver < 210 || ver==0x2020 )
								  {
//									DisplayMessageT(10, 7, 70, 11, NULL, "File not loaded. File version less then 2.10!",NULL, Black, NULL, 3500);
									m=0;
									}
								 }
								 else
								 {
									m=0;
//									DisplayMessageT(10, 7, 70, 11, NULL, "File not loaded. File corrupt.",NULL, Black, NULL, 3500);
								 }

								 if( n != st && m )
								 {
 										strcpy((char *)buf, "File for panel ");
										itoa(n, (char *)&buf[strlen(buf)], 10);
										strcat((char *)buf, ". Load as panel ");
//										itoa(station_num, &buf[strlen(buf)], 10);
										itoa(n, (char *)&buf[strlen(buf)], 10);
										strcat((char *)buf, " ? (Y/N) : ");

									    UserEnsureForm->Left=MainScreen->Left+(MainScreen->Width-UserEnsureForm->Width)/2;
									    UserEnsureForm->Top = MainScreen->Top + 90;
									    UserEnsureForm->EnsureWinLabel->Caption = AnsiString( (char *)buf );
									    UserEnsureForm->ShowModal();
				      // if user want to load this file
									    if ( UserEnsureForm->Ok )
										{
											if(!control)
											{
											 if(local)
											 {
												station_list[Station_NUM-1].state=0;
												station_num = Station_NUM = n;
												strcpy(station_name, station_list[station_num-1].name);
												strcpy(Station_NAME, station_name);
												station_list[station_num-1].state=1;
												ptr_panel->GlPanel=station_num;
												panel_info->panel_number = station_num;
											 }
											 else
                                                m=0;
											}
										}
										else
											m = 0;
								 }
								if (m)
								{
#ifndef WORKSTATION
//									if(local)
//										set_semaphore(&t3000_flag);
//										suspend(VIRTUAL);
#endif
									m=ERROR_COMM;
									if(local)
									{
										clearpanel();
										m=SUCCESS;
									}
									else
									{
//										if( (m=net_call(CLEARPANEL_T3000, 0, buf, 0 , st, networkaddress))!=SUCCESS )
//											m=net_call(CLEARPANEL_T3000, 0, buf, 0 , st, networkaddress);
										if( m==SUCCESS)
										{
//											 loadprgremote=st;
//											 DisplayMessageT(20, 7, 60, 11, NULL, " Wait for panel initialization... ", NULL, Blue, NULL, 5000);
											 l = sizeof(tbl_bank);
//											 m=net_call(COMMAND_50, TABLEPOINTS_COMMAND, (char *)tblbank, &l, st, networkaddress,NETCALL_RETRY);
										}
									}
								 if(m==SUCCESS)
								 {
//									coef = ftell(h);    // save file pointer
//									fseek(h, 0, SEEK_END);
									length = ftell(h);
//									fseek(h, coef, SEEK_SET);   // restore file pointer
//									coef = ((length*1000L)/20000L)*1000L + (((length*1000L)%20000L)*1000L)/20000L ;
//									float coef = (float)length/20.;
									long ltot = 0;

//									creategauge(&gauge,"Loading file");
//									strcpy(buf,panel_text1);
									lin_text[10]=0;
//									gauge->GWPuts(5, 10, lin_text, Lightblue, Black);
									lin_text[10]=' ';
									itoa(st, &buf[6],10);
//									gauge->GWPuts(5, 10, buf, Lightblue, White);

									if(local)
									{
									 present_analog_monitor=0;
									 programs_size = 0;
									 first_free = 0;
									 for(i=0; i < MAX_HEAP_DIGM-1; i++)
										 heap_dmon[i].next = i+1;
									 nsample_mem_dig = 0;
									}

//									set_semaphore_dos();
									int max_prg, max_grp;
									for(i=T3000_OUT; i<=UNIT; i++)
									{
//									 showgauge(gauge, info[i].name);
									 if(i==AMON)
									 {
										 if( ver>=230 && vermini )
												 continue;
										 if( ver<230 && vermini >= 230 )
										 {
											 delete buf;
//											 DisplayMessage(12, 7, 68, 11, NULL, "File not loaded. Versions conflict!",NULL, Black, NULL, 5000);
											 return ERROR_COMM;
										 }
									 }

									 if(i==ALARMM)
										if( ver < 216 )
										{
										 fread( &n, 2, 1, h);
										 fread( &nitem, 2, 1, h);
										 fread(	buf, n, nitem ,h);
										 continue;
										}
									 if(i!=DMON && i!=ALARMM)
									 {
										fread( &n, 2, 1, h);
										fread( &nitem, 2, 1, h);
										l = min(n,info[i].max_points);
										if(i==GRP)
											max_grp = n;
										if(i==PRG)
										{
											max_prg = n;
										}
										if(local)
										{
										 if(nitem == info[i].str_size)
										 {
											 fread(	buf, nitem, l ,h);
//											 fread(	info[i].address, nitem, l ,h);
                                             align( 1, i, buf, 1, l);
										 }
										 else
										 {
											 m = min(nitem, info[i].str_size);
											 for(j=0; j<l; j++)
											 {
												 fread(	buf, m, 1 ,h);
//												 fread(	info[i].address+j*info[i].str_size, m, 1 ,h);
                                                 align( 1, i, buf, j, 1);
												 if(nitem > info[i].str_size)
														fseek(h, nitem-info[i].str_size, SEEK_CUR);
											 }
										 }
										 if(n > l)
											fseek(h, (n-l)*nitem, SEEK_CUR);
										}
										else
										{
/*
										 fread(	buf, n, nitem ,h);
										 l = min(n,tblbank[i]);
										 if(i==PRG)
										 {
											pprg = (Str_program_point *)buf;
											for(j=0;j<l;j++,pprg++)
											{
											 pprg->bytes = 0;
											}
										 }
										 if( i!=ALARMM )
										 {
											if(i==AMON)
											{
											 if( panel_info->panel_type == T3000 )
											 {
												if( saveloadpoint(100,i,l,nitem,st,panel_info->panel_type,h,buf) )
													ret = ERROR_COMM;
											 }
											 else
											 {
												if( panel_info->version <= 110 )
												{
												 if(saveloadpoint(100,i,l,nitem,st,panel_info->panel_type,h,buf))
													ret = ERROR_COMM;
												}
											 }
											}
											else
											{
											 if(saveloadpoint(100,i,l,nitem,st,panel_info->panel_type,h,buf))
													ret = ERROR_COMM;
											}
										 }

//										 if(n > l)
//											fseek(h, (n-l)*nitem, SEEK_CUR);
*/
										}
										ltot += n*nitem+2;
//										showgauge(gauge, coef, ltot);
									 }
									 if(local)
									 {
										if(i==PRG)
											for(j=0; j < info[i].max_points; j++)
												programs[j].errcode = 0;
										if(i==AMON)
										{
											for(j=0;j < info[i].max_points; j++)
											 for(k=0;k<5;k++)
												monitor_work_data[j].start_index_dig[k] = 0x0FFFF;
											for(j=0;j<info[i].max_points; j++)
											{
											 analog_mon[j].double_flag=1;
											}
											for(j=0;j<info[i].max_points; j++)
												for(k=0;k<analog_mon[j].num_inputs;k++)
												{
												if(analog_mon[j].inputs[k].panel+1!=Station_NUM)
												{
//													updateremotepoint(analog_mon[j].inputs[k],analog_mon[j].inputs[k].network);
												}
												}
										}
										if(i==AY)
										{
										 long *p = heap_array;
										 for(j=0; j<info[i].max_points; j++)
										 {
											if(arrays[j].length)
											{
												arrays_data[j] =  p;
												p += arrays[j].length;
											}
											else
											 arrays_data[j] =  0;
										 }
										}
									 }
									}

									char *p,*q;
									HANDLE handle;
									int n1;
                  handle = 0;
									if(local)
									{
									 ind_heap_buf = 0;
									 memset(heap_buf, 0, MAX_HEAP_BUF);
//									 mfarmalloc(&p, PROGRAM_SIZE, handle);
									 p = new char [PROGRAM_SIZE];
									 if (p==NULL)
									 {
//										 mxyputs(10,10,"Error alloc");
										 abort();
									 }
									}

// load programs code
//									l=min(MAX_PRGS,max_prg);
									btext[0]='P';
									btext[1]='R';
									btext[2]='G';
									btext[3]=' ';
									btext[4]=' ';
									btext[5]=0;
									l = min(max_prg,tblbank[PRG]);
									for(i=0;i< l;i++)
									{
									 itoa(i,&btext[3],10);
//									 showgauge(gauge, btext);
									 if(local)
									 {
//										set_semaphore_dos();
										fread(	&n, 2, 1, h);
										fread( p, n, 1, h);
//										clear_semaphore_dos();
/*
										if(n)
										{
										 q = p;
										 memcpy(&n1, q, 2);
										 q += n1 + 2 + 3;
										 memcpy(&n1, q, 2);
										 q += 2;              //local variables
										 for(j=0;j<n1; )
										 {
											switch(q[j]){
												case FLOAT_TYPE:
												case LONG_TYPE:
													k = 4;
													break;
												case INTEGER_TYPE:
													k = 2;
													break;
												case BYTE_TYPE:
													k = 1;
													break;
												default:
													{
													switch(q[j]){
														case FLOAT_TYPE_ARRAY:
														case LONG_TYPE_ARRAY:
															k = 4;
															break;
														case INTEGER_TYPE_ARRAY:
															k = 2;
															break;
														case BYTE_TYPE_ARRAY:
														case STRING_TYPE:
//														case STRING_TYPE_ARRAY:
															k = 1;
															break;
													 }
													 memcpy(&l1, &q[j+1], 2);
													 memcpy(&c1, &q[j+3], 2);
													 if(l1)
														k *= l1*c1;
													 else
														k *= c1;
													 j += 4;
													}
													break;
											}
											j++;
											memset(&q[j], 0, k);
											j += k;
											j += 1+strlen(&q[j]);
										 }
										 q += n1;
										 memcpy(&n1, q, 2);   //time
										 q += 2+n1;
										 memcpy(&n1,q,2);    //ind_remote_local_list
										 q += 2;         //remote_local_list
										 updateremotepoints(n1, (struct remote_local_list *)q);
										}
										if ((program_codes[i]=Heap_alloc(n))!=NULL)
										{
//										 movedata(FP_SEG(p),FP_OFF(p) ,FP_SEG(program_codes[i]),FP_OFF(program_codes[i]), n);
										}
*/
										programs[i].bytes=n;
										programs_size += n;
										ltot += n+2;
//										showgauge(gauge, coef, ltot);
									 }
									 else
									 {
/*
//										set_semaphore_dos();
										fread(	&n, 2, 1, h);
										fread( buf, n, 1, h);
										ltot += n+2;
//										clear_semaphore_dos();
										if(!n)
										{
										 n=11;
										 memset(buf,0,n);
										 buf[2]=0xfe;
										}
//										setindexremote(buf);
//										if(net_call(16+100,  i, buf, &n , st, networkaddress,NETCALL_RETRY))
//											ret = ERROR_COMM;
*/
									 }
									}
//									set_semaphore_dos();
									for(i=l;i<max_prg;i++)
									{
										fread(	&n, 2, 1, h);
										fread( buf, n, 1, h);
										ltot += n+2;
									}
//									clear_semaphore_dos();
									if(local)
                                    {
//									 mfarfree(handle);
                                     if (p)
                                       delete p;
                                    }
//									showgauge(gauge, coef, ltot);
//									showgauge(gauge, "WR-T");

//									set_semaphore_dos();
									m = MAX_WR * 9 * sizeof(Wr_one_day);
									fread(	&n, 2, 1, h);
									l = min(n,m);
									if(local)
									{
										fread( wr_times, l, 1, h);
										if(n > m)
										 fseek(h, n-m, SEEK_CUR);
//										clear_semaphore_dos();
									}
									else
									{
/*
										fread( buf, n, 1, h);
										j = 9*sizeof(Wr_one_day);
										l = min(n/j, tblbank[WR]);
//										clear_semaphore_dos();
										for(i=0;i<l;i++)
										{
//										 if(net_call(WR_TIME+1+100, i, (char *)&buf[i*j], &j,st, networkaddress,NETCALL_RETRY))
//												ret = ERROR_COMM;
										}
*/
									}
									ltot += n+2;
//									showgauge(gauge, coef, ltot);
//									showgauge(gauge, "AR-D");

//									set_semaphore_dos();
									m = MAX_AR * 46;
									fread(	&n, 2, 1, h);
									l = min(n,m);
									if(local)
									{
									 fread( ar_dates, l, 1, h);
									 if(n > m)
										fseek(h, n-m, SEEK_CUR);
//									 clear_semaphore_dos();
									}
									else
									{
										fread( buf, n, 1, h);
//										clear_semaphore_dos();
										l = min(n/46, tblbank[AR]);
										j=46;
										for(i=0;i<l;i++)
										{
//										 if(net_call(AR_Y+1+100, i, &buf[i*46], &j, st, networkaddress,NETCALL_RETRY))
//												ret = ERROR_COMM;
										}
									}
									ltot += n+2;
//									showgauge(gauge, coef, ltot);

									if(local)
									{
									 ind_heap_grp=0;
									 memset(heap_grp, 0, MAX_HEAP_GRP);
//									 mfarmalloc(&p, MAX_ELEM*sizeof(Str_grp_element), handle);
                                     p = new char [MAX_ELEM*sizeof(Str_grp_element)];
									 if (p==NULL)
									 {
//										 mxyputs(10,10,"Error alloc");
										 abort();
									 }
									}
									Str_grp_element *pgrp;
									nitem = sizeof(Str_grp_element);
//									set_semaphore_dos();
									fread(	&nitem, 2, 1, h);
//									clear_semaphore_dos();

									 btext[0]='G';
									 btext[1]='R';
									 btext[2]='P';
									 btext[3]=' ';
									 btext[4]=' ';
									 btext[5]=0;
									l=min(tblbank[GRP],max_grp);
									for(i=0;i< l;i++)
									{
									 itoa(i,&btext[3],10);
//									 showgauge(gauge, btext);
									 if(local)
									 {
//										set_semaphore_dos();
										fread(	&n, 2, 1, h);
										fread( p, n, 1, h);
//									  clear_semaphore_dos();
										pgrp = (Str_grp_element *)p;
//										for(j=0; j<n/sizeof(Str_grp_element); j++, pgrp++ )
/*
										for(j=0; j<n/32; j++, pgrp++ )
										{
										 pgrp->point_absent = 0;
										 if(pgrp->point_info.point.point_type==T3000_OUT+1)
											if(pgrp->point_info.point.number+1>tblbank[T3000_OUT])
												pgrp->point_absent = 1;
										 if(pgrp->point_info.point.point_type==T3000_IN+1)
											if(pgrp->point_info.point.number+1>tblbank[T3000_IN])
												pgrp->point_absent = 1;
										}
*/
										ltot += n+2;
										if ((control_group_elements[i].ptrgrp=(Str_grp_element *)Heap_grp_alloc(sizeof(Str_grp_element)*(n/32)))!=NULL)
                                        {
//											movedata(FP_SEG(p),FP_OFF(p) ,FP_SEG(control_group_elements[i].ptrgrp),FP_OFF(control_group_elements[i].ptrgrp), n);
//                                            memmove(control_group_elements[i].ptrgrp, p, n);
                                            align( 1, READGROUPELEMENTS_T3000, p, i+1, n/32); //sizeof(Str_grp_element)
                                        }
										control_group_elements[i].nr_elements=n/32;   //sizeof(Str_grp_element);
//										grp_updateremotepoint(control_group_elements[i]);
									 }
									 else
									 {
/*
//									  set_semaphore_dos();
										fread(	&n, 2, 1, h);
										fread( buf, n, 1, h);
//									  clear_semaphore_dos();
										pgrp = (Str_grp_element *)buf;
										for(j=0; j<n/sizeof(Str_grp_element); j++, pgrp++ )
										{
										 pgrp->point_absent = 0;
										 if(pgrp->point_info.point.point_type==T3000_OUT+1)
											if(pgrp->point_info.point.number+1>tblbank[T3000_OUT])
												pgrp->point_absent = 1;
										 if(pgrp->point_info.point.point_type==T3000_IN+1)
											if(pgrp->point_info.point.number+1>tblbank[T3000_IN])
												pgrp->point_absent = 1;
										}
//										if(net_call(19+100, i, (char *)buf, &n, st, networkaddress,NETCALL_RETRY))
//												ret = ERROR_COMM;
										ltot += n+2;
*/
									 }
									}
//									set_semaphore_dos();
									for(i=l;i<max_grp;i++)
									{
										fread(	&n, 2, 1, h);
										fread( buf, n, 1, h);
										ltot += n+2;
									}
// 									clear_semaphore_dos();
//									showgauge(gauge, coef, length);
//									set_semaphore_dos();
//                  read icon_name_table
									n=0;
									fread( &n, 2, 1, h);
									if(local)
									{
									 fread( Icon_name_table, n, 1, h);
//									 clear_semaphore_dos();
									}
									else
									{
										memset(buf,0,n);
										fread( buf, n, 1, h);
//										clear_semaphore_dos();
//										if(net_call(COMMAND_50+100, ICON_NAME_TABLE_COMMAND, (char*)buf, (unsigned int *)&n, st, networkaddress, NETCALL_RETRY))
//												ret = ERROR_COMM;
									}
									if(local)
									{
//									 mfarfree(handle);
                                     if(p)
                                      delete p;
									 upcustomunit(ptr_panel->units);
									}
//									set_semaphore_dos();
									fclose(h);
//									clear_semaphore_dos();
//									deletegauge(&gauge);

#ifndef WORKSTATION
									if(local)
									{
//									 just_load = 1;
//									 onemin = 1;
									}
#endif
									if(local)
									{
//									 initanalogmon();
									 pixvar = 1;
									}
									countdeslength();
								 }
								 else
								 {
//										set_semaphore_dos();
										fclose(h);
//										clear_semaphore_dos();
										ret = ERROR_COMM;
								 }
								 if(local)
								 {
//									 clear_semaphore(&t3000_flag);
								 }
								}
								else
								{
//									onemin = 1;
//									set_semaphore_dos();
									fclose(h);
//									clear_semaphore_dos();
									ret = ERROR_COMM;
								}
							 }
//	set_semaphore_dos();
	delete buf;
//	clear_semaphore_dos();
	if(panel_info->panel_type == MINI_T3000)
	{
	 if(panel_info->version>=230)
	 {
		if(ret==SUCCESS)
		{
//		 if( net_call(COMMAND_50, WRITEPRGFLASH_COMMAND, NULL, 0, st, networkaddress)!=SUCCESS )
//			 if( net_call(COMMAND_50, WRITEPRGFLASH_COMMAND, NULL, 0, st, networkaddress)!=SUCCESS )
			 {
				 ret = ERROR_COMM;
//				 DisplayMessage(12, 7, 68, 11, NULL, saveprgflashtxt,NULL, Black, NULL, 5000);
				 return ret;
			 }
//		 DisplayMessage(12, 7, 68, 11, NULL, rebootm,NULL, Black, NULL, 32000);
		 return ret;
		}
//		DisplayMessage(12, 7, 68, 11, NULL, saveprgflashtxt,NULL, Black, NULL, 5000);
	 }
	}
  return ret;
}

void wait(long sec)
{
 long l;
 l = timestart;
 while(timestart < l+sec);
}

void pfarfree(HANDLE handle)
{
	GlobalUnlock( handle);
 	GlobalFree( handle );
}

size_t ffread(void *ptr, size_t size, size_t n, FILE *stream)
{
//ltg  int_dos = 1;
 fread( ptr, size, n, stream);
//ltg  int_dos = 0;
}

size_t ffwrite(const void *ptr, size_t size, size_t n, FILE * stream)
{
//ltg  int_dos = 1;
 fwrite( ptr, size, n, stream);
//ltg  int_dos = 0;
}


/**********************************************************************/
/* LTG:  Aug.  13, 1997.                                              */
/*--------------------------------------------------------------------*/
/*   Check user access level.                                         */
/*--------------------------------------------------------------------*/
/* Calling:  In LTGT3000.cpp                                          */
/*--------------------------------------------------------------------*/
/* Updating: Aug.  13, 1997                                           */
/**********************************************************************/
int check_access(int ptype, int num, int mes)
{
  if( access_level==0 )
	right_access = R_READ_WRITE;
  else
	if( access_level==1 )
	  right_access = R_READ_ONLY;
	else
	{
	  right_access = (rights_access>>(ptype*2L))&0x03;
	  if(num != -1 && ptype == GRP && right_access == R_IND)
	  {
		right_access = (screen_right[num*2/8]>>((num*2)%8))&0x03;
	  }
	}

  if(  ( (mes&R_NA) && right_access == R_NA)     || 
       ( (mes&1) && right_access == R_READ_ONLY)    )
  {
    MessageBox( GetFocus()," Unauthorized access! ","Warning", 
                MB_ICONEXCLAMATION|MB_OK);
  }

  return right_access;
}

void printtime(void)
{
	 char *ptr;
	 t = (long)timesec1970 + (long)timestart;
	 ptr = ctime((const long *)&t);
	 ptr[24] = 0;
//ltg 	 mxyputs(51,2,ptr,White,Blue1);
}

void disconnect_serial(int d)
{
	  RS232Error result;
//ltg 	  if(ser_ptr)
//ltg 		if(ser_ptr->Read_mode() == MASTER )
		{
			chdir(oldpath);
//ltg 			disconnect_time=-1;
// ltg 			GWindow *D=NULL;
			if(d==1)
//ltg 			 DisplayMessage(mode_text?20:160, mode_text?7:150, mode_text?60:500, mode_text?11:210, ctrlbreak_text, "  Wait for disconnection...  ", &D);
			if( serial_access && d!=2)
			{
//ltg 			  ((class PTP *)Routing_table[ser_ptr->port_number].ptr)->NL_parameters.primitive = DL_DISCONNECT_REQUEST;
//ltg 			  resume(((class PTP *)Routing_table[ser_ptr->port_number].ptr)->task);
//ltg 			  Delay(3000);
//				result = t3000Request(50+100, 20, NULL, 0, station_num, networkaddress, 0, 20);
//				result = ser_ptr->Serial_control( 50, WRITE, 0, NULL, 20 );
			}
//ltg 			if( ser_ptr->modem_active )
			{
/* ltg
				if( ( ser_ptr->modem_status = ser_ptr->modem_obj->Disconnect() ) != MODEM_SUCCESS )
				{
				 ;
				}
*/
//ltg 				ser_ptr->connection_established = 0;
//ltg 				Delay(1000);
			}
//ltg 			ser_ptr->activity = FREE;
			if( serial_access )
			{
//ltg 				ser_ptr->FlushRXbuffer();
//ltg 				ser_ptr->FlushTXbuffer();
        setlocalpanel();
				serial_access = 0;
//ltg 				ser_ptr->Set_mode( SLAVE );
//ltg 				if(console_mode) ser_ptr->Set_mode( MASTER );
//ltg 				else
				{
				 memcpy( (char*)station_list, (char*)ptr_panel->local_stations,
																								MAX_STATIONS*sizeof( Station_point ) );
				 memcpy( (char*)&passwords, (char*)&ptr_panel->passwords,	sizeof( passwords ) );
				}
				memcpy(password, Password, sizeof(password));
				memcpy(user_name, User_name, sizeof(user_name));
			}
		 refresh_time_init = 150;
//ltg 		 ser_ptr = NULL;
//ltg 		 if(D) DeleteMessage(D);
		}
}

int local_request(int panel)
{
 if( (!local_panel && panel==Station_NUM && panel!=station_num ) || (local_panel && panel==Station_NUM) )
	return 1;
 else
	return 0;
}

/**********************************************************************/
/* LTG:  July 27, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Check user name and password.                                    */
/*--------------------------------------------------------------------*/
/*   int ex: 0 or ESC ????                                            */
/*--------------------------------------------------------------------*/
/*   Calling:  Proj() In this program.                                */
/*--------------------------------------------------------------------*/
/* Updating: July 27, 1997                                            */
/**********************************************************************/
int get_access(int ex)
{
  int i;

  TLoginForm *LoginForm;
  LoginForm = new TLoginForm(Application);
  while(1)
  {
    LoginForm->Left=
         MainScreen->Left+(MainScreen->Width-LoginForm->Width)/2;
    LoginForm->Top = MainScreen->Top + 150;

    if( LoginForm->ShowModal()!=mrOk )
    {
       delete LoginForm;
       return 0;
    }

    for(i=0;i<MAX_PASSW;i++)
      if(!strcmp(rtrim(user_name),rtrim(passwords.passwords[i].name)) &&
         !strcmp(password,passwords.passwords[i].password) &&
 	     passwords.passwords[i].password[0] )

    if(i!=MAX_PASSW) break;

	if( !user_name[0] && !strcmp(password,supervisor) ) break;

  }
  delete LoginForm;

  if( !user_name[0] && !strcmp(password,supervisor) )
  {
	 access_level = 0;
	 default_group=0;
	 rights_access=0;
  }
  else
  {
	access_level = passwords.passwords[i].access_level;
	if(ex!=ESC)    //27
		station_num = passwords.passwords[i].default_panel;
	default_group=passwords.passwords[i].default_group;
	memcpy(screen_right,  passwords.passwords[i].screen_right, sizeof(passwords.passwords[i].screen_right));
	rights_access=passwords.passwords[i].rights_access;
  }

  if(station_num == 0)
  {
#ifndef WORKSTATION
	station_num = Station_NUM;
	memcpy(station_name, station_list[station_num-1].name,NAME_SIZE);
	local_panel = 1;
#endif
	;
  }
  else
  {
	memcpy(station_name, station_list[station_num-1].name,NAME_SIZE);
	local_panel = 0;

	if(ex!=ESC)    //27
	{
	  if(station_num==Station_NUM) local_panel = 1;
	  else  local_panel = 0;
	}
  }

  return 1;
}

/**********************************************************************/
/* LTG:  Aug.  11, 1997.                                              */
/*--------------------------------------------------------------------*/
/*   Create a copy of the actual .prg file. The file name is the      */
/*   fname, the new extension is .pr_                                 */
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
/* Calling:  In wint3.cpp                                             */
/*--------------------------------------------------------------------*/
/* Updating: Aug.  11, 1997                                           */
/**********************************************************************/
void saveoldprg(char *fname)
{
  char lpszBakFileName[65];
  char *lpszTemp;

  strcpy(lpszBakFileName, fname);
  lpszTemp = strchr(lpszBakFileName, '.');
  if ( lpszTemp == NULL )
    return;
  strcpy(lpszTemp, ".pr_");
  rename(fname, lpszBakFileName);
}

/**********************************************************************/
/* LTG:  Aug.  6, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Save panel file.                                                 */
/*--------------------------------------------------------------------*/
/*   char *lpszFileName: FileName.                                    */
/*   DInt local:                                                      */
/*   (1) local = 1 :  the user is not connected to a remote panel.    */
/*       The data are saved from the local memory (user station)      */
/*      (  Save command when the user is not connected to a remote    */
/*         panel)                                                     */
/*   (2) local = 0 :  the user is  connected to a remote panel.       */
/*       The data should be first read from the remote panel and      */
/*       saved on the user station ( Get command )                    */
/*--------------------------------------------------------------------*/
/* Calling:  In wint3.cpp                                             */
/*--------------------------------------------------------------------*/
/* Updating: Aug. 15, 1997                                            */
/**********************************************************************/

void Panel::savefile(char *lpszFileName, DInt local)
{
/*
  int i, r = SUCCESS;
  int n, nitem, st, net, ver, vermini;
  DUint l;
  Str_out_point   		 *poutputs;
  Str_in_point    		 *pinputs;
  char                   *pold_outs_ins, *ptr;

  if ( local )
  {
    st  = Station_NUM;
	net = NetworkAddress;
  }
  else
  {
	st  = station_num;
	net = networkaddress;
  }

//ltg 		 int_disk1++;

  pold_outs_ins = NULL;

  //ltg: IF connected to a mini panel.  ?????
  if( panel_info1.panel_type == MINI_T3000 )
  {
    pold_outs_ins = new char [ MAX_OUTS_MINI * sizeof(Str_out_point) + 
                               MAX_INS_MINI * sizeof(Str_in_point)  ];
                               
	memset( pold_outs_ins, 0, MAX_OUTS_MINI*sizeof(Str_out_point) + 
                              MAX_INS_MINI*sizeof(Str_in_point) );

	poutputs = (Str_out_point *)pold_outs_ins;
	pinputs  = (Str_in_point *)( pold_outs_ins+MAX_OUTS_MINI *
                                 sizeof(Str_out_point) );

	for(i=0; i < MAX_OUTS_MINI; i++)
	  poutputs[i].digital_analog = 1;

	for(i=0; i < MAX_INS_MINI; i++)
	{
	  pinputs[i].filter = 6;
	  pinputs[i].digital_analog = 1;
	}

	if ((h=fopen(lpszFileName,"rb"))!=NULL)
	{
	  fread( tempbuf, 26, 1, h); //time and date
	  fread( tempbuf, 4, 1, h);  //signature

	  if ( !memcmp(tempbuf, signature, 4) )
	  {
	    fread(	&n, 2, 1, h);        //panel number
		fread(	&nitem, 2, 1, h);    //network number
		fread(	&ver, 2, 1, h);      //ver T3000
		fread(	&vermini, 2, 1, h);  //ver mini
		fread(	tempbuf, 1, 32, h);  // reserved bytes

		if ( ver >= 210 && ver!=0x2020 )
		{
		  if( n == st && vermini )
		  {
//		    fseek(h,70,SEEK_SET);  //  information - length 44 bytes
			fread( &n, 2, 1, h);
			fread( &nitem, 2, 1, h);
//			fread(pold_outs_ins,1,32*sizeof(Str_out_point),h);
			fread((char *)poutputs,n,nitem,h);
//			fseek(h,4,SEEK_CUR);
			fread( &n, 2, 1, h);
			fread( &nitem, 2, 1, h);
//			fread(pold_outs_ins+32*sizeof(Str_out_point),1,32*sizeof(Str_in_point),h);
			fread((char *)pinputs,n,nitem,h);
          }
		}
      }

	  fclose(h);
	}
  }

  if (( h = fopen(lpszFileName,"wb+") ) == NULL )
  {
    if(pold_outs_ins)
	  delete  pold_outs_ins;

    MessageBox(GetFocus(), "Error create file !", "", MB_OK );
//ltg  	int_disk1--;

	return;
  }

  char *buf = new char[7000];
  long length;

  if(local)
  {
    length = sizeof(outputs) + sizeof(inputs) + sizeof(vars) + sizeof(controllers) +
	         sizeof(analog_mon) + sizeof(arrays) + sizeof(custom_tab) + sizeof(alarms_set) +
			 sizeof(weekly_routines) + sizeof(annual_routines) + sizeof(control_groups) +
			 MAX_WR * 9 * sizeof(Wr_one_day) + MAX_AR * 46 + sizeof(units);

	for(i=0;i< MAX_GRPS;i++)
	  if(local)
	  {
		if(control_group_elements[i].ptrgrp)
		  length += control_group_elements[i].nr_elements*sizeof(Str_grp_element);
	  }

	for(i=0;i< MAX_PRGS;i++)
	  if(local)
	  {
		length  += programs[i].bytes;
	  }

  }
  else
    length = 13+tbl_bank[PRG]+tbl_bank[WR]+tbl_bank[AR]+tbl_bank[GRP];

  long coef = ((length*1000L)/20000L)*1000L +
              (( (length*1000L)%20000L )*1000L) / 20000L ;
  long ltot = 0;

//  information - length 40 bytes

//ltg  t = (long)timesec1970 + (long)timestart;


  time_t t;

  time( &t );   // must change property "time" of class "Aio"
                // to other name, ex. "scan_time"
  ptr =  ctime(&t);

  // write file head: time, number, ver, etc.
  fwrite( ptr, 26, 1, h);         //time and date
  fwrite( signature, 4, 1, h);    //signature
  fwrite(	&st, 2, 1, h);        //panel number
  fwrite(	&net, 2, 1, h);       //network number
  n = Version;
  fwrite(	&n, 2, 1, h);         //version T3000
  if( panel_info1.panel_type == T3000 )
	n=0;
  else
	n = panel_info1.version;

  fwrite(	&n, 2, 1, h);         //version mini
  memset(buf, 0, 1000);
  fwrite(	buf, 32, 1, h);       //reserved 32 bytes

  for(i=0; i<=UNIT; i++)  /// 0---13
  {
    if(i!=DMON && i!=ALARMM)
	{
	  nitem = info[i].str_size;
	  n = info[i].max_points;
	  if(local)
	  {
	    fwrite(	&n, 2, 1, h);
		fwrite(	&nitem, 2, 1, h);
		fwrite(	info[i].address, nitem, n, h);
		ltot += n*nitem;
	  }
	  else
	  {
		n = tbl_bank[i];
		ptr = buf;
		if( i==T3000_OUT )
		{
		  if( pold_outs_ins ) ptr = (char *)poutputs;
		}

		if( i==T3000_IN )
		{
		  if( pold_outs_ins ) ptr = (char *)pinputs;
		}

        // READ *** when i = ??
        // i=0 , save all outputs objects.
        // i=1 , save all inputs objects.
        // i=2 , save all variables objects.
        // read controllers
        // read weekly routines
		// read annual routines
        // 7 read programs 
        // read tables
		// read monitors
        // read screens
        // i = 11, read arrays
        
		if( (r=saveloadpoint(0, i, n, nitem, station_num, h, ptr))
            !=SUCCESS)
          break;

        ltot++;
	  }
	}
  }

  if(r==SUCCESS)
  {
    for( i=0;i<tbl_bank[PRG];i++)
	{
	  if(local)
	  {
	    n = programs[i].bytes;
		ltot += n;
		fwrite(	&n, 2, 1, h);
		fwrite( program_codes[i], n, 1, h);
	  }
	  else
	  {
        l=0;

        //ltg: read program code.
		if( ( r = t3000Request(16, i, (DByte *)buf, &l ,station_num, networkaddress) )
              != SUCCESS )
          break;

		fwrite(	&l, 2, 1, h);
		fwrite( buf, l, 1, h);

	  	ltot++;

	  }
	}
  }

  if(r==SUCCESS)
  {
	nitem = 9 * sizeof(Wr_one_day);
	n = tbl_bank[WR] * nitem;
	fwrite(	&n, 2, 1, h);

	if(local)
	{
	  fwrite( wr_times, n, 1, h);
	  ltot += n;
	}
	else
	{
	  for( i=0;i<tbl_bank[WR];i++)
	  {
        // read  weekly schedules.
	    if( (r=t3000Request(WR_TIME+1, i, (DByte *)buf, 0 ,station_num,
                            networkaddress))!=SUCCESS )
          break;

		fwrite( buf, nitem, 1, h);
		ltot++;
      }
	}
  }

  if(r==SUCCESS)
  {
	n = tbl_bank[AR] * 46;
	fwrite(	&n, 2, 1, h);

	if(local)
	{
	  fwrite( ar_dates, n, 1, h);
	  ltot += n;
	}
	else
	{
	  for( i=0;i<tbl_bank[AR];i++)
	  {
        // read  annual schedules. 
		if( (r=t3000Request(AR_Y+1, i, (DByte *)buf, 0 ,station_num,
                            networkaddress))!=SUCCESS )
          break;

		fwrite( buf, 46, 1, h);
		ltot++;
	  }
	}
  }

  if(r==SUCCESS)
  {
	Str_grp_element *buff=NULL;

	if(!local)
	  buff = new Str_grp_element[MAX_ELEM];

	n = sizeof(Str_grp_element);
	fwrite(	&n, 2, 1, h);

	for( i=0;i<tbl_bank[GRP];i++)
	{
	  if(local)
	  {
	    if(control_group_elements[i].ptrgrp)
		  n = control_group_elements[i].nr_elements*
              sizeof(Str_grp_element);
		else
		  n = 0;

		ltot += n;
		fwrite(	&n, 2, 1, h);
		fwrite( control_group_elements[i].ptrgrp, n, 1, h);
	  }
	  else
	  {
        l=0;
        
        // read group elements
		if( (r=t3000Request(19, i, (DByte *)buff, &l, station_num,
                             networkaddress))!=SUCCESS)
          break;

		fwrite(	&l, 2, 1, h);
		fwrite( buff, l, 1, h);
		ltot++;
	  }
	}

	if(!local && buff)
	  delete [] buff;
  }

  // write icon name table.
  n = sizeof(Icon_name_table);
  fwrite(	&n, 2, 1, h);
  fwrite( Icon_name_table, n, 1, h);

  fclose(h);
  delete [] buf;

  if ( r != SUCCESS )
  {
    RestoreOldPrg(lpszFileName);     // In LTGT3000.cpp.
    
    MessageBeep( MB_ICONEXCLAMATION );
    MessageBox(GetFocus(),"Fail to Save Fiel !", "", MB_OK );
  }
  else
  {
    MessageBeep( MB_ICONEXCLAMATION );
    StatusBarMessage(SBText, "Saving end" );
    
    if( local )
      save_prg_flag = 0;
  }    

//ltg 		 int_disk1--;

  return;
*/
 DInt i;
 DInt r=SUCCESS;
 DInt n,nitem,st,net, ver, vermini;
 DUint l;
 Str_out_point   		 *poutputs;
 Str_in_point    		 *pinputs;
 char *pold_outs_ins, *ptr;

 if(local)
 {
 	 st = Station_NUM;
 	 net = NetworkAddress;
 }
 else
 {
 	 st = station_num;
 	 net = networkaddress;
 }
 pold_outs_ins = NULL;

 if( panel_info1.panel_type == MINI_T3000 )
 {
	 pold_outs_ins = new char [MAX_OUTS_MINI*SIZE_STR_OUT_T3000+MAX_INS_MINI*SIZE_STR_IN_T3000];
	 memset(pold_outs_ins,0,MAX_OUTS_MINI*SIZE_STR_OUT_T3000+MAX_INS_MINI*SIZE_STR_IN_T3000);
	 poutputs = (Str_out_point *)pold_outs_ins;
	 pinputs  = (Str_in_point *)(pold_outs_ins+MAX_OUTS_MINI*SIZE_STR_OUT_T3000);
	 for(i=0; i < MAX_OUTS_MINI; i++)
		poutputs[i].digital_analog = 1;
	 for(i=0; i < MAX_INS_MINI; i++)
	 {
		pinputs[i].filter = 6;
		pinputs[i].digital_analog = 1;
	 }
	 if ((h=fopen(lpszFileName,"rb"))!=NULL)
	 {
		  fread( tempbuf, 26, 1, h); //time and date
		  fread( tempbuf, 4, 1, h);  //signature
		  if ( !memcmp(tempbuf, signature, 4) )
		  {
				fread(	&n, 2, 1, h);   //panel number
				fread(	&nitem, 2, 1, h);   //network number
				fread(	&ver, 2, 1, h);       //ver T3000
				fread(	&vermini, 2, 1, h);   //ver mini
				fread(	tempbuf, 1, 32, h); // reserved bytes
				if ( ver >= 210 && ver!=0x2020 )
				{
					 if( n == st && vermini )
					 {
//									  fseek(h,70,SEEK_SET);  //  information - length 44 bytes
									  fread( &n, 2, 1, h);
									  fread( &nitem, 2, 1, h);
//								  fread(pold_outs_ins,1,32*sizeof(Str_out_point),h);
									  fread((char *)poutputs,n,nitem,h);
//								  fseek(h,4,SEEK_CUR);
									  fread( &n, 2, 1, h);
									  fread( &nitem, 2, 1, h);
//								  fread(pold_outs_ins+32*sizeof(Str_out_point),1,32*sizeof(Str_in_point),h);
									  fread((char *)pinputs,n,nitem,h);
                     }
				}
          }
		   fclose(h);
     }
 }
 if ((h=fopen(lpszFileName,"wb+"))==NULL)
 {
//									 strcpy(buf, "Error !!!. File not saved                  ");
									 if(pold_outs_ins)
										delete  pold_outs_ins;
//									 DisplayMessageT(20, 7, 60, 11, NULL, " Error create file ! ", NULL, Blue, NULL, 3000);
									 return;
 }
									char *buf=new char[7000];
//									creategauge(&gauge,"Saving file");
									long length;
									if(local)
									{
									 length = tbl_bank[T3000_OUT]*SIZE_STR_OUT_T3000 + tbl_bank[T3000_IN]*SIZE_STR_IN_T3000 + tbl_bank[VAR]*SIZE_STR_VAR_T3000 +
                                              tbl_bank[T3000_IN]*SIZE_STR_CON_T3000 + tbl_bank[AMON]*SIZE_STR_AMON_T3000 + tbl_bank[AY]*SIZE_STR_AY_T3000 +
                                              tbl_bank[TBL]*SIZE_STR_TBL_T3000 + sizeof(alarms_set) + tbl_bank[WR]*SIZE_STR_WR_T3000 + tbl_bank[AR]*SIZE_STR_AR_T3000 +
                                              tbl_bank[GRP]*SIZE_STR_SCREEN_T3000 + MAX_WR * 9 * sizeof(Wr_one_day) + MAX_AR * 46 + sizeof(units);
									 for(i=0;i< MAX_GRPS;i++)
										if(local)
										{
										 if(control_group_elements[i].ptrgrp)
											length += control_group_elements[i].nr_elements*SIZE_STR_SCREEN_ELEM_T3000;
										}
									 for(i=0;i< MAX_PRGS;i++)
										if(local)
										{
										 length  += programs[i].bytes;
										}
									}
									else
										length = 13+tbl_bank[PRG]+tbl_bank[WR]+tbl_bank[AR]+tbl_bank[GRP];
//									float coef = (float)length/20.;
									long coef = ((length*1000L)/20000L)*1000L + (((length*1000L)%20000L)*1000L)/20000L ;
									long ltot = 0;

//  information - length 40 bytes
									t = (long)timesec1970 + (long)timestart;
									ptr = ctime((const long *)&t);
									fwrite( ptr, 26, 1, h);         //time and date
									fwrite( signature, 4, 1, h);    //signature
									fwrite(	&st, 2, 1, h);          //panel number
									fwrite(	&net, 2, 1, h);      //network number
									n = Version;
									fwrite(	&n, 2, 1, h);            //version T3000
									if( panel_info1.panel_type == T3000 )
									  n=0;
									else
									  n = panel_info1.version;
									fwrite(	&n, 2, 1, h);            //version mini
									memset(buf,0,1000);
									fwrite(	buf, 32, 1, h);      //reserved 32 bytes

									for(i=0; i<=UNIT; i++)
									{
									 if(i!=DMON && i!=ALARMM)
									 {
										nitem = info[i].str_size;
										n = info[i].max_points;
										if(local)
										{
											fwrite(	&n, 2, 1, h);
											fwrite(	&nitem, 2, 1, h);
                                            align(0, i, buf, 1, tbl_bank[i]);
											fwrite(	buf, nitem, n, h);
											ltot += n*nitem;
										}
										else
										{
											n = tbl_bank[i];
											ptr = buf;
											if( i==T3000_OUT )
											{
											 if( pold_outs_ins ) ptr = (char *)poutputs;
											}
											if( i==T3000_IN )
											{
											 if( pold_outs_ins ) ptr = (char *)pinputs;
											}
											if( (r=saveloadpoint(0, i, n, nitem, station_num, h, ptr))!=SUCCESS) break;
											ltot++;
										}
//										showgauge(gauge, coef, ltot);
									 }
									}

								  if(r==SUCCESS)
								  {
//									for( i=0;i<MAX_PRGS;i++)
									for( i=0;i<tbl_bank[PRG];i++)
									{
									 if(local)
									 {
										n = programs[i].bytes;
										ltot += n;
										fwrite(	&n, 2, 1, h);
										fwrite( program_codes[i], n, 1, h);
									 }
									 else
									 {
                              l=0;
										if( (r=t3000Request(16, i, buf, &l ,station_num, networkaddress))!=SUCCESS ) break;
										fwrite(	&l, 2, 1, h);
										fwrite( buf, l, 1, h);
//										Delay(100);
										ltot++;
									 }
									}
//									showgauge(gauge, coef, ltot);
								  }
								  if(r==SUCCESS)
								  {
									nitem = 9 * SIZE_STR_WR_ONE_DAY_T3000;
//									n = MAX_WR * nitem;
									n = tbl_bank[WR] * nitem;
									fwrite(	&n, 2, 1, h);
									if(local)
									{
									 fwrite( wr_times, n, 1, h);
									 ltot += n;
									}
									else
									{
//									 char block[9 * sizeof(Wr_one_day)];
//									 for(i=0;i< MAX_WR;i++)
									 for( i=0;i<tbl_bank[WR];i++)
									 {
										if( (r=t3000Request(WR_TIME+1, i, buf, 0 ,station_num, networkaddress))!=SUCCESS ) break;
//										Delay(100);
										fwrite( buf, nitem, 1, h);
										ltot++;
									 }
									}
//									showgauge(gauge, coef, ltot);
								  }
								  if(r==SUCCESS)
								  {
//									n = MAX_AR * 46;
									n = tbl_bank[AR] * 46;
									fwrite(	&n, 2, 1, h);
									if(local)
									{
									 fwrite( ar_dates, n, 1, h);
									 ltot += n;
									}
									else
									{
//									 char block[46];
//									 for(i=0;i< MAX_AR;i++)
									 for( i=0;i<tbl_bank[AR];i++)
									 {
										if( (r=t3000Request(AR_Y+1, i, buf, 0 ,station_num, networkaddress))!=SUCCESS ) break;
//										Delay(100);
										fwrite( buf, 46, 1, h);
										ltot++;
									 }
									}
//									showgauge(gauge, coef, ltot);
								  }
								  if(r==SUCCESS)
								  {
									Str_grp_element *buff=NULL;
									if(!local)
										buff = new Str_grp_element[MAX_ELEM];
									n = SIZE_STR_SCREEN_T3000;
									fwrite(	&n, 2, 1, h);
//									for(i=0;i< MAX_GRPS;i++)
									for( i=0;i<tbl_bank[GRP];i++)
									{
									 if(local)
									 {
										if(control_group_elements[i].ptrgrp)
											n = control_group_elements[i].nr_elements*SIZE_STR_SCREEN_T3000;
										else
											n = 0;
										ltot += n;
										fwrite(	&n, 2, 1, h);
										fwrite( control_group_elements[i].ptrgrp, n, 1, h);
									 }
									 else
									 {
/*
                                        l=0;
										if( (r=t3000Request(19, i, (char *)buff, &l, station_num, networkaddress))!=SUCCESS) break;
//										Delay(100);
										fwrite(	&l, 2, 1, h);
										fwrite( buff, l, 1, h);
										ltot++;
*/
									 }
									}
//								showgauge(gauge, coef, length);
								if(!local && buff)
									delete [] buff;
								}

								n = sizeof(Icon_name_table);
								fwrite(	&n, 2, 1, h);
								fwrite( Icon_name_table, n, 1, h);

								 fclose(h);
//								 deletegauge(&gauge);
								 delete [] buf;
//								 if(local)
								 if( local )
								  save_prg_flag = 0;
//							 w.GWPuts(4,5,"          <<  OK  >>                        ");
//								 qp=2;
}

/**********************************************************************/
/* LTG:  Aug.  11, 1997.                                              */
/*--------------------------------------------------------------------*/
/*    char *fname: *.prg.                                             */
/*    Restore *.prg from *.pr_.                                       */
/*   Create a copy of the actual .pr_ file. The file name is the      */
/*   fname, the new extension is .prg                                 */
/*--------------------------------------------------------------------*/
/*--------------------------------------------------------------------*/
/* Calling:  In wint3.cpp                                             */
/*--------------------------------------------------------------------*/
/* Updating: Aug.  11, 1997                                           */
/**********************************************************************/
void RestoreOldPrg(char *fname)
{                                    
  char lpszBakFileName[100];
  char *lpszTemp;

  strcpy(lpszBakFileName, fname);

  lpszTemp = strchr(lpszBakFileName, '.');
  if ( lpszTemp == NULL )
    return;

  strcpy(lpszTemp, ".pr_");

  // copy file "fname" to "lpszBakFileName".
  {
    /*
    short int NormalFile, BakFile;

    NormalFile = open(fname, O_RDWR, S_IREAD | S_IWRITE);
    BakFile  = open(lpszBakFileName, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
    dup2( NormalFile, BakFile );

    close(BakFile);
    close(NormalFile);
    */

    FILE * NormalFile, * BakFile;
 //   char ch;
    char lpszBuf[1024000];
    long i = 0;
    long lFileLen;

    NormalFile = fopen(fname, "w");
    BakFile  = fopen(lpszBakFileName, "r");

    lFileLen = filelength( fileno(BakFile) );    //  + 1;
    fseek(NormalFile, 0, SEEK_SET);
    fseek(BakFile, 0, SEEK_SET); 
    
    while ( ( i = i+100 ) <= lFileLen )
    {
      fread(lpszBuf, 100, 1, BakFile );
      fwrite(lpszBuf, 100, 1, NormalFile );
      fseek(NormalFile, i, SEEK_SET);
      fseek(BakFile, i, SEEK_SET); 
    }  

    if ( i != lFileLen )
    {
      fread(lpszBuf, lFileLen - i + 100, 1, BakFile );
      fwrite(lpszBuf, lFileLen - i + 100, 1, NormalFile );
    }


    /*
    while ( fscanf(BakFile, "%c", &ch ) != EOF )
    {
      fprintf(NormalFile, "%c", ch );
      fseek(NormalFile, i, SEEK_SET);
      fseek(BakFile, i, SEEK_SET);
      i++;
    }  

    // fprintf(BakFile, "%c", EOF );  
    */

    fclose(NormalFile);
    fclose(BakFile);
  }

  return;
}

/**********************************************************************/
/* LTG:  July 21, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   load panel file.                                                 */
/*--------------------------------------------------------------------*/
/*   char *file: FileName.                                            */
/*--------------------------------------------------------------------*/
/* Calling:  In wint3.cpp                                             */
/*--------------------------------------------------------------------*/
/* Updating: Aug. 15, 1997                                            */
/**********************************************************************/
/*
DInt Panel::loadprg(char *file)
{
	DByte *buf;
	DLong length, coef;
	DInt k,m,l1,c1,nitem, ver, vermini;
	DUint n,j, l;
    int i;
	Str_program_point *pprg;
	buf=new DByte[7000];   //7000

							 if ((h=fopen(file,"rb+"))==NULL)
							 {
//								 if(contr)
//								 DisplayMessageT(20, 7, 60, 11, NULL, " File not loaded! ",NULL, Black, NULL, 1000);
//								 onemin = 1;
//								 resume(VIRTUAL);
							 }
							 else
							 {
								 m = 1;
								 fread( buf, 26, 1, h); //time and date
								 fread( buf, 4, 1, h);  //signature
								 if ( !memcmp(buf, signature, 4) )
								 {
									fread(	&n, 2, 1, h);   //panel number
								  fread(	&m, 2, 1, h);   //network number
								  fread(	&ver, 2, 1, h);       //ver T3000
								  fread(	&vermini, 2, 1, h);   //ver mini
								  fread(	buf, 1, 32, h); // reserved bytes
								  if ( ver < 210 || ver==0x2020 )
								  {
//									DisplayMessageT(10, 7, 70, 11, NULL, "File not loaded. File version less then 2.10!",NULL, Black, NULL, 3500);
									m=0;
									}
								 }
								 else
								 {
									m=0;
//									DisplayMessageT(10, 7, 70, 11, NULL, "File not loaded. File corrupt.",NULL, Black, NULL, 3500);
								 }

								 if( n != station_num && m )
								 {
										strcpy((char *)buf, "File for panel ");
										itoa(n, (char *)&buf[strlen(buf)], 10);
										strcat((char *)buf, ". Load as panel ");
//										itoa(station_num, &buf[strlen(buf)], 10);
										itoa(n, (char *)&buf[strlen(buf)], 10);
										strcat((char *)buf, " ? (Y/N) : ");

									    UserEnsureForm->Left=MainScreen->Left+(MainScreen->Width-UserEnsureForm->Width)/2;
									    UserEnsureForm->Top = MainScreen->Top + 90;
									    UserEnsureForm->EnsureWinLabel->Caption = AnsiString( (char *)buf );
									    UserEnsureForm->ShowModal();

				      // if user want to load this file
									    if ( UserEnsureForm->Ok )
                                        {
											if(!control)
											{
											 station_list[Station_NUM-1].state=0;
											 station_num = Station_NUM = n;
											 strcpy((char *)station_name, (char *)station_list[station_num-1].name);
											 strcpy((char *)Station_NAME, (char *)station_name);
											 station_list[station_num-1].state=1;
											 ptr_panel->GlPanel=station_num;
											}
										}
										else
											m = 0;
								 }
								 if (m)
								 {
#ifndef WORKSTATION
//									if(local_panel)
//										suspend(VIRTUAL);
#endif
									m=ERROR_COMM;
									if(local_panel)
									{
										clearpanel();
										m=SUCCESS;
									}
									else
									{
										if( (m=t3000Request((DInt)CLEARPANEL_T3000, 0, buf, 0 ,station_num, networkaddress))!=SUCCESS )
											m=t3000Request((DInt)CLEARPANEL_T3000, 0, buf, 0 ,station_num, networkaddress);
										if( m==SUCCESS)
										{
//											 DisplayMessageT(20, 7, 60, 11, NULL, " Wait for panel initialization... ", NULL, Blue, NULL, 5000);
											 l = sizeof(tbl_bank);
											 m=t3000Request((DInt)COMMAND_50, TABLEPOINTS_COMMAND, (DByte *)tbl_bank, &l, station_num, networkaddress);
										}
									}
									if(m==SUCCESS)
									{

									coef = ftell(h);    // save file pointer
									fseek(h, 0, SEEK_END);
									length = ftell(h);
									fseek(h, coef, SEEK_SET);   // restore file pointer
									coef = ((length*1000L)/20000L)*1000L + (((length*1000L)%20000L)*1000L)/20000L ;
//									float coef = (float)length/20.;
									long ltot = 0;
//									creategauge(&gauge,"Loading file");
									if(local_panel)
									{
									 present_analog_monitor=0;
									 programs_size = 0;
									 first_free = 0;
									 for(i=0; i < MAX_HEAP_DIGM-1; i++)
										 heap_dmon[i].next = i+1;
									 nsample_mem_dig = 0;
									}

									int max_prg, max_grp;
									for(i=T3000_OUT; i<=UNIT; i++)
									{
									 if(i==ALARMM)
										if( ver < 216 )
										{
										 fread( &n, 2, 1, h);
										 fread( &nitem, 2, 1, h);
										 fread(	buf, n, nitem ,h);
                     continue;
										}
									 if(i!=DMON && i!=ALARMM)
									 {
										fread( &n, 2, 1, h);
										fread( &nitem, 2, 1, h);
										l = min(n,info[i].max_points);
										if(i==GRP)
											max_grp = n;
										if(i==PRG)
										{
											max_prg = n;
										}
										if(local_panel)
										{
										 if(nitem == info[i].str_size)
										 {
											 fread(	info[i].address, nitem, l ,h);
										 }
										 else
										 {
											 m = min(nitem, info[i].str_size);
											 for(j=0; j<l; j++)
											 {
												 fread(	info[i].address+j*info[i].str_size, m, 1 ,h);
												 if(nitem > info[i].str_size)
														fseek(h, nitem-info[i].str_size, SEEK_CUR);
											 }
										 }
										 if(n > l)
											fseek(h, (n-l)*nitem, SEEK_CUR);
										}
										else
										{
//										 memset(buf,0,7000);
										 fread(	buf, n, nitem ,h);
										 l = min(n,tbl_bank[i]);
										 if(i==PRG)
										 {
											pprg = (Str_program_point *)buf;
											for(j=0;j<l;j++,pprg++)
											{
											 pprg->bytes = 0;
											}
										 }
										 if( i!=ALARMM )
											 saveloadpoint(100,i,l,nitem,station_num,h,buf);
//										 if(n > l)
//											fseek(h, (n-l)*nitem, SEEK_CUR);
										}
										ltot += n*nitem+2;
//										showgauge(gauge, coef, ltot);
									 }
									 if(local_panel)
									 {
										if(i==PRG)
											for(j=0; j < info[i].max_points; j++)
												programs[j].errcode = 0;
										if(i==AMON)
										{
											for(j=0;j < info[i].max_points; j++)
											 for(k=0;k<5;k++)
												monitor_work_data[j].start_index_dig[k] = 0x0FFFF;
											for(j=0;j<info[i].max_points; j++)
											{
											 analog_mon[j].double_flag=1;
											}
											for(j=0;j<info[i].max_points; j++)
												for(k=0;k<analog_mon[j].num_inputs;k++)
												{
												if(analog_mon[j].inputs[k].panel+1!=Station_NUM)
												{
													updateremotepoint(analog_mon[j].inputs[k]);
												}
												}
										}
										if(i==AY)
										{
										 long *p = heap_array;
										 for(j=0; j<info[i].max_points; j++)
										 {
											if(arrays[j].length)
											{
												arrays_data[j] =  p;
												p += arrays[j].length;
											}
											else
											 arrays_data[j] =  0;
										 }
										}
									 }
									}

									char *p,*q;
								  int n1;
								  if(local_panel)
								  {
									ind_heap_buf = 0;
									memset(heap_buf, 0, MAX_HEAP_BUF);
//									mfarmalloc(&p, PROGRAM_SIZE, handle);
                                    p = new char[PROGRAM_SIZE];
									if (p==NULL)
										 {
//										 mxyputs(10,10,"Error alloc");
										 abort();
										 }
//									for(int i=0;i< MAX_PRGS;i++)
//										 Heap_dealloc(&program_codes[i]);
									}

// load programs code
//									l=min(MAX_PRGS,max_prg);
									l = min(max_prg,tbl_bank[PRG]);
									for(i=0;i< l;i++)
									{
									if(local_panel)
									{
									 fread(	&n, 2, 1, h);
									 fread( p, n, 1, h);
									 if(n)
									 {
										q = p;
									  memcpy(&n1, q, 2);
									  q += n1 + 2 + 3;
									  memcpy(&n1, q, 2);
									  q += 2;              //local variables
									  for(j=0;j<n1; )
									  {
										  switch(q[j]){
												case FLOAT_TYPE:
												case LONG_TYPE:
													k = 4;
													break;
												case INTEGER_TYPE:
													k = 2;
													break;
												case BYTE_TYPE:
													k = 1;
													break;
												default:
													{
													switch(q[j]){
														case FLOAT_TYPE_ARRAY:
														case LONG_TYPE_ARRAY:
															k = 4;
															break;
														case INTEGER_TYPE_ARRAY:
															k = 2;
															break;
														case BYTE_TYPE_ARRAY:
														case STRING_TYPE:
//														case STRING_TYPE_ARRAY:
															k = 1;
															break;
													 }
													 memcpy(&l1, &q[j+1], 2);
													 memcpy(&c1, &q[j+3], 2);
													 if(l1)
														k *= l1*c1;
													 else
													  k *= c1;
													 j += 4;
													}
													break;
										  }
										  j++;
										  memset(&q[j], 0, k);
										  j += k;
										  j += 1+strlen(&q[j]);
									  }
										q += n1;
										memcpy(&n1, q, 2);   //time
										q += 2+n1;
										memcpy(&n1,q,2);    //ind_remote_local_list
										q += 2;         //remote_local_list
										updateremotepoints(n1, (struct remote_local_list *)q);
									 }
									 if ((program_codes[i]=Heap_alloc(n))!=NULL)
									 {
//										movedata(FP_SEG(p),FP_OFF(p) ,FP_SEG(program_codes[i]),FP_OFF(program_codes[i]), n);
										memmove(&program_codes[i],p, n);
									 }
										programs[i].bytes=n;
										programs_size += n;
									 ltot += n+2;
//									 showgauge(gauge, coef, ltot);
									}
									else
									{
										fread(	&n, 2, 1, h);
										fread( buf, n, 1, h);
										ltot += n+2;
										if(!n)
										{
										 n=11;
										 memset(buf,0,n);
										 buf[2]=0xfe;
															}
										t3000Request((DInt)(16+100), (DInt)i, buf, &n ,station_num, networkaddress);
									}

									}
									for(i=l;i<max_prg;i++)
									{
										fread(	&n, 2, 1, h);
										fread( buf, n, 1, h);
										ltot += n+2;
									}
									if(local_panel)
									if(max_prg > MAX_PRGS)
									 for(i=0;i< max_prg-MAX_PRGS;i++)
									 {
										fread(	&n, 2, 1, h);
										fread( p, n, 1, h);
										ltot += n+2;
									 }
									if(local_panel)
//									 mfarfree(handle);
                                       delete p;

//									showgauge(gauge, coef, ltot);

									m = MAX_WR * 9 * sizeof(Wr_one_day);
									fread(	&n, 2, 1, h);
									l = min(n,m);
									if(local_panel)
									{
										fread( wr_times, l, 1, h);
										if(n > m)
										 fseek(h, n-m, SEEK_CUR);
									}
									else
									{
										fread( buf, n, 1, h);
										j = 9*sizeof(Wr_one_day);
										l = min(n/j, tbl_bank[WR]);
										for(i=0;i<l;i++)
										{
										 t3000Request((DInt)(WR_TIME+1+100), (DInt)i, (char *)&buf[i*j], &j,station_num, networkaddress);
										}
									}
									ltot += n+2;
//									showgauge(gauge, coef, ltot);

									m = MAX_AR * 46;
									fread(	&n, 2, 1, h);
									l = min(n,m);
									if(local_panel)
									{
									 fread( ar_dates, l, 1, h);
									 if(n > m)
										fseek(h, n-m, SEEK_CUR);
									}
									else
									{
										fread( buf, n, 1, h);
										l = min(n/46, tbl_bank[AR]);
										for(i=0;i<l;i++)
										{
										 t3000Request((DInt)(AR_Y+1+100), (DInt)i, &buf[i*46], &l,station_num, networkaddress);
										}
									}
									ltot += n+2;
//									showgauge(gauge, coef, ltot);


									if(local_panel)
									{
									ind_heap_grp=0;
									memset(heap_grp, 0, MAX_HEAP_GRP);
//									mfarmalloc(&p, MAX_ELEM*sizeof(Str_grp_element), handle);
                                    p = new DByte[MAX_ELEM*sizeof(Str_grp_element)];
									if (p==NULL)
									{
//										 mxyputs(10,10,"Error alloc");
										 abort();
									}
//									for(i=0;i< MAX_GRPS;i++)
//										 Heap_grp_dealloc((char huge **)&control_group_elements[i].ptrgrp,control_group_elements[i].nr_elements*sizeof(Str_grp_element));
									}
													Str_grp_element *pgrp;
									nitem = sizeof(Str_grp_element);
									fread(	&nitem, 2, 1, h);

//								  l=min(MAX_GRPS,max_grp);
									l=min(tbl_bank[GRP],max_grp);
									for(i=0;i< l;i++)
									{
									if(local_panel)
									{
									 fread(	&n, 2, 1, h);
									 fread( p, n, 1, h);
									 pgrp = (Str_grp_element *)p;
									 for(j=0; j<(DUint)(n/sizeof(Str_grp_element)); j++, pgrp++ )
									 {
										 pgrp->point_absent = 0;
										 if(pgrp->point_info.point.point_type==OUT+1)
											if(pgrp->point_info.point.number+1>tbl_bank[T3000_OUT])
												pgrp->point_absent = 1;
										 if(pgrp->point_info.point.point_type==IN+1)
											if(pgrp->point_info.point.number+1>tbl_bank[T3000_IN])
												pgrp->point_absent = 1;
									 }
									 ltot += n+2;
									 if ((control_group_elements[i].ptrgrp=(Str_grp_element *)Heap_grp_alloc(n))!=NULL)
//											movedata(FP_SEG(p),FP_OFF(p) ,FP_SEG(control_group_elements[i].ptrgrp),FP_OFF(control_group_elements[i].ptrgrp), n);
    									 memmove(control_group_elements[i].ptrgrp,p, n);
									 control_group_elements[i].nr_elements=(DInt)(n/sizeof(Str_grp_element));
//									 grp_updateremotepoint(control_group_elements[i]);
									}
									else
									{
									 fread(	&n, 2, 1, h);
									 fread( buf, n, 1, h);
									 pgrp = (Str_grp_element *)buf;
									 for(j=0; j<(DUint)(n/sizeof(Str_grp_element)); j++, pgrp++ )
									 {
										 pgrp->point_absent = 0;
										 if(pgrp->point_info.point.point_type==OUT+1)
											if(pgrp->point_info.point.number+1>tbl_bank[T3000_OUT])
												pgrp->point_absent = 1;
										 if(pgrp->point_info.point.point_type==IN+1)
											if(pgrp->point_info.point.number+1>tbl_bank[T3000_IN])
												pgrp->point_absent = 1;
									 }
									 t3000Request((DInt)(19+100), (DInt)i, (DByte *)buf, (DUint*)&n, station_num, networkaddress);
									 ltot += n+2;
									}
									}
									for(i=l;i<max_grp;i++)
									{
										fread(	&n, 2, 1, h);
										fread( buf, n, 1, h);
										ltot += n+2;
									}
									if(local_panel)
									if(max_grp > MAX_GRPS)
									 for(int i=0;i< max_grp-MAX_GRPS;i++)
									 {
										fread(	&n, 2, 1, h);
										fread( p, n, 1, h);
									  ltot += n+2;
									 }
//								  showgauge(gauge, coef, length);

//                  read icon_name_table
								  fread( &n, 2, 1, h);
								  fread( Icon_name_table, n, 1, h);

									if(local_panel)
								  {
//   								mfarfree(handle);
                                    delete p;
//									upcustomunit(ptr_panel->units);
								  }
									fclose(h);
//									deletegauge(&gauge);

#ifndef WORKSTATION
								  if(local_panel)
								  {
								  }
#endif
								  if(local_panel)
								  {
//									initanalogmon();
									pixvar = 1;
								  }
//									countdeslength();
									}
									else
									{
										fclose(h);
									}
								 }
								 else
								 {
								 }
							 }
	delete buf;
    return 1;
}
*/
//ltg: end of function: int Panel::loadprg(char *file)
/**********************************************************************/
/* LTG:  July 27, 1997.                                               */
/*--------------------------------------------------------------------*/
/*   Del all the blank in the end of string.                          */
/*--------------------------------------------------------------------*/
/* Calling:   In  T3000.cpp
/*--------------------------------------------------------------------*/
/* Updating: July 27, 1997                                            */
/**********************************************************************/
char *rtrim(char *text)
{
  int n,i;

  n=strlen(text);

  for (i=n-1;i>=0;i--)
	if (text[i]!=' ')
		break;

  text[i+1]='\0';

  return text;
}

