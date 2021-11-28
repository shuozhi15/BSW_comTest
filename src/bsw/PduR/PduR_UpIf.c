


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: PduR  / AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/



#include "PduR_Prv.h"
/* Appropriate header files are included to declare the prototypes
 */
#include "PduR_UpIf.h"

#if defined(PDUR_MULTICAST_TO_IF_SUPPORT) && (PDUR_MULTICAST_TO_IF_SUPPORT == 1)
#include "PduR_Mc.h"
#endif


/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_ComTransmit
 *      This function is called by the COM to request a transmission.
 *
 * \param           PduIdType id: ID of COM I-PDU to be transmitted.
 *                  P2CONST( PduInfoType, TYPEDEF, PDUR_APPL_DATA ) ptr: Pointer to a structure with I-PDU related data that shall be transmitted:
 *                                         data length and pointer to I-SDU buffer
 *
 * \retval          E_OK Transmit request has been accepted
 *                  E_NOT_OK Transmit request has not been accepted
 *
 * \seealso         PDUR202, PDUR206
 * \usedresources
 **************************************************************************************************
 */

FUNC( Std_ReturnType, PDUR_CODE ) PduR_ComTransmit( VAR(PduIdType, AUTOMATIC) id, P2CONST( PduInfoType, TYPEDEF, PDUR_APPL_DATA ) ptr)
{
    return ((Std_ReturnType)PduR_rComTransmit( (id) , (ptr)));
}

/* ------------------------------------------------------------------------ */
/* End section for code */
#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"






