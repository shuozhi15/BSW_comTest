

/***************************************************************************************************
*                              Include files                                                       *
***************************************************************************************************/
/* CanIf priv header file */
#include "CanIf_Prv.h"

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */


/****************************************************************************************************
 *                              Function Definition                                                 *
 ****************************************************************************************************/

/**********************************************************************************************************************
** Function name     : CanIf_RxIndication                                                                            **
** Syntax            : void CanIf_RxIndication(const Can_HwType* Mailbox, const PduInfoType* PduInfoPtr)             **
** Service ID[hex]   : 0x14                                                                                          **
** Sync/Async        : Synchronous                                                                                   **
** Reentrancy        : Reentrant                                                                                     **
** Parameters (in)   : Mailbox, PduInfoPtr                                                                           **
** Parameters (inout): None                                                                                          **
** Parameters (out)  : None                                                                                          **
** Return value      : None                                                                                          **
** Description       : This service indicates a successful reception of a received CAN Rx LPDU to the CanIf after    **
**                     passing all filters and validation checks.                                                    **
**********************************************************************************************************************/
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* HIS METRIC PATH,v(G),STMT,LEVEL,RETURN VIOLATION IN CanIf_RxIndication: Function contains very simple "else if" statements and "switch-cases".
 * HIS metric compliance would decrease readability and maintainability. Also Function contains more than one return statement.
 * This is needed mainly because of DET. */
FUNC(void, CANIF_CODE) CanIf_RxIndication_Internal( P2CONST(Can_HwType, AUTOMATIC, CANIF_APPL_DATA) Mailbox,
        P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA)PduInfoPtr)
{
    #if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)

    /* Pointer to HRH configuration */
    P2CONST(CanIf_Cfg_Hrhtype_tst, AUTOMATIC, CANIF_CFG_CONST) HrhCfg_pcst;

    /* Pointer to RxPdu configuration */
    P2CONST(CanIf_Cfg_RxPduType_tst, AUTOMATIC, CANIF_CFG_CONST) RxPduCfg_pcst;

    #if CANIF_CFG_UL_RX == STD_ON
    /*Local variable to hold data for UL */
    VAR(PduInfoType, AUTOMATIC) ULPduInfoTyp_tst;
    #endif

        /*Local variable to hold data and dlc */
    VAR(PduInfoType, AUTOMATIC) PduInfoTemp_tst;

    #if CANIF_CFG_USER_RX_ENABLED == STD_ON
    /*Local variable to hold data for USER UL */
    VAR(CanIf_PduInfoType, AUTOMATIC) ULPduInfoTypUSER_tst;
    #endif

    /*variable used as an index for looping conditions */
    VAR(uint8,AUTOMATIC)Index_u8;

    /*variable used as a status current channel mode */
    VAR(uint8,AUTOMATIC)ModeChkStatus = CANIF_PRV_INVALID_CHANNEL_MODE;

    /*variable to save pduId */
    VAR(PduIdType,AUTOMATIC)PduId_qu8;

    /*variable to save pduId for Range or list structure */
    #if (CANIF_BASIC_CAN_SUPPORT == STD_ON)
    VAR(uint8_least,AUTOMATIC)RngLstPduId_qu8;
    #endif

    /*store canid in the local variable removing Idtype information */
    VAR(Can_IdType,AUTOMATIC)CanIdNoIdType_t;

    /* To store the status of search algorithm */
    VAR(boolean, AUTOMATIC) IdFound_b = FALSE;

    /*Extracting 2MSB's of recieved CanId */
     VAR(uint8, AUTOMATIC) CanIdPduType_u8;

     /*Variable to store sdulength which is sum of Dlc and MetadataLength*/
    // VAR(uint32,AUTOMATIC) SduLength_u32 = 0;

     /* Pointer to Controller State configuration */
     P2VAR(CanIf_ControllerStateType, AUTOMATIC, AUTOMATIC) ControllerState_ps;

     /*Local pointer to hold the data of range type of Pdu */
     //If CANIF_RXPDU_CANID_RANGE macro is used for basic pdu make sure it also is added to this variable
    #if CANIF_CFG_RX_RANGE_CONFIG == STD_ON
     P2CONST(CanIf_RxPduRangeConfigType_tst, AUTOMATIC, AUTOMATIC) CanIdRangeCfg_pcs;
    #endif
     /* FC_VariationPoint_START */
    #if CANIF_CFG_HRH_LIST_ENABLED == STD_ON
     P2CONST(CanIf_RxPduListConfigType_tst, AUTOMATIC, AUTOMATIC) CanIdListCfg_pcs;
    #endif
     /* FC_VariationPoint_END */

     /*To store the controller state (Device mode)*/
     VAR (CanIf_ControllerModeType, AUTOMATIC) DeviceMode_tst;

     /*To store the controller state (Channel mode)*/
     VAR (CanIf_PduModeType, AUTOMATIC) ChannelMode_tst;


    #if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
    /*Pointer to store address of Dynamic RxPduCanId array*/
    P2VAR(uint32,AUTOMATIC,AUTOMATIC) dynRxPduId_pu32;

    VAR(uint32,AUTOMATIC) rxcanid_u32 = 0;
    #endif

    #if(CANIF_CFG_DLC_CHECK == STD_ON)
    P2CONST(CanIf_CallbackFuncType, AUTOMATIC, AUTOMATIC) CallBackPtr_Temp;
	/* This variable holds Dlc value and used for DLC checks */
    VAR(PduLengthType, AUTOMATIC) lConfigRxPduDlc;
    #endif
    #if(CANIF_METADATA_SUPPORT == STD_ON)
    /*Variable to store canId temporarily while adding the data into metadata */
   // VAR(uint8,AUTOMATIC) CanId_Temp_u8;

    /*Data buffer to store payload data and metadata canid bytes*/
    VAR(uint8,AUTOMATIC) SduDataBuffer_au8[SDU_DATA_BUFF_SIZE] = {0};

    #endif

    #if(CANIF_CANNM_TXID_FILTER != STD_OFF)
    P2CONST(Can_IdType, AUTOMATIC, AUTOMATIC) CanNmTxId_pt;
    #endif
    //RxPduCbk_pcst = CanIf_Prv_RxUSERName_ta_fct;

    /* FC_VariationPoint_START */
    #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
    VAR(uint8, AUTOMATIC) localCoreId_u8 ;
    VAR(uint8, AUTOMATIC) destCoreId_u8;
    VAR(uint16, AUTOMATIC) pipeId_u16 ;
    VAR(CanIf_XCore_PipeTxRxParams_st, AUTOMATIC) xCorePipeTxRxParams;
    #endif
    /* FC_VariationPoint_END */

    #if (STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API)
    P2VAR(CanIf_NotifStatusType,AUTOMATIC, AUTOMATIC) RxNotifPtr_pen;

    RxNotifPtr_pen = CanIf_Prv_RxNotification_taen;
    // this array should be initialized to CANIF_NO_NOTIFICATION value in initialization.
    #endif

    #if(CANIF_CFG_DLC_CHECK == STD_ON)
    /*Initializing callback pointer */
    CallBackPtr_Temp = &CanIf_Callback;
    #endif


    /* Det to check for correct inputs */
    /*[SWS_CANIF_00661] Det should be invoked if module is not initialized */
    CANIF_DET_REPORT_ERROR_VOID_RET(CanIf_Prv_InitStatus_b == FALSE, CANIF_RX_INDICATION_SID, CANIF_E_UNINIT);

    /*[SWS_CANIF_00419][CanIf_Ext-3484] Report Det if pointers are invalid */
    CANIF_DET_REPORT_ERROR_VOID_RET(((Mailbox == NULL_PTR) || (PduInfoPtr == NULL_PTR) ||((PduInfoPtr != NULL_PTR) &&
            (PduInfoPtr->SduDataPtr == NULL_PTR))),CANIF_RX_INDICATION_SID, CANIF_E_PARAM_POINTER);

    /*Fetch CanId without CanIdType*/
    CanIdNoIdType_t = Mailbox->CanId & CANIF_PRV_MSB_BIT_RESET;

    /*[CanIf_Ext-3483] Report DET if ControllerId is invalid */
    CANIF_DET_REPORT_ERROR_VOID_RET(Mailbox->ControllerId>= CANIF_TOTAL_CTRLS ,CANIF_RX_INDICATION_SID, CANIF_E_PARAM_POINTER);

    /*[SWS_CANIF_00416] Report Det if Hoh is invalid
     * Checking if the received Hoh is of receive type and
     * checking if the hoh obtained is less than the total configure hoh*/
    CANIF_DET_REPORT_ERROR_VOID_RET((Mailbox->Hoh>=CANIF_CFG_TOTAL_HOH_NUM), CANIF_RX_INDICATION_SID, CANIF_E_PARAM_HOH);
    CANIF_DET_REPORT_ERROR_VOID_RET((CanIf_Prv_ConfigSet_tpst->HrhPduIdTable_Ptr[Mailbox->Hoh] ==  INVALID_ID), CANIF_RX_INDICATION_SID, CANIF_E_PARAM_HOH);

    /*Extracting 2MSB's of recieved CanId */
    /* MR12 RULE 10.3 VIOLATION:The Warning is "Not Critical". The left operand of shift operator is lower in size
     * compared to right operand, as the left operand is only used to store two bits for which unit8 is sufficient  */
    CanIdPduType_u8 = (uint8)((Mailbox->CanId)>>CANIF_CANID_BIT_SHIFT);

    /*[SWS_CANIF_00417] Report Det if canid is invalid */
    CANIF_DET_REPORT_ERROR_VOID_RET((((CanIdPduType_u8 == CANIF_ZERO)||(CanIdPduType_u8 == CANIF_ONE))&& (CanIdNoIdType_t > CANIF_CAN_STD_VAL))||
                    (((CanIdPduType_u8 == CANIF_TWO)||(CanIdPduType_u8 == CANIF_THREE))&& (CanIdNoIdType_t > CANIF_CAN_XTD_VAL)), CANIF_RX_INDICATION_SID, CANIF_E_PARAM_CANID);

    /*[CanIf_Ext-3486]Report Det if Dlc is invalid*/
    CANIF_DET_REPORT_ERROR_VOID_RET((((CanIdPduType_u8 == CANIF_ZERO)||(CanIdPduType_u8 == CANIF_TWO))&& (PduInfoPtr->SduLength > 8u))||
                        (((CanIdPduType_u8 == CANIF_ONE)||(CanIdPduType_u8 == CANIF_THREE))&& (PduInfoPtr->SduLength > 64u)), CANIF_RX_INDICATION_SID, CANIF_E_DATA_LENGTH_MISMATCH);

    /* Get the pointer to desired hrh
     * Multi driver implementation needs to be taken care*/
    HrhCfg_pcst = CanIf_Prv_ConfigSet_tpst->HrhConfig_pcst + CanIf_Prv_ConfigSet_tpst->HrhPduIdTable_Ptr[Mailbox->Hoh];
    #if(CANIF_CANNM_TXID_FILTER != STD_OFF)
    CanNmTxId_pt = CanIf_Prv_ConfigSet_tpst->NmtxIdPtr;
    #endif


    /*Fetching controller Information to local variable */
    ControllerState_ps= CanIf_Prv_ControllerState_ast + Mailbox->ControllerId;


    /*Make a local copy of Device mode and Channel mode*/
    /*Enter critical section*/
    SchM_Enter_CanIf_ControllerState();
    DeviceMode_tst = ControllerState_ps->DeviceMode;
    ChannelMode_tst = ControllerState_ps->ChannelMode;

    #if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON && CANIF_PUBLIC_BUSOFF_RECOVERY_FROM_RXINDICATION == STD_ON )

    /* Set Tx Confirmation state for the controller*/
    ControllerState_ps->CanIf_TxCnfmStatus = CANIF_TX_RX_NOTIFICATION;
    #endif
    /*Leave critical section*/
    SchM_Exit_CanIf_ControllerState();

    /*[SWS_CANIF_00896-AR430] this requirement will be updated when PN id done in CanIf_Transmit */
    #if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    if(ControllerState_ps->BlPnTxFilter == TRUE)
    {
       ControllerState_ps->BlPnTxFilter = FALSE;
    }
    #endif

    /* Check the controller mode is valid */
    if(CANIF_CS_STARTED == DeviceMode_tst)
    {
        /*Check if the channel mode is enabled in receive path*/
        if((CANIF_TX_OFFLINE == ChannelMode_tst)||(CANIF_ONLINE == ChannelMode_tst))
        {
            ModeChkStatus = CANIF_PRV_VALID_CHANNEL_MODE;
        }
        /*For WakeUpValidation the channel mode shall be disabled for Rx Path */
        #if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
        else
        {
            ModeChkStatus = CANIF_PRV_WAKEUP_MODE;
        }
        #endif

        /*Filtering is needed if the channel mode is valid or is for WakeupValidation*/
        #if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_OFF)
        if(ModeChkStatus == CANIF_PRV_VALID_CHANNEL_MODE)
        #endif
        {
            /*Initializing the index for the loop */
            Index_u8 = 0;
            /*Software filtering to be performed */
            switch(HrhCfg_pcst->HrhInfo_e)
            {
                /*FULL CAN Hrh */
                case CANIF_PRV_FULL_E:
                {
                    /*Obtain Index of Rxpdu */
                    PduId_qu8 = HrhCfg_pcst->PduIdx_t;


                    /* [SWS_CANIF_00877] Check for correct recieved CanId type and CanId */
                    /* MR12 RULE 15.4 VIOLATION:The Warning is "Not Critical". Multiple break points are used for optimal
                     * operation of filtering feature*/
                    do{
                        /* Get the pointer to desired Rx pdu */
                        RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + PduId_qu8;

                        #if(CANIF_RB_CHANGERXPDUID_API != STD_OFF)
                        /* Check if the CanRxPduIdType is configured as Dynamic */
                        if((RxPduCfg_pcst->RxPduType) != CANIF_STATIC)
                        {

                            /* Get the address of DynamicRxPduCanId array which stores Dynamic RxPdu CanId.*/
                            dynRxPduId_pu32 = &CanIf_DynRxPduCanId_au32[0];

                            /*Get the CanId value from DynamicRxPduCanId array*/
                            rxcanid_u32 = (Can_IdType)*(dynRxPduId_pu32 + (RxPduCfg_pcst->RxPduType));

                            /* Remove the CanIdtype info(MSB bits) and get the CanId Value */
                            rxcanid_u32 = rxcanid_u32 & CANIF_PRV_MSB_BIT_RESET;

                            /* Check if the received CanIdType is matching with configured CanIdType and the received CanId is same as the CanId present the DynamicRxPduCanId array */
                            if((((RxPduCfg_pcst->CanIdtype_u8) &(CANIF_PRV_CANID_TYPE_MASK))==(((RxPduCfg_pcst->CanIdtype_u8)>>CANIF_PRV_FOUR)& CanIdPduType_u8)) && (CanIdNoIdType_t == rxcanid_u32))
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                                break;
                            }
                        }
                        else
                        #endif
                        {
                            #if (CANIF_RB_CALIBRATION == STD_ON)
                            /* MR12 RULE 13.5 VIOLATION:The Warning is "Not Critical". Brackets are used correctly to
                             * prevent any side effects of binary operator precedence */
                            if((((RxPduCfg_pcst->CanIdtype_u8)&(CANIF_PRV_CANID_TYPE_MASK))==(((RxPduCfg_pcst->CanIdtype_u8)>>CANIF_PRV_FOUR)& CanIdPduType_u8))&&(CanIdNoIdType_t == RxPduCfg_pcst->getCanId()))
                            #else
                            if((((RxPduCfg_pcst->CanIdtype_u8)&(CANIF_PRV_CANID_TYPE_MASK))==(((RxPduCfg_pcst->CanIdtype_u8)>>CANIF_PRV_FOUR)& CanIdPduType_u8))&&(CanIdNoIdType_t == RxPduCfg_pcst->RxPduCanId))
                            #endif
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                                break;
                            }
                        }

                        PduId_qu8++;
                        Index_u8++;
                    }while(HrhCfg_pcst->NumRxPdus_u32>= Index_u8);
                }break;
                #if (CANIF_BASIC_CAN_SUPPORT == STD_ON)
                #if CANIF_CFG_RX_RANGE_CONFIG == STD_ON
                case CANIF_PRV_BASIC_RANGE_E:
                {
                    RngLstPduId_qu8 = HrhCfg_pcst->PduIdx_t;

                    /* [SWS_CANIF_00877] Check for correct recieved CanId type and CanId */
                    while((HrhCfg_pcst->NumRxPdus_u32)> Index_u8)
                    {
                        CanIdRangeCfg_pcs = CanIf_Prv_ConfigSet_tpst->RangeCfg_tpst + RngLstPduId_qu8;

                        /*Check if the filtering is of range or list type
                         * true: mask based filtering
                         * false: range based filtering */
                        if(HrhCfg_pcst->HrhRangeMask_b == TRUE)
                        {
                            /* Here CanIdRangeCfg_pcs->UpperCanId is RxPduCanIdMask and
                             *      CanIdRangeCfg_pcs->LowerCanId is RxPduCanId */
                            #if (CANIF_RB_CALIBRATION == STD_ON)
                            if((CanIdNoIdType_t & CanIdRangeCfg_pcs->getUpperCanId_t()) == (CanIdRangeCfg_pcs->getUpperCanId_t() & CanIdRangeCfg_pcs->getLowerCanId_t()))
                            #else
                            if((CanIdNoIdType_t & CanIdRangeCfg_pcs->UpperCanId_t) == (CanIdRangeCfg_pcs->UpperCanId_t & CanIdRangeCfg_pcs->LowerCanId_t))
                            #endif
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                            }
                        }
                        else
                        {
                            #if (CANIF_RB_CALIBRATION == STD_ON)
                            if((CanIdNoIdType_t >= CanIdRangeCfg_pcs->getLowerCanId_t()) && (CanIdNoIdType_t<= CanIdRangeCfg_pcs->getUpperCanId_t()))
                            #else
                            if((CanIdNoIdType_t >= CanIdRangeCfg_pcs->LowerCanId_t) && (CanIdNoIdType_t<= CanIdRangeCfg_pcs->UpperCanId_t))
                            #endif
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                            }
                        }
                        if( TRUE == IdFound_b )
                        {
                            /* Get the Rx L-Pdu Id from the CanId Range configuration */
                            PduId_qu8 = CanIdRangeCfg_pcs->PduIdx_t;

                            /* Initialize pointer to Rx L-PDU structure */
                            RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + PduId_qu8;

                            /*Check If the type is not matching */
                            if(((RxPduCfg_pcst->CanIdtype_u8) & CANIF_PRV_CANID_TYPE_MASK) == (((RxPduCfg_pcst->CanIdtype_u8)>> CANIF_PRV_FOUR) & CanIdPduType_u8))
                            {
                                /* The Rx L-Pdu is found */
                                IdFound_b = TRUE;
                                break;
                            }
                            else
                            {
                                Index_u8++;

                                /* The Rx L-Pdu is not found */
                                IdFound_b = FALSE;

                                //PduId_qu8++;
                                RngLstPduId_qu8++;
                            }
                        }
                        else
                        {
                            Index_u8++;

                            /* The Rx L-Pdu is not found */
                            //IdFound_b = FALSE;
                          //  PduId_qu8++;
                            RngLstPduId_qu8++;
                        }
                    }
                }
                break;
                #endif
                /* FC_VariationPoint_START */
                #if CANIF_CFG_HRH_LIST_ENABLED == STD_ON
                case CANIF_PRV_BASIC_LIST_E:
                {
                    RngLstPduId_qu8 = HrhCfg_pcst->PduIdx_t;

                    /* [SWS_CANIF_00877] Check for correct recieved CanId type and CanId */
                    while((IdFound_b == FALSE) && (HrhCfg_pcst->NumRxPdus_u32> Index_u8))
                    {
                        CanIdListCfg_pcs = CanIf_Prv_ConfigSet_tpst->ListCfg_tpst + RngLstPduId_qu8;

                        /* Check whether the CanId received is same as configured */
                        #if (CANIF_RB_CALIBRATION == STD_ON)
                        if(CanIdNoIdType_t == CanIdListCfg_pcs->getCanIdLst_t())
                        #else
                        if(CanIdNoIdType_t == CanIdListCfg_pcs->CanId_t)
                        #endif
                        {
                            PduId_qu8 = CanIdListCfg_pcs->PduIdx_t;

                            /* Initialize pointer to Rx L-PDU structure */
                            RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + PduId_qu8;

                            /* validate if the canId type of recievied id is same as configured */
                            if(((RxPduCfg_pcst->CanIdtype_u8) & CANIF_PRV_CANID_TYPE_MASK) == (((RxPduCfg_pcst->CanIdtype_u8)>> CANIF_PRV_FOUR) & CanIdPduType_u8))
                            {
                                IdFound_b = TRUE;
                            }
                            else
                            {
                                IdFound_b = FALSE;
                                RngLstPduId_qu8++;
                                Index_u8++;
                            }
                        }
                        else
                        {
                            IdFound_b = FALSE;
                            RngLstPduId_qu8++;
                            Index_u8++;
                        }
                    }
                }
                break;
                #endif
                #endif
                /* FC_VariationPoint_END */
                default:

                    /* The Rx L-Pdu is not found */
                    IdFound_b = FALSE;
                    break;
             }/* end of switch(HrhCfg_pcst->HrhInfo_e) */
        }/*End of if(ModeChkStatus == CANIF_PRV_VALID_CHANNEL_MODE)...*/
    }/* end of if(CANIF_CS_STARTED == DeviceMode_tst) */

    /*[SWS_CANIF_00389] If Software Filtering rejects the received L-PDU, the CanIf shall end the receive indication */
    if(IdFound_b == TRUE)
    {
        /*[SWS_CANIF_00286] Store First Valid Call of CanIf_RxIndication()*/
        #if(CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
        if(ModeChkStatus==  CANIF_PRV_WAKEUP_MODE)
        {
            #if(CANIF_PUBLIC_WAKEUP_CHECK_VALID_BY_NM == STD_ON)
            if(RxPduCfg_pcst->ULisCanNm_b == TRUE)
            #endif
            {
                ControllerState_ps->ValidationSts_b = CANIF_SET;
            }
        }
        else
        #endif
        {
            /* Filtering of CanId if it belongs to CANNM */
            #if(CANIF_CANNM_TXID_FILTER != STD_OFF)
            /* Check if the received CanId is among the CanNm transmit CanIds */
            if((RxPduCfg_pcst->IndexForUL_u8 == CANIF_ONE) && (CanNmTxId_pt != NULL_PTR) && (*(CanNmTxId_pt + (RxPduCfg_pcst->RxPduTargetId_t)) == CanIdNoIdType_t))
            {
                IdFound_b = FALSE;
            }
            if(IdFound_b==TRUE)
            #endif
            {
                /*[SWS_CANIF_00390] DLC check shall be processed after software filtering is accepted */
                #if ( CANIF_CFG_DLC_CHECK == STD_ON )
                //check if below statement is correct
                //Also the below statement might not be needed as the correct rxpdu would be selected in the above statements
                RxPduCfg_pcst = CanIf_Prv_ConfigSet_tpst->RxPduConfig_pcst + PduId_qu8;

                /* Get the config RxPdu DLC */
                #if (CANIF_RB_CALIBRATION == STD_ON)
                if( RxPduCfg_pcst->getRxPduDlc != NULL_PTR )
                {
                    lConfigRxPduDlc = RxPduCfg_pcst->getRxPduDlc();
                }
                else
                #endif
                {
                    lConfigRxPduDlc = RxPduCfg_pcst->RxPduDlc_u8;
                }

                /* FC_VariationPoint_START */
                /* Check whether the Dlc error function has been configured */
                if( (PduInfoPtr->SduLength < lConfigRxPduDlc) && (NULL_PTR != CallBackPtr_Temp->Dlc_Error_Notification) )
                {
                    /* Report DLC error */
                    /* MR12 RULE 10.3 VIOLATION:The Warning is "Not Critical". The operand SduLength is reduced in size to uint8. The length of pdu will be maximum 64 which is covered in uint8. Thus there is no issue in reducing the size. */
                    (void)CallBackPtr_Temp->Dlc_Error_Notification(PduId_qu8, (uint8)(PduInfoPtr->SduLength));
                }
                /* FC_VariationPoint_END */

                /* [SWS_CANIF_00168] Det should be reported if Dlc check fails */
                CANIF_DET_REPORT_ERROR_VOID_RET((PduInfoPtr->SduLength < lConfigRxPduDlc), CANIF_RX_INDICATION_SID, CANIF_E_INVALID_DLC)

                if( PduInfoPtr->SduLength >= lConfigRxPduDlc )
                #endif /* End of #if(CANIF_CFG_DLC_CHECK == STD_ON) */
                {

                    /* [SWS_CANIF_00844] The L-SDU length is set to the sum of the payload length and MetaDataLength.*/
                    #if(CANIF_METADATA_SUPPORT == STD_ON)
                    PduInfoTemp_tst.SduLength = PduInfoPtr->SduLength + RxPduCfg_pcst->MetadataLength_u8;
                    // ULPduInfoTyp_tst.SduLength = PduInfoPtr->SduLength + RxPduCfg_pcst->MetadataLength_u8;
                    #else
                    PduInfoTemp_tst.SduLength = PduInfoPtr->SduLength;
                    //ULPduInfoTyp_tst.SduLength = PduInfoPtr->SduLength;
                    #endif
                    #if CANIF_CFG_USER_RX_ENABLED == STD_ON
                    ULPduInfoTypUSER_tst.SduLength= PduInfoTemp_tst.SduLength;
                    #endif
                    #if CANIF_CFG_UL_RX == STD_ON
                    ULPduInfoTyp_tst.SduLength= PduInfoTemp_tst.SduLength;
                    #endif

                    /*Calculate SduLength which is sum of DLC and metadatalength*/
                    //SduLength_u32 = ULPduInfoTyp_tst.SduLength;

                    #if(CANIF_METADATA_SUPPORT == STD_ON)

                    /*copy the payload data to SduDataBuffer as per DLC*/
                    for(Index_u8 =0; Index_u8 < PduInfoPtr->SduLength; Index_u8++)
                    {
                        SduDataBuffer_au8[Index_u8] = PduInfoPtr->SduDataPtr[Index_u8];
                    }

                    if(RxPduCfg_pcst->MetadataLength_u8 >= 1u)
                    {
                        /*[SWS_CANIF_00844]copy the received canid bytes into SduDataBuffer with LSB byte first after the payload data*/
                        for(Index_u8 = (uint8)(PduInfoPtr->SduLength); Index_u8 < (uint8)(PduInfoTemp_tst.SduLength); Index_u8++)
                        {
                            /* MR12 RULE 10.4, 10.1 VIOLATION:The Warning is "Not Critical". The left hand operand is reduced in size compared to right operand, as the left operand is only used to store two bits for which unit8 is sufficient  */
                            SduDataBuffer_au8[Index_u8]= (uint8)(CanIdNoIdType_t & CANIF_GET_ONE_BYTE);
                            CanIdNoIdType_t = (CanIdNoIdType_t>>SHIFT_8);
                        }
                    }
                    PduInfoTemp_tst.SduDataPtr = (uint8*)(&SduDataBuffer_au8[0]);
                    //ULPduInfoTyp_tst.SduDataPtr = (uint8*)(&SduDataBuffer_au8[0]);
                    #else
                    PduInfoTemp_tst.SduDataPtr =(uint8*)(PduInfoPtr->SduDataPtr);
                    //ULPduInfoTyp_tst.SduDataPtr = (uint8*)(PduInfoPtr->SduDataPtr);
                    #endif
                    #if CANIF_CFG_USER_RX_ENABLED == STD_ON
                    ULPduInfoTypUSER_tst.SduDataPtr= PduInfoTemp_tst.SduDataPtr;
                    #endif
                    #if CANIF_CFG_UL_RX == STD_ON
                    ULPduInfoTyp_tst.SduDataPtr= PduInfoTemp_tst.SduDataPtr;
                    #endif

                    /*[SWS_CANIF_00392] Notification status for valid reception is set */
                      #if(STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API)
                      if((CANIF_READ_NOTIFSTATUS_DATA == RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8) || (CANIF_READ_NOTIFSTATUS == RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8))
                      {
                          *(RxNotifPtr_pen + PduId_qu8) = CANIF_TX_RX_NOTIFICATION;
                      }
                      #endif
                    // for J1939Tp function pointer i am not able to test in this stand. Please make sure this test is done in Com stack stand.
                    /*Notification to UL shall be provided only for the successfully detected and accepted Pdu's */

                    /*[SWS_CANIF_00297] Buffer the data if CanIfRxPduReadData parameter is enabled */
                    #if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)
                    if((RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8 == CANIF_READ_NOTIFSTATUS_DATA) || (RxPduCfg_pcst->RxPduReadNotifyReadDataStatus_u8== CANIF_READ_DATA))
                    {
                        /* Buffer the received message */
                        CanIf_Prv_WriteRxBuffer(PduInfoTemp_tst.SduDataPtr, RxPduCfg_pcst,PduInfoPtr->SduLength,Mailbox->CanId);
                    }
                    #endif

                    /*[SWS_CANIF_00415] Notify the recive indication to upper layer */
                    /*Notification will be provided only if UL is configured or Name of UL(USER type) is configured */
                    if(RxPduCfg_pcst->IndexForUL_u8 != CANIF_PRV_NO_UL)
                    {
                        #if CANIF_CFG_USER_RX_ENABLED == STD_ON
                        if(RxPduCfg_pcst->IndexForUL_u8 > (CANIF_PRV_SEVEN+ CanIf_Prv_ConfigSet_tpst->NumCddRxPdus_t))
                        {
                            /* Notification to USER Type*/
                            ULPduInfoTypUSER_tst.SduCanId = (uint32)((Mailbox->CanId) & CANIF_MSB_BIT_RESET);
                            CanIf_Prv_ConfigSet_tpst->RxUSER_Ptr[(RxPduCfg_pcst->IndexForUL_u8)-(CANIF_PRV_EIGHT+CanIf_Prv_ConfigSet_tpst->NumCddRxPdus_t)].CanIfRxPduUserRxIndicationUser(RxPduCfg_pcst->RxPduTargetId_t, &ULPduInfoTypUSER_tst);
                        }
                        else
                        #endif
                        {
                            #if CANIF_CFG_UL_RX == STD_ON
                            /* FC_VariationPoint_START */
                            if (RxPduCfg_pcst->IndexForUL_u8 != CANIF_PRV_SIX)
                            {
                                #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
                                uint8_least RxPduCfg_IndexForUL_u8 = 0;
                                localCoreId_u8 = Schm_CanIf_XCore_GetCurrentCoreId();

                                if (RxPduCfg_pcst->IndexForUL_u8 > (uint8_least) CDD)
                                {
                                    RxPduCfg_IndexForUL_u8 = (uint8_least)CDD;
                                    destCoreId_u8 = CanIf_XCore_Config_pcst->CanIf_XCoreUserTypeCoreAffinity[RxPduCfg_IndexForUL_u8];
                                }
                                else
                                {
                                    destCoreId_u8 = CanIf_XCore_Config_pcst->CanIf_XCoreUserTypeCoreAffinity[RxPduCfg_pcst->IndexForUL_u8];
                                }
                                pipeId_u16 = CanIf_XCore_Config_pcst->CanIf_XCoreRxPipeMatrix[localCoreId_u8][destCoreId_u8];

                                if(pipeId_u16 == CANIF_XCORE_INVALID_PIPE_ID)
                                {
                                    /* Notification to the upper layer*/
                                   (void) CanIf_XCore_LocalCore_RxIndication(RxPduCfg_pcst, &ULPduInfoTyp_tst);
                                }
                                else
                                {
                                    xCorePipeTxRxParams.PduId = (PduIdType)PduId_qu8;
                                    xCorePipeTxRxParams.PduInfoPtr = &ULPduInfoTyp_tst;
                                    xCorePipeTxRxParams.callbackIndex_u8 = (uint8)RxPduCfg_pcst->IndexForUL_u8;
                                    xCorePipeTxRxParams.ControllerId_u8 = Mailbox->ControllerId;

                                    /* The request is sent to the pipe when the UL resides in a different core */
                                    (void)CanIf_XCore_PipeWrite(pipeId_u16, &xCorePipeTxRxParams);
                                }
                                #else
                                /* Notification to Autosar types */
                                (void)CanIf_XCore_LocalCore_RxIndication(RxPduCfg_pcst, &ULPduInfoTyp_tst);
                                #endif
                            }
                            else
                                /* FC_VariationPoint_END */
                            {
                                /* Notification to Autosar types */
                                (void)CanIf_XCore_LocalCore_RxIndication(RxPduCfg_pcst, &ULPduInfoTyp_tst);
                            }
                            #endif
                        }
                    }/*end of if(RxPduCfg_pcst->IndexForUL_u8 != CANIF_PRV_NO_UL)*/
                }/*end of if(RxPduCfg_pcst->IndexForUL_u8 > (CANIF_PRV_SEVEN+ CanIf_Prv_ConfigSet_tpst->NumCddRxPdus_t))*/
            }/*end of if(IdFound_b==TRUE)*/
        }/*end of if(ModeChkStatus==  CANIF_PRV_WAKEUP_MODE)*/
    }/*end of if(IdFound_b == TRUE)*/
#endif/*end of #if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)*/
}/* End of CanIf_RxIndication() */

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
#if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
FUNC(Std_ReturnType, CANIF_CODE)CanIf_XCore_LocalCore_RxIndication(P2CONST(CanIf_Cfg_RxPduType_tst, AUTOMATIC, CANIF_CFG_CONST) CanIf_RXPduConfig_pst,
                                                                    P2CONST(PduInfoType, AUTOMATIC, CANIF_CFG_CONST) CanIf_ULPduinfo_pst)
{
        Std_ReturnType lretval = E_OK;
        /* Pointer to Tx Pdu configuration */
        P2CONST(CanIf_Cfg_RxPduType_tst, AUTOMATIC, CANIF_CFG_CONST) RxPduConfig_pst;

        /* If Ptr is NULL, report to DET and return E_NOT_OK */
        CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_RXPduConfig_pst == NULL_PTR), CANIF_INIT_SID, CANIF_E_PARAM_POINTER)

        /* If Ptr is NULL, report to DET and return E_NOT_OK */
        CANIF_DET_REPORT_ERROR_NOT_OK((CanIf_ULPduinfo_pst == NULL_PTR), CANIF_INIT_SID, CANIF_E_PARAM_POINTER)

        RxPduConfig_pst = CanIf_RXPduConfig_pst;

        #if CANIF_CFG_UL_RX == STD_ON

        /* Notification to Autosar types */
        CanIf_Prv_ConfigSet_tpst->RxAutosarUL_Ptr[RxPduConfig_pst->IndexForUL_u8].CanIfRxPduIndicationName(RxPduConfig_pst->RxPduTargetId_t, CanIf_ULPduinfo_pst);
        #endif

        return lretval;
}
#endif/*End of #if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)*/
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/**********************************************************************************************************
*                     End of File
**********************************************************************************************************/
