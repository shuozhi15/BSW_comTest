
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
 * Function Name: Nm_NetworkRelease

 * Description:   This is the AUTOSAR interface to release Network, this API calls either CanNm_NetworkRelease
 *                or FrNm_NetworkRelease based on the configuration.
 * Parameter:     NetworkHandle- Identification of the NM-channel
 * Return:        Std_ReturnType
 * Retval:        E_OK             - No error
 * Retval:        E_NOT_OK         - Release for bus communication has failed
 *************************************************************************************************************/
/* The API is available only if the Passive Mode is Disabled */
#if(NM_PASSIVE_MODE_ENABLED == STD_OFF)

#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(Std_ReturnType, NM_CODE) Nm_NetworkRelease(VAR(NetworkHandleType, AUTOMATIC) NetworkHandle)
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr;
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;


    VAR(Std_ReturnType, AUTOMATIC) RetVal;
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    RetVal = E_NOT_OK;

    /* process only if the channel handle is within allowed range */
    if (Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS)
    {
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];

        #if (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF)
        FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];

        RetVal = (*FuncPtr->BusNm_NetworkRelease)(NetworkHandle);
#else
        if(ConfDataPtr->ClusterIdx != 0xFF)
        {
            /*call internal network release to handle the shutdown and channel state change accordingly*/
            RetVal = Nm_Int_NetworkRelease(NetworkHandle,NM_EXTERNAL_REQUEST);
        }
        /*channel is not a part of any cluster*/
        else
        {
            FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];
            RetVal = (*FuncPtr->BusNm_NetworkRelease)(NetworkHandle);
        }
        #endif

    }
    else
    {
        /* Report to DET as the network handle parameter is not a configured network handle */
        NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_NETWORKRELEASE, NM_E_HANDLE_UNDEF);
    }


    return (RetVal);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"

#endif /* Passive Mode Switch */

