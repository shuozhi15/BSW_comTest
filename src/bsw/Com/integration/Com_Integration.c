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



/*
 * This file includes application functions that are called by Com
 * A dummy implementation is provided here. User has to implement the actual functionality
 * */

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/

#include "Com_Prv.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/
#if (COM_CONFIGURATION_VARIANT == COM_VARIANT_PRE_COMPILE)
#define COM_START_SEC_CONST_UNSPECIFIED
#include "Com_MemMap.h"
/***************************************************************************************************
 Description      : ComConfig - configuration structure is created for variant PRE_COMPILE
					It supports users keep BswM configuration as Com_Init(&ComConfig) when changing between variants.
 ***************************************************************************************************
 */
CONST(Com_ConfigType, AUTOMATIC) ComConfig = {
    NULL_PTR,
    NULL_PTR
};
#define COM_STOP_SEC_CONST_UNSPECIFIED
#include "Com_MemMap.h"

#endif


