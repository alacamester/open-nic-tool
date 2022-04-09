
#define MAP_SIZE 4*1024*1024

#define REG_CTRL			0
#define REG_FLT_A			1
#define REG_FLT_B			2

#define FLT_FIELDS			5
#define FLT_FLAG_IPSRC		0x01
#define FLT_FLAG_IPDST		0x02
#define FLT_FLAG_IPPROTO	0x04
#define FLT_FLAG_L4SRC		0x08
#define FLT_FLAG_L4DST		0x10

typedef struct _onic_filter
{
	unsigned char flt_num;
	unsigned char flt_ctrl;
	unsigned char ip_proto;
	unsigned char res1;
	unsigned int ip_src;
	unsigned int ip_dst;
	unsigned short l4_src;
	unsigned short l4_dst;
} onic_filter;
