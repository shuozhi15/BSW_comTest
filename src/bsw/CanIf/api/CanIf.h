

#ifndef CANIF_H
#define CANIF_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/* Type definitions of CAN Interface */
#include "CanIf_Types.h"

/* CanIf configuration header file */
#include "CanIf_Cfg.h"

#if(CANIF_CONFIGURATION_VARIANT == CANIF_CFG_VAR_PBS)
/* CanIf configuration header file */
#include "CanIf_PBcfg.h"
#endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/




/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/


/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/


/* Declaration of CanIf_Init() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(void, CANIF_CODE) CanIf_Init(P2CONST (CanIf_ConfigType, AUTOMATIC, CANIF_APPL_CONST) ConfigPtr);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/* Declaration of CanIf_SetControllerMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetControllerMode(VAR (uint8,AUTOMATIC) ControllerId,
        VAR(CanIf_ControllerModeType,AUTOMATIC) ControllerMode);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_GetControllerMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetControllerMode(VAR(uint8,AUTOMATIC) ControllerId,
        P2VAR(CanIf_ControllerModeType, AUTOMATIC, CANIF_APPL_DATA) ControllerModePtr);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_Transmit() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_Transmit(VAR(PduIdType,AUTOMATIC) CanIfTxSduId,
        P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_CONST) CanIfTxInfoPtr);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_CancelTransmit() API */
#if (STD_ON == CANIF_PUBLIC_CANCEL_TRANSMIT_SUPPORT)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_CancelTransmit(VAR(PduIdType,AUTOMATIC) CanIfTxSduId);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* Declaration of CanIf_SetPduMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetPduMode(VAR(uint8,AUTOMATIC) ControllerId,
        VAR(CanIf_PduModeType,AUTOMATIC) PduModeRequest);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* FC_VariationPoint_START */
/* Declaration of CanIf_DeInit() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(void, CANIF_CODE) CanIf_DeInit(P2CONST (CanIf_ConfigType, AUTOMATIC, CANIF_APPL_CONST) ConfigPtr);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_Get_DirectHw_Info() API */
#if (CANIF_DIRECT_HW_WRITE == STD_ON)

#include "rba_Can.h"

#define CANIF_START_SEC_CODE_FAST
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_Get_DirectHw_Info (VAR(PduIdType, AUTOMATIC) CanTxPduId,
        P2VAR(rba_CanDirectHWInfo_o, AUTOMATIC, CANIF_APPL_DATA) hw_info_po);

#define CANIF_STOP_SEC_CODE_FAST
#include "CanIf_MemMap.h"

/* Declaration of CanIf_DirectHw_Write() API */

#define CANIF_START_SEC_CODE_FAST
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE)CanIf_DirectHw_Write(P2CONST(uint32, AUTOMATIC, CANIF_APPL_DATA) p_tx_data_buff_u32
        , VAR(uint32, AUTOMATIC) data_len_u32, P2CONST(rba_CanDirectHWInfo_o, AUTOMATIC, CANIF_APPL_DATA) hw_info_po);

#define CANIF_STOP_SEC_CODE_FAST
#include "CanIf_MemMap.h"
#endif
/* FC_VariationPoint_END */


/* Declaration of CanIf_GetPduMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetPduMode(
                                                         VAR(uint8,AUTOMATIC) ControllerId,
                                                         P2VAR(CanIf_PduModeType, AUTOMATIC,CANIF_APPL_DATA) PduModePtr
                                                        );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/* Declaration of CanIf_ReadRxPduData() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_ReadRxPduData(VAR(PduIdType, AUTOMATIC) CanIfRxSduId,
                                                        P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_CONST)CanIfRxInfoPtr);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/* Declaration of CanIf_ReadTxNotifStatus() API */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_ReadTxNotifStatus(
                                                                      VAR(PduIdType,AUTOMATIC) CanIfTxSduId
                                                                      );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/* Declaration of CanIf_ReadRxNotifStatus() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_ReadRxNotifStatus(VAR(PduIdType, AUTOMATIC) CanIfRxSduId);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/* Declaration of CanIf_GetVersionInfo() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(void, CANIF_CODE) CanIf_GetVersionInfo (P2VAR(Std_VersionInfoType, AUTOMATIC, CANIF_APPL_DATA) VersionInfo);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if (CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF)
/* Declaration of CanIf_SetDynamicTxId() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(void, CANIF_CODE) CanIf_SetDynamicTxId(
                                                  VAR(PduIdType,AUTOMATIC) CanTxPduId,
                                                  VAR(Can_IdType,AUTOMATIC) CanId
                                                   );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif

/* FC_VariationPoint_START */
/* Declaration of CanIf_ResetDynamicTxId() API */

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(void, CANIF_CODE) CanIf_ResetDynamicTxId(VAR(PduIdType, AUTOMATIC) CanTxPduId);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/* FC_VariationPoint_END */



#if (CANIF_WAKEUP_SUPPORT != STD_OFF)
/* Declaration of CanIf_CheckWakeup() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckWakeup (VAR(EcuM_WakeupSourceType, AUTOMATIC) WakeupSource);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

#if (CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_API != STD_OFF)
/* Declaration of CanIf_CheckValidation() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckValidation (VAR(EcuM_WakeupSourceType, AUTOMATIC) WakeupSource);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* Declaration of CanIf_SetTrcvMode() API */
#if CANIF_CFG_TRCV_DRV_SUPPORT ==STD_ON
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetTrcvMode(
                                            VAR(uint8, AUTOMATIC) TransceiverId,
                                            VAR(CanTrcv_TrcvModeType, AUTOMATIC) TransceiverMode
                                                          );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif
/* Declaration of CanIf_GetTrcvMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetTrcvMode(
                                            P2VAR(CanTrcv_TrcvModeType, AUTOMATIC, CANIF_APPL_DATA) TransceiverModePtr,
                                            VAR(uint8, AUTOMATIC) TransceiverId
                                                           );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if CANIF_CFG_TRCV_DRV_SUPPORT!=STD_OFF
/* Declaration of CanIf_GetTrcvWakeupReason() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_GetTrcvWakeupReason(
                                            VAR(uint8, AUTOMATIC) TransceiverId,
                                            P2VAR(CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANIF_APPL_DATA) TrcvWuReasonPtr
                                                                 );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/* Declaration of CanIf_SetTrcvWakeupMode() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetTrcvWakeupMode(
                                                VAR(uint8, AUTOMATIC) TransceiverId,
                                                VAR(CanTrcv_TrcvWakeupModeType, AUTOMATIC) TrcvWakeupMode
);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* FC_VariationPoint_START */
/* Declaration of CanIf_ReturnRxPduId() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_ReturnRxPduId(
                                                     VAR(PduIdType, AUTOMATIC) CanIfRxTargetPduId,
                                                    P2VAR(PduIdType, AUTOMATIC, CANIF_APPL_CONST) CanIfRxPduIdPtr
                                                    );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_ReturnTxPduId() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_ReturnTxPduId(
                                                     VAR(PduIdType, AUTOMATIC) CanIfTxTargetPduId,
                                                    P2VAR(PduIdType, AUTOMATIC, CANIF_APPL_CONST) CanIfTxPduIdPtr
                                                    );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/* FC_VariationPoint_END */



#if (CANIF_SET_BAUDRATE_API == STD_ON)
/* Declaration of CanIf_SetBaudrate() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetBaudrate(
                                                VAR (uint8,AUTOMATIC) ControllerId,
                                                CONST(uint16,AUTOMATIC) BaudRateConfigID
                                                );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


#if (CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT != STD_OFF)

/* Declaration of CanIf_GetTxConfirmationState() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(CanIf_NotifStatusType, CANIF_CODE) CanIf_GetTxConfirmationState(
                                       VAR(uint8,AUTOMATIC) ControllerId
                                               );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif


#if(CANIF_PUBLIC_PN_SUPPORT == STD_ON)

/* Declaration of CanIf_ClearTrcvWufFlag() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_ClearTrcvWufFlag(
                                              VAR(uint8,AUTOMATIC) TransceiverId
                                                               );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_CheckTrcvWakeFlag() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_CheckTrcvWakeFlag(
                                              VAR(uint8,AUTOMATIC) TransceiverId
                                                               );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif

/* FC_VariationPoint_START */
#if (CANIF_RB_READMSGID_API != STD_OFF)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_ReadTxMsgId() API */
extern FUNC(uint32, CANIF_CODE) CanIf_ReadTxMsgId(
                                                  VAR(uint16, AUTOMATIC) msgHandleTx_u16
                                                 );

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* Declaration of CanIf_ReadRxMsgId() API */
extern FUNC(uint32, CANIF_CODE) CanIf_ReadRxMsgId(VAR(uint16, AUTOMATIC) msgHandleRx_u16);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#endif

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* Declaration of CanIf_Rb_ReadTxPduCanId() API */
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_Rb_ReadTxPduCanId(
                                                    VAR(PduIdType,AUTOMATIC) CanIfTxPduId,
                                                    P2VAR(uint32, AUTOMATIC, AUTOMATIC) TxPduCanId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
/* Declaration of CanIf_Rb_ReadRxPduCanId() API */
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_Rb_ReadRxPduCanId( VAR(PduIdType, AUTOMATIC)CanIfRxPduId,
        P2VAR(uint32, AUTOMATIC, CANIF_APPL_DATA)RxPduCanId);

#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


#if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
#include "rba_Can.h"

/* Declaration of CanIf_Rb_ChangeRxPduCanId() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_Rb_ChangeRxPduCanId(VAR(PduIdType,AUTOMATIC) CanRxPduId, VAR(Can_IdType,AUTOMATIC) CanId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

/* FC_VariationPoint_END */


/*
 **********************************************************************************************************************
 * Global Variables                                                                                                   *
 **********************************************************************************************************************
*/

#endif   /* CANIF_H */
