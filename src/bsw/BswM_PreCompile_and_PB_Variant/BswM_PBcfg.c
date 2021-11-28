/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "BswM.h"

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */

#define BSWM_CFG_VERSION_INFO  {    /*Version details*/  \
                    6U, /*Vendor Id*/  \
                    42U, /*Module Id*/  \
                    4U, /*SW Major version*/  \
                    0U, /*SW Minor version*/  \
                    0U /*SW Patch version*/  \
               }

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

#define BSWM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED 
#include "BswM_Cfg_MemMap.h"

/* ModeArbitration Type structure  
 * - Number of rules configured for the chosen variant 
 * - Pointer to base address of array of rules (BswM_Cfg_RuleType_tst)
 */
#define BSWM_CFG_MODEARBITRATION {      \
        0, \
        NULL_PTR   \
}

/* ModeControl Type structure  
 * - Pointer to base address of array of actionlists (BswM_Cfg_ActionListType_tst) Currently NULL_PTR
 */
#define BSWM_CFG_MODECONTROL {      \
        NULL_PTR \
}

/* BswM_ConfigType  :
 * - ModeArbitration structure (BswM_Cfg_ModeArbitrationType_tst)
 * - ModeControl structure (BswM_Cfg_ModeControlType_tst)
 * - Version info to compare during module initialization
 */
CONST(BswM_ConfigType, BSWM_CONST) BswM_Config =
{
	BSWM_CFG_MODEARBITRATION,
	BSWM_CFG_MODECONTROL,
	BSWM_CFG_VERSION_INFO
};

#define BSWM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED 
#include "BswM_Cfg_MemMap.h"

#define BSWM_START_SEC_CONFIG_DATA_POSTBUILD_32
#include "BswM_Cfg_MemMap.h"

CONSTP2CONST(BswM_ConfigType,AUTOMATIC,BSWM_CONST) BswM_Configurations_capcst[BSWM_NO_OF_CONFIGSETS]=
{
	&BswM_Config
};

#define BSWM_STOP_SEC_CONFIG_DATA_POSTBUILD_32
#include "BswM_Cfg_MemMap.h"
/**********************************************************************************************************************
 *																										
 **********************************************************************************************************************/
