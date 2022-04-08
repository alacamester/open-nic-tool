#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFLEN 2048

int main(int argc, char *argv[]) 
{
    struct sockaddr_in si_me, si_other;
    int s, i, blen_max, blen, slen = sizeof(si_other);
    char buf[BUFLEN];

    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == -1)
    {
        perror("socket");
        return 1;
   }

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_addr.s_addr = inet_addr(argv[1]);
    si_me.sin_port = ntohs(atoi(argv[2]));

    si_other.sin_family = AF_INET;
    si_other.sin_addr.s_addr = inet_addr(argv[3]);
    si_other.sin_port = 666;

    if (bind(s, (struct sockaddr*) &si_me, sizeof(si_me))==-1)
	{
        perror("bind");
        return 1;
   }

blen_max = atoi(argv[4]);
//blen = atoi(argv[4]);
srand(time(NULL));

while (1)
{
    blen = rand() % blen_max;
    //send answer back to the client
    if (sendto(s, buf, blen, 0, (struct sockaddr*) &si_other, slen) == -1)
    {
        perror("sendto()");
        return 1;
   }
}
    close(s);
    return 0;
}
