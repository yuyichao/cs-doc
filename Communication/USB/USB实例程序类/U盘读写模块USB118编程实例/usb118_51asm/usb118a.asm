/************************************************************************************************
*                           
*                                 USB118a 51汇编测试程序
*                                                               Version 1.1
*                西安达泰电子有限责任公司（Xi'an DATA Electronic Co,Ltd）      
*   网址： http://www.dataie.com 
*   电话： 029-85272421, 88022900
*  
*                                             西安达泰电子有限责任公司        2008.05
*************************************************************************************************/


/************************************************************************************************
*
*  U盘文件读写模块 以字节为单位进行U盘文件读写。
*  MCS-51单片机汇编示例程序 
*  硬件连接方式: 4线制串口（电源，地，输入，输出） 
*
*************************************************************************************************/

/*--------------------------------
	初始化程序
---------------------------------*/
	ORG 0000H
	AJMP MAIN
	ORG 0100H
/*--------------------------------
主函数
---------------------------------*/
MAIN:
	MOV  SP,  #60H		;设置堆栈指针
	MOV  TMOD,#20H		;计数器1工作在方式2
	MOV  TH1, #0F3H		;晶振频率为24MHZ，波特率为9600
	MOV  TL1, #0F3H
		
   ;MOV  TH1,  #0FAH		 ;晶振频率为11.0592MHZ,波特率为9600
   ;MOV  TL1,  #0FAH	

	MOV  PCON,#80H		;smod为1
	SETB  TR1		    ;启动计时
	MOV  SCON,#40H

/*---------------------------------------------------------------------
   延时是由于本人所用的单片机和USB118A是同一电源供电，
起初上电复位后由于USB118A要进行U盘检测，延时大约让检测完成即可发送指令		
----------------------------------------------------------------------*/
	MOV  R1,#10H		;闪烁10次
ML:	
	LCALL DELAY		
	LCALL DELAY		;延时
	LCALL DELAY
	LCALL DELAY
	DJNZ R1,ML

//检测U盘是否连接正常
USB_Detect:	
    CLR    EA			;关闭中断
    CLR    REN		    ;关闭接收位
	MOV    DPTR, #Detect ;检测U盘命令初址
    MOV    R7,  #66		;R7定义为将要发送的字节数
	LCALL  SEND	    	;调用发送子程序
	SETB   REN	    	;打开接收位

    JNB    RI,$         ;等待接收到数据
	CLR    RI
    MOV    A,SBUF		;接收应答码
	CJNE   A,#04H,OUT1	;04检测应答码，对则继续执行 判断是否成功执行 否则转错误处理
 
    JNB    RI,$         ;等待接收到数据
	CLR    RI
	MOV    A,SBUF		;接收应答信号
	CJNE   A,#01H,OUT1	;01判断是否成功执行，成功则继续执行 否则转错误处理


//create file			
    LCALL  CRT
	JNB    RI,$         ;等待接收到数据
	CLR    RI
    MOV    A,SBUF		;接收应答码
	CJNE   A,#0AH,OUT1	;0A检测应答码，对则继续执行 判断是否成功执行 否则转错误处理
 
    JNB    RI,$             ;等待接收到数据
	CLR    RI
	MOV    A,SBUF		;接收应答信号
	CJNE   A,#01H,OUT1	;01判断是否成功执行，成功则继续执行 否则转错误处理



//open file
	LCALL OP
    JNB    RI,$         ;等待接收到数据
	CLR    RI
    MOV    A,SBUF		;接收应答码
	CJNE   A,#0DH,OUT1	;0D检测应答码，对则继续执行 判断是否成功执行 否则转错误处理
 
    JNB    RI,$             ;等待接收到数据
	CLR    RI
	MOV    A,SBUF		;接收应答信号
	CJNE   A,#01H,OUT1	;01判断是否成功执行，成功则继续执行 否则转错误处理

//write data
	LCALL  WRT
	SJMP $	  
    MOV    IE,#93H
    RET

/*----------------------------------------------
函数功能：当为检测到U盘时，从新发送指令检测
-----------------------------------------------*/
OUT1:				    
	MOV R7,#66		  ;R7定义为将要发送的字节数
	LJMP   USB_Detect ;检测到u盘未成功，重新检测
	RET


/*-----------------------------------------
函数功能：Create file
-------------------------------------------*/
CRT:
   	CLR    REN		  ;关闭接收位
	LCALL  DELAY
    MOV R7, #66		  ;R7定义为将要发送的字节数
	MOV DPTR,#Create  ;建立文件命令初址
	LCALL SEND	      ;调用发送子程序
	NOP
	NOP
	NOP
	SETB REN		  ;打开接收位
	RET


/*----------------------------------------
函数功能：Open file
------------------------------------------*/
OP:
    CLR    REN		 ;关闭接收位 	
	LCALL  DELAY
	MOV R7, #66		 ;R7定义为将要发送的字节数
	MOV DPTR,#Open	 ;打开文件命令初址
	LCALL SEND	     ;调用发送子程序
	NOP
	NOP
	NOP
	SETB REN		  ;打开接收位
	RET
	
/*-----------------------------------------
函数功能：Write data
-------------------------------------------*/
WRT:	
	CLR    REN			 ;关闭接收位
	LCALL  DELAY
	MOV R7,#66		 ;R7定义为将要发送的字节数
	MOV DPTR,#Write	 ;写文件命令初址	
	LCALL  SEND		 ;发送数据

	MOV R7,#15		 ;此处是将要发送的写入数据的字节数
ON:	MOV DPTR,#Date1	 ;数据初地址
	LCALL  SEND		 ;发送数据
	NOP
	NOP
	SETB REN		;打开接收位
	RET

/*----------------------------------------------
函数功能：Remove file
------------------------------------------------*/
REM:	
	CLR   REN			 ;关闭接收位
	LCALL DELAY
	MOV DPTR,#Remove ;设置删除命令初址
	LCALL  SEND		 ;发送命令
	NOP
	NOP
	NOP
	SETB REN		 ;打开接收位
	RET

/*--------------------------------------------------
函数功能：设置文件指针	
----------------------------------------------------*/	
SFP:	
	CLR REN			         ;关闭接收位
	LCALL DELAY
	MOV R7,#66
	MOV DPTR,#SetFilePointer ;设置文件指针命令初址
	LCALL SEND		          ;发送命令
	NOP
	NOP
	NOP
	SETB REN		           ;打开接收位
	RET

/*----------------------------------------------
函数功能：Read file
-----------------------------------------------*/
RAD:
	CLR REN			  ;关闭接收位
	LCALL DELAY
	MOV R7,#66
	MOV DPTR,#Read	  ;设置读命令初址
	LCALL SEND		  ;发送命令
	NOP
	NOP
	NOP
	SETB REN		  ;打开接收位
    RET
/*--------------------------------------------------
函数功能：Make  dir
---------------------------------------------------*/
MDIR:
    CLR  REN			;关闭接收位
	LCALL DELAY
	MOV R7, #66
	MOV DPTR, #MakeDir	;设置建立目录初址
	LCALL SEND			;发送命令
	NOP
	NOP 
	NOP
	SETB REN			;打开接收位
	RET

/*--------------------------------------------------
函数功能：In dir
---------------------------------------------------*/
IND:
    CLR  REN		  ;关闭接收位
	LCALL DELAY
	MOV R7, #66
	MOV DPTR, #InDir   ;设置进入目录初址
	LCALL SEND		   ;发送命令
	NOP
	NOP 
	NOP
	SETB REN			;打开接收位
	RET

/*--------------------------------------------------
函数功能：Out dir
---------------------------------------------------*/
OUTD:
    CLR  REN			;关闭接收位
	LCALL DELAY
	MOV R7, #66
	MOV DPTR, #OutDir	 ;设置返回目录初址
	LCALL SEND			 ;发送命令
	NOP
	NOP 
	NOP
	SETB REN			 ;打开接收位
	RET

/*--------------------------------------------------
函数功能：Root dir
---------------------------------------------------*/
ROOTD:
    CLR  REN			;关闭接收位
	LCALL DELAY
	MOV R7, #66
	MOV DPTR, #RootDir	;设置返回根目录初址
	LCALL SEND			;发送命令
	NOP
	NOP 
	NOP
	SETB REN			;打开接收位
	RET

/*-------------------------------------------------
函数功能：Send data
--------------------------------------------------*/
SEND:
	MOV    R0,#00H	 ;计数
LOOP:
	MOV    A,R0
	MOVC   A,@A+DPTR ;查表得到要发送的数据
	MOV    SBUF,A	 ;发送数据
	JNB    TI,$		 ;等待发送完毕
    CLR    TI			    
	INC    R0
	DJNZ   R7,LOOP	 ;R7发送字节数
	RET
		
/*---------------------------------------------------
函数功能：延时子程序
-----------------------------------------------------*/
DELAY:	
    MOV R5,#0FFH
LOOP2:
	MOV R4,#0FFH
LOOP1:
	NOP
	NOP
	DJNZ R4,LOOP1
	DJNZ R5,LOOP2
	RET
																	   
/*------------------------------------------------------------------------------------
说明：以下是要发送的命令和数据
-------------------------------------------------------------------------------------*/
Detect:				;检测u盘命令
	DB 0AAH,0BBH,01H	
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H

Create:				;建立TEST.TXT文件命令
	DB 0AAH,0BBH,07H	
	DB 54H,45H,53H,54H,20H,20H,20H,20H	                    ;Create file name 
	DB 54H,58H,54H					                       	;File type
	DB 20H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 63H,4FH,0FDH,32H	                                 ;Create and  revise file time
	DB 00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H 

Open:				;打开文件命令
	DB 0AAH,0BBH,06H		        	
	DB 54H,45H,53H,54H,20H,20H,20H,20H	                     ;Open file name
	DB 54H,58H,54H						                     ;File type
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H

Write:				;写文件命令	
	DB 0AAH,0BBH,09H,00H	
    DB 0FH,00H		                                          ;Write byte number
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H

Read:				;读文件命令
	DB 0AAH,0BBH,08H
	DB 08H,00H		                                           ;Read file length
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H

Remove:				;删除文件命令
	DB 0AAH,0BBH,11H,00H
	DB 54H,45H,53H,54H,20H,20H,20H,20H	                      ;Delete file name
	DB 54H,58H,54H						                      ;File type
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H

SetFilePointer:			;设置文件指针命令
	DB 0AAH,0BBH,15H,00H
	DB 148,02H,00H,00H	                                       ;设置指针位置
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H
	DB 20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H,20H

MakeDir:			;建立TEST目录
	DB 0AAH,0BBH,32H
	DB 54H,45H,53H,54H,20H,20H,20H,20H,20H,20H,20H,10H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,0E3H,7EH,0FH,33H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H

InDir:				;进入TEST目录
	DB 0AAH,0BBH,54H,45H,53H,54H,20H,20H,20H,20H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H

OutDir:				;返回上一层目录
	DB 0AAH,0BBH,34H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H

RootDir:			;返回根目录
	DB 0AAH,0BBH,35H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H,00H
	DB 00H,00H

Date1:				;数据1
	DB 41H,42H,43H,44H,45H,46H,47H,48H,49H,50H,61H,62H,63H,64H,65H
	DB 30H,31H,32H,33H,34H,35H,36H,37H,38H,39H,61H,62H,63H,64H,65H
	DB 30H,31H,32H,33H,34H,35H,36H,37H,38H,39H,61H,62H,63H,64H,65H
	DB 30H,31H,32H,33H,34H,35H,36H,37H,38H,39H,61H,62H,63H,64H,65H
	DB 30H,31H,32H,33H,0DH,0AH
END

