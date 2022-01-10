
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
 * Function Name: Nm_GetPduData

 * Description:   This is the AUTOSAR interface for getting the whole NM PDU out of the most recently received
 *                NM message, this API calls either CanNm_GetPduData or FrNm_GetPduData based on the channel
 *                handle
 * Parameter1:    NetworkHandle   - Identification of the NM-channel
 * Parameter2:    nmPduData       - Pointer where whole Pdu data out of the most recently received NM
 *                                  message shall be copied to.
 * Return:        Std_ReturnType
 * Retval:        E_OK         - No error
 * Retval:        E_NOT_OK     - Getting of PDU has failed
 ************************************************************************************************************/

#if((NM_NODE_ID_ENABLED != STD_OFF) || (NM_USER_DATA_ENABLED != STD_OFF))
#define NM_START_SEC_CODE
#include "Nm_MemMap.h"
FUNC(Std_ReturnType, NM_CODE) Nm_GetPduData(
                                           VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
                                           P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmPduData
                                          )
{
    P2CONST(Nm_ConfigType,AUTOMATIC,NM_APPL_CONST)      ConfDataPtr; /* Configuration pointer holds referrence of configuration data */
    P2CONST(Nm_BusNmApiType,AUTOMATIC,NM_APPL_CONST)    FuncPtr;     /* Pointer to Bus specific APIs */
    VAR(Std_ReturnType, AUTOMATIC)  RetVal = E_NOT_OK;                           /* Return value */
    VAR(NetworkHandleType, AUTOMATIC) Nm_NetworkHandle;              /* Network handle is received from ComM */
    /**** End of declarations ****/

    /* Receive the Internal NmChannel structure index from the received ComM NetworkHandle*/
    Nm_NetworkHandle = NM_GET_HANDLE(NetworkHandle);

    /* process only if the channel handle is within allowed range and the data pointer is valid */
    if ((Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS) && (nmPduData != NULL_PTR))
    {
        /* Configuration pointer initialization */
        ConfDataPtr = &Nm_ConfData_cs[Nm_NetworkHandle];


        /* Holds the reference to Bus specific APIs */
		if((NM_USER_DATA_ENABLED != STD_OFF) || ((ConfDataPtr->NmNodeIdEnabled) == TRUE))
		{
			FuncPtr = &Nm_BusNmApi[ConfDataPtr->BusNmType];

	        /* Calls the Bus specific API */
	        RetVal = (*FuncPtr->BusNm_GetPduData)(NetworkHandle, nmPduData);
	    }
    }
    else
    {
        if (!(Nm_NetworkHandle < NM_NUMBER_OF_CHANNELS))
        {
            /* Report to DET as the network handle parameter is not a configured network handle */
             NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_GETPDUDATA, NM_E_HANDLE_UNDEF);
        }
        else
        {
            //do nothing
        }
        if (nmPduData == NULL_PTR)
        {
            /* Report to DET as the parameter passed is  a null pointer*/
            NM_DET_REPORT_ERROR(NetworkHandle, NM_SID_GETPDUDATA, NM_E_PARAM_POINTER);
        }
        else
        {
            //do nothing
        }


        /* reject the call: either Nm_NetworkHandle is not within allowed range or data pointer is invalid */
        RetVal = E_NOT_OK;
    }


    return(RetVal);
}
#define NM_STOP_SEC_CODE
#include "Nm_MemMap.h"
#endif

