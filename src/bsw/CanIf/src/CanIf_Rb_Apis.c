

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"

/**********************************************************************************************************************/
/* Function name    : CanIf_DirectHw_Write
   Syntax           : FUNC(Std_ReturnType, CANIF_CODE) CanIf_DirectHw_Write
                                                    (P2VAR(uint32, AUTOMATIC, CANIF_APPL_DATA) p_tx_data_buff_u32,
                                                    VAR(uint32, AUTOMATIC) data_len_u32,
                                                    P2VAR(rba_CanDirectHWInfo_o, AUTOMATIC, CANIF_APPL_DATA) hw_info_po)
 Description      : This service initiates a request for direct HW write for CAN L-PDU specified by the CanTxPduId
 Parameter        : p_tx_data_buff_u32, data_len_u32, hw_info_pco
 Return value     : E_OK / E_NOT_OK

 Notes:
 - This is not an AUTOSAR function.
 - This function must be AS FAST AS POSSIBLE.
 - The only check perofmed is to avoid sending data while CAN Drv not confirmed "running"  (Device mode == STARTED)
   More background info:
    XCP is the only user of this. Not checking if CAN iF is "online" is acceptable because:
    - XCP is used only in diagnose environment (not in "drive software"
    - there are scenarios where the XCP diag data is actually expected as fast as possible,
    thus may actually be desirable to send before BSWM goes online
***********************************************************************************************************************/
#if (CANIF_DIRECT_HW_WRITE == STD_ON)


#define CANIF_START_SEC_CODE_FAST
#include "CanIf_MemMap.h"

FUNC(Std_ReturnType, CANIF_CODE) CanIf_DirectHw_Write (P2CONST(uint32, AUTOMATIC, CANIF_APPL_DATA) p_tx_data_buff_u32,
    VAR(uint32, AUTOMATIC) data_len_u32,
    P2CONST(rba_CanDirectHWInfo_o, AUTOMATIC, CANIF_APPL_DATA) hw_info_po)
{
    VAR (uint8, AUTOMATIC) lControllerId_u8    = 0;
    VAR(uint16, AUTOMATIC)          ltxPduCustId_t;

    /*To store the controller state (Device mode)*/
    VAR (   CanIf_ControllerModeType, AUTOMATIC                 ) lDeviceMode_en;

    VAR (Std_ReturnType, AUTOMATIC ) Status;

    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[hw_info_po->swPduHandle];
   /*Make a local copy of Device Mode. Try to get the CAN Drv controller state*/
    lControllerId_u8 =  CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr[ltxPduCustId_t].CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->CanIf_CtrlConfigPtr->CtrlId;


    lDeviceMode_en = CanIf_Prv_ControllerState_ast[lControllerId_u8].DeviceMode;


    if (CANIF_CS_STARTED == lDeviceMode_en)
    {
        Status = rba_Can_DirectHw_Write (p_tx_data_buff_u32, data_len_u32, hw_info_po);
    }
    else
    {
        Status = E_NOT_OK;
    }

    return Status;
}
#define CANIF_STOP_SEC_CODE_FAST
#include "CanIf_MemMap.h"

#endif

/* FC_FileVariationPoint_END */

/*
 ***************************************************************************************************
 * used functions
 ***************************************************************************************************
 */
/**********************************************************************************************************************/
/* Function name    : CanIf_Get_DirectHw_Info                                                                        */
/* Syntax           : FUNC(Std_ReturnType, CANIF_CODE) CanIf_Get_DirectHw_Info
*                                                    (
*                                                    VAR(PduIdType, AUTOMATIC) CanTxPduId,
                                                     P2VAR(rba_CanDirectHWInfo_o, AUTOMATIC, CANIF_APPL_DATA) hw_info_po
                                                     )                                                              */
/* Description      : This function returns the needed info for the HW direct access to CAN.
                      Extract the PDU handle from CanIf configuration structure (variant dependent)
                      Then call rba_Can_Get_DirectHw_Info() to fill further information in the info structure.   */
/* Parameter        : CanTxPduId, hw_info_pco                                                                */
/* Return value     : E_OK / E_NOT_OK                                                                       */
/**********************************************************************************************************************/

#if (CANIF_DIRECT_HW_WRITE == STD_ON)

#define CANIF_START_SEC_CODE_FAST
#include "CanIf_MemMap.h"

FUNC(Std_ReturnType, CANIF_CODE) CanIf_Get_DirectHw_Info (VAR(PduIdType, AUTOMATIC) CanTxPduId,
                                                  P2VAR(rba_CanDirectHWInfo_o, AUTOMATIC, CANIF_APPL_DATA) hw_info_po

                                                                )
{
    /* Pointer to Tx Pdu configuration */
    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lTxPduConfig_pst;

    VAR(uint16, AUTOMATIC)          ltxPduCustId_t;

    VAR(Can_PduType,AUTOMATIC) PduInfo_Write;
    VAR(Std_ReturnType,AUTOMATIC) Status;
    /* FC_VariationPoint_START */
   #if (CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF)
    /* FC_VariationPoint_END */
    P2VAR(uint32,AUTOMATIC,AUTOMATIC) DynIdPtr_Temp;
   #endif

    /*Initialise the return value*/
    Status = E_NOT_OK;


    /* FC_VariationPoint_START */
    #if (CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF)
    /* FC_VariationPoint_END */
    DynIdPtr_Temp = CanIf_DynTxPduCanId_au32;
    #endif

    /*Since there is no DET implemented for this API, the caller should always pass valid CanTxPduId parameter*/
    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanTxPduId];

    if(ltxPduCustId_t == 0xFFFFu)
    {
        /*Do nothing*/
    }
    else
    {
        lTxPduConfig_pst = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;

       /*Get the L-PDU handle info from TxPduInfo*/
        hw_info_po->swPduHandle = lTxPduConfig_pst->TxPduId;
        hw_info_po->Handle_uo = lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->CanObjectId;

        /* Get the Sdu Handle*/
        PduInfo_Write.swPduHandle = hw_info_po->swPduHandle;

        #if (CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF)
        /* Check if the CanTxPduId is configured as Dynamic */
        if(lTxPduConfig_pst->TxPduType != CANIF_STATIC)
        {

            PduInfo_Write.id = (Can_IdType)*(DynIdPtr_Temp + (lTxPduConfig_pst->TxPduType));

        }
        else
        #endif
        {
            /* Get the CAN ID from Can Tx pdu info to outgoing CAN Message structure */

                PduInfo_Write.id = (Can_IdType)((lTxPduConfig_pst->TxPduCanId));

            /* FC_VariationPoint_START */
            /* setting two most significant bit based on CanIdType */
                PduInfo_Write.id &= CANIF_MSB_BIT_RESET;
                PduInfo_Write.id |= (((Can_IdType)(lTxPduConfig_pst->TxPduCanIdType))<<CANIF_CANID_BIT_SHIFT);
            /* FC_VariationPoint_END*/
        }

        /*Call rba_Can_Get_DirectHw_Info()*/
        Status = rba_Can_Get_DirectHw_Info((Can_HwHandleType)hw_info_po->Handle_uo, &PduInfo_Write, hw_info_po);
    }
    return (Status);
}
#define CANIF_STOP_SEC_CODE_FAST
#include "CanIf_MemMap.h"

#endif

/* FC_FileVariationPoint_END */


/* FC_VariationPoint_START */
/****************************************************************************************************
 *                              Function Definition                                                 *
 ****************************************************************************************************/

/**********************************************************************************************************************
** Function name     : CanIf_ControllerErrorPassive                                                                  **
** Syntax            : void CanIf_ControllerErrorPassive(uint8 ControllerId)                                         **
** Service ID[hex]   : 0x29                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant(for different controllers)                                                          **
** Parameters (in)   : ControllerId                                                                                  **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : Service indicates a CAN controller Error Passive event referring to the corresponding CAN     **
**                     controller.                                                                                   **
**********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE) CanIf_ControllerErrorPassive(VAR(uint8, AUTOMATIC) ControllerId)
{
    /*The Macro is placed inside the API to have backward compatablity */
#if (CANIF_RB_ERROR_PASSIVE_SUPPORT != STD_OFF)
    P2CONST(CanIf_CallbackFuncType, AUTOMATIC, AUTOMATIC) CallBackPtr_Temp;

    /* If CAN Interface is uninitialized, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR((FALSE == CanIf_Prv_InitStatus_b), CANIF_CONTROLLER_ERRORPASSIVE_SID, CANIF_E_UNINIT)


    /*[CanIf_Ext-2831] If Controller is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_VOID_RET((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_CONTROLLER_ERRORPASSIVE_SID, CANIF_E_PARAM_CONTROLLER)

    /*Initializing callback pointer */
    CallBackPtr_Temp = &CanIf_Callback;

    /* Indicate application about error passive condition */
    if(NULL_PTR != CallBackPtr_Temp->User_ControllerErrorPassive)
    {
        CallBackPtr_Temp->User_ControllerErrorPassive(ControllerId);
    }
#else
    /*To avoid compiler warnings*/
    (void)ControllerId;
#endif
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* FC_FileVariationPoint_END */


/*
 ***************************************************************************************************
 * used functions
 ***************************************************************************************************
 */


/****************************************************************************************************************/
/* Function name    : CanIf_ResetDynamicTxId                                                                    */
/* Syntax           : FUNC(void, CANIF_CODE) CanIf_ResetDynamicTxId(VAR(PduIdType, AUTOMATIC) CanTxPduId        */
/* Description      : This service reconfigures the corresponding Static CanId of the requested CAN L-PDU.      */
/* Parameter        : CanTxPduId                                                                                */
/* Return value     : None                                                                                      */
/****************************************************************************************************************/

/* FC_FileVariationPoint_START */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE) CanIf_ResetDynamicTxId(VAR(PduIdType, AUTOMATIC) CanTxPduId)
{
#if ((CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF) && (CANIF_CFG_TX_FEATURE_ENABLED== STD_ON))
    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lTxPduConfig_pst;
    VAR(uint16, AUTOMATIC)          ltxPduCustId_t;
    VAR(Can_IdType, AUTOMATIC) LdCanIdType;

    P2VAR(uint32,AUTOMATIC,AUTOMATIC) lDynId_p;


    /* If CAN Interface is uninitialized, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR((FALSE == CanIf_Prv_InitStatus_b), CANIF_RESET_DYNAMIC_TXID_SID,CANIF_E_UNINIT)

    /* If CanTxPduId is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR((CanTxPduId >= CANIF_TOTAL_TXPDUS), CANIF_RESET_DYNAMIC_TXID_SID,CANIF_E_PARAM_LPDU)

    /* Initialize pointer to Tx L-PDU structure
     * Get the location by using the Base address of the Tx-Pdu structure*/
    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanTxPduId];

    /* If CanTxPduId is invalid for this variant, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR((ltxPduCustId_t == 0xFFFFu), CANIF_RESET_DYNAMIC_TXID_SID,CANIF_E_PARAM_LPDU)

    /*Get the address of config structure of requested PDU*/
    lTxPduConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;

    /* If PDU is of type STATIC, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR((CANIF_STATIC == lTxPduConfig_pst->TxPduType), CANIF_RESET_DYNAMIC_TXID_SID,CANIF_E_PARAM_LPDU)

    lDynId_p = CanIf_DynTxPduCanId_au32;

    /* Check if the CanTxPduId is configured as Dynamic.
     * Esnure that the config set has dynamic Ids configured */
    if(lTxPduConfig_pst->TxPduType != CANIF_STATIC)
    {

        /* Update the new CanId in the global array */

        LdCanIdType = (lTxPduConfig_pst->TxPduCanId);
        LdCanIdType &= CANIF_MSB_BIT_RESET;
        LdCanIdType |= (((Can_IdType)(lTxPduConfig_pst->TxPduCanIdType))<<CANIF_CANID_BIT_SHIFT);


        *(lDynId_p + (lTxPduConfig_pst->TxPduType)) = LdCanIdType;


    }
#else
    (void)CanTxPduId;
#endif
} /* End of CanIf_ResetDynamicTxId() */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* FC_FileVariationPoint_END */


/*
 ***************************************************************************************************
 * used functions
 ***************************************************************************************************
 */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
FUNC(Std_ReturnType, CANIF_CODE) CanIf_Rb_ReadTxPduCanId(
                                                    VAR(PduIdType,AUTOMATIC) CanIfTxPduId,
                                                    P2VAR(uint32, AUTOMATIC, AUTOMATIC) TxPduCanId)
{

    VAR(Std_ReturnType,AUTOMATIC) RetVal = E_NOT_OK;
    /* Pointer to Tx Pdu configuration */
    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lTxPduConfig_pst;
#if (CANIF_PUBLIC_SETDYNAMICTXID_API == STD_ON)
    P2VAR(uint32,AUTOMATIC,AUTOMATIC) lDynId_p;
#endif
    VAR (uint32, AUTOMATIC         ) lCanId_u32             = 0;
    VAR(uint16, AUTOMATIC)          ltxPduCustId_t;



    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_RB_READ_TXPDU_CANID, CANIF_E_UNINIT)

    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIfTxPduId >= CANIF_TOTAL_TXPDUS), \
                                                                    CANIF_RB_READ_TXPDU_CANID, CANIF_E_INVALID_TXPDUID)

    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanIfTxPduId];

    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ltxPduCustId_t == 0xFFFFu), CANIF_RB_READ_TXPDU_CANID, CANIF_E_INVALID_TXPDUID)


    /*Get the address of config structure of requested PDU*/
    lTxPduConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;

#if (CANIF_PUBLIC_SETDYNAMICTXID_API == STD_ON)
        if(lTxPduConfig_pst->TxPduType!= CANIF_STATIC)      /*DYNAMIC*/
        {
            /*Get the address of Dynamic CanId array*/
           lDynId_p = &CanIf_DynTxPduCanId_au32[0];

           /*Get the reconfigured CanId from Dynamic CanId array*/
           lCanId_u32 = (Can_IdType)*(lDynId_p + (lTxPduConfig_pst->TxPduType));


           RetVal = E_OK;
        }
        else
#endif
        {
            if(lTxPduConfig_pst->TxPduCanId!= 0xFFFFFFFFu)           /*Default value*/
            {
                /* Get the configured CanId */
#if (CANIF_RB_CALIBRATION == STD_OFF)
                lCanId_u32 = lTxPduConfig_pst->TxPduCanId;
#else
                lCanId_u32 = lTxPduConfig_pst->getTxPduCanId();
#endif
                lCanId_u32 &= CANIF_MSB_BIT_RESET;

                /* setting two most significant bit based on CanIdType */
                lCanId_u32 |= ( ((Can_IdType)lTxPduConfig_pst->TxPduCanIdType) << CANIF_CANID_BIT_SHIFT );
                RetVal = E_OK;
            }
        }
        if(RetVal == E_OK)
        {
            *TxPduCanId  = lCanId_u32;
        }

    return RetVal;  /*Returns E_NOT_OK if TxPduCanId is not configured in large metadatalength case*/
}
#endif
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* FC_VariationPoint_START */
/**********************************************************************************************************************
** Function name     : CanIf_Rb_ReadRxPduCanId                                                                       **
** Syntax            : Std_ReturnType CanIf_Rb_ReadRxPduCanId(PduIdType CanIfRxPduId, uint32 *RxPduCanId)            **
** Service ID[hex]   : 0x31                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : CanIfRxPduId                                                                                  **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : *RxPduCanId                                                                                   **
** Return value      : Std_ReturnType                                                                                **
** Description       : Service that retutrns the CanId when the CanIfRxPduId is given.                               **
**********************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
FUNC(Std_ReturnType, CANIF_CODE) CanIf_Rb_ReadRxPduCanId( VAR(PduIdType, AUTOMATIC)CanIfRxPduId,
        P2VAR(uint32, AUTOMATIC, CANIF_APPL_DATA)RxPduCanId)
{
    /*Return value*/
    VAR(Std_ReturnType, AUTOMATIC) Status_t = E_OK;

    /* Pointer to RxPdu configuration */
    P2CONST(CanIf_Cfg_RxPduType_tst, AUTOMATIC, CANIF_CFG_CONST) RxPduCfg_pcst;

/* FC_VariationPoint_START */
#if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
    /* Pointer to store address of Dynamic RxPduCanId array*/
    P2VAR(Can_IdType,AUTOMATIC,AUTOMATIC) DynRxPduIdPtr_Temp;
#endif
/* FC_VariationPoint_END */

    /*[CanIf_Ext-3110] Det should be invoked if module is not initialized */
    CANIF_DET_REPORT_ERROR_NOT_OK(CanIf_Prv_InitStatus_b == FALSE, CANIF_RB_READRXPDUCANID_SID,CANIF_E_UNINIT)

    /*[CanIf_Ext-3111]If PduId is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIfRxPduId>=CanIf_Prv_ConfigSet_tpst->NumCanRxPduId_t), CANIF_READ_RX_MSG_ID_SID,
            CANIF_E_INVALID_RXPDUID)

    CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanIfRxPduId]== INVALID_ID), CANIF_READ_RX_MSG_ID_SID,
                        CANIF_E_INVALID_RXPDUID)

    /* Get the pointer to desired Rx pdu */
    RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanIfRxPduId];

    /*[CanIf_Ext-3111]If PduId is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_NOT_OK(RxPduCfg_pcst->RxPduCanId == 0xFFFFFFFFu, CANIF_READ_RX_MSG_ID_SID, \
                                                                                    CANIF_E_INVALID_RXPDUID)

/* FC_VariationPoint_START */
#if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
    if((RxPduCfg_pcst ->RxPduType) != CANIF_STATIC)      /*DYNAMIC*/
    {
        /*Get the address of Dynamic CanId array*/
        DynRxPduIdPtr_Temp = &CanIf_DynRxPduCanId_au32[0];

        /*Get the CanId from Dynamic Rx-Pdu CanId array*/
        *RxPduCanId = (Can_IdType)*(DynRxPduIdPtr_Temp + (RxPduCfg_pcst ->RxPduType));
    }
    else
#endif
/* FC_VariationPoint_END */
    {
        /*Copy the CanId into the out parameter*/
        #if (CANIF_RB_CALIBRATION == STD_ON)
        if(RxPduCfg_pcst->getCanId !=NULL_PTR)
        {
            *RxPduCanId = (uint32)RxPduCfg_pcst->getCanId();
        }
        else
        {
            *RxPduCanId = (uint32)RxPduCfg_pcst->RxPduCanId;
        }
        #else
        *RxPduCanId = (uint32)RxPduCfg_pcst->RxPduCanId;
        #endif
    }

    return (Status_t);
}
#endif
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/* FC_VariationPoint_END */



/****************************************************************************************************************/
/* Function name    : CanIf_ReturnTxPduId                                                                       */
/* Syntax           :FUNC(Std_ReturnType, CANIF_CODE)CanIf_ReturnTxPduId(
                                                     VAR(PduIdType, AUTOMATIC) CanIfTxTargetPduId,
                                                    P2VAR(PduIdType, AUTOMATIC, CANIF_APPL_CONST) CanIfTxPduIdPtr
                                                    ) */
/* Description      :  Service that retutrns the PduId when the Target PduId is given.                                                             */
/* Parameter        : CanIfTxTargetPduId, CanIfTxPduIdPtr                                                                   */
/* Return value     : E_OK / E_NOT_OK                                                                           */
/****************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
FUNC(Std_ReturnType, CANIF_CODE)CanIf_ReturnTxPduId(
                                                     VAR(PduIdType, AUTOMATIC) CanIfTxTargetPduId,
                                                    P2VAR(PduIdType, AUTOMATIC, CANIF_APPL_CONST) CanIfTxPduIdPtr
                                                    )
{
    VAR (uint16_least, AUTOMATIC) CanIf_TotalTxPduId;
    VAR (PduIdType,AUTOMATIC) PduIdx_Temp;
    VAR (Std_ReturnType,AUTOMATIC) Ret_Val = E_NOT_OK;
    /* Pointer to Tx Pdu configuration */
    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lTxPduConfig_pst;

    /*Det should be invoked if module is not initialized */
    CANIF_DET_REPORT_ERROR_NOT_OK(CanIf_Prv_InitStatus_b == FALSE, CANIF_RETURN_TX_PDU_ID_SID,CANIF_E_UNINIT)

    /* If CanIfTxPduIdPtr is null, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == CanIfTxPduIdPtr), CANIF_RETURN_TX_PDU_ID_SID,CANIF_E_PARAM_POINTER)


    CanIf_TotalTxPduId = CanIf_Prv_ConfigSet_tpst->NumOfTxPdus;
    lTxPduConfig_pst = CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr;


    for(PduIdx_Temp=0; PduIdx_Temp<CanIf_TotalTxPduId; PduIdx_Temp++)
    {
        if((lTxPduConfig_pst[PduIdx_Temp].TxPduTargetPduId == CanIfTxTargetPduId) &&
                                        (lTxPduConfig_pst[PduIdx_Temp].TxPduTxUserUL==USER))
        {
            *CanIfTxPduIdPtr = lTxPduConfig_pst[PduIdx_Temp].TxPduId;
            Ret_Val = E_OK;
            break;
        }

    }
    return Ret_Val;
}
#endif
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/* FC_VariationPoint_START */
#include "CanIf_Prv.h"

/**********************************************************************************************************************
** Function name     : CanIf_ReturnRxPduId                                                                           **
** Syntax            : Std_ReturnType CanIf_ReturnRxPduId(PduIdType CanIfRxTargetPduId, PduIdType CanIfRxPduIdPtr*)  **
** Service ID[hex]   : 0x2D                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : CanIfRxTargetPduId                                                                            **
** Parameters (inout): CanIfRxPduIdPtr*                                                                              **
** Parameters (out)  : None                                                                                          **
** Return value      : Std_ReturnType                                                                                **
** Description       : Service that returns the CanIfRxPduId when the Target PduId is given.                         **
**********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
FUNC(Std_ReturnType, CANIF_CODE) CanIf_ReturnRxPduId(VAR(PduIdType, AUTOMATIC) CanIfRxTargetPduId,
        P2VAR(PduIdType, AUTOMATIC, AUTOMATIC) CanIfRxPduIdPtr)
{
    VAR (PduIdType,AUTOMATIC) PduIdx_t;

    /*Return value*/
    VAR(Std_ReturnType,AUTOMATIC) Status_t= E_NOT_OK;

    /* Pointer to Rx Pdu configuration */
    P2CONST(CanIf_Cfg_RxPduType_tst, AUTOMATIC, CANIF_CFG_CONST) RxPduCfg_pcst;

    /*[SWS_CANIF_00661] Det should be invoked if module is not initialized */
    CANIF_DET_REPORT_ERROR_NOT_OK(CanIf_Prv_InitStatus_b == FALSE, CANIF_RETURN_RX_PDU_ID_SID,CANIF_E_UNINIT)

    /* If CanIfRxPduIdPtr is null, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((NULL_PTR == CanIfRxPduIdPtr), CANIF_RETURN_RX_PDU_ID_SID,CANIF_E_PARAM_POINTER)

    RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst;

    /*Check if the pdu obtianed is of USER type*/
    for( PduIdx_t=0;  PduIdx_t<CanIf_Prv_ConfigSet_tpst->NumCanRxPduId_t;  PduIdx_t++)
    {
        if((RxPduCfg_pcst[PduIdx_t].RxPduTargetId_t == CanIfRxTargetPduId) &&
                (RxPduCfg_pcst[PduIdx_t].IndexForUL_u8 >(7u+CanIf_Prv_ConfigSet_tpst->NumCddRxPdus_t)))
        {
            /*PduId is obtained from the index of the RxPduCfg_pcst array*/
            *CanIfRxPduIdPtr = PduIdx_t;
            Status_t = E_OK;
            break;
        }
    }
    return Status_t;
}
#endif
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* FC_FileVariationPoint_END */



/*
 ***************************************************************************************************
 * used functions
 ***************************************************************************************************
 */
/************************************************************************************************************/
/* Function name    : CanIf_ReadTxMsgId                                                                     */
/* Syntax           : FUNC(uint32, CANIF_CODE) CanIf_ReadTxMsgId( VAR(uint16,AUTOMATIC) msgHandleTx_u16 )   */
/* Description      : This function is used to get the message identifier configured in message             */
/*                    configuration table (HOH table of CanDriver).                                         */
/* Parameter        : msgHandleTx_u16                                                                       */
/* Return value     : Configured CanID for the given HOH handle                                             */
/************************************************************************************************************/
#if ((CANIF_RB_READMSGID_API == STD_ON) && (CANIF_CFG_TX_FEATURE_ENABLED== STD_ON))
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

FUNC(uint32, CANIF_CODE) CanIf_ReadTxMsgId( VAR(uint16, AUTOMATIC) msgHandleTx_u16 )
{
    /* Pointer to Tx Pdu configuration */
    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lTxPduConfig_pst;

    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) TxPduConfigTable_Temp;

    VAR(Can_IdType, AUTOMATIC) CanId = 0xFFFFFFFFu;          /*Invalid CanId*/

    VAR(PduIdType, AUTOMATIC) index;

    VAR (uint16_least, AUTOMATIC) CanIf_TotalTxPdus;

    CANIF_DET_REPORT_ERROR(CanIf_Prv_InitStatus_b == FALSE, CANIF_READ_TX_MSG_ID_SID,CANIF_E_UNINIT)
    lTxPduConfig_pst = CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr;
    CanIf_TotalTxPdus = CanIf_Prv_ConfigSet_tpst->NumOfTxPdus;


    for(index = 0; index < CanIf_TotalTxPdus; index++)
    {
        TxPduConfigTable_Temp = lTxPduConfig_pst + index;
        if((uint16)(TxPduConfigTable_Temp->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->CanObjectId) == msgHandleTx_u16)
        {
#if ( CANIF_RB_CALIBRATION == STD_ON )
            if(TxPduConfigTable_Temp->getTxPduCanId != NULL_PTR)
            {
                CanId = TxPduConfigTable_Temp->getTxPduCanId();
            }
            else
#endif
            {
                CanId = TxPduConfigTable_Temp->TxPduCanId;
            }
            break;
        }
    }

    /* CanIf_CanIdType will be 32-bits if Extended CanID is supported, else it will be 16-bits.
     * But, the caller always wants the MsgId to be returned as 32-bits.
     * So, typecasting to 32-bits before returning. */
    return ( (uint32)CanId );   /*Returns invalid CanId(0xFFFFFFFF) if TxPduCanId is not configured in large metadatalength case*/
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


/**********************************************************************************************************************
** Function name     : CanIf_ReadRxMsgId                                                                             **
** Syntax            : uint32 CanIf_ReadRxMsgId(uint16 msgHandleRx_u16)                                              **
** Service ID[hex]   : 0x2F                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : msgHandleRx_u16                                                                               **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : uint32                                                                                        **
** Description       : Configured CanID for the given HOH handle.                                                    **
**********************************************************************************************************************/
#if ((CANIF_RB_READMSGID_API == STD_ON) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(uint32, CANIF_CODE) CanIf_ReadRxMsgId( VAR(uint16, AUTOMATIC) msgHandleRx_u16 )
{
    /* Pointer to HRH configuration */
    P2CONST(CanIf_Cfg_Hrhtype_tst, AUTOMATIC, CANIF_CFG_CONST) HrhCfg_pcst;

    /*Local variable to store canid */
    VAR(Can_IdType, AUTOMATIC) lCanId_t;

    /*initializing canid */
    lCanId_t = 0;

    /*[SWS_CANIF_00661] Det should be invoked if module is not initialized */
    CANIF_DET_REPORT_ERROR_NOT_OK(CanIf_Prv_InitStatus_b == FALSE, CANIF_READ_RX_MSG_ID_SID,CANIF_E_UNINIT)

    /* Report Det if Hoh is invalid */
    CANIF_DET_REPORT_ERROR_NOT_OK((msgHandleRx_u16>=CANIF_CFG_TOTAL_HOH_NUM), CANIF_READ_RX_MSG_ID_SID,
            CANIF_E_PARAM_HRH)

    CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_Prv_ConfigSet_tpst->HrhPduIdTable_Ptr[msgHandleRx_u16] ==  INVALID_ID), CANIF_READ_RX_MSG_ID_SID,
            CANIF_E_PARAM_HRH)

    /*obtain the required hrh */
    HrhCfg_pcst = CanIf_Prv_ConfigSet_tpst->HrhConfig_pcst + CanIf_Prv_ConfigSet_tpst->HrhPduIdTable_Ptr[msgHandleRx_u16];

    /*[CanIf_Ext-3123] If Hrh is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_NOT_OK((HrhCfg_pcst->HrhInfo_e != CANIF_PRV_FULL_E), CANIF_READ_RX_MSG_ID_SID,
            CANIF_E_PARAM_HRH)

    /* obtain the configured canid */
    lCanId_t = HrhCfg_pcst->CanId_t;

    return (lCanId_t);

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* FC_VariationPoint_START */
/*****************************************************************************************************************************/
/* Function name    : CanIf_Rb_ChangeRxPduCanId                                                                              */
/* Syntax           : FUNC(Std_ReturnType, CANIF_CODE) CanIf_Rb_ChangeRxPduCanId(VAR(PduIdType, AUTOMATIC) CanRxPduId        */
/* Description      : This service reconfigures the corresponding CAN identifier of the requested Receive L-PDU.             */
/* Parameters       : CanRxPduId & CanId                                                                                        */
/* Return value     : Std_ReturnType - E_OK / E_NOT_OK                                                                          */
/*
Notes:
 - ASW can call this function to reconfigure/change the CanId of CanRxPduId if the CanIfRxPduType is configured as DYNAMIC
 - This API copy the new CanId into corresponding DynamicRxPduCanId array location of the requested CanRxPduId and
   call the driver function rba_Can_ChangeRxCanId() to update the CanId of the corresponding receive Hardware object
 - API returns E_OK if the new CanId updated successfully by can driver ( rba_Can_ChangeRxCanId() returns E_OK)
 - User should set the two MSB bits (’IDentifier Extensionflag’ (see [12, ISO11898 (CAN)]) and ’CAN FD flag’- AR4.2.2 SWS) of
    the CanId based on the ’CanIfRxPduCanIdType’ of the CanRxPduId, while calling the function CanIf_Rb_ChangeRxPduCanId().
 - ASW can call this function to change the CanId of CanRxPduId only during ECU initialization (Post ECU initialization)
 - API return E_NOT_OK, when it is called by ASW during Runtime.                                                             */
/*****************************************************************************************************************************/

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if ((CANIF_RB_CHANGERXPDUID_API != STD_OFF) && (CANIF_CFG_RX_FEATURE_ENABLED== STD_ON))
FUNC(Std_ReturnType, CANIF_CODE) CanIf_Rb_ChangeRxPduCanId(VAR(PduIdType,AUTOMATIC) CanRxPduId, VAR(Can_IdType,AUTOMATIC) CanId)
{

    /* Pointer to HRH configuration */
    P2CONST(CanIf_Cfg_Hrhtype_tst, AUTOMATIC, CANIF_CFG_CONST) HrhCfg_pcst;

    /* Pointer to RxPdu configuration */
    P2CONST(CanIf_Cfg_RxPduType_tst, AUTOMATIC, CANIF_CFG_CONST) RxPduCfg_pcst;

    /* Pointer to store address of Dynamic RxPduCanId array*/
    P2VAR(Can_IdType,AUTOMATIC,AUTOMATIC) DynRxPduIdPtr_Temp;

    /* local variable to store the Hrh index value */
    VAR(Can_HwHandleType,AUTOMATIC) CanHrh_u8;

    /* Pointer to Controller State configuration */
    P2VAR(CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC) ControllerState_ps;

    /*To store the controller state (Device mode)*/
    VAR (CanIf_ControllerModeType, AUTOMATIC) DeviceMode_tst;

    /* local variable to hold the return value */
    VAR (Std_ReturnType, AUTOMATIC ) Status = E_NOT_OK;


#if CANIF_PUBLIC_DEV_ERROR_DETECT != STD_OFF
    /* Local variable to store two MSB's */
     VAR(uint8, AUTOMATIC)canifMSBbits_u8;
#endif

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
     CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_RB_CHANGERXPDUID_SID, CANIF_E_UNINIT)

    /* If CanRxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((CanRxPduId >= (CanIf_Prv_ConfigSet_tpst->NumCanRxPduId_t)), \
                    CANIF_RB_CHANGERXPDUID_SID,CANIF_E_INVALID_RXPDUID)

     CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanRxPduId]== INVALID_ID), \
                                    CANIF_RB_CHANGERXPDUID_SID,CANIF_E_INVALID_RXPDUID)

    /* Get the pointer to requested CanRxPduId */
    RxPduCfg_pcst = (CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst) + CanIf_Prv_ConfigSet_tpst->RxPduIdTable_Ptr[CanRxPduId];

   CANIF_DET_REPORT_ERROR_NOT_OK((CANIF_STATIC == (RxPduCfg_pcst->RxPduType)), CANIF_RB_CHANGERXPDUID_SID,CANIF_E_INVALID_RXPDUID)


#if CANIF_PUBLIC_DEV_ERROR_DETECT != STD_OFF
    /*shifting the CanId bits to access two MSB's */
    canifMSBbits_u8 = (uint8)(CanId >> CANIF_CANID_BIT_SHIFT);

    /* If CanId is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_NOT_OK((((canifMSBbits_u8 == CANIF_ZERO)|| \
            (canifMSBbits_u8 == CANIF_ONE)) && ((CanId & CANIF_MSB_BIT_RESET) > CANIF_CAN_STD_VAL)),
            CANIF_RB_CHANGERXPDUID_SID, CANIF_E_PARAM_CANID)

    CANIF_DET_REPORT_ERROR_NOT_OK((((canifMSBbits_u8 == CANIF_TWO)||(canifMSBbits_u8 == CANIF_THREE)) && \
            ((CanId & CANIF_MSB_BIT_RESET)  > CANIF_CAN_XTD_VAL)), CANIF_RB_CHANGERXPDUID_SID, CANIF_E_PARAM_CANID)
#endif

    /* Initialize pointer to HRH configuration structure */
    HrhCfg_pcst = (CanIf_Prv_ConfigSet_tpst->HrhConfig_pcst) + RxPduCfg_pcst->Hrhref_t;

    /* Get the Hrh Id from the configuration structure */
    CanHrh_u8 = HrhCfg_pcst -> CanObjectId;

    /*Fetching controller Information to local variable */
    ControllerState_ps = CanIf_Prv_ControllerState_ast + HrhCfg_pcst->ControllerId_u8;

    /* Copy the controller state into local variable */
    DeviceMode_tst = ControllerState_ps->DeviceMode;

    /* Check if the controller is in Stopped state */
    if( DeviceMode_tst  == CANIF_CS_STOPPED)
    {
        /* Check if the CanRxPduId is configured as Dynamic */
        if((RxPduCfg_pcst ->RxPduType) != CANIF_STATIC)
        {
            /*Get the address of DynamicRxPduCanId array location for the CanRxPduId */
            DynRxPduIdPtr_Temp = &CanIf_DynRxPduCanId_au32[0] + (RxPduCfg_pcst ->RxPduType);

            /* Update the new CanId into the corresponding location in the DynamicRxPduCanId array */
            *DynRxPduIdPtr_Temp = CanId;

            /* Call Can driver to change the CanId in the Hardware object structure*/
            Status = rba_Can_ChangeRxCanId(CanHrh_u8, CanId);
        }
    }
    else
    {
        /* Return E_NOT_OK if the controller of the requested CanRxPduId is not in CANIF_CS_STOPPED state */
        Status = E_NOT_OK;
    }

    return Status;

} /* End of CanIf_Rb_ChangeRxPduCanId() */

#endif
/* FC_FileVariationPoint_END */

