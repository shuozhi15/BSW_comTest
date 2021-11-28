/** @file     Rte_Type.h
  *
  * @brief    RTE type definitions
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.1.0 
  *
  * @date     Sun Nov 28 22:56:39 2021
  */

#ifndef RTE_TYPE_H
#define RTE_TYPE_H

#include "Rte.h"
#include "Rte_Cfg.h"

/*******************************************************
 ***
 *** Forward Declarations
 ***
 *******************************************************/

struct Rte_CDS_BswM;
struct Rte_CDS_ComM;
struct Rte_CDS_EcuM;
struct Rte_CDS_swc;

/*******************************************************
 ***
 *** Types
 ***
 *******************************************************/

/* Information: No typedef for ImplementationDataType uint16 because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType uint8 because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType Std_VersionInfoType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType BswM_ConfigType because typeEmitter is a value other than RTE */

typedef uint16 BswM_ModeType;

typedef uint16 BswM_UserType;

/* Information: No typedef for ImplementationDataType Can_ConfigType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType Can_HwHandleType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType Can_IdType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType Can_HwType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType PduIdType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType Can_PduType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType Can_ReturnType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType Can_StateTransitionType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType uint32 because typeEmitter is a value other than RTE */

typedef uint32 CanIf_u32_impl;

typedef uint16 CanIf_u16_impl;

typedef uint8 CanIf_u8_impl;

typedef struct {
   CanIf_u32_impl CanId;
   CanIf_u16_impl swPduHandle;
   CanIf_u8_impl  SduLength;
   CanIf_u8_impl  BufferIndex;
} CanIf_CanIdBuffer_struct_impl;

/* Information: No typedef for ImplementationDataType CanIf_ConfigType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType CanIf_ControllerModeType because typeEmitter is a value other than RTE */

typedef uint8 CanIf_ControllerModeType_Enum_impl;

typedef uint8 CanIf_PduModeType_Enum_impl;

typedef struct {
   CanIf_ControllerModeType_Enum_impl DeviceMode;
   CanIf_PduModeType_Enum_impl        ChannelMode;
} CanIf_ControllerStateType_struct_impl;

typedef CanIf_ControllerStateType_struct_impl CanIf_ControllerState_Astruct_impl[1];
/* Information: No typedef for ImplementationDataType CanIf_NotifStatusType because typeEmitter is a value other than RTE */

typedef uint8 CanIf_NotifStatusType_Enum_impl;

typedef CanIf_NotifStatusType_Enum_impl CanIf_NotifStatusType_Aenum_impl[1];
/* Information: No typedef for ImplementationDataType CanIf_PduGetModeType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType CanIf_PduModeType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType CanIf_PduSetModeType because typeEmitter is a value other than RTE */

typedef uint8 CanIf_Prv_BuffStatus_ten_Enum_impl;

typedef struct {
   CanIf_u8_impl last_index;
   CanIf_u8_impl bufferstatus;
} CanIf_Prv_TxBufferStatus_tst_struct_impl;

typedef uint8 CanIf_boolean_impl;

typedef uint8 CanSM_boolean_Impl;

typedef CanSM_boolean_Impl CanSM_Aboolean_impl[1];
typedef uint8 CanSM_u8_Impl;

typedef CanSM_u8_Impl CanSM_Au8_impl[1];
/* Information: No typedef for ImplementationDataType CanSM_BswMCurrentStateType because typeEmitter is a value other than RTE */

typedef uint8 CanSM_BusOffRecoveryStateType_Enum_impl;

/* Information: No typedef for ImplementationDataType CanSM_ConfigType because typeEmitter is a value other than RTE */

typedef uint8 CanSM_NetworkModeStateType_Enum_impl;

/* Information: No typedef for ImplementationDataType CanSM_StateType because typeEmitter is a value other than RTE */

typedef uint16 CanSM_u16_Impl;

typedef uint8 CanSM_TimerStateType_Enum_impl;

typedef struct {
   CanSM_u16_Impl                 cntTick_u16;
   CanSM_TimerStateType_Enum_impl stTimer;
} CanSM_TimerConfig_tst_struct_impl;

typedef CanSM_TimerConfig_tst_struct_impl CanSM_TimerConfig_ast_Astruct_impl[1];
typedef CanSM_BusOffRecoveryStateType_Enum_impl CanSM_currBOR_State_a_Aenum_impl[1];
typedef CanSM_NetworkModeStateType_Enum_impl CanSM_currComM_Mode_a_Aenum_impl[1];
typedef uint16 Com_impl_u16;

typedef Com_impl_u16 Com_SigBuf_au16_impl[2];
typedef uint8 Com_impl_u8;

typedef Com_impl_u8 Com_implDataType_au8_Size8[8];
/* Information: No typedef for ImplementationDataType boolean because typeEmitter is a value other than RTE */

typedef boolean Com_impl_b;

typedef uint32 Com_impl_u32;

/* Information: No typedef for ImplementationDataType uint64 because typeEmitter is a value other than RTE */

typedef uint64 Com_impl_u64;

typedef uint32 ComM_uint32_Impl;

typedef uint16 ComM_uint16_Impl;

typedef uint8 ComM_uint8_Impl;

typedef uint8 ComM_bool_Impl;

typedef struct {
   ComM_uint32_Impl ChannelState_e;
   ComM_uint32_Impl LightTimeoutCtr_u32;
   ComM_uint16_Impl MinFullComTimeoutCtr_u16;
   ComM_uint8_Impl  ChannelMode_u8;
   ComM_uint8_Impl  BusSmMode_u8;
   ComM_uint8_Impl  UserRequestCtr_u8;
   ComM_uint8_Impl  PassiveRequestState_u8;
   ComM_uint8_Impl  PncRequestCtr_u8;
   ComM_uint8_Impl  InhibitionReqStatus_u8;
   ComM_bool_Impl   NmNetworkRequestStatus_b;
   ComM_bool_Impl   DiagnosticRequestState_b;
   ComM_bool_Impl   CommunicationAllowed_b;
   ComM_bool_Impl   NmBusSleepIndicationStatus_b;
   ComM_bool_Impl   NmPrepareBusSleepIndicationStatus_b;
   ComM_bool_Impl   NmNetworkModeStatus_b;
} ComM_ChannelStruct_Impl;

typedef ComM_ChannelStruct_Impl ComM_ChannelStruct_Array_Impl[1];
typedef uint8 ComM_InhibitionStatusType;

typedef uint8 ComM_ModeType;

typedef uint8 ComM_UserHandleType;

typedef struct {
   ComM_uint16_Impl WakeUpInhibitionCtr_u16;
   ComM_uint16_Impl LimitToNoComCtr_u16;
   ComM_uint8_Impl  RequestedUserMode_t;
   ComM_uint8_Impl  IndicatedUserMode_t;
   ComM_uint8_Impl  numChannelsInFullCom_u8;
   ComM_uint8_Impl  numChannelsInSilentCom_u8;
   ComM_uint8_Impl  numChannelsInNoCom_u8;
} ComM_UserStruct_Impl;

typedef ComM_UserStruct_Impl ComM_UserStruct_Array_Impl[1];
/* Information: No typedef for ImplementationDataType BufReq_ReturnType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType BusTrcvErrorType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType IcomConfigIdType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType IcomSwitch_ErrorType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType NetworkHandleType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType NotifResultType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType PNCHandleType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType PduLengthType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType PduInfoType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType TpDataStateType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType RetryInfoType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType TPParameterType because typeEmitter is a value other than RTE */

typedef uint8 EcuM_BootTargetType;

/* Information: No typedef for ImplementationDataType EcuM_RunStatusType because typeEmitter is a value other than RTE */

typedef uint8 EcuM_ShutdownCauseType;

/* Information: No typedef for ImplementationDataType EcuM_ShutdownInfoType because typeEmitter is a value other than RTE */

typedef uint16 EcuM_ShutdownModeType;

typedef uint8 EcuM_ShutdownTargetType;

/* Information: No typedef for ImplementationDataType EcuM_StateType because typeEmitter is a value other than RTE */

typedef uint32 EcuM_TimeType;

typedef uint16 EcuM_UserType;

/* Information: No typedef for ImplementationDataType EcuM_WakeupSourceType because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType EcuM_WakeupStatusType because typeEmitter is a value other than RTE */

typedef uint8 PduR_uint8_impl;

/* Information: No typedef for ImplementationDataType float32 because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType float64 because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType sint16 because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType sint16_least because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType sint32 because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType sint32_least because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType sint64 because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType sint8 because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType sint8_least because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType uint16_least because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType uint32_least because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType uint8_least because typeEmitter is a value other than RTE */

/* Information: No typedef for ImplementationDataType Std_ReturnType because typeEmitter is a value other than RTE */

typedef P2FUNC(VAR(Std_ReturnType, AUTOMATIC), RTE_CODE, Rte_ImplWriteFP_swc_soc_soc_p) (VAR(uint16, AUTOMATIC) data);

typedef P2CONST(struct Rte_CDS_BswM,AUTOMATIC,RTE_CONST) Rte_SelfType_BswM;

typedef P2CONST(struct Rte_CDS_ComM,AUTOMATIC,RTE_CONST) Rte_SelfType_ComM;

typedef P2CONST(struct Rte_CDS_EcuM,AUTOMATIC,RTE_CONST) Rte_SelfType_EcuM;

typedef P2CONST(struct Rte_CDS_swc,AUTOMATIC,RTE_CONST) Rte_SelfType_swc;


#endif /* !RTE_TYPE_H */
