

#ifndef CANIF_CBK_H
#define CANIF_CBK_H

/***************************************************************************************************
**                      Include Section                                                           **
***************************************************************************************************/
/*File included here to avoid this being included by CanDrv in Can.c and Can_Cfg.c*/
// #include "CanIf_Cfg.h"
#include "CanIf_RxIndication_Integration.h"

/*T:
#if (CANIF_CDD_ENABLED != STD_OFF)
#include CANIF_CDD_HEADERFILE
#endif
*/

/***************************************************************************************************
**                                   Global Callback Function Prototypes                          **
***************************************************************************************************/

/* Declaration of CanIf_RxIndication() API */
//#if CANIF_CFG_NUM_CANRXPDUIDS != CANIF_ZERO
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE) CanIf_RxIndication_Internal(P2CONST(Can_HwType, AUTOMATIC, CANIF_APPL_DATA) Mailbox,
        P2CONST(PduInfoType, AUTOMATIC, CANIF_APPL_DATA)PduInfoPtr);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
//#endif


/* Declaration of CanIf_TrcvModeIndication() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE) CanIf_TrcvModeIndication( VAR(uint8,AUTOMATIC) TransceiverId,
        VAR(CanTrcv_TrcvModeType,AUTOMATIC) TransceiverMode);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_ClearTrcvWufFlagIndication() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE) CanIf_ClearTrcvWufFlagIndication( VAR(uint8,AUTOMATIC) TransceiverId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_CheckTrcvWakeFlagIndication() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE) CanIf_CheckTrcvWakeFlagIndication( VAR(uint8,AUTOMATIC) TransceiverId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_ConfirmPnAvailability() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
FUNC(void, CANIF_CODE) CanIf_ConfirmPnAvailability( VAR(uint8,AUTOMATIC) TransceiverId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/* Declaration of CanIf_TxConfirmation() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE) CanIf_TxConfirmation(VAR(PduIdType,AUTOMATIC) CanTxPduId );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(Std_ReturnType, CANIF_CODE) CanIf_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"


/* Declaration of CanIf_ControllerBusOff() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE) CanIf_ControllerBusOff(VAR(uint8,AUTOMATIC) ControllerId );
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* Declaration of CanIf_ControllerModeIndication() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE) CanIf_ControllerModeIndication( VAR(uint8,AUTOMATIC) ControllerId,
        VAR(CanIf_ControllerModeType, AUTOMATIC) ControllerMode);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"

/* FC_VariationPoint_START */
/* Declaration of CanIf_ControllerErrorPassive() API */
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"
extern FUNC(void, CANIF_CODE) CanIf_ControllerErrorPassive( VAR(uint8, AUTOMATIC) ControllerId);
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
/* FC_VariationPoint_END */

#endif   /* CANIF_CBK_H */


