

#ifndef RBA_TRCV_H_
# define RBA_TRCV_H_

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComStack_Types.h"
#include "rba_Trcv_Cfg_Time.h"
#include "rba_Trcv_Cfg.h"

#ifdef RBA_TRCV_CFG_CONFIGURED

#if RBA_TRCV_CFG_DEM_ENABLED == STD_ON
# include "Dem.h"
#endif

#if (RBA_TRCV_DEV_ERROR_DETECT == STD_ON )
# include "Det.h"
#endif

#if RBA_TRCV_CFG_DIO_USED == STD_ON
# include "Dio.h"
# include "Port.h"
#endif

#if RBA_TRCV_CFG_SPI_USED == STD_ON
# include "Spi.h"
#endif

#if (RBA_TRCV_FR_USED == STD_ON)
# include "Fr_GeneralTypes.h"
# include "FrTrcv_Cfg_SchM.h"
#endif
#if (RBA_TRCV_CAN_USED == STD_ON)
# include "Can_GeneralTypes.h"
# include "CanTrcv_Cfg_SchM.h"
#endif
#if (RBA_TRCV_LIN_USED == STD_ON)
# include "Lin_GeneralTypes.h"
# include "LinTrcv_Cfg_SchM.h"
#endif


/*
 ***************************************************************************************************
 * defines
 ***************************************************************************************************
 */

#define RBA_TRCV_SW_MAJOR_VERSION   6
#define RBA_TRCV_SW_MINOR_VERSION   0
#define RBA_TRCV_SW_PATCH_VERSION   0

/* Expeted version of rba_Trcv generated files */
#define RBA_TRCV_PRV_EXPECTED_GEN_MAJOR_VERSION 6
#define RBA_TRCV_PRV_EXPECTED_GEN_MINOR_VERSION 0

/* suitable interface version to upper Trcv */
#define RBA_TRCV_PRV_SUITABLE_FRTRCV_MAJOR_VERSION  6
#define RBA_TRCV_PRV_SUITABLE_FRTRCV_MINOR_VERSION  0
#define RBA_TRCV_PRV_SUITABLE_CANTRCV_MAJOR_VERSION 6
#define RBA_TRCV_PRV_SUITABLE_CANTRCV_MINOR_VERSION 0
#define RBA_TRCV_PRV_SUITABLE_LINTRCV_MAJOR_VERSION 6
#define RBA_TRCV_PRV_SUITABLE_LINTRCV_MINOR_VERSION 0


#define RBA_TRCV_VENDOR_ID          6
#define RBA_TRCV_MODULE_ID          203                 

#define RBA_TRCV_FUNCTION_NR0 0u
#define RBA_TRCV_FUNCTION_NR1 1u
#define RBA_TRCV_FUNCTION_NR2 2u
#define RBA_TRCV_FUNCTION_NR3 3u
#define RBA_TRCV_FUNCTION_NR4 4u
#define RBA_TRCV_FUNCTION_NR5 5u

#define RBA_TRCV_CONNECTIONTYPE_UNUSED      0
#define RBA_TRCV_CONNECTIONTYPE_NOCONFIG    1
#define RBA_TRCV_CONNECTIONTYPE_DIO         2
#define RBA_TRCV_CONNECTIONTYPE_SPI         3

#define RBA_TRCV_VARIANT_PRECOMPILE     1
#define RBA_TRCV_VARIANT_LINKTIME       2
#define RBA_TRCV_VARIANT_POSTBUILD      3

#define RBA_TRCV_DIO_CLK_POS             0  /* Check if new transceiver are implemented */


// Autosar FrTrcv_BusErrorState:
// See also FRTRCV_ERRORBITPOS_E0 in FrTrcv.h
#define RBA_TRCV_ERRORBITPOS_E0 0u   /* BitE0: Global error. Any of the mandatory errors defined in this table,
                                    please see FrTrcv457, FrTrcv458 */
#define RBA_TRCV_ERRORBITPOS_E1 1u   /* BitE1: Short circuit between bus lines */
#define RBA_TRCV_ERRORBITPOS_E2 2u   /* BitE2: Short circuit between positive bus line and ground */
#define RBA_TRCV_ERRORBITPOS_E3 3u   /* BitE3: Short circuit between positive bus line and power supply */
#define RBA_TRCV_ERRORBITPOS_E4 4u   /* BitE4: Short circuit between negative  bus line and power supply */
#define RBA_TRCV_ERRORBITPOS_E5 5u   /* BitE5: Short circuit between negative bus line and ground */
#define RBA_TRCV_ERRORBITPOS_E6 6u   /* BitE6: Any bus fault, which cannot be resolved according to the description of
                                            bit 1...5 */
#define RBA_TRCV_ERRORBITPOS_E7 7u   /* BitE7: Under voltage of transceiver power supply */
#define RBA_TRCV_ERRORBITPOS_E8 8u   /* BitE8: FlexRay transceiver is permanently enabled */
#define RBA_TRCV_ERRORBITPOS_E9 9u   /* BitE9: Over temperature of transceiver */
// Vendor FrTrcv_BusErrorState:
#define RBA_TRCV_ERRORBITPOS_E10 10u   /* BitE10: TXEN_BGE CLAMPED / TXEN CLAMPED */
#define RBA_TRCV_ERRORBITPOS_E11 11u   /* BitE11: permanent Bit wrong -> No access */
#define RBA_TRCV_ERRORBITPOS_E12 12u   /* BitE12: TRXD Collition */
#define RBA_TRCV_ERRORBITPOS_E13 13u   /* BitE13: BGE CLAMPED */
#define RBA_TRCV_ERRORBITPOS_E14 14u   /* BitE14: BUS ERROR status bit */

// Vendor specific extension of statusbits
#define RBA_TRCV_STATUSBITPOS_S0  16u  /* S0(16) : 5PWON status bit set means PWON flag has been set previously
                                                (wakeup by PWN on) */
#define RBA_TRCV_STATUSBITPOS_S1  17u  /* S1(17) : LOCAL WAKEUP local wake-up source flag is redirected to this bit
                                                (wakeup by pin) */
#define RBA_TRCV_STATUSBITPOS_S2  18u  /* S2(18) : REMOTE WAKEUP remote wake-up source flag is redirected to this bit
                                                 (wakeup by bus) */
#define RBA_TRCV_STATUSBITPOS_S3  19u  /* S3(19) : Wakeup by reset */
#define RBA_TRCV_STATUSBITPOS_S4  20u  /* S4(20) : Wakeup internally (by functioncall goto normal) */
#define RBA_TRCV_STATUSBITPOS_S5  21u  /* S5(21) : NODE CONFIG node configuration flag is redirected to this bit */
#define RBA_TRCV_STATUSBITPOS_S6  22u  /* S6(22) : STAR LOCKED status bit is set if Star-locked mode has been entered
                                                previously */
#define RBA_TRCV_STATUSBITPOS_S7  23u  /* S7(23) : Normal Mode */
#define RBA_TRCV_STATUSBITPOS_S8  24u  /* S8(24) : TransmitterEnabledFlag */
#define RBA_TRCV_STATUSBITPOS_S15 31u  /* S15(31): Non relevant */

#if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
# define rba_Trcv_DemReportErrorStatus(ERROR_CODE, EvId) do{if((ERROR_CODE)!=0) \
                                                            {(void)Dem_ReportErrorStatus((ERROR_CODE),(EvId))};}while(0)
#else
# define rba_Trcv_DemReportErrorStatus(ERROR_CODE, EvId) do{}while(0)
#endif

#define RBA_TRCV_E_FR_UNINIT 0
#define RBA_TRCV_E_FR_INVALID_TRCVIDX 1

#if (RBA_TRCV_VARIANT == RBA_TRCV_VARIANT_PRECOMPILE)
# define RBA_TRCV_DIOARRAY              rba_Trcv_DioArray_ast
# define RBA_TRCV_SETMODESTRUCTCHAIN    rba_Trcv_SetModeStructChain
# define RBA_TRCV_SETMODESTRUCTCHAINPOS_AU8 rba_Trcv_SetModeStructChainPos_au8
# define RBA_TRCV_GETMODEPINCHAIN_AU8   rba_Trcv_GetModePinChain_au8
# define RBA_TRCV_GETMODEARRAY          rba_Trcv_GetModeArray
# define RBA_TRCV_GETERRORBITPOS_AU8    rba_Trcv_GetErrorBitPos_au8
# define RBA_TRCV_SPIARRAY_AST          rba_Trcv_SpiArray_ast
# define RBA_TRCV_ERRORBITSTRUCT        rba_Trcv_ErrorBitStruct
# define RBA_TRCV_ENTERCRITICAL         rba_Trcv_Entercritical
# define RBA_TRCV_LEAVECRITICAL         rba_Trcv_Leavecritical
# define RBA_TRCV_CONFIGDATA            rba_Trcv_ConfigData
# define RBA_TRCV_INDEX                 rba_Trcv_Index      // not used
# define RBA_TRCV_MODULEID              rba_Trcv_ModuleId   // not used
#else
# define RBA_TRCV_DIOARRAY              rba_Trcv_ConfigData_st->rba_Trcv_DioArray_ast
# define RBA_TRCV_SETMODESTRUCTCHAIN    rba_Trcv_ConfigData_st->rba_Trcv_SetModeStructChain
# define RBA_TRCV_SETMODESTRUCTCHAINPOS_AU8 rba_Trcv_ConfigData_st->rba_Trcv_SetModeStructChainPos_au8
# define RBA_TRCV_GETMODEPINCHAIN_AU8   rba_Trcv_ConfigData_st->rba_Trcv_GetModePinChain_au8
# define RBA_TRCV_GETMODEARRAY          rba_Trcv_ConfigData_st->rba_Trcv_GetModeArray
# define RBA_TRCV_GETERRORBITPOS_AU8    rba_Trcv_ConfigData_st->rba_Trcv_GetErrorBitPos_au8
# define RBA_TRCV_SPIARRAY_AST          rba_Trcv_ConfigData_st->rba_Trcv_SpiArray_ast
# define RBA_TRCV_ERRORBITSTRUCT        rba_Trcv_ConfigData_st->rba_Trcv_ErrorBitStruct
# define RBA_TRCV_ENTERCRITICAL         rba_Trcv_Entercritical
# define RBA_TRCV_LEAVECRITICAL         rba_Trcv_Leavecritical
# define RBA_TRCV_CONFIGDATA            rba_Trcv_ConfigData_st->rba_Trcv_ConfigData
# define RBA_TRCV_INDEX                 rba_Trcv_Index
# define RBA_TRCV_MODULEID              rba_Trcv_ModuleId
#endif

/* SID of rba have a continous number to access Bus-specific SID by table  rba_Trcv_DetSid */
enum
{
    RBA_TRCV_SID_INIT                       = 0x00,
    RBA_TRCV_SID_SETTRANSCEIVERMODE         = 0x01,
    RBA_TRCV_SID_GETTRANSCEIVERMODE         = 0x02,
    RBA_TRCV_SID_GETTRANSCEIVERWUREASON     = 0x03,
    RBA_TRCV_SID_GETVERSIONINFO             = 0x04,
    RBA_TRCV_SID_DISABLETRANCEIVERWAKEUP    = 0x05,
    RBA_TRCV_SID_ENABLETRANCEIVERWAKEUP     = 0x06,
    RBA_TRCV_SID_CLEARTRANCEIVERWAKEUP      = 0x07,
    RBA_TRCV_SID_CLEARTRANCEIVERWAKEUPREASON= 0x08,
    RBA_TRCV_SID_CHECKWAKEUPBYTRANSCEIVER   = 0x09,
    RBA_TRCV_SID_GETTRANSCEIVERERROR        = 0x0A,
    RBA_TRCV_SID_WAITUS                     = 0x10,
    RBA_TRCV_SID_GETERRORPIN                = 0x11
};

enum
{
    RBA_TRCV_E_INVALID_TRANSCEIVER        =0x01,
    RBA_TRCV_E_PARAM_POINTER              =0x02,
    RBA_TRCV_E_UNINIT                     =0x11,
    RBA_TRCV_E_TRCV_NOT_STANDBY           =0x21,
    RBA_TRCV_E_TRCV_NOT_NORMAL            =0x22,
    RBA_TRCV_E_PARAM_TRCV_WAKEUP_MODE     =0x23,
    RBA_TRCV_E_PARAM_TRCV_OPMODE          =0x24,
    RBA_TRCV_E_PARAM_TIMEOUT              =0x30,
    RBA_TRCV_E_SPI                        =0x31
};

/*
 ***************************************************************************************************
 * typedefs
 ***************************************************************************************************
 */

#if RBA_TRCV_CFG_DEM_ENABLED == STD_ON
typedef P2FUNC(void,           RBA_TRCV_APPL_CODE, rba_Trcv_Dem_ReportErrorStatusFunctionType)( Dem_EventIdType EventId, Dem_EventStatusType EventStatus );
#endif


#if RBA_TRCV_CFG_DEM_ENABLED == STD_ON
typedef struct
{
    rba_Trcv_Dem_ReportErrorStatusFunctionType Dem_ReportErrorStatusFunction;
    Dem_EventIdType                            EventId;
}rba_Trcv_DemStruct_tst;
#endif


/*
 ***************************************************************************************************
 * Variable declaration
 ***************************************************************************************************
 */

#if (RBA_TRCV_VARIANT == RBA_TRCV_VARIANT_POSTBUILD)
# ifdef RBA_TRCV_MEMMAP_USED
#  define RBA_TRCV_START_SEC_CONFIG_DATA_POSTBUILD_HEADER

#  include "rba_Trcv_MemMap.h"
# endif

extern P2CONST(rba_Trcv_ConfigType, AUTOMATIC,RBA_TRCV_CFG) rba_Trcv_ConfigData_st;

# ifdef RBA_TRCV_MEMMAP_USED
#  define RBA_TRCV_STOP_SEC_CONFIG_DATA_POSTBUILD_HEADER

#  include "rba_Trcv_MemMap.h"
# endif
#endif


#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_START_SEC_CONFIG_DATA_UNSPECIFIED

# include "rba_Trcv_MemMap.h"
#endif

# if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
extern CONST(rba_Trcv_DemStruct_tst,RBA_TRCV_CFG) rba_Trcv_DemStruct[];
#endif
extern CONST(rba_Trcv_IsrFctPtrType,RBA_TRCV_PBCFG) rba_Trcv_Entercritical[];
extern CONST(rba_Trcv_IsrFctPtrType,RBA_TRCV_PBCFG) rba_Trcv_Leavecritical[];
extern CONST(uint8,RBA_TRCV_CFG)    rba_Trcv_Index[];
extern CONST(uint8,RBA_TRCV_CFG)    rba_Trcv_ModuleId[];

#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_STOP_SEC_CONFIG_DATA_UNSPECIFIED

# include "rba_Trcv_MemMap.h"
#endif

#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_START_SEC_VAR_NO_INIT_UNSPECIFIED

# include "rba_Trcv_MemMap.h"
#endif

/* BSW_SWS_AR4_0_R2_FlexRayTransceiverDriver-4786, FrTrcv405      The mode (FrTrcv_TrcvModeType) of the FlexRay
   Transceiver shall be available for debugging.    */
extern VAR(rba_Trcv_TrcvModeType_ten, RBA_TRCV_VAR)  rba_Trcv_Prv_Mode_au8[];
extern VAR(boolean, RBA_TRCV_VAR)  rba_Trcv_Prv_InitState_b;
/* BSW_SWS_AR4_0_R2_FlexRayTransceiverDriver-4787, FrTrcv406        The wake up reason (FrTrcv_TrcvWUReasonType) of
    the FlexRay Transceiver shall be available for debugging */
extern VAR( rba_Trcv_TrcvWUReasonType_ten, RBA_TRCV_VAR) rba_Trcv_WakeupReason_au8[RBA_TRCV_CFG_NUMBER_OF_TRCV];
/* BSW_SWS_AR4_0_R2_FlexRayTransceiverDriver-4788, FrTrcv407:       The wake up state of the FlexRay Transceiver shall
   be available for debugging */
#if (RBA_TRCV_CFG_WAKEUP_BY_NODE_USED == STD_ON)
extern VAR( uint8, RBA_TRCV_VAR) rba_Trcv_WakeupState_au8[RBA_TRCV_CFG_NUMBER_OF_TRCV];
#endif
extern VAR( uint16, RBA_TRCV_VAR) rba_Trcv_GetTransceiverError_Reentrantflag;
extern VAR( uint16, RBA_TRCV_VAR) rba_Trcv_GetTransceiverError_Reentrantflag_Count;

extern VAR( rba_Trcv_Cfg_Tick_to,RBA_TRCV_VAR) rba_Trcv_StateChangeTime_ao[RBA_TRCV_CFG_NUMBER_OF_TRCV];

#if ( RBA_TRCV_CFG_SPI_USED == STD_ON )
extern uint16 rba_Trcv_SpiSeq1RxBuf_ao[RBA_TRCV_SPI_SEQ_MAX];
#endif


#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_STOP_SEC_VAR_NO_INIT_UNSPECIFIED

# include "rba_Trcv_MemMap.h"
#endif
/*
 ***************************************************************************************************
 * Function declaration
 ***************************************************************************************************
 */
#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_START_SEC_CODE

# include "rba_Trcv_MemMap.h"
#endif

#if(RBA_TRCV_VARIANT != RBA_TRCV_VARIANT_PRECOMPILE)
FUNC (void, RBA_TRCV_CODE) rba_Trcv_Init( uint8 rba_Trcv_TrcvIdx,const rba_Trcv_ConfigType  * rba_Trcv_Config_cpst);
#else
FUNC (void, RBA_TRCV_CODE) rba_Trcv_Init( uint8 rba_Trcv_TrcvIdx );
#endif

FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_SetTransceiverMode( uint8 rba_Trcv_TrcvIdx,
                                                rba_Trcv_TrcvModeType_ten rba_Trcv_TrcvMode);
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverMode( uint8 rba_Trcv_TrcvIdx,
                                                rba_Trcv_TrcvModeType_ten* rba_Trcv_TrcvModePtr );
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverWUReason( uint8 rba_Trcv_TrcvIdx,
                                                rba_Trcv_TrcvWUReasonType_ten* rba_Trcv_TrcvWUReasonPtr );
FUNC (void, RBA_TRCV_CODE) rba_Trcv_GetVersionInfo( Std_VersionInfoType* versioninfo );
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_ClearTransceiverWakeup(uint8 rba_Trcv_TrcvIdx );
#if (RBA_TRCV_CFG_CLEAR_WUPREASON_API_USED == STD_ON)
FUNC (void, RBA_TRCV_CODE) rba_Trcv_ClearTransceiverWuReason(uint8 rba_Trcv_TrcvIdx );
#endif
#if (RBA_TRCV_CFG_WAKEUP_BY_NODE_USED == STD_ON)
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_CheckWakeupByTransceiver( uint8 rba_Trcv_TrcvIdx );
#endif
#if (RBA_TRCV_FR_USED == STD_ON)
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverError( uint8 rba_Trcv_TrcvIdx,
                 uint8 rba_Trcv_BranchIdx, uint32* rba_Trcv_BusErrorState );
# if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_NR0_USED == STD_ON)
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverError_N0(uint8 rba_Trcv_TrcvIdx, uint32* rba_Trcv_BusErrorState);
# endif
#endif
#if (RBA_TRCV_FR_USED == STD_ON)
# if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_NR1_USED == STD_ON)
#  if ( RBA_TRCV_CFG_SPI_USED == STD_ON )
extern FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverError_N1(uint8 rba_Trcv_TrcvIdx, uint32* rba_Trcv_BusErrorState);
#  endif
# endif
#endif

FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_DisableTransceiverBranch( uint8 rba_Trcv_TrcvIdx,
                                                                    uint8 rba_Trcv_BranchIdx );
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_EnableTransceiverBranch( uint8 rba_Trcv_TrcvIdx,
                                                                   uint8 rba_Trcv_BranchIdx );

#if (RBA_TRCV_MODE_PROPOGATION_REPORTING == STD_ON)
FUNC (void, RBA_TRCV_CODE)  rba_Trcv_CheckModePropagation(uint8 rba_Trcv_TrcvIdx);
#endif


#if (RBA_TRCV_CHANGE_MODE_FUNCTION1_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_ChangeModeFunction1(uint8 rba_Trcv_TrcvIdx);
#endif
#if (RBA_TRCV_CHANGE_MODE_FUNCTION2_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_ChangeModeFunction2(uint8 rba_Trcv_TrcvIdx);
#endif
#if (RBA_TRCV_CHANGE_MODE_FUNCTION3_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_ChangeModeFunction3(uint8 rba_Trcv_TrcvIdx);
#endif
#if (RBA_TRCV_CHANGE_MODE_FUNCTION4_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_ChangeModeFunction4(uint8 rba_Trcv_TrcvIdx);
#endif
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_GetErrorPin(uint8 rba_Trcv_TrcvIdx, uint8 *rba_Trcv_Level);
FUNC(Std_ReturnType, RBA_TRCV_APPL_CODE) rba_Trcv_TestTiming(uint8 rba_Trcv_TrcvIdx );


#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_STOP_SEC_CODE

# include "rba_Trcv_MemMap.h"
#endif

#endif // RBA_TRCV_CFG_CONFIGURED
#endif  /* RBA_TRCV_H */
