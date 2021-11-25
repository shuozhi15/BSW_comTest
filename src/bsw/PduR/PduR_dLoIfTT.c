


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: PduR  / AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/



#include "PduR_Prv.h"

#include "PduR_Cfg.h"
/* Appropriate header files are included to declare the prototypes
 */
#include "PduR_LoIfTT.h"

/* ------------------------------------------------------------------------ */
/* Begin section for code */
#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_CanNmRxIndication
 *    This function is called by the CanNm after a FlexRay L-PDU has been received.
 *
 *
 * \param           PduIdType Id    : ID of FlexRay L-PDU that has been received.
 *                  const uint8 *ptr: Pointer to FlexRay SDU (buffer of received payload)
 *
 * \retval          None
 * \seealso         PDUR195
 * \usedresources
 **************************************************************************************************
 */

FUNC( void, PDUR_CODE ) PduR_CanNmRxIndication( VAR(PduIdType, AUTOMATIC) id, P2CONST( PduInfoType, TYPEDEF, PDUR_APPL_DATA ) ptr)
{
    PduR_rCanNmRxIndication((id), (ptr));
}


/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"


/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_CanNmTxConfirmation - This function is called by the CanNm after the PDU has been transmitted on the
 *                           network.
 *
 *
 * \param           PduIdType id -  ID of  L-PDU that has been transmitted.
 *                                  Range: 0..(maximum number of L-PDU IDs which may be transmitted by
 *                                             Interface) - 1
 *
 *
 * \retval          None
 * \seealso         PDUR196
 * \usedresources
 **************************************************************************************************
 */

FUNC( void, PDUR_CODE ) PduR_CanNmTxConfirmation( VAR(PduIdType, AUTOMATIC) id)
{
    PduR_rCanNmTxConfirmation(id);
}


/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"


/* ------------------------------------------------------------------------ */
/* Begin section for code */

#define PDUR_START_SEC_CODE
#include "PduR_MemMap.h"

/**
 **************************************************************************************************
 * PduR_CanNmTriggerTransmit - This function is called by the CanNm for sending out a  frame.
 * The trigger transmit is initiated by the  schedule. Whether this function is called or not is statically
 * configured for each PDU.
 *
 *
 * \param           PduIdType id -  ID of FlexRay L-PDU that is requested to be transmitted.
 *                                  Range: 0..(maximum number of L-PDU IDs which may be transmitted by FlexRay
 *                                             Interface) - 1
 *
 *                  uint8 *  ptr - Pointer to place inside the transmit buffer of the L-PDU where data shall be copied
 *                                 to.
 *
 * \retval          None
 * \seealso         PDUR199
 * \usedresources
 **************************************************************************************************
 */

FUNC( Std_ReturnType, PDUR_CODE ) PduR_CanNmTriggerTransmit( VAR(PduIdType, AUTOMATIC) id, P2VAR( PduInfoType, TYPEDEF, PDUR_APPL_DATA ) ptr)
{
    return ((Std_ReturnType)PduR_rCanNmTriggerTransmit((id), (ptr)));
}


/* ------------------------------------------------------------------------ */
/* End section for code */

#define PDUR_STOP_SEC_CODE
#include "PduR_MemMap.h"





