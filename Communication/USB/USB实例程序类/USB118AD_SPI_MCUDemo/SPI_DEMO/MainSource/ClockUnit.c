//=====================================================================================================================
/*
	本单元实现了一个定时器，定时单位为1ms。
*/
//=====================================================================================================================

#include	"Config.h"
#include	"ClockUnit.h"
#include	"IRQ_Unit.h"

//	本单元内部使用的宏定义。
//=====================================================================================================================
//=====================================================================================================================


//	本单元内部使用的全局变量。
//=====================================================================================================================

//定时器使用到的全局变量
struct	tCLK_TimerRec
{
	uint32	Interval;
	uint32	LastTime;
	tCLK_OnTimer*	UserOnTimer;
};

struct	tCLK_TimerRec	vCLK_TimerRec[cCLK_MaxTimer];
uint32	vCLK_TimerCount;	//定时器的计数器，每1ms加1。
//=====================================================================================================================





//*********************************************************************************************************************
/*

		Timer0的中断服务程序。

*/
//=====================================================================================================================
void  IRQ_Timer0(void)  __irq
{
	int	i;

	//计数器加1
	vCLK_TimerCount++;

	for	(i=0;	i<cCLK_MaxTimer;	i++)
	{
		//如果Timer采用中断方式，而不是查询方式的话。
		if	(vCLK_TimerRec[i].UserOnTimer	!=	0)
		{
			uint32	NowInterval,	NeedInterval;
			NowInterval	=	vCLK_TimerCount	-	vCLK_TimerRec[i].LastTime;
			NeedInterval	=	vCLK_TimerRec[i].Interval;
			if	(NowInterval	>=	NeedInterval)
			{
				if	(NowInterval	<	(NeedInterval	<<	1))
				{
					vCLK_TimerRec[i].LastTime	=	vCLK_TimerRec[i].LastTime	+	NeedInterval;
				}
				else
				{
					vCLK_TimerRec[i].LastTime	=	vCLK_TimerCount;
				}
				(*(vCLK_TimerRec[i].UserOnTimer))();
			}
		}
	}

	T0IR		=	0x01;					//	清除中断标志
	VICVectAddr	=	0x00;					//	中断处理结束
}
//=====================================================================================================================






//*********************************************************************************************************************
/*

		初始化Timer0。

入口参数：
	WhichIntSlot	分配给 Timer0	使用的中断号。
*/
//=====================================================================================================================
void	CLK_Init(void)
{
	int	i;

	vCLK_TimerCount	=	0;
	for	(i=0;	i<cCLK_MaxTimer;	i++)
	{
		vCLK_TimerRec[i].Interval	=	1000;
		vCLK_TimerRec[i].LastTime	=	0;
		vCLK_TimerRec[i].UserOnTimer	=	0;
	}

	
	//#define Fosc            24M                    //Crystal frequence,10MHz~25MHz，should be the same as actual status. 
	//#define Fcclk           (Fosc *  2) = 48M                  //System frequence,should be (1~32)multiples of Fosc,and should be equal or less  than 60MHz. 
	//#define Fpclk           (Fcclk / 4) * 1   =  12M          //VPB clock frequence , must be 1、2、4 multiples of (Fcclk / 4).
	//初始化为1ms定时间隔
	T0PR	=	119;		//	设置定时器0分频为120分频，得100,000Hz
	T0MCR	=	0x03;		//	匹配通道0匹配中断并复位T0TC
	T0MR0	=	100;		//	比较值100(1ms定时值)
	T0TCR	=	0x03;		//	启动并复位T0TC
	T0TCR	=	0x01;

	//初始化中断
	IRQ_SetInt(cIRQ_Timer0,	0,	(DWORD)IRQ_Timer0);
}
//=====================================================================================================================






//*********************************************************************************************************************
/*

		初始化Timer通道。

入口参数：
	WhichIntTimer	哪个Timer。
	Interval		Timer定时间隔，以ms为单位。
	UserOnTimer		用户的定时响应程序。如果该值为0，则表示采用查询方式。
*/
//=====================================================================================================================
void	CLK_SetupTimer(uint32	WhichTimer,	uint32	Interval,	tCLK_OnTimer*	UserOnTimer)
{
	if	(Interval	==	0)
		Interval	=	1;
	

	vCLK_TimerRec[WhichTimer].Interval	=	Interval;
	vCLK_TimerRec[WhichTimer].LastTime	=	vCLK_TimerCount;
	vCLK_TimerRec[WhichTimer].UserOnTimer	=	UserOnTimer;
}
//=====================================================================================================================




//*********************************************************************************************************************
/*

		判断是否某个编号的Timer是否应该执行。

入口参数：
	WhichIntTimer	哪个Timer。
出口参数：
	真：该Timer已经应该被触发。
*/
//=====================================================================================================================
BOOL	CLK_OnTimer(uint32	WhichTimer)
{
	uint32	NowInterval,	NeedInterval,	NowCount;

	NowCount	=	vCLK_TimerCount;
	NowInterval	=	NowCount	-	vCLK_TimerRec[WhichTimer].LastTime;
	NeedInterval	=	vCLK_TimerRec[WhichTimer].Interval;
	if	(NowInterval	>=	NeedInterval)
	{
		if	(NowInterval	<	(NeedInterval	<<	1))
		{
			vCLK_TimerRec[WhichTimer].LastTime	=	vCLK_TimerRec[WhichTimer].LastTime	+	NeedInterval;
		}
		else
		{
			vCLK_TimerRec[WhichTimer].LastTime	=	NowCount;
		}
		return	TRUE;
	}

	return	FALSE;
}
//=====================================================================================================================


//*********************************************************************************************************************
/*

		延迟一段时间。

入口参数：
	WhichIntTimer	哪个Timer。
出口参数：
	真：该Timer已经应该被触发。
*/
//=====================================================================================================================
void	CLK_Delay(uint32	WhichTimer,	uint32	Interval)
{
	CLK_SetupTimer(WhichTimer,	Interval,	NULL);
	while	(!CLK_OnTimer(WhichTimer))
	{}
}
//=====================================================================================================================


