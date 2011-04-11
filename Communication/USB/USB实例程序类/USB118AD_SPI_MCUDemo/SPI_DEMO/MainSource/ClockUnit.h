//=====================================================================================================================
//	本单元封装了一个时钟和定时器
//=====================================================================================================================
#ifndef	__Clock_Unit_H
#define	__Clock_Unit_H

#include	"Config.h"



//=====================================================================================================================
//	分配的定时器编号
#define	cCLK_TimerCommand	0x00
#define	cCLK_TimerBlink		0x01
#define	cCLK_TimerDelay		0x02
#define	cCLK_MaxTimer		0x03
//=====================================================================================================================


//某个Timer对应的函数。
typedef	void	tCLK_OnTimer	(void);




//初始化Timer0，固定定时间隔为5ms。
void	CLK_Init(void);

//初始化某一个编号的Timer
void	CLK_SetupTimer(uint32	WhichTimer,	uint32	Interval,	tCLK_OnTimer*	UserOnTimer);

//判断是否某个编号的Timer是否应该执行
BOOL	CLK_OnTimer(uint32	WhichTimer);

//延迟一段时间
void	CLK_Delay(uint32	WhichTimer,	uint32	Interval);


#endif

