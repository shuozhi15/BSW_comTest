
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


/*************************************************************************************************************
 * Function
 *************************************************************************************************************
 *************************************************************************************************************
 * Function Name: Nm_GetNodeIdentifier

 * Description:   This function is used to get the node identifier out of the last NM message received on the bus.
 * Parameter1:    NetworkHandle   - Identification of the NM-channel
 * Parameter2:    nmNodeIdPtr     - Pointer where node identifier out of the last successfully
 *                                  received NM message shall be copied to
 * Return:        Std_ReturnType
 * Retval:        E_OK         - Getting of node identifier is successful
 * Retval:        E_NOT_OK     - Getting of node identifier has failed
 ************************************************************************************************************/

/* This function is only available if NmNodeIdEnabled is set to TRUE */
#if(NM_NODE_ID_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(Std_ReturnType, NM_CODE) Nm_GetNodeIdentifier(
                                                  VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
                                                  P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmNodeIdPtr
                                                 )
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr; /* Configuration pointer holds referrence of configuration data */
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;     /* Pointer to Bus specific APIs */
    VAR(Std_ReturnType, AUTOMATIC) RetVal = E_NOT_OK;                /* Return value */
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;              /* Network handle is received from ComM */
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    /* process only if the channel handle is within allowed range and the Node Id pointer is valid */
    if ((Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS) && (nmNodeIdPtr != NULL_PTR))
    {
        /* Configuration pointer initialization */
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
        /* Holds the reference to Bus specific APIs */

        if(ConfDataPtr->NmNodeIdEnabled)
        {
	        FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];

	        /* Calls the Bus specific API */
	        RetVal = (*FuncPtr->BusNm_GetNodeIdentifier)(NetworkHandle, nmNodeIdPtr);
        }
    }
    else
    {

        if (!(Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS))
        {
            /* Report to DET as the network handle parameter is not a configured network handle */
             NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_GETNODEIDENTIFIER, NM_E_HANDLE_UNDEF);
        }
        else
        {
            //do nothing
        }
        if (nmNodeIdPtr == NULL_PTR)
        {
            /* Report to DET as the parameter passed is  a null pointer*/
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_GETNODEIDENTIFIER, NM_E_PARAM_POINTER);
        }
        else
        {
            //do nothing
        }


        /* Reject the call: either Nm_NetworkHandle is not within allowed range or nodeId pointer is invalid */
        RetVal = E_NOT_OK;
    }


    return(RetVal);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

