

 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanNm / AR42.6.0.0             Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1               Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 </VersionHead>*/


 /*
 ******************************************************************************
 * Includes
 ******************************************************************************
 */

#include "CanNm_Prv.h"
#include "CanNm_PBcfg.h"



#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"

    
/* Configuration structure of CanNm channels */
#if (defined(BSWSIM) && (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_POSTBUILD_SELECTABLE))
VAR(CanNm_ChannelConfigType, CANNM_CONST) CanNm_ChannelConfigData_cs[CANNM_NUMBER_OF_CHANNELS]=
#else
CONST(CanNm_ChannelConfigType, CANNM_CONST) CanNm_ChannelConfigData_cs[CANNM_NUMBER_OF_CHANNELS]=
#endif
{
/*CanNmChannelConfig_NmConfig_0_CanNmCluster_0*/
    {
        10000,   /* Message cycle time         */
        0, /* Message cycle offset time  */
        10000,  /* Message timeout time       */
        0,    /* Message reduced time       */
        10000,   /* Immediate Nm transmission cycle time         */
        20000,    /* Network Timeout for NM-Messages       */        
        0,    /* Timeout for Repeat Message State       */
        0,    /* Timeout for Remote Sleep Indication       */
        10000,    /* Timeout for bus calm down phase       */
        2,   /* CanIf transmit PDU Id      */
#if (CANNM_COM_USER_DATA_SUPPORT != STD_OFF)
        0,   /* PduR Handle Id for TxConfirmation   */
#endif
        0,   /* Handle of the ComM channel     */
        8,   /* Pdu Length   */
        CANNM_PDU_BYTE_0,  /* Node Id Position     */
        CANNM_PDU_BYTE_1,  /* Control Bit Vector position      */
        FALSE,  /* Node detection enabled      */
        FALSE,  /* Node ID enabled      */
        FALSE,  /* Repeat Message indication enabled      */
        0x1, /* CanNm Node Id              */
        6,  /* User Data Length     */
        0,    /* number of Immediate Nm Pdus     */
#if (CANNM_ERACALC_ENABLED != STD_OFF)
        0,    /* Pdu id of ERA referred pdu in Com */
        0,                                        /* channel index of the channel in ERA array */
        FALSE,                /* ERA status for each channel */
#endif
        TRUE,    /* Configure whether channel has to be initialized */
        FALSE,    /* Enable/Disable bus load reduction */
#if (CANNM_PN_ENABLED != STD_OFF)
        FALSE,            /* PN enabled for the channel ? */
        FALSE,        /* Keep awake on any message */
        FALSE,        /* Enable/Disable pn multiple network request handle */
#endif
       
#if (CANNM_CAR_WAKEUP_RX_ENABLED!= STD_OFF)        
        FALSE,
#if (CANNM_CAR_WAKEUP_FILTER_ENABLED!= STD_OFF)         
        FALSE, 
        0,
#endif        
        0,
        0,        
#endif      

        FALSE         /* Enables/Disables the handling of the Active Wakeup Bit */
    }
};


#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"






#if ((CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)||(CANNM_PN_ENABLED != STD_OFF))
#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"


/* CanNm global configuration structure */
CONST(CanNm_GlobalConfigType, CANNM_CONST) CanNm_GlobalConfigData_cs =
{
#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
        CanNm_ChannelConfigData_cs
#endif
#if (CANNM_PN_ENABLED != STD_OFF)
        CanNm_PnFilterMask
#endif
#if (CANNM_EIRACALC_ENABLED != STD_OFF)|| (CANNM_ERACALC_ENABLED != STD_OFF)
        ,CanNm_PNInfoTable
        ,0
#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
        ,CANNM_NUMBER_OF_PNS
#endif
#endif
};


#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
#endif

#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
static CONST(Std_VersionInfoType, CANNM_CONST)  CanNm_VersionInfo =
{
    6,              // Vendor Id
    31,              // Module Id
    6,       // Sw Major Version
    0,       // Sw Minor Version
    0        // Sw Patch Version
};
#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
#endif

#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)
#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"


/* Base configuration structure */
CONST(CanNm_ConfigType, CANNM_CONST) CanNmGlobalConfig = {
    /* Void pointer initialised with pointer to an instance of structure CanNm_GlobalConfigType */
    /* MR12 DIR 1.1 VIOLATION: 'pointer-to-void' is being cast to a 'pointer-to-object' type and this conversion will not result in any misalignment */
    &CanNm_GlobalConfigData_cs,
    &CanNm_VersionInfo
};


#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
#endif
