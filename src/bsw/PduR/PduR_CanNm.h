
#ifndef PDUR_CANNM_H
#define PDUR_CANNM_H

#include "PduR.h"

#include "CanNm.h"
#if (!defined(CANNM_AR_RELEASE_MAJOR_VERSION) || (CANNM_AR_RELEASE_MAJOR_VERSION != PDUR_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANNM_AR_RELEASE_MINOR_VERSION) || (CANNM_AR_RELEASE_MINOR_VERSION != PDUR_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif


/* Interface translation layers ------------------------------------- */

#define PduR_RF_CanNm_Transmit_Func   CanNm_Transmit

/* PduR_CanNmRxIndication  */
extern FUNC( void, PDUR_CODE ) PduR_CanNmRxIndication( VAR(PduIdType, AUTOMATIC) id, P2CONST( PduInfoType, TYPEDEF, PDUR_APPL_DATA ) ptr );

/* PduR_CanNmTxConfirmation  */
extern FUNC( void, PDUR_CODE ) PduR_CanNmTxConfirmation( VAR(PduIdType, AUTOMATIC) id );

/* PduR_CanNmTriggerTransmit  */
extern FUNC( Std_ReturnType, PDUR_CODE ) PduR_CanNmTriggerTransmit( VAR(PduIdType, AUTOMATIC) id, P2VAR( PduInfoType, TYPEDEF, PDUR_APPL_DATA ) ptr );

#endif /* PDUR_CANNM_H    */
