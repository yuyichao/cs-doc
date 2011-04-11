/* server_thread.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  存在缓冲区溢出漏洞的多线程服务端演示
*/

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>


#pragma comment(lib,"ws2_32")

SOCKET    listenFD;

void overflow(char *p)
{
    char buff[0x10];
    strcpy(buff, p);
    printf("%s", buff);
}

DWORD WINAPI tt(LPVOID lp)
{
    
    SOCKET    s = (SOCKET)lp;
    
    int        ret;
    char    buff[0x800];

    while(1)
    {
        ret = recv(s, buff, sizeof(buff)-1, 0);//overflow
        printf("recv %d bytes!\n", ret);
        if(ret > 0)
        {
            buff[ret] = '\x0';
            overflow(buff);
            send(s, buff, ret, 0);
        }
        else
        {
            printf("[-] recv error:%d\n", WSAGetLastError());
            break;
        }
    }
    closesocket(s);
    return 0;
}

void main()
{
    SOCKET    s1, s2,s3;
    struct sockaddr_in server;
    WSADATA    wsd;
    char    buff[0x500];

    WSAStartup(MAKEWORD(2,2), &wsd);

        s1 = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        s2 = WSASocket(2,1,0,0,0,0);

        listenFD = s2;

        printf("[+] listen socket: %x\n", listenFD);

        server.sin_family = AF_INET;
        server.sin_port = htons(4444);
        server.sin_addr.s_addr= 0;

        bind(listenFD, (struct sockaddr *)&server, sizeof(server));

        listen(listenFD, 100);
        
        while(1)
        {
            s3 = accept(listenFD, 0, 0);
            printf("[+] client socket: %x\n", s3);
            CreateThread(0, 0, tt, s3, 0, 0);
        }
} 
