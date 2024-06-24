#ifndef _BASECLAS_H
#define _BASECLAS_H

#include "recdef.h"
//#include "color.hpp"
//#include "tif.hpp"
//#include "tifcodec.hpp"
//#include "color.hpp"
#include "t3000def.h"

//extern void *newalloc(DLong size);
//extern void newdelete(void *ptr);

//class GGrid {};

typedef struct {
	DLong     time;
	DUint     onoff  : 1 ;  // 0=OFF 1=ON
    DUint     next   :15 ;
} Heap_dmon;

typedef struct {
	DByte view_name[11];
	DByte onoff;
	DLong timerange;
} Views;

typedef union 	{
	Analog_units_equate analog_unit;
	Digital_units_equate digital_unit;
} Point_info_units;

/* Point*/
class PointT{
 public:
	DByte number		;
	DByte point_type;
};


/* Point_T3000;*/
class Point_T3000 {
 public:
	DByte number		;
	DByte point_type;
	DByte panel	 ;
 public:
	void setzero(void);
	DInt zero(void);
	DInt cmp(DByte num, DByte p_type, DByte p);
	void putpoint( DByte num, DByte p_type, DByte p);
	void getpoint( DByte *num, DByte *p_type, DByte *p);
	DInt operator==(Point_T3000 a);
	void operator=(const Point_T3000 &copy);
};

/* Point_Net_T3000;*/
class Point_Net {
 public:
	DByte number		;
	DByte point_type;
	DByte panel		;
	DInt  network;
 public:
	void setzero(void);
	DInt zero(void);
	DInt cmp(	DByte num, DByte p_type, DByte p, DInt  net);
	void putpoint(	DByte num, DByte p_type, DByte p, DInt  net);
	void getpoint(	DByte *num, DByte *p_type, DByte *p, DInt *net);
	DInt operator==(Point_Net compare);
	void operator=(const Point_Net &copy);
};

typedef struct{
		DLong prev_header;
		DByte n_inputs;
		Point_Net  inputs[MAX_POINTS_IN_MONITOR];
		DUlong      time_last_sample[MAX_POINTS_IN_MONITOR];
		DUint       n_sample[MAX_POINTS_IN_MONITOR];
		DLong       offset[MAX_POINTS_IN_MONITOR];
		DByte       onoff[MAX_POINTS_IN_MONITOR];
} Header_dmon;

typedef struct{
		 DLong prev_header;
		 DByte n_inputs;
		 Point_Net inputs[MAX_POINTS_IN_MONITOR];
		 DLong interval;
		 DUlong time_last_sample;
		 DInt n_sample;
} Header_amon;

typedef struct {
	Point_Net point;
	DLong point_value;
	DInt  auto_manual	     : 1;  // 0=auto, 1=manual
	DUint digital_analog	   : 1;  // 0=digital, 1=analog
	DUint description_label : 3;  // 0=display description, 1=display label
	DUint security	         : 2;  // 0-3 correspond to 2-5 access level
	DUint decomisioned	     : 1;  // 0=normal, 1=point decommissioned
	DUint units             : 8;
} Point_info;


typedef struct {
	DByte   panel;
	DInt    network;
	DByte   panel_name[16];
	DByte   network_name[16];
  } PanelId;

typedef struct {
	DByte   panel_type;
	DUlong  active_panels;
   DUint   des_length;
	DInt    version;
	DByte   panel_number;
	DByte   panel_name[NAME_SIZE];
	DInt    network;
	DByte   network_name[NAME_SIZE];
} Panel_info1;

typedef struct {
	DByte   channel_descriptor;
	DByte   cards_status;
	DByte   cards_power;
	DByte   power_down;
  } Panel_info2;

typedef struct {
	DByte   port_name;
	DUlong  baud_rate;
	DByte   media_type;
  } MiniCommInfo;

typedef DByte  Subtype;; // (1 DByte ; 128-252 = types 1-125, 253=type 0)


typedef DByte Sub_a_b; //	(1 DByte ; 0=SubA, 1=SubB)


typedef DByte Mass_x;; // (1 DByte , bit0; 0=bad, 1=good)


class icon {
 public:
	DByte name[13];
	DInt  width;
	DInt  height;
//	rgb  pal[16];
    signed char status;
//	TIFFIMAGEDEF idef;
//	TiffDecoder  tifd;
	icon(void){};
	icon( DByte *fn ){};
	void load_icon( char *fn ){};
	~icon( void ){};
};

class Icon 
{
/*
 public:
	DInt  x,y;
	signed char bkgnd_color;
	char *image_under;
	icon *ptr_icon;
	Icon(icon *p, DInt bkgnd = -9);
	DInt draw_icon(DInt x, DInt y, char copy = 0, rgb *current_pal=0, char save_under=1);
	~Icon(void);
*/
};


typedef struct
{
	Point_info 	point_info; 				/* 8 bytes*/

	DUint 	show_point		: 1;
	DUint 	icon_name_index: 7;
	DUint 	nr_element     : 8;

	DLong        high_limit;
	DLong        low_limit;

	DUint 	graphic_y_coordinate	: 10;
	DUint 	off_low_color			: 4;
	DUint 	type_icon	  	      : 2;

	DUint 	graphic_x_coordinate	: 10;
	DUint 	on_high_color			: 4;
	DUint 	display_point_name	: 1;
	DUint 	default_icon			: 1;

	DUint 	text_x_coordinate    : 7;
	DUint 	modify               : 1;
	DUint 	point_absent         : 1;    // 0 - point present, 1 - point absent
	DUint 	where_point          : 2;
	DUint 	text_y_coordinate    : 5;

	signed char bkgnd_icon;

	DUint		xicon                : 10;
	DUint 	text_place				: 4;
	DUint 	text_present			: 1;
	DUint 	icon_present			: 1;

	DUint		yicon                : 10;
	DUint 	text_size		      : 2;
	DUint 	normal_color	      : 4;

}	Str_grp_element; /*8+2+4+4+2+2+1+1+2+2 = 28*/

typedef struct
{
	char        *ptr_save_bkgnd;
	DByte        nchar;
//	struct Icon *icon;
	class icon   icon;
} Str_grp_work_data;

typedef struct
{
	Str_grp_element	    *ptrgrp;
	DInt 			   	nr_elements;
} Control_group_elements;

typedef struct
{
 public:
	DByte current_group;
	DByte nummber_of_groups;
	Str_grp_element *str_grp_element;
} Str_grp_block;

typedef struct
{
	DInt second;		// 0-59
	DInt minute;    		// 0-59
	DInt hour;      		// 0-23
	DInt day;       		// 0-31
	DInt month;     		// 0-11
	DInt year;      		// 0-99
	DInt weekday;   		// 0-6, 0=Sunday
	DInt day_of_year; 	// 0-365
	DInt unused;
} Time_block_sm;


typedef struct {
	DByte grpsize;       // 0-6 inputs per analog monitor
	DByte number_of_mon; // 0-99 monitors with this definition
	DByte length;        // 0-255 data elements
	DUint digital_analog	:1;  // 0-digital monitor, 1-analog monitor
	DUint unused		   :3;
	DUint two_or_4_bytes :1;  // 0=2 DByte s per analog sample, 1=4 DByte s
	DUint not_used	      :3;
} Monitor_element;

class Block
{
 public:
//	HHeader h	;
//	RecordStruct r;
//	GGrid *pgrid;
	DByte current_point,panel_type,panel,network;
	DByte number_of_points;
	DByte bank;
	DByte point_number;
	signed char type;
	DInt fields_number;


	char *ptr_line_buf;

//	 Block(GGrid *pthis, DByte pt, DByte p, DInt net, signed char type, DByte pnum, DInt fields, char *p_line_buf);
	 Block( DByte pt, DByte p, DInt net, signed char type, DByte pnum, DInt fields, char *p_line_buf);
///	 DInt GetAlpha(FIELDHANDLE hfield, DInt bufsize, char *dest);
//	 DInt PutAlpha(FIELDHANDLE hfield, char *buf);
//	 char *GetFldName(FIELDHANDLE hfield);
//	 DInt GetFldSize(FIELDHANDLE hfield);
	 DInt NextRec(void);
	 DInt FirstRec(void);
//	 DInt GotoRec(RECORDNUMBER record_number);
	 DInt NRec(void);
	 DInt NFields(void);
	 void nr_point(char *buf);
	 void SetBank(DByte b);
//	 DInt getalpha(FIELDHANDLE field, char *dest, DInt length);
//	 DInt getpoint(FIELDHANDLE field, Point_Net point, char *dest, DInt read);
//	 DInt getpoint(FIELDHANDLE field, Point_T3000 point, char *dest, DInt read);
//	 DInt readint(FIELDHANDLE field, DInt *pi,  DInt length, char *p, DInt read, DInt min=0, DInt max=0);
//	 DInt readfloat(FIELDHANDLE field, DLong *pf,  DInt length, DInt dec, char *p, DInt read, float min=-1000000000, float max=1000000000);
//	 DInt readinterval(FIELDHANDLE field, char *buf, DLong *seconds , DInt *minutes, DInt *hours );
//	 void *operator new(size_t size){return(newalloc((DLong)size));};
//	void operator delete(void *ptr){newdelete(ptr);};
};

typedef struct {
	char user_name[8];
	DUint port	: 1;
	DUint unused	: 15;
	DLong log_on_time;
	DLong log_off_time;
	} User_element;

typedef struct
{
	DUint current_user;
	DUint number_of_users;
	User_element *block;
} User_block;

typedef struct {
	 char direct;
	 char digital_units_off[12];       //12 bytes; string)
	 char digital_units_on[12];        //12 bytes; string)
} Units_element;             // 30 bytes;

class Units_block  : public Block
{
 public:
	Units_element *block;
	struct UNITS_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

//	 Units_block(GGrid *pthis, DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 Units_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Units_block();
};

typedef struct
{
	char description[21]; 	     /* (21 bytes; string)*/
	char label[9];		         /* (9 bytes; string)*/

	DLong value;		         /* (4 bytes; DLong) */

	DUint auto_manual	   : 1;  /* (1 bit; 0=auto, 1=manual)*/
	DUint digital_analog   : 1;  /* (1 bit; 0=digital, 1=analog)*/
	DUint access_level	   : 3;  /* (3 bits; 0-5)*/
	DUint control          : 1;  /* (1 bit; 0=off, 1=on)*/
	DUint digital_control  : 1;  /* (1 bit)*/
	DUint decom	       	   : 1;  /* (1 bit; 0=ok, 1=point decommissioned)*/
	DUint range            : 8;	 /* (1 DByte ; output_range_equate)*/

	DByte m_del_low;            /* (1 DByte ; if analog then low)*/
	DByte s_del_high;           /* (1 DByte ; if analog then high)*/
	DUint delay_timer;          /* (2 bytes;  seconds,minutes)*/

} Str_out_point;  /* 21+9+2+2 = 40 */

class Output_block : public Block
{
 public:
	Str_out_point *block;
	struct OUT_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

//	 Output_block(GGrid *pthis, DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 Output_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Output_block();
};


typedef struct
{
	char description[21]; 	      /* (21 bytes; string)*/
	char label[9];		      	/* (9 bytes; string)*/

	DLong value;		     						/* (4 bytes; DLong)*/

	DUint filter		   : 3;  /* (3 bits; 0=1,1=2,2=4,3=8,4=16,5=32,6=64,7=128)*/
	DUint decom		      : 1;  /*	(1 bit; 0=ok, 1=point decommissioned)*/
	DUint sen_on		   : 1;  /* (1 bit)*/
	DUint sen_off	      : 1;  /* (1 bit)*/
	DUint control 	      : 1; /*  (1 bit; 0=OFF, 1=ON)*/
	DUint auto_manual		: 1;  /* (1 bit; 0=auto, 1=manual)*/
	DUint digital_analog	: 1;  /* (1 bit; 0=digital, 1=analog)*/
	DUint calibration_sign         : 1;  /* (0 positive; 1 negativ)*/
	DUint calibration_increment    : 1;  /* (1 bit;  0=0.1, 1=1.0)*/
	DUint unused         	       : 5;


	DByte  calibration;  /* (8 bits; -256.0 to 256.0 / -25.6 to 25.6 (msb is sign))*/

	DByte        range;	      			/* (1 DByte ; input_range_equate)*/

} Str_in_point; /* 21+1+4+1+1+9+1 = 38 */

class Input_block : public Block
{
 public:
	Str_in_point *block;
	struct IN_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

//	 Input_block(GGrid *pthis, DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 Input_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Input_block();
};


typedef struct
{
	char description[21];	      /*  (21 bytes; string)*/
	char label[9];		      /*  (9 bytes; string)*/

	DLong value;		      /*  (4 bytes; float)*/

	DUint auto_manual	   : 1;  /*  (1 bit; 0=auto, 1=manual)*/
	DUint digital_analog	: 1;  /*  (1 bit; 0=digital, 1=analog)*/
	DUint control	      : 1;
	DUint unused		   : 5;
	DUint range          : 8; /*  (1 DByte ; variable_range_equate)*/

}	Str_variable_point; /* 21+9+4+2 = 36*/

class Var_block : public Block
{
 public:
	Str_variable_point *block;
	struct VAR_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Var_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Var_block();
};

typedef struct
{
	Point_T3000 input;	      /* (2 bytes; point)*/
	DLong input_value; 	      /* (4 bytes; float)*/
	DLong value;		      /* (4 bytes; float)*/
	Point_T3000 setpoint;	      /* (2 bytes; point)*/
	DLong setpoint_value;	      /* (4 bytes; float)*/
	DByte units;    /* (1 DByte ; Analog_units_equate)*/

	DUint auto_manual	    : 1; /* (1 bit; 0=auto, 1=manual)*/
	DUint action		    : 1; /* (1 bit; 0=direct, 1=reverse)*/
	DUint repeats_per_min : 1; /* (1 bit; 0=repeats/hour,1=repeats/min)*/
	DUint unused		    : 1; /* (1 bit)*/
	DUint prop_high	    : 4; /* (4 bits; high 4 bits of proportional bad)*/

	DUint proportional    : 8;   /* (1 DByte ; 0-2000 with prop_high)*/
	DByte reset;	      /* (1 DByte ; 0-255)*/
	DByte bias;	      /* (1 DByte ; 0-100)*/
	DByte rate;	      /* (1 DByte ; 0-2.00)*/
}	Str_controller_point; /* 3+4+4+3+4+1+1+4 = 24*/

class Controller_block : public Block
{
public:
	Str_controller_point *block;
	struct CON_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Controller_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Controller_block();
};

typedef struct
{
	char description[21];		     /* (21 bytes; string)*/
	char label[9];		      	     /*	(9 bytes; string)*/

	DUint value 			   :1;  /* (1 bit; 0=off, 1=on)*/
	DUint auto_manual		   :1;  /* (1 bit; 0=auto, 1=manual)*/
	DUint override_1_value	:1;  /* (1 bit; 0=off, 1=on)*/
	DUint override_2_value	:1;  /* (1 bit; 0=off, 1=on)*/
	DUint off               :1;
	DUint unused			   :11; /* (11 bits)*/

	Point_T3000 override_1;	     /* (2 bytes; point)*/
	Point_T3000 override_2;	     /* (2 bytes; point)*/

}	Str_weekly_routine_point; /* 21+2+2+2+10 = 38*/

class Weekly_routine_block : public Block
{
public:
	Str_weekly_routine_point *block;
	struct WR_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Weekly_routine_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Weekly_routine_block();
};

typedef struct
{
	char description[21]; 	    /* (21 bytes; string)*/
	char label[9];		      		/* (9 bytes; string)*/
	DUint value				   : 1;  /* (1 bit; 0=off, 1=on)*/
	DUint auto_manual	      : 1;  /* (1 bit; 0=auto, 1=manual)*/
	DUint unused				: 14; 	/* ( 12 bits)*/
}	Str_annual_routine_point;   /* 21+9+2=32 bytes*/

class Annual_routine_block : public Block
{
public:
	Str_annual_routine_point *block;
	struct AR_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Annual_routine_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Annual_routine_block();
};

typedef struct
{
	char description[21]; 	        // (21 bytes; string)*/
	char label[9];			           // (9 bytes; string)*/

	DUint bytes;		        // (2 bytes; size in bytes of program)*/
	DUint     on_off      : 1;    // (1 bit; 0=off; 1=on)*/
	DUint     auto_manual : 1;    // (1 bit; 0=auto; 1=manual)*/
	DUint     com_prg	    : 1;	  // (1 bit; 0=normal , 1=com program)*/
	DUint     errcode	    : 5;	  // (1 bit; 0=normal end, 1=too long in program)*/
	char      unused;                // because of mini's
}	Str_program_point;	  /* 21+9+2+1+1 = 34 bytes*/

class Program_block : public Block
{
public:
	Str_program_point *block;
	DByte *code;
	DInt length;
	struct PRG_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Program_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Program_block();
};

typedef struct
{
	Point_T3000 input;		      	// (2 DByte s; point)
	DByte seconds_on_time;	// (1 DByte ; 0-59)
	DByte minutes_on_time;	// (1 DByte ; 0-59)
	DUint hours_on_time;	// (2 DByte s; 0-65535)
	DUlong start_time;	// (4 DByte s; time)
	DUint  head_index;	// (2 DByte s; index to next data entry)
	DByte wrap_flag;	// (1 DByte ; 0=no wrap, 1=data wrapped)
	DByte reset_flag;	// (1 DByte ; 0=not reset, 1=reset)
	DUint total_starts;	// (2 DByte s; 0-65535 starts since start_time)
	DByte day_starts;	// (1 DByte ; 0-255 starts since 0:00 Hrs)
	DByte data_length; 	// (1 DByte ; 0-254 on/off times recorded)
	DUint data_segment;	// (2 DByte s; used internally by panel)
} Str_digital_monitor_point;

class Digital_monitor_block:public Block
{
public:
	DByte current_point;
	DByte number_of_points;
	Str_digital_monitor_point	*block;
};

typedef struct
{
	char label[9];		      	  					/* 9 bytes; string */

	Point_Net        inputs[MAX_POINTS_IN_MONITOR];	/* 70 bytes; array of Point_T3000 */
	DByte			 range[MAX_POINTS_IN_MONITOR]; /* 14 bytes */

	DByte second_interval_time; 				/* 1 DByte ; 0-59 */
	DByte minute_interval_time; 				/* 1 DByte ; 0-59 */
	DByte hour_interval_time;   				/* 1 DByte ; 0-255 */

	DByte max_time_length;      /* the length of the monitor in time units */

	Views views[MAX_VIEWS];			/* 16 x MAX_VIEWS bytes */

	DUint num_inputs     :4; 	/* total number of points */
	DUint anum_inputs    :4; 	/* number of analog points */
	DUint unit 	 		 :2; 	/* 2 bits - minutes=0, hours=1, days=2	*/
	DUint ind_views		 :2; 	/* number of views */
	DUint wrap_flag		 :1;		/* (1 bit ; 0=no wrap, 1=data wrapped)*/
	DUint status		 :1;		/* monitor status 0=OFF / 1=ON */
	DUint reset_flag	 :1; 	/* 1 bit; 0=no reset, 1=reset	*/
	DUint double_flag	 :1; 	/* 1 bit; 0= 4 bytes data, 1= 2 bytes data */

}	Str_monitor_point; 		/* 9+70+14+3+1+48+2 = 147 bytes */

typedef struct
{
	char *data_segment;
	DUint start   :1;    	          // (1 bit)
	DUint saved	  :1;    	          // (1 bit)
	DUint unused  :6;    	          // (6 bits)
	DUlong next_sample_time;
	DUint  head_index;	    // (2 DByte s; index to next data entry)
	DUlong last_sample_time;	    // (4 DByte s; time)
	DUlong last_sample_saved_time;	    // (4 DByte s; time)
	DUint  start_index_dig[MAX_POINTS_IN_MONITOR];
	DUint  end_index_dig[MAX_POINTS_IN_MONITOR];

}	Str_monitor_work_data;

class Monitor_block: public Block
{
 public:
	Str_monitor_point 	*block;
	struct AMON_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Monitor_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 DInt compute_size(void);
	 ~Monitor_block();
};

class Amon_inputs_block: public Block
{
 public:
	Point_Net *block;
	DInt anum_inputs;
	struct AMON_INPUT_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Amon_inputs_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Amon_inputs_block();
};

typedef struct
{
	char description[21];				/* (21 bytes; string)	*/
	char label[9];							/* (9 bytes; string)	*/
	char picture_file[11];			/* (11 bytes; string)	*/

	DByte update_time;          /* refresh time */
	DUint mode	     :1;		/* (1 bit ; 0=text, 1=graphic)	*/
    DUint state      :1;        /* 1 group displayed on screen  */
	DUint xcur_grp   :15;       /* x coordinate cursor */
	DInt  ycur_grp;             /* y coordinate cursor */

} Control_group_point;				/* (size = 46 bytes)	*/

class Control_group_block: public Block
{
public:
	Control_group_point *block;
	struct GRP_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Control_group_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Control_group_block();

};

class Control_graphic_group_block: public Block
{
public:
	Str_grp_element *block;
	struct G_GRP_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Control_graphic_group_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Control_graphic_group_block();
};

class Str_table_block
{
public:
	DByte number_of_arrays;
	Analog_units_equate table1_units;
	char unused1[5];
	Analog_units_equate table2_units;
	char unused2[5];
	Analog_units_equate table3_units;
	char unused3[5];
	Analog_units_equate table4_units;
	char unused4[5];
	Analog_units_equate table5_units;
	char unused5[5];
	float *table1_array;
	float *table2_array;
	float *table3_array;
	float *table4_array;
	float *table5_array;

};


typedef struct 		// (size = 11 DByte s)
{
	char label[9];		     // (9 DByte s; string)
	DInt  length;  	 // (1 DByte ; 0-255)
} Str_array_point;

class Array_block: public Block
{
 public:
	Str_array_point *block;
	struct AY_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Array_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Array_block();
};

typedef struct
{
	DLong value;
} Str_ayvalue_point;

class Ayvalue_block: public Block
{
 public:
	Str_ayvalue_point *block;
	struct AYvalue_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField
	DInt current_array;

	 Ayvalue_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Ayvalue_block();
};

typedef struct
{
	DUint online_to_network 	  	 :1;  // (1 bit; 0=no, 1=yes)
	DUint program_installed 	 	 :1;  // (1 bit; 0=no, 1=yes)
	DUint mainnet_outpoints_low 	 :6;  // (6 bits; 0-127 with outpoints_high_bit)
	DByte scan_rate;	     			 						// (1 DByte ; 0-255 scans in 3 seconds)
	DUint version 		 					:11; // (11 bits; 0-2047 // 0.00 - 20.47)
	DUint network_controller_flag    :1;  // (1 bit; 0=32ioboard, 1=network controller board)
	DUint port_configuration	 	 	:3;  // (3 bits; Port_configuration_equate)
	DUint mini_panel_flag	 		 	:1;  // (1 bit; 0=standard_panel, 1=mini_panel)
	char panel_name[18];		      				// (18 DByte s; string)
	DUint outpoints_high_bit	 	 	:1;  // (1 bit; MSB of mainnet outpoints)
	DUint unused		      	 		:7;  // (7 bits)
	DUint memory_remaining;	      // (2 DByte s)
	DUint line_status 	      	   :1;  // (1 bit; used internally by panel)
	DUint mainnet_inpoints	 		   :7;  // (7 bits; 0-127)

} Str_status_point;

class Status_block
{
public:
	DByte number_of_points;
	Str_status_point *block;
};

typedef struct {

	Point_Net   point;

	DUint 	modem     	  : 1;
	DUint 	printer		  : 1;
	DUint 	alarm			  : 1;
	DUint 	restored      : 1;
	DUint 	acknowledged  : 1;
	DUint	   ddelete		  : 1;
	DUint    type          : 4; /* DDDD */
	DUint    cond_type     : 4;

	DUlong 		alarm_time;
	char 			alarm_count;
	char 			alarm_message[70];

	DByte alarm_panel;   /* (1 DByte ; 1-32, panel alarm originated from)	*/
	DByte where1;	      /* (1 DByte ; panel# to send alarm to, 255 = all)	*/
	DByte where2;	      /* (1 DByte ; panel# to send alarm to, 0 = none)	*/
	DByte where3;	      /* (1 DByte ; panel# to send alarm to, 0 = none)	*/

} Alarm_point;

class Alarm_block: public Block
{
public:
	Alarm_point *block;
	struct ALARMM_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField
	char nrmes[128];
	 Alarm_block( DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 DInt sortalarm(void);
	 DInt savemessage(char count, char type, DInt point, char cond_type, char *mes, DLong time_mes);
	 ~Alarm_block();
};

typedef struct
{
	Point_T3000 point;
	Point_T3000 point1;
	DUint cond1;
	DLong waylow;
	DLong low;
	DLong normal;
	DLong hi;
	DLong wayhi;
	DUint time;
	signed char nrmes;
	DUint count;
} Alarm_set_point;

class Alarm_set_block: public Block
{
public:
	Alarm_set_point *block;
	struct ALARMS_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField
	 Alarm_set_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Alarm_set_block();
};

/*
class Password_point	// (size = 22 DByte s)
{
public:
	char name[10]; 		       // (10 DByte s; string)
	char code[9]; 		       // (9 DByte s; string)
	DUint master_group 	   :6; // (6 bits; 0-63)
	DUint unused		   :2; // (2 bits)
	DByte access_level;    // (1 DByte ; 0-99)
	DByte master_menu;     // (1 DByte ; 0-30)

//	Password_point( void *x );
};
*/

typedef struct
{
	DByte name[16]; 		       // (10 byte s; string)
	DByte password[9]; 		       // (9 byte s; string)
	DByte access_level;    // (1 byte ; 0-99)
	DUlong rights_access;
	DByte default_panel;
	DByte default_group;
	DByte screen_right[8];     // 2 biti per grp
	DByte program_right[8];    // 2 biti per prg
} Password_point;

typedef struct {
		DInt ind_passwords;
		Password_point	passwords[MAX_PASSW];
} Password_struct;

class Password_block: public Block
{
public:
	Password_struct *block;
	struct USER_NAME_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField
	Password_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	DInt GetRec(void);
	DInt GetField(DInt , char *);
	DInt Read(DInt , char);
	~Password_block();
};

typedef struct	// (size = 6 DByte s)
{
	DInt value; 		       // (10 DByte s; string)
	DLong unit; 		       // (9 DByte s; string)
}  Tbl_point;

typedef struct
{
	char label[9];		      // (9 bytes; string)
	Tbl_point table[16];
} Str_tbl_point;  // size = 9 + 16*6 = 105

class Tbl_block: public Block
{
public:
	Str_tbl_point  *block;
	DInt tblno;
	struct TBL_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField
	Tbl_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	DInt GetRec(void);
	DInt GetField(DInt , char *);
	DInt Read(DInt , char);
	~Tbl_block();
};

typedef struct
{
	char name[11]; 		       // (10 DByte s; string)
} Array1_point;

class Array1_block: public Block
{
public:
	Array1_point *block;
	struct ARRAY1_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField
	 Array1_block( DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Array1_block();
};

typedef struct
{
	DByte hard_name[NAME_SIZE];         // (17 bytes; string)
	DByte name[NAME_SIZE]; 		       // (17 bytes; string)
	DByte number;
	DUint des_length;
	DUint descksum;
	DByte state;
	DByte panel_type;
	DInt  version;
	DByte tbl_bank[MAX_TBL_BANK];
} Station_point;


class Station_block: public Block
{
public:
	Station_point *block;
	struct ST_LineBuf line_buf;
	char stuff[2];         // un octet de zero pt apel ultim field cu GetField
	DInt stype;

	 Station_block(DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Station_block();
};

typedef struct 		// (size = 16 DByte s)
{
	union {
	 struct {
		DByte time_on_minutes1;		// (1 DByte ; 0-59)
		DByte time_on_hours1;		// (1 DByte ; 0-23)
		DByte time_off_minutes1;	// (1 DByte ; 0-59)
		DByte time_off_hours1;		// (1 DByte ; 0-23)
		DByte time_on_minutes2;		// (1 DByte ; 0-59)
		DByte time_on_hours2;		// (1 DByte ; 0-23)
		DByte time_off_minutes2;	// (1 DByte ; 0-59)
		DByte time_off_hours2;		// (1 DByte ; 0-23)
		DByte time_on_minutes3;		// (1 DByte ; 0-59)
		DByte time_on_hours3;		// (1 DByte ; 0-23)
		DByte time_off_minutes3;	// (1 DByte ; 0-59)
		DByte time_off_hours3;		// (1 DByte ; 0-23)
		DByte time_on_minutes4;		// (1 DByte ; 0-59)
		DByte time_on_hours4;		// (1 DByte ; 0-23)
		DByte time_off_minutes4;	// (1 DByte ; 0-59)
		DByte time_off_hours4;		// (1 DByte ; 0-23)
		} time1;
		char time2[16];
	};
} Wr_one_day;

class Weekly_routine_time_block : public Block
{
public:
	DByte number_of_days;
	DByte block[9][16];
//	Wr_one_day block[9];	// (array of  wr_one_day types)
	Wr_one_day *wr_day;
	DInt routine_num;
	struct WR_TIME_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Weekly_routine_time_block( DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 ~Weekly_routine_time_block();
};

class Annual_routine_time_block : public Block
{
public:
	DByte number_of_days;
	DByte *block;
	DByte block_year[46];
	DByte year[6*7*3];
	DByte quarter;
	DInt routine_num;
	struct AR_Y_LineBuf line_buf;
	char stuff;         // un octet de zero pt apel ultim field cu GetField

	 Annual_routine_time_block( DByte pt, DByte p, DInt net, signed char type, DByte pnum);
	 DInt GetRec(void);
	 DInt GetField(DInt , char *);
	 DInt Read(DInt , char);
	 DInt Annual_routine_time_block::ShowDates(DByte quarter);
	 ~Annual_routine_time_block();
};

// Note: January 1 is bit 0 of the first DByte , January 2 is bit 1 of the first
// DByte  and so on. February is always considered to have 29 days.


typedef struct
{
	DUint dst_on_day;	// (2 DByte s; 1-366, day of year dst starts)
	DUint dst_off_day;	// (2 DByte s; 1-366, day of year dst ends)
} Daylight_savings_block;

typedef struct
{
	DUint host_panel	: 7;	// (7 bits, 1-32)
	DUint suba_subb	: 1;	// (1 bit, 0=subA, 1=subB)
} Host_block;

typedef struct
{
		char system_name[L_DIAL_SYSTEM_NAME];   		//
		char phone_number[L_DIAL_PHONE_NUMBER];    	//
		char directory[L_DIAL_DES_FILE];     				//
} Dial_list_point;

class Dial_list_block : public Block
{
	public:
		Dial_list_point *block;
		struct DIAL_LineBuf line_buf;
		char stuff;         // un octet de zero pt apel ultim field cu GetField

		Dial_list_block( DByte pt, DByte p, DInt net, signed char type, DByte pnum);
		DInt GetRec(void);
		DInt GetField(DInt , char *);
		DInt Read(DInt , char);
		~Dial_list_block();
};

typedef struct
{
	DInt  ti_sec;         // 0-59
	DInt  ti_min;         // 0-59
	DInt  ti_hour;           // 0-23
	DInt  dayofmonth;   // 1-31
	DInt  month;          // 0-11
	DInt  year;           // year - 1900
	DInt  dayofweek;        // 0-6 ; 0=sunday
	DInt  dayofyear;    // 0-365 gmtime
	DInt  isdst;
} Time_block;

typedef struct
{
	DByte  ti_sec;         // 0-59
	DByte  ti_min;         // 0-59
	DByte  ti_hour;           // 0-23
	DByte  dayofmonth;   // 1-31
	DByte  month;          // 0-11
	DByte  year;           // year - 1900
	DByte  dayofweek;        // 0-6 ; 0=sunday
	DInt   dayofyear;    // 0-365 gmtime
	signed char isdst;
} Time_block_mini;

struct remote_local_list {
	 Point_Net point;
	 DInt index;
	 };

typedef struct
{
	Point_Net   point;
} WANT_POINTS;

typedef struct
{
	Point_info		info;
}	NETWORK_POINTS;

typedef struct
{
	char			  	state;
	Point_info	 	info;
	char           count;
	DUint		 	read			:2;
	DUint		 	write			:2;
	DUint		 	read_write	:2;
	DUint		 	change		:2;
}	REMOTE_POINTS;

//Added By Paul Mao on 6/8/97 for the GRID edit function:
typedef struct {
	int Index;
    int Rows;
	AnsiString PickListCommaText;
} IndexedPickList;

#endif // _BASECLASS_H