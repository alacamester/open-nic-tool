#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "onic_tool.h"

void Usage(char *fname)
{

printf("onic_tool %s by Laca\n", VERSION);
printf("Usage: %s <device> <switch> <value(s)>\n", fname);
printf(" switches:\n");
printf(" r <register> : read register address (hex)\n");
printf(" w <register> <value> : write value to register address(hex)\n");
printf(" f <filename> : initialize filter with csv file content\n");
printf(" c : clear filters (drop all packets)\n");

}

const char *ifs[] = {"A", "B", "A+B", "A+swap(B)", "swap(A)+B"};


void Clear_Filters(unsigned int *ptr)
{
int i;
onic_filter_base aflt = {0,};

aflt.flt_ctrl = FLT_FLAG_ALL; // match for all

for (i=0;i<FILTER_COUNT;i++)
{
    aflt.flt_num = i;
    for (int z = 0; z < sizeof(aflt)/sizeof(unsigned int); z++)
    {
//	printf("%X\n", *((unsigned int *)&aflt + z));
	ptr[REG_FLT_A] = *((unsigned int *)&aflt + z);
	ptr[REG_FLT_B] = *((unsigned int *)&aflt + z);
    }

}
printf("All filters cleared!\n");
}

char GetIPAddress(char *pch, onic_aip *pip)
{
	char *pmask;
	pmask = strchr(pch, '/');
	if (pmask)
	{
		pip->mask = atoi(pmask + 1) - 1;
		*pmask = 0;
	}
	else
		pip->mask = 0; 

	memset(&pip->ip, 0x00, sizeof(pip->ip));
	if (inet_pton(AF_INET, pch, &pip->ip)) // is IPv4
	{
		pip->type = TYPE_IPv4;
		if (!pip->mask)
			pip->mask = 127;
		else
			pip->mask += 96;
	}
	else if (inet_pton(AF_INET6, pch, &pip->ip)) // is IPv6
	{
		pip->type = TYPE_IPv6;
		if (!pip->mask)
		    pip->mask = 127;
	}
	else
	{
		pip->type = TYPE_NONE;
		return 0;
	}
	return 1;
}

void Load_Filter(unsigned int *ptr, char *fname)
{
	FILE *fin;
	char *line = NULL;
	char *pch;
	int i;
	unsigned int iface;
	size_t len = 0;
	ssize_t nread;
	onic_aip aip, bip;
	onic_filter aflt = {0,};
	onic_filter bflt = {0,};

	fin = fopen(fname, "r");
	if (!fin)
	{
		printf("File: %s not found!\n", fname);
		return;
	}

	aflt.flt_num = 0;

	while ((nread = getline(&line, &len, fin)) != -1) 
	{
		pch = strtok(line, " ,");
		if (pch[0] == '\n' || pch[0] == '\r') break;
		if (pch[0] == '#') continue; // skip comment
		aflt.flt_ctrl = 0;
		for (i = 0; i < FLT_FIELDS; i++)
		{
			switch (i)
			{
			case 0: // iface
				iface = atoi(pch);
				break;
			case 1:
				if (pch[0] == 'd' || pch[0] == 'D') // drop
					aflt.flt_ctrl |= FLT_FLAG_DROP;
				break;
			case 2: // IP-SRC
				if (GetIPAddress(pch, &aip) && aip.type) // valid
					aflt.flt_ctrl |= FLT_FLAG_IPSRC;
				break;
			case 3: // IP-DST
				if (GetIPAddress(pch, &bip) && bip.type) // valid
					aflt.flt_ctrl |= FLT_FLAG_IPDST;
				break;
			case 4: // IP-protocol
				aflt.ip_proto = atoi(pch);
				if (aflt.ip_proto) // valid
					aflt.flt_ctrl |= FLT_FLAG_IPPROTO;
				break;
			case 5: // Port-SRC
				aflt.l4_src = ntohs(atoi(pch));
				if (aflt.l4_src) // valid
					aflt.flt_ctrl |= FLT_FLAG_L4SRC;
				break;
			case 6: // Port-DST
				aflt.l4_dst = ntohs(atoi(pch));
				if (aflt.l4_dst) // valid
					aflt.flt_ctrl |= FLT_FLAG_L4DST;
				break;
			default:
				break;
			}
			pch = strtok(NULL, " ,");
		}
		if (i == FLT_FIELDS && iface < 5)
		{
			if (aip.type == TYPE_IPv6 || bip.type == TYPE_IPv6)
				aflt.ipv6 = 1;
			else
				aflt.ipv6 = 0;

			memcpy(aflt.ip_src, aip.ip, sizeof(aip.ip));
			memcpy(aflt.ip_dst, bip.ip, sizeof(bip.ip));
			aflt.mask_src = aip.mask;
			aflt.mask_dst = bip.mask;

			printf("Uploading filter: %d to IFs: %s\n", aflt.flt_num, ifs[iface]);

			switch (iface)
			{
			    case 0: // iface A
			    case 1: // iface B
			    case 2: // iface A+B
				break;
			    case 3: // iface A, src/dst swap for B
			    case 4: // iface B, src/dst swap for A
				memcpy(bflt.ip_src, bip.ip, sizeof(bip.ip));
				memcpy(bflt.ip_dst, aip.ip, sizeof(aip.ip));
				bflt.mask_src = bip.mask;
				bflt.mask_dst = aip.mask;
				bflt.ipv6 = aflt.ipv6;
				bflt.l4_src = aflt.l4_dst;
				bflt.l4_dst = aflt.l4_src;
				bflt.flt_ctrl = aflt.flt_ctrl;
				bflt.ip_proto = aflt.ip_proto;
				bflt.flt_num = aflt.flt_num;
				break;
			    default:
				break;
			}

			for (int z = 0; z < sizeof(onic_filter)/sizeof(unsigned int); z++)
			{
				if (!aflt.ipv6) // IPv4, skip IPv6-prefix parts
				{
					if (z > 1 && z <= 4) continue; // skip SRC
					if (z > 5 && z <= 8) continue; // skip DST
				}
//				printf("%.8X\n", *((unsigned int *)&aflt + z));
				switch (iface)
				{
				    case 0:
					ptr[REG_FLT_A] = *((unsigned int *)&aflt + z);
					break;
				    case 1:
					ptr[REG_FLT_B] = *((unsigned int *)&aflt + z);
					break;
				    case 2:
					ptr[REG_FLT_A] = *((unsigned int *)&aflt + z);
					ptr[REG_FLT_B] = *((unsigned int *)&aflt + z);
					break;
				    case 3:
					ptr[REG_FLT_A] = *((unsigned int *)&aflt + z);
					ptr[REG_FLT_B] = *((unsigned int *)&bflt + z);
					break;
				    case 4:
					ptr[REG_FLT_A] = *((unsigned int *)&bflt + z);
					ptr[REG_FLT_B] = *((unsigned int *)&aflt + z);
					break;
				    default:
					break;
				}
			}
			aflt.flt_num++;
		}
	}

	free(line);
	fclose(fin);

}

int main(int argc, char *argv[])
{
int fd;
unsigned int areg;
char *ptr;

if (argc < 3)
{
    Usage(argv[0]);
    return 1;
}

fd = open(argv[1], O_RDWR);

if (fd < 0)
{
    printf("Open failed!\n");
    return 1;
}

ptr = mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
if (!ptr)
{
    printf("Map failed!\n");
    close(fd);
    return 1;
}

switch (argv[2][0])
{
case 'r':
	areg = strtol(argv[3], NULL, 16);
	printf("Reg[%X]: %.8X\n", areg, *(unsigned int *)(ptr + areg));
	break;

case 'w':
	areg = strtol(argv[3], NULL, 16);
	*(unsigned int *)(ptr + areg) = strtol(argv[4], NULL, 16);
	break;

case 'f':
	Load_Filter((unsigned int *)ptr, argv[3]);
	break;

case 'c':
	Clear_Filters((unsigned int *)ptr);
	break;

default:
	Usage(argv[0]);
	break;
}

munmap(ptr, MAP_SIZE);
close(fd);

return 0;
}
