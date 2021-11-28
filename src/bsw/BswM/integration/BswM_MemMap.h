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



/*********************************************************************************************************************
* CONTAINS THE BSWM-MEMMAP SEC CODE DECLARATIONS
*********************************************************************************************************************/

#if defined BswM_START_SEC_CODE
   /* For SWC memmap mapping, Integrator has to Map it with the configured value */
    #undef BswM_START_SEC_CODE
#elif defined BswM_STOP_SEC_CODE
   /* For SWC memmap mapping, Integrator has to Map it with the configured value */
    #undef BswM_STOP_SEC_CODE

#elif defined BSWM_START_SEC_CODE
    #define  BSW_START_SEC_CODE
    #include "Bsw_MemMap.h"
    #undef BSWM_START_SEC_CODE
#elif defined BSWM_STOP_SEC_CODE
    #define  BSW_STOP_SEC_CODE
    #include "Bsw_MemMap.h"
    #undef BSWM_STOP_SEC_CODE
#endif


/*
 **********************************************************************************************************************
 * End of header file *
 **********************************************************************************************************************
*/
