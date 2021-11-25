

#ifndef CANNM_CBK_H
#define CANNM_CBK_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm.h"

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/

/* APIs called by Can Interface module */

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(void, CANNM_CODE) CanNm_RxIndication(
                                                    VAR(PduIdType, AUTOMATIC) RxPduId,
                                                    P2CONST(PduInfoType, AUTOMATIC, CANNM_APPL_DATA) PduInfoPtr
                                                );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(void, CANNM_CODE) CanNm_TxConfirmation( VAR(PduIdType, AUTOMATIC) TxPduId );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#endif /* CANNM_CBK_H */


