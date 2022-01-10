

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanNm_Prv.h"

/*
 ***************************************************************************************************
 Function name    : CanNm_ChangeState
 Description      : This is an internal function of CanNm. This function changes the state and mode
                    of CanNm without data-inconsistency
 Parameter        : RamPtr_ps  - pointer to the RAM data structure of the current channel
                  : nmState_en - state to be changed to
                  : nmMode_en  - mode to be changed to
 Return value     : None
 ***************************************************************************************************
 */

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"

FUNC(void, CANNM_CODE) CanNm_ChangeState(
                                          P2VAR(CanNm_RamType, AUTOMATIC, CANNM_APPL_DATA) RamPtr_ps,
                                          VAR(Nm_StateType, AUTOMATIC) nmState_en,
                                          VAR(Nm_ModeType, AUTOMATIC) nmMode_en
                                        )
{

    /* Protect from interrupts to ensure data consistency between state and mode updates */
    SchM_Enter_CanNm_ChangeStateNoNest();

    /* Update state and mode of the corresponding channel with the passed state and mode */
    RamPtr_ps->State_en = nmState_en;
    RamPtr_ps->Mode_en = nmMode_en;

    /* Unprotect */
    SchM_Exit_CanNm_ChangeStateNoNest();

    return;
}

#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


