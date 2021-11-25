
/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

/* NM Interface private header file, this file is included only by Nm module */
#include "Nm_Priv.h"

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/***************************************************************************************************
 * Function
 ***************************************************************************************************
 ***************************************************************************************************
 * Function Name: Nm_GetState

 * Description:   This is the AUTOSAR interface to get the current state of NM,
 *                this API calls either CanNm_GetState or FrNm_GetState based on the configuration.
 *
 * Parameter1:    NetworkHandle - Identification of the NM-channel
 * Parameter2:    nmStatePtr    - Pointer to the location where the state of the network
 *                                        management shall be copied.
 * Parameter3:    nmModePtr     - Pointer to the location where the mode of the network
 *                                        management shall be copied.
 * Return:        Std_ReturnType
 * Retval:        E_OK         - No error
 * Retval:        E_NOT_OK     - Getting of NM state has failed
 ****************************************************************************************************/

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(Std_ReturnType, NM_CODE) Nm_GetState(
                                         VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
                                         P2VAR(Nm_StateType, AUTOMATIC, NM_APPL_DATA) nmStatePtr,
                                         P2VAR(Nm_ModeType, AUTOMATIC, NM_APPL_DATA) nmModePtr
                                        )
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr; /* Configuration pointer holds referrence of configuration data */
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;     /* Pointer to Bus specific APIs */
    VAR(Std_ReturnType, AUTOMATIC)  RetVal;                           /* Return value */
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;              /* Network handle is received from ComM */
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    /* process only if the channel handle is within allowed range and the data pointer is valid */
    if ((Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS) && (nmStatePtr != NULL_PTR) && (nmModePtr != NULL_PTR))
    {
        /* Configuration pointer initialization */
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
        /* Holds the reference to Bus specific APIs */
        FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];

        /* Calls the Bus specific API */
        RetVal = (*FuncPtr->BusNm_GetState)(NetworkHandle,nmStatePtr,nmModePtr);
    }
    else
    {
        if (!(Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS))
        {
            /* Report to DET as the network handle parameter is not a configured network handle */
             NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_GETSTATE, NM_E_HANDLE_UNDEF);
        }
        else
        {
            //do nothing
        }
        if ((nmStatePtr == NULL_PTR) || (nmModePtr == NULL_PTR))
        {
            /* Report to DET as the parameter passed is  a null pointer*/
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_GETSTATE, NM_E_PARAM_POINTER);
        }
        else
        {
            //do nothing
        }


        /* reject the call as pointers passed are not valid */
        RetVal = E_NOT_OK;
    }


    return(RetVal);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

