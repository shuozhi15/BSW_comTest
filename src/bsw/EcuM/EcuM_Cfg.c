

/*
***************************************************************************************************
*                                   Includes
***************************************************************************************************
*/
#include "EcuM.h" /*BSW_HeaderInc_002*/

#include "Mcu.h"
#if (!defined(MCU_AR_RELEASE_MAJOR_VERSION) || (MCU_AR_RELEASE_MAJOR_VERSION != 4))
	#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(MCU_AR_RELEASE_MINOR_VERSION) || (MCU_AR_RELEASE_MINOR_VERSION != 2))
	#error "AUTOSAR minor version undefined or mismatched"
#endif
#include "ComM.h"
#if (!defined(COMM_AR_RELEASE_MAJOR_VERSION) || (COMM_AR_RELEASE_MAJOR_VERSION != 4))
	#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COMM_AR_RELEASE_MINOR_VERSION) || (COMM_AR_RELEASE_MINOR_VERSION != 2))
	#error "AUTOSAR minor version undefined or mismatched"
#endif
/****************************************************************************************************
 *                                   CONFIGURATION PARAMETERS
 ***************************************************************************************************/



#define ECUM_START_SEC_CONST_8
#include "EcuM_Cfg_MemMap.h"


#define ECUM_STOP_SEC_CONST_8
#include "EcuM_Cfg_MemMap.h"

#define ECUM_START_SEC_CONST_UNSPECIFIED
#include "EcuM_Cfg_MemMap.h"

CONST( EcuM_Cfg_dataWkupPNCRefStruct_tst,  ECUM_CONST)  EcuM_Cfg_dataWkupPNCRef_cast[ECUM_NUM_OF_WKP_SOURCES]=
{
    { 0      ,   NULL_PTR },
    { 0      ,   NULL_PTR },
    { 0      ,   NULL_PTR },
    { 0      ,   NULL_PTR },
    { 0      ,   NULL_PTR }
};


/*The structure remains same as EcuM3.x implementation. ResetReason has to be acquired from Mcu configuration*/
/*************************************************************************************************
Container Name    : EcuMWakeupSource
Description       : Contains all the Pre Compile Parameters related to WakeupSource container.
****************************************************************************************************/
CONST( EcuM_Cfg_dataWkupSrcStruct_tst,  ECUM_CONST)  EcuM_Cfg_idxWakeupSourcesPC_au32[ECUM_NUM_OF_WKP_SOURCES] =
{/*IsComChannelPresent,       ComChannelReferance,                     ResetReason,             ValidationTimeout,   IsWakeupSourcePolling,                WakeupSourceId,                     CheckWakeupTimeout                  */
{      FALSE          ,       255                                   ,  MCU_RESET_UNDEFINED   ,         0         ,        FALSE  ,              EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_POWER,               
    0             },
{      FALSE          ,       255                                   ,  MCU_RESET_UNDEFINED   ,         0         ,        FALSE  ,              EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_RESET,               
    0             },
{      FALSE          ,       255                                   ,  MCU_RESET_UNDEFINED   ,         0         ,        FALSE  ,              EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_RESET,               
    0             },
{      FALSE          ,       255                                   ,  MCU_RESET_UNDEFINED   ,         0         ,        FALSE  ,              EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_WDG,               
    0             },
{      FALSE          ,       255                                   ,  MCU_RESET_UNDEFINED   ,         0         ,        FALSE  ,              EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_EXTERNAL_WDG,               
    0             }
};

#if ECUM_SLEEP_SUPPORT_ENABLE
/*********************************************************************************************************************
Container Name    : EcuMSleepMode
Description       : Contains all the Pre Compile Parameters related to SleepModes container
*********************************************************************************************************************/

CONST( EcuM_Cfg_dataSleepModeStruct_tst,  ECUM_CONST)  EcuM_Cfg_idxSleepModes_au32[ECUM_CFG_NUM_SLEEP_MODES]=
{/*    IsCPUSuspend   ,   EquivalentMcuMode  ,     WakeupSourceMask */

};
#endif//ECUM_SLEEP_SUPPORT_ENABLE


#define ECUM_STOP_SEC_CONST_UNSPECIFIED
#include "EcuM_Cfg_MemMap.h"


#if ECUM_SLEEP_SUPPORT_ENABLE

#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_Cfg_MemMap.h"
/*The array of structure containing a list of wakeupsources configured with checkwakeuptimeout and flag to indicate status of wakeup check*/
/*************************************************************************************************
Container Name    : EcuMWakeupSource
Description       : Contains all the Pre Compile Parameters related to WakeupSource container.
****************************************************************************************************/
volatile VAR( EcuM_Cfg_CheckWakeupSrc_tst,  ECUM_VAR_CLEARED_UNSPECIFIED)  EcuM_Cfg_CheckWakeupTimer_ast[ECUM_NUM_OF_WKP_SOURCES] =
{/* WakeupSourceId,                                 CheckWakeupTimeout,               CheckWakeupStatus                  */

{  EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_POWER,                0 ,      ECUM_FLAG_RESET_E},
{  EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_RESET,                0 ,      ECUM_FLAG_RESET_E},
{  EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_RESET,                0 ,      ECUM_FLAG_RESET_E},
{  EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_INTERNAL_WDG,                0 ,      ECUM_FLAG_RESET_E},
{  EcuMConf_EcuMWakeupSource_ECUM_WKSOURCE_EXTERNAL_WDG,                0 ,      ECUM_FLAG_RESET_E}
};
#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_Cfg_MemMap.h"

#endif//ECUM_SLEEP_SUPPORT_ENABLE

#define ECUM_START_SEC_CONST_16
#include "EcuM_Cfg_MemMap.h"

/*************************************************************************************************
Container Name    : EcuMGoDownAllowedUsers
Description       : Contains Module ID's configured for calling EcuM_GoDown
****************************************************************************************************/
CONST( uint16,  ECUM_CONST)  EcuM_Cfg_idxGoDownValidCallerArr_au16[ECUM_CFG_GODOWN_CALLER_ARR_LEN]  =
{
    42
};





#define ECUM_STOP_SEC_CONST_16
#include "EcuM_Cfg_MemMap.h"

