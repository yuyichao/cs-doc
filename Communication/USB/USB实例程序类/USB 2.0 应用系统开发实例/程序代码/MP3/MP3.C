
#include "AT89C51SND1_REG.H"
#include "MCU_KEYPAD.H"
#include "MP3_PLAYER.H"
#include "FILE_SYS.H"
void main()
{     
void main()
{     
	 //MP3 Player
	 /*初始化*/
	 MP3player_Init(); //解码器初始化
	 Mutestate(1);//初始化为声状态                                                 
	 P1 = 0x0f;
	 KBCON = 0x00;
	 IEN1 |=EKB;	  //按键初始化                                            
	 EA = 1;      //中断使能  
	/*获得曲目数量*/   
	Number_Song = GetMP3NUM(); 
	/*处理播放曲目程序*/ 
	Playingflag= 0;                
	while(1) //循环
	 {
	 if(Number_Song != 0)//如果存在MP3文件
	 {
	 PlayInit(&SONG[11 * Playingflag]);			
	 Mutestate(0);//有声模式
	 PlayMP3(&SONG[11 * Playingflag]);//播放指定MP3文件
	 Mutestate(1);
	 }
	if(Playingflag ==Number_Song)//循环播放 
	{	
	 Playingflag= 0;
	}
    } 
  }
}


}
//-----------------------------------------------------------------------
void MP3player_Init(void)
{
	MP3VOR = 0x0a;//右声道控制寄存器
	MP3VOL = 0x0a;//左声道音量控制寄存器
	MP3BAS = 0x10;//低音控制寄存器
	MP3MED = 0x10;//媒介控制寄存器
	MP3TRE = 0x10;

	MP3CON &= (~MSKREQ);	                          //Clear to allow the 
	                                                  //MPREQ flag to generate a MP3 interrupt.
	MP3CON |= MPEN;	                                  //使能MP3解码.
}
//------------------------------------------------------------------------
void Mutestate(unsigned char sw)
{ 
    static char volume;	
    
	if(sw)
	    { 
		  volume = MP3VOL;
		  MP3VOR = 0x00;
	      MP3VOL = 0x00;
		  AUDCON1 &= (~0x01);
		  MP3CON &= (~MPEN);
    	 }
    else
	    { 
		  MP3VOR = volume;
	      MP3VOL = volume;
		  AUDCON1 |= 0x01;
		  MP3CON |= MPEN;
		 }
}
//---------------------------------------------------------------------------
unsigned char GetMP3NUM(void)
{
	unsigned char i = 0, j = 0, l = 0;
	int k = 0;
	unsigned char MP3[3] = {'M', 'P', '3'};
    GetFATInfo();
	k = BootSector + RsdSector + 2 * SectorofFatSize;
	ReadPage(k / 32, k % 32, Page_Buf);
 	while (Page_Buf[0] != 0)
	{
	for (j=0; j<16; j++)
	{
	 if (!Page_Buf[j * 32]) break;
	 if (Page_Buf[j * 32] == 0xE5) continue;
	 if (!memcmp(MP3, &Page_Buf[j * 32 + 8], 3))					  
	   {
		for (i=0; i<11; i++) 
		{
		SONG[l * 11 + i] = Page_Buf[j * 32 + i];
		 }
			l++;
		   }
		 }
		 k++;
		 ReadPage(0 + k / 32, k % 32, Page_Buf);
	}
	return l;
}
//----------------------------------------------------------------------
void GetFATInfo(void)
{

	ReadPage(Begin_Cluster, 0, Page_Buf);

	if (!(Page_Buf[0] == 0xeb && Page_Buf[2] == 0x90))	
	{
		
		BootSector = Page_Buf[454] + Page_Buf[455] * 256 + Page_Buf[456] * (256 * 256) + Page_Buf[457] * (256 * 256 * 256);
	}
	else 
	{
		BootSector = 0;
	}


	ReadPage(Begin_Cluster, BootSector, Page_Buf);
	RsdSector = Page_Buf[14] + Page_Buf[15] * 256;
	SecPerClus = Page_Buf[13];

	BytesPerSec = Page_Buf[12] * 256 + Page_Buf[11];

	TotalSector = (Page_Buf[20] * 256 + Page_Buf[19]);
	TotalCapacity = TotalSector * BytesPerSec;
	TotalCluster = TotalSector / SecPerClus; 

	SectorofFatSize = ((Page_Buf[22] + Page_Buf[23] * 256));
	RootEntry = (Page_Buf[18] * 256 + Page_Buf[17]);
 
	FirstDataSec = BootSector + RsdSector + (SectorofFatSize * 2) + ((RootEntry * 32 + (BytesPerSec-1)) / BytesPerSec);

	if (TotalCluster > 65525)
	{               
		FAT_TYPE = FAT32;

		if (TotalSector == 0) 
		{
			TotalSector = (Page_Buf[32] + Page_Buf[33] * 256 + Page_Buf[34] * 256 * 256 + Page_Buf[35] * 256 * 256 * 256);
		}
		TotalCapacity = TotalSector * BytesPerSec;
		TotalCluster = TotalSector / SecPerClus;

		SectorofFatSize = (Page_Buf[36] + Page_Buf[37] * 256 + Page_Buf[38] * 256 * 256 + Page_Buf[39] * 256 * 256 * 256);
		if (SectorofFatSize > (TotalCluster * 16 / 512))
		{
			SectorofFatSize = ((Page_Buf[22] + Page_Buf[23] * 256));
		}
		RootEntry = (Page_Buf[44] * 256 + Page_Buf[43]);
		FirstDataSec = BootSector+RsdSector + (SectorofFatSize * 2) + ((RootEntry * 32 + (BytesPerSec-1)) / BytesPerSec);
		
	}
	else if ((TotalCluster > 0) && (TotalCluster < 4085)) 
	{
		FAT_TYPE = FAT12;
	}
	else
	{	
		FAT_TYPE = FAT16;
	}
}
//-------------------------------------------------------
char PlayInit(unsigned char *SongName)
{
		int i = 0, j = 0, f = 0;
		int k = 0;
		unsigned int total_size;
		RDCOUNT = Read_Sector(SongName, Page_Buf);      
		/*判断ID3V2*/
		if (Page_Buf[0] == 0x49)
		if ((Page_Buf[1] == 0x44) && (Page_Buf[2] == 0x33))
		{
		/*获得MP3标签长度*/
	  total_size = (Page_Buf[6] & 0x7F) * 0x200000 + (Page_Buf[7] & 0x7F) * 0x4000 + (Page_Buf[8] & 0x7F) * 0x80 + (Page_Buf[9] & 0x7F);
		while (total_size > 512)
		{
		Read_Sector(SongName, Page_Buf);
		total_size -=512;
		}
		i = total_size;
	  }
	  /*MP3帧头*/
	if (Page_Buf[i] != 0xFF)i += 10;				 
	  /*获得MP3文件的信息*/			                                                                 
	if ((Page_Buf[i] == 0xFF) && (Page_Buf[i + 1] & 0xF0 == 0xF0))
	{
		MP3_Framehead[0] = Page_Buf[i];
		MP3_Framehead[1] = Page_Buf[i + 1];
		MP3_Framehead[2] = Page_Buf[i + 2];
		MP3_Framehead[3] = Page_Buf[i + 3];
	}
	    /*设置MP3解码器和AUDIO接口的时钟*/
	  if (MP3_Framehead[1] & 0x08) 
	  {
	  switch ((MP3_Framehead[2] & 0x0C) >> 2) 
	  {
	  case 0x00 : MP3FsInit(24, 126, 3, 5); break;          //Fs=44.1kHz
	  case 0x01 : MP3FsInit(124, 575, 3, 4); break;         //Fs=48kHz
	  case 0x02 : MP3FsInit(124, 511, 3, 9); AUDCON0 = 0x76; break;//Fs=32kHz
	  default : break;
	  }
	  } 
	else 
	  {
	  switch ((MP3_Framehead[2] & 0x0C) >> 2)
	  {
	  case 0x00 : MP3FsInit(24, 126, 3, 11); break;         //Fs=22.05kHz
	  case 0x01 : MP3FsInit(124, 575, 3, 9); break;         //Fs=24kHz
	  case 0x02 : MP3FsInit(124, 511, 3, 19); AUDCON0 = 0x76; break;  //Fs=16kHz
	  default : break;
	  }
	}						
	  DataRead = 0;
	  return 1; 
}
//-------------------------------------------------------------------------------
int ReadSector(unsigned char *Name, unsigned char *databuff)
{
	  int i, k, Page;
	  unsigned  long CurrentSector;
	  if (DataRead == 0) 
	  {
	   Page = BootSector + RsdSector + 2 * SectorofFatSize;//文件目录表地址
	   ReadPage(Page / 32, Page % 32, databuff);    //读取文件目录表内容
	   while (databuff[0] != 0)//文件目录表有内容
		{
		for (i=0; i<16; i++)
		{
		if (!memcmp(Name, &databuff[i * 32], 11))//比较文件名
		{
		//获得文件所在簇号
		Current_Cluster = databuff[32 * i + 27] * 256 + databuff[32 * i + 26];
		for (k=31; k>27; k--) 
		{
		DataRemain = (DataRemain << 8) | databuff[i * 32 + k];
		}
		//文件所在逻辑扇区号
		CurrentSector = (Current_Cluster - 2) * SecPerClus + FirstDataSec;
		//读取一个扇区的文件内容
		ReadPage(CurrentSector / 32, CurrentSector % 32, databuff);
		DataRead += 512;
		DataRemain -= 512;
		if (DataRemain < 0) 
		{
		DataRead = 0;
		return (DataRemain + 512);
		}
		else
		{
		return (512);
		}
		}
		}
		Page++;
		ReadPage(0 + Page / 32, Page % 32, databuff);
		}
		return (0);
		}
		else
		{
		Current_Cluster++;
		CurrentSector = (Current_Cluster - 2) * SecPerClus + FirstDataSec;
		ReadPage(CurrentSector / 32, CurrentSector % 32, databuff);
		DataRead += 512;
		DataRemain -= 512;
		if (DataRemain < 0) 
		{
		DataRead = 0;
		return (DataRemain + 512);
		}
		else return (512);
		}
}

//-----------------------------------------------------------------
void PlayMP3(unsigned char *SongName)
{
	int i =0,m =0;

	while (1)
		{   
 			RDCOUNT = ReadSector(SongName, Page_Buf); 
			for (i=0; i<RDCOUNT; i++) 
			{
				while (!(MP3STA1 & MPBREQ)) 
					while (!PlayState);
 
				MP3DAT = Page_Buf[i]; 
			}
			if (ChangeSong) 
			{ 
				DataRead = 0;
				ChangeSong = 0;
				m =1;
			}

			if (RDCOUNT < 512) 
			{
				NowPlaying++;
				MP3InitFlag = 1;
				m =1;
			}

			if(m)
 			 {  
			    while(MP3STA1 & MPFREQ)
			      MP3DAT = 0x00;
				return;
			  }
		}
	
}
//-------------------------------------------------
void key_interrupt() interrupt 11
{
  unsigned char i,j,k;
  /*屏蔽MP3中断*/
  EA = 0;	
  IEN1 &= (～EKB);	           
  k=KBSTA & 0x0f;
  for(j=0;j<10;j++)		   
  for(i=0;i<100;i++);
  if(k==(～P1 & 0x0f))
  /*中断任务处理*/
  switch (k)
  {		                                                     
  case 1 : Func(); break;	//		
  case 2 : Next(); break;
  case 4 : Previous(); break;
  case 8 : PlayPause(); break;//播放暂停
  default : break;
  }
  /*使能MP3中断*/
  IEN1 |= EKB;	          
  EA = 1;	
  k=KBSTA;	                                                  
  return;
}

void Func()
{
  if (function < 3)
	{
	function++;
	}
  else if (function == 3)
	{
	function = 1;
	}
}
void Next()
{
	switch (function)
	{		
	   case SELECT_MP3	:
	   if (Playing == (number_mp3 - 1))
		   Playing = 0; 
	   else 
		   Playing++;	
		   MP3InitFlag = 1;
		   ChangeSong = 1;	
		break;
		case VOLUME	:
		if (MP3VOR > 0x00) 
		{
		   MP3VOR -= 0x01;
		   MP3VOL -= 0x01; 
		}
		break;
		case EFFECT : 
		if (MP3BAS > 2)
		{
		MP3BAS -= 2;
		MP3TRE += 2;
		}
		break;
	  default: break;
	}
  }

void Previous()
{
	switch (function)
	{		
	   case SELECT_MP3	: 
	 	  if (Playing > 0)
		     Playing--; 
		  else 
		     Playing = number_mp3 - 1;
		      MP3InitFlag = 1;
			 ChangeSong = 1;	
	   break;
	   case VOLUME :
		  if (MP3VOR < 0x1f)
	         {
		      MP3VOR += 0x01; 
		      MP3VOL += 0x01; 
	         }
	         break;
		  case EFFECT: 
	         if (MP3BAS < 0x1e)
	           {
			   MP3BAS += 2;
			   MP3TRE -= 2;
	           }
	         break;
		  default:      break;
	}
}

void PlayPause()
{
	PlayState = !PlayState;
	player_state(!PlayState);	
}
