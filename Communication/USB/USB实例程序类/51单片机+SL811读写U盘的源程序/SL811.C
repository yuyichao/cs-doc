#include "common.h"
#include "SL811.H"
#include "HAL.H"

extern XXGFLAGS bdata bXXGFlags;

XXGPKG usbstack;
unsigned char remainder;

unsigned char xdata DBUF[BUFFER_LENGTH];

pUSBDEV  idata	uDev;	// Multiple USB devices attributes, Max 5 devices
//xdata pHUBDEV		uHub;			// Struct for downstream device on HUB
//pDevDesc  idata	pDev;			// Device descriptor struct
//pCfgDesc idata	pCfg;			// Configuration descriptor struct
//pIntfDesc idata	pIfc;			// Interface descriptor struct
//pEPDesc idata	pEnp;			// Endpoint descriptor struct
//pStrDesc idata	pStr;			// String descriptor struct
//xdata pHidDesc 		pHid;			// HID class descriptor struct
//xdata pHubDesc 		pHub;			// HUD class descriptor struct
//xdata pPortStatus	pStat;			// HID ports status

//*****************************************************************************************
// SL811H variables initialization
//*****************************************************************************************
unsigned char SL811_GetRev(void)
{
	//SL811Write(SL811_ADDR_PORT, 0x0e);
	return SL811Read(0x0e);
}

void USBReset(void)
{
	unsigned char temp;
    	temp=SL811Read(CtrlReg);   
 	SL811Write(CtrlReg,temp|0x08);
	DelayMs(25);		
	 	
    	SL811Write(CtrlReg,temp);    
}

//*****************************************************************************************
// usbXfer:
// successful transfer = return TRUE
// fail transfer = return FALSE
//*****************************************************************************************
unsigned char usbXfer(void)
{  
	
	unsigned char	xferLen, data0, data1,cmd;
	unsigned char intr,result,remainder,dataX,bufLen,addr,timeout;
	
	//------------------------------------------------
	// Default setting for usb trasnfer
	//------------------------------------------------
	dataX=timeout=0;
	//result 	  = SL811Read(EP0Status);	
	data0 = EP0_Buf;					// DATA0 buffer address
	data1 = data0 + (unsigned char)usbstack.wPayload;	// DATA1 buffer address
	bXXGFlags.bits.DATA_STOP=FALSE;
	bXXGFlags.bits.TIMEOUT_ERR=FALSE;
	//------------------------------------------------
	// Define data transfer payload
	//------------------------------------------------
	if (usbstack.wLen >= usbstack.wPayload)  		// select proper data payload
		xferLen = usbstack.wPayload;			// limit to wPayload size 
	else							// else take < payload len
		xferLen = usbstack.wLen;			//	
	
	// For IN token
	if (usbstack.pid==PID_IN)				// for current IN tokens
	{												//
		cmd = sDATA0_RD;			// FS/FS on Hub, sync to sof
	}
	// For OUT token
	else if(usbstack.pid==PID_OUT)				// for OUT tokens
	{  	
		if(xferLen)									// only when there are	
			{
			//intr=usbstack.setup.wLength;
			//usbstack.setup.wLength=WordSwap(usbstack.setup.wLength);
			SL811BufWrite(data0,usbstack.buffer,xferLen); 	// data to transfer on USB
			//usbstack.setup.wLength=intr;
			}
		cmd = sDATA0_WR;						// FS/FS on Hub, sync to sof
	// implement data toggle
		bXXGFlags.bits.bData1 = uDev.bData1[usbstack.endpoint];
        	uDev.bData1[usbstack.endpoint] = (uDev.bData1[usbstack.endpoint] ? 0 : 1); // DataToggle
		
		if(bXXGFlags.bits.bData1)
          		cmd |= 0x40;                              // Set Data1 bit in command
	}
	// For SETUP/OUT token
	else											// for current SETUP/OUT tokens
	{  	
		if(xferLen)									// only when there are	
			{
			intr=usbstack.setup.wLength;
			usbstack.setup.wValue=WordSwap(usbstack.setup.wValue);
			usbstack.setup.wIndex=WordSwap(usbstack.setup.wIndex);
			usbstack.setup.wLength=WordSwap(usbstack.setup.wLength);
			SL811BufWrite(data0,(unsigned char *)&usbstack.setup,xferLen); 	// data to transfer on USB
			usbstack.setup.wLength=intr;
			}
		cmd = sDATA0_WR;						// FS/FS on Hub, sync to sof
	}
	//------------------------------------------------
	// For EP0's IN/OUT token data, start with DATA1
	// Control Endpoint0's status stage.
	// For data endpoint, IN/OUT data, start ????
	//------------------------------------------------
	if (usbstack.endpoint == 0 && usbstack.pid != PID_SETUP) 	// for Ep0's IN/OUT token
		cmd |= 0x40; 					// always set DATA1
	//------------------------------------------------
	// Arming of USB data transfer for the first pkt
	//------------------------------------------------
	SL811Write(EP0Status,((usbstack.endpoint&0x0F)|usbstack.pid));	// PID + EP address
	SL811Write(EP0Counter,usbstack.usbaddr);					// USB address
	SL811Write(EP0Address,data0);					// buffer address, start with "data0"
	SL811Write(EP0XferLen,xferLen);					// data transfer length
	SL811Write(IntStatus,INT_CLEAR); 				// clear interrupt status
	SL811Write(EP0Control,cmd);						// Enable ARM and USB transfer start here
	
	//------------------------------------------------
	// Main loop for completing a wLen data trasnfer
	//------------------------------------------------
	while(TRUE)
	{   
		//---------------Wait for done interrupt------------------
		while(TRUE)												// always ensure requested device is
		{														// inserted at all time, then you will
			//intr=SL811Read(cSOFcnt);
			//intr=SL811Read(IntEna);
			intr = SL811Read(IntStatus);	
								// wait for interrupt to be done, and 
			if((intr & USB_RESET) || (intr & INSERT_REMOVE))	// proceed to parse result from slave 
			{													// device.
				bXXGFlags.bits.DATA_STOP = TRUE;								// if device is removed, set DATA_STOP
				return FALSE;									// flag true, so that main loop will 
			}													// know this condition and exit gracefully
			if(intr & USB_A_DONE)								
				break;											// interrupt done !!!
		}

		SL811Write(IntStatus,INT_CLEAR); 						// clear interrupt status
		result 	  = SL811Read(EP0Status);						// read EP0status register
		remainder = SL811Read(EP0Counter);						// remainder value in last pkt xfer

		//-------------------------ACK----------------------------
		if (result & EP0_ACK)									// Transmission ACK
		{	

			// SETUP TOKEN
			if(usbstack.pid == PID_SETUP)								// do nothing for SETUP/OUT token 
				break;											// exit while(1) immediately

			// OUT TOKEN				
			else if(usbstack.pid == PID_OUT)
				break;

			// IN TOKEN
			else if(usbstack.pid == PID_IN)
			{													// for IN token only
				usbstack.wLen  -= (WORD)xferLen;	// update remainding wLen value
				cmd   ^= 0x40;    			// toggle DATA0/DATA1
				dataX++;				// point to next dataX

				//------------------------------------------------	
				// If host requested for more data than the slave 
				// have, and if the slave's data len is a multiple
				// of its endpoint payload size/last xferLen. Do 
				// not overwrite data in previous buffer.
				//------------------------------------------------	
				if(remainder==xferLen)			// empty data detected
					bufLen = 0;			// do not overwriten previous data
				else					// reset bufLen to zero
					bufLen = xferLen;		// update previous buffer length
				
				//------------------------------------------------	
				// Arm for next data transfer when requested data 
				// length have not reach zero, i.e. wLen!=0, and
				// last xferlen of data was completed, i.e.
				// remainder is equal to zero, not a short pkt
				//------------------------------------------------	
				if(!remainder && usbstack.wLen)							// remainder==0 when last xferLen
				{												// was all completed or wLen!=0
					addr    = (dataX & 1) ? data1:data0; 		// select next address for data
					xferLen = (BYTE)(usbstack.wLen>=usbstack.wPayload) ? usbstack.wPayload:usbstack.wLen;	// get data length required
					//if (FULL_SPEED)								// sync with SOF transfer
					cmd |= 0x20;							// always sync SOF when FS, regardless 
					SL811Write(EP0XferLen, xferLen); 			// select next xfer length
					SL811Write(EP0Address, addr);           	// data buffer addr 
					SL811Write(IntStatus,INT_CLEAR);			// is a LS is on Hub.
					SL811Write(EP0Control,cmd);					// Enable USB transfer and re-arm
				}				

				//------------------------------------------------
				// Copy last IN token data pkt from prev transfer
				// Check if there was data available during the
				// last data transfer
				//------------------------------------------------
				if(bufLen)										
				{	
					SL811BufRead(((dataX&1)?data0:data1), usbstack.buffer, bufLen);
					usbstack.buffer += bufLen;								
				}

				//------------------------------------------------
				// Terminate on short packets, i.e. remainder!=0
				// a short packet or empty data packet OR when 
				// requested data len have completed, i.e.wLen=0
				// For a LOWSPEED device, the 1st device descp,
				// wPayload is default to 64-byte, LS device will
				// only send back a max of 8-byte device descp,
				// and host detect this as a short packet, and 
				// terminate with OUT status stage
				//------------------------------------------------
				if(remainder || !usbstack.wLen)
					break;
			}// PID IN							
		}
			
		//-------------------------NAK----------------------------
		if (result & EP0_NAK)									// NAK Detected
		{														
			if(usbstack.endpoint==0)										// on ep0 during enumeration of LS device
			{													// happen when slave is not fast enough,
				SL811Write(IntStatus,INT_CLEAR);				// clear interrupt status, need to
				SL811Write(EP0Control,cmd);						// re-arm and request for last cmd, IN token
                		result = 0;                                     // respond to NAK status only
			}
			else												// normal data endpoint, exit now !!! , non-zero ep
				break;											// main loop control the interval polling
		}
	
		//-----------------------TIMEOUT--------------------------
		if (result & EP0_TIMEOUT)								// TIMEOUT Detected
		{														
			if(usbstack.endpoint==0)										// happens when hub enumeration
			{
				if(++timeout >= TIMEOUT_RETRY)
				{	
				    timeout--;
					break;										// exit on the timeout detected	
				}
				SL811Write(IntStatus,INT_CLEAR);				// clear interrupt status, need to
				SL811Write(EP0Control,cmd);						// re-arm and request for last cmd again
			}
			else												
			{													// all other data endpoint, data transfer 
				bXXGFlags.bits.TIMEOUT_ERR = TRUE;								// failed, set flag to terminate transfer
				break;											// happens when data transfer on a device
			}													// through the hub
		}

		//-----------------------STALL----------------------------
		if (result & EP0_STALL)  								// STALL detected
			return TRUE;										// for unsupported request.
																		
		//----------------------OVEFLOW---------------------------
		if (result & EP0_OVERFLOW)  							// OVERFLOW detected
			//result=result;
			break;
		//-----------------------ERROR----------------------------
		if (result & EP0_ERROR)  								// ERROR detected
			//result=result;
			break;
	}	// end of While(1)
   
	if (result & EP0_ACK) 	// on ACK transmission
		return TRUE;		// return OK

	return FALSE;			// fail transmission

}
//*****************************************************************************************
// Control Endpoint 0's USB Data Xfer
// ep0Xfer, endpoint 0 data transfer
//*****************************************************************************************
unsigned char ep0Xfer(void)
{
	//unsigned char wLen;
	
	//wLen=usbstack.wLen;
	usbstack.endpoint=0;
	//----------------------------------------------------
	// SETUP token with 8-byte request on endpoint 0
	//----------------------------------------------------
	usbstack.pid=PID_SETUP;
	usbstack.wLen=8;
	//usbstack.buffer=&usbstack.setup;
	if (!usbXfer()) 
   		return FALSE;
	//DelayMs(10);
	usbstack.pid  = PID_IN;
	//----------------------------------------------------
	// IN or OUT data stage on endpoint 0	
	//----------------------------------------------------
	usbstack.wLen=usbstack.setup.wLength;
   	if (usbstack.wLen)											// if there are data for transfer
	{
		if (usbstack.setup.bmRequest & 0x80)		// host-to-device : IN token
		{
			usbstack.pid  = PID_IN;	
			
			if(!usbXfer())
				return FALSE;
			//usbstack.wPayload = 0;
			usbstack.pid  = PID_OUT;
		}
		else											// device-to-host : OUT token
   		{							
			usbstack.pid  = PID_OUT;
				
			if(!usbXfer())
				return FALSE;
			usbstack.pid  = PID_IN;
		}
	}
	//DelayMs(10);
	//----------------------------------------------------
	// Status stage IN or OUT zero-length data packet
	//----------------------------------------------------
	usbstack.wLen=0;
	if(!usbXfer())
		return FALSE;

	return TRUE;											
					
}


unsigned char epBulkSend(unsigned char *pBuffer,unsigned int len)
{
	usbstack.usbaddr=0x1;
	usbstack.endpoint=usbstack.epbulkout;
	usbstack.pid=PID_OUT;
	usbstack.wPayload=64;
	usbstack.wLen=len;
	usbstack.buffer=pBuffer;
	while(len>0)
	{
		if (len > usbstack.wPayload)
			usbstack.wLen = usbstack.wPayload;
		else				
			usbstack.wLen = len;	
		if(!usbXfer())
			return FALSE;
		len-=usbstack.wLen;
		usbstack.buffer=usbstack.buffer+usbstack.wLen;
		//DelayUs(10);
	}
	return TRUE;	
}

unsigned char epBulkRcv(unsigned char *pBuffer,unsigned int len)
{
	usbstack.usbaddr=0x1;
	usbstack.endpoint=usbstack.epbulkin;
	usbstack.pid=PID_IN;
	usbstack.wPayload=64;
	usbstack.wLen=len;
	usbstack.buffer=pBuffer;
	if(usbstack.wLen)
	{
		if(!usbXfer())
			return FALSE;
	}
	return TRUE;
}
//*****************************************************************************************
// Control endpoint
//*****************************************************************************************
//void VendorCmd(void)
//{ 
//      ep0Xfer();
//}

//*****************************************************************************************
// Set Device Address : 
//*****************************************************************************************
unsigned char SetAddress(unsigned char addr)
{
	usbstack.usbaddr=0;
	usbstack.setup.bmRequest=0;
	usbstack.setup.bRequest=SET_ADDRESS;
	usbstack.setup.wValue=addr;
	usbstack.setup.wIndex=0;
	usbstack.setup.wLength=0;
	//usbstack.buffer=&usbstack.setup;
	return ep0Xfer();

}

//*****************************************************************************************
// Set Device Configuration : 
//*****************************************************************************************
unsigned char Set_Configuration(void)
{
	//usbstack.usbaddr=usbaddr;
	//usbstack.
	usbstack.setup.bmRequest=0;
	usbstack.setup.bRequest=SET_CONFIG;
	//usbstack.setup.wValue=wVal;
	usbstack.setup.wIndex=0;
	usbstack.setup.wLength=0;
	usbstack.buffer=NULL;
	return ep0Xfer();

}

//*****************************************************************************************
// Get Device Descriptor : Device, Configuration, String
//*****************************************************************************************
unsigned char GetDesc(void)
{ 
	
	usbstack.setup.bmRequest=0x80;
	usbstack.setup.bRequest=GET_DESCRIPTOR;
	usbstack.setup.wValue=WordSwap(usbstack.setup.wValue);
	
	usbstack.wPayload=uDev.wPayLoad[0];
	//usbstack.buffer=&usbstack.setup;
	return ep0Xfer();
}

//*****************************************************************************************
// USB Data Endpoint Read/Write
// wLen is in low byte first format
//*****************************************************************************************
/*
unsigned char DataRW(BYTE epaddr)
{
	//xdata BYTE pid = PID_OUT;

	usbstack.pid=PID_OUT;
	usbstack.endpoint=epaddr&0x0F;
	
	if(epaddr & 0x80)	// get direction of transfer
		usbstack.pid = PID_IN;				
	
	if(usbXfer())
		return TRUE;

	return FALSE;
}
*/

//*****************************************************************************************
// USB Device Enumeration Process
// Support 1 confguration and interface #0 and alternate setting #0 only
// Support up to 1 control endpoint + 4 data endpoint only
//*****************************************************************************************
unsigned char EnumUsbDev(BYTE usbaddr)
{  
	unsigned char i;											// always reset USB transfer address 
	unsigned char uAddr = 0;							// for enumeration to Address #0
	unsigned char epLen;
	//unsigned short strLang;
	
	pDevDesc  pDev;	
	pCfgDesc pCfg;
	pIntfDesc pIfc;
	pEPDesc pEnp;
	//------------------------------------------------
	// Reset only Slave device attached directly
	//------------------------------------------------
	uDev.wPayLoad[0] = 64;	// default 64-byte payload of Endpoint 0, address #0
	if(usbaddr == 1)		// bus reset for the device attached to SL811HS only
		USBReset();		// that will always have the USB address = 0x01 (for a hub)
    	
    	DelayMs(25);
	
	//i = SL811Read(EP0Status);	
	//i=SL811Read(IntStatus);
	//------------------------------------------------
	// Get USB Device Descriptors on EP0 & Addr 0
	// with default 64-byte payload
	//------------------------------------------------
	pDev =(pDevDesc)DBUF;					// ask for 64 bytes on Addr #0
	
	usbstack.usbaddr=uAddr;
	usbstack.setup.wValue=DEVICE;
	usbstack.setup.wIndex=0;
	usbstack.setup.wLength=18;
	//usbstack.setup.wLength=sbstack.setup.wLength);
	usbstack.buffer=DBUF;
	
	if (!GetDesc())			// and determine the wPayload size
		return FALSE;								// get correct wPayload of Endpoint 0
	uDev.wPayLoad[0]=pDev->bMaxPacketSize0;// on current non-zero USB address

	//------------------------------------------------
	// Set Slave USB Device Address
	//------------------------------------------------
	if (!SetAddress(usbaddr)) 						// set to specific USB address
		return FALSE;								//
	uAddr = usbaddr;								// transfer using this new address

	//------------------------------------------------
	// Get USB Device Descriptors on EP0 & Addr X
	//------------------------------------------------
	pDev =(pDevDesc)DBUF;
	usbstack.usbaddr=uAddr;
	
	usbstack.setup.wLength=pDev->bLength;
	usbstack.setup.wValue=DEVICE;
	usbstack.setup.wIndex=0;
	
	//usbstack.setup.wLength=0x12;//(unsigned short)DBUF[0];//pDev->bLength;
	usbstack.buffer=DBUF;
	
	if (!GetDesc()) 	
		return FALSE;								// For this current device:
	uDev.wVID  = pDev->idVendor;			// save VID
	uDev.wPID  = pDev->idProduct;			// save PID
	uDev.iMfg  = pDev->iManufacturer;		// save Mfg Index
	uDev.iPdt  = pDev->iProduct;			// save Product Index

	//------------------------------------------------
	// Get String Descriptors
	//------------------------------------------------
	/*
	pStr = (pStrDesc)DBUF;	
	
	usbstack.usbaddr=uAddr;
	usbstack.setup.wValue=STRING;
	usbstack.setup.wIndex=0;
	usbstack.setup.wLength=4;
	usbstack.buffer=DBUF;
	
	if (!GetDesc()) 			// Get string language
		return FALSE;								

	strLang = pStr->wLang;	
	
	usbstack.usbaddr=uAddr;
	usbstack.setup.wValue=(unsigned short)(uDev.iMfg<<8)|STRING;
	usbstack.setup.wIndex=strLang;
	usbstack.setup.wLength=4;
	usbstack.buffer=DBUF;	
						// get iManufacturer String length
	if (!GetDesc()) 		
		return FALSE;	
	
	usbstack.usbaddr=uAddr;
	usbstack.setup.wValue=(unsigned short)(uDev.iMfg<<8)|STRING;
	usbstack.setup.wIndex=strLang;
	usbstack.setup.wLength=pStr->bLength;
	//usbstack.buffer=DBUF;	
							// get iManufacturer String descriptors
	if (!GetDesc()) 		
		return FALSE;			
	*/
	//------------------------------------------------
	// Get Slave USB Configuration Descriptors
	//------------------------------------------------
	
	pCfg = (pCfgDesc)DBUF;	
	
	usbstack.usbaddr=uAddr;
	usbstack.setup.wValue=CONFIGURATION;
	usbstack.setup.wIndex=0;
	usbstack.setup.wLength=64;
	usbstack.buffer=DBUF;	
	if (!GetDesc()) 		
		return FALSE;	
	/*	
	usbstack.usbaddr=uAddr;
	usbstack.setup.wValue=CONFIGURATION;
	usbstack.setup.wIndex=0;
	usbstack.setup.wLength=WordSwap(pCfg->wLength);
	
	usbstack.buffer=DBUF;	
	if (!GetDesc()) 	
		return FALSE;		
	*/
	
	pIfc = (pIntfDesc)(DBUF + 9);					// point to Interface Descp
	uDev.bClass 	= pIfc->iClass;			// update to class type
	uDev.bNumOfEPs = (pIfc->bEndPoints <= MAX_EP) ? pIfc->bEndPoints : MAX_EP;
	
	if(uDev.bClass==8) //mass storage device
		bXXGFlags.bits.bMassDevice=TRUE;
	//------------------------------------------------
	// Set configuration (except for HUB device)
	//------------------------------------------------
	usbstack.usbaddr=uAddr;
	usbstack.setup.wValue=DEVICE;
	//if (uDev[usbaddr].bClass!=HUBCLASS)				// enumerating a FS/LS non-hub device
		if (!Set_Configuration())		// connected directly to SL811HS
				return FALSE;

	//------------------------------------------------
	// For each slave endpoints, get its attributes
	// Excluding endpoint0, only data endpoints
	//------------------------------------------------
	
	epLen = 0;
	for (i=1; i<=uDev.bNumOfEPs; i++)				// For each data endpoint
	{
		pEnp = (pEPDesc)(DBUF + 9 + 9 + epLen);	   			// point to Endpoint Descp(non-HID)
		//if(pIfc->iClass == HIDCLASS)	
		//	pEnp = (pEPDesc)(DBUF + 9 + 9 + 9 + epLen);		// update pointer to Endpoint(HID)
		uDev.bEPAddr[i]  	= pEnp->bEPAdd;			// Ep address and direction
		uDev.bAttr[i]		= pEnp->bAttr;			// Attribute of Endpoint
		uDev.wPayLoad[i] 	= WordSwap(pEnp->wPayLoad);		// Payload of Endpoint
		uDev.bInterval[i] 	= pEnp->bInterval;		// Polling interval
	    	uDev.bData1[i] = 0;			            // init data toggle
		epLen += 7;
		//////////////////////////////
		if(uDev.bAttr[i]==0x2)
		{
		    if(uDev.bEPAddr[i]&0x80)
		    	usbstack.epbulkin=uDev.bEPAddr[i];
		    else
		    	usbstack.epbulkout=uDev.bEPAddr[i];
		}
		//////////////////////////////
	}
	
	return TRUE;
}

//*****************************************************************************************
// Full-speed and low-speed detect - Device atttached directly to SL811HS
//*****************************************************************************************
/*
void speed_detect(void) 
{
	//pNumPort	= 0;					// zero no. of downstream ports
	bXXGFlags.bits.SLAVE_FOUND	= FALSE;				// Clear USB device found flag
	//bXXGFlags.bits.FULL_SPEED  = TRUE;					// Assume full speed device
	//HUB_DEVICE  = FALSE;				// not HUB device
	bXXGFlags.bits.DATA_STOP	= FALSE;				//
 
	SL811Write(cSOFcnt,0xAE);      		// Set SOF high counter, no change D+/D-, host mode
	SL811Write(CtrlReg,0x08);      		// Reset USB engine, full-speed setup, suspend disable
	DelayMs(10);					// Delay for HW stablize
	SL811Write(CtrlReg,0x00);      		// Set to normal operation
	SL811Write(IntEna,0x61);      		// USB-A, Insert/Remove, USB_Resume.
	SL811Write(IntStatus,INT_CLEAR);	// Clear Interrupt enable status
	DelayMs(10);					// Delay for HW stablize

	if(SL811Read(IntStatus)&USB_RESET)
	{									// test for USB reset
		SL811Write(IntStatus,INT_CLEAR);// Clear Interrupt enable status
		DelayMs(30);				// Blink LED - waiting for slave USB plug-in
		//OUTB ^= ACTIVE_BLINK;			// Blink Active LED
		//OUTA |= PORTX_LED;				// clear debug LEDs
		return ;						// exit speed_detect()
	}

	//if((SL811Read(IntStatus)&USB_DPLUS)==0)	// Checking full or low speed	
	//{									// ** Low Speed is detected ** //
	//	SL811Write(cSOFcnt,0xEE);   	// Set up host and low speed direct and SOF cnt
	//	SL811Write(cDATASet,0xE0); 		// SOF Counter Low = 0xE0; 1ms interval
	//	SL811Write(CtrlReg,0x21);  		// Setup 6MHz and EOP enable         
		//uHub.bPortSpeed[1] = 1;			// low speed for Device #1
	//	bXXGFlags.bits.FULL_SPEED = FALSE;				// low speed device flag
	//}
	//else	
	{									// ** Full Speed is detected ** //
		SL811Write(cSOFcnt,0xAE);   	// Set up host & full speed direct and SOF cnt
		SL811Write(cDATASet,0xE0);  	// SOF Counter Low = 0xE0; 1ms interval
		SL811Write(CtrlReg,0x05);   	// Setup 48MHz and SOF enable
		//uHub.bPortSpeed[1] = 0;			// full speed for Device #1
	}

	//OUTB |= ACTIVE_BLINK;				// clear Active LED
	//bXXGFlags.bits.SLAVE_FOUND = TRUE;					// Set USB device found flag
	bXXGFlags.bits.SLAVE_ENUMERATED = FALSE;			// no slave device enumeration

	SL811Write(EP0Status,0x50);   		// Setup SOF Token, EP0
	SL811Write(EP0Counter,0x00);		// reset to zero count
	SL811Write(EP0Control,0x01);   		// start generate SOF or EOP

	DelayMs(25);					// Hub required approx. 24.1mS
	SL811Write(IntStatus,INT_CLEAR);	// Clear Interrupt status
	//return 0;    						// exit speed_detect();
}
*/
//*****************************************************************************************
// Detect USB Device
//*****************************************************************************************
/*
void slave_detect(void)
{
	//int retDataRW = FALSE;
	
	//-------------------------------------------------------------------------
	// Wait for EZUSB enumeration
	//-------------------------------------------------------------------------
	//if(!CONFIG_DONE)			// start SL811H after EZ-USB is configured
	//	return;

	//-------------------------------------------------------------------------
	// Wait for SL811HS enumeration
	//-------------------------------------------------------------------------
	if(!bXXGFlags.bits.SLAVE_ENUMERATED)					// only if slave is not configured
	{
		speed_detect();						// wait for an USB device to be inserted to 
		if(bXXGFlags.bits.SLAVE_FOUND)						// the SL811HST host
		{
	  		if(EnumUsbDev(1))				// enumerate USB device, assign USB address = #1
			{
			   	bXXGFlags.bits.SLAVE_ENUMERATED = TRUE;	// Set slave USB device enumerated flag
				//uHub.bPortPresent[1] = 1;	// set device addr #1 present
				Set_ezDEV(1);				// inform master of new attach/detach
			}	
		}
	}

	//-------------------------------------------------------------------------
	// SL811HS enumerated, proceed accordingly
	//-------------------------------------------------------------------------
	else									
	{													
		//OUTB &= ~ACTIVE_BLINK;				// Turn on Active LED, indicate successful slave enum
		if(Slave_Detach())					// test for slave device detach ???
			return;						// exit now.
		//----------------------------------------------
		// HUB DEVICE Polling (Addr #1, EndPt #1)		
		//----------------------------------------------
		// Polling of Hub deivce Endpoint #1 for any Port Attachement
		// for onboard HUB device, after enumeration, start to
		// transfer IN token to check for ports attachment
		// wLen = wPayload = 1 byte, always use USB address #1
		// if return is TRUE, a data pkt was ACK, data in HubChange
		// else is a NAK, no data was received 
	} // end of else

	return ;
}
*/
//*****************************************************************************************
// Slave_Detach
//*****************************************************************************************
/*
unsigned char Slave_Detach(void)
{
	if( (SL811Read(IntStatus)&INSERT_REMOVE) || (SL811Read(IntStatus)&USB_RESET) )
	{												// Test USB detached?
		bXXGFlags.bits.SLAVE_ENUMERATED = FALSE;					// return to un-enumeration
		//uHub.bPortPresent[1] = 0;					// Device #1 not present

		Set_ezDEV(1);								// inform master of slave detach
		SL811Write(IntStatus,INT_CLEAR); 			// clear interrupt status
		return TRUE;								// exit now !!!
	}

	return FALSE;
}
*/
//*****************************************************************************************
// Indicate to EZUSB's endpoint #2 IN of a new device attach/detach
//*****************************************************************************************
/*
void Set_ezDEV(BYTE chg)
{
	//if( (dsPoll) && (!(IN2CS & bmEPBUSY)) )
	//{
	//	IN2BUF[0] = chg;	// Arm endpoint #2, inform EZUSB host of attach/detatch
	//	IN2BC = 1;
	//}
}
*/
///////////////////////////////////////////////////////////////////////////////////////////
void SL811_Init(void)
{	
	//int i;

	//for(i=0;i<MAX_DEV;i++)
	//{
	//	uHub.bPortPresent[i] = 0;
	//	uHub.bPortNumber[i] = 0;
	//}

	//SL811H_DATA = 0x00;
	//SL811H_ADDR = 0x00;
	//pNumPort	= 0x00;			

	//bXXGFlags.bits.FULL_SPEED = TRUE;	
	//HUB_DEVICE = FALSE;
	bXXGFlags.bits.SLAVE_ONLINE = FALSE;
	bXXGFlags.bits.SLAVE_FOUND = FALSE;
	bXXGFlags.bits.SLAVE_REMOVED=FALSE;
	
	bXXGFlags.bits.SLAVE_ENUMERATED = FALSE;
	bXXGFlags.bits.SLAVE_IS_ATTACHED = FALSE;
	
	//bXXGFlags.bits.BULK_OUT_DONE = FALSE;
	//DESC_XFER = FALSE;
	//DATA_XFER = FALSE;
	//DATA_XFER_OUT = FALSE;
	//DATA_INPROCESS = FALSE;
	//pLS_HUB = FALSE;
	//IN2BUF[0] = 0;

    	//dsPoll = 1;				    // poll downstream port conections

	//----------------------------
	// SL811H + EZUSB I/Os setup
	//----------------------------
	//PORTBCFG &= 0xAC;			// Select i/o function for PB6, PB4, PB1, PB0
	//OEB      |= 0x43;			// Set  PB6(Output), PB4(Input), PB1(Output), PB0(Output) 
	//OUTB     |= 0x43;			// Default output high

	//PORTACFG &= 0x0F;			// Select i/o function for PA7~PA4
	//OEA      |= 0xF0;			// Set PA7~PA4(Output)
	//OUTA     |= 0xF0;			// Default output high

	//----------------------------
	// Debug Monitor I/Os setup
	//----------------------------
	//PORTBCFG |= 0x0C;			// Select alternate function RxD1(PB2) & RxD1(PB3)
	//PORTCCFG |= 0xC0;			// Select alternate function nWR(PC6) & nRD(PC7)
								// Also needed for SL811H
	//----------------------------
	// SL811HST hardware reset
	//----------------------------
	//OUTB &= ~nRESET;			// reset SL811HST
	//EZUSB_Delay(5);				// for 5ms
	//OUTB |= nRESET;				// clear reset
	//OUTB &= ~nHOST_SLAVE_MODE;	// set to Host mode
	///////////////////////////////////////////////////////
	
	SL811Write(cDATASet,0xe0);
	SL811Write(cSOFcnt,0xae);
	SL811Write(CtrlReg,0x5);
			
	SL811Write(EP0Status,0x50);
	SL811Write(EP0Counter,0);
	SL811Write(EP0Control,0x01);
			
	
	SL811Write(IntEna,0x20);      		// USB-A, Insert/Remove, USB_Resume.
	SL811Write(IntStatus,INT_CLEAR);	// Clear Interrupt enable status
}

void check_key_LED(void)
{
	unsigned char intr;
	//SL811Write(IntStatus,INSERT_REMOVE);
	intr=SL811Read(IntStatus);
	if(intr & USB_RESET)
		   {
		   		//bXXGFlags.bits.SLAVE_IS_ATTACHED = FALSE;	// Set USB device found flag
		   if(bXXGFlags.bits.SLAVE_ONLINE ==TRUE)
		   	{bXXGFlags.bits.SLAVE_REMOVED=TRUE;
		   	bXXGFlags.bits.SLAVE_ONLINE =FALSE;}
		   }
	else	{
		   		//bXXGFlags.bits.SLAVE_IS_ATTACHED = TRUE;
		   if(bXXGFlags.bits.SLAVE_ONLINE == FALSE)
		   	{bXXGFlags.bits.SLAVE_FOUND=TRUE;
		   	bXXGFlags.bits.SLAVE_ONLINE =TRUE;}
		   }
	//bXXGFlags.bits.SLAVE_FOUND;
	
	//bXXGFlags.bits.SLAVE_REMOVED=0;
	SL811Write(IntStatus,INT_CLEAR);
	SL811Write(IntStatus,INSERT_REMOVE);
		
}
