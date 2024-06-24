#ifndef _AIO_H
#define _AIO_H
#include "baseclas.h"

#define MAX_BOARDS	4

#define BOARD_PRESENT	0

typedef struct {
		DUint		base_adr;
		DByte		board_number;
		DLong		quartz_freq;
		DUint		sample_time;
		DUint 		watchdog_interval;
		DByte		no_outs;
		DByte		no_ins;
		DByte		in_mask;
		DByte		out_mask;
		DByte		status;
		DUint     watchdog_flag	:1;
		DUint     unused		:7;
		} 			Board_info;

typedef struct {
		DByte		status;
		DByte		in_mask;
		DByte		out_mask;
		} 	Aux_board_info;

typedef enum { SET_POINT, GET_POINT, SET_ALL, GET_ALL, SET_RATE, DESCRIPTION,
	GET_STATUS, TEST_INTERFACE, RESET_PC, RESET_CARD, WATCHDOG_ON, WATCHDOG_OFF,
	SET_WATCHDOG, GET_NO_POINTS, GET_ALL_POINTS, POWER_TOGGLE, VERSION = 0x0A0,
	SPECIAL = 0xE0 } Order;

typedef enum { AIO_OK, AIO_BLOCKED, AIO_ERROR, FUNCTION_NOT_AVAILABLE } Aio_Error;

typedef enum { VER0_CARD, VER1_CARD, VER2_CARD } Card_Type;

typedef struct {
	DInt		filter_last;
	DLong	filter_sum;
	DUint	average;
	DUint	last;
	DUint 	ticks;
	}			In_aux;

typedef struct {
	DLong	old_err;
	DLong 	error_area;
	DLong 	oi;
	}	Con_aux;

typedef struct {
	char *address;
	DInt str_size;
	DInt max_points;
	char *name;
	} Info_Table;


class Aio
{
public:
	DByte                               GlPanel, GlPanel_type;
	DInt                                GlNetwork;
	Card_Type						    card_type;
	DByte 								number_of_boards;
	DByte								panel_number;
	DByte 								user_number;
	DInt								scan_time;  // scan interval in seconds
	char 								system_name[22];
//	char 								master_password[20];
	DByte								table_bank[MAX_TBL_BANK];
	DByte								lock[MAX_INFO_TYPE];
	In_aux							    in_aux[MAX_INS];
	Con_aux							    con_aux[MAX_CONS];
	Info_Table					        info[18];
	Str_out_point   				    outputs[MAX_OUTS];
	Str_in_point    				    inputs[MAX_INS];
	Str_variable_point			        vars[MAX_VARS];
	Str_controller_point 		        controllers[MAX_CONS];
	Str_monitor_point				    analog_mon[MAX_ANALM];
	Str_monitor_work_data               monitor_work_data[MAX_ANALM];
	Str_weekly_routine_point  	        weekly_routines[MAX_WR];
	Wr_one_day                          wr_times[MAX_WR][9];
	Str_annual_routine_point	        annual_routines[MAX_AR];
	DByte                               ar_dates[MAX_AR][46];
	Str_program_point	 			    programs[MAX_PRGS];
	DByte 			     				*program_codes[MAX_PRGS];   //  pointer to code
	Control_group_point	 		        control_groups[MAX_GRPS];
	Control_group_elements              control_group_elements[MAX_GRPS];
	Station_point					    local_stations[32];
	Password_struct                     passwords;
	Alarm_point 				        alarms[MAX_ALARMS];
	DInt                                ind_alarms;
	Alarm_set_point 		            alarms_set[MAX_ALARMS_SET];
	DInt                                ind_alarms_set;
	Str_array_point 				    arrays[MAX_ARRAYS];
	DLong 								*arrays_data[MAX_ARRAYS];
	char 		            		    *program_code;
	DUint              		            code_length;
	Units_element					    units[MAX_UNITS];
	DInt						        in_data[MAX_OUTS];
	DInt						        out_data[MAX_INS];
//	char                       Icon_name_table[MAX_ICON_NAME_TABLE][14];
    Dial_list_point                     system_list[64];

public:
	void clearpanel(void);
	Aio( DByte nr_boards);
	~Aio();
//	Aio_Error Aio_Control( Order comm, DByte *data_buffer, DByte p_number );
//	void Aio::Config_dacqd_card( void );
//	void	Controller( DByte p_number );
//	void convert_in();
//	void convert_out();
//	DLong convert_in_point( DByte, DByte );
private:
//	DLong calibration( DByte point );
//	DLong test_match( Analog_input_range_equate range, DUint raw );
//	DLong test_match_custom( Analog_input_range_equate  range, DLong raw );
//	DUint filter( DUint sample, DByte point );
	void init_info_table( void );
};

class GWDialog;

class Panel:public Aio
{
public:
			char Program_Path[64]   ;
			char Descriptor_Path[64];
			char Basic_Path[64]     ;
			char Monitor_Path[64]   ;
			char Graphics_Path[64]  ;
			char DataBase_Path[64]  ;
			char Default_Program[64];
			DInt qt;
	 public:


			Panel();
			Panel(Panel &copy);
			void operator=(const Panel &copy);

			DByte	point_type;
			void select_panel(DInt);
			void select_subpanel();
			void subselect(GWDialog *D);
			void Select(DInt man);
			DInt  connect_panel_proc(DInt t=0);
			void Path();
			void go_graphic(void);
			void go_text(void);
			void showpoints(DInt man, DInt arg=0);
			DInt loadprg(char *file);
			void file(void);
      void savefile(char *,DInt local=1);

			friend void file_path_o1(GWDialog *D);
			friend void file_path_o2(GWDialog *D);
			friend void file_path_o3(GWDialog *D);
			friend void file_path_o4(GWDialog *D);
			friend void file_path_o5(GWDialog *D);
			friend void pselect(GWDialog *D);

};
#endif