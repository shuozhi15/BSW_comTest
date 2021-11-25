

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "CanIf_Prv.h"

/***********************************************************************************************************************
 * Function name    : CanIf_SetBaudrate
 * Syntax           : FUNC(void, CANIF_CODE) CanIf_SetBaudrate(
                                                       VAR (uint8,AUTOMATIC) ControllerId,
                                                       CONST(uint16,AUTOMATIC) BaudRateConfigID
                                                    )
 * Description      : This service returns E_OK if the baudrate is changed successfully for
                      all Can controllers else returns E_NOT_OK
 * Parameter        : Controller ,  BaudRateConfigID
 * Return value     : Std_ReturnType
 **********************************************************************************************************************/
#if (CANIF_SET_BAUDRATE_API == STD_ON)
#define CANIF_START_SEC_CODE
#include "CanIf_MemMap.h"

FUNC(Std_ReturnType, CANIF_CODE) CanIf_SetBaudrate(
                                         VAR(uint8,AUTOMATIC) ControllerId,
                                         CONST(uint16,AUTOMATIC) BaudRateConfigID
                                         )
{

    /* Variable to store the CanStatus returned by Can_SetBaudrate() */
    VAR (Std_ReturnType, AUTOMATIC   ) retVal_en           = E_NOT_OK;
    P2CONST(CanIf_Cfg_CtrlConfig_tst, AUTOMATIC, CANIF_CFG_CONST) lCtrlConfig_pst;
    VAR(uint8, AUTOMATIC)          lCtrlCustId_u8;

    /* If CAN Interface is uninitialized, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((FALSE == CanIf_Prv_InitStatus_b), CANIF_SETBAUDRATE_SID, CANIF_E_UNINIT)

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((ControllerId >= CANIF_TOTAL_CTRLS), CANIF_SETBAUDRATE_SID, CANIF_E_PARAM_CONTROLLERID)

    lCtrlCustId_u8 = CanIf_Prv_ConfigSet_tpst->CtrlIdTable_Ptr[ControllerId];

    /* If Controller is invalid, report to DET and return E_NOT_OK */
    CANIF_DET_REPORT_ERROR_NOT_OK((lCtrlCustId_u8 == 0xFFu), CANIF_SETBAUDRATE_SID, CANIF_E_PARAM_CONTROLLERID)


    /*Get configured data of requested controller*/
    lCtrlConfig_pst  = (CanIf_Prv_ConfigSet_tpst->CanIf_CtrlConfigPtr) + lCtrlCustId_u8;

    retVal_en = Can_SetBaudrate((uint8)(lCtrlConfig_pst->CtrlCanCtrlRef), BaudRateConfigID);

    return(retVal_en);

}
#define CANIF_STOP_SEC_CODE
#include "CanIf_MemMap.h"
#endif

