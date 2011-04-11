/* heapvul1.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  Win32堆溢出服务端演示程序1
*/

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#pragma comment (lib,"ws2_32")

#define PORT 8888
#define BUFFLEN 1024

int main()
{
    WSADATA     wsd;
    SOCKET      sListen, sClient;
    struct      sockaddr_in local, client;
    int         iAddrSize;
    unsigned long lBytesRead;
    HANDLE      hHeap;
      
    char        *buf1, *buf2;
    char        buff[0x2000];
      
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("Failed to load Winsock!\n");
        return 1;
    }

    sListen = WSASocket(2,1,0,0,0,0);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    if (bind(sListen, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
    {
        printf("bind() failed: %d\n", WSAGetLastError());
        return 1;
    }
    listen(sListen, 8);
    iAddrSize = sizeof(client);

    hHeap = HeapCreate(HEAP_GENERATE_EXCEPTIONS, 0x10000, 0xfffff);

    sClient = accept(sListen, (struct sockaddr *)&client, &iAddrSize);        
    if (sClient == INVALID_SOCKET)
    {        
        printf("accept() failed: %d\n", WSAGetLastError());
        return 1;
    }
    printf("connect form: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    while (1)
    {
        buf1 = HeapAlloc(hHeap, 0, BUFFLEN);
        buf2 = HeapAlloc(hHeap, 0, BUFFLEN);

        lBytesRead = recv(sClient, buff, 0x2000, 0);
        if (lBytesRead <= 0) break;

        memcpy(buf1, buff, lBytesRead);
        printf("fd: %x buf1: %s\n", sClient, buf1);

        HeapFree(hHeap, 0, buf2);
        HeapFree(hHeap, 0, buf1);
    }

    closesocket(sClient);
    closesocket(sListen);
    WSACleanup();
    return 0;
}
