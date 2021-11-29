/** @file     Rte.c
  *
  * @brief    RTE data structures
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.1.0 
  *
  * @date     Mon Nov 29 16:06:17 2021
  */

/* RTE_HEADER_START */

#define RTE_CORE

#ifdef RTE_PREINCLUDE
#include RTE_PREINCLUDE
#endif
/* Box: RTE: OS Headers begin */
#include "Rte_Const.h"
#include "Os.h"
#if defined(RTE_REQUIRES_IOC)
#include "Ioc.h"
#endif /* defined(RTE_REQUIRES_IOC) */
/* Box: RTE: OS Headers end */
/* Box: RTE headers begin */
#include <Rte.h>
#include <Rte_Intl.h>
#include <Rte_Main.h>
#include "Rte_Type.h"
#include "Rte_Cfg.h"
#include "Rte_Cbk.h"
#if defined(RTE_REQUIRES_COM)
#include <Com.h>
#endif /* defined(RTE_REQUIRES_COM) */
#if defined(RTE_REQUIRES_NVM)
#include <NvM.h>
#endif /* defined(RTE_REQUIRES_NVM) */
#include "Rte_ComM.h"
#include "Rte_swc.h"
#include "SchM_CanNm.h"
#include "SchM_CanSM.h"
#include "SchM_Com.h"
#include "SchM_ComM.h"
/* Box: RTE headers end */
/* RTE_HEADER_END */


/* ----- Function prototypes ----- */

/* EXTERN_FN_PROTOTYPES_START */


/* EXTERN_FN_PROTOTYPES_END */

/* RTE_FN_PROTOTYPES_START */

/* Global functions */
/* Instance: /AUTOSAR_CanNm/BswImplementations/CanNm (BSWImpl0_CanNm), Type: CanNm */
/* Instance: /AUTOSAR_CanSM/BswImplementations/CanSM (BSWImpl2_CanSM), Type: CanSM */
/* Instance: /AUTOSAR_Com/BswImplementations/Com (BSWImpl1_Com), Type: Com */
/* Instance: /AUTOSAR_ComM/BswImplementations/ComM (BSWImpl3_ComM), Type: ComM */
/* Instance: /BMS_FlatView/SwComponentTypes/BMS_FlatView/CPT_ComM (CPT_ComM), Type: ComM */
/* Instance: /BMS_FlatView/SwComponentTypes/BMS_FlatView/CPT_swc (CPT_swc), Type: swc */
#define RTE_SWC_START_SEC_CODE
#include "Rte_MemMap.h"
FUNC(Std_ReturnType, RTE_CODE) Rte_ImplWrite_swc_soc_p_DataElements_VariableDataPrototype_0(VAR(uint16, AUTOMATIC) data);
#define RTE_SWC_STOP_SEC_CODE
#include "Rte_MemMap.h"

/* RTE_FN_PROTOTYPES_END */

/* ----- External variables ----- */

/* RTE_IMPORTS_START */

/* RTE_IMPORTS_END */

/* ----- Global variables ----- */

/* RTE_GLOBALS_START */

#define RTE_START_SEC_VAR_CLEARED_16
#include "Rte_MemMap.h"
VAR(Rte_ImplicitBufsType, RTE_DATA) Rte_ImplicitBufs;
#define RTE_STOP_SEC_VAR_CLEARED_16
#include "Rte_MemMap.h"

#define RTE_START_SEC_VAR_CLEARED_8
#include "Rte_MemMap.h"
VAR(uint8, RTE_DATA) Rte_Rx_000000;
#define RTE_STOP_SEC_VAR_CLEARED_8
#include "Rte_MemMap.h"

#define RTE_START_SEC_VAR_CLEARED_16
#include "Rte_MemMap.h"
VAR(uint16, RTE_DATA) Rte_Rx_000001;
#define RTE_STOP_SEC_VAR_CLEARED_16
#include "Rte_MemMap.h"

#define RTE_START_SEC_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
#if ( defined(RTE_WOWP_EVENTS) && ( RTE_WOWP_EVENTS > 0 ) )
CONST(Rte_TaskInfo, RTE_CONST) Rte_Tasks[1] = {
   { RTE_REF_FROM_TASK(OsTask_10ms), RTE_NUM_TIMEOUT_ALARMS, 0 }
};
#endif /* ( defined(RTE_WOWP_EVENTS) && ( RTE_WOWP_EVENTS > 0 ) ) */
#define RTE_STOP_SEC_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
/* RTE_GLOBALS_END */


/* ----- Data structures ----- */

/* RTE_CDS_START */

/* Instance: /AUTOSAR_CanNm/BswImplementations/CanNm (BSWImpl0_CanNm), Type: CanNm */
/* Omitted for BSW */
/* Instance: /AUTOSAR_CanSM/BswImplementations/CanSM (BSWImpl2_CanSM), Type: CanSM */
/* Omitted for BSW */
/* Instance: /AUTOSAR_Com/BswImplementations/Com (BSWImpl1_Com), Type: Com */
/* Omitted for BSW */
/* Instance: /AUTOSAR_ComM/BswImplementations/ComM (BSWImpl3_ComM), Type: ComM */
/* Omitted for BSW */
#define RTE_START_SEC_CONST_UNSPECIFIED
#include "Rte_MemMap.h"
/* Instance: /BMS_FlatView/SwComponentTypes/BMS_FlatView/CPT_ComM (CPT_ComM), Type: ComM */
CONST(struct Rte_CDS_ComM, RTE_CONST) Rte_Inst_ComM = {
   0U                                                /* Rte_Dummy */
};
CONST(Rte_SelfType_ComM, RTE_CONST) Rte_Self_CPT_ComM = &Rte_Inst_ComM;
/* Instance: /BMS_FlatView/SwComponentTypes/BMS_FlatView/CPT_swc (CPT_swc), Type: swc */
CONST(struct Rte_CDS_swc, RTE_CONST) Rte_Inst_swc = {
   /* Data handles */
   &Rte_ImplicitBufs.isa_1.Rte_OsTask_10ms.rbuf0,    /* RunnableEntity_0_crash_r_DataElements_VariableDataPrototype_0 */
   &Rte_ImplicitBufs.isa_1.Rte_OsTask_10ms.sbuf1     /* RunnableEntity_0_soc_p_DataElements_VariableDataPrototype_0 */
};
CONST(Rte_SelfType_swc, RTE_CONST) Rte_Self_CPT_swc = &Rte_Inst_swc;
#define RTE_STOP_SEC_CONST_UNSPECIFIED
#include "Rte_MemMap.h"

/* RTE_CDS_END */

/* ----- Function implementations ----- */

/* RTE_FUNCTIONS_START */

/* Instance: /AUTOSAR_CanNm/BswImplementations/CanNm (BSWImpl0_CanNm), Type: CanNm */
/* Instance: /AUTOSAR_CanSM/BswImplementations/CanSM (BSWImpl2_CanSM), Type: CanSM */
/* Instance: /AUTOSAR_Com/BswImplementations/Com (BSWImpl1_Com), Type: Com */
/* Instance: /AUTOSAR_ComM/BswImplementations/ComM (BSWImpl3_ComM), Type: ComM */
/* Instance: /BMS_FlatView/SwComponentTypes/BMS_FlatView/CPT_ComM (CPT_ComM), Type: ComM */
/* Instance: /BMS_FlatView/SwComponentTypes/BMS_FlatView/CPT_swc (CPT_swc), Type: swc */
/* Direct API 'Rte_IRead_RunnableEntity_0_crash_r_DataElements_VariableDataPrototype_0' for 'CPT_swc' optimized to implicit read. */
/* No function body generated - API implemented as macro. */
/* Function definition Rte_IRead_swc_RunnableEntity_0_crash_r_DataElements_VariableDataPrototype_0 omitted -- not used */

/* Direct API 'Rte_IWriteRef_RunnableEntity_0_soc_p_DataElements_VariableDataPrototype_0' for 'CPT_swc' optimized to implicit write. */
/* No function body generated - API implemented as macro. */
/* Function definition Rte_IWriteRef_swc_RunnableEntity_0_soc_p_DataElements_VariableDataPrototype_0 omitted -- not used */

/* Direct API 'Rte_IWrite_RunnableEntity_0_soc_p_DataElements_VariableDataPrototype_0' for 'CPT_swc' optimized to implicit write. */
/* No function body generated - API implemented as macro. */
/* Function definition Rte_IWrite_swc_RunnableEntity_0_soc_p_DataElements_VariableDataPrototype_0 omitted -- not used */

#define RTE_SWC_START_SEC_CODE
#include "Rte_MemMap.h"
FUNC(Std_ReturnType, RTE_CODE)
Rte_ImplWrite_swc_soc_p_DataElements_VariableDataPrototype_0(VAR(uint16, AUTOMATIC) data) /* 1 */
{
   VAR(Std_ReturnType, AUTOMATIC) rtn = RTE_E_OK;

   /* The signal is BMS_BattCellOrder18Vol */
   if ( ((VAR(StatusType, AUTOMATIC))E_OK) != Com_SendSignal(((VAR(Com_SignalIdType, AUTOMATIC))2), &data) )
   {
      rtn = ((VAR(Std_ReturnType, AUTOMATIC))RTE_E_COM_STOPPED);
   }

   /* Send complete */
   return rtn;
}

#define RTE_SWC_STOP_SEC_CODE
#include "Rte_MemMap.h"
/* ----- API functions ----- */

#define RTE_SIG_TMR_VOLTAGEHV_START_SEC_CODE
#include "Rte_MemMap.h"
FUNC(void, RTE_CODE)
Rte_COMCbk_TMR_VoltageHV(void)
{
   StatusType comstatus;
   uint8 data;
   boolean read_ok = TRUE;

   /* Box: test begin */
#if !defined(RTE_OMIT_UNINIT_CHECK)
   if ( TRUE != Rte_Initialized )
   {
      return;
   }
#endif /* !defined(RTE_OMIT_UNINIT_CHECK) */
   /* Box: test end */
   /* Box: initialize begin */
   /* Box: initialize end */
   /* Box: receive begin */
   comstatus = Com_ReceiveSignal(((VAR(Com_SignalIdType, AUTOMATIC))4), &data);
   if ( ((VAR(StatusType, AUTOMATIC))E_OK) != comstatus )
   {
      read_ok = ((VAR(boolean, AUTOMATIC))FALSE);
   }

   /* Box: receive end */
   /* Box: process begin */
   if ( TRUE == read_ok )
   {
      /* Optimizer: Removed resource lock */
      Rte_Rx_000000 = data;

   }
   /* Box: process end */
}

#define RTE_SIG_TMR_VOLTAGEHV_STOP_SEC_CODE
#include "Rte_MemMap.h"
/* RTE_FUNCTIONS_END */

/* ----- Task Bodies ----- */

/* RTE_TASKBODIES_START */

extern FUNC(Std_ReturnType, RTE_CODE) Rte_ImplWrite_swc_soc_p_DataElements_VariableDataPrototype_0(VAR(uint16, AUTOMATIC) data);

#define RTE_OSTASK_10MS_START_SEC_CODE
#include "Rte_MemMap.h"
TASK(OsTask_10ms)
{
   /* Box: Implicit Buffer Initialization begin */
   /* Pre-fill of write buffer */
   Rte_ImplicitBufs.isa_1.Rte_OsTask_10ms.sbuf1.value = Rte_Rx_000001;
   /* Box: Implicit Buffer Initialization end */
   /* Box: Implicit Buffer Fill begin */
   Rte_SuspendAllInterrupts();
   Rte_ImplicitBufs.isa_1.Rte_OsTask_10ms.rbuf0.value = Rte_Rx_000000;
   Rte_ResumeAllInterrupts();
   /* Box: Implicit Buffer Fill end */
   {
      /* Box: CPT_swc begin */
      RunnableEntity_swc_func();
      /* Box: CPT_swc end */
   }
   {
      /* Box: BSWImpl3_ComM begin */
      ComM_MainFunction_ComMChannel_Can_NCluster_Channel();
      /* Box: BSWImpl3_ComM end */
   }
   {
      /* Box: BSWImpl1_Com begin */
      Com_MainFunctionRx();
      /* Box: BSWImpl1_Com end */
   }
   {
      /* Box: BSWImpl2_CanSM begin */
      CanSM_MainFunction();
      /* Box: BSWImpl2_CanSM end */
   }
   {
      /* Box: BSWImpl0_CanNm begin */
      CanNm_MainFunction();
      /* Box: BSWImpl0_CanNm end */
   }
   {
      /* Box: BSWImpl1_Com begin */
      Com_MainFunctionTx();
      /* Box: BSWImpl1_Com end */
   }
   /* Box: Implicit Buffer Flush begin */
   Rte_ImplWrite_swc_soc_p_DataElements_VariableDataPrototype_0(Rte_ImplicitBufs.isa_1.Rte_OsTask_10ms.sbuf1.value);
   /* Writeback to global receive buffer because it is needed as the source of the buffer fill for --deviate-implicit-modify-for-loopbacks semantics */
   /* Write to Sender-Receiver DataItem /port/soc/DataElements_VariableDataPrototype_0 in port /swc/swc/soc_p on Application Software Component Instance /BMS_FlatView/SwComponentTypes/BMS_FlatView/CPT_swc */
   Rte_Rx_000001 = Rte_ImplicitBufs.isa_1.Rte_OsTask_10ms.sbuf1.value;
   /* Box: Implicit Buffer Flush end */
   TerminateTask();
} /* OsTask_10ms */
#define RTE_OSTASK_10MS_STOP_SEC_CODE
#include "Rte_MemMap.h"

/* RTE_TASKBODIES_END */

