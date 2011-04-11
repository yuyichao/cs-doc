#include <AT89X52.H>

#define NULL		0
#define TRUE   		1
#define FALSE  		0

#define UARTBUF_LENGTH  16384
#define BUFFER_LENGTH	8192
#define MAX_READ_LENGTH 16384
#define MAX_WRITE_LENGTH 2048
// Disable all interrupts
#define DISABLE_INTERRUPTS		EA = 0
// Enable all interrupts
#define ENABLE_INTERRUPTS		EA = 1

typedef unsigned char       INT8;
typedef unsigned short      INT16;
typedef unsigned long       INT32;

typedef union {
    struct 
    {
        INT8    Type;
        INT8    Index;
    } Descriptor;

    struct 
    {
        INT8 tx0;  // MSB for 8051 Keil C
        INT8 tx1;   
        INT8 endp;   
        INT8 c0;   // LSB for 8051 Keil C
    } chars;

    struct 
    {
        INT8 c3;  // MSB for 8051 Keil C
        INT8 c2;   
        INT8 c1;   
        INT8 c0;   // LSB for 8051 Keil C
    } chars0;

    struct
    {
        INT16 i1;    // MSW for 8051 keil C
        INT16 i0;    // LSW for 8051 Keil C
    } ints;

    INT32 u0;

} FLEXI_INT32;

typedef union  {
    struct 
    {
        INT8 c1;   // MSB for 8051 Keil C
        INT8 c0;   // LSB for 8051 Keil C
    } chars;

    INT16 i0;
} FLEXI_INT16, * PFLEXI_INT16;


typedef union _XXG_FLAGS
{
	struct _FLAGS
	{
		unsigned char bTimer               	: 1;
		//unsigned char bBus_Reset           	: 1;
		//unsigned char bSuspend             	: 1;
		//unsigned char bSetup_Packet  	  	: 1;
		//unsigned char bRemote_Wakeup		: 1;
		unsigned char bIN_ISR		      	: 1;
		//unsigned char bControl_State		: 2;

		//unsigned char bConfiguration		: 1;
		//unsigned char bVerbose				: 1;
		//unsigned char bEP1_RxDone			: 1;
//		unsigned char bSetup_DMA			: 2; // V2.3
		//unsigned char bDMA_State      		: 2;
//		unsigned char bPower_Down			: 1; // Smart Board
		//unsigned char bLED					: 2; // Used for LED
		//unsigned char bRead					: 1; // TRUE = Read, FALSE = Write. Now this bit is not used, just reserved for future usage.
//		unsigned char bCodeTransfered		: 1; // TRUE = Finished, FALSO = Need more data
		unsigned char   bCOM_ERR		:1;
		unsigned char   bTimeout		:1;
		unsigned char   SLAVE_IS_ATTACHED	:1;
		unsigned char   SLAVE_REMOVED		:1;
		unsigned char  	SLAVE_FOUND		:1;	// Slave USB device found
		unsigned char  	SLAVE_ENUMERATED	:1;	// slave USB device enumeration done
		unsigned char   SLAVE_ONLINE		:1;
		//unsigned char  	FULL_SPEED		:1;	// Full-Speed = TRUE, Low-Speed = FALSE
		//unsigned char	HUB_DEVICE		:1;	// HUB device = TRUE
//BOOL 	BULK_OUT_DONE;				// Set when EZUSB's OUT1 hostcmd xfer is done
//BOOL 	DESC_XFER;					// Set when there is data for EZUSB's IN1 desc xfer
//BOOL 	DATA_XFER;					// Set when there is data for EZUSB's IN3 data xfer
//BOOL 	DATA_XFER_OUT;				// Set when there is data for EZUSB's OUT3 data xfer
//BOOL 	CONFIG_DONE;				// Set when EZUSB completes its enumeration process.
		unsigned char 	TIMEOUT_ERR		:1;				// timeout error during data endpoint transfer
		unsigned char	DATA_STOP		:1;					// device unplugged during data transfer
//BOOL	DATA_INPROCESS;				// set when we are in a data pipe transfer
//BOOL	pLS_HUB;					// indicate previous command is a LS device on hub

//BOOL	dsPoll;				        // poll downstream port conections

		unsigned char bData1			:1;
		unsigned char bUartInDone		:1;
		///////////////////////////////////////////
		unsigned char bMassDevice		:1;
		
	} bits;
	//unsigned short nValue;
} XXGFLAGS;

