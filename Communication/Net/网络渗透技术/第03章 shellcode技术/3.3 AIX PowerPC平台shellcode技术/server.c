/* server.c -  overflow demo
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  存在缓冲区溢出的服务端程序
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

char Buff[1024];
void overflow(char * s,int size)
{
    char s1[50];
    printf("receive %d bytes",size);
    s[size]=0;
    //strcpy(s1,s);
    memcpy(s1, s, size);
    sync(); // 溢出后必须有一些操作产生系统调用，否则直接返回后又会碰到I-cache的问题，shellcode是无法执行的
}

int main(int argc, char *argv[]) 
{ 
    int s, c, ret, lBytesRead; 
    struct sockaddr_in srv; 

    s = socket(AF_INET, SOCK_STREAM, 0); 
    srv.sin_addr.s_addr = INADDR_ANY; 
    srv.sin_port = htons(4444); 
    srv.sin_family = AF_INET; 

    bind(s, &srv, sizeof(srv)); 
    listen(s, 3);

    c = accept(s,NULL,NULL);

    while(1)
    { 
        lBytesRead = recv(c, Buff, 1024, 0);
        if(lBytesRead<=0)    break;

        printf("fd = %x recv %d bytes\n", c, lBytesRead);
        overflow(Buff, lBytesRead);  

        ret=send(c,Buff,lBytesRead,0);
        if(ret<=0)    break;
    }

    close(s);
    close(c);
}
