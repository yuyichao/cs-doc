/*++

Copyright (c) 1989-1995  Microsoft Corporation

Module Name:

    cfg.h

Abstract:

    This module contains the common Configuration Manager definitions for
    both user mode and kernel mode code.

Author:

    Paula Tomlinson (paulat) 06/19/1995

Revision History:

--*/

#ifndef _CFG_INCLUDED_
#define _CFG_INCLUDED_

//
// The following definitions are also used by kernel mode code to
// set up the registry.
//

//
// DevInst problem values, returned by call to CM_Get_DevInst_Status
//
#define CM_PROB_NOT_CONFIGURED         (0x00000001)   // no config for device
#define CM_PROB_DEVLOADER_FAILED       (0x00000002)   // service load failed
#define CM_PROB_OUT_OF_MEMORY          (0x00000003)   // out of memory
#define CM_PROB_ENTRY_IS_WRONG_TYPE    (0x00000004)   // WINDOWS 95 ONLY
#define CM_PROB_LACKED_ARBITRATOR      (0x00000005)   // WINDOWS 95 ONLY
#define CM_PROB_BOOT_CONFIG_CONFLICT   (0x00000006)   // boot config conflict
#define CM_PROB_FAILED_FILTER          (0x00000007)   // WINDOWS 95 ONLY
#define CM_PROB_DEVLOADER_NOT_FOUND    (0x00000008)   // Devloader not found
#define CM_PROB_INVALID_DATA           (0x00000009)   // WINDOWS 95 ONLY
#define CM_PROB_FAILED_START           (0x0000000A)   // WINDOWS 95 ONLY
#define CM_PROB_LIAR                   (0x0000000B)   // ???
#define CM_PROB_NORMAL_CONFLICT        (0x0000000C)   // config conflict
#define CM_PROB_NOT_VERIFIED           (0x0000000D)   // WINDOWS 95 ONLY
#define CM_PROB_NEED_RESTART           (0x0000000E)   // requires restart
#define CM_PROB_REENUMERATION          (0x0000000F)   // WINDOWS 95 ONLY
#define CM_PROB_PARTIAL_LOG_CONF       (0x00000010)   // WINDOWS 95 ONLY
#define CM_PROB_UNKNOWN_RESOURCE       (0x00000011)   // unknown res type
#define CM_PROB_REINSTALL              (0x00000012)   // WINDOWS 95 ONLY
#define CM_PROB_REGISTRY               (0x00000013)   // WINDOWS 95 ONLY
#define CM_PROB_VXDLDR                 (0x00000014)   // WINDOWS 95 ONLY
#define CM_PROB_WILL_BE_REMOVED        (0x00000015)   // devinst will remove
#define CM_PROB_DISABLED               (0x00000016)   // devinst is disabled
#define CM_PROB_DEVLOADER_NOT_READY    (0x00000017)   // Devloader not ready
#define CM_PROB_DEVICE_NOT_THERE       (0x00000018)   // device doesn't exist
#define CM_PROB_MOVED                  (0x00000019)   // WINDOWS 95 ONLY
#define CM_PROB_TOO_EARLY              (0x0000001A)   // WINDOWS 95 ONLY
#define CM_PROB_NO_VALID_LOG_CONF      (0x0000001B)   // no valid log config
#define CM_PROB_FAILED_INSTALL         (0x0000001C)   // install failed
#define CM_PROB_HARDWARE_DISABLED      (0x0000001D)   // device disabled
#define CM_PROB_CANT_SHARE_IRQ         (0x0000001E)   // can't share IRQ
#define NUM_CM_PROB                    (0x0000001F)

//
// Configuration Manager Global State Flags (returned by CM_Get_Global_State)
//
#define CM_GLOBAL_STATE_CAN_DO_UI            (0x00000001) // Can  do UI?
#define CM_GLOBAL_STATE_ON_BIG_STACK         (0x00000002) // WINDOWS 95 ONLY
#define CM_GLOBAL_STATE_SERVICES_AVAILABLE   (0x00000004) // CM APIs available?
#define CM_GLOBAL_STATE_SHUTTING_DOWN        (0x00000008) // CM shutting down
#define CM_GLOBAL_STATE_DETECTION_PENDING    (0x00000010) // detection pending

//
// Device Instance status flags, returned by call to CM_Get_DevInst_Status
//
#define DN_ROOT_ENUMERATED (0x00000001) // Was enumerated by ROOT
#define DN_DRIVER_LOADED   (0x00000002) // Has Register_Device_Driver
#define DN_ENUM_LOADED     (0x00000004) // Has Register_Enumerator
#define DN_STARTED         (0x00000008) // Is currently configured
#define DN_MANUAL          (0x00000010) // Manually installed
#define DN_NEED_TO_ENUM    (0x00000020) // May need reenumeration
#define DN_NOT_FIRST_TIME  (0x00000040) // Has received a config
#define DN_HARDWARE_ENUM   (0x00000080) // Enum generates hardware ID
#define DN_LIAR            (0x00000100) // Lied about can reconfig once
#define DN_HAS_MARK        (0x00000200) // Not CM_Create_DevInst lately
#define DN_HAS_PROBLEM     (0x00000400) // Need device installer
#define DN_FILTERED        (0x00000800) // Is filtered
#define DN_MOVED           (0x00001000) // Has been moved
#define DN_DISABLEABLE     (0x00002000) // Can be rebalanced
#define DN_REMOVABLE       (0x00004000) // Can be removed
#define DN_PRIVATE_PROBLEM (0x00008000) // Has a private problem
#define DN_MF_PARENT       (0x00010000) // Multi function parent
#define DN_MF_CHILD        (0x00020000) // Multi function child
#define DN_WILL_BE_REMOVED (0x00040000) // DevInst is being removed

#endif // _CFG_INCLUDED_

