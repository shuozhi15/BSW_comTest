

/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * Generator__: ComM / AR42.5.4.0                Module Package Version
 * Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * Model______: 2.3.0.4             ECU Parameter Definition Version
 *
 </VersionHead>*/


/***********************************************************************************/
/*                                        INCLUDES                                 */
/***********************************************************************************/
#include "ComStack_Types.h"
#include "ComM_Cfg.h"
#include "ComM_Priv.h"
#include "Std_Types.h"

#if(COMM_CAN == STD_ON)
#include "CanSM_ComM.h"
#endif
#if(COMM_LIN == STD_ON)
#include "LinSM.h"
#endif
#if(COMM_FLXRY == STD_ON)
#include "FrSM.h"
#endif
#if(COMM_ETH == STD_ON)
#include "EthSM.h"
#endif

#if(COMM_RTE_SUPPORT != STD_OFF)
/*****************************************Start of RTE API's*******************************************/
#define COMM_START_SEC_CODE
#include "ComM_Cfg_MemMap.h"

FUNC(void, COMM_CODE) ComM_Rte_Switch_UM_currentMode(VAR(ComM_UserHandleType, AUTOMATIC) UserId, VAR(uint8, AUTOMATIC) lowestMode)
{
    Std_ReturnType retVal;
    (void) lowestMode;

    switch (UserId)
    {
        case (ComMConf_ComMUser_ComMUser_Can_Network_Channel):
        {
            retVal = (Std_ReturnType)(Rte_Switch_UM_ComMUser_Can_Network_Channel_currentMode(lowestMode));
        }
        break;
        default:
        {
            retVal = E_OK;
        }
        break;
    }
    (void)retVal;
}
#define COMM_STOP_SEC_CODE
#include "ComM_Cfg_MemMap.h"


#if ((COMM_FULLCOMREQ_NOTIF != STD_OFF))
#define COMM_START_SEC_CODE
#include "ComM_Cfg_MemMap.h"
/* MR12 RULE 8.13 VIOLATION:The object addressed by the pointer parameter 'data' is not modified and so the pointer 
 *  could be of type 'pointer to const' is Incorrect warning*/
FUNC(void, COMM_CODE) ComM_CurrentChannelRequest_Rte_Write(VAR(uint8, AUTOMATIC) ChannelId,P2VAR(ComM_UserHandleArrayType, AUTOMATIC, COMM_APPL_DATA) data)
{
    Std_ReturnType retVal;
    switch (ChannelId)
    {
        default:
        {
            retVal = E_OK;
        }
        break;
    }
    (void)retVal;
}
#define COMM_STOP_SEC_CODE
#include "ComM_Cfg_MemMap.h"

#endif  /* #if ((COMM_FULLCOMREQ_NOTIF != STD_OFF) */

#endif  /* #if(COMM_RTE_SUPPORT != STD_OFF) */


/**********************************Declaration and definition of dummy api's****************************/
#define COMM_START_SEC_CODE
#include "ComM_Cfg_MemMap.h"
static FUNC(Std_ReturnType, COMM_CODE) ComM_Dummy_RequestComMode
                                                (
                                                VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
                                                VAR(ComM_ModeType, AUTOMATIC) ComM_Mode
                                                );

static FUNC(Std_ReturnType,COMM_CODE) ComM_Dummy_GetCurrentComMode
                                                (
                                                VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
                                                P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComM_ModePtr
                                                );


static FUNC(Std_ReturnType, COMM_CODE) ComM_Dummy_RequestComMode
                                                (
                                                VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
                                                VAR(ComM_ModeType, AUTOMATIC) ComM_Mode
                                                )
{
    (void) NetworkHandle;
    (void) ComM_Mode;
    return E_OK;
}

static FUNC(Std_ReturnType,COMM_CODE) ComM_Dummy_GetCurrentComMode
                                                (
                                                VAR(NetworkHandleType, AUTOMATIC) NetworkHandle,
                                                P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComM_ModePtr
                                                )
{
    *ComM_ModePtr = ComM_ChannelStruct[NetworkHandle].ChannelMode_u8;
    return E_OK;
}
#define COMM_STOP_SEC_CODE
#include "ComM_Cfg_MemMap.h"

#define COMM_START_SEC_CONST_UNSPECIFIED
#include "ComM_Cfg_MemMap.h"
CONST(ComM_BusSmApiType, COMM_CONST) ComM_BusSmApi[] =
{
    /* MR12 RULE 8.3 VIOLATION:Different <Bus>SM's use different parameters. Hence it is not possible to
    solve this warning */
    {
        /* COMM_BUS_TYPE_CAN  */
        &CanSM_RequestComMode,
        &CanSM_GetCurrentComMode
    }
    ,
    {
        /* COMM_BUS_TYPE_ETH  */
        &ComM_Dummy_RequestComMode,
        &ComM_Dummy_GetCurrentComMode
    }
    ,
    {
        /* COMM_BUS_TYPE_FR  */
        &ComM_Dummy_RequestComMode,
        &ComM_Dummy_GetCurrentComMode
    }
    ,
    {
        /* COMM_BUS_TYPE_INTERNAL  */
        &ComM_Dummy_RequestComMode,
        &ComM_Dummy_GetCurrentComMode
    }
    ,
    {
        /* COMM_BUS_TYPE_LIN  */
        &ComM_Dummy_RequestComMode,
        &ComM_Dummy_GetCurrentComMode
    }
};
#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include "ComM_Cfg_MemMap.h"


#if ((COMM_LIMIT_TO_NOCOM_ENABLED != STD_OFF)||(COMM_PREVENT_WAKEUP_ENABLED != STD_OFF))
#define COMM_START_SEC_CONST_8
#include "ComM_Cfg_MemMap.h"
CONST(ComM_InhibitionStatusType, COMM_CONST) ComM_EcuGroupClassification_Init = 0x00;

#define COMM_STOP_SEC_CONST_8
#include "ComM_Cfg_MemMap.h"
#endif /* #if (COMM_MODE_LIMITATION != STD_OFF)||(COMM_WAKEUP_INHIBITION != STD_OFF)  */


