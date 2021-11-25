


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"


/*********************************************************************************************************************
 * Function name : CanIf_SetControllerMode
 * Syntax        : FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllerMode(
 *                                                              VAR (uint8,AUTOMATIC)ControllerId,
 *                                                              VAR(CanIf_ControllerModeType,AUTOMATIC) ControllerMode)
 * Description   : This service calls the corresponding CAN driver service for changing of the CAN controller mode.
 * Parameter     : Controller, ControllerMode
 * Return value  : E_OK / E_NOT_OK
 *********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllerMode(
                                                            VAR (uint8,AUTOMATIC) ControllerId,
                                                            VAR(CanIf_ControllerModeType,AUTOMATIC) ControllerMode
                                                        )
{

    /* Pointer to Controller State configuration */
    P2VAR(CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC) lControllerState_p;

    VAR (Can_ReturnType, AUTOMATIC   ) lCanStatus_en        = CAN_NOT_OK;
    VAR (Std_ReturnType, AUTOMATIC   ) lRetVal_en           = E_NOT_OK;


    /* Transition mode of the can driver */
    VAR(Can_StateTransitionType,AUTOMATIC) lCanMode;

    P2CONST(CanIf_Cfg_CtrlConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lCtrlConfig_pst;

    VAR(uint8, AUTOMATIC)          lCtrlCustId_u8;


    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_SET_CONTROLLER_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_SET_CONTROLLER_SID, CANIF_E_PARAM_CONTROLLERID)

    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_SET_CONTROLLER_SID, CANIF_E_PARAM_CONTROLLERID)

    /* If Controller mode is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((CANIF_CS_STARTED != ControllerMode) && (CANIF_CS_STOPPED != ControllerMode) && \
                (CANIF_CS_SLEEP != ControllerMode), CANIF_SET_CONTROLLER_SID, CANIF_E_PARAM_CTRLMODE)

    /*Get current status of the requested controller*/
    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;
    /*Get configured data of requested controller*/
    lCtrlConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + lCtrlCustId_u8;


    switch(ControllerMode)
    {

        case CANIF_CS_SLEEP:

            lCanStatus_en = Can_SetControllerMode((uint8)(lCtrlConfig_pst->CtrlCanCtrlRef), CAN_T_SLEEP);
            if(lCanStatus_en != CAN_NOT_OK)
            {
                lRetVal_en = E_OK;

                /*Set PduMode to CANIF_OFFLINE*/
                lControllerState_p->ChannelMode = CANIF_OFFLINE;


#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
                /*Check for the flag at least one pdu linked to this controller is a pnfilter pdu*/
                if((lCtrlConfig_pst->PnCtrlEn == TRUE) && (lControllerState_p->BlPnTxFilter != TRUE))
                {
                    /*As per requirement [SWS_CANIF_00749] PnTxFilter shall be enabled */
                    lControllerState_p->BlPnTxFilter = TRUE;
                }

#endif
            #if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
                lControllerState_p->ValidationSts_b = CANIF_RESET;
            #endif
#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)

                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();

                /*If buffering is enabled, clear all the buffers assigned to requested controller*/
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
            }

            break;


        case CANIF_CS_STARTED:

            lCanStatus_en = Can_SetControllerMode((uint8)(lCtrlConfig_pst->CtrlCanCtrlRef), CAN_T_START);
            if(lCanStatus_en!= CAN_NOT_OK)
            {
               #if (CANIF_LITE_CONFIGURATION == STD_ON)
               /*Set PduMode to CANIF_ONLINE*/
               //lControllerState_p->ChannelMode = CANIF_ONLINE;
               lControllerState_p->DeviceMode  = ControllerMode;
               #endif
               lRetVal_en = E_OK;
            }

            break;


        case CANIF_CS_STOPPED:

            if(lControllerState_p->DeviceMode == CANIF_CS_SLEEP)
            {
                lCanMode = CAN_T_WAKEUP;        /*SWS_CANIF_00487*/
            }
            else
            {
                lCanMode = CAN_T_STOP;          /*[SWS_CANIF_00480], [SWS_CANIF_00488]*/
            }

            lCanStatus_en = Can_SetControllerMode((uint8)(lCtrlConfig_pst->CtrlCanCtrlRef), lCanMode);
            if(lCanStatus_en != CAN_NOT_OK)
            {
                lRetVal_en = E_OK;

                /*Set PduMode to CANIF_TX_OFFLINE*/
                lControllerState_p->ChannelMode = CANIF_TX_OFFLINE;

#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();

                /*If buffering is enabled, clear all the buffers assigned to requested controller*/
                CanIf_Prv_ClearTxChannelBuffer(lCtrlCustId_u8);

                /* Enter Critical Section */
                SchM_Exit_CanIf_TxBufAccessNoNest();
#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
                /* If XCore is enabled, Clear the pipe packets assigned to requested controller in Tx and Tx confirmation pipes */
                CanIf_XCore_ClearPipes(lCtrlCustId_u8);
#endif
/* FC_VariationPoint_END */

            }
            break;
        default:
            /* Intentionally Empty */
            break;
    }

return(lRetVal_en);
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"




/**********************************************************************************************************************
 * Function name    : CanIf_GetControllerMode
 * Syntax           : FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerMode(
 *                                         VAR(uint8,AUTOMATIC) ControllerId,
 *                                         P2VAR(CanIf_ControllerModeType, AUTOMATIC, CANIF_APPL_DATA) ControllerModePtr
 *                                                                              )
 *
 * Description      : Service reports about the current status of the requested CAN Controller.
 * Parameter        : Controller, ControllerModePtr
 * Return value     : E_OK / E_NOT_OK
 **********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerMode(
                                           VAR(uint8,AUTOMATIC) ControllerId,
                                           P2VAR(CanIf_ControllerModeType, AUTOMATIC, CANIF_APPL_DATA) ControllerModePtr
                                                        )
{

    /* Pointer to Controller State configuration */
    P2VAR(CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC) lControllerState_p;
#if CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON
    VAR(uint8, AUTOMATIC)          lCtrlCustId_u8;
#endif
    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_GET_CONTROLLER_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_GET_CONTROLLER_SID, \
                                                                            CANIF_E_PARAM_CONTROLLERID)
#if CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_GET_CONTROLLER_SID, \
                                                                            CANIF_E_PARAM_CONTROLLERID)

    /* If ControllerModePtr is null, report to DET and return E_NOT_OK*/
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == ControllerModePtr), CANIF_GET_CONTROLLER_SID,CANIF_E_PARAM_POINTER)

    /*Get current status of the requested controller*/
    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;


    /* Copy the status of the controller */
    *ControllerModePtr = lControllerState_p->DeviceMode;


    return E_OK;
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"



/***********************************************************************************************************************
 * Function name    : CanIf_ControllerBusOff
 * Syntax           : FUNC(void, CANIF_CODE) CanIf_ControllerBusOff(
                                                                    VAR(uint8,AUTOMATIC) ControllerId
                                                                    )
 * Description      : Service indicates a CAN controller BusOff event referring to the corresponding
                      CAN controller. This service is called by the CAN driver.
 * Parameter        : Controller
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

FUNC(void, CANIF_CODE) CanIf_ControllerBusOff(
                                                VAR(uint8,AUTOMATIC) ControllerId
                                             )

{

    /* Pointer to Controller State configuration */
    P2VAR(CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC) lControllerState_p;
#if ((CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)||(CANIF_PUBLIC_TXBUFFERING == STD_ON) || (CANIF_XCORE_CFG_ENABLED != STD_OFF))
    VAR(uint8, AUTOMATIC)          lCtrlCustId_u8;
#endif
    /*Pointer to the structure CanIf_CallbackFuncType */
    P2CONST(CanIf_CallbackFuncType, AUTOMATIC, AUTOMATIC) lCallBackPtr_p;


    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_CONTROLLER_BUSOFF_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_VOID_RET((ControllerId >= CANIF_TOTAL_CTRLS), \
                                                                    CANIF_CONTROLLER_BUSOFF_SID,CANIF_E_PARAM_CONTROLLER)
#if ((CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)||(CANIF_PUBLIC_TXBUFFERING == STD_ON)||(CANIF_XCORE_CFG_ENABLED != STD_OFF))
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_VOID_RET((lCtrlCustId_u8 == 0xFFu), CANIF_CONTROLLER_BUSOFF_SID,CANIF_E_PARAM_CONTROLLER)



    /*Get current status of the requested controller*/
    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

    /*Get the address of structure having dispatch function pointers*/
    lCallBackPtr_p = &CanIf_Callback;

    /* Enter Critical Section */
    SchM_Enter_CanIf_ControllerState();

    if(lControllerState_p->DeviceMode != CANIF_CS_UNINIT)
    {
        lControllerState_p->DeviceMode = CANIF_CS_STOPPED;
    }


#if CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT!= STD_OFF

    /* Reset Tx Confirmation state for the controller*/
    lControllerState_p->CanIf_TxCnfmStatus = CANIF_NO_NOTIFICATION;

#endif

    /*Set PduMode to CANIF_TX_OFFLINE*/
    lControllerState_p->ChannelMode = CANIF_TX_OFFLINE;

    /* Leave Critical Section */
    SchM_Exit_CanIf_ControllerState();

#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
    /* Enter Critical Section */
    SchM_Enter_CanIf_TxBufAccessNoNest();

    /*If buffering is enabled, clear all the buffers assigned to requested controller*/
    CanIf_Prv_ClearTxChannelBuffer(lCtrlCustId_u8);

    /* Leave Critical Section */
    SchM_Exit_CanIf_TxBufAccessNoNest();

#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
    /* If XCore is enabled, Clear the pipe packets assigned to requested controller in Tx and Tx confirmation pipes */
    CanIf_XCore_ClearPipes(lCtrlCustId_u8);
#endif
/* FC_VariationPoint_END */

    if(lCallBackPtr_p->User_ControllerBusOff != NULL_PTR)
    {
        /*Invoke User_ControllerBusOff for configured user(CANSM or CDD)*/
        lCallBackPtr_p->User_ControllerBusOff(ControllerId);
    }

} /* End of CanIf_ControllerBusOff() */
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/***********************************************************************************************************************
 * Function name    : CanIf_ControllerModeIndication
 * Syntax           : FUNC(void, CANIF_CODE) CanIf_ControllerModeIndication(
                                                                    VAR(uint8,AUTOMATIC) ControllerId,
                                                                    CanIf_ControllerModeType ControllerMode
                                                                    )
 * Description      : Service indicates a mode change is successful for the particular
                      CAN controller. This service is called by the CAN driver.
 * Parameter        : Controller
 * Return value     : None
 **********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

FUNC(void, CANIF_CODE) CanIf_ControllerModeIndication(  VAR(uint8,AUTOMATIC) ControllerId,
                                                        VAR(CanIf_ControllerModeType,AUTOMATIC) ControllerMode
                                                    )
{
    /* Pointer to Controller State configuration */
    P2VAR(CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC) lControllerState_p;
    /*Pointer to the structure CanIf_CallbackFuncType */
    P2CONST(CanIf_CallbackFuncType, AUTOMATIC, AUTOMATIC) lCallBackPtr_p;
#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    VAR(uint8, AUTOMATIC)          lCtrlCustId_u8;
#endif

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_CONTROLLER_MODE_IND_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((ControllerId >= CANIF_TOTAL_CTRLS), \
                                                                CANIF_CONTROLLER_MODE_IND_SID,CANIF_E_PARAM_CONTROLLER)
#if (CANIF_PUBLIC_DEV_ERROR_DETECT == STD_ON)
    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];
#endif
    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((lCtrlCustId_u8 == 0xFFu), CANIF_CONTROLLER_MODE_IND_SID,CANIF_E_PARAM_CONTROLLER)


    lControllerState_p = CanIf_Prv_ControllerState_ast + ControllerId;

    /*Get the address of structure having dispatch function pointers*/
    lCallBackPtr_p =  &CanIf_Callback;

    if(ControllerMode != CANIF_CS_UNINIT)
    {
        /*Update the controller mode*/
        lControllerState_p->DeviceMode = ControllerMode;

    }

    /*Invoke User_ControllerBusOff for configured user(CANSM or CDD)*/

    if(lCallBackPtr_p->User_ControllerModeIndication != NULL_PTR)
    {
        /*Invoke User_ControllerBusOff for configured user(CANSM or CDD)*/
        lCallBackPtr_p->User_ControllerModeIndication(ControllerId, ControllerMode);
    }

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
