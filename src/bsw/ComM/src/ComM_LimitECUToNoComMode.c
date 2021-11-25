


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComM_Priv.h"

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */
/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */


#if (COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

#define COMM_START_SEC_CODE
#include "ComM_Cfg_MemMap.h"


/*******************************************************************************
 * Function name    : ComM_LimitECUToNoComMode
 * Syntax           : Std_ReturnType ComM_LimitChannelToNoComMode( NetworkHandleType Channel, ComM_StateType* State )
 * Description      : API to enable or disable limit to no com for the ECU.
 * Parameters       : Channel-> The Network Channel for the requested state of ComM state machine.
 *                    Status-> Status of limit to no com
 * Return value     : E_OK: Successfully changed inhibition status for the channel
 *                    E_NOT_OK: Changed of inhibition status for the channel failed
 *                    COMM_E_UNINIT: ComM not initialized
 ******************************************************************************/

FUNC(Std_ReturnType, COMM_CODE) ComM_LimitECUToNoComMode
                                (
                                    VAR(boolean, COMM_VAR) Status
                                )
{
    /* Local variable declaration */

    uint8 channelId_u8;
    VAR(Std_ReturnType, AUTOMATIC) ret_val;
    P2VAR(ComM_ChannelVarType, AUTOMATIC, COMM_APPL_DATA) channelRamPtr_ps;
    P2VAR(ComM_GlobalVarType, AUTOMATIC, COMM_APPL_DATA) globalRamPtr_ps;

    /* Local variable initialization */

    ret_val = E_OK;

    /************************************DET CHECK STARTS HERE*********************************/
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    /* Check for ComM Initialized */
    if (ComM_GlobalStruct.ComM_InitStatus != COMM_INIT)
    {
        (void)Det_ReportError(COMM_MODULE_ID, COMM_INSTANCE_ID, COMM_LIMIT_CHNLTO_NOCOMMODE_APIID, COMM_E_NOT_INITED);
        return E_NOT_INITIALIZED;
    }
    #endif  /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */

    /************************************DET CHECK ENDS HERE**********************************/

    /* Proceed only if the request has changed from previous request */
    globalRamPtr_ps = &ComM_GlobalStruct;

    if((globalRamPtr_ps->ComM_LimitECUToNoCom_b) != Status)
    {
        /* Update the global limit ecu to no com status */
        globalRamPtr_ps->ComM_LimitECUToNoCom_b = Status;

        /* If limit ECU to no com is requested */
        if(Status != FALSE)
        {
            /* Set limit to no com for all channels */
            /* MR12 RULE 14.2 VIOLATION: The variable incremented in the third expression of this 'for' statement is actually the variable identified as the 'loop control variable.
                               * Hence it is an incorrect warning reported by Misra */
            for(channelId_u8 = C_ZERO, channelRamPtr_ps = &ComM_ChannelStruct[C_ZERO]; channelId_u8 < COMM_NO_OF_CHANNELS; ++channelId_u8)
            {
               /*Internal funtion to set Limit to No Com for Channel based on the Current state is invoked*/
                ret_val |= ComM_LProcessLimitToNoCom (channelId_u8, TRUE);
                ++channelRamPtr_ps;
            }
        }
        /* If limit ECU to no com is released */
        else
        {
            /* Clear limit to no com for all channels */
            /* MR12 RULE 14.2 VIOLATION: The variable incremented in the third expression of this 'for' statement is actually the variable identified as the 'loop control variable.
                   * Hence it is an incorrect warning reported by Misra */
            for(channelId_u8 = C_ZERO, channelRamPtr_ps = &ComM_ChannelStruct[channelId_u8]; channelId_u8 < COMM_NO_OF_CHANNELS; channelId_u8++)
            {
                /* Internal function to Clear limit to no com flag for all configured channels*/
                (void)ComM_LProcessLimitToNoCom (channelId_u8, FALSE);
                channelRamPtr_ps++;
            }
        }
    }

    return ret_val;
}

#define COMM_STOP_SEC_CODE
#include "ComM_Cfg_MemMap.h"

#endif  /*  (COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)  */

