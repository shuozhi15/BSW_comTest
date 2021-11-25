
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
 * Function Name: Nm_PassiveStartUp

 * Description:   This is the AUTOSAR interface for triggering passive start up of NM, this API calls either
 *                CanNm_PassiveStartUp or FrNm_PassiveStartUp based on the configuration
 * Parameter:     NetworkHandle- Identification of the NM-channel
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Passive start of network management has failed
 *************************************************************************************************************/


#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(Std_ReturnType, NM_CODE) Nm_PassiveStartUp(
                                               VAR(NetworkHandleType, AUTOMATIC) NetworkHandle
                                              )
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;


    //todo: handle RetVal in all APIs properly
    VAR(Std_ReturnType, AUTOMATIC) RetVal;
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    RetVal = E_NOT_OK;
    /* process only if the channel handle is within allowed range */
    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        /* Configuration pointer initialization */
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];
        /* Holds the reference to Bus specific APIs */
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF)
        FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];

        /* Calls the Bus specific API*/
        RetVal = (*FuncPtr->BusNm_PassiveStartUp)(NetworkHandle);
#else
        if(ConfDataPtr->ClusterIdx != 0xFF)
        {
            /*call internal network request to call bus specific passive startup and to handle channel state change accordingly*/
            RetVal = Nm_Int_NetworkRequest(FALSE,NetworkHandle,NM_EXTERNAL_REQUEST);
        }
        /*channel is not a part of any cluster*/
        else
        {
            FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];
            RetVal = (*FuncPtr->BusNm_PassiveStartUp)(NetworkHandle);
        }
        #endif
    }

    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_PASSIVESTARTUP, NM_E_HANDLE_UNDEF);
    }

    return(RetVal);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

