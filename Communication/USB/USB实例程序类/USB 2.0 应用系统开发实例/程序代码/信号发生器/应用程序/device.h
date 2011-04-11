//devintf.h - include file for classes CDeviceInterfaceClass and CDeviceInterface
//=============================================================================
//
// Compuware Corporation
// NuMega Lab
// 9 Townsend West
// Nashua, NH 03060  USA
//
// Copyright (c) 1998 Compuware Corporation. All Rights Reserved.
// Unpublished - rights reserved under the Copyright laws of the
// United States.
//
// U.S. GOVERNMENT RIGHTS-Use, duplication, or disclosure by the
// U.S. Government is subject to restrictions as set forth in 
// Compuware Corporation license agreement and as provided in 
// DFARS 227.7202-1(a) and 227.7202-3(a) (1995), 
// DFARS 252.227-7013(c)(1)(ii)(OCT 1988), FAR 12.212(a) (1995), 
// FAR 52.227-19, or FAR 52.227-14 (ALT III), as applicable.  
// Compuware Corporation.
// 
// This product contains confidential information and trade secrets 
// of Compuware Corporation. Use, disclosure, or reproduction is 
// prohibited without the prior express written permission of Compuware 
// Corporation.
//
//=============================================================================

// This file is for APPLICATIONS, not drivers. It implements two classes:

//
// CDeviceInterfaceClass - this class wraps the call to SetupDiGetClassDevs
// CDeviceInterface - this class wraps the calls to SetupDiEnumDeviceInterfaces and
//                    SetupDiGetInterfaceDeviceDetail
//

// Here is an example:
/*
	static GUID TestGuid = 
	{ 0x4747be20, 0x62ce, 0x11cf, { 0xd6, 0xa5, 0x28, 0xdb, 0x04, 0xc1, 0x00, 0x00 } };

	HANDLE OpenByInterface(GUID* pClassGuid, DWORD instance, PDWORD pError)
	{
		CDeviceInterfaceClass DevClass(pClassGuid, pError);
	
		if (*pError != ERROR_SUCCESS)
			return INVALID_HANDLE_VALUE;
	
		CDeviceInterface DevInterface(&DevClass, instance, pError);
	
		if (*pError != ERROR_SUCCESS)
			return INVALID_HANDLE_VALUE;
	
		cout << "The device path is " << DevInterface.DevicePath() << endl;
	
		HANDLE hDev;
		
		hDev = CreateFile(
			DevInterface.DevicePath(),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
	
		if (hDev == INVALID_HANDLE_VALUE)
			*pError = GetLastError();
	
		return hDev;
	}
*/


#include <setupapi.h>
#include <tchar.h>	// Note UNICODE may not work for Win98??

#define DEVINTF_INLINE inline

//////////////////////////////////////////////////////////////////////////////
// class CDeviceInterfaceClass
//
class CDeviceInterfaceClass
{
public:
	CDeviceInterfaceClass(GUID* pClassGuid, PDWORD status);
	~CDeviceInterfaceClass(void);
	GUID* GetGuid(void)      { return &m_Guid; }
	HDEVINFO GetHandle(void) { return m_hInfo; }

protected:
	HDEVINFO		m_hInfo;
	GUID			m_Guid;
};

//////////////////////////////////////////////////////////////////////////////
// class CDeviceInterface
//
class CDeviceInterface
{
public:
	DEVINTF_INLINE CDeviceInterface( CDeviceInterfaceClass* pClassObject, DWORD Index, PDWORD Error );
	DEVINTF_INLINE ~CDeviceInterface(void);
	DEVINTF_INLINE TCHAR* DevicePath(void);

protected:
	CDeviceInterfaceClass*				m_Class;
	SP_DEVICE_INTERFACE_DATA			m_Data;
	PSP_INTERFACE_DEVICE_DETAIL_DATA	m_Detail;
};


//////////////////////////////////////////////////////////////////////////////
// CDeviceInterfaceClass constructor
//
DEVINTF_INLINE CDeviceInterfaceClass::CDeviceInterfaceClass(
	GUID* pClassGuid, 
	PDWORD status
	) 
{
	DWORD flags = DIGCF_DEVICEINTERFACE | DIGCF_PRESENT ;
	m_hInfo = INVALID_HANDLE_VALUE;
	ZeroMemory(&m_Guid,sizeof(GUID));

	try
	{
		*status = ERROR_INVALID_PARAMETER;
		m_Guid = *pClassGuid;
		m_hInfo = SetupDiGetClassDevs(pClassGuid, NULL, NULL, flags);

		if ( m_hInfo == INVALID_HANDLE_VALUE )
			*status = GetLastError();
		else
			*status = ERROR_SUCCESS;

	}
	catch (...)
	{
		m_hInfo = INVALID_HANDLE_VALUE;
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterfaceClass destructor
//
DEVINTF_INLINE CDeviceInterfaceClass::~CDeviceInterfaceClass(void)
{
	if ( m_hInfo != INVALID_HANDLE_VALUE )
		SetupDiDestroyDeviceInfoList(m_hInfo);

	m_hInfo = INVALID_HANDLE_VALUE;
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterface constructor
//
DEVINTF_INLINE CDeviceInterface::CDeviceInterface(
	CDeviceInterfaceClass* pClassObject, 
	DWORD Index,
	PDWORD Error
	)
{
	m_Class = pClassObject;

	BOOL status;
	DWORD ReqLen;
	
	m_Detail = NULL;
	m_Data.cbSize = sizeof SP_DEVICE_INTERFACE_DATA;

	try
	{
		*Error = ERROR_INVALID_PARAMETER;

		status = SetupDiEnumDeviceInterfaces(
			m_Class->GetHandle(), 
			NULL, 
			m_Class->GetGuid(), 
			Index, 
			&m_Data
			);

		if ( !status )
		{
			*Error = GetLastError();
			return;
		}					  

		SetupDiGetInterfaceDeviceDetail (
			m_Class->GetHandle(),
			&m_Data,
			NULL,
			0,
			&ReqLen,
			NULL 
			);

		*Error = GetLastError();

		if ( *Error != ERROR_INSUFFICIENT_BUFFER )
			return;

		m_Detail = PSP_INTERFACE_DEVICE_DETAIL_DATA(new char[ReqLen]);

		if ( !m_Detail )
		{
			*Error = ERROR_NOT_ENOUGH_MEMORY;
			return;
		}

		m_Detail->cbSize = sizeof SP_INTERFACE_DEVICE_DETAIL_DATA;

		status = SetupDiGetInterfaceDeviceDetail (
			m_Class->GetHandle(),
			&m_Data,
			m_Detail,
			ReqLen,
			&ReqLen,
			NULL 
			);

		if ( !status )
		{
			*Error = GetLastError();
			delete m_Detail;
			m_Detail = NULL;
			return;
		}
	
		*Error = ERROR_SUCCESS;
	}
	catch (...)
	{
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterface destructor
//
DEVINTF_INLINE CDeviceInterface::~CDeviceInterface(void)
{
	if (m_Detail)
	{
		delete m_Detail;
		m_Detail = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////
// CDeviceInterface::DevicePath
//
DEVINTF_INLINE TCHAR* CDeviceInterface::DevicePath(void)
{
	try
	{
		if ( m_Detail)
			return m_Detail->DevicePath;
		else
			return NULL;
	}
	catch (...)
	{
		return NULL;
	}

}

