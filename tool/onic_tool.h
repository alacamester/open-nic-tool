
#define VERSION "v0.3"

#define MAP_SIZE 4*1024*1024

#define REG_BASE_ADDR			(0x200000/sizeof(unsigned int))

#define REG_CTRL			(REG_BASE_ADDR+0)
#define REG_FLT_A			(REG_BASE_ADDR+1)
#define REG_FLT_B			(REG_BASE_ADDR+2)

#define FLT_FIELDS			7
#define FLT_FLAG_IPSRC		0x01
#define FLT_FLAG_IPDST		0x02
#define FLT_FLAG_IPPROTO	0x04
#define FLT_FLAG_L4SRC		0x08
#define FLT_FLAG_L4DST		0x10
#define FLT_FLAG_DROP		0x80 // pass/drop filter
#define FLT_FLAG_ALL		FLT_FLAG_IPSRC | FLT_FLAG_IPDST | FLT_FLAG_IPPROTO | FLT_FLAG_L4SRC | FLT_FLAG_L4DST

#define TYPE_NONE			0
#define TYPE_IPv4			1
#define TYPE_IPv6			2

#define FILTER_COUNT		256

typedef struct _onic_aip
{
	unsigned char type;
	unsigned char mask : 7;
	unsigned char res  : 1;
	unsigned char ip[16];
} onic_aip;

typedef struct _onic_filter
{
	unsigned char flt_num;
	unsigned char res1;
	unsigned char mask_src;
	unsigned char mask_dst : 7;
	unsigned char ipv6 : 1;

	unsigned char ip_src[16];
	unsigned char ip_dst[16];
	unsigned short l4_src;
	unsigned short l4_dst;

	unsigned char flt_ctrl;
	unsigned char ip_proto;
	unsigned short res3;
} onic_filter;

typedef struct _onic_filter_base
{
	unsigned char flt_num;
	unsigned char res1;
	unsigned char mask_src;
	unsigned char mask_dst;

	unsigned int ip_src;
	unsigned int ip_dst;
	unsigned short l4_src;
	unsigned short l4_dst;

	unsigned char flt_ctrl;
	unsigned char ip_proto;
	unsigned short res3;
} onic_filter_base;