

#ifndef CANNM_H
#define CANNM_H

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComStack_Types.h"
#include "CanNm_Cfg_Internal.h"
#include "CanNm_Cfg.h"
#include "CanNm_PBcfg.h"

#if (!defined(COMTYPE_AR_RELEASE_MAJOR_VERSION) || (COMTYPE_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COMTYPE_AR_RELEASE_MINOR_VERSION) || (COMTYPE_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif


#include "Nm.h"
#if (!defined(NM_AR_RELEASE_MAJOR_VERSION) || (NM_AR_RELEASE_MAJOR_VERSION != CANNM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(NM_AR_RELEASE_MINOR_VERSION) || (NM_AR_RELEASE_MINOR_VERSION != CANNM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

/* Development Error Codes for DET Support*/

#define CANNM_E_NO_INIT               0x01 /* API service used without module initialization */
#define CANNM_E_INVALID_CHANNEL       0x02 /* API service called with wrong channel handle */
#define CANNM_E_INVALID_PDUID         0x03 /* API service called with wrong PDU-ID */
#define CANNM_E_NET_START_IND         0x04 /* Reception of NM PDUs in Bus-Sleep Mode */
#define CANNM_E_INIT_FAILED           0x05 /* CanNm Initialisation has failed */
#define CANNM_E_NETWORK_TIMEOUT       0x11 /* NM-Timeout Timer has abnormally expired outside of the Ready Sleep State*/
#define CANNM_E_PARAM_POINTER         0x12 /* Null pointer has been passed as an argument */


/*
 ***************************************************************************************************
 * Extern declarations
 ***************************************************************************************************
 */

#define CANNM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"
extern CONST(CanNm_ConfigType, CANNM_CONST)                               CanNmGlobalConfig;
#define CANNM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(void, CANNM_CODE) CanNm_Init(
                                          P2CONST(CanNm_ConfigType, AUTOMATIC, CANNM_APPL_CONST) cannmConfigPtr
                                         );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_PassiveStartUp(
                                                              VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle
                                                             );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_NetworkRequest(
                                                              VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle
                                                             );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_NetworkRelease(
                                                              VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle
                                                             );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_DisableCommunication(
                                                                    VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle
                                                                   );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_EnableCommunication(
                                                                   VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle
                                                                  );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_SetUserData(
                                                           VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                           P2CONST(uint8, AUTOMATIC, CANNM_APPL_DATA) nmUserDataPtr
                                                          );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetUserData(
                                                           VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                           P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) nmUserDataPtr
                                                          );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#if((CANNM_COM_USER_DATA_SUPPORT != STD_OFF) || (CANNM_PN_ENABLED != STD_OFF))
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_Transmit(
                                                        PduIdType CanNmTxPduId,
                                                        P2CONST(PduInfoType, AUTOMATIC, CANNM_APPL_DATA) PduInfoPtr
                                                       );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetNodeIdentifier(
                                                                 VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                                 P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) nmNodeIdPtr
                                                                );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"


#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetLocalNodeIdentifier(
                                                                      VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                                    P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) nmNodeIdPtr
                                                                     );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_RepeatMessageRequest(
                                                                    VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle
                                                                   );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetPduData(
                                                          VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                          P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) nmPduDataPtr
                                                         );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetState(
                                                        VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                        P2VAR(Nm_StateType, AUTOMATIC, CANNM_APPL_DATA) nmStatePtr,
                                                        P2VAR(Nm_ModeType, AUTOMATIC, CANNM_APPL_DATA) nmModePtr
                                                       );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#if (CANNM_VERSION_INFO_API != STD_OFF)
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(void, CANNM_CODE) CanNm_GetVersionInfo(
                                                    P2VAR(Std_VersionInfoType, AUTOMATIC, CANNM_APPL_CONST) versioninfo
                                                   );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_RequestBusSynchronization(
                                                                         VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle
                                                                        );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_CheckRemoteSleepIndication(
                                                                          VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                                          P2VAR(boolean, AUTOMATIC, CANNM_APPL_DATA) nmRemoteSleepIndPtr
                                                                         );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_SetSleepReadyBit(
                                                                VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle,
                                                                VAR(boolean, AUTOMATIC) nmSleepReadyBit
                                                               );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

/* If RTE is in use then the declaration will be provided by SchM file */
#if (CANNM_ECUC_RB_RTE_IN_USE == STD_OFF)
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(void, CANNM_CODE) CanNm_MainFunction( void );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#if (CANNM_PN_ENABLED != STD_OFF)
#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(void, CANNM_CODE) CanNm_ConfirmPnAvailability(
                                                            VAR(NetworkHandleType, AUTOMATIC) nmChannelHandle
                                                           );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"
#endif

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(void, CANNM_CODE) CanNm_SetPnState(
                                               const NetworkHandleType nmChannelHandle,
                                               boolean state
                                              );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(void, CANNM_CODE) CanNm_SetAllMessageKeepAwake(
                                                           const NetworkHandleType nmChannelHandle,
                                                           boolean state
                                                          );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#define CANNM_START_SEC_CODE
#include "CanNm_MemMap.h"
extern FUNC(void, CANNM_CODE) CanNm_SetPnHandleMultipleNetworkReqState(
                                                                       const NetworkHandleType nmChannelHandle,
                                                                       boolean state
                                                                      );
#define CANNM_STOP_SEC_CODE
#include "CanNm_MemMap.h"

#endif /* CANNM_H */


