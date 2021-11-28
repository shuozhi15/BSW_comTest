


/*
 **************************************************************************************************
 * Includes
 **************************************************************************************************
 */

#include "EcuM.h" /*BSW_HeaderInc_002*/

#include "EcuM_Callout.h"
#include "EcuM_RunTime.h" /*For run time measurement*/

/*Extern module Headers*/
#if(ECUM_CFG_MULTICORE_ENABLED == STD_ON)
#include "Os.h" /* required for Core Id Check */
#if (!defined(OS_AR_RELEASE_MAJOR_VERSION) || (OS_AR_RELEASE_MAJOR_VERSION != 4))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(OS_AR_RELEASE_MINOR_VERSION) || (OS_AR_RELEASE_MINOR_VERSION != 2))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif/*ECUM_CFG_MULTICORE_ENABLED */


    
 #if (ECUM_INCLUDE_DET == STD_ON)
    #include "Det.h"
 #endif

 #include "Dem.h"

 #include "Mcu.h"

 #include "McuFunc.h"

 #include "Port.h"

 #include "Gpt.h"

 #include "Wdg_17_Scu.h"


#include "EcuM_Cfg_Startup.h"

/*EcuM Private headers*/
#include "EcuM_Prv.h"

#if ECUM_STARTUP_DURATION == TRUE /*will activate the Run time measurement*/

#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_Cfg_MemMap.h"

/*To store the start and End time EcuM_AL_DriverInitZero, EcuM_AL_DriverInitOne and EcuM_DeterminePbConfiguration
 * respectively*/

static VAR(EcuM_TimeTicks_tu64, ECUM_VAR_CLEARED_UNSPECIFIED)   EcuM_DriverInitZero_begin_ticks_u64;
static VAR(EcuM_TimeTicks_tu64, ECUM_VAR_CLEARED_UNSPECIFIED)   EcuM_DriverInitZero_end_ticks_u64;

            
            
static VAR(EcuM_TimeTicks_tu64, ECUM_VAR_CLEARED_UNSPECIFIED)   EcuM_DriverInitOne_begin_ticks_u64;
static VAR(EcuM_TimeTicks_tu64, ECUM_VAR_CLEARED_UNSPECIFIED)   EcuM_DriverInitOne_end_ticks_u64;
VAR(EcuM_TimeTicks_tu64, ECUM_VAR_CLEARED_UNSPECIFIED)   EcuM_DeterminePbConfiguration_begin_ticks_u64;
VAR(EcuM_TimeTicks_tu64, ECUM_VAR_CLEARED_UNSPECIFIED)   EcuM_DeterminePbConfiguration_end_ticks_u64;

#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "EcuM_Cfg_MemMap.h"


#define ECUM_START_SEC_VAR_CLEARED_32
#include "EcuM_Cfg_MemMap.h"
/*To store the Duration of EcuM_AL_DriverInitZero, EcuM_AL_DriverInitOne and EcuM_DeterminePbConfiguration
 * respectively*/

static VAR(uint32, ECUM_VAR_CLEARED_32)              EcuM_DriverInitZero_delta_us_u32;  

            
            
static VAR(uint32, ECUM_VAR_CLEARED_32)             EcuM_DriverInitOne_delta_us_u32;
static VAR(uint32, ECUM_VAR_CLEARED_32)              EcuM_DriverInitOne_LastReset_begin_us_u32;
static VAR(uint32, ECUM_VAR_CLEARED_32)              EcuM_DeterminePbConfiguration_delta_us_u32;

static VAR(uint32, ECUM_VAR_CLEARED_32)              EcuM_DriverInitZero_LastReset_begin_us_u32;

static VAR(uint32, ECUM_VAR_CLEARED_32)              EcuM_DeterminePbConfiguration_LastReset_begin_us_u32;

#define ECUM_STOP_SEC_VAR_CLEARED_32
#include "EcuM_Cfg_MemMap.h"

#endif /*ECUM_STARTUP_DURATION*/

/*************************************************************************************************
                            CALLOUTS
*************************************************************************************************/
#define ECUM_START_SEC_CALLOUT_CODE
#include "EcuM_Cfg_MemMap.h"
/************************************************************************************************
  Function name     :   EcuM_AL_DriverInitZero
  Description       :   This callout shall provide driver initialization and other hardware-related
                        startup activities for loading the post-build configuration data. Beware:
                        Here only pre-compile and link-time configurable modules may be used.
  Parameter (in)    :   None
  Parameter (inout) :   None
  Parameter (out)   :   None
  Return value      :   None
  Remarks           :
************************************************************************************************/

FUNC(void , ECUM_CODE) EcuM_AL_DriverInitZero(void)
{
#if ECUM_STARTUP_DURATION == TRUE /*will activate the Run time measurement*/
    EcuM_DriverInitZero_begin_ticks_u64    =   EcuM_GetTicks();
#endif /*ECUM_STARTUP_DURATION*/

    
            #if (ECUM_INCLUDE_DET == STD_ON)
                    (void)Det_Init();
                
            #endif
                    (void)Dem_PreInit();

#if ECUM_STARTUP_DURATION == TRUE /*will activate the Run time measurement*/
    EcuM_DriverInitZero_end_ticks_u64    =   EcuM_GetTicks();
#endif /*ECUM_STARTUP_DURATION*/
}

/**********************************************************************************
  Function name     :   EcuM_AL_DriverInitOne
  Description       :   This callout shall provide driver initialization and other
                        hardware-related startup activities in case of a power on reset.
  Parameter (in)    :   ConfigPtr
  Parameter (inout) :   None
 ***********************************************************************************/

FUNC(void , ECUM_CODE) EcuM_AL_DriverInitOne( P2CONST (EcuM_ConfigType, AUTOMATIC, ECUM_APPL_CONST) ConfigPtr )
{
    

    
            
    #if ECUM_STARTUP_DURATION == TRUE /*will activate the Run time measurement*/
    EcuM_DriverInitOne_begin_ticks_u64    =   EcuM_GetTicks();
#endif /*ECUM_STARTUP_DURATION*/
        
        
                    (void)Mcu_Init(ConfigPtr->ModuleInitPtrPB.McuInitConfigPtr_cpst);
                    (void)McuFunc_InitializeClock();
                    (void)Port_Init(ConfigPtr->ModuleInitPtrPB.PortInitConfigPtr_cpst);
                    (void)Gpt_Init(ConfigPtr->ModuleInitPtrPB.GptInitConfigPtr_cpst);
                    (void)Wdg_17_Scu_Init(ConfigPtr->ModuleInitPtrPB.Wdg_17_ScuInitConfigPtr_cpst);
            
        

      
#if ECUM_STARTUP_DURATION == TRUE /*will activate the Run time measurement*/
 
    EcuM_DriverInitOne_end_ticks_u64    =   EcuM_GetTicks();

    EcuM_DriverInitZero_delta_us_u32 = EcuM_GetDuration(EcuM_DriverInitZero_end_ticks_u64,EcuM_DriverInitZero_begin_ticks_u64);
    EcuM_DriverInitZero_LastReset_begin_us_u32 = EcuM_GetTicksSinceLastReset(EcuM_DriverInitZero_begin_ticks_u64);
    EcuM_DriverInitOne_delta_us_u32 = EcuM_GetDuration(EcuM_DriverInitOne_end_ticks_u64,EcuM_DriverInitOne_begin_ticks_u64);
    EcuM_DriverInitOne_LastReset_begin_us_u32 = EcuM_GetTicksSinceLastReset(EcuM_DriverInitOne_begin_ticks_u64);
    EcuM_DeterminePbConfiguration_delta_us_u32 = EcuM_GetDuration(EcuM_DeterminePbConfiguration_end_ticks_u64,EcuM_DeterminePbConfiguration_begin_ticks_u64);
    EcuM_DeterminePbConfiguration_LastReset_begin_us_u32 = EcuM_GetTicksSinceLastReset(EcuM_DeterminePbConfiguration_begin_ticks_u64);
        

#endif /*ECUM_STARTUP_DURATION*/
     

            

}

#if ECUM_SLEEP_SUPPORT_ENABLE
/**********************************************************************************
  Function name     :   EcuM_AL_DriverRestart
  Description       :   This callout shall provide driver re initialization and other
                        hardware-related startup activities in Wakeup Case.
  Parameter (in)    :   ConfigPtr
  Parameter (inout) :   None
 ***********************************************************************************/

FUNC(void , ECUM_CODE) EcuM_AL_DriverRestart( P2CONST (EcuM_ConfigType, AUTOMATIC, ECUM_APPL_CONST) ConfigPtr )
{
}
#endif
#define ECUM_STOP_SEC_CALLOUT_CODE
#include "EcuM_Cfg_MemMap.h"

