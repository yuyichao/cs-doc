This README file is generated automatically by DriverWizard

To complete the driver, follow these steps:

 o  Build the driver
		Build | Build ds7.sys

 o  Search for the string "TODO" and follow the instructions to complete your driver.

 o  Review the registry settings created in ds7.inf.

The Wizard created the following files:

Files that comprise your driver:
  readme.txt
  	Contains information shown here.
  sys\Ds7.cpp
  	Driver class implementation.
  sys\Ds7.h
  	Driver class header file.
  sys\Ds7.inf
  	INF file defines driver for plug and play installation.
  ds7ioctl.h
  	Definition of control codes
  Ds7DeviceInterface.h
	Header file containing the GUID for the device interface.
  sys\Ds7Device.cpp
  	Device (Ds7Device) implementation.
  sys\Ds7Device.h
  	Device (Ds7Device) header file.
  sys\function.h
  	Used by DriverWorks library to determine which
	handlers to provide.
  sys\Ds7.rc
  	Shell for resource file (used for event messages,
	version resource)

Files used by build utilities:
  sys\sources
  	Used by BUILD program to determine what files
	comprise your driver.
  sys\makefile
  	Used by BUILD program to build your driver.

Files used by the test application:
  exe\Test_ds7.cpp
  	Console application with driver interface
  exe\OpenByIntf.cpp
	Used to open the device using a GUID interface.
  exe\sources
  	Used by BUILD program to determine what files
	comprise your test application.
  exe\makefile
  	Used by BUILD program to build your test application.
