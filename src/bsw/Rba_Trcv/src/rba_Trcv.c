

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */
#include "rba_Trcv.h"
/*
 ***************************************************************************************************
 * Exclusioncheck
 ***************************************************************************************************
 */
#ifdef RBA_TRCV_CFG_CONFIGURED
/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */
#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_START_SEC_CODE

# include "rba_Trcv_MemMap.h"
#endif

static FUNC (boolean, RBA_TRCV_CODE) rba_Trcv_CheckTimeElapsed(rba_Trcv_Cfg_Tick_to CurrentTime_o,
                                                  rba_Trcv_Cfg_Tick_to StartTime_o, rba_Trcv_Cfg_Tick_to TimePeriod_o);
static FUNC (void, RBA_TRCV_CODE) rba_Trcv_WaitUs(rba_Trcv_Cfg_Tick_to StartTime_o, rba_Trcv_Cfg_Tick_to TimePeriod_o);
#if RBA_TRCV_CFG_DEM_ENABLED == STD_ON
static FUNC (void, RBA_TRCV_CODE) rba_Trcv_Dem_ReportErrorStatusFunction(uint16 DemPos, Dem_EventStatusType rba_Trcv_Dem_EventStatus);
#endif

#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_STOP_SEC_CODE

# include "rba_Trcv_MemMap.h"
#endif
/*
 ***************************************************************************************************
 * Global Variables
 ***************************************************************************************************
 */
#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_START_SEC_VAR_NO_INIT_UNSPECIFIED

# include "rba_Trcv_MemMap.h"
#endif

VAR(boolean, RBA_TRCV_VAR)  rba_Trcv_Prv_InitState_b = FALSE;
VAR( rba_Trcv_TrcvModeType_ten, RBA_TRCV_VAR) rba_Trcv_Prv_Mode_au8[RBA_TRCV_CFG_NUMBER_OF_TRCV];

//  rba_Trcv_StateChangeTime_ao is set in SetTransceiverMode and rba_Trcv_GetTransceiverError
VAR( rba_Trcv_Cfg_Tick_to,RBA_TRCV_VAR) rba_Trcv_StateChangeTime_ao[RBA_TRCV_CFG_NUMBER_OF_TRCV];
#if (RBA_TRCV_CFG_WAKEUP_BY_NODE_USED == STD_ON)
VAR( uint8, RBA_TRCV_VAR) rba_Trcv_WakeupState_au8[RBA_TRCV_CFG_NUMBER_OF_TRCV];   /* 0: no wakeup; 1=wakeup detected */
#endif
VAR( rba_Trcv_TrcvWUReasonType_ten, RBA_TRCV_VAR) rba_Trcv_WakeupReason_au8[RBA_TRCV_CFG_NUMBER_OF_TRCV];

#if ( RBA_TRCV_CFG_SPI_USED == STD_ON )
uint16 rba_Trcv_SpiSeq1RxBuf_ao[RBA_TRCV_SPI_SEQ_MAX];
#endif

// Flag if GetTransceiverError is called when it is executed
VAR( uint16, RBA_TRCV_VAR) rba_Trcv_GetTransceiverError_Reentrantflag;
// Counter of reentrant calls of GetTransceiverError
VAR( uint16, RBA_TRCV_VAR) rba_Trcv_GetTransceiverError_Reentrantflag_Count;

#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_STOP_SEC_VAR_NO_INIT_UNSPECIFIED

# include "rba_Trcv_MemMap.h"
#endif

#if(RBA_TRCV_VARIANT != RBA_TRCV_VARIANT_PRECOMPILE)

#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_START_SEC_CONFIG_DATA_UNSPECIFIED

# include "rba_Trcv_MemMap.h"
#endif

P2CONST(rba_Trcv_ConfigType, AUTOMATIC,RBA_TRCV_CFG) rba_Trcv_ConfigData_st;

#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_STOP_SEC_CONFIG_DATA_UNSPECIFIED

# include "rba_Trcv_MemMap.h"
#endif

#endif

/**
***************************************************************************************************
*
*
*               Version Check
*
*
****************************************************************************************************
*/
#if(RBA_TRCV_PRV_EXPECTED_GEN_MAJOR_VERSION != RBA_TRCV_PRV_GEN_MAJOR_VERSION)
# error "Generator version does not fit to software version"
#endif
#if(RBA_TRCV_PRV_EXPECTED_GEN_MINOR_VERSION != RBA_TRCV_PRV_GEN_MINOR_VERSION)
# error "Generator version does not fit to software version"
#endif
/**
***************************************************************************************************
*
*
*               DEFINES
*
*
****************************************************************************************************
*/


#if (RBA_TRCV_DEV_ERROR_DETECT == STD_ON)
#define RBA_TRCV_DET_REPORTERROR(rba_TrcvId, ApiId, ErrId )            (void)Det_ReportError(RBA_TRCV_MODULE_ID, (rba_TrcvId), (ApiId), (ErrId));
#else
#define RBA_TRCV_DET_REPORTERROR(rba_TrcvId, ApiId, ErrId )
#endif

#define RBA_TRCV_DEM_FRTRCV_E_FR_NO_CONTROL_TRCV    0u
#define RBA_TRCV_DEM_FRTRCV_E_FR_BUSERROR_TRCV      1u
#define RBA_TRCV_DEM_FRTRCV_E_FR_ERRN_TRCV          2u

/**
***************************************************************************************************
*
*
*               RBA TRANSCEIVER DRIVER SPECIFIC FUNCTIONS
*
*
****************************************************************************************************
*/
#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_START_SEC_CODE

# include "rba_Trcv_MemMap.h"
#endif

/**
 ***************************************************************************************************
 * \moduledescription
 * Internal Init Function for transceivers
 *
 *
 *
 * \param   -
 * \arg     -
 * \return  void
 * \see
 * \note       Function is called from CanTrcv, LinTrcv and FrTrcv. It does not need to be called from upper layers!
 ***************************************************************************************************
 */
#if(RBA_TRCV_VARIANT != RBA_TRCV_VARIANT_PRECOMPILE)
FUNC (void, RBA_TRCV_CODE) rba_Trcv_Init( uint8 rba_Trcv_TrcvIdx,const rba_Trcv_ConfigType  *rba_Trcv_Config_cpst)
#else
FUNC (void, RBA_TRCV_CODE) rba_Trcv_Init( uint8 rba_Trcv_TrcvIdx )
#endif
{
    uint8           RetryCountInInit_u8 = 0;
    Std_ReturnType  RetVal_Set;
    Std_ReturnType  RetVal_Read;
    boolean         FlagOk_b;

    /*Check the channel ID*/
    if(rba_Trcv_TrcvIdx < RBA_TRCV_CFG_NUMBER_OF_TRCV)
    {
        // To have the possibility to start a timer which is used by trcv, at first
            // init-function-call, the macro rba_Trcv_TimeInit (defined in template rba_Trcv_Time.h)
            // is called at first init-call
            if(rba_Trcv_Prv_InitState_b != TRUE)
            {
                rba_Trcv_Prv_InitState_b = TRUE;
                rba_Trcv_TimeInit();        // Init timer once to avoid reset of Trcv if other Trcv are init later
            }

#if(RBA_TRCV_VARIANT != RBA_TRCV_VARIANT_PRECOMPILE)
                rba_Trcv_ConfigData_st = rba_Trcv_Config_cpst;
#endif

            // Reset _Reentrantflag at each init
            // possible reentrant GetTransceiverError calles during another trcv is init is accepted
            rba_Trcv_GetTransceiverError_Reentrantflag       = 0;
            rba_Trcv_GetTransceiverError_Reentrantflag_Count = 0;

            rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] = RBA_TRCV_WU_NOT_SUPPORTED;      /* Clear wakeup reason */
            // Repeat init-procedure as much RetryCountInInit requires
            do{
                FlagOk_b = TRUE;

# if RBA_TRCV_CFG_SPI_USED == STD_ON
                // Call once in InitPhase to init SPI and call first transfer (for async transmission)
                if(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].ConnectionType == RBA_TRCV_CONNECTIONTYPE_SPI)
                {
                    uint8 SpiPos_u8;
                    SpiPos_u8 = RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].GetError.StartPos;
                    rba_Trcv_SpiSeq1RxBuf_ao[SpiPos_u8] = 0;
#  if RBA_TRCV_SPI_EXTERNBUFFER == STD_ON
                    /* MR12 RULE 11.3 VIOLATION: The buffer-parametertypes are given by Autosar as uint8. But HW works with uint16. */
                    if(E_OK != Spi_RbSetupEB(RBA_TRCV_SPIARRAY_AST[SpiPos_u8].Channel,
                                (const uint8*)RBA_TRCV_SPIARRAY_AST[SpiPos_u8].TxBuf,
                                (uint8*)&(rba_Trcv_SpiSeq1RxBuf_ao[SpiPos_u8]),
                                RBA_TRCV_SPIARRAY_AST[SpiPos_u8].NumOfData ))
                    {
                        FlagOk_b = FALSE;
                    }
#  else
                    if(E_OK != Spi_WriteIB(RBA_TRCV_SPIARRAY_AST[SpiPos_u8].Channel,
                                RBA_TRCV_SPIARRAY_AST[SpiPos_u8].TxBuf ))     // points to Trcv generated data in ROM for initdata
                    {
                        FlagOk_b = FALSE;
                    }
#  endif
                    /* MR12 RULE 13.4 VIOLATION: Depending on used integration code there can be a Misra warning.*/
                    if (E_OK != RBA_TRCV_SPI_TRANSMIT(RBA_TRCV_SPIARRAY_AST[SpiPos_u8].Sequence ))
                    {
                        FlagOk_b = FALSE;
                    }
                }
# endif

                /* current state is readout in SetTransceiverMode before geterror could be called*/
                RetVal_Set  = rba_Trcv_SetTransceiverMode( rba_Trcv_TrcvIdx,
                                                            RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].InitState);
                /* Read state and writes internal static variable. Will be checked afterwards*/
                RetVal_Read = rba_Trcv_GetTransceiverMode( rba_Trcv_TrcvIdx, &rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx]);
                if((E_OK != RetVal_Set) || (E_OK != RetVal_Read) ||
                   (rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx] != RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].InitState))
                {
                    FlagOk_b = FALSE;
                }
                RetryCountInInit_u8++;
            }while((FlagOk_b == FALSE) && (RetryCountInInit_u8 <= RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].RetryCountInInit));
# if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
            {
                /* points to first DEM for Trcv. This is always NO_TRCV_CONTROL */
                if(FlagOk_b == FALSE)
                {
                    rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos, DEM_EVENT_STATUS_PREFAILED);
                }
                else
                {
                    rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos, DEM_EVENT_STATUS_PREPASSED);
                }
            }
# endif

    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_INIT, RBA_TRCV_E_INVALID_TRANSCEIVER)
    }


}
/**
 ***************************************************************************************************
 * \moduledescription
 * This function sets the transceiver mode
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx            identifies Transceiver to which the API has to be applied
 * \param   uint8                       rba_Trcv_TrcvMode
 * \arg     rba_Trcv_TrcvMode           Selects the state the transceiver will transit to
 *                                      Value points to table rba_Trcv_SetModeStructChain
 * \return     E_OK:                    The state request was successful
 *             E_NOT_OK:                The state request could not be executed, because of transceiver error
 *                                      or wrong parameters
 * \see
 * \note
 ***************************************************************************************************
 */
/* HIS METRIC PATH,RETURN VIOLATION IN rba_Trcv_SetTransceiverMode: PATH - Functionality is given by Autosar. Splitting does not make sense.RETURN- The early return is easier to understand and maintain than a local variable and lot of check during whole function  */
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_SetTransceiverMode( uint8 rba_Trcv_TrcvIdx, rba_Trcv_TrcvModeType_ten rba_Trcv_TrcvMode)
{
    rba_Trcv_ModeStructChain_tst const* ModeStructChain_pst;
    rba_Trcv_ConfigData_tst      const* ConfigData_st;
    rba_Trcv_Cfg_Tick_to rba_Trcv_StateChangeTime_o;
    uint8 idx; // pos in rba_Trcv_SetModeStructChainPos_au8
    uint8 end;
    Std_ReturnType RetVal = E_NOT_OK;

    // Check all input parameters
    if(rba_Trcv_Prv_InitState_b != FALSE)
    {
           /* No action if requested mode is same as current mode */
           /* To ensure correct mode (from external influence) read Mode again */
           (void)rba_Trcv_GetTransceiverMode( rba_Trcv_TrcvIdx, &rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx]);
           if(rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx] == rba_Trcv_TrcvMode)
           {
               RetVal = E_OK;
           }
           else
           {
                ConfigData_st      = &(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx]);

                if((rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_TRCVMODE_SLEEP) &&  /* check for RBA_TRCV_WU_INTERNALLY */
                 (rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_WU_NOT_SUPPORTED) &&   /* if current mode is TRCVMODE_SLEEP */
                 (rba_Trcv_TrcvMode <= RBA_TRCV_TRCVMODE_RECEIVEONLY))
                {   /* RBA_TRCV_WU_INTERNALLY is set if Sw brings Trcv in an active state and no other reason available*/
                  rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] = RBA_TRCV_WU_INTERNALLY;
                }

                if(rba_Trcv_TrcvMode == RBA_TRCV_TRCVMODE_SLEEP)    /* clear WakeupReason_au8 if requested mode TRCVMODE_SLEEP */
                {
                  /* clear WU Reason when goto sleep to be prepared for next wakeup */
                  rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] = RBA_TRCV_WU_NOT_SUPPORTED;
                }

                // get relevant configuration parameter
                idx = ConfigData_st->SetMode.StartPos + (uint8)rba_Trcv_TrcvMode; // pos in rba_Trcv_SetModeStructChainPos_au8
                end = RBA_TRCV_SETMODESTRUCTCHAINPOS_AU8[idx+1];

                idx = RBA_TRCV_SETMODESTRUCTCHAINPOS_AU8[idx];
                /* wait init time to be sure hardware is prepared to get new state */
                rba_Trcv_WaitUs(rba_Trcv_StateChangeTime_ao[rba_Trcv_TrcvIdx], ConfigData_st->WaitInit );
                // Loop through all configured states of trcv to go into requested state.
                // different states are needed because it can not be guaranteed that multiple pins can be set at same time.
                do{
                  ModeStructChain_pst = &RBA_TRCV_SETMODESTRUCTCHAIN[idx];    /* ModeStructChain_pst points to conf data */
                  if(ModeStructChain_pst->ChangeModeFunction != NULL_PTR)     /* Function pointer to special function */
                  {
                      ModeStructChain_pst->ChangeModeFunction(rba_Trcv_TrcvIdx);  // e.g. read out wakeup reason
                  }

                  Dio_WriteChannel(RBA_TRCV_DIOARRAY[ModeStructChain_pst->IoNr].Pin, ModeStructChain_pst->Value ^ (RBA_TRCV_DIOARRAY[ModeStructChain_pst->IoNr].Invert));
                  if(ModeStructChain_pst->WaitTime > 0)
                  {
                      /* MR12 RULE 11.3 VIOLATION: Convertion of OS-Time to rba_Trcv_Time. As this is integrator code, different time-values-types has to be mapped to a general rba_Trcv-time type */
                      RBA_TRCV_GETTIME(&rba_Trcv_StateChangeTime_o);  // Get Time after DioPin changed
                      rba_Trcv_WaitUs(rba_Trcv_StateChangeTime_o, ModeStructChain_pst->WaitTime );
                  }
                  idx++;
                }while(idx != end);
                /* MR12 RULE 11.3 VIOLATION: Convertion of OS-Time to rba_Trcv_Time. As this is integrator code, different time-values-types has to be mapped to a general rba_Trcv-time type */
                RBA_TRCV_GETTIME(&rba_Trcv_StateChangeTime_ao[rba_Trcv_TrcvIdx]);   // store time when mode changed
                rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx] = rba_Trcv_TrcvMode;

                RetVal = E_OK;

           }

    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_SETTRANSCEIVERMODE, RBA_TRCV_E_UNINIT)
    }

return RetVal;
}
/**
 ***************************************************************************************************
 * \moduledescription
 * This function returns the actual state of the transceiver.
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \param   uint8*        rba_Trcv_TrcvMode
 * \arg     rba_Trcv_TrcvMode             Current state of the transceiver
 * \return     E_OK:        The state can be evaluated
 *             E_NOT_OK:    The state could not be evaluated, either because of transceiver error
 *                          or wrong parameters
 * \see
 * \note
 ***************************************************************************************************
 */
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverMode( uint8 rba_Trcv_TrcvIdx,
                                                rba_Trcv_TrcvModeType_ten* rba_Trcv_TrcvModePtr )
{
    uint8        Value = 0;
    const uint8 *GetModePinChainPos_pu8;
    rba_Trcv_ConfigData_tst   const* ConfigData_st;
    uint8 GetModeArrayPos_u8;
    Std_ReturnType RetVal = E_NOT_OK;

    // Check all input parameters
    if(rba_Trcv_Prv_InitState_b != FALSE)
    {
        *rba_Trcv_TrcvModePtr = RBA_TRCV_TRCVMODE_NORMAL;     // Default value on error
        // Get relevant configuration parameter
        ConfigData_st          = &(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx]);
        GetModePinChainPos_pu8 = &RBA_TRCV_GETMODEPINCHAIN_AU8[
                                            ConfigData_st->GetMode.StartPos]; /* position of start of Mode-Pins */
        GetModeArrayPos_u8     = ConfigData_st->GetModeArray;

        /* get value of two Pins defined in rba_Trcv_GetModePinChain_au8 */
        RBA_TRCV_ENTERCRITICAL[ConfigData_st->BusType]();
        Value  = (uint8)((Dio_ReadChannel(RBA_TRCV_DIOARRAY[*GetModePinChainPos_pu8].Pin) ^ (RBA_TRCV_DIOARRAY[*GetModePinChainPos_pu8].Invert)) & 0x01u);
        GetModePinChainPos_pu8 = &GetModePinChainPos_pu8[1];
        Value |= (uint8)(((Dio_ReadChannel(RBA_TRCV_DIOARRAY[*GetModePinChainPos_pu8].Pin) ^ (RBA_TRCV_DIOARRAY[*GetModePinChainPos_pu8].Invert)) & 0x01u) << 1u);
        RBA_TRCV_LEAVECRITICAL[ConfigData_st->BusType]();
        /* caluclate Mode according rba_Trcv_GetModeArray and read out pins */
        *rba_Trcv_TrcvModePtr = RBA_TRCV_GETMODEARRAY[Value + GetModeArrayPos_u8];
        RetVal = E_OK;
    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_GETTRANSCEIVERMODE, RBA_TRCV_E_UNINIT)
    }

    return RetVal;
}
/**
 ***************************************************************************************************
 * \moduledescription
 * This function returns the internal stored wakeup reason.
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \param   rba_Trcv_TrcvWUReasonType_ten*    rba_Trcv_TrcvWUReasonPtr
 * \arg     rba_Trcv_TrcvWUReasonPtr      Evaluated wakeup reason
 * \return     E_OK:                    The wakeup reason can be evaluated
 *             E_NOT_OK:                The wakeup reason could not be evaluated, either because of transceiver error
 *                                      or wrong parameters
 * \see
 * \note        Called from  FrTrcv_GetTransceiverWUReason
 *              The function does not read out the current state as it can depend on much possibilites in trcv
 ***************************************************************************************************
 */
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverWUReason( uint8 rba_Trcv_TrcvIdx,
                                                rba_Trcv_TrcvWUReasonType_ten* rba_Trcv_TrcvWUReasonPtr )
{

    Std_ReturnType RetVal = E_NOT_OK;

    // Check all input parameters
    if(rba_Trcv_Prv_InitState_b != FALSE)
    {
        *rba_Trcv_TrcvWUReasonPtr = RBA_TRCV_WU_NOT_SUPPORTED; // Default value on DET failure

        *rba_Trcv_TrcvWUReasonPtr = rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx];

        RetVal = E_OK;
    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_GETTRANSCEIVERWUREASON, RBA_TRCV_E_UNINIT)
    }

    return RetVal;
}
/**
 ***************************************************************************************************
 * \moduledescription
 * This service returns the version information of this module.
 *
 *
 *
 * \param   Std_VersionInfoType*         versioninfo
 * \arg     versioninfo                  maps to Buffer of version information
 * \return  void
 * \see
 * \note
 ***************************************************************************************************
 */
#if (RBA_TRCV_GET_VERSION_INFO == STD_ON)
FUNC (void, RBA_TRCV_CODE) rba_Trcv_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, RBA_TRCV_APPL_DATA) versioninfo )
{
    // Check all input parameters
    if(versioninfo != NULL_PTR)
    {
        versioninfo->vendorID         = RBA_TRCV_VENDOR_ID;
        versioninfo->moduleID         = RBA_TRCV_MODULE_ID;
        versioninfo->sw_major_version = RBA_TRCV_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = RBA_TRCV_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = RBA_TRCV_SW_PATCH_VERSION;
    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(0, RBA_TRCV_SID_GETVERSIONINFO, RBA_TRCV_E_PARAM_POINTER)
    }
}
#endif
/**
 ***************************************************************************************************
 * \moduledescription
 * This function clears a pending wake up event.
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \return     E_OK:                    The wakeup reason can be cleared
 *             E_NOT_OK:                The wakeup reason can not be cleared, either because of transceiver error
 *                                      or wrong parameters
 * \see
 * \note  To avoid inconsistencies and side effects in current implementation additional functioality is not allowed.
 ***************************************************************************************************
 */
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_ClearTransceiverWakeup(uint8 rba_Trcv_TrcvIdx )
{
    //Check all input parameters
    Std_ReturnType RetVal = E_NOT_OK;

    if(rba_Trcv_Prv_InitState_b != FALSE)
    {
        if(rba_Trcv_TrcvIdx < RBA_TRCV_CFG_NUMBER_OF_TRCV)
        {
            /* ! To easy the access only one variables is used which is changed during return at GetWUReason !*/
#if (RBA_TRCV_CFG_WAKEUP_BY_NODE_USED == STD_ON)
            rba_Trcv_WakeupState_au8[rba_Trcv_TrcvIdx] = 0; /* 0: no wakeup; 1=wakeup detected */
#else
            (void) (rba_Trcv_TrcvIdx);    // RBA_TRCV_PRV_PARAM_UNUSED
#endif
            RetVal = E_OK;

        }
        else
        {
            RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_CLEARTRANCEIVERWAKEUP, RBA_TRCV_E_INVALID_TRANSCEIVER)

        }
    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_CLEARTRANCEIVERWAKEUP, RBA_TRCV_E_UNINIT)
    }

    return RetVal;
}
/**
 ***************************************************************************************************
 * \moduledescription
 * This function clears the internal stored wakeup reason.
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \return     -
 *
 * \see
 * \note        Depending on Config parameter XXX_CFG_CLEAR_WUPREASON_IN_SETWAKEUPMODE this function is used
 *              It is called out of CanTrcv_SetWakeupMode, FrTrcv_ClearTransceiverWakeup, LinTrcv_SetWakeupMode
 ***************************************************************************************************
 */
#if (RBA_TRCV_CFG_CLEAR_WUPREASON_API_USED == STD_ON)
FUNC (void, RBA_TRCV_CODE) rba_Trcv_ClearTransceiverWuReason( uint8 rba_Trcv_TrcvIdx )
{
    //Check all input parameters
    if(rba_Trcv_TrcvIdx < RBA_TRCV_CFG_NUMBER_OF_TRCV)
    {
        rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] = RBA_TRCV_WU_NOT_SUPPORTED;
    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_CLEARTRANCEIVERWAKEUPREASON, RBA_TRCV_E_INVALID_TRANSCEIVER)
    }

}
#endif
/**
 ***************************************************************************************************
 * \moduledescription
 * This function provides the current error of the transceiver
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \param   uint8                       rba_Trcv_BranchIdx
 * \arg     rba_Trcv_BranchIdx            identifies the branch of the transceiver to which the API has to be applied
 * \param   uint32*                     rba_Trcv_BusErrorState
 * \arg     rba_Trcv_BusErrorState        identifies current error of the transceiver
 * \return     E_OK:                    The error state can be read
 *             E_NOT_OK:                The error state can not be read, either because of transceiver error
 *                                      or wrong parameters
 * \see
 * \note    if rba_Trcv_WakeupReason_au8 is not set, rba_Trcv_WakeupReason_au8 could be set. It is not overwritten
 *          when it is not empty (asumption: earlier wakeup is correct wakeup).
 ***************************************************************************************************
 */
#if (RBA_TRCV_FR_USED == STD_ON)
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverError( uint8 rba_Trcv_TrcvIdx, uint8 rba_Trcv_BranchIdx,
                                                               uint32* rba_Trcv_BusErrorState )
{
/* suj1si AB requirement: Function is allowed to lock interrupt at maximum of 10us */
/* suj1si AB requirement: Function can be  interrupted for max 500us  */

    // Declare and set local variables
    Std_ReturnType              Retval = E_NOT_OK;

# if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_SWITCH_USED == STD_ON)
    uint8_least                 FuncNo_qu8;
# endif
    // Check all input parameters
    if(rba_Trcv_Prv_InitState_b != FALSE)
    {
        if(rba_Trcv_TrcvIdx < RBA_TRCV_CFG_NUMBER_OF_TRCV)
        {
            if(rba_Trcv_BusErrorState != NULL_PTR)
            {
                // Set default return value
                *rba_Trcv_BusErrorState = 0;

                if (rba_Trcv_GetTransceiverError_Reentrantflag == 0)
                {
                    rba_Trcv_GetTransceiverError_Reentrantflag = 1;

# if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_SWITCH_USED == STD_ON)

                    // Get Function No
                    FuncNo_qu8      = RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].GetError.FuncNumber;
# endif

# if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_NR0_USED == STD_ON)
#  if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_SWITCH_USED == STD_ON)
                    if (FuncNo_qu8 == RBA_TRCV_FUNCTION_NR0)
#  endif
                    {
                        Retval = rba_Trcv_GetTransceiverError_N0(rba_Trcv_TrcvIdx, rba_Trcv_BusErrorState);
                    }
# endif
# if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_NR1_USED == STD_ON)
#  if ( RBA_TRCV_CFG_SPI_USED == STD_ON )
#   if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_SWITCH_USED == STD_ON)
                    if (FuncNo_qu8 == RBA_TRCV_FUNCTION_NR1)
#   endif
                    {
                        /* HIS METRIC LEVEL VIOLATION IN rba_Trcv_GetTransceiverError: Additional if nesting is required to call the proper function.*/
                        Retval = rba_Trcv_GetTransceiverError_N1(rba_Trcv_TrcvIdx, rba_Trcv_BusErrorState);
                    }
#  endif // RBA_TRCV_CFG_SPI_USED == STD_ON
# endif // RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_NR1_USED == STD_ON

                    rba_Trcv_GetTransceiverError_Reentrantflag = 0;

                }
                else
                {
                    // Increase debug variable
                    rba_Trcv_GetTransceiverError_Reentrantflag_Count++;
                }
                (void) (rba_Trcv_BranchIdx);  // RBA_TRCV_PRV_PARAM_UNUSED

            }
            else
            {
                RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_GETTRANSCEIVERERROR, RBA_TRCV_E_PARAM_POINTER)
            }

        }
        else
        {
            RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_GETTRANSCEIVERERROR, RBA_TRCV_E_INVALID_TRANSCEIVER)

        }
    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_GETTRANSCEIVERERROR, RBA_TRCV_E_UNINIT)
    }

    return Retval;
}
#endif

/**
 ***************************************************************************************************
 * \moduledescription
 * This function provides the current error of the transceiver
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \param   uint8                       rba_Trcv_BranchIdx
 * \arg     rba_Trcv_BranchIdx            identifies the branch of the transceiver to which the API has to be applied
 * \param   uint32*                     rba_Trcv_BusErrorState
 * \arg     rba_Trcv_BusErrorState        identifies current error of the transceiver
 * \return     E_OK:                    The error state can be read
 *             E_NOT_OK:                The error state can not be read, either because of transceiver error
 *                                      or wrong parameters
 * \see
 * \note    if rba_Trcv_WakeupReason_au8 is not set, rba_Trcv_WakeupReason_au8 could be set. It is not overwritten
 *          when it is not empty (asumption: earlier wakeup is correct wakeup).
 ***************************************************************************************************
 */
#if (RBA_TRCV_FR_USED == STD_ON)
# if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_NR0_USED == STD_ON)
/* HIS METRIC RETURN VIOLATION IN rba_Trcv_GetTransceiverError_N0: The early return is easy to understand and maintain. Better than an local variable and many conditions*/
/* HIS METRIC v(G) VIOLATION IN rba_Trcv_GetTransceiverError_N0: This function is very time critial and evaluated with custome. His-violations are required to fulfill timing constraints*/
/* HIS METRIC PATH VIOLATION IN rba_Trcv_GetTransceiverError_N0: This function is very time critial and evaluated with custome. His-violations are required to fulfill timing constraints*/
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverError_N0(uint8 rba_Trcv_TrcvIdx, uint32* rba_Trcv_BusErrorState)
{
    // Declare and set local variables for Function No 0
    Std_ReturnType              Retval = E_NOT_OK;
    uint8                       ArrayPos;               // For Dio: Pos in rba_Trcv_ErrorBitStruct,
    uint8                       const *ErrBitPos_pst;   /* counter in array rba_Trcv_GetErrorBitPos_au8 */
    uint8_least                 BusType_qu8;

    rba_Trcv_DioArray_tst       const *Dio_po;

    rba_Trcv_Cfg_Tick_to        TimestampOuter;
    rba_Trcv_Cfg_Tick_to        TimestampInner = 0;
    rba_Trcv_Cfg_Tick_to        TimestampOld;

    uint8 CounterErrBitPos_u8;
    uint8_least FlagEnd_qu8;              /* 0=no end, 1=ok, 2=inner Timeout, 3= outer timeout */
    uint32 Value_u32 = 0;
    Dio_LevelType   Val1_o;
    Dio_LevelType   Val2_o;
    rba_Trcv_ErrorBitStruct_tst const *ErrorBitStruct_pst;

//# if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
//    rba_Trcv_DemStruct_tst const *dem_p;
//# endif

    // get relevant configuration parameter
    BusType_qu8 = RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].BusType;
    ArrayPos        = RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].GetError.StartPos;
    ErrorBitStruct_pst    = &(RBA_TRCV_ERRORBITSTRUCT[ArrayPos]);
    ErrBitPos_pst = &(RBA_TRCV_GETERRORBITPOS_AU8[ErrorBitStruct_pst->StartPosGetErrorBitPos]);

    // Check if Trcv is in correct mode (e.g. AS8221 can only toggle out data in Normal/ReceiveOnly mode
    if(ErrorBitStruct_pst->TrcvState <  rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx])
    {
        return E_NOT_OK;    // Error can not read out -> exit here. Otherwise an additional if is needed.
    }

    // Get time for outer timeout (how long does the function try to read bits)
    /* MR12 RULE 11.3 VIOLATION: Convertion of OS-Time to rba_Trcv_Time. As this is integrator code, different time-values-types has to be mapped to a general rba_Trcv-time type */
    RBA_TRCV_GETTIME(&TimestampOuter);

    // Get time of last change of state (e.g. En Pin).
    TimestampOld = rba_Trcv_StateChangeTime_ao[rba_Trcv_TrcvIdx]; /* last change of state */

    // Store current state of EnPin -
    Dio_po       = &(RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].StartDio]);
    Val1_o = STD_LOW;
    Val2_o = STD_HIGH;
    if((Dio_ReadChannel(Dio_po[RBA_TRCV_DIO_CLK_POS].Pin)) == STD_LOW)
    {
        Val1_o = STD_HIGH;
        Val2_o = STD_LOW;
    }
    do      // Outer loop (complete read out of all bits)
    {
        // initiate outer loop
        FlagEnd_qu8         = 0;
        Value_u32           = 0;
        CounterErrBitPos_u8 = 0;
        // Wait either since last StateChange or timeout of inner loop
        //   inside outer loop to reset TRCV time
        //   for first loop it does not wait as waited outside
        rba_Trcv_WaitUs(TimestampOld,RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WaitInit );
        /* Read ERRN Pin level as first diagnostic bit */
        if((Dio_ReadChannel(RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].ErrorPinPos].Pin) ^ (RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].ErrorPinPos].Invert)) == ErrBitPos_pst[0])
        {
#  if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
//            dem_p = &rba_Trcv_DemStruct[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_ERRN_TRCV];
//            dem_p->Dem_ReportErrorStatusFunction(dem_p->EventId, DEM_EVENT_STATUS_PREFAILED );

            rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_ERRN_TRCV, DEM_EVENT_STATUS_PREFAILED);

#  endif
            /* Set Diagnostic-Bit if ERRN == STD_HIGH */
            *rba_Trcv_BusErrorState |= (uint32)1 << (ErrBitPos_pst[1]); /* first value in table is errn bit */
        }
#  if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
        else
        {
//            dem_p = &rba_Trcv_DemStruct[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_ERRN_TRCV];
//            dem_p->Dem_ReportErrorStatusFunction(dem_p->EventId, DEM_EVENT_STATUS_PREPASSED );

            rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_ERRN_TRCV, DEM_EVENT_STATUS_PREPASSED);
        }
#  endif
        /* MR12 RULE 15.4 VIOLATION:  This function is very time critical (for some Trcv loop shorter than 1us) so this violation is checked and accepted */
        do  // Inner loop (for each bit)
        {
            RBA_TRCV_ENTERCRITICAL[BusType_qu8]();  // avoid state change according previous state
            TimestampOld = TimestampInner;          // store previous timestamp for comparation
            //GETTIME inside critical section to avoid incorrect (to short) impulse which could not
            //  be detected by inner timeout check
            /* MR12 RULE 11.3 VIOLATION: Convertion of OS-Time to rba_Trcv_Time. As this is integrator code, different time-values-types has to be mapped to a general rba_Trcv-time type */
            RBA_TRCV_GETTIME(&TimestampInner);      // store inner timestamp for "bitreading"-timeout

            // first pulse?
            if(CounterErrBitPos_u8 != 0) // no check after first pulse as it is not of interrest.
            {                            //   and TimestampOld has old value
                // Check for outer timeout
                // Compare time to previous 'read time' = bitlength
                if(rba_Trcv_CheckTimeElapsed(TimestampInner,TimestampOuter,ErrorBitStruct_pst->TimeoutOuter))
                {
                    FlagEnd_qu8=3;
                    RBA_TRCV_LEAVECRITICAL[BusType_qu8]();
                    /* MR12 RULE 15.4 VIOLATION:  This function is very time critical (for some Trcv loop shorter than 1us) so this violation is checked and accepted */
                    break;          // Ends inner do-loop
                }
                if(rba_Trcv_CheckTimeElapsed(TimestampInner, TimestampOld, ErrorBitStruct_pst->TimeoutInner))
                {
                    // store previous timestamp for comparation to WaitInit
                    //   add last pulse change for correct WaitInit
                    TimestampOld += ErrorBitStruct_pst->WaitBeforeRead * RBA_TRCV_CFG_TIME_FACTOR;
                    // Check if it makes sense to wait (timeoutOuter will be reached after WaitInit)
                    if(rba_Trcv_CheckTimeElapsed (TimestampOld + (RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WaitInit * RBA_TRCV_CFG_TIME_FACTOR), TimestampOuter, ErrorBitStruct_pst->TimeoutOuter))
                    {
                        FlagEnd_qu8 = 3;
                    }
                    else
                    {
                        FlagEnd_qu8 = 2;
                    }
                    RBA_TRCV_LEAVECRITICAL[BusType_qu8]();
                    /* MR12 RULE 15.4 VIOLATION:  This function is very time critical (for some Trcv loop shorter than 1us) so this violation is checked and accepted */
                    break;          // Ends inner do-loop
                }
            }
            // Change EnPin
            Dio_WriteChannel(Dio_po[RBA_TRCV_DIO_CLK_POS].Pin, Val1_o);
            CounterErrBitPos_u8 += 2;
            rba_Trcv_WaitUs(TimestampInner,ErrorBitStruct_pst->WaitBeforeRead );
            Dio_WriteChannel(Dio_po[RBA_TRCV_DIO_CLK_POS].Pin,Val2_o);
            RBA_TRCV_LEAVECRITICAL[BusType_qu8]();
            //wait configured time (after Pin-Toggling)
            rba_Trcv_WaitUs(TimestampInner,ErrorBitStruct_pst->WaitAfterRead );
            // Shows bit an error?
            if((Dio_ReadChannel(RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].ErrorPinPos].Pin)
                    ^ (RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].ErrorPinPos]).Invert)==
                                                             (ErrBitPos_pst[CounterErrBitPos_u8]))
            {
                //store Error information
                Value_u32 |= (uint32)1 << (ErrBitPos_pst[CounterErrBitPos_u8+1]);
            }
            if(CounterErrBitPos_u8 == ErrorBitStruct_pst->NumberOfErrorBits)
            {
                // final time check
                FlagEnd_qu8 = 1u;
                /* check duration (correctness)) of last bit */
                TimestampOld = TimestampInner;          /* store previous timestamp for comparation */
                /* MR12 RULE 11.3 VIOLATION: Convertion of OS-Time to rba_Trcv_Time. As this is integrator code, different time-values-types has to be mapped to a general rba_Trcv-time type */
                RBA_TRCV_GETTIME(&TimestampInner);      /* initial store to go for loop initialization */
                if(rba_Trcv_CheckTimeElapsed(TimestampInner, TimestampOld, ErrorBitStruct_pst->TimeoutInner))
                {
                    FlagEnd_qu8 = 2u;
                    // On inner timeout, check outer timeout to decide if loop starts again.
                    if(rba_Trcv_CheckTimeElapsed (TimestampInner + (RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WaitInit * RBA_TRCV_CFG_TIME_FACTOR), TimestampOuter, ErrorBitStruct_pst->TimeoutOuter))
                    {
                        FlagEnd_qu8 = 3u;
                    }
                }
            }
        }while(FlagEnd_qu8 == 0u);   /* 0=no end, 1=ok, 2=inner Timeout, 3= outer timeout */
    }while(FlagEnd_qu8 == 2u);       /* no repeat at ok, outer Timeout   but at inner timeout. */

    rba_Trcv_StateChangeTime_ao[rba_Trcv_TrcvIdx] = TimestampInner; // store last Pin change time
    // Readout complete
    if(FlagEnd_qu8 == 1u)
    {
        *rba_Trcv_BusErrorState |= Value_u32;
        /* If read out ok -> store wakeup reason */
        if((rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_WU_NOT_SUPPORTED) ||
           (rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_WU_INTERNALLY))
        {
            /* MR12 RULE 10.5, 10.8 VIOLATION: The reason (enum) is calculated out of the bitmask (uint32) given by hardware. */
            rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] = (rba_Trcv_TrcvWUReasonType_ten)((Value_u32 >> 16u) & 0x1Fu); /* return state */
        }
#  if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
        /* Report Buserror to DEM */
//        dem_p = &rba_Trcv_DemStruct[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_BUSERROR_TRCV];
        if((*rba_Trcv_BusErrorState & ((uint32)1 << RBA_TRCV_ERRORBITPOS_E6)) != 0u)
        {
//            dem_p->Dem_ReportErrorStatusFunction(dem_p->EventId, DEM_EVENT_STATUS_PREFAILED );
            rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_BUSERROR_TRCV, DEM_EVENT_STATUS_PREFAILED);
        }
        else
        {
//            dem_p->Dem_ReportErrorStatusFunction(dem_p->EventId, DEM_EVENT_STATUS_PREPASSED );
            rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_BUSERROR_TRCV, DEM_EVENT_STATUS_PREPASSED);
        }
#  endif
        Retval = E_OK;
    }
    else
    {
        Retval = E_NOT_OK;
    }
    return Retval;
}
# endif
#endif


/**
 ***************************************************************************************************
 * \moduledescription
 * This function provides the current error of the transceiver
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \param   uint8                       rba_Trcv_BranchIdx
 * \arg     rba_Trcv_BranchIdx            identifies the branch of the transceiver to which the API has to be applied
 * \param   uint32*                     rba_Trcv_BusErrorState
 * \arg     rba_Trcv_BusErrorState        identifies current error of the transceiver
 * \return     E_OK:                    The error state can be read
 *             E_NOT_OK:                The error state can not be read, either because of transceiver error
 *                                      or wrong parameters
 * \see
 * \note    if rba_Trcv_WakeupReason_au8 is not set, rba_Trcv_WakeupReason_au8 could be set. It is not overwritten
 *          when it is not empty (asumption: earlier wakeup is correct wakeup).
 ***************************************************************************************************
 */
#if (RBA_TRCV_FR_USED == STD_ON)
# if (RBA_TRCV_GETTRANSCEIVERERROR_FUNCTION_NR1_USED == STD_ON)
#  if ( RBA_TRCV_CFG_SPI_USED == STD_ON )
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_GetTransceiverError_N1(uint8 rba_Trcv_TrcvIdx, uint32* rba_Trcv_BusErrorState)
{
// Declare and set local variables for Function No 1
    Std_ReturnType              Retval = E_OK;
    uint8                       ArrayPos;               // For Dio: Pos in rba_Trcv_ErrorBitStruct,
    uint8                       const *ErrBitPos_pst;   /* counter in array rba_Trcv_GetErrorBitPos_au8 */

    uint8_least n;
    uint8 SpiPos_u8;
    uint32 SpiRxValue;
    Std_ReturnType RetFunc;
    rba_Trcv_Spi_tst const *ErrorBitStruct_pst;

# if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
//    rba_Trcv_DemStruct_tst const *dem_p;
# endif


    // get relevant configuration parameter
    ArrayPos            = RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].GetError.StartPos;
    ErrorBitStruct_pst  = &(RBA_TRCV_SPIARRAY_AST[ArrayPos]);
    ErrBitPos_pst       = &(RBA_TRCV_GETERRORBITPOS_AU8[ErrorBitStruct_pst->StartPosGetErrorBitPos]);

    /* Read ERRN Pin level as first diagnostic bit */
    *rba_Trcv_BusErrorState = 0;
    if((Dio_ReadChannel(RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].ErrorPinPos].Pin)
            ^ (RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].ErrorPinPos]).Invert) == ErrBitPos_pst[0])
    {
        // Store error
        *rba_Trcv_BusErrorState |= (uint32)1 << (ErrBitPos_pst[1]); /* first value in table is errn bit */
#   if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
//        dem_p = &rba_Trcv_DemStruct[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_ERRN_TRCV];
//        dem_p->Dem_ReportErrorStatusFunction(dem_p->EventId, DEM_EVENT_STATUS_PREFAILED );

        rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_ERRN_TRCV, DEM_EVENT_STATUS_PREFAILED);
    }
    else
    {
//        dem_p = &rba_Trcv_DemStruct[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_ERRN_TRCV];
//        dem_p->Dem_ReportErrorStatusFunction(dem_p->EventId, DEM_EVENT_STATUS_PREPASSED );

        rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_ERRN_TRCV, DEM_EVENT_STATUS_PREPASSED);
#   endif
    }
    // Read out SPI values of previous answer
    SpiPos_u8 = RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].GetError.StartPos;
    SpiRxValue = (uint32)rba_Trcv_SpiSeq1RxBuf_ao[SpiPos_u8];
    // Start new SPI transfer
    // Start transmission. If previous tranmit was not successfull, a pending SPI shows a "not ok"
    //    Async: possible return N_OK also previous answer is rejected
    //    Sync:  previous answer is handled. If previous was N_OK, data stayed 0 and are not handled.
    rba_Trcv_SpiSeq1RxBuf_ao[SpiPos_u8] = 0;    // Clear buffer to detect errors.

#if RBA_TRCV_SPI_EXTERNBUFFER == STD_OFF
    if(E_OK != Spi_ReadIB(RBA_TRCV_SPIARRAY_AST[SpiPos_u8].Channel,  &(rba_Trcv_SpiSeq1RxBuf_ao[SpiPos_u8])))
    {
        Retval = E_NOT_OK;
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_GETTRANSCEIVERERROR, RBA_TRCV_E_SPI)
    }

    if(Retval == E_OK)
    {
        if(E_OK != Spi_WriteIB(RBA_TRCV_SPIARRAY_AST[SpiPos_u8].Channel, RBA_TRCV_SPIARRAY_AST[SpiPos_u8].TxBuf))
        {
            Retval = E_NOT_OK;
            RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_GETTRANSCEIVERERROR, RBA_TRCV_E_SPI)
        }

    }
    if(Retval == E_OK)
    {
#endif

    RetFunc = (uint8)RBA_TRCV_SPI_TRANSMIT(RBA_TRCV_SPIARRAY_AST[ArrayPos].Sequence );

#if RBA_TRCV_SPI_EXTERNBUFFER == STD_OFF
    }
#endif
    // SPI transfer ok?

    if((E_OK == RetFunc ) && (SpiRxValue != 0u) && (Retval == E_OK))
    {
        // Loop over all errorbits
        for(n = 2; n <= ErrorBitStruct_pst->NumberOfErrorBits; n += 2u)
        {
            if(ErrBitPos_pst[n] == (SpiRxValue & (uint32)0x1))
            {
                // store Error information
                *rba_Trcv_BusErrorState |= ((uint32)0x1u) << ErrBitPos_pst[n + 1u];
            }
            SpiRxValue >>= 1u;
        }
        // store wakeup reason
        *rba_Trcv_BusErrorState &= 0x7fffffffuL;  /* remove highest (unrelevant) bit */
        if((rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_WU_NOT_SUPPORTED) ||
           (rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_WU_INTERNALLY))
        {
            /* MR12 RULE 10.5, 10.8 VIOLATION: The reason (enum) is calculated out of the bitmask (uint32) given by hardware. */
            rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] = (rba_Trcv_TrcvWUReasonType_ten)((*rba_Trcv_BusErrorState >> 16u) & 0x1Fu);
        }

#   if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
        /* Report Buserror to DEM */

//        dem_p = &rba_Trcv_DemStruct[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_BUSERROR_TRCV];
        if((*rba_Trcv_BusErrorState & ((uint32)1 << RBA_TRCV_ERRORBITPOS_E0)) != 0u)
        {
//           dem_p->Dem_ReportErrorStatusFunction(dem_p->EventId, DEM_EVENT_STATUS_PREFAILED );
           rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_BUSERROR_TRCV, DEM_EVENT_STATUS_PREFAILED);
        }
        else
        {
//           dem_p->Dem_ReportErrorStatusFunction(dem_p->EventId, DEM_EVENT_STATUS_PREPASSED );
           rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_BUSERROR_TRCV, DEM_EVENT_STATUS_PREPASSED);
        }
#   endif

    }
    else
    {
        /* DEM Error:  SPI transfer ok?*/
#   if (RBA_TRCV_CFG_DEM_ENABLED == STD_ON)
//        dem_p = &rba_Trcv_DemStruct[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_NO_CONTROL_TRCV];
//        dem_p->Dem_ReportErrorStatusFunction(dem_p->EventId, DEM_EVENT_STATUS_PREFAILED );

        rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].DemPos + RBA_TRCV_DEM_FRTRCV_E_FR_NO_CONTROL_TRCV, DEM_EVENT_STATUS_PREFAILED);
#   endif
        *rba_Trcv_BusErrorState = 0;
        Retval = E_NOT_OK;
    }

    return Retval;
}
#  endif
# endif
#endif

/**
 ***************************************************************************************************
 * \moduledescription
 * This function provides the current error of the transceiver
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \param   uint8                       rba_Trcv_BranchIdx
 * \arg     rba_Trcv_BranchIdx            identifies the branch of the transceiver to which the API has to be applied
 * \param   uint32*                     rba_Trcv_BusErrorState
 * \arg     rba_Trcv_BusErrorState        identifies current error of the transceiver
 * \return     E_OK:                    The error state can be read
 *             E_NOT_OK:                The error state can not be read, either because of transceiver error
 *                                      or wrong parameters
 * \see
 * \note    if rba_Trcv_WakeupReason_au8 is not set, rba_Trcv_WakeupReason_au8 could be set. It is not overwritten
 *          when it is not empty (asumption: earlier wakeup is correct wakeup).
 ***************************************************************************************************
 */




/**
 ***************************************************************************************************
 * \moduledescription
 * This function disables the specified branch on the addressed (active star) transceiver.
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \param   uint8                       rba_Trcv_BranchIdx
 * \arg     rba_Trcv_BranchIdx          index to One of Possible Configuration Indexes
 * \return     E_OK:                    The transceiver can be disabled
 *             E_NOT_OK:                The transceiver could not be disabled, either because of transceiver error
 *                                      or wrong parameters
 * \see
 * \note
 ***************************************************************************************************
 */
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_DisableTransceiverBranch
                                               (uint8 rba_Trcv_TrcvIdx, uint8 rba_Trcv_BranchIdx )
{
    (void) (rba_Trcv_TrcvIdx);    // RBA_TRCV_PRV_PARAM_UNUSED
    (void) (rba_Trcv_BranchIdx);  // RBA_TRCV_PRV_PARAM_UNUSED
    return E_OK;
}
/**
 ***************************************************************************************************
 * \moduledescription
 * This function enables the specified branch on the addressed (active star) transceiver.
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx              identifies Transceiver to which the API has to be applied
 * \param   uint8                       rba_Trcv_BranchIdx
 * \arg     rba_Trcv_BranchIdx            index to One of Possible Configuration Indexes
 * \return     E_OK:                    The transceiver can be denabled
 *             E_NOT_OK:                The transceiver could not be enabled, either because of transceiver error
 *                                      or wrong parameters
 * \see
 * \note
 ***************************************************************************************************
 */
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_EnableTransceiverBranch( uint8 rba_Trcv_TrcvIdx, uint8 rba_Trcv_BranchIdx)
{
    (void) (rba_Trcv_TrcvIdx);    // RBA_TRCV_PRV_PARAM_UNUSED
    (void) (rba_Trcv_BranchIdx);  // RBA_TRCV_PRV_PARAM_UNUSED
    return E_OK;
}
/**
 ***************************************************************************************************
 * \moduledescription
 * Checks HW for current or previous wakeup and returns value
 *
 *
 * \param   uint8               rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx    identifies Transceiver to which the API has to be applied
 * \return  E_OK:               Wakeup occured
 *          E_NOT_OK:           No wakeup occured
 * \see
 * \note    if correct mode, function checks wakeup info direct from transceiver
 *          If only the current trcv wakeup state/pin want to be checked,
 *              FrTrcv_ClearTransceiverWakeup as to be called first to clear old wakeups
 ***************************************************************************************************
 */
#if (RBA_TRCV_CFG_WAKEUP_BY_NODE_USED == STD_ON)
FUNC (Std_ReturnType, RBA_TRCV_CODE) rba_Trcv_CheckWakeupByTransceiver( uint8 rba_Trcv_TrcvIdx )
{
    Dio_LevelType  Value;
    uint8 GetWakeupPinPos_pu8;
    Std_ReturnType RetVal = E_NOT_OK;

    // Check all input parameters

    if(rba_Trcv_Prv_InitState_b != FALSE)
    {
        if((rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_TRCVMODE_STANDBY) ||
           (rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_TRCVMODE_SLEEP))
        {
            if(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WakeupPossibility == 1)
            {
                GetWakeupPinPos_pu8 = RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WakePinPos;
                Value = Dio_ReadChannel(RBA_TRCV_DIOARRAY[GetWakeupPinPos_pu8].Pin) ^ (RBA_TRCV_DIOARRAY[GetWakeupPinPos_pu8].Invert);
                if(Value == STD_LOW)
                {
                    rba_Trcv_WakeupState_au8[rba_Trcv_TrcvIdx] = 1; /* 0: no wakeup; 1=wakeup detected */
                }
            }
        }
        if(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WakeupPossibility != 0)
        {
            if(rba_Trcv_WakeupState_au8[rba_Trcv_TrcvIdx] == 1)   /* 0: no wakeup; 1=wakeup detected */
            {
                RetVal = E_OK;
            }
        }

    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_CHECKWAKEUPBYTRANSCEIVER, RBA_TRCV_E_UNINIT)
    }

    return RetVal;
}
#endif /* RBA_TRCV_CFG_WAKEUP_BY_NODE_USED == STD_ON */
/**
*******************************************************************************************************
* Function name:  rba_Trcv_WaitUs
* Description:    Waits given time of micro seconds until given time
* Arguments:    - rba_Trcv_Cfg_Tick_to StartTime_o:         start of time period (in timeticks)
*               - rba_Trcv_Cfg_Tick_to TimePeriod_o:        length of time period in us
* Returns:      -
* Usage guide:
*
* Scheduling:
*               This function is to be used internally only !
* Remarks:      When using timers to wait for an amount of time, there's the possibility of a timer
*               overflow, which might lead to wrong timing if undetected. The main purpose of this
*               function is to check if a certain time has expired, taking into account
*               the possibility of a timer overflow. The function is based on timer values only, and
*               therefore independent of the timer source. It can be used with any hardware or
*               software timer.
*               See function TIM_IsTimeExpiredFastTimer for more information.
*               If the timer stands and is not changed, a DET is raised.
*******************************************************************************************************
*/
static FUNC (void, RBA_TRCV_CODE) rba_Trcv_WaitUs(rba_Trcv_Cfg_Tick_to StartTime_o, rba_Trcv_Cfg_Tick_to TimePeriod_o)
{
    rba_Trcv_Cfg_Tick_to    CurrentTimerValue_o;
    rba_Trcv_Cfg_Tick_to    OldTimerValue = 0;
    uint32 TimeoutCounter_u32;

    TimeoutCounter_u32 = 0;
    do
    {
        TimeoutCounter_u32++;
        /* MR12 RULE 11.3 VIOLATION: Convertion of OS-Time to rba_Trcv_Time. As this is integrator code, different time-values-types has to be mapped to a general rba_Trcv-time type */
        RBA_TRCV_GETTIME(&CurrentTimerValue_o);
        if(CurrentTimerValue_o == OldTimerValue)
        {
            TimeoutCounter_u32++;
        }
        else
        {
            TimeoutCounter_u32 = 0;
            OldTimerValue = CurrentTimerValue_o;
        }
    }while((rba_Trcv_CheckTimeElapsed(CurrentTimerValue_o,StartTime_o,TimePeriod_o) == FALSE )  &&
           (TimeoutCounter_u32 < RBA_TRCV_TIMEMEASUREMENT_TIMEOUT));

    if(TimeoutCounter_u32 >= RBA_TRCV_TIMEMEASUREMENT_TIMEOUT)
    {
        RBA_TRCV_DET_REPORTERROR(0, RBA_TRCV_SID_WAITUS, RBA_TRCV_E_PARAM_TIMEOUT)
    }

}
/**
*******************************************************************************************************
* Function name:  rba_Trcv_CheckTimeElapsed
* Description:    Check if a given time period has expired (internal function)
* Arguments:    - rba_Trcv_Cfg_Tick_to CurrentTime_o:   current time (in timeticks)
*               - rba_Trcv_Cfg_Tick_to StartTime_o:     start of time period (in timeticks)
*               - rba_Trcv_Cfg_Tick_to TimePeriod_o     length of time period in us
* Returns:      TRUE:   Time has expired
*               FALSE:  Time has not yet expired
* Usage guide:
*
* Scheduling:
*               This function is to be used internally only !
* Remarks:      When using timers to wait for an amount of time, there's the possibility of a timer
*               overflow, which might lead to wrong timing if undetected. The main purpose of this
*               function is to check if a certain time has expired, taking into account
*               the possibility of a timer overflow. The function is based on timer values only, and
*               therefore independent of the timer source. It can be used with any hardware or
*               software timer.
*               See function TIM_IsTimeExpiredFastTimer for more information.
*******************************************************************************************************
*/
static FUNC (boolean, RBA_TRCV_CODE) rba_Trcv_CheckTimeElapsed(rba_Trcv_Cfg_Tick_to CurrentTime_o,
                                                rba_Trcv_Cfg_Tick_to StartTime_o, rba_Trcv_Cfg_Tick_to TimePeriod_o)
{
    rba_Trcv_Cfg_Tick_to   DiffTime_o;
    boolean RetVal = TRUE;

    // Check if StartTime_o + TimePeriod_o is after or same as CurrentTime_o
    DiffTime_o = (CurrentTime_o - StartTime_o) ;
    if (DiffTime_o >= (TimePeriod_o * ((uint32)RBA_TRCV_CFG_TIME_FACTOR)))
    {
        RetVal = TRUE;    /* Time has expired ! (1)*/
    }
    else
    {
        RetVal = FALSE;   /* Time has not yet expired ! (0)*/
    }
    return RetVal;
}
/**
 ***************************************************************************************************
 * \rba_Trcv_ChangeModeFunction1
 * This function reads Wakeupsignalisation Pin and sets wakeup state (Wakeup detected except PWR on)
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx            identifies Transceiver to which the API has to be applied
 *                                      Value points to table rba_Trcv_SetModeStructChain
 * \return     -   :                    -
 *
 * \see
 * \note    Function is called before TJA1040/TJA1041/.. is set to normal mode.
 *          It read Wakeup-Pin as it displays  wake-up state.
 *          Wakeup flag is set and can be read out. Wakeup flag is cleared in normal mode
 ***************************************************************************************************
 */
#if (RBA_TRCV_CHANGE_MODE_FUNCTION1_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_ChangeModeFunction1(uint8 rba_Trcv_TrcvIdx)
{
    /* Read WakePin from Transceiver before normal mode and store value as it is cleared in normal mode */
# if (RBA_TRCV_CFG_WAKEUP_BY_NODE_USED == STD_ON)
    /* The function can be called in a sequence of Pin/Mode-changes. To check if not normal state is reached and  */
    /* Bit has correct state, the current state is read out and the bit is only read in Not normal mode  */
    if(E_OK == rba_Trcv_GetTransceiverMode( rba_Trcv_TrcvIdx, &rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx]))
    {
        if(rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx] != RBA_TRCV_TRCVMODE_NORMAL)
        {
            if(STD_LOW == (Dio_ReadChannel(RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WakePinPos].Pin)
                             ^ (RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WakePinPos].Invert)))
            {
                rba_Trcv_WakeupState_au8[rba_Trcv_TrcvIdx] |= 1u; /* 0: no wakeup; 1=wakeup detected */
            }
        }
    }
    return;
#else
    (void) (rba_Trcv_TrcvIdx);    // RBA_TRCV_PRV_PARAM_UNUSED
# endif
}
#endif
/**
 ***************************************************************************************************
 * \moduledescription
 * This function reads out Wakeup Pin after reaching normal mode and sets wakeup reason
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx            identifies Transceiver to which the API has to be applied
 *                                      Value points to table rba_Trcv_SetModeStructChain
 * \return     -:                       -
 *                                      or wrong parameters
 * \see
 * \note    Function is called after TJA1040/TJA1041/.. reaches normal mode. It checks the
 *          WakeupPin as it displays  wake-up source
 *          on local wakeup (wakeup pin) the internal  wake-up source flag is set and can be read out
 *          ! There shall be NO communication between mode change to normal and call of this function !
 ***************************************************************************************************
 */
#if (RBA_TRCV_CHANGE_MODE_FUNCTION2_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_ChangeModeFunction2(uint8 rba_Trcv_TrcvIdx)
{
# if (RBA_TRCV_CFG_WAKEUP_BY_NODE_USED == STD_ON)
    if(rba_Trcv_WakeupState_au8[rba_Trcv_TrcvIdx] == 1) // rba_Trcv_WakeupState_au8 is set in rba_Trcv_ChangeModeFunction1
    {
        if(STD_LOW == (Dio_ReadChannel(RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WakePinPos].Pin)
                       ^ (RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WakePinPos].Invert)))
        {
            rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] = RBA_TRCV_WU_BY_PIN;
        }
        else
        {
            rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] = RBA_TRCV_WU_BY_BUS;
        }
    }
    return;
#else
    (void) (rba_Trcv_TrcvIdx);    // RBA_TRCV_PRV_PARAM_UNUSED
# endif
}
#endif
/*
***************************************************************************************************
 * \moduledescription
 * This function reads wakeup reason by evaluation of rba_Trcv_GetTransceiverError and sets rba_Trcv_WakeupReason_au8
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx            identifies Transceiver to which the API has to be applied
 * \return  -
 *
 * \see
 * \note    Function is called before TJA1080,TJA1081,TJA1082; NCV7383 reaches normal mode.
 ***************************************************************************************************
 */
#if (RBA_TRCV_CHANGE_MODE_FUNCTION3_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_ChangeModeFunction3(uint8 rba_Trcv_TrcvIdx)
{
    uint32 ErrorState_u32;

    if((rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_WU_NOT_SUPPORTED) ||
       (rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] == RBA_TRCV_WU_INTERNALLY))
    {
        /*  Read hardware-bits. E.g. for AS8221 there is a mode change to ReceiveOnly before this function is called
            and there is a mode check inside rba_Trcv_GetTransceiverMode. Therefore the rba_Trcv_Prv_Mode_au8 has to be
            up to date before function call rba_Trcv_GetTransceiverMode
            Also the time is get here (because this function can be called between mode change and there should
            be no GETTIME after each pin change. So the GETTIME is here, even is some time lost */
        /* MR12 RULE 11.3 VIOLATION: Convertion of OS-Time to rba_Trcv_Time. As this is integrator code, different time-values-types has to be mapped to a general rba_Trcv-time type */
        RBA_TRCV_GETTIME(&rba_Trcv_StateChangeTime_ao[rba_Trcv_TrcvIdx]);   // store time when mode changed
        (void)rba_Trcv_GetTransceiverMode( rba_Trcv_TrcvIdx, &rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx]);

        // The wakeup reason is stored inside of function rba_Trcv_GetTransceiverError
        (void)rba_Trcv_GetTransceiverError( rba_Trcv_TrcvIdx, 0, &ErrorState_u32 );
    }
}
#endif
/**
 ***************************************************************************************************
 * \rba_Trcv_ChangeModeFunction4
 * This function reads Wakeupsignalisation Pin and sets wakeup state PWR on
 *
 *
 *
 * \param   uint8                       rba_Trcv_TrcvIdx
 * \arg     rba_Trcv_TrcvIdx            identifies Transceiver to which the API has to be applied
 *                                      Value points to table rba_Trcv_SetModeStructChain
 * \return     -   :                    -
 *
 * \see
 * \note    Function is called after TJA1040/TJA1041/.. is in PWON/ListenOnlyMode and reads out PWR-Flag on WakePin.
 *          It read Wakeup-Pin as it displays  PWR-ON wake-up state.
 *          Wakeup flag is set and can be read out. Wakeup flag is cleared in normal mode
 ***************************************************************************************************
 */
#if (RBA_TRCV_CHANGE_MODE_FUNCTION4_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_ChangeModeFunction4(uint8 rba_Trcv_TrcvIdx)
{
    /* Read WakePin from Transceiver before normal mode and store value as it is cleared in normal mode */
# if (RBA_TRCV_CFG_WAKEUP_BY_NODE_USED == STD_ON)
    if(STD_LOW == (Dio_ReadChannel(RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WakePinPos].Pin)
                    ^ RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].WakePinPos].Invert))
    {
        rba_Trcv_WakeupReason_au8[rba_Trcv_TrcvIdx] = RBA_TRCV_WU_POWER_ON; /* 0: no wakeup; 1=wakeup detected */
    }
    return;
#else
    (void) (rba_Trcv_TrcvIdx);    // RBA_TRCV_PRV_PARAM_UNUSED
# endif
}
#endif
/**
 ***************************************************************************************************
 * \rba_Trcv_ChangeModeFunction4
 * This function reads Wakeupsignalisation Pin and sets wakeup state PWR on
 *
 *
 *
 * \arg     rba_Trcv_TrcvIdx            identifies Transceiver to which the API has to be applied
 * \arg     rba_Trcv_Level              0= Low, 1= High
 * \return     -   :                    -
 *
 * \see
 * \note    Function is called after TJA1040/TJA1041/.. is in PWON/ListenOnlyMode and reads out PWR-Flag on WakePin.
 *          It read Wakeup-Pin as it displays  PWR-ON wake-up state.
 *          Wakeup flag is set and can be read out. Wakeup flag is cleared in normal mode
 ***************************************************************************************************
 */
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_GetErrorPin(uint8 rba_Trcv_TrcvIdx, uint8 *rba_Trcv_Level)
{

    // Check all input parameters
    if(rba_Trcv_Level != NULL_PTR)
    {
        /* Read ErrorPin from Transceiver and return value */
        *rba_Trcv_Level = 0;
        if(STD_HIGH == (Dio_ReadChannel(RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].ErrorPinPos].Pin)
                          ^ (RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].ErrorPinPos].Invert)))
        {
            *rba_Trcv_Level = 1;
        }

    }
    else
    {
        RBA_TRCV_DET_REPORTERROR(rba_Trcv_TrcvIdx, RBA_TRCV_SID_GETERRORPIN, RBA_TRCV_E_PARAM_POINTER)
    }

}

/*
***************************************************************************************************
 * \moduledescription
 * This function checks Latest mode with the curent mmode of Transceiver.
 * If latest requested mode is not matching with the current mode then function will report DEM Error
 * \param   rba_Trcv_TrcvIdx. If0, then FrTrcv is searched, otherwise the No is used
 * \return  -
 * \see
 * \note    Function is always commented out. Can be commented in and called from testcode
 ***************************************************************************************************
 */
#if (RBA_TRCV_MODE_PROPOGATION_REPORTING == STD_ON)
FUNC (void, RBA_TRCV_CODE)rba_Trcv_CheckModePropagation(uint8 rba_Trcv_TrcvIdx)
{
    /*Get the current mode of Transceiver by reading DIO Pin level*/
    rba_Trcv_TrcvModeType_ten rba_Trcv_CurrentMode = RBA_TRCV_TRCVMODE_UNDEF;

//    rba_Trcv_DemStruct_tst const *no_p = &rba_Trcv_DemStruct[RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].PropDemPos];

    /*If the previous requested state is sleep then Popogation of mode change may be invalid as
     * Transciver may autamatically transit to standby mode on reception of wakeup pusle
     * and this transition is valid */
    (void)rba_Trcv_GetTransceiverMode(rba_Trcv_TrcvIdx, &rba_Trcv_CurrentMode);
    if(rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx] != RBA_TRCV_TRCVMODE_SLEEP)
    {
        if(rba_Trcv_Prv_Mode_au8[rba_Trcv_TrcvIdx] != rba_Trcv_CurrentMode)
        {
            /*Report Dem Fail condition*/
//            no_p->Dem_ReportErrorStatusFunction(no_p->EventId, DEM_EVENT_STATUS_PREFAILED );
            rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].PropDemPos, DEM_EVENT_STATUS_PREFAILED);
        }
        else
        {
            /*Report Dem Pass condition*/
//            no_p->Dem_ReportErrorStatusFunction(no_p->EventId, DEM_EVENT_STATUS_PREPASSED );
            rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].PropDemPos, DEM_EVENT_STATUS_PREPASSED);
        }
    }
    else
    {
        if(rba_Trcv_CurrentMode == RBA_TRCV_TRCVMODE_NORMAL)
        {
            /*Report Dem Fail condition*/
//            no_p->Dem_ReportErrorStatusFunction(no_p->EventId, DEM_EVENT_STATUS_PREFAILED );
            rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].PropDemPos, DEM_EVENT_STATUS_PREFAILED);
        }
        else
        {
            /*Report Dem Pass condition*/
//            no_p->Dem_ReportErrorStatusFunction(no_p->EventId, DEM_EVENT_STATUS_PREPASSED );
            rba_Trcv_Dem_ReportErrorStatusFunction(RBA_TRCV_CONFIGDATA[rba_Trcv_TrcvIdx].PropDemPos, DEM_EVENT_STATUS_PREPASSED);
        }

    }
}
#endif

/******************************************************************************************************/

#if (RBA_TRCV_FR_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_SchM_Enter_FrTrcv_Resource(void)
{
    SchM_Enter_FrTrcv_RESOURCE;
}
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_SchM_Exit_FrTrcv_Resource(void)
{
    SchM_Exit_FrTrcv_RESOURCE;
}
#endif
#if (RBA_TRCV_CAN_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_SchM_Enter_CanTrcv_Resource(void)
{
    SchM_Enter_CanTrcv_RESOURCE;
}
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_SchM_Exit_CanTrcv_Resource(void)
{
    SchM_Exit_CanTrcv_RESOURCE;
}
#endif
#if (RBA_TRCV_LIN_USED == STD_ON)
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_SchM_Enter_LinTrcv_Resource(void)
{
    SchM_Enter_LinTrcv_RESOURCE;
}
FUNC(void,RBA_TRCV_APPL_CODE) rba_Trcv_SchM_Exit_LinTrcv_Resource(void)
{
    SchM_Exit_LinTrcv_RESOURCE;
}
#endif


/*
***************************************************************************************************
 * \moduledescription
 * This function calls DEM with appropriate parameters.
 * \param   rba_Trcv_TrcvIdx. corresponding Trcv_index
 * \param   rba_Trcv_Dem_EventStatus  Dem status
 * \return  -
 *
 * \see
 * \note    Function is only required if DEM is configured. Main task is to check if EventId equal 0
 ***************************************************************************************************
 */
#if RBA_TRCV_CFG_DEM_ENABLED == STD_ON
static FUNC (void, RBA_TRCV_CODE) rba_Trcv_Dem_ReportErrorStatusFunction(uint16 DemPos, Dem_EventStatusType rba_Trcv_Dem_EventStatus)
{

    /* points to first DEM for Trcv. This is always NO_TRCV_CONTROL */
    rba_Trcv_DemStruct_tst const *no_p = &rba_Trcv_DemStruct[DemPos];
    if(no_p->EventId !=0u)
    {
        no_p->Dem_ReportErrorStatusFunction(no_p->EventId, rba_Trcv_Dem_EventStatus );
    }

    return;
}
#endif
/*
***************************************************************************************************
 * \moduledescription
 * This function checks timing. Only for development and integration relevant
 * It switches the enable pin of first Dio-FR
 * high then,
 * wait 10us then
 * switch Pin to low, to high and low again, then start again
 * this is done for 100 times
 * \param   rba_Trcv_TrcvIdx. If0, then FrTrcv is searched, otherwise the No is used
 * \return  -
 *
 * \see
 * \note    Function is always commented out. Can be commented in and called from testcode
 ***************************************************************************************************
 */
#if 0
FUNC(Std_ReturnType, RBA_TRCV_APPL_CODE) rba_Trcv_TestTiming(uint8 rba_Trcv_TrcvIdx )
// Testcode to check timing
{
    Std_ReturnType RetVal = E_NOT_OK;
    uint32  count=0;
    uint8   trcvNo;
    rba_Trcv_Cfg_Tick_to TimestampTmp;
    rba_Trcv_Cfg_Tick_to TimestampWait;
    Dio_ChannelType  const *Dio_po;


    // Search for first FrTrcv with Dio
    if (rba_Trcv_TrcvIdx == 0)
    {
        for(trcvNo = 0; trcvNo < RBA_TRCV_CFG_NUMBER_OF_TRCV; trcvNo++)
        {
            if((RBA_TRCV_CONFIGDATA[trcvNo].ConnectionType == RBA_TRCV_CONNECTIONTYPE_DIO) &&
                    (RBA_TRCV_CONFIGDATA[trcvNo].BusType == 2 ))      //  0=CAN, 1=LIN, 2=Fr
            {
                break;
            }
        }
    }
    else
    {
        trcvNo = rba_Trcv_TrcvIdx;
    }
    // If a Trcv was found, then apply the test
    if(trcvNo != RBA_TRCV_CFG_NUMBER_OF_TRCV)
    {
        /* MR12 RULE 11.3 VIOLATION: Convertion of OS-Time to rba_Trcv_Time. As this is integrator code, different time-values-types has to be mapped to a general rba_Trcv-time type */
        RBA_TRCV_GETTIME(&TimestampTmp);
        Dio_po = &(RBA_TRCV_DIOARRAY[RBA_TRCV_CONFIGDATA[trcvNo].StartDio].Pin);
        TimestampWait= 0;
        do
        {
            Dio_WriteChannel(Dio_po[RBA_TRCV_DIO_CLK_POS], STD_HIGH);
            TimestampWait += 10;
            count++;
            rba_Trcv_WaitUs(TimestampTmp, TimestampWait);
            Dio_WriteChannel(Dio_po[RBA_TRCV_DIO_CLK_POS], STD_LOW);
            Dio_WriteChannel(Dio_po[RBA_TRCV_DIO_CLK_POS], STD_HIGH);
            Dio_WriteChannel(Dio_po[RBA_TRCV_DIO_CLK_POS], STD_LOW);
        }while(count < 100);
        RetVal = E_OK;
    }
    return RetVal;
}

#endif //rba_Trcv_TestTiming

#ifdef RBA_TRCV_MEMMAP_USED
# define RBA_TRCV_STOP_SEC_CODE

# include "rba_Trcv_MemMap.h"
#endif
#endif  // RBA_TRCV_CFG_CONFIGURED
