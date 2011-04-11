//=====================================================================================================================
//	本单元封装了异步串行口的基本操作细节。
//=====================================================================================================================
#ifndef	IRQ_UNIT_H
#define	IRQ_UNIT_H


#include  "config.h" 

//=====================================================================================================================
//	定义单元的全局常量
//=====================================================================================================================
		//定义字长
#define	cIRQ_WDT		0x00
#define	cIRQ_SoftInt	0x01
#define	cIRQ_DBGRx		0x02
#define	cIRQ_DBGTx		0x03
#define	cIRQ_Timer0		0x04
#define	cIRQ_Timer1		0x05
#define	cIRQ_UART0		0x06
#define	cIRQ_UART1		0x07
#define	cIRQ_PWM0		0x08
#define	cIRQ_I2C		0x09
#define	cIRQ_SPI0		0x0A
#define	cIRQ_SPI1		0x0B
#define	cIRQ_PLL		0x0C
#define	cIRQ_RTC		0x0D
#define	cIRQ_EInt0		0x0E
#define	cIRQ_EInt1		0x0F
#define	cIRQ_EInt2		0x10
#define	cIRQ_EInt3		0x11
#define	cIRQ_ADC		0x12
//=====================================================================================================================
//	定义单元的全局函数常量类型
//=====================================================================================================================


void	IRQ_SetInt(uint32	WhichInt,	uint32	WhichSlot,	uint32	ISR_Addr);
void	IRQ_Enable(uint32	WhichInt);
void	IRQ_Disable(uint32	WhichInt);


#endif
