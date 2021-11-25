


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */


#include "CanIf_Prv.h"



/***********************************************************************************************************************
 * Function name    : CanIf_SetPduMode
 * Syntax           : FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetPduMode(
                                                                        VAR(uint8,AUTOMATIC)ControllerId,
                                                                        VAR(CanIf_PduModeType,AUTOMATIC)PduModeRequest
                                                                        )
 * Description      : This service sets the requested mode at all L-PDUs of the predefined CAN controller.
 * Parameter        : Controller, PduModeRequest
 * Return value     : E_OK / E_NOT_OK
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetPduMode(
                                                    VAR(uint8,AUTOMATIC) ControllerId,
                                                    VAR(CanIf_PduModeType,AUTOMATIC) PduModeRequest
                                                 )
{

    /* Pointer to Controller State configuration */
    P2VAR(CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC) lControllerState_p;

#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    P2CONST(CanIf_Cfg_CtrlConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lCtrlConfig_pst;
#endif
#if ((CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)||(CANIF_PUBLIC_TXBUFFERING == STD_ON)||(CANIF_PUBLIC_PN_SUPPORT == STD_ON) || (CANIF_XCORE_CFG_ENABLED != STD_OFF))
    VAR(uint8, AUTOMATIC)          lCtrlCustId_u8;
#endif
    VAR (Std_ReturnType, AUTOMATIC   ) lRetVal_en           = E_NOT_OK;

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_SET_PDU_MODE_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_SET_PDU_MODE_SID, \
            CANIF_E_PARAM_CONTROLLERID)
#if ((CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)||(CANIF_PUBLIC_TXBUFFERING == STD_ON)||(CANIF_PUBLIC_PN_SUPPORT == STD_ON)||(CANIF_XCORE_CFG_ENABLED != STD_OFF))
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_SET_PDU_MODE_SID, \
            CANIF_E_PARAM_CONTROLLERID)



    CANIF_DET_REPORT_ERROR_NOT_OK((CANIF_OFFLINE != PduModeRequest) && (CANIF_TX_OFFLINE != PduModeRequest) \
            && (CANIF_ONLINE != PduModeRequest)
/* MR12 RULE 1.3 VIOLATION: Warning is "Not Critical".Verified through testing that there is no undefined behaviour in reporting DET with usage of pre-processor inside macro  */
#if(CANIF_TXOFFLINEACTIVE_SUPPORT == STD_ON)
            && (CANIF_TX_OFFLINE_ACTIVE != PduModeRequest)
/* MR12 RULE 1.3 VIOLATION: Warning is "Not Critical".Verified through testing that there is no undefined behaviour in reporting DET with usage of pre-processor inside macro  */
#endif
/* MR12 RULE 1.3 VIOLATION: Warning is "Not Critical".Verified through testing that there is no undefined behaviour in reporting DET with usage of pre-processor inside macro  */
#if(CANIF_USER_TP_TX_OFFLINE_MODE == STD_ON)
            && (CANIF_TX_TP_ONLINE != PduModeRequest) && (CANIF_TX_USER_TP_ONLINE != PduModeRequest)
/* MR12 RULE 1.3 VIOLATION: Warning is "Not Critical".Verified through testing that there is no undefined behaviour in reporting DET with usage of pre-processor inside macro  */
#endif
            , CANIF_SET_PDU_MODE_SID, CANIF_E_PARAM_PDU_MODE)



#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    lCtrlConfig_pst           = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + lCtrlCustId_u8;
#endif
    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

    if(lControllerState_p->DeviceMode!= CANIF_CS_STARTED)               /*[SWS_CANIF_00874]*/
    {
        lRetVal_en = E_NOT_OK;
    }
    else
    {

        switch(PduModeRequest)
        {
            case CANIF_OFFLINE:
#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();

                CanIf_Prv_ClearTxChannelBuffer(lCtrlCustId_u8);

                /* Leave Critical Section */
                SchM_Exit_CanIf_TxBufAccessNoNest();
#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)

                /* Flag set to True to clear all Pipes */
                CanIf_XCore_RxClrPipeFlag_b = TRUE;
                /* If XCore is enabled, Clear the pipe packets assigned to requested controller in Tx and Tx confirmation pipes */
                CanIf_XCore_ClearPipes(lCtrlCustId_u8);
#endif
/* FC_VariationPoint_END */

                break;

            case CANIF_TX_OFFLINE:
#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();

                CanIf_Prv_ClearTxChannelBuffer(lCtrlCustId_u8);

                /* Leave Critical Section */
                SchM_Exit_CanIf_TxBufAccessNoNest();
#endif

#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
                /*Check for the flag at least one pdu linked to this controller is a pnfilter pdu*/

                if((lCtrlConfig_pst->PnCtrlEn == TRUE) && (lControllerState_p->BlPnTxFilter != TRUE))
                {
                    lControllerState_p->BlPnTxFilter = TRUE;
                }

#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
                /* If XCore is enabled, Clear the pipe packets assigned to requested controller in Tx and Tx confirmation pipes */
                CanIf_XCore_ClearPipes(lCtrlCustId_u8);
#endif
/* FC_VariationPoint_END */
                break;

          /*This is for sake of removing warning of switch-case*/
            case CANIF_ONLINE:
#if(CANIF_TXOFFLINEACTIVE_SUPPORT == STD_ON)
            case CANIF_TX_OFFLINE_ACTIVE:
#endif

#if(CANIF_USER_TP_TX_OFFLINE_MODE != STD_OFF)
            case CANIF_TX_TP_ONLINE:
            case CANIF_TX_USER_TP_ONLINE:
#endif
                break;

            default:
                /* Intentionally Empty */
                break;

        }


        lControllerState_p->ChannelMode = PduModeRequest;


        lRetVal_en = E_OK;
    }
    return(lRetVal_en);
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/***********************************************************************************************************************
 * Function name    : CanIf_GetPduMode
 * Syntax           : FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetPduMode(
                                                                         VAR(uint8,AUTOMATIC) ControllerId,
                                                                         P2VAR(CanIf_PduModeType, AUTOMATIC,CANIF_APPL_DATA) PduModePtr
                                                                       )
 * Description      : This service reports the current mode of the requested Controller.
 * Parameter        : Controller, ChannelModePtr
 * Return value     : E_OK / E_NOT_OK
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetPduMode(
                                                    VAR(uint8,AUTOMATIC) ControllerId,
                                                    P2VAR(CanIf_PduModeType, AUTOMATIC,CANIF_APPL_DATA) PduModePtr
                                                 )
{

    /* Pointer to Controller State configuration */
    P2VAR(CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC) lControllerState_p;
#if(CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    VAR(uint8, AUTOMATIC)          lCtrlCustId_u8;
#endif

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_GET_PDU_MODE_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_GET_PDU_MODE_SID, \
            CANIF_E_PARAM_CONTROLLERID)
#if(CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_GET_PDU_MODE_SID, \
            CANIF_E_PARAM_CONTROLLERID)

    /* If PduModePtr is null, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == PduModePtr), CANIF_GET_PDU_MODE_SID, CANIF_E_PARAM_POINTER)

    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

    /*Copy the PduMode to out parameter*/
    *PduModePtr = lControllerState_p->ChannelMode;

    return E_OK;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
