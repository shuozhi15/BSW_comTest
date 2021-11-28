#ifndef BSWM_PBCFG_H
#define BSWM_PBCFG_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
 */
#include "Std_Types.h"
#include "ComStack_Types.h"

#include "BswM_Cfg_General.h"

#include "CanSM_BswM.h"
#if (!defined(CANSM_AR_RELEASE_MAJOR_VERSION) || (CANSM_AR_RELEASE_MAJOR_VERSION != BSWM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(CANSM_AR_RELEASE_MINOR_VERSION) || (CANSM_AR_RELEASE_MINOR_VERSION != BSWM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#include "ComM.h"
#if (!defined(COMM_AR_RELEASE_MAJOR_VERSION) || (COMM_AR_RELEASE_MAJOR_VERSION != BSWM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(COMM_AR_RELEASE_MINOR_VERSION) || (COMM_AR_RELEASE_MINOR_VERSION != BSWM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#include "EcuM.h"
#if (!defined(ECUM_AR_RELEASE_MAJOR_VERSION) || (ECUM_AR_RELEASE_MAJOR_VERSION != BSWM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(ECUM_AR_RELEASE_MINOR_VERSION) || (ECUM_AR_RELEASE_MINOR_VERSION != BSWM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#include "PduR.h"
#if (!defined(PDUR_AR_RELEASE_MAJOR_VERSION) || (PDUR_AR_RELEASE_MAJOR_VERSION != BSWM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(PDUR_AR_RELEASE_MINOR_VERSION) || (PDUR_AR_RELEASE_MINOR_VERSION != BSWM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif

#include "BswM_Cfg_MRP.h"
#include "BswM_Cfg_AC.h"
/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
 */
#define BSWM_TOTAL_NO_OF_ACTIONLIST                         0

/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
/* C-Symbols for EcuM for configuration elements which are not unique */

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
 */

/* Type definition for the Logical Expression evaluate function, referred in BswMRule Structure */
typedef void (*BswM_LogicalExpression_tpfct)(boolean *isValidMode_pb,
		boolean *hasLogExpRes_pb);

/* ActionListItem Type Definition 
 * - AbortOnFail flag 
 * - ActionListItem type enumeration [Action/ActionList]
 * - Pointer to referred Action/ActionList structure
 */
typedef struct {
	boolean isActionListAbortOnFail_b;
	BswM_ActionListItemType_ten dataActionListItemType_en;
	const void *adrActionListItemRef_pv;
} BswM_Cfg_ActionListItemType_tst;

/*
 * - ActionListExecution type enumeration : Triggered/Conditional 
 * - Number of Action List Items 
 * - Pointer to ActionListItem structure 
 */
typedef struct {
	BswM_ActionListExecutionType_ten dataActionListExecutionType_en;
	uint8 nrActionListItems_u8;
	const BswM_Cfg_ActionListItemType_tst *adrActionListItem_pst;
	uint16 idxActionListnum;
} BswM_Cfg_ActionListType_tst;

/* BswMRule Type Definition 
 * - Rule state type enumeration : True/False/Undefined
 * - Function pointer to logical expression referred by Rule
 * - Pointer to TrueActionList structure
 * - Pointer to FalseActionList structure
 */
typedef struct {
	BswM_RuleStateType_ten dataRuleInitState_en;
	BswM_LogicalExpression_tpfct adrlogExp_pfct;
	const BswM_Cfg_ActionListType_tst *adrTrueAL_pst;
	const BswM_Cfg_ActionListType_tst *adrFalseAL_pst;
} BswM_Cfg_RuleType_tst;

/* BswMModeArbitration Type Definition 
 * - Number of rules configured in the system
 * - Pointer to array of Rules
 */
typedef struct {
	uint16 nrRules_u16;
	const BswM_Cfg_RuleType_tst *adrArbitrationRule_pst;
} BswM_Cfg_ModeArbitrationType_tst;

/* BswMModeControl Type Definition 
 * - Pointer to array of ActionLists
 */
typedef struct {
	const BswM_Cfg_ActionListType_tst *adrActionList_pst;
} BswM_Cfg_ModeControlType_tst;

/* BswMConfig Type Definition 
 * - Structure containing ModeArbitration elements 
 * - Structure containing ModeControl elements 
 */
typedef struct {
	BswM_Cfg_ModeArbitrationType_tst dataModeArbitration_st;
	BswM_Cfg_ModeControlType_tst dataModeControl_st;
	Std_VersionInfoType dataVersionInfo_st;
} BswM_ConfigType;

#define BSWM_NO_OF_CONFIGSETS 1U

#define BSWM_START_SEC_CONFIG_DATA_POSTBUILD_32
#include "BswM_Cfg_MemMap.h"

extern CONSTP2CONST(BswM_ConfigType,AUTOMATIC,BSWM_CONST) BswM_Configurations_capcst[BSWM_NO_OF_CONFIGSETS];

#define BSWM_STOP_SEC_CONFIG_DATA_POSTBUILD_32
#include "BswM_Cfg_MemMap.h"

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
 */

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
 */

/* C-Structures for EcuM_Init: TPS_ECUC_08011 + Description of EcucPostBuildVariants*/
/*MR12 RULE 5.5 VIOLATION:identifier name depends on the configured pre-defined variants hence the first 31 significant
 * characters of one macro may match macro name generated for another config set*/
extern const BswM_ConfigType BswM_Config;

#endif
/**********************************************************************************************************************
 * End of header file
 **********************************************************************************************************************/

