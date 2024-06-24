/*
 *   XMS.H - XMS Memory Interface Functions
 */

#ifndef _XMS_H_
#define _XMS_H_

/*
 * note - this struct is packing sensitive!
 */

struct XMSmovestruct
{
   DUlong   nbytes;
   DUint    srchandle;
   DUlong   srcoffset;
   DUint    dsthandle;
   DUlong   dstoffset;
};

#if defined( __cplusplus )
extern "C" {
#endif

DInt       XMS_installed( void );
DInt       XMS_alloc( DInt nKBytes );
void       XMS_free( DInt XMShand );
DInt       XMS_move( XMSmovestruct *ms );

#if defined( __cplusplus )
}
#endif


typedef struct {
			DInt des_length[12];
			DInt panel_length;
			} Des_index_table;

class XmsDes {
	public:
		DInt status;
//		HANDLE handle;     // xms handle
//		int handle;     // xms handle
        DByte  *ptr;
		DInt netlength;
		DByte buffer[4096];
		Des_index_table  des_index_table[32];
//		XMSmovestruct c2x;   // for move to xms
//		XMSmovestruct x2c;   // for move to cnv
//		XMSmovestruct x2x;   // for move to xms

	XmsDes(DInt nbytes);
	DByte *get(DByte panel, DByte point_type);
	DInt put(DByte panel, DByte point_type, DInt newlength);
	DByte *islabel(DByte *token, DByte *num_point,DByte *var_type, DByte *point_type,DByte *num_panel);
	DByte *look_label(DByte panel, DByte point_type, DByte num);
	DByte *look_descriptor(DByte panel, DByte point_type, DByte num);
	~XmsDes();

	};


class XmsMem {
 public:
	DInt status;
	DInt nitems;
	struct {
	 DUlong offset;
	 DUlong length;
	} table[4];
	DInt handle;     // xms handle
	XMSmovestruct c2x;   // for move to xms
	XMSmovestruct x2c;   // for move to cnv

  XmsMem();
  XmsMem::XmsMem( DUlong nbytes );
  ~XmsMem( );
  void get( DInt item , DByte *buffer);
  void put( DByte *data , DUlong l);
  void Save( DInt sx, DInt sy, DInt jx, DInt jy);
  void Restore(DInt ltopx, DInt ltopy, DInt rbottomx, DInt rbottomy);
  };

#endif
