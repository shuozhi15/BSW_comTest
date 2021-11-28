
#include "BswM.h" // required for BswM_RequestMode

#if (BSWM_RTE_IN_USE == TRUE)
#include "Rte_BswM.h"
#include "SchM_BswM.h"
#endif

#define BSWM_START_SEC_CODE
#include "BswM_Cfg_MemMap.h"

#define BSWM_STOP_SEC_CODE
#include "BswM_Cfg_MemMap.h"

