

/****************************************************************************************************/
/*                                  Include Section                                                 */
/****************************************************************************************************/

#include "CanIf_Prv.h"

/***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/* Flag to check if CANIF is Initialized(TRUE) or Not Initialized (FALSE)*/
#define CANIF_START_SEC_VAR_CLEARED_BOOLEAN
#include "CanIf_MemMap.h"
VAR(boolean, CANIF_VAR) CanIf_Prv_InitStatus_b;
#define CANIF_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "CanIf_MemMap.h"


#if (CANIF_PUBLIC_READRXPDU_DATA_API == STD_ON && CANIF_CFG_READRXPDU_DATA_IN_RXPDUS == STD_ON)

/* RxBuffer Array */
#define CANIF_START_SEC_VAR_CLEARED_8
#include "CanIf_MemMap.h"
VAR(uint8, CANIF_VAR) CanIf_Prv_RxBuffer_tau8[CANIF_CFG_RX_BUFFER_SIZE];
#define CANIF_STOP_SEC_VAR_CLEARED_8
#include "CanIf_MemMap.h"
#endif


#if (CANIF_READRXPDU_NOTIFY_STATUS_API != STD_OFF)
/* Store the Notification Status of each CanRxPduId  */
#define CANIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_Prv_RxNotification_taen[CANIF_CFG_NUM_CANRXPDUIDS];
#define CANIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
#endif


/*Trcv LUT*/
#if (CANIF_CFG_TRCV_DRV_SUPPORT != STD_OFF)
#define CANIF_START_SEC_CONST_8
#include "CanIf_MemMap.h"
CONST (uint8, CANIF_CONST) CanIf_Trcv_LUT[CANIF_NUM_TRANSCEIVERS] = {0};
#define CANIF_STOP_SEC_CONST_8
#include "CanIf_MemMap.h"
#endif


/*This hold the current config set details*/
#define CANIF_START_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"
P2CONST(CanIf_ConfigType, AUTOMATIC, AUTOMATIC) CanIf_Prv_ConfigSet_tpst;
#define CANIF_STOP_SEC_VAR_FAST_CLEARED_UNSPECIFIED
#include "CanIf_MemMap.h"


/*To hold the index of the active config set*/
#define CANIF_START_SEC_VAR_FAST_CLEARED_8
#include "CanIf_MemMap.h"
VAR(uint8, AUTOMATIC) CanIf_ActiveCfgSet;
#define CANIF_STOP_SEC_VAR_FAST_CLEARED_8
#include "CanIf_MemMap.h"

/************************************************************************/

/*Tx global variable definition*/

VAR(CanIf_ControllerStateType, CANIF_VAR) CanIf_Prv_ControllerState_ast[CANIF_TOTAL_CTRLS];

#if(CANIF_PUBLIC_TXBUFFERING == STD_ON)
VAR(CanIf_Prv_TxBufferStatus_tst, AUTOMATIC)  CanIf_Prv_TxBufferRam_ast[CANIF_TOTAL_TXBUFFERS];
VAR(CanIf_Prv_TxPduStatus_tst, AUTOMATIC)     CanIf_Prv_TxPduRam_ast[CANIF_TOTAL_TXPDUS];
#endif

#if (CANIF_PUBLIC_SETDYNAMICTXID_API != STD_OFF)
VAR(uint32, CANIF_VAR) CanIf_DynTxPduCanId_au32[CANIF_TOTAL_DYNAMIC_PDUS];
#endif


#if (CANIF_READTXPDU_NOTIFY_STATUS_API != STD_OFF)
VAR(CanIf_NotifStatusType, CANIF_VAR) CanIf_Prv_TxNotification_aen[CANIF_TOTAL_TXPDUS];
#endif

/*Rx global variable definition*/

#if (CANIF_RB_CHANGERXPDUID_API != STD_OFF)
VAR(uint32, CANIF_VAR) CanIf_DynRxPduCanId_au32[CANIF_RX_DYNAMIC_PDUS];
#endif
