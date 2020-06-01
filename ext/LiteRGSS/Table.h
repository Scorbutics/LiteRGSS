#ifndef L_TABLE_HEADER
#define L_TABLE_HEADER

struct rb_Table_Struct_Header {
	unsigned int dim = 0u;
	unsigned int xsize = 0u;
	unsigned int ysize = 0u;
	unsigned int zsize = 0u;
	unsigned int data_size = 0u;
};

struct rb_Table_Struct {
	rb_Table_Struct_Header header {};
	short* heap = nullptr;
};

#endif
