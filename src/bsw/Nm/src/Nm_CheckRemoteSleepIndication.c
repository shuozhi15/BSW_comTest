
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
 * Function Name: Nm_CheckRemoteSleepIndication

 * Description:   This AUTOSAR interface gets the status of remote sleep detection
 * Parameter1:    NetworkHandle- Identification of the NM-channel
 * Parameter2:    nmRemoteSleepIndPtr - Pointer to the location where the check result of remote sleep
 *                                      indication shall be copied.
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Checking of remote sleep indication bits has failed
 *************************************************************************************************************/


/* This function is only available if NmRemoteSleepIndEnabled is set to TRUE */
#if (NM_REMOTE_SLEEP_IND_ENABLED != STD_OFF)
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(Std_ReturnType, NM_CODE) Nm_CheckRemoteSleepIndication(
                                                VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
                                                P2VAR(boolean, AUTOMATIC, NM_APPL_DATA) nmRemoteSleepIndPtr)
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;
    VAR(Std_ReturnType, AUTOMATIC)  RetVal;
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    /* process only if the channel handle is within allowed range and the nmRemoteSleepIndPtr pointer is valid */
    if ((Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS) && (nmRemoteSleepIndPtr != NULL_PTR))
    {
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
        FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];

        /* Calls the Bus specific API */
        RetVal = (*FuncPtr->BusNm_CheckRemoteSleepIndication)(NetworkHandle, nmRemoteSleepIndPtr);
    }
    else
    {
        if (!(Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS))
        {
            /* Report to DET as the network handle parameter is not a configured network handle */
             NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_CHECKREMOTESLEEPINDICATION, NM_E_HANDLE_UNDEF);
        }
        else
        {
            //do nothing
        }
        if (nmRemoteSleepIndPtr == NULL_PTR)
        {
            /* Report to DET as the parameter passed is  a null pointer*/
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_CHECKREMOTESLEEPINDICATION, NM_E_PARAM_POINTER);
        }
        else
        {
            //do nothing
        }


        RetVal = E_NOT_OK;
    }


    return(RetVal);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

