


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */


#include "CanIf_Prv.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* HIS METRIC PATH,v(G),LEVEL VIOLATION IN CanIf_TxConfirmation: Function contains very simple "if-else" statements.
 * HIS metric compliance would decrease readability and maintainability */
void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
#if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
    /* Pointer to Tx Pdu configuration */
    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lTxPduConfig_pst;
    VAR(uint16, AUTOMATIC)          ltxPduCustId_t;
    P2CONST(CanIf_Cfg_CtrlConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lCtrlConfig_pst;
    /* Pointer to Controller State configuration */
    P2VAR(  CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC    ) lControllerState_p;
    VAR (uint8, AUTOMATIC           ) lControllerId_u8      = 0;
    /*To store the controller state (Channel mode)*/
    VAR (   CanIf_PduModeType, AUTOMATIC                        ) lChannelMode_en;

#if (CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)
    P2VAR(CanIf_NotifStatusType, AUTOMATIC, AUTOMATIC) lTxNotifPtr;
#endif


#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF)

    /*To store the controller state (Device mode)*/
    VAR (CanIf_ControllerModeType, AUTOMATIC) lDeviceMode_en;

#endif

/* FC_VariationPoint_START */
#if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
    VAR(uint8, AUTOMATIC) llocalCoreId_u8 ;
    VAR(uint8, AUTOMATIC) ldestCoreId_u8;
    VAR(uint16, AUTOMATIC) lpipeId_u16 ;
    VAR(CanIf_XCore_PipeTxRxParams_st, AUTOMATIC) lpipeParams_st;
    VAR(PduInfoType, AUTOMATIC) pduInfoDummy_st;
#endif
/* FC_VariationPoint_END */

#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
    const CanIf_Cfg_TxBufferConfig_tst* lCanIf_TxBufferConfig_p;
    uint8* lDataBuf_p;
    P2VAR (CanIf_Cfg_CanIdBuffer_tst,AUTOMATIC, AUTOMATIC  ) lCanIdBuf_p;
    uint32 lBufferId;
    VAR(uint8, AUTOMATIC) sdu_temp[CANIF_PRV_MAX_BUFFER_SIZE] = {0};
    Can_PduType Pdu_temp = {NULL_PTR, 0, 0, 0};
    Can_HwHandleType lHth_uo = 0;
    Can_ReturnType lCanRetVal;
    Std_ReturnType lTxBufferRetVal = E_NOT_OK;
    uint8 lClearIndex;
    uint8 lDataIndex;
    uint8 lDataMaxLen;
    Pdu_temp.sdu = &sdu_temp[0];
#endif




    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET((FALSE == CanIf_Prv_InitStatus_b), CANIF_TX_CONFIRMATION_SID, CANIF_E_UNINIT)

    /* If CanTxPduId is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_VOID_RET(CanTxPduId >= CANIF_TOTAL_TXPDUS, CANIF_TX_CONFIRMATION_SID,CANIF_E_PARAM_LPDU)

    ltxPduCustId_t = CanIf_Prv_ConfigSet_tpst->TxPduIdTable_Ptr[CanTxPduId];

    /* If CanTxPduId is invalid, report to DET and exit the function */
    CANIF_DET_REPORT_ERROR_VOID_RET((ltxPduCustId_t == 0xFFFFu), CANIF_TX_CONFIRMATION_SID, CANIF_E_PARAM_LPDU)

    lTxPduConfig_pst   = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + ltxPduCustId_t;
#if (CANIF_RB_NODE_CALIBRATION == STD_OFF)
    lCtrlConfig_pst    = lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->CanIf_CtrlConfigPtr;
#else
    lCtrlConfig_pst = &(CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr[lTxPduConfig_pst->CanIf_TxBufferConfigPtr->CanIf_HthConfigPtr->getCtrlCfg()]);
#endif
    lControllerId_u8   = lCtrlConfig_pst->CtrlId;


    /*Get the current status of the linked controller */
    lControllerState_p = CanIf_Prv_ControllerState_ast + lControllerId_u8;
    lChannelMode_en    = lControllerState_p->ChannelMode;

#if (CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)
    lTxNotifPtr = CanIf_Prv_TxNotification_aen;
#endif

#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
    /*Identify the Buffer for this HTH*/
    lCanIf_TxBufferConfig_p = lTxPduConfig_pst->CanIf_TxBufferConfigPtr;
    lDataBuf_p                 = lCanIf_TxBufferConfig_p->DataBuf;
    lCanIdBuf_p                  = lCanIf_TxBufferConfig_p->CanIdBuf;
    lDataMaxLen                  = lCanIf_TxBufferConfig_p->CanIfBufferMaxDataLength;
    lBufferId                = lCanIf_TxBufferConfig_p->CanIfBufferId;
    lHth_uo                   = lCanIf_TxBufferConfig_p->CanIf_HthConfigPtr->CanObjectId;
#endif


#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)


    /*Check for the flag at least one pdu linked to this controller is a pnfilter pdu*/
    if((lCtrlConfig_pst->PnCtrlEn == TRUE) && (lControllerState_p->BlPnTxFilter != FALSE))
    {
        lControllerState_p->BlPnTxFilter = FALSE;
    }

#endif


#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF)
    /* Enter Critical Section */
    SchM_Enter_CanIf_ControllerState();

    /*Make local copy of Device mode*/
    lDeviceMode_en = lControllerState_p->DeviceMode;


/*Update the TxCnfmStatus  if controller is in STARTED mode*/
if( ((lControllerState_p->CanIf_TxCnfmStatus) != CANIF_TX_RX_NOTIFICATION) && (lDeviceMode_en == CANIF_CS_STARTED))
{
    /* Set Tx Confirmation state for the controller*/
    lControllerState_p->CanIf_TxCnfmStatus = CANIF_TX_RX_NOTIFICATION;
}
    /* Leave Critical Section */
    SchM_Exit_CanIf_ControllerState();
#endif

#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)

    if (CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus  != CANIF_TXBUFFER_EMPTY)            /*Buffer is NOT empty*/
    {

        /* Enter Critical Section */
        SchM_Enter_CanIf_TxBufAccessNoNest();               /*RR*/

        /*Read the PDU having highest priority*/
        lTxBufferRetVal = CanIf_Prv_ReadTxBuffer(lCanIf_TxBufferConfig_p, &Pdu_temp);

        /* Leave Critical Section */
        SchM_Exit_CanIf_TxBufAccessNoNest();

        if(lTxBufferRetVal == E_OK)
        {
            /*Call Can_Write() for the pdu*/
            lCanRetVal = Can_Write(lHth_uo, &Pdu_temp);

            if((lCanRetVal == CAN_OK)||(lCanRetVal == CAN_NOT_OK))
            {
                /* Enter Critical Section */
                SchM_Enter_CanIf_TxBufAccessNoNest();               /*RR*/

                /*[SWS_CANIF_00183]*/

                /* Clear the read data from DataBuf and mark the index as free*/
                lClearIndex = (CanIf_Prv_TxBufferRam_ast[lBufferId].last_index);
               *(lDataBuf_p + ((lCanIdBuf_p[lClearIndex].BufferIndex)*(lDataMaxLen+1)) + 0)    = 0xFF;       /*Index is available*/
               #if(CANIF_TRIGGERTRANSMIT_SUPPORT == STD_ON)
                /*Get TxPdu config structure for the requested TxPdu*/
                lTxPduConfig_pst    = (CanIf_Prv_ConfigSet_tpst->CanIf_TxPduConfigPtr) + \
                                                                ((lCanIdBuf_p + lClearIndex)->swPduHandle);
                if(lTxPduConfig_pst->TxPduTriggerTransmit!=TRUE)
#endif
                {
                        /* Remove this loop in future release. runtime opt*/
                     for(lDataIndex=1; lDataIndex < lDataMaxLen; lDataIndex++)
                     {
                         *(lDataBuf_p + (lCanIdBuf_p[lClearIndex].BufferIndex*(lDataMaxLen+1)) + lDataIndex) = 0;
                     }
                }

                 CanIf_Prv_TxPduRam_ast[(lCanIdBuf_p + lClearIndex)->swPduHandle].pdu_buffered_flag = FALSE;
                 (lCanIdBuf_p + lClearIndex)->BufferIndex = 0;
                  (lCanIdBuf_p + lClearIndex)->CanId = 0;
                  (lCanIdBuf_p + lClearIndex)->SduLength = 0;
                 (lCanIdBuf_p + lClearIndex)->swPduHandle = 0;

                 (CanIf_Prv_TxBufferRam_ast[lBufferId].last_index) = (lClearIndex - 1)&0xFFu;
                 if((CanIf_Prv_TxBufferRam_ast[lBufferId].last_index) == 0xFF)  /*All elements are read from the buffer*/
                 {
                     (CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus) = CANIF_TXBUFFER_EMPTY;
                 }
                 else if(CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus == CANIF_TXBUFFER_FULL)
                 {
                     (CanIf_Prv_TxBufferRam_ast[lBufferId].bufferstatus) = CANIF_TXBUFFER_READY;
                 }
                 else
                 {
                     /*Do nothing*/
                 }

                 /* Leave Critical Section */
                 SchM_Exit_CanIf_TxBufAccessNoNest();
            }
        }
    }
#endif

#if (CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)

    /* Check if the Notification status can be read */
if( FALSE != lTxPduConfig_pst->TxPduReadNotifyStatus )
{

    /* Update the notification status */
    *(lTxNotifPtr + CanTxPduId) = CANIF_TX_RX_NOTIFICATION;

}
#endif
/* Indication to UL about confirmation of data transmission */
if((NULL_PTR != lTxPduConfig_pst->UserTxConfirmation) && (lChannelMode_en == CANIF_ONLINE))
{

    if ((lTxPduConfig_pst->TxPduTxUserUL == PDUR ) || (lTxPduConfig_pst->TxPduTxUserUL == USER ) || (CANIF_XCORE_CFG_ENABLED == STD_OFF))
    {
        (void)CanIf_XCore_LocalCore_TxConfirmation(lTxPduConfig_pst);
    }
    else
    {
		/* FC_VariationPoint_START */
        #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
        llocalCoreId_u8 = Schm_CanIf_XCore_GetCurrentCoreId();
        ldestCoreId_u8 = CanIf_XCore_Config_pcst->CanIf_XCoreUserTypeCoreAffinity[lTxPduConfig_pst->TxPduTxUserUL];
        lpipeId_u16 = CanIf_XCore_Config_pcst->CanIf_XCoreTxConfirmationPipeMatrix[llocalCoreId_u8][ldestCoreId_u8];
        pduInfoDummy_st.SduDataPtr = NULL_PTR;
        pduInfoDummy_st.SduLength = 0;

        lpipeParams_st.userType_e = lTxPduConfig_pst->TxPduTxUserUL;
        lpipeParams_st.PduId = CanTxPduId;
        lpipeParams_st.PduInfoPtr = &pduInfoDummy_st;
        lpipeParams_st.ControllerId_u8 = lControllerId_u8;

        if(lpipeId_u16 == CANIF_XCORE_INVALID_PIPE_ID)
        {
            (void)CanIf_XCore_LocalCore_TxConfirmation(lTxPduConfig_pst);
        }
        else
        {
            (void)CanIf_XCore_PipeWrite(lpipeId_u16, &lpipeParams_st);
        }
        #endif
		/* FC_VariationPoint_END */
    }
}
#endif/*End of #if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)*/
}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
FUNC(Std_ReturnType, CANIF_CODE)CanIf_XCore_LocalCore_TxConfirmation(P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) CanIf_TxPduConfig_pst)
{
    Std_ReturnType lretval = E_OK;
    /* Pointer to Tx Pdu configuration */
    P2CONST(CanIf_Cfg_TxPduConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lTxPduConfig_pst;

    /* If Ptr is NULL, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_TxPduConfig_pst == NULL_PTR), CANIF_INIT_SID, CANIF_E_PARAM_POINTER)

    lTxPduConfig_pst = CanIf_TxPduConfig_pst;

    lTxPduConfig_pst->UserTxConfirmation(lTxPduConfig_pst->TxPduTargetPduId);

    return lretval;
}
#endif/*End of #if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)*/
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
