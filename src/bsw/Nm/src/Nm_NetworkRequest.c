
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
 * Function Name: Nm_NetworkRequest

 * Description:   This is the AUTOSAR interface to request Network. This API calls either CanNm_NetworkRequest
 *                or FrNm_NetworkRequest based on the configuration
 * Parameter:     NetworkHandle- Identification of the NM-channel
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Request for bus communication has failed
 *************************************************************************************************************/
/* The API is available only if the Passive Mode is Disabled */
#if(NM_PASSIVE_MODE_ENABLED == STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(Std_ReturnType, NM_CODE) Nm_NetworkRequest(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle)
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST) ConfDataPtr; /* Configuration pointer holds referrence of configuration data */
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST) FuncPtr;   /* Pointer to Bus specific APIs */

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
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF)
        /* Holds the reference to Bus specific APIs */
        FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];

        RetVal = (*FuncPtr->BusNm_NetworkRequest)(NetworkHandle);
#else
        if(ConfDataPtr->ClusterIdx != 0xFF)
        {
            /*call internal network request to call bus Specific network request and to handle  channel state change accordingly*/
            RetVal = Nm_Int_NetworkRequest(TRUE,NetworkHandle,NM_EXTERNAL_REQUEST);
        }
        /*channel is not a part of any cluster*/
        else
        {
            FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];
            RetVal = (*FuncPtr->BusNm_NetworkRequest)(NetworkHandle);
        }
#endif
    }

    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_NETWORKREQUEST, NM_E_HANDLE_UNDEF);
    }

    return(RetVal);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif /* Passive Mode Switch */

