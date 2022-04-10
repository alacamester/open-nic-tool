#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "onic_tool.h"

void Load_Filter(unsigned int *ptr, char *fname)
{
	FILE *fin;
	char *line = NULL;
	char *pch;
	int i;
	size_t len = 0;
	ssize_t nread;
	onic_filter aflt;

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
		aflt.flt_ctrl = 0;
		for (i = 0; i < FLT_FIELDS; i++)
		{
			switch (i)
			{
			case 0: // IPv4-SRC
				aflt.ip_src = inet_addr(pch);
				if (aflt.ip_src) // valid
					aflt.flt_ctrl |= FLT_FLAG_IPSRC;
				break;
			case 1: // IPv4-DST
				aflt.ip_dst = inet_addr(pch);
				if (aflt.ip_dst) // valid
					aflt.flt_ctrl |= FLT_FLAG_IPDST;
				break;
			case 2: // IP-protocol
				aflt.ip_proto = atoi(pch);
				if (aflt.ip_proto) // valid
					aflt.flt_ctrl |= FLT_FLAG_IPPROTO;
				break;
			case 3: // Port-SRC
				aflt.l4_src = ntohs(atoi(pch));
				if (aflt.l4_src) // valid
					aflt.flt_ctrl |= FLT_FLAG_L4SRC;
				break;
			case 4: // Port-DST
				aflt.l4_dst = ntohs(atoi(pch));
				if (aflt.l4_dst) // valid
					aflt.flt_ctrl |= FLT_FLAG_L4DST;
				break;
			default:
				break;
			}
			pch = strtok(NULL, " ,");
		}
		if (i == FLT_FIELDS)
		{
			printf("Uploading filter: %d\n", aflt.flt_num);
			printf("CTRL: %X\n", aflt.flt_ctrl);
			printf("IP-SRC: %X\n", aflt.ip_src);
			printf("IP-DST: %X\n", aflt.ip_dst);
			printf("IP-PROTO: %X\n", aflt.ip_proto);
			printf("L4-SRC: %X\n", aflt.l4_src);
			printf("L4-DST: %X\n", aflt.l4_dst);
			printf("----------\n");

			for (int z = 0; z < sizeof(onic_filter)/sizeof(unsigned int); z++)
			{
//				printf("%X\n", *((unsigned int *)&aflt + z));
				ptr[REG_FLT_A] = *((unsigned int *)&aflt + z);
				ptr[REG_FLT_B] = *((unsigned int *)&aflt + z);
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

default:
	break;
}

munmap(ptr, MAP_SIZE);
close(fd);

return 0;
}
