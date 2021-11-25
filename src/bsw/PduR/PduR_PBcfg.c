

 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: PduR  / AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/


#include "PduR_PBcfg.h"
#include "PduR_UpIf.h"

#include "PduR_LoIfTT.h"

#include "PduR_LoIf.h"


#include "PduR_Mc.h"
#include "PduR_Gw.h"


#include "PduR_Gw_Cfg.h"
/* Generating PbCfg_c::PduR_UpIfToLo_PBcfg_c::upIf_To_Lo */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
#if defined(PDUR_CONFIG_SINGLE_IFTX_LO)
#define PduR_comToLo   NULL_PTR
#else
static CONST(PduR_RT_UpToLo,PDUR_CONST ) PduR_comToLo[] = {
    { CanIfConf_CanIfTxPduCfg_BMS_Vol_5_CANNODE_0 , (PduR_loTransmitFP)PduR_RF_CanIf_Transmit, (PduR_loCancelTransmitFP)PduR_IH_CancelTransmit }    /*BMS_Vol_5_CANNODE_0_Com2PduR*/

};
#endif /* PDUR_CONFIG_SINGLE_IFTX_LO */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"

/* Generating PbCfg_c::PduR_UpTpToLo_PBcfg_c::upTp_To_Lo */

/* Generating PbCfg_c::PduR_LoIfRxToUp_PBcfg_c::loIfRx_To_Up */
    
        

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
#if defined(PDUR_CONFIG_SINGLE_IFRX)
#define PduR_CanIfRxToUp   NULL_PTR
#else
static CONST( PduR_RT_LoIfRxToUp, PDUR_CONST ) PduR_CanIfRxToUp[] = {        
    { ComConf_ComIPdu_OBC_4_CANNODE_0, (PduR_upIfRxIndicationFP) PduR_RF_Com_RxIndication },   /*OBC_4_CANNODE_0_CanIf2PduR*/        
    { ComConf_ComIPdu_TMR_3_CANNODE_0, (PduR_upIfRxIndicationFP) PduR_RF_Com_RxIndication }    /*TMR_3_CANNODE_0_CanIf2PduR*/
 };
#endif  /* PDUR_CONFIG_SINGLE_IFRX */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"


        


        
    
/* Generating PbCfg_c::PduR_LoIfDTxToUp_PBcfg_c::loIf_DTxToUp */
    
        

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
#if defined(PDUR_CONFIG_SINGLE_IFTX_UP )
#define PduR_CanIfTxToUp NULL_PTR
#else

static CONST(PduR_RT_LoIfTxToUp, PDUR_CONST ) PduR_CanIfTxToUp [] = { 
{ ComConf_ComIPdu_BMS_Vol_5_CANNODE_0 , (PduR_upIfTxConfirmationFP) PduR_RF_Com_TxConfirmation}     /* Index: 0  SrcPdu: BMS_Vol_5_CANNODE_0_Com2PduR  DestPdu: BMS_Vol_5_CANNODE_0_PduR2CanIf*/
};
#endif  /* PDUR_CONFIG_SINGLE_IFTX_UP */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"

        
    
/* Generating PbCfg_c::PduR_LoIfTTxToUp_PBcfg_c::loIf_TTxToUp */
    

        
    
/* Generating PbCfg_c::PduR_LoTpRxToUp_PBcfg_c::loTpRx_To_Up */

/* Generating PbCfg_c::PduR_LoTpTxToUp_PBcfg_c::loTpTx_To_Up */
    
/* Generating PbCfg_c::PduR_Mc_ComToLo_PBcfg_c::mcComToLow */
/* Generating PbCfg_c::PduR_Mc_SecOCToLo_PBcfg_c::MC_SecOCUp_Lo */
/* Generating PbCfg_c::PduR_Mc_DcmToLo_PBcfg_c::mcDcmToLow */
/* Generating PbCfg_c::PduR_Mc_IpduMToLo_PBcfg_c::MC_IpduMUp_Lo */
/* Generating PbCfg_c::PduR_Mc_TpTxToUp_PBcfg_c::xpandMcTpTxToUp */
/* Generating PbCfg_c::PduR_Mc_GwToLo_PBcfg_c::DisplayPduR_mcGwToLo */
/* Generating PbCfg_c::PduR_GwIfTx_PBcfg_c::display_GwIfTx */
/* Generating PbCfg_c::PduR_GwIf_PBcfg_c::display_GwIf */
/* Generating PbCfg_c::PduR_Gw_IfBuf_PBcfg_c::PduR_gw_Buf_If_structure */
/* Generating PbCfg_c::PduR_Rpg_PBcfg_c::display_PduR_RPG*/


#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)



/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"

static CONST (PduR_RPGInfoType, PDUR_CONST) PduR_RPGInfo[] = {

    {
     NULL_PTR,
     NULL_PTR,
     PDUR_RPGID_NULL,
     0,
     0
    },   /* PDUR_RPGID_NULL */

    
};

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_8
#include "PduR_MemMap.h"
/* Routing enable disbale flag to control routing. */
CONST(boolean, PDUR_CONST) PduR_RPG_EnRouting[] =
{
  TRUE, /*PDUR_RPGID_NULL*/
  
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_8
#include "PduR_MemMap.h"


#endif /* #if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0) */


/* Generating PbCfg_c::PduR_Gw_TpBuf_PBcfg_c::PduR_gw_Buf_TP_structure*/

    /* Generating PbCfg_c::PduR_GwTp_PBcfg_c::display_GwTp */
    /* Generating PbCfg_c::PduR_PbConfigType_PBcfg_c::PduR_BswLoCfg */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
static CONST( PduR_LoIfDConfig, PDUR_CONST ) PduR_LoIfDCfg[] = {
  {
    PduR_CanIfRxToUp,       /* CanIf */
    PduR_CanIfTxToUp,       /* CanIf */
    2,        /* CanIf RxToUp NrEntries*/
    1      /* CanIf TxToUp NrEntries*/
    }
  };

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
static CONST( PduR_LoIfTTConfig, PDUR_CONST ) PduR_LoIfTTCfg[] = {
  {
    NULL_PTR,       /* CanNm */
    NULL_PTR,       /* CanNm */
    0,  /* CanNm */
    0   /* CanNm */
    }
  };

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
/* Generating PbCfg_c::PduR_PbConfigType_PBcfg_c::PduR_BswUpCfg */


/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
static CONST( PduR_UpConfig, PDUR_CONST ) PduR_UpIfCfg[] = {
  {
    PduR_comToLo,     /* Com */
    NULL_PTR,       /* mcComToLo */
    1      /* Com */
    }
  };

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
/* Generating PbCfg_c::PduR_Cdd_PBcfg_c::PduR_CddCfg */
/* Generating PbCfg_c::PduR_PbConfigType_PBcfg_c::PduR_BswUpToLoRxCfg */


    


/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"

CONST( PduR_RPTablesType, PDUR_CONST ) PduR_RoutingPathTables = {
    (P2CONST( PduR_loTransmitFuncType, AUTOMATIC, PDUR_CONST ))             PduR_loTransmitTable,
    (P2CONST( PduR_loCancelReceiveFuncType, AUTOMATIC,  PDUR_CONST))        PduR_loCancelRxTable,
    (P2CONST( PduR_loCancelTransmitFuncType, AUTOMATIC, PDUR_CONST ))       PduR_loCancelTransmitTable,
    (P2CONST( PduR_upIfRxIndicationFuncType, AUTOMATIC, PDUR_CONST ))       PduR_upIfRxIndicationTable,
    (P2CONST( PduR_upIfTxConfirmationFuncType, AUTOMATIC, PDUR_CONST ))     PduR_upIfTxConfirmationTable,
    (P2CONST( PduR_upTpCopyRxDataFuncType, AUTOMATIC, PDUR_CONST ))         PduR_upTpCopyRxDataTable,
    (P2CONST( PduR_upTpStartOfReceptionFuncType, AUTOMATIC, PDUR_CONST ))   PduR_upTpStartOfReceptionTable,
    (P2CONST( PduR_upTpRxIndicationFuncType, AUTOMATIC, PDUR_CONST ))       PduR_upTpRxIndicationTable,
    (P2CONST( PduR_upTpCopyTxDataFuncType, AUTOMATIC, PDUR_CONST ))         PduR_upTpCopyTxDataTable,
    (P2CONST( PduR_upTpTxConfirmationFuncType, AUTOMATIC, PDUR_CONST ))     PduR_upTpTxConfirmationTable,
    (P2CONST( PduR_upIfTriggerTxFuncType, AUTOMATIC, PDUR_CONST ))          PduR_upIfTriggerTxTable

};


/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"

    
/*
    These structures are generated by the code generator tool. Respective module's function names are generated
    only if it is present in the PduR_PbCfg.c file in any one of the entries.
*/


/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"


CONST(PduR_loTransmitFuncType, PDUR_CONST) PduR_loTransmitTable[] =
{
    {&PduR_RF_CanIf_Transmit_Func}
};




CONST(PduR_loCancelReceiveFuncType, PDUR_CONST) PduR_loCancelRxTable[] =
{
    {NULL_PTR}
};




CONST(PduR_loCancelTransmitFuncType, PDUR_CONST) PduR_loCancelTransmitTable[] =
{
    {&PduR_IH_CancelTransmit_Func}
};



CONST(PduR_upIfRxIndicationFuncType, PDUR_CONST) PduR_upIfRxIndicationTable[] =
{
    {&PduR_RF_Com_RxIndication_Func}
};



CONST(PduR_upIfTxConfirmationFuncType, PDUR_CONST) PduR_upIfTxConfirmationTable[] =
{
    {&PduR_RF_Com_TxConfirmation_Func}
};



CONST(PduR_upIfTriggerTxFuncType, PDUR_CONST) PduR_upIfTriggerTxTable[] =
{
    {NULL_PTR}
};



CONST(PduR_upTpCopyRxDataFuncType, PDUR_CONST) PduR_upTpCopyRxDataTable[] =
{
    {NULL_PTR}
};



CONST(PduR_upTpStartOfReceptionFuncType, PDUR_CONST) PduR_upTpStartOfReceptionTable[] =
{
    {NULL_PTR}
};



CONST(PduR_upTpRxIndicationFuncType, PDUR_CONST) PduR_upTpRxIndicationTable[] =
{
    {NULL_PTR}
};




CONST(PduR_upTpCopyTxDataFuncType, PDUR_CONST) PduR_upTpCopyTxDataTable[] =
{
    {NULL_PTR}
};



CONST(PduR_upTpTxConfirmationFuncType, PDUR_CONST) PduR_upTpTxConfirmationTable[] =
{
    {NULL_PTR}
};






/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
/* Generating PbCfg_c::PduR_PbConfigType_PBcfg_c::pdur_PBConfigType */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
CONST( PduR_PBConfigType, PDUR_CONST ) PduR_GlobalPBConfig = {
  (P2CONST( PduR_CddConfig, AUTOMATIC, PDUR_CONST )) NULL_PTR,                /* PduR_CddCfg */
  (P2CONST( PduR_LoTpConfig, AUTOMATIC, PDUR_CONST )) NULL_PTR,               /* PduR_LoTpCfg */
     (P2CONST( PduR_LoIfDConfig, AUTOMATIC, PDUR_CONST )) PduR_LoIfDCfg,         /* Pointer to Direct lowerlayer If config structure */
     (P2CONST( PduR_LoIfTTConfig, AUTOMATIC, PDUR_CONST )) PduR_LoIfTTCfg,       /* Pointer to TT lowerlayer If config structure */
     (P2CONST( PduR_UpConfig, AUTOMATIC, PDUR_CONST )) PduR_UpIfCfg,             /* Pointer to Upperlayer If config structure */
  (P2CONST( PduR_UpConfig, AUTOMATIC, PDUR_CONST )) NULL_PTR,                 /* PduR_UpTpCfg */
     (P2CONST( PduR_MT_UpToLo, AUTOMATIC, PDUR_CONST )) NULL_PTR,                /* mcGwToLo */
     (P2CONST( PduR_MT_LoIfTxToUp, AUTOMATIC, PDUR_CONST )) NULL_PTR,            /* McIfRx */
     (P2CONST( PduR_MT_LoTpTxToUp, AUTOMATIC, PDUR_CONST )) NULL_PTR,            /* McTpTx */
     (P2VAR(   PduR_MS_LoTpTxToUp, AUTOMATIC, PDUR_VAR )) NULL_PTR,              /* PduR_msTpTxToUp*/
     (P2CONST( PduR_GT_IfTx, AUTOMATIC, PDUR_CONST )) NULL_PTR,                  /* gwIfTx */
     (P2CONST( PduR_GT_If, AUTOMATIC, PDUR_CONST )) NULL_PTR,                    /* gwIf        */
     (P2CONST( PduR_GT_Tp, AUTOMATIC, PDUR_CONST )) NULL_PTR,                    /* GwTp */
     (P2CONST( PduR_RPTablesType, AUTOMATIC, PDUR_CONST )) &PduR_RoutingPathTables, /* PduR_RoutingPathTables */
#if defined(PDUR_TPGATEWAY_SUPPORT) && (PDUR_TPGATEWAY_SUPPORT != STD_OFF)
    (P2CONST(PduR_GwTp_SessionListType, AUTOMATIC, PDUR_CONST )) NULL_PTR, /*PduR_TpSession_Dynamic*/
#endif
#if defined(PDUR_MULTICAST_TO_IF_SUPPORT) && (PDUR_MULTICAST_TO_IF_SUPPORT != 0)
     (P2CONST(PduR_UpIfTxConf_Config, AUTOMATIC, PDUR_CONST)) PduR_UpIfTxConf_ConfigList,
#endif
#if defined(PDUR_MODE_DEPENDENT_ROUTING) && (PDUR_MODE_DEPENDENT_ROUTING != 0)
     (P2CONST (PduR_RPGInfoType, AUTOMATIC, PDUR_CONST))        PduR_RPGInfo,        /* RoutingPathGroup ConfigInfo */
     (P2CONST (boolean, AUTOMATIC, PDUR_CONST))   PduR_RPG_EnRouting,  /* RoutingControl StatusInfo */
     (P2VAR  (boolean , TYPEDEF , PDUR_VAR))  PduR_RPG_Status,        /*RAM status for each RPG*/
     (PduR_RoutingPathGroupIdType)                              0,        /* Number of RPGs.*/
#endif
     (P2CONST (PduR_UpTpToLoTpRxConfig, TYPEDEF, PDUR_CONST)) NULL_PTR,          /* Pointer to PduR_UpTpToLoTpRxConfig structure for supporting Cancel Receive API */
0,                  /* PDUR_CONFIGURATION_ID */
0, /*Total no of Gw Tp Routing Path*/
0, /*Total no of Gw If Routing path*/
     (PduIdType) 0       /* McTpTx */
};

/* ------------------------------------------------------------------------ */
/* Begin section for constants */

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"







#define PDUR_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"
CONST(PduR_ConfigType, PDUR_CONST) PduR_Config = {
    NULL_PTR,    /* Void pointer initialised with null pointer as PduR_Config will not be used in case of PDUR_VARIANT_PRE_COMPILE */
    NULL_PTR
};

#define PDUR_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "PduR_MemMap.h"


