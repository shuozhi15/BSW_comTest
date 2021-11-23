



/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanIf / AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.0.2.1                ECU Parameter Definition Version
 *

 </VersionHead>*/



#ifndef  CANIF_CFG_H
#define  CANIF_CFG_H
/**************************************************************************/
/*                            Include Section                             */
/**************************************************************************/

/*EcuM_Cbk.h required to use EcuM_WakeupSourceType*/
/*Version check for EcuM and CanIf done in CanIf_Priv.h*/



/*
 **************************************************************************
 * Defines
 **************************************************************************
 */
/**************************************************************************/
/*  Common Published Information                                          */
/**************************************************************************/
 /**
 * @ingroup CANIF_H
 *
 * Vendor Id
 */
#define CANIF_VENDOR_ID                          6u

/**
 * @ingroup CANIF_H
 *
 * Module Id
 */
#define CANIF_MODULE_ID                          60u

/**
 * @ingroup CANIF_H
 *
 * Autosar Release Major Version
 */
#define CANIF_AR_RELEASE_MAJOR_VERSION           4u
/**
 * @ingroup CANIF_H
 *
 * Autosar Release Minor Version
 */
#define CANIF_AR_RELEASE_MINOR_VERSION           2u
/**
 * @ingroup CANIF_H
 *
 * Autosar Release revised version
 */
#define CANIF_AR_RELEASE_REVISION_VERSION        2u

/**
 * @ingroup CANIF_H
 *
 * Software Release Major Version
 */
#define CANIF_SW_MAJOR_VERSION                   6u
/**
 * @ingroup CANIF_H
 *
 * Software Release Minor Version
 */
#define CANIF_SW_MINOR_VERSION                   0u
/**
 * @ingroup CANIF_H
 *
 * Software Release Patch Version
 */
#define CANIF_SW_PATCH_VERSION                   0u

/* FC_VariationPoint_START */

/* FC_VariationPoint_END */



#define CANIF_CFG_VAR_PC                                1

#define CANIF_CFG_VAR_PBS                               2

#define CANIF_CONFIGURATION_VARIANT         CANIF_CFG_VAR_PC

#define CANIF_TOTAL_TXPDUS      3u 
                              
#define CANIF_TOTAL_TXBUFFERS   3u                
  
#define CANIF_TOTAL_CTRLS       1u                 
  
#define CANIF_TOTAL_DYNAMIC_PDUS 0u  


#define CANIF_RX_DYNAMIC_PDUS 0u





#define CANIF_METADATA_SUPPORT                  STD_OFF
#define CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT  STD_OFF
#define CANIF_READRXPDU_NOTIFY_STATUS_API       STD_ON
#define CANIF_TRIGGERTRANSMIT_SUPPORT                  STD_OFF
#define CANIF_TXOFFLINEACTIVE_SUPPORT             STD_OFF
#define CANIF_SET_BAUDRATE_API                  STD_OFF
/**************************************************************************/

/* Description :  Enables or disables CanIf_SetBaudrate API               */
/*                                                                        */
/* Range       :  STD_ON/STD_OFF                                          */
/**************************************************************************/
//#define CANIF_PUBLIC_SET_BAUDRATE_SUPPORT             STD_OFF

/**************************************************************************/

/* Description :  Enables or disables the API CanIf_CancelTransmit()     */
/*                                                                         */
/* Range       :  STD_ON/STD_OFF                                          */
/**************************************************************************/
#define CANIF_PUBLIC_CANCEL_TRANSMIT_SUPPORT        STD_OFF

/**************************************************************************/
/* Description :  Enables or disables Rx indication for busoff recovery   */
/*                                                                        */
/* Range       :  STD_ON/STD_OFF                                          */
/**************************************************************************/

#define CANIF_PUBLIC_BUSOFF_RECOVERY_FROM_RXINDICATION        STD_OFF

/*******************************************************************************************************/
/* Description :  Enables or disables the API CanIf_DirectHw_Write() &  CanIf_Get_DirectHw_Info() */
/*                                                                        */
/* Range       :  STD_ON/STD_OFF                                          */
/*******************************************************************************************************/
#define CANIF_DIRECT_HW_WRITE               STD_OFF




/******************************************************************************
** Description :  Enables or disables the development error detection and    **
**                notification mechanism                                     **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_PUBLIC_DEV_ERROR_DETECT                        STD_OFF


/**************************************************************************/
/* Description :  Enables/disables API for reading the notification status*/
/*                of transmit and receive L-PDU                           */
/* Range       :  STD_ON/STD_OFF                                                  */
/**************************************************************************/
#define CANIF_READTXPDU_NOTIFY_STATUS_API         STD_ON

/***************************************************************************/
/* Description :  Enables / Disables the APIs CanIf_ReadTxmsgId() and      */
/*                CanIf_ReadRxmsgId() for reading configured CanId for     */
/*                configured hardware object (HOH).                        */
/* Range       :  STD_ON/STD_OFF                                           */
/**************************************************************************/
#define CANIF_READMSGID_API                STD_OFF


/**************************************************************************/
/* Description : Enables/disables the user notification when a PDU in the TxBuffer is cleared */
/*  				due to CAN driver couldn't accept the transmission request                */
/* Range       :  STD_ON/STD_OFF											      */
/**************************************************************************/
#define CANIF_USER_TXBUFCLEARNOTIFY_API 		STD_OFF
/**************************************************************************/

/* Description :  Configure Error Passive support                         */
/* Range STD_OFF   :  STD_ON/STD_OFF                                                  */
/**************************************************************************/
#define CANIF_ERROR_PASSIVE_SUPPORT             STD_OFF
 

/**************************************************************************/
/* Description :  Configure Controller Wakeup support                                */
/* Range       :  STD_ON/STD_OFF                                                  */
/**************************************************************************/
#define CANIF_CTRL_WAKEUP_SUPPORT                   STD_OFF

/**************************************************************************/
/* Description :  Configure  Transceiver Wakeup support                                */
/* Range       :  STD_ON/STD_OFF                                                  */
/**************************************************************************/
#define CANIF_TRCV_WAKEUP_SUPPORT                     STD_OFF

/**************************************************************************/
/* Description :  Configure Wakeup support                                */
/* Range       :  STD_ON/STD_OFF                                                  */
/**************************************************************************/
#define CANIF_CFG_TRCV_DRV_SUPPORT                             STD_OFF

/**************************************************************************/
/* Description :  Configure Wakeup Validation                                 */
/* Range       :  STD_ON/STD_OFF                                                  */
/**************************************************************************/
#define CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_API       STD_OFF

/**************************************************************************/
/* Description :  Configure Wakeup Validation                                 */
/* Range       :  0....Num of transceivers                                                  */
/**************************************************************************/
#define CANIF_NUM_TRANSCEIVERS                        0


/**************************************************************************/
/* Description :  Enables/disables API for reading the version information*/
/*           .     about the CAN interface                                */
/* Range       :  STD_ON/STD_OFF                                                  */
/**************************************************************************/
#define CANIF_PUBLIC_VERSIONINFO_API                     STD_OFF

/**************************************************************************/
/* Description :  Enables/disables API for reconfiguration of the CAN     */
/*                Identifier for each transmit L-PDU                      */
/* Range       :  STD_ON/STD_OFF                                                  */
/**************************************************************************/
#define CANIF_PUBLIC_SETDYNAMICTXID_API                 STD_OFF


/**************************************************************************/
/* Description :  Enables/disables API for reconfiguration of the CAN     */
/*                Identifier for each receive L-PDU                      */
/* Range       :  STD_ON/STD_OFF                                          */
/**************************************************************************/
 #define CANIF_RB_CHANGERXPDUID_API                    STD_OFF



/**************************************************************************/
/* Description :  Enables/disables use of transmit buffers                */
/* Range       :  STD_ON/STD_OFF                                                  */
/**************************************************************************/

#define CANIF_PUBLIC_TXBUFFERING                     STD_OFF






/**************************************************************************/

/* Description :  Enables/disables use of receive buffers                 */
/* Range       :  STD_ON/STD_OFF                                                  */
/**************************************************************************/


/********************************************************************************************************/
/* Description :  Max Pdu Index for FULL CAN Pdus based on ecuC.ecucPduCollection.pduIdTypeEnum selection           */
/* Range       :  0xFFu/ 0xFFFFu                                                                                    */
/********************************************************************************************************/
#define CANIF_MAX_PDU_INDEX  0xFFFFu










/******************************************************************************
** Description :  Enables and disables the API for reading the               **
**                notification status of receive L-PDUs.                     **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API      STD_ON
#define CanIf_CFG_Size_Of_CANID                       sizeof(Can_IdType)

/******************************************************************************
** Description :  Enables/disables API for reading received L-PDU data       **
** Range       : STD_ON/STD_OFF                                              **
******************************************************************************/
#define CANIF_PUBLIC_READRXPDU_DATA_API               STD_OFF
/******************************************************************************
** Description :  Selects support for wake up validation                     **
** Range       : STD_ON/STD_OFF                                              **
******************************************************************************/
#define CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT               STD_OFF
/******************************************************************************
** Description :  Configure Wakeup support                                   **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_WAKEUP_SUPPORT                    STD_OFF






/******************************************************************************
** Description :  Selects support for Rx feature based on configuration      **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_CFG_RX_FEATURE_ENABLED                     STD_ON
/******************************************************************************
** Description :  Total Number of Rx pdus to be handled                      **
** Range       :  0..Max Number of defined CanRxPduIds                       **
******************************************************************************/
#define CANIF_CFG_NUM_CANRXPDUIDS                     5u

/******************************************************************************
** Description :  Total Number of Hrh instances to be handled                **
** Range       :  0..Max Number of defined canIfHrhCfg                       **
******************************************************************************/

#define CANIF_CFG_TOTAL_HRH_NUM                     5u

/******************************************************************************
** Description :  Total Number of Hoh instances to be handled                **
** Range       :  0..Max Number of defined canIfHrhCfg                       **
******************************************************************************/

#define CANIF_CFG_TOTAL_HOH_NUM                     8u

/******************************************************************************
** Description :  Total Number of Hrh instances to be handled                **
** Range       :  0..Max Number of defined canIfHrhCfg                       **
******************************************************************************/

#define CANIF_CFG_NUM_HRH                     5u

/******************************************************************************
** Description :  Total Number of Rx Pdu's whose                             ** 
**                CanIfRxPduUserRxIndicationUL is configured(Not USER)       **
** Range       :  0..Total Number of Rxpdu's defined other than user         **
******************************************************************************/

#define CANIF_CFG_NUM_CANRXPDUIDS_NOT_USER                     5u

/******************************************************************************
** Description :  Macro is generated as true if CanIfHrhRangeCfg is          ** 
**                configured atleast for one Hrh                             **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/

#define CANIF_CFG_RX_RANGE_CONFIG                     STD_OFF
/******************************************************************************
** Description :  Macro generates the size of an array needed to buffer Rx   ** 
**                Pdu's                                                      **
** Range       :  Buffer size                                                **
******************************************************************************/

#define CANIF_CFG_RX_BUFFER_SIZE                     0u



/******************************************************************************
** Description :  To obtain total number unique of Hoh across all variants   **  
** Range       :  1..Total no of hoh configured                              **
******************************************************************************/
#define CANIF_CFG_TOTAL_HOH                    8


/******************************************************************************
** Description :  To obtain total number unique of Hoh across all variants   **  
** Range       :  1..Total no of hoh configured                              **
******************************************************************************/
#define CANIF_CFG_READRXPDU_DATA_IN_RXPDUS                    STD_OFF

/******************************************************************************
** Description :  To obtain total number USER Rxpdus across all variants     **  
** Range       :  0..Total no of user pdus configured                        **
******************************************************************************/
#define CANIF_CFG_USER_RX_ENABLED              STD_OFF

/******************************************************************************
** Description :  To obtain total number Rxpdus across all variants having UL**  
** Range       :  0..Total no of pdus with CanIfRxPduUserRxIndicationUL      **
**                configured                                                 **
******************************************************************************/
#define CANIF_CFG_UL_RX              STD_ON


/******************************************************************************
** Description :  Total number of unique top level variants                  **  
** Range       :  1..Total no unique TLV configured                          **
******************************************************************************/
#define CANIF_CFG_NO_OF_TLV                    1



/******************************************************************************
** Description :  Macro is generated as true if atleast one canIfTrcvDrvCfg  **
**                is configured                                              **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_CFG_TRCV_DRV_SUPPORT                    STD_OFF
/******************************************************************************
** Description :  Macro generates no of tranceivers configured for all       **
**                tranceiver driver.                                         **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/

#define CANIF_CFG_NUM_TRCVS                           0
/******************************************************************************
** Description :  Macro is generated as true if atleast one                  **  
**                canIfTrcvWakeupSupport is enabled                          **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_CFG_TRCV_WAKEUP_SUPPORT                    STD_OFF

/******************************************************************************
** Description :  Macro is generated as true if atleast one                  **  
**                canIfTrcvWakeupSupport is enabled                          **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_CFG_NO_TRCV_WAKEUP_SUPPORT                 0u

/******************************************************************************
** Description :  Macro is generated as true if atleast one                  **  
**                canTrcvHwPnSupport is enabled                              **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_CFG_TRCV_PN_SUPPORT                 STD_OFF


/* Total number of Controllers present in one or more drivers */


#define CANIF_NUM_CONTROLLERS       1


/******************************************************************************
** Description :  Selects support for Rx feature based on configuration      **
** Range       :  STD_ON/STD_OFF                       **
******************************************************************************/
#define CANIF_CFG_TX_FEATURE_ENABLED                     STD_ON



/******************************************************************************
** Description :  Macro is generated as true if CanIfHrhListCfg  is          ** 
**                configured atleast for one Hrh                             **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_CFG_HRH_LIST_ENABLED                     STD_OFF

/******************************************************************************
** Description :  Macro is generated as true if CanIfHrhListCfg  is          ** 
**                configured atleast for one Hrh                             **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/


/* Configure support of filtering of CanNm transmit CanIds during reception */
#define CANIF_CANNM_TXID_FILTER                 STD_OFF

/******************************************************************************
** Description :  Enables/Disables the APIs CanIf_ReadTxmsgId() and          **
**                CanIf_ReadRxmsgId() for reading configured CanId for       **
**                configured HRH.                                            **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_RB_READMSGID_API               STD_OFF
/******************************************************************************
** Description :  Enables/Disables the APIs CanIf_ControllerErrorPassive()   **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_RB_ERROR_PASSIVE_SUPPORT               STD_OFF
/******************************************************************************
** Description :  Enables/Disables calibration feature                       **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/

#define CANIF_RB_CALIBRATION                STD_OFF


/******************************************************************************
** Description :  Enables/Disables Node calibration feature                  **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_RB_NODE_CALIBRATION                STD_OFF








/******************************************************************************
** Description :  Selects whether the DLC check is supported                 **
** Range       :  STD_ON/STD_STD_OFF                                         **  
******************************************************************************/
#define CANIF_CFG_DLC_CHECK                         STD_OFF







/**************************************************************************/
/* Description :  Gives the Number of BASIC CAN HTHs.                      */
/* Range       :  0..Max Number of BASIC CAN HTHs                            */
/**************************************************************************/



/******************** Parameters Newly Introduced *************************/





/*Total number of Tx-Pdus configured in CanIf*/
//#define CANIF_TOTAL_TXPDUS                  3

/*Total number of Tx-Pdus configured in CanIf*/
//#define CANIF_TOTAL_TXBUFFERS               3 


/*Total number of Rx-Pdus configured in CanIf*/
#define CANIF_TOTAL_RXPDUS                  5


/*Number of entries in CanIf_TxPduId_CustId_LUT[][]*/
#define CANIF_NUM_CUSTID_ENTRIES            3













/* Configuration for use of extended CAN identifiers */
#define CANIF_FD_SUPPORT                       STD_OFF



/* Instance ID */

#define CANIF_INSTANCE_ID                         1







/* Configure CanIf Lite */
#define CANIF_LITE_CONFIGURATION                STD_OFF




/* Configure transmission of CANTP/USER Pdus when the Tx Pdu mode is OFFLINE */
#define CANIF_USER_TP_TX_OFFLINE_MODE            STD_OFF


/* Configuration for enabling/disabling the API CanIf_GetTxConfirmationState() */
//#define CANIF_PUBLIC_TXCONFIRM_SUPPORT          STD_OFF


/*PN support*/
#define CANIF_PUBLIC_PN_SUPPORT            STD_OFF



/* CanIf variant info */
#define CANIF_VARIANT_INFO				1   /* PRE-COMPILE */
    



/* Configure support of filtering of CanNm transmit CanIds during reception */
#define CANIF_CANNM_TXID_FILTER                 STD_OFF






/* Enable/Disable CanNm Support */
#define CANIF_CANNM_ENABLED                     STD_ON




 
 
 
 



/* Enable/Disable CanSm Support */
#define CANIF_CANSM_ENABLED                     STD_ON




/* Enable/Disable J1939Nm Support */
#define CANIF_J1939NM_ENABLED                     STD_OFF


/* Enable/Disable CanTp Support */
#define CANIF_CANTP_ENABLED                     STD_OFF



/* Enable/Disable CAN_TSYN Support */
#define CANIF_CANTSYN_ENABLED                    STD_OFF




/* Enable/Disable PduR Support */
#define CANIF_PDUR_ENABLED                     STD_ON


 






/* Enable/Disable CDD Support */
#define CANIF_CDD_RX_ENABLED                    STD_OFF



/* Enable/Disable USER Support */
#define CANIF_USER_RX_ENABLED                    STD_OFF




/* Enable/Disable CDD Support */
#define CANIF_CDD_TX_ENABLED                STD_OFF



/* Enable/Disable USER Support */
#define CANIF_USER_TX_ENABLED                STD_OFF







/* Enable/Disable XCP Support */
#define CANIF_XCP_ENABLED                       STD_OFF







/* Enable/Disable XCP Support */
#define CANIF_J1939TP_ENABLED                       STD_OFF


/******************************************************************************
** Description :  Macro is generated as true if EcuM   is configured         **
**                                                                           **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_ECUM_ENABLED                    STD_OFF


/*Wakeup Validation by NM-Pdu*/
#define CANIF_PUBLIC_WAKEUP_CHECK_VALID_BY_NM         STD_OFF


/* Configure support of BASIC CAN Reception */
#define CANIF_BASIC_CAN_SUPPORT                   STD_OFF

/* Configure support of CanId List for reception */
#define CANIF_BASIC_CAN_SUPPORT_LIST            STD_OFF

/* Macro generated via scripts when multiple Ranges of CanIds are configured under a HRH */
#define CANIF_RXPDU_CANID_RANGE                    STD_OFF
























/* Definition of Rx Receive Handles (HRH) for CanIfInitCfg*/





/* Definition of TxPdu handles(CanIfTxPduId)*/
#define CanIfConf_CanIfTxPduCfg_BMS_20_Can_Network_CANNODE_0        0
#define CanIfConf_CanIfTxPduCfg_BMS_Vol_8_Can_Network_CANNODE_0        1
#define CanIfConf_CanIfTxPduCfg_NMm_BMS_Can_Network_CANNODE_0        2


/* Definition of CanIfCtrlId*/
#define CanIfConf_CanIfCtrlCfg_Can_Network_CANNODE_0        0

/* Definition of CanIfTrcvId*/





/* Definition of RxPdu handles(CanIfRxPduId)*/
#define CanIfConf_CanIfRxPduCfg_TMR_3_Can_Network_CANNODE_0        0
#define CanIfConf_CanIfRxPduCfg_NMm_HCU_Can_Network_CANNODE_0        1
#define CanIfConf_CanIfRxPduCfg_NMm_CDU_Can_Network_CANNODE_0        2
#define CanIfConf_CanIfRxPduCfg_NMm_MCUR_Can_Network_CANNODE_0        3
#define CanIfConf_CanIfRxPduCfg_OBC_1_Can_Network_CANNODE_0        4








/* FC_VariationPoint_START */
/******************************************************************************
** Description :  Enables or disables CanIf XCore                            **
** Range       :  STD_ON/STD_OFF                                             **
******************************************************************************/
#define CANIF_XCORE_CFG_ENABLED  (STD_OFF)



/* FC_VariationPoint_END */



extern VAR(uint32, CANIF_VAR) CanIf_CustId_Table[] ;



/*PduType used by USER */
typedef struct
{
    P2VAR(uint8,TYPEDEF,CANIF_APPL_DATA) SduDataPtr;
    PduLengthType       SduLength;
    Can_IdType     SduCanId;    


}CanIf_PduInfoType;


/** Ram structure for CanId Request buffer */
typedef struct
{
    Can_IdType CanId;
    PduIdType swPduHandle;
    uint8 SduLength;
    uint8 BufferIndex;

}CanIf_Cfg_CanIdBuffer_tst;

typedef enum
{
    STANDARD_CAN    = 0x00,
    STANDARD_FD_CAN = 0x01,
    EXTENDED_CAN    = 0x02,
    EXTENDED_FD_CAN = 0x03
}CanIf_Cfg_TxPduCanIdType_ten;

  
 enum BufferIdx
{
    
    
    CanIf_Buffer_Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_1_e
    ,
    CanIf_Buffer_Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_2_e
    ,
    CanIf_Buffer_Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_3_e
};

enum HthIdx
{
    
    
    CanIf_Hth_Can_Network_CANNODE_0_Tx_Std_MailBox_1_e
    ,
    CanIf_Hth_Can_Network_CANNODE_0_Tx_Std_MailBox_2_e
    ,
    CanIf_Hth_Can_Network_CANNODE_0_Tx_Std_MailBox_3_e
};

enum CtrlIdx
{
    
    
    CanIf_Ctrl_Can_Network_CANNODE_0_e
};

typedef enum
{
    CANIF_NO_UL = 0,
    CAN_NM,
    CAN_TP,
    CAN_TSYN,
    J1939NM,
    J1939TP,
    PDUR,
    XCP,
    CDD,
    USER,
    MAX_USER_TYPE

}CanIf_Cfg_UserType_ten;


typedef enum
{
    CANIF_BASIC,
    CANIF_FULL

}CanIf_Cfg_CanHandleType_ten;

/* Defines the type of HRH.  */
typedef enum
{
    CANIF_PRV_FULL_E = 0,
    CANIF_PRV_BASIC_RANGE_E,
    /* FC_VariationPoint_START */
    CANIF_PRV_BASIC_LIST_E
    /* FC_VariationPoint_END */
}CanIf_Prv_HrhType_ten;

/* Return value of the DLC error notification. */
typedef enum
{
    CANIF_DLC_OK = 0,
    CANIF_DLC_E_FAILED

}CanIf_DlcErrorReturnType;

/*Structure for Callback functions*/
typedef struct
{
    #if CANIF_CFG_TRCV_DRV_SUPPORT == STD_ON
    /*User_TransceiverModeIndication*/
    P2FUNC(void, TYPEDEF, User_TransceiverModeIndication) (VAR(uint8,AUTOMATIC) ,VAR(CanTrcv_TrcvModeType,AUTOMATIC));

    #if (CANIF_CFG_TRCV_WAKEUP_SUPPORT == STD_ON && CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON)
    /*User_ValidateWakeupEvent*/
    P2FUNC(void, TYPEDEF, User_ValidateWakeupEvent) (VAR(EcuM_WakeupSourceType,AUTOMATIC) WakeupSource);
    #endif
    #endif/*End of #if CANIF_CFG_TRCV_DRV_SUPPORT == STD_ON */

    #if(CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    P2FUNC(void, TYPEDEF, User_ClearTrcvWufFlagIndication) (VAR(uint8,AUTOMATIC) Transceiver);
    P2FUNC(void, TYPEDEF, User_CheckTrcvWakeFlagIndication) (VAR(uint8,AUTOMATIC) Transceiver);
    P2FUNC(void, TYPEDEF, User_ConfirmPnAvailability) (VAR(uint8,AUTOMATIC) TransceiverId);
    #endif

    P2FUNC(void, TYPEDEF, User_ControllerBusOff) (VAR(uint8,AUTOMATIC) ControllerId);

    P2FUNC(void, TYPEDEF, User_ControllerModeIndication) (VAR(uint8,AUTOMATIC) ControllerId,
            VAR(CanIf_ControllerModeType,AUTOMATIC) ControllerMode);

    /* FC_VariationPoint_START */
    /*User_ControllerErrorPassive*/
    #if (CANIF_RB_ERROR_PASSIVE_SUPPORT != STD_OFF)
    P2FUNC(void, TYPEDEF, User_ControllerErrorPassive) (VAR(uint8,AUTOMATIC));
    #endif
        
    #if (CANIF_CFG_DLC_CHECK != STD_OFF)
    /*Dlc_Error_Notification*/
    P2FUNC(CanIf_DlcErrorReturnType, TYPEDEF, Dlc_Error_Notification) (VAR(PduIdType,AUTOMATIC) ,VAR(uint8,AUTOMATIC));
    #endif
    /* FC_VariationPoint_END */    
}CanIf_CallbackFuncType;


/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */
 
#if(CANIF_TRCV_WAKEUP_SUPPORT== STD_ON)
typedef struct
{
    uint8 TransceiverId;
    uint8 ControllerId;
    EcuM_WakeupSourceType WakeupSourceId;
}CanIf_Wakeup;
#endif

typedef struct
{
    P2FUNC(void, TYPEDEF, CddRxIndication)(VAR(PduIdType, AUTOMATIC),P2CONST(PduInfoType, AUTOMATIC,CANIF_APPL_CONST));
} CanIf_CddRxCbk_Prototype;




/* Hrh configuration structure */
typedef struct
{
    /*Hrh Type
     * Full        -- 0
     * Basic Range -- 1
     * Basic list  -- 2 */
    CanIf_Prv_HrhType_ten           HrhInfo_e;

    /*Index to Recieve pdu
    * FULL CAN: generation of stPduIdx_t is same as pduid.
    * Basic Range: Gives the mapping index to the Range structure CanIf_RxPduRangeConfig
    * Basic List: Gives mapping index to the List structure */
    PduIdType               PduIdx_t;

    /*Number of Rx pdu's referring the hrh */
    uint32                  NumRxPdus_u32;

    /*This element is enabled if Mask based filtering is used for the Hrh */
    boolean                  HrhRangeMask_b;

    /*Controller referred by the Hrh*/
    uint8                   ControllerId_u8;
    
    /*CanId accepted by the Hrh */
    #if (CANIF_RB_READMSGID_API == STD_ON)
    Can_IdType              CanId_t;
    #endif
   
   
    #if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
    /*HW object referred by the Hrh*/
    Can_HwHandleType        CanObjectId;
    #endif
    

}CanIf_Cfg_Hrhtype_tst;

//An array for above structure will be generated(in exsisting desing in CanIf_Cfg_Internal.h).
//extern CONST(CanIf_HrhConfigType, CANIF_CONST) CanIf_HrhConfig_0[];
//the above array should have hardware object information sorted as per hardware object Id generated by Can Driver.

/* Receive L-Pdu configuration structure */
typedef struct
{
    #if(STD_ON == CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API) || (CANIF_PUBLIC_READRXPDU_DATA_API != STD_OFF)
    /*The element provides if CanIfRxPduReadData and CanIfRxPduReadNotifyStatus is enabled
     * Configration                                                     Value
     * CanIfRxPduReadData= true, CanIfRxPduReadNotifyStatus= true       CANIF_READ_NOTIFSTATUS_DATA
     * CanIfRxPduReadData= true, CanIfRxPduReadNotifyStatus= false      CANIF_READ_DATA
     * CanIfRxPduReadData= false, CanIfRxPduReadNotifyStatus= true      CANIF_READ_NOTIFSTATUS
     * CanIfRxPduReadData= false, CanIfRxPduReadNotifyStatus= false     CANIF_READ_NONE   */
    uint8 RxPduReadNotifyReadDataStatus_u8;
    #endif
    #if(CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON)
    /* Stores the Offset of the L-PDU buffer in RAM */
    uint16    RxBuffer_u16;

    #endif

    #if((CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_SUPPORT == STD_ON) && (CANIF_PUBLIC_WAKEUP_CHECK_VALID_BY_NM== STD_ON))
    /*Element is true if the CanIfRxPduUserRxIndicationUL is CANNM */
    boolean ULisCanNm_b;
    #endif

    /* Index to the fetch the recive callback function name
     * For all Pdus for which CanIfRxPduUserRxIndicationUL is configured function name is fetched from
     * CanIf_Prv_ULName_ta_fct array.
     * For USER type of pdu the function name is fetched from CanIf_Prv_RxUSERName_ta_fct array
     * Index        Function Name
     * 0            For the pdu with no CanIfRxPduUserRxIndicationUL and canIfRxPduUserRxIndicationName
     * 1            CanNm_RxIndication
     * 2            CanTp_RxIndication
     * 3            CanTSyn_RxIndication
     * 4            J1939Nm_RxIndication
     * 5            J1939Tp_RxIndication
     * 6            PduR_CanIfRxIndication
     * 7            Xcp_CanIfRxIndication
     * 8            Cdd1 (from 8 CDD function starts)
     * 9            Cdd2
     * 10           USER1(Index for USER starts after CDD indexing is completed)
     * 11           USER2*/
    uint8_least IndexForUL_u8;

    /* CanId type*/
    /* To optimize the checking of CanIdType in source code CanIdType is generated in Following way                   *
     * Type                 Value                                                                                     *
     * STANDARD_CAN         0x20                                                                                      *
     * STANDARD_FD_CAN      0x31                                                                                      *
     * STANDARD_NO_FD_CAN   0x30                                                                                      *
     * EXTENDED_CAN         0x22                                                                                      *
     * EXTENDED_FD_CAN      0x33                                                                                      *
     * EXTENDED_NO_FD_CAN   0x32                                                                                      *
     * Lower nibble provides the CanIdType as forwarded by Can Driver. Eg: STD_CAN:0, EXT_FD:3                        *
     * Higher nibble: is used maily for STD_CAN and EXT_CAN. As STD_CAN can have STD_FD or STD_NO_FD hence FD bit     *
     * should be a dont care bit. Hence 0010 is used. This is same for EXT_CAN.                                       *
     * For other cases e.g STD_FD it is just used as a mask for canidtype provided by can Driver                      */
    uint8 CanIdtype_u8;

    /* Pdu Dlc*/
    uint8 RxPduDlc_u8;    
    
    #if (CANIF_RB_CALIBRATION == STD_ON)
    /*getRxPduDlc*/
    P2FUNC(uint8, TYPEDEF,  getRxPduDlc)(void);
    #endif
    

    #if CANIF_METADATA_SUPPORT == STD_ON
    /*Pdu CanIdMask  */
    uint32 RxPduCanIdMask_u32;

    /* Pdu Metadata Length*/
    uint8 MetadataLength_u8;
    #endif

    /*Pdu CanId */
    Can_IdType RxPduCanId;
    
    
    #if (CANIF_RB_CALIBRATION == STD_ON)
    /*getCanId*/
    P2FUNC(Can_IdType, TYPEDEF,  getCanId)(void);
    #endif
    

    /*Hrh ref */
    PduIdType Hrhref_t;

    /*Target Id*/
    PduIdType RxPduTargetId_t;
    
    
    #if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
    /* PduId Type */
    PduIdType RxPduType;
    #endif
    

}CanIf_Cfg_RxPduType_tst;


/* Pointer to the function to invoke reception callback CanIfRxPduUserRxIndicationUser */
typedef struct
{
    P2FUNC(void, TYPEDEF, CanIfRxPduIndicationName) (VAR(PduIdType,AUTOMATIC), P2CONST(PduInfoType, TYPEDEF, CANIF_APPL_DATA ));
}CanIf_RxCbk_Prototype;

/* Pointer to the function to invoke reception callback for USER functions */
typedef struct
{
    P2FUNC(void, TYPEDEF, CanIfRxPduUserRxIndicationUser) (VAR(PduIdType,AUTOMATIC), P2CONST(CanIf_PduInfoType, TYPEDEF, CANIF_APPL_DATA ));
}CanIf_RxUSERCbk_Prototype;


#if (CANIF_CFG_RX_RANGE_CONFIG == STD_ON)
typedef struct
{
    /* Lower CanId as defined by CanIfRxPduCanIdRangeLowerCanId */
    #if (CANIF_RB_CALIBRATION == STD_ON)
    P2FUNC(Can_IdType, TYPEDEF,  getLowerCanId_t)(void);
    P2FUNC(Can_IdType, TYPEDEF,  getUpperCanId_t)(void);
    #else
    Can_IdType      LowerCanId_t;
    Can_IdType      UpperCanId_t;
    #endif
    PduIdType       PduIdx_t;

}CanIf_RxPduRangeConfigType_tst;
#endif

/*Hrh structure for List type */
#if CANIF_CFG_HRH_LIST_ENABLED == STD_ON
typedef struct
{
    #if (CANIF_RB_CALIBRATION == STD_ON)
    P2FUNC(Can_IdType, TYPEDEF,  getCanIdLst_t)(void);
    #else
    Can_IdType      CanId_t;
    #endif
    PduIdType       PduIdx_t;

}CanIf_RxPduListConfigType_tst;
#endif
//An array for above structure will be generated(in exsisting desing in CanIf_Cfg_Internal.h).
// extern CONST(CanIf_RxPduConfigType, CANIF_CONST) CanIf_RxPduConfig_0[];
//the above array should have Rx Pdu information sorted as per PduId generated by CanIf.


/**
 * @ingroup CANIF_H
 *
 * Structure type for Tx Callback function  \n
 * typedef struct                           \n
 * {                                        \n
 *   Name of Transmit confirmation services to target upper layers - User specific modules \n
 *   P2FUNC(void, CANIF_APPL_CODE, UserTxConfirmation)(PduIdType);
 * } CanIf_TxCbk_Prototype;                 \n
 */
typedef struct
{
    P2FUNC(void, CANIF_APPL_CODE, UserTxConfirmation)(PduIdType);
} CanIf_TxCbk_Prototype;


typedef struct{
    
#if(CANIF_PUBLIC_TXBUFFERING ==STD_ON)     //take care this in generation
    uint32* BufferIdPtr;
    uint32  TotalBufferCount;
    uint32* TxPduIdPtr;
    uint32  TotalTxPduCount;
#endif
    uint8   CtrlId;
    uint8   CtrlCanCtrlRef;
    boolean CtrlWakeupSupport;
#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    boolean PnCtrlEn;               /*True, if at least one pdu linked to this controller is a pn filter pdu*/
#endif
}CanIf_Cfg_CtrlConfig_tst;

typedef struct{

    const CanIf_Cfg_CtrlConfig_tst   *CanIf_CtrlConfigPtr;
    Can_HwHandleType              CanObjectId;
    CanIf_Cfg_CanHandleType_ten   CanHandleType;
    

}CanIf_Cfg_HthConfig_tst;


typedef struct{
    const CanIf_Cfg_HthConfig_tst   *CanIf_HthConfigPtr;
#if (CANIF_PUBLIC_TXBUFFERING == STD_ON)
 
    uint8*                      DataBuf;
    CanIf_Cfg_CanIdBuffer_tst*   CanIdBuf;
    uint32                       CanIfBufferId;
    uint8                        CanIfBufferSize;
    uint8 CanIfBufferMaxDataLength;
#endif

}CanIf_Cfg_TxBufferConfig_tst;

typedef struct{
    const CanIf_Cfg_TxBufferConfig_tst   *CanIf_TxBufferConfigPtr;

#if(CANIF_METADATA_SUPPORT == STD_ON)
    uint32                            TxPduCanIdMask;
    uint8                             MetaDataLength;          /*Max value = 4*/          
#endif

    PduIdType                         TxPduId;
    PduIdType                         TxPduTargetPduId;       
    PduIdType                         TxPduType;                  
    CanIf_Cfg_TxPduCanIdType_ten      TxPduCanIdType;
    CanIf_Cfg_UserType_ten            TxPduTxUserUL;
    P2FUNC(void, CANIF_APPL_CODE, UserTxConfirmation)(PduIdType);
    Can_IdType                        TxPduCanId;
#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
    boolean                           TxPduPnFilterPdu;
#endif

#if (CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON)
    boolean                           TxPduReadNotifyStatus;
#endif

#if(CANIF_TRIGGERTRANSMIT_SUPPORT== STD_ON)

    P2FUNC(Std_ReturnType, CANIF_APPL_CODE, UserTriggerTransmit)(PduIdType, PduInfoType*);
    boolean                           TxPduTriggerTransmit;
#endif
/*provides information if data truncation is supported for this pdu*/
boolean TxTruncEnabled_b;
/*Pdulength of Ecuc Pdu referred by this pdu*/
uint8 TxPduLength_u8;
}CanIf_Cfg_TxPduConfig_tst;

/* FC_VariationPoint_START */
 #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
 
 typedef enum {
    CANIF_XCORE_PIPE_RX = 0,
    CANIF_XCORE_PIPE_TX,
    CANIF_XCORE_PIPE_TXCONF
}CanIf_XCore_PipeProcesingType;


/* Holds the configuration information of the Pipe. */
typedef struct {
    uint16 PipeId_u16;
    uint16 SrcCoreId_u16;        /* Pipe Source core ID */
    uint16 DestCoreId_u16;       /* Pipe Destination core ID */
    uint32 FifoRamSizeBytes_u32; /* Size of the associated FIFO in bytes */
    uint32 DestFlags_u32;        /* The properties of the receiver on the destination core: (BIT0, BIT1) = {0x0 = "POLLING"; 0x1 = "Task"; 0x2 = "SWI"} */
    TaskType DestRecvId;       /* TaskID of the receiver on the destination core. */
    void* PipeFifoRam_pv;        /* FIFO for PIPE. It is placed either in shared RAM or in the detstination core local RAM */
    CanIf_XCore_PipeProcesingType PipeType; /*Pipe proccessing type */
} CanIf_XCore_PipeConfigType_st;


typedef struct {
    uint8 CanIf_XCoreCanControllerCoreAffinity[CANIF_XCORE_NUMCANCTRL];                     /* Used to identify the destination core of a specific Can transmit operation based on the Controller id. */
    uint8 CanIf_XCoreUserTypeCoreAffinity[MAX_USER_TYPE];                                              /* Used to identify the destination core of a specific Tx confirmation/Rx indication based on the configured UserType. */
    uint16 CanIf_XCoreTxPipeMatrix[CANIF_XCORE_NUMCORES][CANIF_XCORE_NUMCORES];             /* Used to determine the Tx pipe id based on the src core id and destination core id. */
    uint16 CanIf_XCoreTxConfirmationPipeMatrix[CANIF_XCORE_NUMCORES][CANIF_XCORE_NUMCORES]; /* Used to determine the Tx confirmation pipe id based on the src core id and destination core id. */
    uint16 CanIf_XCoreRxPipeMatrix[CANIF_XCORE_NUMCORES][CANIF_XCORE_NUMCORES];             /* Used to determine the Rx pipe id based on the src core id and destination core id. */
    uint16 NumPipes_u16;                                                                    /* Number of configured pipes */
    CanIf_XCore_PipeConfigType_st PipeConfigs_ast[CANIF_XCORE_MAX_NUM_PIPES];               /* Holds the pipe configurations */
} CanIf_XCore_ConfigType;
#endif
/* FC_VariationPoint_END */

/* Configration type structure */
typedef struct
{
    #if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
    /*these elements should be available if there are any Rx-Pdu's*/
    /* pointer to Hrh global array */
    P2CONST(CanIf_Cfg_Hrhtype_tst,TYPEDEF,CANIF_CFG_CONST)    HrhConfig_pcst;

    /* pointer to Rx pdu global array */
    P2CONST(CanIf_Cfg_RxPduType_tst,TYPEDEF,CANIF_CFG_CONST)  RxPduConfig_pcst;

    /* Number of RxPdu's configured for this variant */
    VAR(PduIdType,TYPEDEF)                                        NumCanRxPduId_t;
    #endif
    /* Number of Controller's configured for this variant */
    VAR(uint8,TYPEDEF)                                        NumCanCtrl_u8;

    #if(CANIF_CFG_RX_FEATURE_ENABLED== STD_ON)
    /*Number of CDD pdu's*/
    VAR(PduIdType,TYPEDEF)                                    NumCddRxPdus_t;
    
    /*Pointer to the range container */
    #if (CANIF_CFG_RX_RANGE_CONFIG == STD_ON)
    P2CONST(CanIf_RxPduRangeConfigType_tst,TYPEDEF,CANIF_CFG_CONST)RangeCfg_tpst;
    #endif
        /*Pointer to the List container */
    #if (CANIF_CFG_HRH_LIST_ENABLED == STD_ON)
    P2CONST(CanIf_RxPduListConfigType_tst,TYPEDEF,CANIF_CFG_CONST)ListCfg_tpst;
    #endif
    const uint16                            *RxPduIdTable_Ptr;
    const uint16                            *HrhPduIdTable_Ptr;
    #endif
    /*Index of the selected variant*/
    VAR(uint8,TYPEDEF)CfgSetIndex_u8;
     #if(CANIF_CFG_TX_FEATURE_ENABLED== STD_ON)
    const CanIf_Cfg_TxPduConfig_tst      *CanIf_TxPduConfigPtr;
    const CanIf_Cfg_TxBufferConfig_tst   *CanIf_TxBufferConfigPtr;
    #endif
    const CanIf_Cfg_CtrlConfig_tst       *CanIf_CtrlConfigPtr;
    uint32                            NumOfTxPdus;
    uint32                            NumOfTxBuffers;
    const uint16                            *TxPduIdTable_Ptr;
    const uint8                             *CtrlIdTable_Ptr;
    #if CANIF_CFG_USER_RX_ENABLED == STD_ON
    const CanIf_RxUSERCbk_Prototype                             *RxUSER_Ptr;
    #endif
    #if CANIF_CFG_RX_FEATURE_ENABLED == STD_ON
   const CanIf_RxCbk_Prototype                             *RxAutosarUL_Ptr;
    #endif
    #if(CANIF_CANNM_TXID_FILTER != STD_OFF)
    P2CONST(Can_IdType,TYPEDEF,CANIF_CFG_CONST)         NmtxIdPtr;
    #endif
    /* FC_VariationPoint_START */
    #if (CANIF_XCORE_CFG_ENABLED != STD_OFF)
    P2CONST(CanIf_XCore_ConfigType,TYPEDEF,CANIF_CFG_CONST)         CanIf_XCoreConfigPtr;
    #endif
    /* FC_VariationPoint_END */
}CanIf_ConfigType;

/*************************************************************/


#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
extern CONST(CanIf_ConfigType, CANIF_CONST) CanIf_Config;
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


/* FC_VariationPoint_START */
/* FC_VariationPoint_END */

/*TxBufferConfig custId*/




#define CanIf_Buffer_CustId_Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_1       0u   


#define CanIf_Buffer_CustId_Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_2       1u   


#define CanIf_Buffer_CustId_Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_3       2u   
 
 










/*HthConfig custId */





#define CanIf_Hth_CustId_Can_Network_CANNODE_0_Tx_Std_MailBox_1   0u  


#define CanIf_Hth_CustId_Can_Network_CANNODE_0_Tx_Std_MailBox_2   1u  


#define CanIf_Hth_CustId_Can_Network_CANNODE_0_Tx_Std_MailBox_3   2u  
 










/*CtrlConfig custId */





#define CanIf_Ctrl_CustId_Can_Network_CANNODE_0       0u
 










#endif /* CANIF_CFG_H */
