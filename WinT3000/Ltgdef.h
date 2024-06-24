
// Following From old T3000Def.h IN ..\WinT3000
//#define OUT 		 0
//#define IN  		 1

#define MAX_OUTS_MINI  64
#define MAX_INS_MINI   64

typedef struct {
	 long start_address;
	 long size;
	 char logical_number;
	 unsigned char   used:1;
	 unsigned char erased:1;
	 unsigned char  error:1;
	} SectorInfo;

