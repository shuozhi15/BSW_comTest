
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

/**************************************************************************************************************
 * Function
 **************************************************************************************************************
 **************************************************************************************************************
 * Function Name: Nm_RepeatMessageRequest

 * Description:   This AUTOSAR interface causes Repeat Message Request to be transmitted next on the bus
 * Parameter:     NetworkHandle- Identification of the NM-channel
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Repeat Message Request has failed
 *************************************************************************************************************/

/* This function is only available if NmNodeDetectionEnabled is TRUE */
#if (NM_NODE_DETECTION_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(Std_ReturnType, NM_CODE) Nm_RepeatMessageRequest(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle)
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr; /* Configuration pointer holds referrence of configuration data */
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;     /* Pointer to Bus specific APIs */
    VAR(Std_ReturnType, AUTOMATIC)  RetVal = E_NOT_OK;                            /* Return value */
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;              /* Network handle is received from ComM */

    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    /* process only if the channel handle is within allowed range */
    if ((Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS))
    {
        /* Configuration pointer initialization */
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
        /* Holds the reference to Bus specific APIs */
        if(ConfDataPtr->NmNodeDetectionEnabled)
        {
	        FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];

	        /* Calls the Bus specific API */
	        RetVal = (*FuncPtr->BusNm_RepeatMessageRequest)(NetworkHandle);
        }
    }
    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_REPEATMESSAGEREQUEST, NM_E_HANDLE_UNDEF);


        /* reject the call: Nm_NetworkHandle is not within allowed range */
        RetVal = E_NOT_OK;
    }


    return(RetVal);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

