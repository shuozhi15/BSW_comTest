


/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComM_Priv.h"
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */
/*
 ***************************************************************************************************
 * Type definitions
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */
#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)
LOCAL_INLINE FUNC(void, COMM_CODE) ComM_LUpdateNvMInhibitionStatus(uint8 ChannelId, uint8 InhibitionStatus, P2VAR(uint8, AUTOMATIC,COMM_APPL_DATA) NvM_InhibitionArray);
#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */

#define COMM_START_SEC_CODE
#include "ComM_Cfg_MemMap.h"


/*******************************************************************************
 * Function name    : ComM_DeInit
 * Syntax           : void ComM_DeInit(void)
 * Description      : De-initializes the AUTOSAR Communication Manager.
 * Parameters       : void
 * Return value     : void
 ******************************************************************************/
FUNC(void,COMM_CODE) ComM_DeInit(void)
{
    /* Local Variables Declaration */
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    #endif  /*  #if (COMM_DEV_ERROR_DETECT != STD_OFF)  */
    VAR(uint8 ,AUTOMATIC)  ChannelIndexCtr;
    P2VAR(ComM_GlobalVarType, AUTOMATIC, COMM_APPL_DATA) globalRamPtr_ps;

#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)
    VAR(Std_ReturnType, AUTOMATIC) Ret_NvM_Writeval_u8;
    VAR(uint8,AUTOMATIC) ChannelId;
    P2VAR(ComM_ChannelVarType, AUTOMATIC, COMM_APPL_DATA) channelRamPtr_ps;
#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */

    globalRamPtr_ps = &ComM_GlobalStruct;

    /* Local variable initialization */
    /* DET check for communication manger initialization */
    #if (COMM_DEV_ERROR_DETECT != STD_OFF)
    /* check if communication manager in initialized. Else report DET */
    if (/* check if communication manager in initialized.
        Else report DET */
    globalRamPtr_ps->ComM_InitStatus != COMM_INIT)
    {
        /* Report DET with ComM not initialised status*/
        (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_DEINIT_APIID,COMM_E_NOT_INITED);
        return;
    }
    else
    {
        /* do nothing */
    }
    #endif  /*  #if (COMM_DEV_ERROR_DETECT != STD_OFF)  */

    /* Set the init status as Uninit */
    /* De-initializes (terminates) the AUTOSAR Communication Manager*/
    globalRamPtr_ps->ComM_InitStatus = COMM_UNINIT;
    for (ChannelIndexCtr = C_ZERO ; ChannelIndexCtr < COMM_NO_OF_CHANNELS ; ChannelIndexCtr++)
    {
        /* loop thro all the ComM channel,and if any of the channel is found to
        be not in NO_COMMUNICATION state then deinit call is ignored */
        /* Check if the current channel mode of all the channel is No communication .
        If yes change the INIT status as Uninit */
        if (ComM_ChannelStruct[ChannelIndexCtr].ChannelState_e != COMM_NO_COM_NO_PENDING_REQUEST)
        {
            /*  Initialize the status if any one of the channel is not in No communication mode and break it */
            globalRamPtr_ps->ComM_InitStatus = COMM_INIT;
            break;
        }
    }

#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)

        /*Storing ComMEcuGroupClassification and  Inhibit counter values on to NvM*/
        ComM_NvMblock.ComMEcuGroupClassification_u8 = ComM_GlobalStruct.ComM_EcuGroupClassification_u8;
        ComM_NvMblock.Inhibitioncounter_u16 = ComM_GlobalStruct.ComM_InhibitCounter_u16;

        /*Loop through all Channel to store the ComMNoWakeup value to NvM */
        /* MR12 RULE 14.2 VIOLATION: The variable incremented in the third expression of this 'for' statement is actually the variable identified as the 'loop control variable.
               * Hence it is an incorrect warning reported by Misra */

        for (ChannelId = C_ZERO; ChannelId < COMM_NO_OF_CHANNELS ;++ChannelId)
        {
            /*MR12 RULE 10.3 VIOLATION:  An expression of 'essentially Boolean' type (_Bool) is being converted to unsigned type, 'unsigned char' on assignment.
            * This is a Incorrect Misra Warning. This is taken care in review.*/
            channelRamPtr_ps = &ComM_ChannelStruct[ChannelId];
            ComM_LUpdateNvMInhibitionStatus(ChannelId, COMM_GET_PREVENT_WAKEUP(channelRamPtr_ps->InhibitionReqStatus_u8), ComM_NvMblock.ComMNoWakeup_u8);
        }

        /*MR12 DIR 1.1 VIOLATION: NvM_WriteBlock's NvM_SrcPtr return value should be void as per Autosar guidelines.*/
        Ret_NvM_Writeval_u8 = NvM_WriteBlock(COMM_NVM_BLOCK_REFERENCE,&ComM_NvMblock);

        /* Control shouldn't come here.If at all happens, report to DET */
        #if (COMM_DEV_ERROR_DETECT != STD_OFF)
        if(Ret_NvM_Writeval_u8 != E_OK)
        {
            (void)Det_ReportError(COMM_MODULE_ID,COMM_INSTANCE_ID,COMM_NVM_REFERENCE_BLOCK,COMM_E_NVM_WRITE_FAILED);
            return;
        }
        #endif   /*  (COMM_DEV_ERROR_DETECT != STD_OFF)  */

#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */

        return;


}

#define COMM_STOP_SEC_CODE
#include "ComM_Cfg_MemMap.h"

/*****************************************************************************************************
 * Function name    : ComM_LUpdateNvMInhibitionStatus
 * Syntax           : ComM_LUpdateNvMInhibitionStatus
 *                    (
 *                      uint8 ChannelId, uint8 InhibitionStatus, P2VAR(uint8, AUTOMATIC,COMM_APPL_DATA) NvM_InhibitionArray
 *                    )
 * Description      : Internal function to store the NvM ComMNoWakeup value in an array.(Each bit constitutes the value of ComMNoWakeup)
 * Parameters       : ChannelId -> Channel handle Id
 *                    InhibitionStatus -> Staus taken from the config structure.
 *                  : NvM_InhibitionArray -> Array which stores ComMNoWakeup value .
 * Return value     : void
 *
 ****************************************************************************************************/
#if(COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)

LOCAL_INLINE FUNC(void, COMM_CODE) ComM_LUpdateNvMInhibitionStatus
                                    (
                                      uint8 ChannelId, uint8 InhibitionStatus, P2VAR(uint8, AUTOMATIC,COMM_APPL_DATA) NvM_InhibitionArray
                                    )

{
    /* Local variable declaration*/
    uint8 mask;
    uint8 byteIndex;
    uint8 bitPosition;

    /* Local variable initialization*/
    byteIndex = (ChannelId) >> 3;
    bitPosition = (ChannelId) % 8 ;

    if(InhibitionStatus == 0x01)
    {
        /*If the InhibitionStautus is 1 then set the bit*/
         mask = ((uint8)1 << bitPosition);
        *(NvM_InhibitionArray+byteIndex) = (uint8)(*(NvM_InhibitionArray+byteIndex) | (mask)) ;
    }
    else
    {
        /*If the InhibitionStautus is 0 then reset the bit*/
         mask = (~ ((uint8)1 << bitPosition));
        *(NvM_InhibitionArray+byteIndex) = (uint8)(*(NvM_InhibitionArray+byteIndex) & (mask));
    }

}
#endif  /* (COMM_INHIBITION_ENABLED) && (COMM_NVM_ENABLED)  */



