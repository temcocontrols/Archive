#ifndef _BURKEDEF_H
#define _BURKEDEF_H				/* avoid recompilation */

//#include <vcl/sysdefs.h>
#include "define.h"


typedef struct {
	DInt range;
	char range_text[SIZE_TEXT_RANGE];
	char aunits[SIZE_TEXT_UNITS];
	char dunits[2][12];
	char value;
} range_form;

typedef struct {
	DInt range;
	char range_text[SIZE_TEXT_RANGE];
	char aunits[SIZE_TEXT_UNITS];
	char value;
} an_range_form;

typedef struct {
	DInt range;
	char range_text[SIZE_TEXT_RANGE];
	char dunits[2][12];
	char value;
} dig_range_form;


/*
typedef struct {
	DUint status    :4;
    DUint pri       :4;
	DInt    length;
	DInt    next;
	DInt    last;
} Header_pool;

class POOL {
 private:
	DInt length;
	DInt first;
	DInt last;
 public:
   char *buf;
   char access;
	POOL(void);
	POOL(char *p, DInt l);
	void init_pool(char *p, DInt l);
	char *put(char *p, DInt l);
	char *next(void);
	void release(void);
	DUint alloc(DInt l);
	void free(DInt offset);
};
*/

#endif




