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



/* this template has to be used when generation of module specific mem map files is disabled during RTE generation*/
/* Integrator has to map these sections to appropriate memmap sections*/
/* MR12 DIR 4.10 VIOLATION: MemMap header concept - no protection against repeated inclusion intended. */

#if defined ComM_START_SEC_CODE
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef, AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef ComM_START_SEC_CODE
    /* Integrator has to Map it with the configured value */

#elif defined ComM_STOP_SEC_CODE
/* MR12 RULE 20.5 VIOLATION: AUTOSAR MemMap concept requires #undef, AUTOSAR MemMap requirements are incompatible to MISRA */
    #undef ComM_STOP_SEC_CODE
    /* Integrator has to Map it with the configured value */
#endif

#include "MemMap.h"


