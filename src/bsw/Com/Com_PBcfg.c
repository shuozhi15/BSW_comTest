



 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/



/*
 * If COM_DontUseExternalSymbolicNames is defined before including Com_Cfg.h file, then external symbolic names will
 * not be visible.
 * Com_PBcfg.c file should not use external symbolic names.
 * This mechanism is used to prevent the accidental usage of external symbolic names in this file
 * This file should use only internal symbolic name defined in  Com_PBcfg_InternalId.h
 */
#define COM_DontUseExternalSymbolicNames
#include "Com_Prv.h"
#include "Com_Cbk.h"
#include "PduR_Com.h"
#include "Com_PBcfg.h"

/*
 * The file Com_PBcfg_InternalId.h defines internal symbolic names
 * These names should be used in the tables generated in this file
 * Regular symbolic names should not be used here
 */
#include "Com_PBcfg_InternalId.h"


/* START: Tx Signal Details  */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_Prv_xTxSigCfg_tst ,COM_CONST) Com_Prv_xTxSigCfg_acst[COM_NUM_TX_SIGNALS] =
{
        
    {  /* BMS_BattTempSensorOrder40 */
#ifdef COM_TxInvalid
        (uint32)(0uL),                                                /*DataInvalid_Val*/
#endif
        (uint32)(0uL),                                                /* Init_Val*/
        
        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = COM_NOTCONFIGURED
            DataInvalidType : 1;    = COM_FALSE
            TimeOutEnabled  : 1;    = COM_FALSE
        }Com_TxSigPropType;    */
        0x54,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        56,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0xff,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        0xff,                                              /*OldVal_Index*/
#endif

        (Com_IpduIdType)ComConf_ComIPdu_Internal_BMS_Tem_5_Can_Network_CANNODE_0,             /*Ipdu Reference*/
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* BMS_BattTempSensorOrder39 */
#ifdef COM_TxInvalid
        (uint32)(0uL),                                                /*DataInvalid_Val*/
#endif
        (uint32)(0uL),                                                /* Init_Val*/
        
        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = COM_NOTCONFIGURED
            DataInvalidType : 1;    = COM_FALSE
            TimeOutEnabled  : 1;    = COM_FALSE
        }Com_TxSigPropType;    */
        0x54,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        48,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0xff,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        0xff,                                              /*OldVal_Index*/
#endif

        (Com_IpduIdType)ComConf_ComIPdu_Internal_BMS_Tem_5_Can_Network_CANNODE_0,             /*Ipdu Reference*/
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* BMS_BattTempSensorOrder38 */
#ifdef COM_TxInvalid
        (uint32)(0uL),                                                /*DataInvalid_Val*/
#endif
        (uint32)(0uL),                                                /* Init_Val*/
        
        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = COM_NOTCONFIGURED
            DataInvalidType : 1;    = COM_FALSE
            TimeOutEnabled  : 1;    = COM_FALSE
        }Com_TxSigPropType;    */
        0x54,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        40,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0xff,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        0xff,                                              /*OldVal_Index*/
#endif

        (Com_IpduIdType)ComConf_ComIPdu_Internal_BMS_Tem_5_Can_Network_CANNODE_0,             /*Ipdu Reference*/
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* BMS_BattTempSensorOrder37 */
#ifdef COM_TxInvalid
        (uint32)(0uL),                                                /*DataInvalid_Val*/
#endif
        (uint32)(0uL),                                                /* Init_Val*/
        
        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = COM_NOTCONFIGURED
            DataInvalidType : 1;    = COM_FALSE
            TimeOutEnabled  : 1;    = COM_FALSE
        }Com_TxSigPropType;    */
        0x54,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        32,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0xff,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        0xff,                                              /*OldVal_Index*/
#endif

        (Com_IpduIdType)ComConf_ComIPdu_Internal_BMS_Tem_5_Can_Network_CANNODE_0,             /*Ipdu Reference*/
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* BMS_BattTempSensorOrder36 */
#ifdef COM_TxInvalid
        (uint32)(0uL),                                                /*DataInvalid_Val*/
#endif
        (uint32)(0uL),                                                /* Init_Val*/
        
        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = COM_NOTCONFIGURED
            DataInvalidType : 1;    = COM_FALSE
            TimeOutEnabled  : 1;    = COM_FALSE
        }Com_TxSigPropType;    */
        0x54,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        24,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0xff,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        0xff,                                              /*OldVal_Index*/
#endif

        (Com_IpduIdType)ComConf_ComIPdu_Internal_BMS_Tem_5_Can_Network_CANNODE_0,             /*Ipdu Reference*/
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* BMS_BattTempSensorOrder35 */
#ifdef COM_TxInvalid
        (uint32)(0uL),                                                /*DataInvalid_Val*/
#endif
        (uint32)(0uL),                                                /* Init_Val*/
        
        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = COM_NOTCONFIGURED
            DataInvalidType : 1;    = COM_FALSE
            TimeOutEnabled  : 1;    = COM_FALSE
        }Com_TxSigPropType;    */
        0x54,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        16,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0xff,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        0xff,                                              /*OldVal_Index*/
#endif

        (Com_IpduIdType)ComConf_ComIPdu_Internal_BMS_Tem_5_Can_Network_CANNODE_0,             /*Ipdu Reference*/
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* BMS_BattTempSensorOrder34 */
#ifdef COM_TxInvalid
        (uint32)(0uL),                                                /*DataInvalid_Val*/
#endif
        (uint32)(0uL),                                                /* Init_Val*/
        
        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = COM_NOTCONFIGURED
            DataInvalidType : 1;    = COM_FALSE
            TimeOutEnabled  : 1;    = COM_FALSE
        }Com_TxSigPropType;    */
        0x54,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        8,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0xff,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        0xff,                                              /*OldVal_Index*/
#endif

        (Com_IpduIdType)ComConf_ComIPdu_Internal_BMS_Tem_5_Can_Network_CANNODE_0,             /*Ipdu Reference*/
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    },
        
    {  /* BMS_BattTempSensorOrder33 */
#ifdef COM_TxInvalid
        (uint32)(0uL),                                                /*DataInvalid_Val*/
#endif
        (uint32)(0uL),                                                /* Init_Val*/
        
        /*
        {
            TransProp       : 3;    = TRIGGERED
            FilterAlgorithm : 4;    = COM_NOTCONFIGURED
            DataInvalidType : 1;    = COM_FALSE
            TimeOutEnabled  : 1;    = COM_FALSE
        }Com_TxSigPropType;    */
        0x54,                                               /* Transmission Fields */

#ifdef COM_TxSigUpdateBit
        COM_UPDATE_MAX,                                    /*Update bit Position*/
#endif /* #ifdef COM_TxSigUpdateBit */

        0,                                                /*BitPosition*/

        8,/*BitSize*/

#ifdef COM_TxFilters
        0xff,                                              /*Filter_Index*/
#endif

#ifdef COM_EffectiveSigTOC
        0xff,                                              /*OldVal_Index*/
#endif

        (Com_IpduIdType)ComConf_ComIPdu_Internal_BMS_Tem_5_Can_Network_CANNODE_0,             /*Ipdu Reference*/
        /*
        {
            Type:5;       = UINT8
            Endianess:1;  = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType;*/
        0x20                                               /*General*/

    }

};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/* END: Tx Signal Details  */

/* START: Rx Signal Details  */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_Prv_xRxSigCfg_tst ,COM_CONST) Com_Prv_xRxSigCfg_acst[COM_NUM_RX_SIGNALS] =
{    
        
    {  /* OBC_ChargeSourceMode */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       (uint32)(0uL),    /* DataInvalid_Val */
#endif
           (uint32)(0uL),        /*Init_Val */
#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif /* #ifdef COM_RxSigUpdateBit */
       52,                         /*BitPosition*/
       0,                /* Signal Buffer Index */
       
      2, /*BitSize*/

#ifdef COM_RxFilters
        0xff,                           /*Filter_Index*/
#endif

       (Com_IpduIdType)ComConf_ComIPdu_Internal_OBC_4_Can_Network_CANNODE_0,                       /*Ipdu Reference*/
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   =  NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = COM_FALSE
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* OBC_FaultCode */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       (uint32)(0uL),    /* DataInvalid_Val */
#endif
           (uint32)(0uL),        /*Init_Val */
#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif /* #ifdef COM_RxSigUpdateBit */
       32,                         /*BitPosition*/
       1,                /* Signal Buffer Index */
       
      8, /*BitSize*/

#ifdef COM_RxFilters
        0xff,                           /*Filter_Index*/
#endif

       (Com_IpduIdType)ComConf_ComIPdu_Internal_OBC_4_Can_Network_CANNODE_0,                       /*Ipdu Reference*/
        /*
        {
            Type:5;        = UINT8
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType; */
        0x20,       /*General*/

        /*
        {
            DataInvalidType:2;   =  NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = COM_FALSE
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* OBC_MaxOutputVoltage */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       (uint32)(0uL),    /* DataInvalid_Val */
#endif
           (uint32)(0uL),        /*Init_Val */
#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif /* #ifdef COM_RxSigUpdateBit */
       24,                         /*BitPosition*/
       0,                /* Signal Buffer Index */
       
      16, /*BitSize*/

#ifdef COM_RxFilters
        0xff,                           /*Filter_Index*/
#endif

       (Com_IpduIdType)ComConf_ComIPdu_Internal_OBC_4_Can_Network_CANNODE_0,                       /*Ipdu Reference*/
        /*
        {
            Type:5;        = UINT16
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType; */
        0x22,       /*General*/

        /*
        {
            DataInvalidType:2;   =  NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = COM_FALSE
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    ,
        
    {  /* OBC_MaxOutputCurrent */
#ifdef COM_RxSignalNotify
        /* Notification Signal part */
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalidNotify
        /* Com Invalid Notification */
        
        NULL_PTR,
#endif
#ifdef COM_RxSigInvalid
       (uint32)(0uL),    /* DataInvalid_Val */
#endif
           (uint32)(0uL),        /*Init_Val */
#ifdef COM_RxSigUpdateBit
       COM_UPDATE_MAX,                    /*Update bit Position*/
#endif /* #ifdef COM_RxSigUpdateBit */
       8,                         /*BitPosition*/
       1,                /* Signal Buffer Index */
       
      16, /*BitSize*/

#ifdef COM_RxFilters
        0xff,                           /*Filter_Index*/
#endif

       (Com_IpduIdType)ComConf_ComIPdu_Internal_OBC_4_Can_Network_CANNODE_0,                       /*Ipdu Reference*/
        /*
        {
            Type:5;        = UINT16
            Endianess:1;   = BIG_ENDIAN
            UpdBitConf:1;  = COM_FALSE
            Not_Used:1;
        }Com_GeneralType; */
        0x22,       /*General*/

        /*
        {
            DataInvalidType:2;   =  NONE
            FilterAlgorithm:4;   = COM_NOTCONFIGURED
            DataTimeoutType:1;   = NONE
            IsGwSignal:1         = COM_FALSE
        } Com_RxSigPropType; */
        0x28         /* Reception Fields */
    }
    

};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/* END: Rx Signal Details  */











/* START: TMS Details  */


/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/* BMS_Tem_5_Can_Network_CANNODE_0 has a TMS switch */
static CONST(Com_TransModeInfo ,COM_CONST) Com_BMS_Tem_5_Can_Network_CANNODE_0_TransModeInfo[] =
{
    /* True Mode configuration */
    {
        100, /* TimePeriod */
        
        1, /* TimeOffset */
        
        0, /* RepetitionPeriod */
        
        0,  /* NumRepetitions */

#ifdef COM_MIXEDPHASESHIFT
        COM_TXMODE_PERIODIC, /* Mode */
        COM_FALSE    /* MixedPhaseShift status */
#else
        COM_TXMODE_PERIODIC /* Mode */
#endif /* #ifdef COM_MIXEDPHASESHIFT */

    }
,
    /* False Mode configuration */
    {
        100, /* TimePeriod */
        
        1, /* TimeOffset */
        
        0, /* RepetitionPeriod */
        
        0,  /* NumRepetitions */

#ifdef COM_MIXEDPHASESHIFT
        COM_TXMODE_PERIODIC, /* Mode */
        COM_FALSE    /* MixedPhaseShift status */
#else
        COM_TXMODE_PERIODIC /* Mode */
#endif /* #ifdef COM_MIXEDPHASESHIFT */

    }
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/* END: TMS Details  */



/* START: Tx IPDU Details  */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_Prv_xTxIpduInfoCfg_tst ,COM_CONST) Com_Prv_xTxIpduCfg_acst[COM_NUM_TX_IPDU] =
{
    {   /*Ipdu: BMS_Tem_5_Can_Network_CANNODE_0*/

        Com_IpduBuf_BMS_Tem_5_Can_Network_CANNODE_0,              /*Pointer to the Ipdu Buffer*/

        
        Com_BMS_Tem_5_Can_Network_CANNODE_0_TransModeInfo,
        
        #ifdef COM_TxIPduCallOuts
        /* Ipdu Callout Function*/
        NULL_PTR,
        
        #endif
        #ifdef COM_TxIPduNotification
        /* Ipdu Notification Function*/

        NULL_PTR,
        #endif

        #ifdef COM_ERRORNOTIFICATION
        /* Error Notification part */

        NULL_PTR,
        #endif

        #ifdef COM_TxIPduTimeOutNotify
        /* Timeout Notification part*/

        NULL_PTR,
        #endif


        #ifdef COM_METADATA_SUPPORT
        NULL_PTR,               /* Pointer to the MetaData Ipdu Buffer */
        #endif


        8,/*Size in Bytes*/
        #ifdef COM_TxIPduTimeOut
        100,               /*Timeout Fact*/
        #endif /* #ifdef COM_TxIPduTimeOut */
        0, /*MinDelay Time factor*/

        8,                    /*No Of Signals present in the IPDU*/
        #ifdef COM_TX_SIGNALGROUP

        0,               /*No of Signal Groups present in the IPDU*/
        #endif

        (PduIdType)PduRConf_PduRSrcPdu_BMS_Tem_5_Can_Network_CANNODE_0_Com2PduR,              /* PduR Id */

        (Com_TxIntSignalIdType)ComSignal_Internal_BMS_BattTempSensorOrder40,     /*Index to First Signal within this Ipdu*/
        #ifdef COM_TX_SIGNALGROUP

        0,                            /*This IPDU does not contain any Signal Groups*/
        #endif






/*
{
    Signal_Processing:1;          = IMMEDIATE
    TMSCalclation:2;              = MODE_VALID
    NoneModeTimeOut:1;            = COM_FALSE
    ClearUpdBit:2                 = CLEAR_UPDATEBIT_NOT_APPLICABLE
    FilterEvalReq:1               = COM_FALSE
    IsDynPdu:1;                   = COM_FALSE
    IsGwDestPdu:1;                = COM_FALSE
    IsCalloutFrmTrigTrans:1;      = COM_FALSE
    isLargeDataPdu:1;             = COM_FALSE
    isCancelTransmitSupported:1;  = COM_FALSE
    IsMetaDataSupported:1;        = COM_FALSE
    defaultTMSStatus:1;           = COM_FALSE
    ipduPartOfIpduGrp:1;          = COM_TRUE
    Not_Used:1;
}Com_TxIpduFlagType;
*/
        0x4000,  /*Transmission Type*/

        /* Com_MainFunctionTx() */
        ComMainFunction_Internal_MainFunctionTx,

        0xFF               /*Padding Byte*/

    }
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/* END: Tx IPDU Details  */

/* START : Time out information structure for signals with update-bits */
#ifdef COM_RxSigUpdateTimeout
#endif /* #ifdef COM_RxSigUpdateTimeout */
/* END : Time out information structure for signals with update-bits */

/* START : Time out information structure for signal groups with update-bits */
#ifdef COM_RxSigGrpUpdateTimeout
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */

#ifdef COM_RxSigUpdateTimeout
#endif /* #ifdef COM_RxSigUpdateTimeout */
/* END : Time out information structure for signals with update-bits */

/* START : Time out information structure for signal groups with update-bits */
#ifdef COM_RxSigGrpUpdateTimeout
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */
/* END : Time out information structure for signal groups with update-bits */



/* START: Rx IPDU Details  */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"


CONST(Com_Prv_xRxIpduInfoCfg_tst ,COM_CONST) Com_Prv_xRxIpduCfg_acst[COM_NUM_RX_IPDU] =
{
    {   /*Ipdu: OBC_4_Can_Network_CANNODE_0*/
        

        Com_LocalBuf_OBC_4_Can_Network_CANNODE_0,              /*Pointer to the Local Ipdu Buffer*/
        
        #ifdef COM_RxSigUpdateTimeout

        NULL_PTR,                       /* Pointer to timeout information structure for signals with update-bits, within the IPdu */
        #endif /* #ifdef COM_RxSigUpdateTimeout */
        #ifdef COM_RxSigGrpUpdateTimeout

        NULL_PTR,                       /* Pointer to timeout information structure for signal groups with update-bits, within the IPdu */
        #endif /* #ifdef COM_RxSigGrpUpdateTimeout */

        #ifdef COM_RxIPduCallOuts
        /* Ipdu Callout */

        NULL_PTR,
        
        #endif

        #ifdef COM_RxIPduTimeoutNotify
        /* Timeout Notification part*/

        NULL_PTR,
        #endif /* COM_RxIPduTimeoutNotify */

        #ifdef COM_RxIPduNotification
        /* Ipdu Notification Function*/

        NULL_PTR,              /* Rx IPdu notification callback */
        #endif

        8, /*Size in Bytes*/

        #ifdef COM_RxIPduTimeout

        0,        /* First time out value after IPDU group start */

        0,              /*CC requriment: Support Rx IPDU Timeout*/
        #endif /* #ifdef COM_RxIPduTimeout */

        4,                /*No Of Signals present in the IPDU*/
        #ifdef COM_RX_SIGNALGROUP

        0,           /*No of Signal Groups present in the IPDU*/
        #endif

        (Com_RxIntSignalIdType)ComSignal_Internal_OBC_ChargeSourceMode,   /*Index to First Signal within this Ipdu*/
         #ifdef COM_RX_SIGNALGROUP

        0,                            /*This IPDU does not contain any Signal Groups*/
        #endif
#ifdef COM_SIGNALGATEWAY
        0,       /* Number of signals with gateway */
#endif /* #ifdef COM_SIGNALGATEWAY */
#ifdef COM_SIGNALGROUPGATEWAY
        0,   /* Number of signal groups with gateway */
#endif /* #ifdef COM_SIGNALGROUPGATEWAY */

        /* Com_MainFunctionRx() */
        ComMainFunction_Internal_MainFunctionRx,

        /*
        {
            Signal_Processing:1;  = IMMEDIATE
            Notify_Cbk:1;         = false
            IsGwIPdu:1;           = false
            Not_Used:5;
        } Com_RxIpduFlagType;
        */
        0x0                 /* Reception Type */


    }
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/* END: Rx IPDU Details  */



/* START: IPDU Group Details  */

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_Prv_xIpduGrpInfoCfg_tst ,COM_CONST) Com_Prv_xIpduGrpCfg_acst[2] =
{           /* "Index of First IPdu"               "No of Rx-Ipdus" */
{
    /* ComIPduGroup_Rx */
    
    0,
    
    1
}
,{
    /* ComIPduGroup_Tx */
    
    1,
    
    0
}

};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/* END: IPDU Group Details  */

   





/* Reference to Ipdus belonging to the Ipdu Groups */


/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_IpduIdType ,COM_CONST) Com_Prv_xIPduGrp_IpduRefCfg_au8[2] =
{

    /* ComIPduGroup_Rx */

    ComConf_ComIPdu_Internal_OBC_4_Can_Network_CANNODE_0,
    /* ComIPduGroup_Tx */

    (COM_NUM_RX_IPDU + (Com_IpduIdType)ComConf_ComIPdu_Internal_BMS_Tem_5_Can_Network_CANNODE_0)
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"


#if defined ( COM_TX_SIGNALGROUP_ARRAY )

/* Tx-SignalGroup Array are not configured */
# define Com_Prv_xTxSigGrpArrayCfg_acst      ( NULL_PTR )
#endif /* #if defined ( COM_TX_SIGNALGROUP_ARRAY ) */

#if defined ( COM_RX_SIGNALGROUP_ARRAY )
/* Rx-SignalGroup Array are not configured */
# define Com_Prv_xRxSigGrpArrayCfg_acst      ( NULL_PTR )
#endif /* #if defined ( COM_RX_SIGNALGROUP_ARRAY ) */

/* Start section for RAM variables */

/* START: I-PDU Buffers */

   /* Tx Ipdu buffers */


/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"


    VAR(uint8,COM_VAR) Com_IpduBuf_BMS_Tem_5_Can_Network_CANNODE_0[8];


/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"



    /* Rx-Ipdu Local Buffers */


/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"


    VAR(uint8,COM_VAR) Com_LocalBuf_OBC_4_Can_Network_CANNODE_0[8];


/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"



/* END: I-PDU Buffers */

#ifdef COM_F_MASKEDNEWDIFFERSOLD

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint32 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_32
#include "Com_MemMap.h"
VAR(uint32,COM_VAR) Com_F_OldVal[1];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint32 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_32
#include "Com_MemMap.h"
#endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */

#if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC)

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
VAR(Com_OldValTrigOnChngType,COM_VAR) Com_OldValTrigOnChng[1];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* #if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC) */

#ifdef COM_F_ONEEVERYN

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
CONST(Com_OneEveryNType,COM_CONST) Com_OneEveryN_Const[1] =
{
    /* Period   Offset  Occurence*/


{    1,    5, 1    }    /*  DummyForMisra    */
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"


/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
VAR(Com_OneEveryNType,COM_VAR) Com_OneEveryN[1];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* #ifdef COM_F_ONEEVERYN */


/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

#ifdef COM_TP_IPDUTYPE
VAR(PduLengthType,COM_VAR)          Com_TpTxIpduLength_auo[COM_NUM_TX_IPDU];
#endif

VAR(Com_TxIpduRamData,COM_VAR)      Com_TxIpduRam_s[COM_NUM_TX_IPDU];

VAR(Com_RxIpduRamData,COM_VAR)      Com_RxIpduRam_s[COM_NUM_RX_IPDU];

VAR(Com_TxSignalFlagType,COM_VAR)   Com_TxSignalFlag[COM_NUM_TX_SIGNALS];

VAR(Com_RxSignalFlagType,COM_VAR)   Com_RxSignalFlag[COM_NUM_RX_SIGNALS];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"


/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"

VAR(uint8,COM_VAR)    Com_IpduCounter_s[COM_NUM_TX_IPDU + COM_NUM_RX_IPDU];

VAR(uint8,COM_VAR)    Com_IpduCounter_DM[COM_NUM_RX_IPDU];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"


#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
VAR(PduIdType,COM_VAR)    Com_RxGwQueue_aux[COM_NUM_GWSRC_IPDUS+1u];

VAR(Com_RxGwQueueRAMType,COM_VAR)   Com_RxGwQueue_S;

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */


#ifdef COM_RX_SIGNALGROUP

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
VAR(Com_RxSignalGrpFlagType,COM_VAR) Com_RxSignalGrpFlag[1u];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
#endif

#ifdef COM_TX_SIGNALGROUP
#if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY)

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
VAR(Com_TxSignalGrpFlagType,COM_VAR) Com_TxSignalGrpFlag[1u];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* #if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY) */


/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of struct/enum/pointer type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
VAR(Com_TxGrpSignalFlagType,COM_VAR) Com_TxGrpSignalFlag[1u];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of struct/enum/pointer type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
#endif

#ifdef COM_RxIPduDeferredProcessing

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"

    /* Buffer to hold data for deferred signal processing */
    VAR(uint8,COM_VAR) Com_DeferredBuffer[ 8 ];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"

#endif /* #ifdef COM_RxIPduDeferredProcessing */

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of boolean type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
#include "Com_MemMap.h"

    VAR(boolean, AUTOMATIC) Com_IsGwSignalsUpdated_ab[ COM_NUM_GWSRC_IPDUS ];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of boolean type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
#include "Com_MemMap.h"

#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

/* End section for RAM variables */


#if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX)

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_MaskXType ,COM_CONST) Com_MaskX[1] =
{
    /*Mask       X*/



{    1,    5    }    /*  DummyForMisra    */
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"


#endif /* #if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX) */


#ifdef COM_F_MASKEDNEWDIFFERSOLD

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_32
#include "Com_MemMap.h"

CONST(uint32 ,COM_CONST) Com_Mask[1] =
{
    /*Mask*/
    1    /* DummyForMisra */
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_32
#include "Com_MemMap.h"

#endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */


#if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS)

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_POSMinMaxType ,COM_CONST) Com_POSMinMax[1]=
{
    /*  Min      Max */





{    1,    5    }    /*  DummyForMisra    */


};


/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

#endif /* #if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS) */


#if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG)

/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_NEGMinMaxType ,COM_CONST) Com_NEGMinMax[1] =
{
    /*  Min      Max */


{    1,    5    }    /*  DummyForMisra    */
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

#endif /* #if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG) */


/* Signal Buffers */
/* Type: UINT8, SINT8, UINT8_N, BOOLEAN */

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"

VAR(uint8,COM_VAR) Com_SigType_u8[2];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"


/* Type: UINT16, SINT16 */

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint16 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_16
#include "Com_MemMap.h"

VAR(uint16,COM_VAR) Com_SigType_u16[2];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint16 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_16
#include "Com_MemMap.h"


/* Type: UINT32, SINT32 */
#define Com_SigType_u32          ( NULL_PTR )

/* Type: UINT64, SINT64 */
#ifdef COM_RXSIG_INT64
#define Com_SigType_u64          ( NULL_PTR )
#endif /* #ifdef COM_RXSIG_INT64 */


#ifdef COM_RXSIG_FLOAT64SUPP
#define Com_SigType_f64          ( NULL_PTR )
#endif /* #ifdef COM_RXSIG_FLOAT64SUPP */

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
#define Com_SigType_dyn          ( NULL_PTR )
#endif

#ifdef COM_RX_SIGNALGROUP_ARRAY
#define Com_RxSigGrpArrayBuf_u8  ( NULL_PTR )
#endif /* #ifdef COM_RX_SIGNALGROUP_ARRAY */

#ifdef COM_RX_SIGNALGROUP
/* Rx Group Signal Buffers */
/* Type: UINT8, SINT8, UINT8_N, BOOLEAN */
#define Com_RxGrpSigType_u8      ( NULL_PTR )

/* Type: UINT16, SINT16 */
#define Com_RxGrpSigType_u16     ( NULL_PTR )

/* Type: UINT32, SINT32 */
#define Com_RxGrpSigType_u32     ( NULL_PTR )

/* Type: UINT64, SINT64 */
#ifdef COM_RXGRPSIG_INT64
#define Com_RxGrpSigType_u64     ( NULL_PTR )
#endif /* #ifdef COM_RXGRPSIG_INT64 */

#ifdef COM_RXGRPSIG_FLOAT64SUPP
#define Com_RxGrpSigType_f64     ( NULL_PTR )
#endif

/* The function Com_ReceiveSignalGroup() is not interrupt safe, hence there is need to
 * to generate second buffer for all the group signals
 */
/* Type: UINT8, SINT8, UINT8_N, BOOLEAN */
#define Com_SecondRxGrpSigType_u8        ( NULL_PTR )

/* Type: UINT16, SINT16 */
#define Com_SecondRxGrpSigType_u16       ( NULL_PTR )

/* Type: UINT32, SINT32 */
#define Com_SecondRxGrpSigType_u32       ( NULL_PTR )

/* Type: UINT64, SINT64 */
#ifdef COM_RXGRPSIG_INT64
#define Com_SecondRxGrpSigType_u64       ( NULL_PTR )
#endif /* #ifdef COM_RXGRPSIG_INT64 */

#ifdef COM_RXGRPSIG_FLOAT64SUPP
#define Com_SecondRxGrpSigType_f64       ( NULL_PTR )
#endif /* #ifdef COM_RXGRPSIG_FLOAT64SUPP */

#endif /* #ifdef COM_RX_SIGNALGROUP */


/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_Prv_xRxRamBuf_tst, COM_CONST) Com_Prv_xRxRamBuf_acst[ 1u ] =
{
    {
        (P2VAR(uint8, TYPEDEF, COM_APPL_DATA))              Com_SigType_u8

#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
        ,(P2VAR(uint8, TYPEDEF, COM_APPL_DATA))             Com_SigType_dyn
#endif

        ,(P2VAR(uint16, TYPEDEF, COM_APPL_DATA))            Com_SigType_u16

        ,(P2VAR(uint32, TYPEDEF, COM_APPL_DATA))            Com_SigType_u32

#ifdef COM_RXSIG_INT64
        ,(P2VAR(uint64, TYPEDEF, COM_APPL_DATA))            Com_SigType_u64
#endif

#ifdef COM_RXSIG_FLOAT64SUPP
        ,(P2VAR(float64, TYPEDEF, COM_APPL_DATA))           Com_SigType_f64
#endif

#ifdef COM_RX_SIGNALGROUP
        ,(P2VAR(uint8, TYPEDEF, COM_APPL_DATA))             Com_RxGrpSigType_u8

        ,(P2VAR(uint16, TYPEDEF, COM_APPL_DATA))            Com_RxGrpSigType_u16

        ,(P2VAR(uint32, TYPEDEF, COM_APPL_DATA))            Com_RxGrpSigType_u32

#ifdef COM_RXGRPSIG_INT64
        ,(P2VAR(uint64, TYPEDEF, COM_APPL_DATA))            Com_RxGrpSigType_u64
#endif

#ifdef COM_RXGRPSIG_FLOAT64SUPP
        ,(P2VAR(float64, TYPEDEF, COM_APPL_DATA))           Com_RxGrpSigType_f64
#endif

        ,(P2VAR(uint8, TYPEDEF, COM_APPL_DATA))             Com_SecondRxGrpSigType_u8

        ,(P2VAR(uint16, TYPEDEF, COM_APPL_DATA))            Com_SecondRxGrpSigType_u16

        ,(P2VAR(uint32, TYPEDEF, COM_APPL_DATA))            Com_SecondRxGrpSigType_u32

#ifdef COM_RXGRPSIG_INT64
        ,(P2VAR(uint64, TYPEDEF, COM_APPL_DATA))            Com_SecondRxGrpSigType_u64
#endif

#ifdef COM_RXGRPSIG_FLOAT64SUPP
        ,(P2VAR(float64, TYPEDEF, COM_APPL_DATA))           Com_SecondRxGrpSigType_f64
#endif

#ifdef COM_RX_SIGNALGROUP_ARRAY
        ,(P2VAR(uint8, TYPEDEF, COM_APPL_DATA))             Com_RxSigGrpArrayBuf_u8
#endif

#endif /* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_RxIPduDeferredProcessing
        ,(P2VAR(uint8, TYPEDEF, COM_APPL_DATA))             Com_DeferredBuffer
#endif
    }

};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"



#ifdef COM_TX_SIGNALGROUP



/* Tx Group Signal Buffers  */
/* Type: UINT8, SINT8, UINT8_N, BOOLEAN */
#define Com_TxGrpSigType_u8          ( NULL_PTR )

/* Type: UINT16, SINT16 */
#define Com_TxGrpSigType_u16         ( NULL_PTR )

/* Type: UINT32, SINT32 */
#define Com_TxGrpSigType_u32         ( NULL_PTR )

/* Type: UINT64, SINT64 */
#ifdef COM_TXGRPSIG_INT64
#define Com_TxGrpSigType_u64         ( NULL_PTR )
#endif /* #ifdef COM_TXGRPSIG_INT64 */


#ifdef COM_TXGRPSIG_FLOAT64SUPP
/* Type: FLOAT64 */
#define Com_TxGrpSigType_f64         ( NULL_PTR )
#endif /* #ifdef COM_TXGRPSIG_FLOAT64SUPP */




/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_Prv_xTxSigGrpRamBuf_tst, COM_CONST) Com_Prv_xTxSigGrpRamBuf_acst[ 1u ] =
{
    {
        (P2VAR(uint8, TYPEDEF, COM_APPL_DATA))              Com_TxGrpSigType_u8,

        (P2VAR(uint16, TYPEDEF, COM_APPL_DATA))             Com_TxGrpSigType_u16,

        (P2VAR(uint32, TYPEDEF, COM_APPL_DATA))             Com_TxGrpSigType_u32

#ifdef COM_TXGRPSIG_INT64
        ,(P2VAR(uint64, TYPEDEF, COM_APPL_DATA))            Com_TxGrpSigType_u64
#endif

#ifdef COM_TXGRPSIG_FLOAT64SUPP
        ,(P2VAR(float64, TYPEDEF, COM_APPL_DATA))           Com_TxGrpSigType_f64
#endif

    }

};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"


#endif/* #ifdef COM_TX_SIGNALGROUP */


/* ------------------------------------------------------------------------ */
/* Begin section for constants */
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

CONST(Com_MainFunctionCfgType, COM_CONST) Com_MainFunctionCfg[ COM_NUM_OF_MAINFUNCTION ] =
{
    {
        /* Com_MainFunctionRx() - cylce time Rx: 0.01 s */
        0u,  /* Start RxIPdu-Id */
        1u,  /* Num of RxIpdus */
        10u /* TimeBase in ms */
    }
    ,{
        /* Com_MainFunctionTx() - cylce time Tx: 0.01 s */
        0u,  /* Start TxIPdu-Id */
        1u,  /* Num of TxIpdus */
        10u /* TimeBase in ms */
    }
};

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"



