
/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "BswM.h"
#include "BswM_Prv.h"

/**********************************************************************************************************************
 *  Definition of Global Variables                                                                                                    
 **********************************************************************************************************************/

#define BSWM_START_SEC_VAR_INIT_UNSPECIFIED
#include "BswM_Cfg_MemMap.h"

/* 
 * BswM_GetDelayedReqInfoType_tpfct BswM_GetDelayedReqInfo_capfct [] : 
 * Array of function pointers, wherein each index indicates the function that 
 * is used to access the required information for a particular MRP Type
 */

VAR(BswM_GetDelayedReqInfoType_tpfct, BSWM_VAR) BswM_GetDelayedReqInfo_capfct[] =
{
	NULL_PTR, /* BSWM_MRP_BSW_MODE_NOTIFICATION */
	NULL_PTR, /* BSWM_MRP_CANSM_IND */
	NULL_PTR, /*BSWM_MRP_COMM_IND*/
	NULL_PTR, /*BSWM_MRP_COMM_INITIATE_RESET*/
	NULL_PTR, /*BSWM_MRP_COMM_PNC_REQST*/
	NULL_PTR, /*BSWM_MRP_DCM_APPL_UPDATE_IND*/
	NULL_PTR, /*BSWM_MRP_DCM_COM_MODE_REQST*/
	NULL_PTR, /*BSWM_MRP_ECUM_IND*/
	NULL_PTR, /*BSWM_MRP_ECUM_RUN_REQST_IND*/
	NULL_PTR, /*BSWM_MRP_ECUM_WKUP_SOURCE*/
	NULL_PTR, /*BSWM_MRP_ETHIF_PORT_GROUP_STATE*/
	NULL_PTR, /*BSWM_MRP_ETHSM_IND*/
	NULL_PTR, /*BSWM_MRP_FRSM_IND*/
	NULL_PTR, /*BSWM_MRP_BSW_MODE_NOTIFICATION + BSWM_MRP_GENERIC_REQST + BSWM_MRP_SWC_MODE_NOTIFICATION + BSWM_MRP_SWC_MODE_REQUEST*/
	NULL_PTR, /*BSWM_MRP_J1939_DCM_BROADCAST_STATS*/
	NULL_PTR, /*BSWM_MRP_J1939_NM_IND*/
	NULL_PTR, /*BSWM_MRP_LINSM_IND*/
	NULL_PTR, /*BSWM_MRP_LINSM_SCHEDULE_IND*/
	NULL_PTR, /*BSWM_MRP_LINTP_MODE_REQST*/
	NULL_PTR, /*BSWM_MRP_NM_CAR_WKUP_IND*/
	NULL_PTR, /*BSWM_MRP_NVM_JOB_MODE_IND*/
	NULL_PTR, /*BSWM_MRP_NVM_REQST*/
	NULL_PTR, /*BSWM_MRP_SD_CLNT_SERV_CURR_STATE*/
	NULL_PTR, /*BSWM_MRP_SD_CNSMD_EVNT_GRP_CURR_STATE*/
	NULL_PTR, /*BSWM_MRP_SD_EVNT_HNDLR_CURR_STATE*/
	NULL_PTR, /*BSWM_MRP_SWC_MODE_NOTIFICATION*/
	NULL_PTR, /*BSWM_MRP_SWC_MODE_REQUEST*/

};

#define BSWM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "BswM_Cfg_MemMap.h"

/**********************************************************************************************************************
 *																										
 **********************************************************************************************************************/
