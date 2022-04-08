#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAP_SIZE 4*1024*1024

int main(int argc, char *argv[])
{
int fd;
unsigned int areg;

fd = open(argv[1], O_RDWR);

if (fd < 0)
{
    printf("Open failed!\n");
    return 1;
}

char *ptr = mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
if (!ptr)
{
    printf("Map failed!\n");
    close(fd);
    return 1;
}
areg = strtol(argv[3], NULL, 16);

if (argv[2][0] == 'r')
    printf("Reg[%X]: %.8X\n", areg, *(unsigned int *)(ptr+areg));
else
    *(unsigned int *)(ptr+areg) = strtol(argv[4], NULL, 16);

munmap(ptr, MAP_SIZE);
close(fd);

return 0;
}
