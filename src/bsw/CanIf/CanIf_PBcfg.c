



/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanIf / AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.0.2.1                ECU Parameter Definition Version
 *

 </VersionHead>*/


/******************************************************************************/
/*                                  Include Section                                */
/******************************************************************************/

/* CanIf Private header */
#include "CanIf_Prv.h"











/*
 ******************************************************************************
 * Variables
 ******************************************************************************
 */









/*======================================================================================================================
 *                                VARIANT:  PRE_COMPILE
 *======================================================================================================================
*/







/*====================================================================================================================*/
/*
 *                                  CANIF CONTROLLER CONFIG STRUCTURE             
 *                                     
 * Structure contains following members:
 *{
 *#if(CANIF_PUBLIC_TXBUFFERING ==STD_ON) 
 *  (0) Pointer to CanIf_BufferId_Ctrl array,
 *  (1) Size of CanIf_BufferId_Ctrl array,
 *  (2) Pointer to CanIf_CanIf_TxPdus_Ctrl array,
 *  (3) Size of CanIf_CanIf_TxPdus_Ctrl array,
 *#endif
 *  (4) CanIfCtrlId,
 *  (5) CanCtrlId,
 *  (6) WakeupSupport,
 *#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
 *  (7) PnSupport            
 *#endif
 *}
*/


#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

static CONST(CanIf_Cfg_CtrlConfig_tst, CANIF_CONST) CanIf_CtrlGen_a[]=
{

/*Can_Network_CANNODE_0*/
{ /*(4)*/0,   /*(5)*/CanConf_CanController_Can_Network_CANNODE_0,   /*(6)*/FALSE  }
}; 




#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

/*                                  CANIF HTH CONFIG STRUCTURE               
 * Structure contains following members:                                         
 *{
 *  (0)Pointer to Controller config structure,
 *  (1)CanObjectId,
 *  (2)CanHandleType
 *  (3)Pointer to HthTxPduMap array
 *}
 */
 

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

static CONST(CanIf_Cfg_HthConfig_tst, CANIF_CONST) CanIf_HthGen_a[]=
{

/*Can_Network_CANNODE_0_Tx_Std_MailBox_1*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_Can_Network_CANNODE_0], /*(1)*/ CanConf_CanHardwareObject_Can_Network_CANNODE_0_Tx_Std_MailBox_1, /*(2)*/CANIF_FULL}

,
/*Can_Network_CANNODE_0_Tx_Std_MailBox_2*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_Can_Network_CANNODE_0], /*(1)*/ CanConf_CanHardwareObject_Can_Network_CANNODE_0_Tx_Std_MailBox_2, /*(2)*/CANIF_FULL}

,
/*Can_Network_CANNODE_0_Tx_Std_MailBox_3*/
{/*(0)*/ &CanIf_CtrlGen_a[CanIf_Ctrl_CustId_Can_Network_CANNODE_0], /*(1)*/ CanConf_CanHardwareObject_Can_Network_CANNODE_0_Tx_Std_MailBox_3, /*(2)*/CANIF_FULL}


};


#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

/*                                  CANIF TXBUFFER CONFIG STRUCTURE       

 * Structure contains following members:
 *{
 * (0)Pointer to hth config structure
 *#if (CANIF_PUBLIC_TXBUFFERING == STD_ON)
 * (1)Pointer to SDU Buffer
 * (2)Pointer to request buffer
 * (3)BufferId
 * (4)CanIfBufferSize
 * (5)MaxPduLength 
 *#endif
 *}
 */     
 
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
                                            
static CONST(CanIf_Cfg_TxBufferConfig_tst, CANIF_CONST) CanIf_TxBufferGen_a[]=
{


/*Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_1*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_Can_Network_CANNODE_0_Tx_Std_MailBox_1]}
,

/*Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_2*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_Can_Network_CANNODE_0_Tx_Std_MailBox_2]}
,

/*Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_3*/
{/*(0)*/&CanIf_HthGen_a[CanIf_Hth_CustId_Can_Network_CANNODE_0_Tx_Std_MailBox_3]}
};

#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

/*                                  CANIF TXPDU CONFIG STRUCTURE       
 * Structure contains following members:
 *{
 * (0)Pointer to TxBuffer config structure
 *#if(CANIF_METADATA_SUPPORT == STD_ON)
 * (1)TxPduCanIdMask
 * (2)MetaDataLength          
 *#endif
 * (3)TxPduId
 * (4)TxPduTargetPduId       
 * (5)TxPduType                
 * (6)TxPduCanIdType
 * (7)TxPduTxUserUL
 * (8)Function pointer to TxPduTxUserULName
 * (9)TxPduCanId
 *
 *#if (CANIF_PUBLIC_PN_SUPPORT == STD_ON)
 * (10)TxPduPnFilterPdu
 *#endif
 *
 *#if (CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON)
 * (11)TxPduReadNotifyStatus
 *#endif
 *
 *#if(CANIF_TRIGGERTRANSMIT_SUPPORT== STD_ON)
 * (12)Function pointer to UserTriggerTransmitName
 * (13)TxPduTriggerTransmit
 *#endif
 *(14)TxTruncEnabled
 *(15)TxPduLength
 *}
 */  

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
                              
 
static CONST(CanIf_Cfg_TxPduConfig_tst, CANIF_CONST) CanIf_TxPduGen_a[]=
{


/*BMS_20_Can_Network_CANNODE_0*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_2],    /*TxPduId*/CanIfConf_CanIfTxPduCfg_BMS_20_Can_Network_CANNODE_0, /*TxPduTargetPduId*/ 0 , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/PDUR,      /*UserTxConfirmation*/&PduR_CanIfTxConfirmation, /*TxPduCanId*/ 0x5C0   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ FALSE,/*TxPduLength*/8u}
,

/*BMS_Vol_8_Can_Network_CANNODE_0*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_1],    /*TxPduId*/CanIfConf_CanIfTxPduCfg_BMS_Vol_8_Can_Network_CANNODE_0, /*TxPduTargetPduId*/ 1 , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/PDUR,      /*UserTxConfirmation*/&PduR_CanIfTxConfirmation, /*TxPduCanId*/ 0x547   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ FALSE,/*TxPduLength*/8u}
,

/*NMm_BMS_Can_Network_CANNODE_0*/

{ /*CanIf_TxBufferConfigPtr*/&CanIf_TxBufferGen_a[CanIf_Buffer_CustId_Buffer_Can_Network_CANNODE_0_Tx_Std_MailBox_3],    /*TxPduId*/CanIfConf_CanIfTxPduCfg_NMm_BMS_Can_Network_CANNODE_0, /*TxPduTargetPduId*/ 0 , 
/*TxPduType*/CANIF_STATIC, /*TxPduCanIdType*/STANDARD_CAN,   /*TxPduTxUserUL*/CAN_NM,      /*UserTxConfirmation*/&CanNm_TxConfirmation, /*TxPduCanId*/ 0x60F   , /*TxPduReadNotifyStatus*/ FALSE    ,/*TxTruncEnabled*/ FALSE,/*TxPduLength*/8u}
};


#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

/*=====================================================================================================================
 *=====================================================================================================================
 *=====================================================================================================================
 */







#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

static CONST(CanIf_Cfg_Hrhtype_tst, CANIF_CONST) CanIf_Prv_HrhConfig_tacst[5] =
{
          /*HrhInfo_e,    PduIdx_t,  NumRxPdus_u32,  HrhRangeMask_b,  ControllerId_qu8 */
    
/*Can_Network_CANNODE_0_Rx_Std_MailBox_1*/
    {CANIF_PRV_FULL_E,       0u,             1u,            FALSE,           0u           },
/*Can_Network_CANNODE_0_Rx_Std_MailBox_2*/
    {CANIF_PRV_FULL_E,       1u,             1u,            FALSE,           0u           },
/*Can_Network_CANNODE_0_Rx_Std_MailBox_3*/
    {CANIF_PRV_FULL_E,       2u,             1u,            FALSE,           0u           },
/*Can_Network_CANNODE_0_Rx_Std_MailBox_4*/
    {CANIF_PRV_FULL_E,       3u,             1u,            FALSE,           0u           },
/*Can_Network_CANNODE_0_Rx_Std_MailBox_5*/
    {CANIF_PRV_FULL_E,       4u,             1u,            FALSE,           0u           }
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"








#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
static CONST(CanIf_Cfg_RxPduType_tst, CANIF_CONST) CanIf_Prv_RxPduConfig_tacst[5]=
{   
   /*RxPduReadNotifyStatus_b       IndexForUL_u8   CanIdtype_u8    RxPduDlc_u8      RxPduCanId      Hrhref_t    RxPduTargetId_t 	*/
    
    /*TMR_3_Can_Network_CANNODE_0*/
    {
        CANIF_READ_NOTIFSTATUS,     6u,           0x20u,            8u,         808u,       0,         PduRConf_PduRSrcPdu_TMR_3_Can_Network_CANNODE_0_CanIf2PduR,            },
    /*NMm_HCU_Can_Network_CANNODE_0*/
    {
        CANIF_READ_NOTIFSTATUS,     1u,           0x20u,            8u,         1550u,       1,         0,            },
    /*NMm_CDU_Can_Network_CANNODE_0*/
    {
        CANIF_READ_NOTIFSTATUS,     1u,           0x20u,            8u,         1583u,       2,         0,            },
    /*NMm_MCUR_Can_Network_CANNODE_0*/
    {
        CANIF_READ_NOTIFSTATUS,     1u,           0x20u,            8u,         1582u,       3,         0,            },
    /*OBC_1_Can_Network_CANNODE_0*/
    {
        CANIF_READ_NOTIFSTATUS,     6u,           0x20u,            8u,         1061u,       4,         PduRConf_PduRSrcPdu_OBC_1_Can_Network_CANNODE_0_CanIf2PduR,            }
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
























/* Array for mapping Hoh Id(CanObjectId) and Hrh */
static CONST(uint16, CANIF_CONST) CanIf_CFG_HrhIdMapping_au16[] =
{/*Can_Network_CANNODE_0_Rx_Std_MailBox_1*/
0,
/*Can_Network_CANNODE_0_Rx_Std_MailBox_2*/
1,
/*Can_Network_CANNODE_0_Rx_Std_MailBox_3*/
2,
/*Can_Network_CANNODE_0_Rx_Std_MailBox_4*/
3,
/*Can_Network_CANNODE_0_Rx_Std_MailBox_5*/
4,
/*Can_Network_CANNODE_0_Tx_Std_MailBox_1*/
INVALID_ID,
/*Can_Network_CANNODE_0_Tx_Std_MailBox_2*/
INVALID_ID,
/*Can_Network_CANNODE_0_Tx_Std_MailBox_3*/
INVALID_ID};



/* Array for mapping CanIfRxpduId accross the variant */
static CONST(uint16, CANIF_CONST) CanIf_CFG_RxPduIdMapping_au16[] =
{
/*TMR_3_Can_Network_CANNODE_0*/
0,
/*NMm_HCU_Can_Network_CANNODE_0*/
1,
/*NMm_CDU_Can_Network_CANNODE_0*/
2,
/*NMm_MCUR_Can_Network_CANNODE_0*/
3,
/*OBC_1_Can_Network_CANNODE_0*/
4};









/* array of function pointer which provies Callback name to the UL*/
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
static CONST(CanIf_RxCbk_Prototype, CANIF_CONST) CanIf_Prv_ULName_ta__fct[] =
{
    {NULL_PTR},
    {&CanNm_RxIndication},
    {NULL_PTR},
    {NULL_PTR},
    {NULL_PTR},
    {NULL_PTR},
    {&PduR_CanIfRxIndication},
    {NULL_PTR},
    
};
#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"










/* CANIF callback configuration */

#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_CallbackFuncType, CANIF_CONST) CanIf_Callback =
{

   
    /* User_ControllerBusOff */
    &CanSM_ControllerBusOff,

    
    /* User_ControllerBusOff */
    &CanSM_ControllerModeIndication,

    /*User_ControllerErrorPassive*/

    
    
};

#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"










/*This mapping table is generated for finding invalid TxPduIs passed via CanIf APIs in Post-Build.
 *Size of the array is total number of Tx PDUs across the variants. Each element is the index of Tx Pdu config structure.
 * If a TxPdu is not present in this variant, an invalid value 0xFFFF is generated in the particular position.
 */
 
 #define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



static CONST(uint16, CANIF_CONST) CanIf_TxPduId_MappingTable[] = 
{
      
0,	      
1,	      
2 
};  







#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"


/*This mapping table is generated for finding invalid CtrlIds passed via CanIf APIs in Post-Build.
 *Size of the array is total number of controllers across the variants. Each element is the index of Controller config structure.
 * If a CtrlId is not present in this variant, an invalid value 0xFF is generated in the particular position.
 */
 
#define CANIF_START_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"



static CONST(uint8, CANIF_CONST) CanIf_CtrlId_MappingTable[] = 
{

0 
};  







#define CANIF_STOP_SEC_CONST_UNSPECIFIED
#include "CanIf_MemMap.h"

/* FC_VariationPoint_START */

/* FC_VariationPoint_END */


/*Configuration structure for __KW_COMMON*/
#define CANIF_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanIf_MemMap.h"
CONST(CanIf_ConfigType, CANIF_CONST) CanIf_Config =
{
    
    /* HrhConfig_pcst */
    CanIf_Prv_HrhConfig_tacst,
    /* RxPduConfig_pcst */
    CanIf_Prv_RxPduConfig_tacst,
   /* NumCanRxPduId_t */
   5u,
   /*NumCanCtrl_u8*/
   1,
   /*NumCddRxPdus_t*/
   
   0,
   /*RxPduIdTable_Ptr*/
   &CanIf_CFG_RxPduIdMapping_au16[0],
   /*HrhPduIdTable_Ptr*/
   &CanIf_CFG_HrhIdMapping_au16[0],
   /*CfgSetIndex_u8*/
   0,
   &CanIf_TxPduGen_a[0u],             /*TxPdu Config*/
   &CanIf_TxBufferGen_a[0u],          /*TxBuffer Config*/
   &CanIf_CtrlGen_a[0u],              /*Ctrl Config*/
   3,      /*CANIF TOTAL TXPDUS for this variant*/
   3,        /*CANIF_TOTAL_TXBUFFERS for this variant*/
   &CanIf_TxPduId_MappingTable[0],        /*TxPdu Config index table*/
   &CanIf_CtrlId_MappingTable[0],
   /*RxAutosarUL_Ptr*/
   &CanIf_Prv_ULName_ta__fct[0]
};
#define CANIF_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanIf_MemMap.h"








#if(CANIF_VARIANT_INFO == 3)
/* Generate an instance of structure Std_VersionInfoType in CanIf_PBcfg_c.xpt*/
#define CANIF_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanIf_MemMap.h"
static CONST(Std_VersionInfoType, CANIF_CONST)  CanIf_VersionInfo =
{
    6,    /* Vendor Id */
    60,    /* Module Id */
    6,    /* Sw Major Version */
    0,    /* Sw Minor Version */
    0     /* Sw Patch Version */
};
#define CANIF_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif

