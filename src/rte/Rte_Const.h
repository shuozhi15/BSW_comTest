/** @file     Rte_Const.h
  *
  * @brief    RTA-RTE related configuration constants
  *
  * @note     AUTOMATICALLY GENERATED FILE! DO NOT EDIT!
  *
  * @note     Generated by ETAS RTA-RTE
  * @note     Version 7.1.0 
  *
  * @date     Mon Nov 29 00:19:24 2021
  */

#ifndef RTE_CONST_H
#define RTE_CONST_H

#define RTE_CALPRM_NONE                                      (1)
#define RTE_COMPATIBILITY_MODE                               (1)
#define RTE_MAINFUNCTION_PERIOD_MS                           (10)
#define RTE_MAINFUNCTION_PERIOD_NS                           (10000000)
#define RTE_MAINFUNCTION_PERIOD_US                           (10000)
#define RTE_MAX_USED_CORE_ID                                 (-1)
#define RTE_MAX_USED_OSAPP_ID                                (0)
#define RTE_MEASUREMENT_ENABLED                              (1)
#define RTE_NUM_TASKS                                        (1)
#define RTE_NUM_TIMEOUT_ALARMS                               (0)
#define RTE_OSAPI_AUTOSAR_R40                                (1)
#define RTE_OSCFG_AUTOSAR_R40                                (1)
#define RTE_PERIODIC_COUNTER_TICK_INTERVAL_US                (10000)
#define RTE_REQUIRES_COM                                     (1)
#define RTE_RIPS_SUPPORT                                     (0)
#define RTE_TASKS_ARRAYSIZE                                  (1)
#define RTE_WOWP_EVENTS                                      (0)
#define _BMS_FlatView_SwComponentTypes_BMS_FlatView_CPT_BswM CPT_BswM
#define _BMS_FlatView_SwComponentTypes_BMS_FlatView_CPT_ComM CPT_ComM
#define _BMS_FlatView_SwComponentTypes_BMS_FlatView_CPT_EcuM CPT_EcuM
#define _BMS_FlatView_SwComponentTypes_BMS_FlatView_CPT_swc  CPT_swc
/* Box: Rte: OS Environment begin */
/* The following OSENV_ macros are supported by this OS DLL: */
/* OSENV_UNSUPPORTED */
/* OSENV_RTAOS40 */
#if defined(OSENV_RTAOS40)
#if defined(OSENV_UNSUPPORTED)
#error "Please define exactly one of [ OSENV_RTAOS40, OSENV_UNSUPPORTED ] for OS_AUTOSAR40 OS API."
#endif /* defined(OSENV_UNSUPPORTED) */
#elif defined(OSENV_UNSUPPORTED)
#else
#error "Please define exactly one of [ OSENV_RTAOS40, OSENV_UNSUPPORTED ] for OS_AUTOSAR40 OS API."
#endif
/* Box: User declared atomic BSW types: begin */
/* Box: User declared atomic BSW types: end */
/* Box: Rte: OS Environment end */

#endif /* !RTE_CONST_H */
