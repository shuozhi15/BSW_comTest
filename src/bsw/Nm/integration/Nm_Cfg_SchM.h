/*
 * This is a template file. It defines integration functions necessary to complete RTA-BSW.
 * The integrator must complete the templates before deploying software containing functions defined in this file.
 * Once templates have been completed, the integrator should delete the #error line.
 * Note: The integrator is responsible for updates made to this file.
 *
 * To remove the following error define the macro NOT_READY_FOR_TESTING_OR_DEPLOYMENT with a compiler option (e.g. -D NOT_READY_FOR_TESTING_OR_DEPLOYMENT)
 * The removal of the error only allows the user to proceed with the building phase
 */
#ifndef NOT_READY_FOR_TESTING_OR_DEPLOYMENT
#error The content of this file is a template which provides empty stubs. The content of this file must be completed by the integrator accordingly to project specific requirements
#else
#warning The content of this file is a template which provides empty stubs. The content of this file must be completed by the integrator accordingly to project specific requirements
#endif /* NOT_READY_FOR_TESTING_OR_DEPLOYMENT */




#ifndef NM_CFG_SCHM_H
#define NM_CFG_SCHM_H

/**
 ***************************************************************************************************
 * \moduledescription
 *                      BSW Scheduler header file defining Nm Exclusive areas
 *
 * \scope               This is included only by Nm module
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "rba_BswSrv.h"

/* If the RTE code generation is Involved and if Coordinator Feature is enabled , please include the below file */
#if ((NM_ECUC_RB_RTE_IN_USE != STD_OFF) && (NM_COORDINATOR_SUPPORT_ENABLED != STD_OFF))
#include "SchM_Nm.h"
#endif
/*
 ***************************************************************************************************
 * Prototypes
 ***************************************************************************************************
 */

LOCAL_INLINE FUNC(void, NM_CODE) SchM_Enter_Nm_CallbacksNoNest(void);
LOCAL_INLINE FUNC(void, NM_CODE) SchM_Exit_Nm_CallbacksNoNest(void);

LOCAL_INLINE FUNC(void, NM_CODE) SchM_Enter_Nm_MainFunctionNoNest(void);
LOCAL_INLINE FUNC(void, NM_CODE) SchM_Exit_Nm_MainFunctionNoNest(void);


LOCAL_INLINE FUNC(void, NM_CODE) SchM_Enter_Nm_ChangeStateNoNest(void);
LOCAL_INLINE FUNC(void, NM_CODE) SchM_Exit_Nm_ChangeStateNoNest(void);


LOCAL_INLINE FUNC(void, NM_CODE) SchM_Enter_Nm_GetStateNoNest(void);
LOCAL_INLINE FUNC(void, NM_CODE) SchM_Exit_Nm_GetStateNoNest(void);
/* NOTE on Exclusive area SchM_Enter_Nm_CallbacksNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area
 *
 * */

LOCAL_INLINE FUNC(void, NM_CODE)  SchM_Enter_Nm_CallbacksNoNest(void)
{
    /*The integrator should place the code here which Disables/Locks the interrupt*/
}

LOCAL_INLINE FUNC(void, NM_CODE) SchM_Exit_Nm_CallbacksNoNest(void)
{
    /*The integrator should place the code here which Enables/Unlocks the interrupt*/
}

/* NOTE on Exclusive area SchM_Enter_Nm_MainFunctionNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area
 *
 * */

LOCAL_INLINE FUNC(void, NM_CODE) SchM_Enter_Nm_MainFunctionNoNest(void)
{
    /*The integrator should place the code here which Disables/Locks the interrupt*/
}

LOCAL_INLINE FUNC(void, NM_CODE) SchM_Exit_Nm_MainFunctionNoNest(void)
{
    /*The integrator should place the code here which Enables/Unlocks the interrupt*/
}


/* NOTE on Exclusive area SchM_Enter_Nm_ChangeStateNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area
 *
 * */

LOCAL_INLINE FUNC(void, NM_CODE) SchM_Enter_Nm_ChangeStateNoNest(void)
{
    /*The integrator should place the code here which Disables/Locks the interrupt*/
}

LOCAL_INLINE FUNC(void, NM_CODE) SchM_Exit_Nm_ChangeStateNoNest(void)
{
    /*The integrator should place the code here which Enables/Unlocks the interrupt*/
}

/* NOTE on Exclusive area SchM_Enter_Nm_ChangeStateNoNest:
 *
 * macros to suspend and resume interrupts for Exclusive area
 *
 * */

LOCAL_INLINE FUNC(void, NM_CODE) SchM_Enter_Nm_GetStateNoNest(void)
{
    /*The integrator should place the code here which Disables/Locks the interrupt*/
}

LOCAL_INLINE FUNC(void, NM_CODE) SchM_Exit_Nm_GetStateNoNest(void)
{
    /*The integrator should place the code here which Enables/Unlocks the interrupt*/
}
#endif /* _NM_CFG_SCHM_H */
