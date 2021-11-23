

#ifndef COM_PRV_H
#define COM_PRV_H

/*
 **********************************************************************************************************************
 * Includes
 **********************************************************************************************************************
*/
#include "Com.h"
#include "Com_Cfg_Internal.h"
#include "Com_Cfg_SchM.h"
#include "Com_Prv_Types.h"
#include "rba_BswSrv.h"

#if (COM_CONFIGURATION_VARIANT == COM_VARIANT_PRE_COMPILE)
#include "Com_PBcfg.h"
#endif /* #if (COM_CONFIGURATION_VARIANT == COM_VARIANT_PRE_COMPILE) */

#if(COM_CONFIGURATION_USE_DET == STD_ON)
#include "Det.h"
/* [SWS_BSW_00036] Inter Module Checks */
#if (!defined(DET_AR_RELEASE_MAJOR_VERSION) || (DET_AR_RELEASE_MAJOR_VERSION != COM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(DET_AR_RELEASE_MINOR_VERSION) || (DET_AR_RELEASE_MINOR_VERSION != COM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif

#ifdef COM_ARSRVLIB_SUPPORT
#include "Bfx.h"
/* [SWS_BSW_00036] Inter Module Checks */
#if (!defined(BFX_AR_RELEASE_MAJOR_VERSION) || (BFX_AR_RELEASE_MAJOR_VERSION != COM_AR_RELEASE_MAJOR_VERSION))
#error "AUTOSAR major version undefined or mismatched"
#endif
#if (!defined(BFX_AR_RELEASE_MINOR_VERSION) || (BFX_AR_RELEASE_MINOR_VERSION != COM_AR_RELEASE_MINOR_VERSION))
#error "AUTOSAR minor version undefined or mismatched"
#endif
#endif /* #ifndef COM_ARSRVLIB_SUPPORT */

/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/
/* FC_VariationPoint_START */
#ifdef COM_ENABLE_READRXIPDULENGTH
/**
 * @ingroup COM_H
 *
 * This macro represents the service ID for the API Com_ReadRxIPduLength()
 * This is some random number assigned to this API \n
 */
#define COMServiceId_ReadRxIPduLength                                   (0x95)
#endif /* #ifdef COM_ENABLE_READRXIPDULENGTH */
/* FC_VariationPoint_END */

/**
 * @ingroup COM_PRV_H
 * Macro to convert boolean to Unsigned value
 */
#define COM_BOOL_TO_UNSIGNED(BoolValue) ((BoolValue) ? (COM_SET) : (COM_RESET))

/**
 * @ingroup COM_PRV_H
 *
 *  Endianes type: Little endian
 */
#define COM_LITTLEENDIAN    0u
/**
 * @ingroup COM_PRV_H
 *
 *  Endianes type: Big Endian
 */
#define COM_BIGENDIAN       1u
/**
 * @ingroup COM_PRV_H
 *
 * Endianes type:  Opaque
 */
#define COM_OPAQUE          COM_LITTLEENDIAN

#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#define COM_CPU_ENDIANNESS COM_LITTLEENDIAN
#else
#define COM_CPU_ENDIANNESS COM_BIGENDIAN
#endif

/* Start: Internal #defines*/

/**
 * @ingroup COM_PRV_H
 *
 * Ipdu processing: Immediate
 */
#define COM_IMMEDIATE   FALSE
/**
 * @ingroup COM_PRV_H
 *
 * Ipdu processing:  Deffered
 */
#define COM_DEFERRED    TRUE

/**
 * @ingroup COM_PRV_H
 *
 * Signal Property: Triggered on Change
 */
#define COM_TRIGGERED_ON_CHANGE                     0u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Property: Triggered on Change Without Repetition
 */
#define COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION  1u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Property: Pending
 */
#define COM_PENDING                                 2u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Property: Triggered Without Repetition
 */
#define COM_TRIGGERED_WITHOUT_REPETITION            3u
/**
 * @ingroup COM_PRV_H
 *
 *  Signal Property: Triggered
 */
#define COM_TRIGGERED                               4u

/* Returns true only if signal transfer property is configured as
 * COM_TRIGGERED or
 * COM_TRIGGERED_WITHOUT_REPETITION */
#define COM_SIG_TP_IS_TRIGGERED(TransProp)             (TransProp > COM_PENDING)

/* Returns true only if signal transfer property is configured as
 * COM_TRIGGERED_ON_CHANGE or
 * COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION */
#define COM_SIG_TP_IS_TRIGGERED_ON_CHANGE(TransProp)   (TransProp < COM_PENDING)

/* Returns true only if signal transfer property is configured as
 * COM_TRIGGERED_WITHOUT_REPETITION or
 * COM_TRIGGERED_ON_CHANGE_WITHOUT_REPETITION */
#define COM_SIG_TP_IS_WITHOUT_REPETITION(TransProp)    ((TransProp & 0x1u) != 0u)

/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Flag status RESET
 */
#define COM_RESET       0u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Flag status SET
 */
#define COM_SET         1u

/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Flag status value REPLACE_TO_INIT, for signal group
 * Action taken : The Signal group buffer is filled with init value
 */
#define COM_REPLACE_TO_INIT  2u

/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Flag status STOP
 */
#define COM_STOP        FALSE
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Flag status START
 */
#define COM_START       TRUE

/* Defines are arranged to optimise runtime during checks */
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Tx Ipdu mode status DIRECT
 */
#define COM_TXMODE_DIRECT      0x00u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Tx Ipdu mode status MIXED
 */
#define COM_TXMODE_MIXED       0x01u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Tx Ipdu mode status NONE
 */
#define COM_TXMODE_NONE        0x02u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Tx Ipdu mode status PERIODIC
 */
#define COM_TXMODE_PERIODIC    0x03u

/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Clear Update bit status - TRANSMIT
 */
#define     COM_CLRUPDATEBIT_TRANSMIT           1u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Clear Update bit status - TRIGGERTRANSMIT
 */
#define     COM_CLRUPDATEBIT_TRIGGERTRANSMIT    2u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Clear Update bit status - CONFIRMATION
 */
#define     COM_CLRUPDATEBIT_CONFIRMATION       3u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Clear Update bit status - NOT SET
 */
#define     COM_CLRUPDATEBIT_NONE               0u

/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Flag status - TRUE
 */
#define COM_TRUE        TRUE
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Flag status - FALSE
 */
#define COM_FALSE       FALSE

/**
 * @ingroup COM_PRV_H
 *
 *  Invalid value actions: None
 */
#define COM_NONE        0u
/**
 * @ingroup COM_PRV_H
 *
 * Invalid value actions: Replace
 */
#define COM_REPLACE     1u
/**
 * @ingroup COM_PRV_H
 *
 * Invalid value actions:  Notify
 */
#define COM_NOTIFY      2u

/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent Flag status - Bit is not used/Unused Index
 */
#define COM_NOT_USED    0u


#define COM_MAXIMUM_GWSYNC_REPITIONS 2

/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent status - InvalidValue is configured or not in GroupSignal
 */
#define COM_ZERO       0u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent status - InvalidValue is configured or not in GroupSignal
 */
#define COM_ONE        1u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code with Modulus operator
 */
#define COM_TWO        2u


/* START: Signal Type */

/*
 * VERY IMPORTANT : Do not change the following hash defines.
 * These values are used in the code, to optimise the handling.
 * 1. All the odd values are used by signed types
 */
/**
 * @ingroup COM_PRV_H
 *
 *  Signal Type UINT8
 */
#define COM_UINT8                           0x00u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type SINT8
 */
#define COM_SINT8                           0x01u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type UINT16
 */
#define COM_UINT16                          0x02u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type SINT16
 */
#define COM_SINT16                          0x03u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type UINT32
 */
#define COM_UINT32                          0x04u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type SINT32
 */
#define COM_SINT32                          0x05u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type UINT64
 */
#define COM_UINT64                          0x10u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type SINT64
 */
#define COM_SINT64                          0x11u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type BOOLEAN
 */
#define COM_BOOLEAN                         0x06u

/**
 * @ingroup COM_PRV_H
 *
 * NOTE : Signal Type COM_UNDEFTYPE, All the signed data types have '1' as LSB.
 * All the signed data types have special operation based on above check, Be care full in Adding a new Data type.
 */
#define COM_UNDEFTYPE                       0x07

/**
 * @ingroup COM_PRV_H
 *
 * Signal Type UINT8_N
 */
#define COM_UINT8_N                         0x08u
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type COM_UINT8_DYN
 */
#define COM_UINT8_DYN                       0x0Au
/* FC_VariationPoint_START */
/**
 * @ingroup COM_PRV_H
 *
 * Signal Type COM_FLOAT32
 */
#define COM_FLOAT32                         0x0Cu

/**
 * @ingroup COM_PRV_H
 *
 * Signal Type COM_FLOAT32
 */
#define COM_FLOAT64                         0x0Eu
/* FC_VariationPoint_END */
/* END: Signal Type */

/* End: Internal #defines */

/* START: Signal Filter Type */

/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm ALWAYS
 */
#define COM_ALWAYS                          0u  /*If filter is not configured it should be COM_ALWAYS */
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm NEVER
 */
#define COM_NEVER                           1u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm MASKED_NEW_EQUALS_X
 */
#define COM_MASKED_NEW_EQUALS_X             2u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm MASKED_NEW_DIFFERS_X
 */
#define COM_MASKED_NEW_DIFFERS_X            3u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm MASKED_NEW_DIFFERS_MASKED_OLD
 */
#define COM_MASKED_NEW_DIFFERS_MASKED_OLD   4u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm NEW_IS_WITHIN_POS
 */
#define COM_NEW_IS_WITHIN_POS               5u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm NEW_IS_WITHIN_NEG
 */
#define COM_NEW_IS_WITHIN_NEG               6u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm NEW_IS_OUTSIDE_POS
 */
#define COM_NEW_IS_OUTSIDE_POS              7u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm NEW_IS_OUTSIDE_NEG
 */
#define COM_NEW_IS_OUTSIDE_NEG              8u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm ONE_EVERY_N
 */
#define COM_ONE_EVERY_N                     9u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent signal filter algorithm is not configured
 */
#define COM_NOTCONFIGURED                   10u

/* END: Signal Filter Type */

/* START: TMS calculation for IPDU */
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent IPdu has configured mode state TRUE and FALSE
 */
#define COM_MODE_VALID                      0u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent IPdu has configured mode state TRUE
 */
#define COM_TRUEMODE_VALID                  1u
/**
 * @ingroup COM_PRV_H
 *
 * Macro used in code to represent IPdu has configured mode state FALSE
 */
#define COM_FALSEMODE_VALID                 2u

/* END: TMS calculation for IPDU */

/**
 * @ingroup COM_PRV_H
 *
 * Macro is used to initialize instance of 'Com_SendIpduInfo' structure in Com Code
 */
#define COM_CLEAR_SENDIPDU_FLAGS            {0,0,0,0,0,0,0}

/**
 * @ingroup COM_PRV_H
 *
 * Used to indicate that rx-signals are not-configured with update bit and timeout
 */
#define UPDATEBIT_NOT_APPLICABLE    0x00u
/**
 * @ingroup COM_PRV_H
 *
 * Used to indicate that rx-signals are configured with update bit and timeout
 */
#define UPDATEBIT_APPLICABLE        0x01u
#define UPDATEBIT_SET               0x02u

/* Start: DET Checking macro */
#if(COM_CONFIGURATION_USE_DET == STD_ON)
/* Macro is used which indicates No DET error */
#define COM_DET_NO_ERROR  0xFFu

/* Macro is used to call DET error function */
#define COM_REPORT_ERROR_CHECK(API, ERROR)   (void)Det_ReportError(COM_MODULE_ID, COM_INSTANCE_ID, (API), (ERROR))
#endif /* #if(COM_CONFIGURATION_USE_DET == STD_ON) */
/* End: DET Checking macro */

/**
 * @ingroup COM_PRV_H
 *
 * Macro to perform sign extension
 */
#define Com_PerformSignExtension(Value,Size)    (Value) | (COM_SIGN_EXT_MASK << ((Size) - 1))

/*Macros for Bit-fields for RAM variables to Set the value*/
/* FC_VariationPoint_START */
/*
typedef struct
{
    uint16 PDUSTATUS:1;         Flag gives the old status of the Pdu Activation status
    uint16 CONFIR:1;            Used to check whether Confirmation for the Tx frame has been received or not
    uint16 TICKTXTO:1;          Flag which tells whether Timer was started/Stopped
    uint16 MDT:1;               Flag which tells whether Minimum delay time has not completed
    uint16 EVENTTRIG:1;         Flag which stores the information whether IPdu is transmitted in event triggered
                                    call from Com_SendSignal/Com_SendDynSignal/Com_SendSignalGroup/
                                    Com_SendSignalGroupArray/Com_TriggerIpduSend/Com_TriggerIpduSendWithMetadata/
                                    Com_SwitchIpduTxMode
    uint16 ISGWTRIGGRD:1;       Flag which stores if there was a Trigger on the Gw Pdu
    uint16 ISTOENABLED:1;       Flag which stores information if the Timeout timer has to be started for Ipdu.
    uint16 LARGEDATAINPROG:1;   Flag is set, if the Tp Pdu for large data pdu handling is in progress
    uint16 GWIPDUTMSCHANGED:1;  Flag is set, if the Gw-IPDU's Tranmission Mode has changed after its last
                                    transmission from Com
    uint16 RETRYFAILEDTXREQ:1;  Flag is et, if any failed Tx-Requests are present
    uint16 Not_Used:6;          Un-used bits
} Com_TxFlagsType;
*/
/* FC_VariationPoint_END */

/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the PDU activation status of Tx IPdu
 */
/* The below macros transforms to --> Com_TxFlagType.PduStatus = 1 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_PDUSTATUS(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint16)0xFFFE)) | (uint16)(BitVal))

#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 16-bit variable Data.
 * Argument 1: pointer to a 16-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */

#define Com_SetRamValue_TXIPDU_PDUSTATUS(ByteValue,BitVal)         Bfx_PutBit_u16u8u8(&(ByteValue),0,(boolean)(BitVal))
#endif

/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the PDU activation status of Tx IPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_PDUSTATUS(ByteValue)         ((uint16)(ByteValue) & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 16-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_TXIPDU_PDUSTATUS(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),0)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs the status of Tx confirmation of a Tx Ipdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_CONFIR(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint16)0xFFFD)) | (uint16)((BitVal)<<1))
#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_TXIPDU_CONFIR(ByteValue,BitVal)         Bfx_PutBit_u16u8u8(&(ByteValue),1,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the status of Tx confirmation of a Tx Ipdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_CONFIR(ByteValue)         ((uint16)((ByteValue)>>1) & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_TXIPDU_CONFIR(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),1)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs the Time out status flag for Tx IPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_TICKTXTO(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint16)0xFFFB)) | (uint16)((BitVal)<<2))
#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_TXIPDU_TICKTXTO(ByteValue,BitVal)         Bfx_PutBit_u16u8u8(&(ByteValue),2,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Time out status flag for Tx IPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_TICKTXTO(ByteValue)         ((uint16)((ByteValue) >> 2) & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_TXIPDU_TICKTXTO(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),2)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the MDT flag for Tx IPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_MDT(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint16)0xFFF7)) | (uint16)((BitVal)<<3))
#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_TXIPDU_MDT(ByteValue,BitVal)         Bfx_PutBit_u16u8u8(&(ByteValue),3,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the MDT flag for Tx IPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_MDT(ByteValue)         ((uint16)((ByteValue) >> 3)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */

#define Com_GetRamValue_TXIPDU_MDT(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),3)
#endif


 /**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs the Tx value of the flag which stores the information whether IPdu is transmitted in event triggered\n
 *  call from Com_SendSignal/Com_SendSignalGroup/Com_SendDynSignal/Com_TriggerIpduSend
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_EVENTTRIG(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint16)0xFFEF)) | (uint16)((BitVal)<<4))
#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_TXIPDU_EVENTTRIG(ByteValue,BitVal)         Bfx_PutBit_u16u8u8(&(ByteValue),4,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 *   This Macro unpacks and returns the Tx value of the flag which stores the information whether IPdu is transmitted in event triggered\n
 *  call from Com_SendSignal/Com_SendSignalGroup/Com_SendDynSignal/Com_TriggerIpduSend
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_EVENTTRIG(ByteValue)         ((uint16)((ByteValue) >> 4)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_TXIPDU_EVENTTRIG(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),4)
#endif

/**
* @ingroup COM_PRV_H
*
*  This Macro packs the flag value which stores the information whether a Gw IPdu is triggered for event transmission\n
*  The trigger can happen in Com_MainfunctionRouteSignals and Com_SendSignal/Com_SendSignalGroup
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_ISGWTRIGGRD(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint16)0xFFDF)) | (uint16)((BitVal)<<5))
#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
* Argument 1: pointer to a 8-bit variable, where a bit should be modified
* Argument 2: position of the bit in the byte
* Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
* */
#define Com_SetRamValue_TXIPDU_ISGWTRIGGRD(ByteValue,BitVal)         Bfx_PutBit_u16u8u8(&(ByteValue),5,(boolean)(BitVal))
#endif


/**
* @ingroup COM_PRV_H
*
*   This Macro unpacks and returns the Tx value of the flag which stores the information whether IPdu is transmitted in event triggered\n
*  call by application OR Com_MainFunctionTx()
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_ISGWTRIGGRD(ByteValue)         ((uint16)((ByteValue) >> 5)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
* Argument 1    : 8-bit Base variable, where should be read from.
* Argument 2    : bit position from, where the bit shall be read
* Return value    : Returns the bit value
* */
#define Com_GetRamValue_TXIPDU_ISGWTRIGGRD(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),5)
#endif

/**
* @ingroup COM_PRV_H
*
*  This Macro packs the flag value which stores the information whether a Gw signal which triggered IPdu transmission has configured timeout\n
*  The trigger can happen in Com_MainfunctionRouteSignals and Com_SendSignal/Com_SendSignalGroup
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_ISTOENABLED(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint16)0xFFBF)) | (uint16)((BitVal)<<6))
#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
* Argument 1: pointer to a 8-bit variable, where a bit should be modified
* Argument 2: position of the bit in the byte
* Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
* */
#define Com_SetRamValue_TXIPDU_ISTOENABLED(ByteValue,BitVal)         Bfx_PutBit_u16u8u8(&(ByteValue),6,(boolean)(BitVal))
#endif


/**
* @ingroup COM_PRV_H
*
*  This Macro unpacks and returns the flag value which stores the information whether a Gw signal which triggered IPdu transmission has configured timeout\n
*  The trigger can happen in Com_MainfunctionRouteSignals and Com_SendSignal/Com_SendSignalGroup
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_ISTOENABLED(ByteValue)         ((uint16)((ByteValue) >> 6)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
* Argument 1    : 8-bit Base variable, where should be read from.
* Argument 2    : bit position from, where the bit shall be read
* Return value    : Returns the bit value
* */
#define Com_GetRamValue_TXIPDU_ISTOENABLED(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),6)
#endif


/**
* @ingroup COM_PRV_H
*
*  This Macro packs the flag value which stores the information whether a Gw signal which triggered IPdu transmission has configured timeout\n
*  The trigger can happen in Com_MainfunctionRouteSignals and Com_SendSignal/Com_SendSignalGroup
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_LARGEDATAINPROG(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint16)0xFF7F)) | (uint16)((BitVal) << 7))
#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
* Argument 1: pointer to a 8-bit variable, where a bit should be modified
* Argument 2: position of the bit in the byte
* Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
* */
#define Com_SetRamValue_TXIPDU_LARGEDATAINPROG(ByteValue,BitVal)         Bfx_PutBit_u16u8u8(&(ByteValue),7,(boolean)(BitVal))
#endif

/**
* @ingroup COM_PRV_H
*
*  This Macro unpacks and returns the flag value which stores the information whether a Gw signal which triggered IPdu transmission has configured timeout\n
*  The trigger can happen in Com_MainfunctionRouteSignals and Com_SendSignal/Com_SendSignalGroup
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_LARGEDATAINPROG(ByteValue)        ((uint16)((ByteValue) >> 7)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
* Argument 1    : 8-bit Base variable, where should be read from.
* Argument 2    : bit position from, where the bit shall be read
* Return value  : Returns the bit value
* */
#define Com_GetRamValue_TXIPDU_LARGEDATAINPROG(ByteValue)        Bfx_GetBit_u16u8_u8((ByteValue),7)
#endif

/**
* @ingroup COM_PRV_H
*
* This Macro packs the flag value which stores the information whether TMS has changed after
* its last tranmission
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_GWIPDUTMSCHANGED(ByteValue,BitVal)  (ByteValue) = (((ByteValue) & ((uint16)0xFEFF)) | \
                                                                                               (uint16)((BitVal) << 8))
#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
* Argument 1: pointer to a 8-bit variable, where a bit should be modified
* Argument 2: position of the bit in the byte
* Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
* */
#define Com_SetRamValue_TXIPDU_GWIPDUTMSCHANGED(ByteValue,BitVal)  Bfx_PutBit_u16u8u8(&(ByteValue),8,(boolean)(BitVal))
#endif

/**
* @ingroup COM_PRV_H
*
* This Macro unpacks and returns the flag value which stores the information whether TMS has changed after
* its last tranmission
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_GWIPDUTMSCHANGED(ByteValue)         ((uint16)((ByteValue) >> 8)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
* Argument 1    : 8-bit Base variable, where should be read from.
* Argument 2    : bit position from, where the bit shall be read
* Return value  : Returns the bit value
* */
#define Com_GetRamValue_TXIPDU_GWIPDUTMSCHANGED(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),8)
#endif

/**
* @ingroup COM_PRV_H
*
* Macro to store failed Tx-IPdu requests, to be retried in the next Com_MainFunctionTx()
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_RETRYFAILEDTXREQ(ByteValue,BitVal)  (ByteValue) = (((ByteValue) & ((uint16)0xFDFF)) | \
                                                                                              (uint16)((BitVal) << 9))
#else
/* Bfx_PutBit_u16u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
* Argument 1: pointer to a 8-bit variable, where a bit should be modified
* Argument 2: position of the bit in the byte
* Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
* */
#define Com_SetRamValue_TXIPDU_RETRYFAILEDTXREQ(ByteValue,BitVal)  Bfx_PutBit_u16u8u8(&(ByteValue),9,(boolean)(BitVal))
#endif

/**
* @ingroup COM_PRV_H
*
* Macro to unpack and check for any failed Tx-IPdu requests, to be retried in the Com_MainFunctionTx()
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_RETRYFAILEDTXREQ(ByteValue)         ((uint16)((ByteValue) >> 9)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
* Argument 1    : 8-bit Base variable, where should be read from.
* Argument 2    : bit position from, where the bit shall be read
* Return value  : Returns the bit value
* */
#define Com_GetRamValue_TXIPDU_RETRYFAILEDTXREQ(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),9)
#endif

/* FC_VariationPoint_START */
/*
 typedef struct
{
    uint8 PduStatus:1;              Flag gives the old status of the Pdu Activation status
    uint8 Pdu_DMStatus:1;           Flag gives the old status of the Pdu Activation status
    uint8 RxIndicationFlag:1;       Flag used to Indicate that Ipdu was received
    uint8 IsRxGwIpduProcessed:1;    Flag used to Indicate if the Gw Ipdu is processed after its reception
    uint8 rxTPIPduFlag:1;           Flag used to Indicate the reception of TP Rx Ipdu
    uint8 Not_Used:3;               Not used Bit field

} Com_RxFlagsType;
 */
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the PDU activation status of Rx IPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXIPDU_PDUSTATUS(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFE)) | (uint8)(BitVal))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXIPDU_PDUSTATUS(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),0,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the PDU activation status of Rx IPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXIPDU_PDUSTATUS(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_RXIPDU_PDUSTATUS(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),0)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs the Dead line monitoring Activation status for RxIPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXIPDU_DMSTATUS(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFD)) | (uint8)((BitVal)<<1))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXIPDU_DMSTATUS(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),1,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns  the Dead line monitoring Activation status for RxIPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXIPDU_DMSTATUS(ByteValue)         ((uint8)((ByteValue) >>1) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_RXIPDU_DMSTATUS(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),1)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs the Rx indication status for RxIPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXIPDU_INDICATION(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFB)) | (uint8)((BitVal)<<2))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXIPDU_INDICATION(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),2,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns  the Rx indication status for RxIPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXIPDU_INDICATION(ByteValue)         ((uint8)((ByteValue) >>2)  & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_RXIPDU_INDICATION(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),2)
#endif

/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs the processed status for Gw Rx Ipdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXIPDU_GWPDU_TOBEPROCESSED(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xF7)) | (uint8)((BitVal)<<3))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXIPDU_GWPDU_TOBEPROCESSED(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),3,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the processed status for Gw Rx Ipdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXIPDU_GWPDU_TOBEPROCESSED(ByteValue)         ((uint8)((ByteValue) >>3)  & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_RXIPDU_GWPDU_TOBEPROCESSED(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),3)
#endif

/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs the processed status for Large data TP Ipdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXIPDU_LARGEDATAINPROG(ByteValue,BitVal)        (ByteValue) = (((ByteValue) & ((uint8)0xEF)) | (uint8)((BitVal)<<4))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXIPDU_LARGEDATAINPROG(ByteValue,BitVal)        Bfx_PutBit_u8u8u8(&(ByteValue),4,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the processed status for Large data TP Ipdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXIPDU_LARGEDATAINPROG(ByteValue)       ((uint8)((ByteValue) >>4)  & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1   : 8-bit Base variable, where should be read from.
 * Argument 2   : bit position from, where the bit shall be read
 * Return value : Returns the bit value
 * */
#define Com_GetRamValue_RXIPDU_LARGEDATAINPROG(ByteValue)       Bfx_GetBit_u8u8_u8((ByteValue),4)
#endif

/* FC_VariationPoint_START */
/*
typedef struct
{
    uint8 LatestTransMode:2;    Stores the Latest Transmission Mode of the IPDU
    uint8 TxModeState:1;        Stores the Transmission mode State i.e TRUE/FALSE
    uint8 MixedPhaseShift:1;    MixedPhaseShift status. If this is SET, then current mode is MixedPhaseShift
    uint8 Not_Used:4;           Not used Bit field
}Com_TransModeType;
*/
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs Tx Ipdu Latest mode
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_LATESTMODE(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFC)) | (uint8)(BitVal))
#else
/* Bfx_PutBits_u8u8u8u8 : puts the bits, as per the bit pattern starting from a given position till the size in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bits should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: length of the bits
 * Argument 4: value of bits, bit pattern to be written
 * */
#define Com_SetRamValue_TXIPDU_LATESTMODE(ByteValue,BitVal)         Bfx_PutBits_u8u8u8u8(&(ByteValue),0,2,(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro returns Tx Ipdu Latest mode
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_LATESTMODE(ByteValue)         ((uint8)((ByteValue)) & (uint8)(0x03))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value    : Returns the bit pattern
 * */
#define Com_GetRamValue_TXIPDU_LATESTMODE(ByteValue)         Bfx_GetBits_u8u8u8_u8((ByteValue),0,2)
#endif

/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs Tx Ipdu State(current)
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_TXMODESTATE(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFB)) | (uint8)((BitVal) << 2))
#else
/* Bfx_PutBits_u8u8u8u8 : puts the bits, as per the bit pattern starting from a given position till the size in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bits should be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: length of the bits
 * Argument 4: value of bits, bit pattern to be written
 * */
#define Com_SetRamValue_TXIPDU_TXMODESTATE(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),2,(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro returns Tx Ipdu Tx mode state(current)
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_TXMODESTATE(ByteValue)         ((uint8)((ByteValue) >> 2) & (uint8)(0x01))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value    : Returns the bit pattern
 * */
#define Com_GetRamValue_TXIPDU_TXMODESTATE(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),2)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs MIXEDPHASESHIFT status of the tx Ipdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXIPDU_MIXEDPHASESHIFT(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xF7)) | (uint8)((BitVal)<<3))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_TXIPDU_MIXEDPHASESHIFT(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),3,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro returns true, if the Tx-IPdu current mode is selected as MIXEDPHASESHIFT
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_MIXEDPHASESHIFT(ByteValue)         ((uint8)((ByteValue) >> 3) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_TXIPDU_MIXEDPHASESHIFT(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),3)
#endif


/* FC_VariationPoint_START */

/*
 typedef struct
{
    uint8 TrigOnChange:1;    Flag is set to indicate that SignalGroup is triggered
    uint8 IsGwUpdated:1;     Flag is set to indicate that Gw-Tx-SignalGroup is updated
    uint8 Not_Used:6;        Not used Bit field

}Com_TxSignalGrpFlagType;

 */
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs Triggered_on_Change status of Tx Signal Group
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXSIGGRP_TRIGONCHNG(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFE)) | (uint8)(BitVal))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_TXSIGGRP_TRIGONCHNG(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),0,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Triggered_on_Change status of Tx Signal Group
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXSIGGRP_TRIGONCHNG(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_TXSIGGRP_TRIGONCHNG(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),0)
#endif


#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXSIGGRP_ISGWUPDTD(ByteValue,BitVal)       (ByteValue) = (((ByteValue) & ((uint8)0xFD)) | (uint8)((BitVal)<<1))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_TXSIGGRP_ISGWUPDTD(ByteValue,BitVal)       Bfx_PutBit_u8u8u8(&(ByteValue),1,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Triggered_on_Change status of Tx Signal Group
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXSIGGRP_ISGWUPDTD(ByteValue)      ((uint8)((ByteValue) >> 1) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1   : 8-bit Base variable, where should be read from.
 * Argument 2   : bit position from, where the bit shall be read
 * Return value : Returns the bit value
 * */
#define Com_GetRamValue_TXSIGGRP_ISGWUPDTD(ByteValue)      Bfx_GetBit_u8u8_u8((ByteValue),1)
#endif

/* FC_VariationPoint_START */
/*
typedef struct
{
    uint8 SigGrpNotify_Flag:1;      Flagged to provide notification callback
    uint8 SigGrpFillBit:1           Flagged to know whether the Rx-SigGrp is to be gatewayed
    uint8 RxDMFilteringPassed:1     Acc. to [SWS_Com_00793], After the RxTimeouttimer elapses, the first reception of
                                    the signal shall not be filtered if it is of MASKED_NEW_DIFFERS_MASKED_OLD type
    uint8 Not_Used:5;               Not used Bit field

}Com_RxSignalGrpFlagType;
*/
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the Notify flag for Rx Signal Groups
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXSIGGRP_SIGNOTIF(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFE)) | (uint8)(BitVal))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXSIGGRP_SIGNOTIF(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),0,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns  the Notify flag for Rx Signal Groups
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXSIGGRP_SIGNOTIF(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_RXSIGGRP_SIGNOTIF(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),0)
#endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the fill bit status for Rx Signal Groups
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXSIGGRP_SIGNALFILLBIT(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFD)) | (uint8)(BitVal<<1))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXSIGGRP_SIGNALFILLBIT(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),1,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns  the fill bit status for Rx Signal Groups
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXSIGGRP_SIGNALFILLBIT(ByteValue)        ((uint8)((ByteValue)>>1) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1   : 8-bit Base variable, where should be read from.
 * Argument 2   : bit position from, where the bit shall be read
 * Return value : Returns the bit value
 * */
#define Com_GetRamValue_RXSIGGRP_SIGNALFILLBIT(ByteValue)        Bfx_GetBit_u8u8_u8((ByteValue),1)
#endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the Rx deadline monitoring flag to ignore filter processing for Rx SignalGroups
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXSIGGRP_DMFILTERING(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFB)) | (uint8)(BitVal<<2))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXSIGGRP_DMFILTERING(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),2,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Rx deadline monitoring flag to ignore filter processing for Rx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXSIGGRP_DMFILTERING(ByteValue)         ((uint8)((ByteValue)>>2) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_RXSIGGRP_DMFILTERING(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),2)
#endif


/*
 typedef struct
{
    uint8 SigTxChangeMode:1;
    uint8 Not_Used:7;               Not used Bit field

}Com_TxGrpSignalFlagType;
 */
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 *  This Macro packs the SigTxChangeMode status of a Tx Group Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXGRPSIG_SIGCHNGMODE(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFE)) | (uint8)(BitVal))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_TXGRPSIG_SIGCHNGMODE(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),0,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the SigTxChangeMode status  of a Tx Group Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXGRPSIG_SIGCHNGMODE(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_TXGRPSIG_SIGCHNGMODE(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),0)
#endif

/* FC_VariationPoint_START */
/*
typedef struct
{
    uint8 SigTxChangeMode:1;
    uint8 Not_Used:7;               Not used Bit field

}Com_TxSignalFlagType;
*/
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the SigTxChangeMode status of a Tx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_TXSIG_SIGCHNGMODE(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFE)) | (uint8)(BitVal))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_TXSIG_SIGCHNGMODE(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),0,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the SigTxChangeMode status  of a Tx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXSIG_SIGCHNGMODE(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_TXSIG_SIGCHNGMODE(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),0)
#endif


/* FC_VariationPoint_START */
/*
typedef struct
{
    uint8 SigNotify_Flag:1;         Flag used for Conditional notification
    uint8 Sig_FillBit:1;            Flag used to know the status of Signal buffer
                                    COM_TRUE = Signal buffer updated
                                    COM_FALSE = Signal buffer not updated
    uint8 RxDMFilteringPassed:1;    Acc. to [SWS_Com_00793], After the RxTimeouttimer elapses, the first reception of
                                    the signal shall not be filtered if it is of MASKED_NEW_DIFFERS_MASKED_OLD type
    uint8 Not_Used:5;               Not used Bit field
}Com_RxSignalFlagType;
*/
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the Notify flag for Rx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXSIG_SIGNOTIF(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFE)) | (uint8)(BitVal))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXSIG_SIGNOTIF(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),0,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns  the Notify flag for Rx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXSIG_SIGNOTIF(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_RXSIG_SIGNOTIF(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),0)
#endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the Signal filled status flag for Rx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXSIG_SIGNALFILLBIT(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFD)) | (uint8)(BitVal<<1))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXSIG_SIGNALFILLBIT(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),1,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Signal filled status flag for Rx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXSIG_SIGNALFILLBIT(ByteValue)         ((uint8)((ByteValue)>>1) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_RXSIG_SIGNALFILLBIT(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),1)
#endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro packs the Rx deadline monitoring flag to ignore filter processing for Rx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_SetRamValue_RXSIG_DMFILTERING(ByteValue,BitVal)         (ByteValue) = (((ByteValue) & ((uint8)0xFB)) | (uint8)(BitVal<<2))
#else
/* Bfx_PutBit_u8u8u8 : Sets or clears a bit at a given position BitPn in the 8-bit variable Data.
 * Argument 1: pointer to a 8-bit variable, where a bit schould be modified
 * Argument 2: position of the bit in the byte
 * Argument 3: defines the new bit, 0 clears the bit and 1 sets the bit
 * */
#define Com_SetRamValue_RXSIG_DMFILTERING(ByteValue,BitVal)         Bfx_PutBit_u8u8u8(&(ByteValue),2,(boolean)(BitVal))
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Rx deadline monitoring flag to ignore filter processing for Rx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_RXSIG_DMFILTERING(ByteValue)         ((uint8)((ByteValue)>>2) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetRamValue_RXSIG_DMFILTERING(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),2)
#endif

/* FC_VariationPoint_START */
/* These Set of Macros return the CONST BitField values */

/*
typedef struct
{
    uint16 Signal_Processing:1;     DEFERRED/IMMEDIATE
    uint16 TMSCalclation:2;         Transmission mode status
                                        COM_MODE_VALID          If both the modes are configured
                                        COM_TRUEMODE_VALID      If TRUE mode is configured
                                        COM_FALSEMODE_VALID     If FALSE mode is configured
                                        COM_MODE_INVALID        If both the modes are not configured
    uint16 NoneModeTimeOut:1;       Deadline monitering for the IPdu for TX mode none
    uint16 ClearUpdBit:2            Status of where to clear update bit
    uint16 FilterEvalReq:1          Filter evaluation required
    uint16 IsDynPdu:1;              Is Ipdu of Dynamic type
    uint16 IsGwDestPdu:1;           If the Ipdu has Gw destination signal
    uint16 IsCalloutFrmTrigTrans:1  Tells if the callout is from Com_triggerTransmit
    isLargeDataPdu:1;               Indicates the Pdutype, Normal or TP type
    isCancelTransmitSupported:1;    Indicates if the cancel support is required or not
    IsMetaDataSupported:1;          Indicates if this Pdu has MetaData support enabled
    defaultTMSStatus:1;             Indicates the default TMS status of the Pdu after evaluating the Filter algo during code generation time
    ipduPartOfIpduGrp:1;            Indicates if the Ipdu is referring any IpduGroup
    uint16 Not_Used:1;              Unused bitfields, reserved for future use
} Com_TxIpduFlagType;
*/
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Signal Processing of the Tx and Rx IPDU
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXIPDU_SIGPROC(ByteValue)         ((uint16)(ByteValue) & ((uint16)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXIPDU_SIGPROC(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),0)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns TMS value of the Tx IPDU
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXIPDU_TMS(ByteValue)         ((uint16)((ByteValue) >>1) & ((uint16)0x03))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value    : Returns the bit pattern
 * */
#define Com_GetValue_TXIPDU_TMS(ByteValue)         Bfx_GetBits_u16u8u8_u16((ByteValue),1,2)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns None mode Dm status of the Tx IPDU
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXIPDU_NONETOSTATUS(ByteValue)         ((uint16)((ByteValue) >>3) & ((uint16)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXIPDU_NONETOSTATUS(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),3)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns ClearUpdateBit status of a Tx IPdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXIPDU_CLRUPDBIT(ByteValue)         ((uint16)((ByteValue) >>4) & ((uint16)0x03))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value    : Returns the bit pattern
 * */
#define Com_GetValue_TXIPDU_CLRUPDBIT(ByteValue)         Bfx_GetBits_u16u8u8_u16((ByteValue),4,2)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns TMS value of the Tx IPDU
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXIPDU_FILTEREVALREQ(ByteValue)         ((uint16)((ByteValue) >>6) & ((uint16)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXIPDU_FILTEREVALREQ(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),6)
#endif


/**
* @ingroup COM_PRV_H
*
*   This Macro unpacks and returns the Tx value of the flag which stores the information whether IPdu is transmitted in event triggered\n
*  call by application OR Com_MainFunctionTx()
*/
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetRamValue_TXIPDU_ISDYNIPDU(ByteValue)         ((uint16)((ByteValue) >>7)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
* Argument 1    : 8-bit Base variable, where should be read from.
* Argument 2    : bit position from, where the bit shall be read
* Return value    : Returns the bit value
* */
#define Com_GetRamValue_TXIPDU_ISDYNIPDU(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),7)
#endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the information if the Tx IPDU is Gw Pdu (has Gw signals)
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXIPDU_ISGWPDU(ByteValue)         ((uint16)((ByteValue) >>8)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXIPDU_ISGWPDU(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),8)
#endif
/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the information if the Tx IPDU is Gw Pdu (has Gw signals)
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXIPDU_ISCALLOUTFRMTRIGTRANS(ByteValue)         ((uint16)((ByteValue) >>9)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXIPDU_ISCALLOUTFRMTRIGTRANS(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),9)
#endif
/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the information if the Tx IPDU is Large data pdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXIPDU_ISLARGEDATAPDU(ByteValue)        ((uint16)((ByteValue) >>10)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1   : 8-bit Base variable, where should be read from.
 * Argument 2   : bit position from, where the bit shall be read
 * Return value : Returns the bit value
 * */
#define Com_GetValue_TXIPDU_ISLARGEDATAPDU(ByteValue)        Bfx_GetBit_u16u8_u8((ByteValue),10)
#endif
/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the information if the Tx IPDU is Large data pdu
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXIPDU_ISCANCELTRANSMITSUPPORTED(ByteValue)        ((uint16)((ByteValue) >>11)  & (uint16)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1   : 8-bit Base variable, where should be read from.
 * Argument 2   : bit position from, where the bit shall be read
 * Return value : Returns the bit value
 * */
#define Com_GetValue_TXIPDU_ISCANCELTRANSMITSUPPORTED(ByteValue)        Bfx_GetBit_u16u8_u8((ByteValue),11)
#endif

#ifdef COM_METADATA_SUPPORT
/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns boolean whether MetaData is supported for the IPdu
 */
# ifndef COM_ARSRVLIB_SUPPORT
#  define Com_GetValue_TXIPDU_ISMETADATASUPPORTED(ByteValue)  ( ( uint16 )( ( ByteValue ) >> 12 ) & ( ( uint16 )0x01 ) )
# else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 16-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value  : Returns the bit value
 * */
#  define Com_GetValue_TXIPDU_ISMETADATASUPPORTED(ByteValue)  Bfx_GetBit_u16u8_u8( ( ByteValue ), 12 )
# endif
#endif /* #ifdef COM_METADATA_SUPPORT */

/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns boolean of Default TMS status for the IPdu
 */
# ifndef COM_ARSRVLIB_SUPPORT
#  define Com_GetValue_TXIPDU_DEFAULTTMSSTATUS(ByteValue)  ( ( uint16 )( ( ByteValue ) >> 13 ) & ( ( uint16 )0x01 ) )
# else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 16-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value  : Returns the bit value
 * */
#  define Com_GetValue_TXIPDU_DEFAULTTMSSTATUS(ByteValue)  Bfx_GetBit_u16u8_u8( ( ByteValue ), 13 )
# endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns boolean if the Ipdu is referring any IpduGroup
 */
# ifndef COM_ARSRVLIB_SUPPORT
#  define Com_GetValue_TXIPDU_PDUWITHPDUGRP(ByteValue)  ( ( uint16 )( ( ByteValue ) >> 14 ) & ( ( uint16 )0x01 ) )
# else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 16-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value  : Returns the bit value
 * */
#  define Com_GetValue_TXIPDU_PDUWITHPDUGRP(ByteValue)  Bfx_GetBit_u16u8_u8( ( ByteValue ), 14 )
# endif

/* FC_VariationPoint_START */
/*
typedef struct
{
    uint8 Signal_Processing:1;  Signal Processing of each signal within the IPDU
                                     COM_TRUE  : COM_DEFERRED  IPdu Processing
                                     COM_FALSE : COM_IMMEDIATE IPdu Processing
    uint8 Notify_Cbk:1;         Set, If any of the Signal within the Ipdu has Notification cbk configured
    uint8 IsRxGwIPdu:1;         Set, if RxIpdu has any data to be Gatewayed
    uint8 Not_Used:5;           Not used Bit field

} Com_RxIpduFlagType;
*/
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Signal Processing of the Tx and Rx IPDU
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXIPDU_SIGPROC(ByteValue)         ((uint8)(ByteValue) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_RXIPDU_SIGPROC(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),0)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Notification Cbk status of the Rx IPDU
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXIPDU_NOTIFCBK(ByteValue)         ((uint8)((ByteValue) >> 1) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_RXIPDU_NOTIFCBK(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),1)
#endif

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the information if the Rx IPDU is Gw Pdu (has Gw signals or Gw Group signals)
 */
# ifndef COM_ARSRVLIB_SUPPORT
#  define Com_GetValue_RXIPDU_ISGWPDU(ByteValue)        ((uint8)((ByteValue) >> 2)  & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1         : 8-bit Base variable, where should be read from.
 * Argument 2         : bit position from, where the bit shall be read
 * Return value       : Returns the bit value
 * */
#  define Com_GetValue_RXIPDU_ISGWPDU(ByteValue)        Bfx_GetBit_u8u8_u8((ByteValue),2)
# endif /* #ifndef COM_ARSRVLIB_SUPPORT */
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

/* FC_VariationPoint_START */
/*
 typedef struct
{
    uint8 DataInvalidType:2;    DataInvalid Action
    uint8 FilterAlgorithm:4;    Filter Algorithm
    uint8 DataTimeoutType:1;    DataTimeout Action
    uint8 IsGwSignal:1;         Is gateway signal
}Com_RxSigPropType;
*/
/* FC_VariationPoint_END */

/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Rx Signal Invalid Action
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXSIG_INVACTION(ByteValue)         (((uint8)(ByteValue)) & ((uint8)0x03))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value    : Returns the bit pattern
 * */
#define Com_GetValue_RXSIG_INVACTION(ByteValue)         Bfx_GetBits_u8u8u8_u8((ByteValue),0,2)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Signal Filter algorithm of a Tx/RX Signal
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXSIG_FILTRALG(ByteValue)         ((uint8)((ByteValue) >> 2) & (uint8)(0x0F))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value    : Returns the bit pattern
 * */
#define Com_GetValue_RXSIG_FILTRALG(ByteValue)         Bfx_GetBits_u8u8u8_u8((ByteValue),2,4)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Rx Signal or Signal group Timeout Action
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXSIG_TOACTION(ByteValue)         (((uint8)((ByteValue) >> 6)) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_RXSIG_TOACTION(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),6)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the
 * COM_TRUE = If Rx signal is ComGw Src signal
 * COM_FALSE = If Rx signal is not ComGw Src signal
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXSIG_ISGWSIG(ByteValue)         (((uint8)((ByteValue) >> 7)) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_RXSIG_ISGWSIG(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),7)
#endif

/* FC_VariationPoint_START */
/*
typedef struct
{
    uint16 TransProp      :3;    Transfer Property of a Signal
    uint16 FilterAlgorithm:4;    Filter Algorithm
    uint16 DataInvalidType:1;    DataInvalid Action
    uint16 TimeOutEnabled :1;    Is time out configured for the signal

} Com_TxSigPropType;
 */
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Transfer property of a Tx Signal
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXSIG_TP(ByteValue)         ((uint16)(ByteValue) & (uint16)(0x07))
#else
/* Bfx_GetBits_u16u8u8_u16 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 16-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value  : Selected bitfield sequence as uint16
 * */
#define Com_GetValue_TXSIG_TP(ByteValue)          Bfx_GetBits_u16u8u8_u16((ByteValue),0,3)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Signal Filter algorithm of a Tx/RX Signal
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXSIG_FILTRALG(ByteValue)         ((uint16)((ByteValue) >> 3) & (uint16)(0x0F))
#else
/* Bfx_GetBits_u16u8u8_u16 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 16-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value  : Selected bitfield sequence as uint16
 * */
#define Com_GetValue_TXSIG_FILTRALG(ByteValue)         Bfx_GetBits_u16u8u8_u16((ByteValue),3,4)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Invalidation status for a Tx Signal
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXSIG_INVACTION(ByteValue)         ((uint16)((ByteValue) >> 7) & ((uint16)0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 16-bit variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value boolean  : Returns the bit value from Data at position BitPn
 * */
#define Com_GetValue_TXSIG_INVACTION(ByteValue)        Bfx_GetBit_u16u8_u8((ByteValue),7)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Tx Signal or Signal group Timeout enable status
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXSIG_TOENABLED(ByteValue)         (((uint16)((ByteValue) >> 8)) & ((uint16)0x01))
#else
/* Bfx_GetBit_u16u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXSIG_TOENABLED(ByteValue)         Bfx_GetBit_u16u8_u8((ByteValue),8)
#endif

/* FC_VariationPoint_START */
/*
typedef struct
{
     uint8 TransProp      :3;    Transfer Property of a SignalGroup
     uint8 TimeOutEnabled :1;    COM_TRUE : If time out is configured
                                 COM_FALSE: If time out is not configured for the signal group
     uint8 FilterEvalReq  :1;    COM_TRUE : If there exist group signal's with filter configured
                                 COM_FALSE: If there don't exist group signal's with filter configured
     uint8 UpdBitConf     :1;    COM_TRUE - Configured, COM_FALSE - Not configured
     uint8 ArrayAccess    :1;    COM_TRUE - Array Access Enabled, COM_FALSE - Array Access Disabled
     uint8 Not_Used       :1;    Unused Bit
} Com_TxSigGrpPropType;
 */
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Transfer property of a Tx Signal Group
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXSIGGRP_TP(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x07))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value  : Returns the bit pattern
 * */
#define Com_GetValue_TXSIGGRP_TP(ByteValue)         Bfx_GetBits_u8u8u8_u8((ByteValue),0,3)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Tx Signal group Timeout enable status
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXSIGGRP_TOENABLED(ByteValue)         (((uint8)((ByteValue) >> 3)) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXSIGGRP_TOENABLED(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),3)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Tx Signal group Filter Evaluation required status
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXSIGGRP_FILTEREVALREQ(ByteValue)         (((uint8)((ByteValue) >> 4)) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXSIGGRP_FILTEREVALREQ(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),4)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Tx Signal group UpdateBit configured status
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXSIGGRP_UPDBITCONF(ByteValue)         (((uint8)((ByteValue) >> 5)) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXSIGGRP_UPDBITCONF(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),5)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Tx Signal group Array Access enable status
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXSIGGRP_ARRAYACCESS(ByteValue)         (((uint8)((ByteValue) >> 6)) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value  : Returns the bit value
 * */
#define Com_GetValue_TXSIGGRP_ARRAYACCESS(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),6)
#endif


/* FC_VariationPoint_START */
/*
typedef struct
{
    uint8 DataInvalidType:2; DataInvalid Action
    uint8 DataTimeoutType:1; DataTimeout Action
    uint8 IsGwSigGrp:1;      SignalGroup Gateway
    uint8 UpdBitConf:1;      Is UpdateBit set
    uint8 ArrayAccess:1;     Is SignalGroup Array Access enabled
    uint8 FilterEvalReq:1;   Is Filter processing required at any of its GroupSignal
    uint8 Not_Used:1;
} Com_RxSigGrpPropertyType;
 */
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Rx Signal Invalid Action
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXSIGGRP_INVACTION(ByteValue)         (((uint8)(ByteValue)) & ((uint8)0x03))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value    : Returns the bit pattern
 * */
#define Com_GetValue_RXSIGGRP_INVACTION(ByteValue)         Bfx_GetBits_u8u8u8_u8((ByteValue),0,2)
#endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Rx Signal group Timeout Action
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXSIGGRP_TOACTION(ByteValue)         (((uint8)((ByteValue) >> 2)) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_RXSIGGRP_TOACTION(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),2)
#endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Rx Signal group gateway enabled status
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXSIGGRP_ISGWSIGGRP(ByteValue)       (((uint8)((ByteValue) >> 3)) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1   : 8-bit Base variable, where should be read from.
 * Argument 2   : bit position from, where the bit shall be read
 * Return value : Returns the bit value
 * */
#define Com_GetValue_RXSIGGRP_ISGWSIGGRP(ByteValue)       Bfx_GetBit_u8u8_u8((ByteValue),3)
#endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Rx Signal group UpdateBit configured status
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXSIGGRP_UPDBITCONF(ByteValue)       (((uint8)((ByteValue) >> 4)) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1   : 8-bit Base variable, where should be read from.
 * Argument 2   : bit position from, where the bit shall be read
 * Return value : Returns the bit value
 * */
#define Com_GetValue_RXSIGGRP_UPDBITCONF(ByteValue)       Bfx_GetBit_u8u8_u8((ByteValue),4)
#endif

/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Rx Signal group Array Access enabled status
 */
# ifndef COM_ARSRVLIB_SUPPORT
#  define Com_GetValue_RXSIGGRP_ARRAYACCESS(ByteValue)       (((uint8)((ByteValue) >> 5)) & ((uint8)0x01))
# else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1   : 8-bit Base variable, where should be read from.
 * Argument 2   : bit position from, where the bit shall be read
 * Return value : Returns the bit value
 * */
#  define Com_GetValue_RXSIGGRP_ARRAYACCESS(ByteValue)       Bfx_GetBit_u8u8_u8((ByteValue),5)
# endif

/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Rx Signal group Filter Evaluation required status
 */
# ifndef COM_ARSRVLIB_SUPPORT
#  define Com_GetValue_RXSIGGRP_FILTEREVALREQ(ByteValue)         (((uint8)((ByteValue) >> 6)) & ((uint8)0x01))
# else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value  : Returns the bit value
 * */
#  define Com_GetValue_RXSIGGRP_FILTEREVALREQ(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),6)
# endif

/* FC_VariationPoint_START */
/*
typedef struct
{
    uint8 Type:5;               DataType
    uint8 Endianess:1;          Endianess of the signal
    uint8 UpdBitConf:1;         Update Bit configured
    uint8 Not_Used:1;

} Com_GeneralType;
*/
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Signal Type of a Tx/RX Signals and Group Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_GEN_TYPE(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x1F))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value  : Returns the bit pattern
 * */
#define Com_GetValue_GEN_TYPE(ByteValue)         Bfx_GetBits_u8u8u8_u8((ByteValue),0,5)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Signal Endianess of a Tx/Rx Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_GEN_ENDIANESS(ByteValue)         ((uint8)((ByteValue) >> 5) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value  : Returns the bit value
 * */
#define Com_GetValue_GEN_ENDIANESS(ByteValue)         (Bfx_GetBit_u8u8_u8((ByteValue),5) ? \
                                                       COM_BIGENDIAN : COM_LITTLEENDIAN)
#endif

/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Signal Endianess of a Tx/RX Signals and Group Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_GEN_UPDBITCONF(ByteValue)         ((uint8)((ByteValue) >> 6) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value  : Returns the bit value
 * */
#define Com_GetValue_GEN_UPDBITCONF(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),6)
#endif


/* FC_VariationPoint_START */
/*
typedef struct
{
    uint8 Type:5;           DataType
    uint8 Endianess:1;      Endianess of Grp signal
    uint8 TransProp:1;      set, if the group signal contributes to the evaluation of transfer property signal group
    uint8 GrpSigInvalid:1;  Indicates if the group signal has configured invalid value

} Com_TxGrpSigPropertyType;
*/
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Signal Type of a Tx/RX Signals and Group Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXGRPSIG_TYPE(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x1F))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value    : Returns the bit pattern
 * */
#define Com_GetValue_TXGRPSIG_TYPE(ByteValue)         Bfx_GetBits_u8u8u8_u8((ByteValue),0,5)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns the Signal Endianess of a Tx Group Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXGRPSIG_ENDIANESS(ByteValue)         ((uint8)((ByteValue) >> 5) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXGRPSIG_ENDIANESS(ByteValue)         (Bfx_GetBit_u8u8_u8((ByteValue),5) ? \
                                                            COM_BIGENDIAN : COM_LITTLEENDIAN)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Tx Group Signal Transfer property
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXGRPSIG_TP(ByteValue)         ((uint8)((ByteValue) >> 6) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXGRPSIG_TP(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),6)
#endif


/**
 * @ingroup COM_PRV_H
 *
 *  This Macro unpacks and returns Invalidation status for a Tx Group signal
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_TXGRPSIG_INVACTION(ByteValue)         ((uint8)((ByteValue) >> 7) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value    : Returns the bit value
 * */
#define Com_GetValue_TXGRPSIG_INVACTION(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),7)
#endif


/* FC_VariationPoint_START */
/*
typedef struct
{
    uint8 Type:5;               DataType
    uint8 Endianess:1;          Endianess of the signal
    uint8 GrpSigInvalid:1;      Indicates if the group signal has configured invalid value
    uint8 Not_Used:1;
} Com_RxGrpSigPropertyType;
*/
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Signal Type of a Tx/RX Signals and Group Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXGRPSIG_TYPE(ByteValue)         ((uint8)(ByteValue) & (uint8)(0x1F))
#else
/* Bfx_GetBits_u8u8u8_u8 : Returns a sequence of the variable Data in form of a bitfield.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : Position of the first bit to be read out
 * Argument 3    : Length of the section, which should be read.
 * Return value  : Returns the bit pattern
 * */
#define Com_GetValue_RXGRPSIG_TYPE(ByteValue)         Bfx_GetBits_u8u8u8_u8((ByteValue),0,5)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns the Signal Endianess of a Rx Group Signals
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXGRPSIG_ENDIANESS(ByteValue)         ((uint8)((ByteValue) >> 5) & (uint8)(0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value  : Returns the bit value
 * */
#define Com_GetValue_RXGRPSIG_ENDIANESS(ByteValue)         (Bfx_GetBit_u8u8_u8((ByteValue),5) ? \
                                                            COM_BIGENDIAN : COM_LITTLEENDIAN)
#endif


/**
 * @ingroup COM_PRV_H
 *
 * This Macro unpacks and returns Invalidation status for a Tx Group signal
 */
#ifndef COM_ARSRVLIB_SUPPORT
#define Com_GetValue_RXGRPSIG_INVACTION(ByteValue)         ((uint8)((ByteValue) >> 6) & ((uint8)0x01))
#else
/* Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
 * Argument 1    : 8-bit Base variable, where should be read from.
 * Argument 2    : bit position from, where the bit shall be read
 * Return value  : Returns the bit value
 * */
#define Com_GetValue_RXGRPSIG_INVACTION(ByteValue)         Bfx_GetBit_u8u8_u8((ByteValue),6)
#endif

#define Com_SetUpdateBitValue(PduPtr,UpdBitPos,ByteNo)                                                          \
        /* Bfx_SetBit_u8u8 : Sets a bit at a defined position BitPn.
         * Argument 1    : pointer to the 8-bit variable, to be modified.
         * Argument 2    : position to be set
         * (UpdBitPos - (Com_BitpositionType)(ByteNo * 8)) =  Get the position of update bit in the byte
         * */                                                                                                   \
        Com_Setbit((uint8 *)((PduPtr)->BuffPtr + (ByteNo)),(uint8)((UpdBitPos) - (Com_BitpositionType)((ByteNo) * 8u)));

#define Com_ClearUpdateBitValue(PduPtr,UpdBitPos,ByteNo)                                                        \
            /* Bfx_ClrBit_u8u8 : Clears a bit at a defined position BitPn.
             * Argument 1    : pointer to the 8-bit variable, to be modified.
             * Argument 2    : position to be cleared
             * (UpdBitPos - (Com_BitpositionType)(ByteNo * 8)) ->  Get the position of update bit in the byte
             * */                                                                                               \
        Com_Clearbit((uint8 *)((PduPtr)->BuffPtr + (ByteNo)),(uint8)((UpdBitPos) - (Com_BitpositionType)((ByteNo) * 8u)));

#define Com_GetUpdateBitValue(dataPtr,UpdBitPos,ByteNo)                                                         \
        /* Bfx_GetBit_u8u8_u8 : Bfx_GetBit_u8u8_u8 : Gets a bit at the variable Data at a described position BitPn.
             * Argument 1    : 8-bit Base variable, where should be read from.
             * ((dataPtr) + (ByteNo)) -> Data pointer pointing to the byte with update bit position
             * Argument 2    : bit position from, where the bit shall be read
             * ((UpdBitPos) - (Com_BitpositionType)((ByteNo) * 8)))-> Get the position of update bit in the byte
             * Return value    : Returns the bit value
             * */                                                                                               \
        Com_Getbit(*((uint8 *)((dataPtr) + (ByteNo))),(uint8)((UpdBitPos) - (Com_BitpositionType)((ByteNo) * 8u)))

#ifdef COM_ARSRVLIB_SUPPORT
#define Com_Clearbit(ByteValue, Bitpos) Bfx_ClrBit_u8u8(ByteValue,Bitpos)
#else
#define Com_Clearbit(ByteValue, Bitpos) (*ByteValue) = ((uint8)(*ByteValue) & (~((uint8)(1u<<(Bitpos)))))
#endif /* #ifdef COM_ARSRVLIB_SUPPORT */

#ifdef COM_ARSRVLIB_SUPPORT
#define Com_Setbit(ByteValue, Bitpos) Bfx_SetBit_u8u8(ByteValue,Bitpos)
#else
#define Com_Setbit(ByteValue, Bitpos) (*ByteValue) = ((uint8)(*ByteValue) | ((uint8)(1u<<(Bitpos))))
#endif /* #ifdef COM_ARSRVLIB_SUPPORT */

#ifdef COM_ARSRVLIB_SUPPORT
#define Com_Getbit(ByteValue, Bitpos) Bfx_GetBit_u8u8_u8(ByteValue,Bitpos)
#else
#define Com_Getbit(ByteValue, Bitpos) (((ByteValue) & ((uint8)(1u << (Bitpos)))) != 0)
#endif /* #ifdef COM_ARSRVLIB_SUPPORT */

/**
 * @ingroup COM_PRV_H
 *
 * Macro to concatenate ELEMENTTYPE,PROP with Com_GetValue_ and pass ByteValue
 */
#define     Com_GetValue(ELEMENTTYPE,PROP,ByteValue)            Com_GetValue_##ELEMENTTYPE##PROP(ByteValue)
/**
 * @ingroup COM_PRV_H
 *
 * Macro to concatenate ELEMENTTYPE,PROP with Com_GetRamValue_ and pass ByteValue
 */
#define     Com_GetRamValue(ELEMENTTYPE,PROP,ByteValue)         Com_GetRamValue_##ELEMENTTYPE##PROP(ByteValue)
/**
 * @ingroup COM_PRV_H
 *
 * Macro to concatenate ELEMENTTYPE,PROP with Com_SetRamValue_ and pass ByteValue,Value
 */
#define     Com_SetRamValue(ELEMENTTYPE,PROP,ByteValue,Value)   Com_SetRamValue_##ELEMENTTYPE##PROP(ByteValue,Value)


 /**
  * @ingroup COM_PRV_H
  *
  * Macro to check if Rx Ipdu deadline monitoring status is Started /stopped
  */
#define COM_CheckRxIPduDMStatus(IpduId) \
                         (Com_GetRamValue(RXIPDU,_DMSTATUS,COM_GET_RXPDURAM_S(IpduId).RxFlags) != COM_STOP)

/*
 **********************************************************************************************************************
 * Extern declarations
 **********************************************************************************************************************
*/
 /* FC_VariationPoint_START */
#ifdef COM_RX_IPDUCONTROL_VIA_CALIBRATION
 /**
  * @ingroup COM_PRV_H
  *
  *  Service check if the Calibration variable value is set     \n
  *
  *  @param  IpduId - Id of the Tx Ipdu   \n
  *
  *  @return Boolean : Status value of the calibration variable \n
  */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
 FUNC(boolean, COM_CODE) COM_CheckRxIPduCalibrationVarStatus(VAR(PduIdType, AUTOMATIC) IpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_RX_IPDUCONTROL_VIA_CALIBRATION */

#ifdef COM_TX_IPDUCONTROL_VIA_CALIBRATION
 /**
  * @ingroup COM_PRV_H
  *
  *  Service check if the Calibration variable value is set     \n
  *
  *  @param  IpduId - Id of the Rx Ipdu   \n
  *
  *  @return Boolean : Status value of the calibration variable \n
  */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
 FUNC(boolean, COM_CODE) COM_CheckTxIPduCalibrationVarStatus(VAR(PduIdType, AUTOMATIC) IpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_TX_IPDUCONTROL_VIA_CALIBRATION */

#ifdef COM_TXPDU_TIMEPERIOD_VIA_CALIBRATION
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint16, COM_CODE)  COM_getTxIPduCyclePeriod(VAR(PduIdType, AUTOMATIC) IpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_TXPDU_TIMEPERIOD_VIA_CALIBRATION */

#ifdef COM_RXPDU_TIMEOUT_VIA_CALIBRATION
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint16, COM_CODE)  Com_GetRxIPduTimeoutCalibration(VAR(PduIdType, AUTOMATIC) IpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_RXPDU_TIMEOUT_VIA_CALIBRATION */

#ifdef COM_TXPDU_TIMEOFFSET_VIA_CALIBRATION
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint16, COM_CODE)  Com_GetTxIPduTimeOffsetCalibration(VAR(PduIdType, AUTOMATIC) IpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_TXPDU_TIMEOFFSET_VIA_CALIBRATION */

#ifdef COM_TXPDU_NUMOFREPETITION_VIA_CALIBRATION
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint8, COM_CODE)  Com_GetTxIPduNumberOfRepetitionCalibration(VAR(PduIdType, AUTOMATIC) IpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_TXPDU_NUMOFREPETITION_VIA_CALIBRATION */

#ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
 /**
  * @ingroup COM_PRV_H
  *
  *  Service to check if the IpduControl via RbaNds_EcuVariant value is set     \n
  *
  *  @param  IpduId - Id of the Tx Ipdu   \n
  *
  *  @return Boolean : Status of the Ipdu, either started or stopped \n
  */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
 FUNC(boolean, COM_CODE)  Com_CheckTxIPduControlViaRbaNdsEcuVariant(VAR(PduIdType, AUTOMATIC) IpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #ifdef COM_TXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */
#ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT
 /**
  * @ingroup COM_PRV_H
  *
  *  Service to check if the IpduControl via RbaNds_EcuVariant value is set     \n
  *
  *  @param  IpduId - Id of the Rx Ipdu   \n
  *
  *  @return Boolean : Status of the Ipdu, either started or stopped \n
  */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
 FUNC(boolean, COM_CODE)  Com_CheckRxIPduControlViaRbaNdsEcuVariant(VAR(PduIdType, AUTOMATIC) IpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_RXIPDUCONTROL_VIA_RBA_NDS_ECUVARIANT */
/* FC_VariationPoint_END */

 /**
  * @ingroup COM_PRV_H
  *
  *  Service for copying the bytes from source to destination              \n
  *
  *  @param  dest - Pointer to Destination           \n
  *  @param  src - Pointer to source \n
  *  @param  Length - No.of bytes   \n
  *
  *  @return None \n
  */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_ByteCopy( P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)   dest,
                                   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) src,
                                   VAR(uint32, AUTOMATIC)                   Len );
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 *  Service for copying the value passed to destination for UINT8_N Signal              \n
 *
 *  @param  dest - Pointer to Destination           \n
 *  @param  Init_Val - initial value \n
 *  @param  Length - No.of bytes   \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_ByteCopyInit( P2VAR(uint8, AUTOMATIC, COM_APPL_DATA)   dest,
                                       VAR(uint32, AUTOMATIC)                   Init_Val,
                                       VAR(uint32, AUTOMATIC)                   Len );
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 *  Internal function used for packing the signal into Ipdubuffer              \n
 *
 *  @param  Endianess - Little Endian/Big Endian           \n
 *  @param  Bit_Pos   - Position of the signal in the ipdu \n
 *  @param  Bitsize   - Size of the signal   \n
 *  @param  Src_Buf   - Buffer where signal data is available \n
 *  @param  Dest_Buf  - Pointer to buffer where the signal data needs to be packed \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_PackSignal( VAR(uint8, AUTOMATIC)                  Endianess,
                                     VAR(Com_BitpositionType, AUTOMATIC)    Bit_Pos,
                                     VAR(Com_BitsizeType, AUTOMATIC)        Bitsize,
                                     VAR(Com_SigMaxType, AUTOMATIC)         Src_Buf,
                                     P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) Dest_Buf );
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* FC_VariationPoint_START */
/**
 * @ingroup COM_PRV_H
 *
 *  Internal function used for packing the float64 signal/group signal into Ipdubuffer              \n
 *
 *  @param  Endianess - Little Endian/Big Endian           \n
 *  @param  Bit_Pos -   Position of the signal in the ipdu \n
 *  @param  Src_Buf -   Buffer where signal data is available \n
 *  @param  Dest_Buf -  ponter to buffer where the signal data needs to be packed \n
 *
 *  @return None \n
 */
#if (defined(COM_TXSIG_FLOAT64SUPP) || defined(COM_TXGRPSIG_FLOAT64SUPP) ||\
    (defined(COM_RX_SIGNALGROUP_ARRAY) && defined(COM_RXGRPSIG_FLOAT64SUPP)))
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_PackFloatSignal( VAR(uint8, AUTOMATIC)                  Endianess,
                                          VAR(Com_BitpositionType, AUTOMATIC)    Bit_Pos,
                                          VAR(float64,AUTOMATIC)                 Src,
                                          P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) Dest_Buf );
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 *  Internal function used for unpacking the signal from Ipdu Buffer              \n
 *
 *  @param  Endianess - Little Endian/Big Endian           \n
 *  @param  Bit_Pos -   Position of the signal in the ipdu \n
 *  @param  Bitsize -  Size of the signal   \n
 *  @param  Src_Buf -   Buffer where signal data is available \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(Com_SigMaxType, COM_CODE) Com_UnpackSignal( VAR(uint8, AUTOMATIC)                    Endianess,
                                                 VAR(Com_BitpositionType, AUTOMATIC)      Bit_Pos,
                                                 VAR(Com_BitsizeType, AUTOMATIC)          Bitsize,
                                                 P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) Src_Buf,
                                                 VAR(boolean, AUTOMATIC)                  IsSigned );

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* FC_VariationPoint_START */
/**
 * @ingroup COM_PRV_H
 *
 *  Internal function used for unpacking the signal from Ipdu Buffer              \n
 *
 *  @param  Endianess - Little Endian/Big Endian           \n
 *  @param  Bit_Pos -   Position of the signal in the ipdu \n
 *  @param  Src_Buf -   Buffer where signal data is available \n
 *
 *  @return float64 \n
 */
#if defined (COM_RXSIG_FLOAT64SUPP ) || defined(COM_RXGRPSIG_FLOAT64SUPP)
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(float64, COM_CODE) Com_UnPackFloatSignal( VAR(uint8, AUTOMATIC)                    Endianess,
                                               VAR(Com_BitpositionType, AUTOMATIC)      Bit_Pos,
                                               P2CONST(uint8,AUTOMATIC,COM_APPL_DATA)   Src_Buf);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif /* #if defined (COM_RXSIG_FLOAT64SUPP ) || defined(COM_RXGRPSIG_FLOAT64SUPP) */
/* FC_VariationPoint_END */
/**
 * @ingroup COM_PRV_H
 *
 *  Internal function used for unpacking a UINT8_N opaque signal from Ipdu Buffer              \n
 *
 *  @param  SignalLength - length of signal           \n
 *  @param  Bit_Pos -   Position of the signal in the ipdu \n
 *  @param  Src_Buf -   Buffer where signal data is available \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(uint32, COM_CODE) Com_UnpackOpaqueSignal( VAR(Com_BitpositionType, AUTOMATIC)      Bit_Pos,
                                               VAR(Com_BitsizeType, AUTOMATIC)          SignalLength,
                                               P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) Src_Buf);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 *  Internal function used for Transmitting Tx Ipdu's              \n
 *
 *  @param  ComTxPduId- Id of the Tx ipdu,Timeouts           \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_Prv_SendIpdu( VAR(PduIdType, AUTOMATIC) ComTxPduId,
                                       VAR(Com_SendIpduInfo, AUTOMATIC) sendIpduFlag_u16 );
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 *  Internal function used for Transmission Mode Selector              \n
 *
 *  @param   TxIpduId- Id of the Tx ipdu           \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void,COM_CODE) Com_TxChangeMode(VAR(Com_IpduIdType, AUTOMATIC) TxIpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#ifdef COM_TxFilters
/**
 * @ingroup COM_PRV_H
 *
 *  This function evaluates the current TMS of the IPDU, based on the corresponding signal which is updated.   \n
 *
 *  @param   SignalId - Id of the signal           \n
 *  @return  COM_TRUE
 *  @return  COM_FALSE
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(boolean,COM_CODE) Com_Prv_SigTxChangeMode(VAR(Com_SignalIdType, AUTOMATIC) SignalId,
                                                                VAR(uint32, AUTOMATIC) TxNewVal);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#ifdef COM_TX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 *  This function evaluates the current TMS of the IPDU, based on the corresponding signal group which is updated.   \n
 *  @param   SignalGroupId - Id of the signal Group          \n
 *  @return  COM_TRUE
 *  @return  COM_FALSE
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(boolean,COM_CODE) Com_Prv_SigGrpTxChangeMode(VAR(Com_SignalGroupIdType, AUTOMATIC) SignalGroupId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_TX_SIGNALGROUP */

#endif /* #ifdef COM_TxFilters */


/**
 * @ingroup COM_PRV_H
 *
 *  Internal function to process rx-signal's in the received Pdu \n
 *
 *  @param   RxPduId    - received Pdu-Id \n
 *  @param   PduInfoPtr - received Pdu info pointer \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_Prv_ProcessSignal(
                                VAR(PduIdType, AUTOMATIC)                       RxPduId,
                                P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr
                                          );
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


/**
 * @ingroup COM_PRV_H
 *
 *  Internal function to process rx-signal groups's in the received Pdu \n
 *
 *  @param   RxPduId    - received Pdu-Id \n
 *  @param   PduInfoPtr - received Pdu info pointer \n
 *
 *  @return None \n
 */
#ifdef COM_RX_SIGNALGROUP

#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_Prv_ProcessSignalGroup(
                                VAR(PduIdType, AUTOMATIC)                       RxPduId,
                                P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr
                                               );
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #ifdef COM_RX_SIGNALGROUP */


/**
 * @ingroup COM_PRV_H
 *
 *  Service to stop the ipdu             \n
 *
 *  @param   IpduId - Id of the ipdu         \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_TxIPduStop(VAR(Com_IpduIdType, AUTOMATIC) IpduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"


#ifdef COM_TX_SIGNALGROUP
    /**
     * @ingroup COM_PRV_H
     *
     *  Internal function used for copying the Signal Group data from the shadow buffer to IPDU buffer       \n
     *
     *  @param  SignalGrpId - Id of the SignalGroup       \n
     *
     *  @return None \n
     */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
    FUNC(void, COM_CODE) Com_ShadowBuffToIpduBuff(VAR(Com_SignalGroupIdType, AUTOMATIC) SignalGrpId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif

#if (defined(COM_TxFilters) || defined(COM_RxFilters))
    /**
     * @ingroup COM_PRV_H
     *
     *  Service used for filtering the signal depending on the conditions              \n
     *
     *  @param  Algorithm - Filter type           \n
     *  @param  NewVal  - New value for filtering \n
     *  @param  FilterIndex -  Filter Index \n
     *  @param  CallContext_b - Whether to increment occurrence for One_Every_N filters \n
     *
     *  @return   COM_TRUE
     *  @return   COM_FALSE
     */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(boolean, COM_CODE) Com_Filter( VAR(uint32, AUTOMATIC) NewVal, VAR(uint8, AUTOMATIC) Algorithm,
                                    VAR(Com_FilterType, AUTOMATIC) FilterIndex, VAR(boolean,AUTOMATIC) CallContext_b);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if (defined(COM_TxFilters) || defined(COM_RxFilters)) */

/**
 * @ingroup COM_PRV_H
 *
 *  Service called by the lower layer after an I-PDU has been received.      \n
 *
 *  @param  RxPduId    - Id of the Rx ipdu       \n
 *  @param  PduInfoPtr - Pointer to the buffer where the received data is stored       \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_ProcessRxIPdu(
                                VAR(PduIdType, AUTOMATIC)                       RxPduId,
                                P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA)  PduInfoPtr
                                      );
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 *  Internal function used for clearing the update bits of signals and signal groups in the IPdu      \n
 *
 *  @param  TxIPduConstPtr - Pointer to Tx ipdu static configuration      \n
 *  @param  Buff_Add - Pointer to the buffer where the received data is stored       \n
 *
 *  @return None \n
 */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC (void,COM_CODE) Com_Prv_ClearUpdateBits( Com_TxIpduConstPtrType TxIPduConstPtr);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
/* END: Internal Functions declarations */

/**
 * @ingroup COM_PRV_H
 *
 *  Internal function used updating the Rx signal gw queue      \n
 *
 *  @param  Com_SignalIdType - Value to be updated in the queue      \n
 *
 *  @return Std_ReturnType \n
 */

#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(Std_ReturnType, COM_CODE) Com_WriteSigGwReceiveQueue(PduIdType ComRxPduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_Prv_PackRxSignalGwBufferData(Com_RxSigConstPtrType RxSigConstPtr,Com_SignalIdType l_TxGwDestId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP))
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_Prv_PackRxGrpSigGwBufferData(Com_RxGrpSigConstPtrType RxGrpSigConstPtr,Com_TxIntGrpSignalIdType l_TxGwDestId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#endif /* #if defined COM_SIGNALGROUPGATEWAY && (defined (COM_RX_SIGNALGROUP) && defined (COM_TX_SIGNALGROUP)) */

#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_InternalProcessTxConfirmation(VAR(PduIdType, AUTOMATIC) TxPduId);
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

#define COM_START_SEC_CODE
#include "Com_MemMap.h"

extern FUNC(void, COM_CODE)     ComAppl_IncompatibleGenerator(void);
extern FUNC(uint8, COM_CODE)    Com_InternalSendSignal( VAR(Com_SignalIdType, AUTOMATIC)        SignalId,
                                                        P2CONST(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr );
extern FUNC(uint8, COM_CODE)    Com_InternalSendSignalGroup(VAR(Com_SignalGroupIdType, AUTOMATIC) SignalGroupId);
extern FUNC(void, COM_CODE)     Com_InternalInvalidateShadowSignal(VAR(Com_SignalIdType, AUTOMATIC) SignalId);
extern FUNC(void, COM_CODE)     Com_InternalMainFunctionRx( VAR(Com_MainFuncType,AUTOMATIC) RxMainFuncId );
extern FUNC(void, COM_CODE)     Com_InternalMainFunctionTx( VAR(Com_MainFuncType,AUTOMATIC) TxMainFuncId );

#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"

/*
 **********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************
*/

/*
 **********************************************************************************************************************
 * Variables
 **********************************************************************************************************************
*/

/********************* START: Configuration Part ********************************/


/*  START:  Global Variables declaration */

/**
 * @ingroup COM_PRV_H
 *
 * COM module status flag
 */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_StatusType,COM_VAR)          Com_Uninit_Flag;    /*COM module status flag*/
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 * Used to store PduLength of the Tx-Ipdu just befor PduR_ComTransmit() is called.
 */
#ifdef COM_TP_IPDUTYPE

#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(PduLengthType,COM_VAR)           Com_TpTxIpduLength_auo[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

#endif /* #ifdef COM_TP_IPDUTYPE */

#ifdef COM_RX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Array of Rx Signal group flags
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_RxSignalGrpFlagType,COM_VAR) Com_RxSignalGrpFlag[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
#endif
#ifdef COM_TX_SIGNALGROUP
#if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY)
/**
 * @ingroup COM_PRV_H
 *
 * Array of Tx Signal group flags
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_TxSignalGrpFlagType,COM_VAR) Com_TxSignalGrpFlag[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
#endif /* #if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY) */
/*extern VAR(Com_GrpSignalFlagType,COM_VAR) Com_GrpSignalFlag[];*/
/**
 * @ingroup COM_PRV_H
 *
 * Array of Tx  group Signal flags
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_TxGrpSignalFlagType,COM_VAR) Com_TxGrpSignalFlag[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
#endif
/**
 * @ingroup COM_PRV_H
 *
 * Array of Rx Ipdu Ram flags
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_RxIpduRamData,COM_VAR)    Com_RxIpduRam_s[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 * Array of Tx Ipdu Ram flags
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_TxIpduRamData,COM_VAR)    Com_TxIpduRam_s[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 * Array of Tx signal flags
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_TxSignalFlagType,COM_VAR)   Com_TxSignalFlag[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 * Array of Rx signal flags
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_RxSignalFlagType,COM_VAR)   Com_RxSignalFlag[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 * Counter for every Ipdu which gives number of Started Ipdu groups, this Ipdu is part of.
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"
extern VAR(uint8,COM_VAR)    Com_IpduCounter_s[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 * Counter for every Ipdu which gives number of Deadline monitoring enbaled Rx Ipdu groups, this Ipdu is part of.
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"
extern VAR(uint8,COM_VAR)    Com_IpduCounter_DM[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"

/**
 * @ingroup COM_PRV_H
 *
 * Vector which hold the status of each Ipdugroup
 * bit value - 1 : Ipdu group started
 * bit value - 0 : Ipdu group stopped
 */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_IpduGroupVector, COM_VAR) Com_IpduGrpVector;
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
/**
 * @ingroup COM_PRV_H
 *
 * Vector which hold the Deadline monitoring status of each Ipdugroup
 * bit value - 1 : Ipdu group started
 * bit value - 0 : Ipdu group stopped
 */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_IpduGroupVector, COM_VAR) Com_IpduGrpVector_DM;
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"

/*  START:  Global Variables declaration */

/* FC_VariationPoint_START */
#if (COM_COMMON_TX_PERIOD == STD_ON)
/**
 * @ingroup COM_PRV_H
 *
 * Value of reduced period
 */
#define COM_START_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
extern VAR(uint16,COM_VAR) Com_ReducedPeriod_u16;
#define COM_STOP_SEC_VAR_CLEARED_16
#include "Com_MemMap.h"
#endif /* COM_COMMON_TX_PERIOD */
/* FC_VariationPoint_END */

/**
 * @ingroup COM_PRV_H
 *
 * Gw Rx Ipdu queue
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(PduIdType,COM_VAR)    Com_RxGwQueue_aux[];
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/**
 * @ingroup COM_PRV_H
 *
 * Structure for Gw Rx queue
 */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"
extern VAR(Com_RxGwQueueRAMType, COM_VAR) Com_RxGwQueue_S;
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/**
 * @ingroup COM_PRV_H
 *
 * Internal pointer to config structure
 */
#define COM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"
extern P2CONST(Com_ConfigData_tst,AUTOMATIC,COM_APPL_CONST) Com_Prv_Config_pcst;
#define COM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Com_MemMap.h"

/**
 * @ingroup COM_PRV_H
 *
 * Structure which contains Tx mode NONE related values
 */
#define COM_START_SEC_CONST_UNSPECIFIED
#include "Com_MemMap.h"
extern CONST(Com_TransModeInfo ,COM_CONST) Com_NONE_TransModeInfo;
#define COM_STOP_SEC_CONST_UNSPECIFIED
#include "Com_MemMap.h"

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
# define COM_START_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
# include "Com_MemMap.h"
    extern VAR(boolean, AUTOMATIC) Com_IsGwSignalsUpdated_ab[ ];
# define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_BOOLEAN
# include "Com_MemMap.h"
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

/* Start Type Declarations*/
#define COM_START_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

#ifdef COM_SIGNALGATEWAY
/**
 * @ingroup COM_PRV_H
 *
 * Array of Signal gateway signal config structure
 */
extern CONST(Com_Prv_xGwMapSigCfg_tst ,COM_CONST) Com_Prv_xGwMapSigCfg_acst[];

extern CONST(Com_Prv_xGwMapSigIdCfg_tst ,COM_CONST) Com_Prv_xGwMapSigIdCfg_acst[];
#endif /* #ifdef COM_SIGNALGATEWAY */

#ifdef COM_SIGNALGROUPGATEWAY
/**
 * @ingroup COM_PRV_H
 *
 * Array of Signal gateway group signal config structure
 */
extern CONST(Com_Prv_xGwMapGrpSigCfg_tst ,COM_CONST) Com_Prv_xGwMapGrpSigCfg_acst[];

extern CONST(Com_Prv_xGwMapGrpSigIdCfg_tst ,COM_CONST) Com_Prv_xGwMapGrpSigIdCfg_acst[];
#endif /* #ifdef COM_SIGNALGROUPGATEWAY */
/**
 * @ingroup COM_PRV_H
 *
 * Signal Details
 */
/* ------------------------------------------------------------------------ */
/* Begin section for constants */
extern CONST(Com_Prv_xTxSigCfg_tst,COM_CONST)         Com_Prv_xTxSigCfg_acst[];           /*Signal Details*/
/**
 * @ingroup COM_PRV_H
 *
 * Signal Details
 */
extern CONST(Com_Prv_xRxSigCfg_tst,COM_CONST)         Com_Prv_xRxSigCfg_acst[];           /*Signal Details*/
/**
 * @ingroup COM_PRV_H
 *
 * Tx ipdu Details
 */
extern CONST(Com_Prv_xTxIpduInfoCfg_tst,COM_CONST)      Com_Prv_xTxIpduCfg_acst[];        /*Tx ipdu Details*/
/**
 * @ingroup COM_PRV_H
 *
 * Rx ipdu Details
 */
extern CONST(Com_Prv_xRxIpduInfoCfg_tst,COM_CONST)      Com_Prv_xRxIpduCfg_acst[];        /*Rx ipdu Details*/
/**
 * @ingroup COM_PRV_H
 *
 * Ipdu Group details
 */
extern CONST(Com_Prv_xIpduGrpInfoCfg_tst,COM_CONST)     Com_Prv_xIpduGrpCfg_acst[];          /*Ipdu Group details*/

/**
 * @ingroup COM_PRV_H
 *
 * Internal config structure
 */

extern CONST(Com_ConfigData_tst,COM_APPL_CONST) Com_Prv_Config_cst;

/**
 * @ingroup COM_PRV_H
 *
 * Array of Ipdu's placed in the order they occur in Ipdu groups
 */
extern CONST(Com_IpduIdType ,COM_CONST) Com_Prv_xIPduGrp_IpduRefCfg_au8[];

#ifdef COM_TX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Array of internal Tx signal group configuration
 */
extern CONST(Com_Prv_xTxSigGrpCfg_tst,COM_CONST)  Com_Prv_xTxSigGrpCfg_acst[];

#ifdef COM_TX_SIGNALGROUP_ARRAY
/**
 * @ingroup COM_PRV_H
 *
 * Array of internal Tx signal group Array configuration information
 */
extern CONST(Com_Prv_xTxSigGrpArrayCfg_tst, COM_CONST) Com_Prv_xTxSigGrpArrayCfg_acst[];

#endif

#endif

#ifdef COM_RX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Array of internal Rx signal group configuration
 */
extern CONST(Com_Prv_xRxSigGrpCfg_tst,COM_CONST)  Com_Prv_xRxSigGrpCfg_acst[];

#ifdef COM_RX_SIGNALGROUP_ARRAY
/**
 * @ingroup COM_PRV_H
 *
 * Array of internal Tx signal group Array configuration information
 */
extern CONST(Com_Prv_xRxSigGrpArrayCfg_tst, COM_CONST) Com_Prv_xRxSigGrpArrayCfg_acst[];
#endif

#endif
/* Structure for GroupSignals */
#ifdef COM_TX_SIGNALGROUP
/*extern CONST(Com_GrpSigInfo,COM_CONST)  Com_GrpSig[];*/
/**
 * @ingroup COM_PRV_H
 *
 * Array of Tx  group signal flags
 */
extern CONST(Com_Prv_xTxGrpSigCfg_tst,COM_CONST)  Com_Prv_xTxGrpSigCfg_acst[];
#endif

#ifdef COM_RX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Array of Rx group signal flags
 */
extern CONST(Com_Prv_xRxGrpSigCfg_tst,COM_CONST)  Com_Prv_xRxGrpSigCfg_acst[];
#endif
/**
 * @ingroup COM_H
 *
 * COM multiple MainFunction configuration for multi-core processing
 */
extern CONST(Com_MainFunctionCfgType, COM_CONST) Com_MainFunctionCfg[];

/**
 * @ingroup COM_PRV_H
 *
 * Set of Rx-buffers generated for each ComMainFunction
 */
extern CONST(Com_Prv_xRxRamBuf_tst, COM_CONST) Com_Prv_xRxRamBuf_acst[];

#ifdef COM_TX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Set of Tx-GroupSignal shadow buffers generated for each ComMainFunction
 */
extern CONST(Com_Prv_xTxSigGrpRamBuf_tst, COM_CONST) Com_Prv_xTxSigGrpRamBuf_acst[];

#endif /* #ifdef COM_TX_SIGNALGROUP */

/* ------------------------------------------------------------------------ */
/* End section for constants */
#define COM_STOP_SEC_CONFIG_DATA_POSTBUILD_UNSPECIFIED
#include "Com_MemMap.h"

/*  END: Type Declaration  */

/*  END:  Global Variables declaration */

/********************* END: Configuration Part ********************************/

/* START: Access MACROS */
#if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST TxIpdu data when post build variant is selected
 */
#define COM_GET_TX_IPDU_CONSTDATA(COMPDUID)                &(Com_Prv_Config_pcst->Com_TxIpdu_pcst[COMPDUID])
/**
 * @ingroup COM_PRV_H
 *
* Access macro to get internal HandleId from external HandleId , for Tx IPdus
 */
#define COM_GET_TX_IPDU_ID(COMPDUID)                       (Com_Prv_Config_pcst->Com_TxIpduMapping_pcst[COMPDUID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST RxIpdu data when post build variant is selected
 */
#define COM_GET_RX_IPDU_CONSTDATA(COMPDUID)                &(Com_Prv_Config_pcst->Com_RxIpdu_pcst[COMPDUID])
 /**
 * @ingroup COM_PRV_H
 *
* Access macro to get internal HandleId from external HandleId , for Rx IPdus
 */
#define COM_GET_RX_IPDU_ID(COMPDUID)                       (Com_Prv_Config_pcst->Com_RxIpduMapping_pcst[COMPDUID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST IpduGroup when post build variant is selected
 */
#define COM_GET_IPDUGRP_CONSTDATA(COMPDUID)                &(Com_Prv_Config_pcst->Com_IpduGrp_pcst[COMPDUID])
 /**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId for Ipdu group
 */
#define COM_GET_IPDUGRP_ID(COMPDUID)                       (Com_Prv_Config_pcst->Com_IpduGrpMapping_pcu16[COMPDUID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST IpduGroupIpduRef when post build variant is selected
 */
#define COM_GET_IPDUGRP_IPDUREF_CONSTDATA(COMPDUID)        &(Com_Prv_Config_pcst->Com_IPduGrp_IpduRef_pcst[COMPDUID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST RxSignal when post build variant is selected
 */
#define COM_GET_RXSIG_CONSTDATA(COMSIGID)                  &(Com_Prv_Config_pcst->Com_RxSig_pcst[COMSIGID])
  /**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Rx Signals
 */
#define COM_GET_RXSIGNAL_ID(COMSIGID)                      (Com_Prv_Config_pcst->Com_RxSignalMapping_pct[COMSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST TxSignal when post build variant is selected
 */
#define COM_GET_TXSIG_CONSTDATA(COMSIGID)                  &(Com_Prv_Config_pcst->Com_TxSig_pcst[COMSIGID])
 /**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Tx Signals
 */
#define COM_GET_TXSIGNAL_ID(COMSIGID)                      (Com_Prv_Config_pcst->Com_TxSignalMapping_pct[COMSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST TxSignalGroup when post build variant is selected
 */
#define COM_GET_TXSIGGRP_CONSTDATA(COMSIGGRPID)            &(Com_Prv_Config_pcst->Com_TxSigGrp_pcst[COMSIGGRPID])
  /**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Tx Signal Groups
 */
#define COM_GET_TXSIGNALGRP_ID(COMSIGGRPID)                (Com_Prv_Config_pcst->Com_TxSignalGrpMapping_pct[COMSIGGRPID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST RxSignalGroup when post build variant is selected
 */
#define COM_GET_RXSIGGRP_CONSTDATA(COMSIGGRPID)            &(Com_Prv_Config_pcst->Com_RxSigGrp_pcst[COMSIGGRPID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Rx Signal Groups
 */
#define COM_GET_RXSIGNALGRP_ID(COMSIGGRPID)                (Com_Prv_Config_pcst->Com_RxSignalGrpMapping_pct[COMSIGGRPID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST TxGroupSignal when post build variant is selected
 */
#define COM_GET_TXGRPSIG_CONSTDATA(COMGRPSIGID)            &(Com_Prv_Config_pcst->Com_TxGrpSig_pcst[COMGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Tx Group Signals
 */
#define COM_GET_TXGRPSIGNAL_ID(COMGRPSIGID)                (Com_Prv_Config_pcst->Com_TxGrpSignalMapping_pct[COMGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST RxGroupSignal when post build variant is selected
 */
#define COM_GET_RXGRPSIG_CONSTDATA(COMGRPSIGID)            &(Com_Prv_Config_pcst->Com_RxGrpSig_pcst[COMGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Rx Group Signals
 */
#define COM_GET_RXGRPSIGNAL_ID(COMGRPSIGID)                (Com_Prv_Config_pcst->Com_RxGrpSignalMapping_pct[COMGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST Gateway Signal when post build variant is selected
 */
#define COM_GET_GWSIG_CONSTDATA(COMGWID)                   &(Com_Prv_Config_pcst->Com_GwSig_pcst[COMGWID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST Gateway Signal Id mapping when post build variant is selected
 */
#define COM_GET_GWSIGIDMAP_CONSTDATA(COMGWID)              &(Com_Prv_Config_pcst->Com_GwSigIdMapping_pcst[COMGWID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST Gateway GroupSignal when post build variant is selected
 */
#define COM_GET_GWGRPSIG_CONSTDATA(COMGWGRPSIGID)          &(Com_Prv_Config_pcst->Com_GwGrpSig_pcst[COMGWGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST Gateway GroupSignal Id mapping table when post build variant is selected
 */
#define COM_GET_GWGRPSIGIDMAP_CONSTDATA(COMGWGRPSIGID)     &(Com_Prv_Config_pcst->Com_GwGrpSigIdMapping_pcst[COMGWGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Total number of IpduGroups configured when post build variant is selected
 */
#define COM_GET_NUM_TOTAL_IPDU_GRP                         (Com_Prv_Config_pcst->noOfIpduGroup)

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get total number of Pdus mapped to the last IPduGroup member
 * in the generated table - Com_Prv_xIpduGrpCfg_acst.
 * when pre-compile variant is selected
 */
#define COM_GET_NUM_IPDUS_IN_LAST_IPDUGRP                  (Com_Prv_Config_pcst->noOfIpdusInLastIpduGrp)

#if defined ( COM_TX_SIGNALGROUP_ARRAY )
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Tx SignalGroup Array Constant Structure, when post-build variant is selected
 */
# define COM_GET_TXSIGGRP_ARRAY_CONSTDATA(SIGGRP_INDEX)    &(Com_Prv_Config_pcst->Com_TxSigGrpArray_pcst[(SIGGRP_INDEX)])
#endif

#if defined ( COM_RX_SIGNALGROUP_ARRAY )
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Rx SignalGroup Array Constant Structure, when post-build variant is selected
 */
# define COM_GET_RXSIGGRP_ARRAY_CONSTDATA(SIGGRP_INDEX)    &(Com_Prv_Config_pcst->Com_RxSigGrpArray_pcst[(SIGGRP_INDEX)])
#endif

#if defined(COM_TXSIG_FLOAT64SUPP ) || defined(COM_TXGRPSIG_FLOAT64SUPP) || defined(COM_RXGRPSIG_FLOAT64SUPP) || defined(COM_RXSIG_FLOAT64SUPP)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the FloatValArray when post build variant is selected
 */
#define COM_GET_FLOAT_ARRAY_VALUE(val)                ((Com_Prv_Config_pcst->floatValArray_pcaf64)[(val)])
#endif

#if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the Com_MaskX when post build variant is selected
 */
#define COM_GET_MASK_X(FilterIndex)                   (Com_Prv_Config_pcst->Com_MaskX_pcst[FilterIndex])
#endif /* #if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX) */

#ifdef COM_F_MASKEDNEWDIFFERSOLD
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the Com_Mask when post build variant is selected
 */
#define COM_GET_MASK(FilterIndex)                     (Com_Prv_Config_pcst->Com_Mask_pu32[FilterIndex])
#endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */

#if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the Com_POSMinMax when post build variant is selected
 */
#define COM_GET_POSMINMAX(FilterIndex)                (Com_Prv_Config_pcst->Com_POSMinMax_pcst[FilterIndex])
#endif /* #if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS) */

#if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the Com_NEGMinMax when post build variant is selected
 */
#define COM_GET_NEGMINMAX(FilterIndex)                (Com_Prv_Config_pcst->Com_NEGMinMax_pcst[FilterIndex])
#endif /* #if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG) */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get ComMainFunction-Rx configuration, when post build variant is selected
 */
#define COM_GET_MAINFUNCTION_CFG(MF_ID)          (Com_Prv_Config_pcst->Com_MainFunctionCfg_pcst[ MF_ID ])

#ifdef COM_F_ONEEVERYN
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST array which holds the OneEveryN filter configuration in PB variant
 */
#define COM_GET_ONEEVERYN_CONST(FILTER_INDEX)              (Com_Prv_Config_pcst->Com_OneEveryN_Const_pcst[FILTER_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds the OneEveryN variable in PB variant
 */
#define COM_GET_ONEEVERYN(FILTER_INDEX)                    (Com_Prv_Config_pcst->Com_OneEveryN_pe[FILTER_INDEX])
#endif /* #ifdef COM_F_ONEEVERYN */

/* RAM Variables access macros */
#ifdef COM_F_MASKEDNEWDIFFERSOLD
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds the signals old value during PB variant
 */
#define COM_GET_F_OLDVALUE(FILTER_INDEX)                   (Com_Prv_Config_pcst->Com_F_OldVal_pu32[FILTER_INDEX])
#endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */

#if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds the signals/signal groups old value for
 * triggered on change transfer property signals/signal groups during PB variant
 */
#define COM_GET_OLDVALTRIGONCHANGE(OLDVAL_INDEX)           (Com_Prv_Config_pcst->Com_OldValTrigOnChng_puo[OLDVAL_INDEX])
#endif /* #if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC) */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint8 buffer in PB variant
 */
#define COM_GET_SIGTYPEUINT8_BUFF(MF_ID,SIGNAL_INDEX)            (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SigType_pu8[SIGNAL_INDEX])
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint8_dyn buffer in PB variant
 */
#define COM_GET_SIGTYPEUINT8DYN_BUFF(MF_ID,SIGNAL_INDEX)         (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SigType_dyn_pu8[SIGNAL_INDEX])
#endif /* #ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT */
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint16 buffer in PB variant
 */
#define COM_GET_SIGTYPEUINT16_BUFF(MF_ID,SIGNAL_INDEX)           (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SigType_pu16[SIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint32 buffer in PB variant
 */
#define COM_GET_SIGTYPEUINT32_BUFF(MF_ID,SIGNAL_INDEX)           (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SigType_pu32[SIGNAL_INDEX])

#ifdef COM_RXSIG_INT64
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint64 buffer in PB variant
 */
#define COM_GET_SIGTYPEUINT64_BUFF(MF_ID,SIGNAL_INDEX)           (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SigType_pu64[SIGNAL_INDEX])
#endif /* #ifdef COM_RXSIG_INT64 */

#ifdef COM_RXSIG_FLOAT64SUPP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type float64 buffer in PB variant
 */
#define COM_GET_SIGTYPEFLOAT64_BUFF(MF_ID,SIGNAL_INDEX)          (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SigType_pf64[SIGNAL_INDEX])
#endif /* #ifdef COM_RXSIG_FLOAT64SUPP */

#ifdef COM_RX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type uint8 buffer in PB variant
 */
#define COM_GET_RXGRPSIGTYPEUINT8_BUFF(MF_ID,GRPSIGNAL_INDEX)    (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_RxGrpSigType_pu8[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type uint16 buffer in PB variant
 */
#define COM_GET_RXGRPSIGTYPEUINT16_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_RxGrpSigType_pu16[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type uint32 buffer in PB variant
 */
#define COM_GET_RXGRPSIGTYPEUINT32_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_RxGrpSigType_pu32[GRPSIGNAL_INDEX])

#ifdef COM_RXGRPSIG_INT64
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type uint64 buffer in PB variant
 */
#define COM_GET_RXGRPSIGTYPEUINT64_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_RxGrpSigType_pu64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_RXGRPSIG_INT64 */

#ifdef COM_RXGRPSIG_FLOAT64SUPP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type float64 buffer in PB variant
 */
#define COM_GET_RXGRPSIGTYPEFLOAT64_BUFF(MF_ID,GRPSIGNAL_INDEX)  (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_RxGrpSigType_pf64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_RXGRPSIG_FLOAT64SUPP */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type uint8 buffer in PB variant
 */
#define COM_GET_SECRXGRPSIGTYPEU8_BUFF(MF_ID,GRPSIGNAL_INDEX)    (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SecondRxGrpSigType_pu8[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type uint16 buffer in PB variant
 */
#define COM_GET_SECRXGRPSIGTYPEU16_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SecondRxGrpSigType_pu16[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type uint32 buffer in PB variant
 */
#define COM_GET_SECRXGRPSIGTYPEU32_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SecondRxGrpSigType_pu32[GRPSIGNAL_INDEX])

#ifdef COM_RXGRPSIG_INT64
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type uint64 buffer in PB variant
 */
#define COM_GET_SECRXGRPSIGTYPEU64_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SecondRxGrpSigType_pu64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_RXGRPSIG_INT64 */

#ifdef COM_RXGRPSIG_FLOAT64SUPP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type float64 buffer in PB variant
 */
#define COM_GET_SECRXGRPSIGTYPEFLOAT64_BUFF(MF_ID,GRPSIGNAL_INDEX) (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_SecondRxGrpSigType_pf64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_RXGRPSIG_FLOAT64SUPP */

#ifdef COM_RX_SIGNALGROUP_ARRAY
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx SignalGroup of Array type buffer in PB variant
 */
# define COM_GET_RXSIGGRP_ARRAY_BUFF(MF_ID,SIGGRP_INDEX)        (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_RxSigGrpArrayBuf_pu8[(SIGGRP_INDEX)])
#endif

#endif/* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_TX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type uint8 buffer in PB variant
 */
#define COM_GET_TXGRPSIGTYPEU8_BUFF(MF_ID,GRPSIGNAL_INDEX)       \
        (Com_Prv_Config_pcst->Com_TxSigGrpRamBuf_pcst[MF_ID].Com_TxGrpSigType_pu8[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type uint16 buffer in PB variant
 */
#define COM_GET_TXGRPSIGTYPEU16_BUFF(MF_ID,GRPSIGNAL_INDEX)      \
        (Com_Prv_Config_pcst->Com_TxSigGrpRamBuf_pcst[MF_ID].Com_TxGrpSigType_pu16[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type uint32 buffer in PB variant
 */
#define COM_GET_TXGRPSIGTYPEU32_BUFF(MF_ID,GRPSIGNAL_INDEX)      \
        (Com_Prv_Config_pcst->Com_TxSigGrpRamBuf_pcst[MF_ID].Com_TxGrpSigType_pu32[GRPSIGNAL_INDEX])

#ifdef COM_TXGRPSIG_INT64
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type uint64 buffer in PC variant
 */
#define COM_GET_TXGRPSIGTYPEU64_BUFF(MF_ID,GRPSIGNAL_INDEX)      \
        (Com_Prv_Config_pcst->Com_TxSigGrpRamBuf_pcst[MF_ID].Com_TxGrpSigType_pu64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_TXGRPSIG_INT64 */

#ifdef COM_TXGRPSIG_FLOAT64SUPP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type float64 buffer in PB variant
 */
#define COM_GET_TXGRPSIGTYPEFLOAT64_BUFF(MF_ID,GRPSIGNAL_INDEX)  \
        (Com_Prv_Config_pcst->Com_TxSigGrpRamBuf_pcst[MF_ID].Com_TxGrpSigType_pf64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_TXGRPSIG_FLOAT64SUPP */

#endif/* #ifdef COM_TX_SIGNALGROUP */

#ifdef COM_TP_IPDUTYPE
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds PduLength for tx-ipdu in PC variant
 */
#define COM_GET_TPTXIPDULENGTH_AUO(COMTXIPDUID)            (Com_Prv_Config_pcst->Com_TpTxIpduLength_pauo[COMTXIPDUID])
#endif /* #ifdef COM_TP_IPDUTYPE */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Tx ipdu RAM information in PB variant
 */
#define COM_GET_TXPDURAM_S(COMTXIPDUID)                    (Com_Prv_Config_pcst->Com_TxIpduRam_ps[COMTXIPDUID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Rx ipdu RAM information in PB variant
 */
#define COM_GET_RXPDURAM_S(COMRXIPDUID)                    (Com_Prv_Config_pcst->Com_RxIpduRam_ps[COMRXIPDUID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Tx Signal flag information in PB variant
 */
#define COM_GET_TXSIGNALFLAG(COMTXSIGID)                    (Com_Prv_Config_pcst->Com_TxSignalFlag_ps[COMTXSIGID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Rx Signal flag information in PB variant
 */
#define COM_GET_RXSIGNALFLAG(COMRXSIGID)                    (Com_Prv_Config_pcst->Com_RxSignalFlag_ps[COMRXSIGID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Ipdu counter information in PB variant
 */
#define COM_GET_IPDUCOUNTER_S(COMPDUID)                    (Com_Prv_Config_pcst->Com_IpduCounter_ps[COMPDUID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Ipdu counter deadline monitoring information in PB variant
 */
#define COM_GET_IPDUCOUNTER_DM(COMPDUID)                    (Com_Prv_Config_pcst->Com_IpduCounter_DM_ps[COMPDUID])

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Rx Gateway queue in PB variant
 */
#define COM_GET_RXGWQUEUE_AUX(COMPDUID)                    (Com_Prv_Config_pcst->Com_RxGwQueue_paux[COMPDUID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for Rx Gateway queue in PB variant
 */
#define COM_GET_RXGWQUEUE_S                                (Com_Prv_Config_pcst->Com_RxGwQueue_ps)
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

#ifdef COM_RX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for Rx signal group flag in PB variant
 */
#define COM_GET_RXSIGGRPFLAG(COMSIGGRPID)                  (Com_Prv_Config_pcst->Com_RxSignalGrpFlag_ps[COMSIGGRPID])
#endif /* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_TX_SIGNALGROUP
#if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for Tx signal group flag in PB variant
 */
#define COM_GET_TXSIGGRPFLAG(COMSIGGRPID)                  (Com_Prv_Config_pcst->Com_TxSignalGrpFlag_ps[COMSIGGRPID])
#endif /* #if defined(COM_EffectiveSigGrpTOC) || defined (COM_SIGNALGROUPGATEWAY) */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for Tx group signal flag in PB variant
 */
#define COM_GET_TXGRPSIGFLAG(COMGRPSIGID)                  (Com_Prv_Config_pcst->Com_TxGrpSignalFlag_ps[COMGRPSIGID])
#endif /* #ifdef COM_TX_SIGNALGROUP */

#ifdef COM_RxIPduDeferredProcessing
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for deferred buffer in PB variant
 */
#define COM_GET_DEFERREDBUFFER(MF_ID)                    (Com_Prv_Config_pcst->Com_RxRamBuf_pcst[MF_ID].Com_DeferredBuffer_pu8)
#endif /* #ifdef COM_RxIPduDeferredProcessing */

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds if Gw Signals are updated in RxIndication, in PB variant
 */
#define COM_GET_IS_GWSIG_UPDATED(RXPDU_ID)                 (Com_Prv_Config_pcst->Com_IsGwSignalsUpdated_pab[(RXPDU_ID)])
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

#define COM_GET_NUM_RX_SIGNALS                             (Com_Prv_Config_pcst->noOfRxSignals)

#define COM_GET_NUM_TX_SIGNALS                             (Com_Prv_Config_pcst->noOfTxSignals)

#define COM_GET_NUM_TX_SIGNALGRP                           (Com_Prv_Config_pcst->noOfTxSignalGroup)

#define COM_GET_NUM_RX_SIGNALGRP                           (Com_Prv_Config_pcst->noOfRxSignalGroup)

#define COM_GET_NUM_TX_GRPSIGNALS                          (Com_Prv_Config_pcst->noOfTxGroupSignal)

#define COM_GET_NUM_RX_GRPSIGNALS                          (Com_Prv_Config_pcst->noOfRxGroupSignal)

#define COM_GET_NUM_TX_IPDU                                (Com_Prv_Config_pcst->noOfTxIpdu)

#define COM_GET_NUM_RX_IPDU                                (Com_Prv_Config_pcst->noOfRxIpdu)

#define COM_GET_NUM_GWSRC_SIGNALS                          (Com_Prv_Config_pcst->noOfGwSrcSignals)

#define COM_GET_NUM_GWSRC_GRPSIGNALS                       (Com_Prv_Config_pcst->noOfGwSrcGrpSignals)

#define COM_GET_NUM_GWSRC_IPDUS                            (Com_Prv_Config_pcst->noOfGwSrcIpdu)

#define COM_GET_NUM_GRPSIG_NOGW                            (Com_Prv_Config_pcst->noOfGrpSigNoGw)

#ifdef COM_F_ONEEVERYN
#define COM_GET_NUM_SIG_GRPSIG_WITH_ONE_EVERY_N_FILTER     (Com_Prv_Config_pcst->noOfOneEveryNSigAndGrpSig)
#endif /* #ifdef COM_F_ONEEVERYN */

#else
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST TxIpdu data when pre-compile variant is selected
 */
#define COM_GET_TX_IPDU_CONSTDATA(COMPDUID)                (&Com_Prv_xTxIpduCfg_acst[COMPDUID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Tx IPdus
 * For pre-compile variant no mapping required, both the values will be same
 */
#define COM_GET_TX_IPDU_ID(COMPDUID)                       (COMPDUID)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST RxIpdu data when pre-compile variant is selected
 */
#define COM_GET_RX_IPDU_CONSTDATA(COMPDUID)                (&Com_Prv_xRxIpduCfg_acst[COMPDUID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Rx IPdus
 * For pre-compile variant no mapping required, both the values will be same
 */
#define COM_GET_RX_IPDU_ID(COMPDUID)                       (COMPDUID)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST IpduGroup when pre-compile variant is selected
 */
#define COM_GET_IPDUGRP_CONSTDATA(COMPDUID)                (&Com_Prv_xIpduGrpCfg_acst[COMPDUID])
 /**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId for Ipdu group
 * For pre-compile variant no mapping required, both the values will be same
 */
#define COM_GET_IPDUGRP_ID(COMPDUID)                       (COMPDUID)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST IpduGroupIpduRef when pre-compile variant is selected
 */
#define COM_GET_IPDUGRP_IPDUREF_CONSTDATA(COMPDUID)        (&Com_Prv_xIPduGrp_IpduRefCfg_au8[COMPDUID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST RxSignal when pre-compile variant is selected
 */
#define COM_GET_RXSIG_CONSTDATA(COMSIGID)                  (&Com_Prv_xRxSigCfg_acst[COMSIGID])
  /**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Rx Signals
 * For pre-compile variant no mapping required, both the values will be same
 */
 #define COM_GET_RXSIGNAL_ID(COMSIGID)                     (COMSIGID)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST TxSignal when pre-compile variant is selected
 */
#define COM_GET_TXSIG_CONSTDATA(COMSIGID)                  (&Com_Prv_xTxSigCfg_acst[COMSIGID])
 /**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Tx Signals
 * For pre-compile variant no mapping required, both the values will be same
 */
#define COM_GET_TXSIGNAL_ID(COMSIGID)                      (COMSIGID)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST TxSignalGroup when pre-compile variant is selected
 */
#define COM_GET_TXSIGGRP_CONSTDATA(COMSIGGRPID)            (&Com_Prv_xTxSigGrpCfg_acst[COMSIGGRPID])
   /**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Tx Signal Groups
 * For pre-compile variant no mapping required, both the values will be same
 */
#define COM_GET_TXSIGNALGRP_ID(COMSIGGRPID)                (COMSIGGRPID)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST RxSignalGroup when pre-compile variant is selected
 */
#define COM_GET_RXSIGGRP_CONSTDATA(COMSIGGRPID)            (&Com_Prv_xRxSigGrpCfg_acst[COMSIGGRPID])
    /**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Rx Signal Groups
 * For pre-compile variant no mapping required, both the values will be same
 */
#define COM_GET_RXSIGNALGRP_ID(COMSIGID)                   (COMSIGID)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST TxGroupSignal when pre-compile variant is selected
 */
#define COM_GET_TXGRPSIG_CONSTDATA(COMGRPSIGID)            (&Com_Prv_xTxGrpSigCfg_acst[COMGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Tx Group Signals
 * For pre-compile variant no mapping required, both the values will be same
 */
#define COM_GET_TXGRPSIGNAL_ID(COMGRPSIGID)                (COMGRPSIGID)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST RxGroupSignal when pre-compile variant is selected
 */
#define COM_GET_RXGRPSIG_CONSTDATA(COMGRPSIGID)            (&Com_Prv_xRxGrpSigCfg_acst[COMGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get internal HandleId from external HandleId , for Rx Group Signals
  * For pre-compile variant no mapping required, both the values will be same
 */
#define COM_GET_RXGRPSIGNAL_ID(COMGRPSIGID)                (COMGRPSIGID)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST Gateway Signal when pre-compile variant is selected
 */
#define COM_GET_GWSIG_CONSTDATA(COMGWID)                   (&Com_Prv_xGwMapSigCfg_acst[COMGWID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST Gateway Signal Id mapping when pre-compile variant is selected
 */
#define COM_GET_GWSIGIDMAP_CONSTDATA(COMGWID)              (&Com_Prv_xGwMapSigIdCfg_acst[COMGWID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST Gateway GroupSignal when pre-compile variant is selected
 */
#define COM_GET_GWGRPSIG_CONSTDATA(COMGWGRPSIGID)          (&Com_Prv_xGwMapGrpSigCfg_acst[COMGWGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST Gateway GroupSignal Id mapping when pre-compile variant is selected
 */
#define COM_GET_GWGRPSIGIDMAP_CONSTDATA(COMGWGRPSIGID)     (&Com_Prv_xGwMapGrpSigIdCfg_acst[COMGWGRPSIGID])
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Total number of IpduGroups configured when pre-compile variant is selected
 */
#define COM_GET_NUM_TOTAL_IPDU_GRP                         COM_NUM_TOTAL_IPDU_GRP

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Total number of IPdus in the last configured IpduGroup configured
 * when pre-compile variant is selected
 */
#define COM_GET_NUM_IPDUS_IN_LAST_IPDUGRP                  COM_NUM_IPDUS_IN_LAST_IPDUGRP

#if defined ( COM_TX_SIGNALGROUP_ARRAY )
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Tx SignalGroup Array Constant Structure, when pre-compile variant is selected
 */
# define COM_GET_TXSIGGRP_ARRAY_CONSTDATA(SIGGRP_INDEX)    (&Com_Prv_xTxSigGrpArrayCfg_acst[(SIGGRP_INDEX)])
#endif

#if defined ( COM_RX_SIGNALGROUP_ARRAY )
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Rx SignalGroup Array Constant Structure, when pre-compile variant is selected
 */
# define COM_GET_RXSIGGRP_ARRAY_CONSTDATA(SIGGRP_INDEX)    (&Com_Prv_xRxSigGrpArrayCfg_acst[(SIGGRP_INDEX)])
#endif

#if defined(COM_TXSIG_FLOAT64SUPP ) || defined(COM_TXGRPSIG_FLOAT64SUPP) || defined(COM_RXGRPSIG_FLOAT64SUPP) || defined(COM_RXSIG_FLOAT64SUPP)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the FloatValArray when pre-compile variant is selected
 */
#define COM_GET_FLOAT_ARRAY_VALUE(val)                     (Com_FloatValArray[(val)])
#endif

#if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the Com_MaskX when PC variant is selected
 */
#define COM_GET_MASK_X(FilterIndex)                        (Com_MaskX[FilterIndex])
#endif /* #if defined (COM_F_MASKEDNEWEQUALSX ) || defined(COM_F_MASKEDNEWDIFFERSX) */

#ifdef COM_F_MASKEDNEWDIFFERSOLD
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the Com_Mask when PC variant is selected
 */
#define COM_GET_MASK(FilterIndex)                          (Com_Mask[FilterIndex])
#endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */

#if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the Com_POSMinMax when PC variant is selected
 */
#define COM_GET_POSMINMAX(FilterIndex)                     (Com_POSMinMax[FilterIndex])
#endif /* #if defined (COM_F_NEWISWITHIN_POS) || defined(COM_F_NEWISOUTSIDE_POS) */

#if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get Init Value from the Com_NEGMinMax when PC variant is selected
 */
#define COM_GET_NEGMINMAX(FilterIndex)                     (Com_NEGMinMax[FilterIndex])
#endif /* #if defined (COM_F_NEWISWITHIN_NEG) || defined(COM_F_NEWISOUTSIDE_NEG) */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get ComMainFunction configuration, when post build variant is selected
 */
#define COM_GET_MAINFUNCTION_CFG( MF_ID )                  (Com_MainFunctionCfg[ MF_ID ])

#ifdef COM_F_ONEEVERYN
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get CONST array which holds the OneEveryN filter configuration in PC variant
 */
#define COM_GET_ONEEVERYN_CONST(FILTER_INDEX)              (Com_OneEveryN_Const[FILTER_INDEX])

/* RAM Variables access macros */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds the OneEveryN variable in PC variant
 */
#define COM_GET_ONEEVERYN(FILTER_INDEX)                    (Com_OneEveryN[FILTER_INDEX])
#endif /* #ifdef COM_F_ONEEVERYN */

#ifdef COM_F_MASKEDNEWDIFFERSOLD
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds the signals old value during PC variant
 */
#define COM_GET_F_OLDVALUE(FILTER_INDEX)                   (Com_F_OldVal[FILTER_INDEX])
#endif /* #ifdef COM_F_MASKEDNEWDIFFERSOLD */

#if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds the signals/signal groups old value for
 * triggered on change transfer property signals/signal groups during PB variant
 */
#define COM_GET_OLDVALTRIGONCHANGE(OLDVAL_INDEX)           (Com_OldValTrigOnChng[OLDVAL_INDEX])
#endif /* #if defined(COM_EffectiveSigTOC) || defined(COM_EffectiveSigGrpTOC) */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint8 buffer in PC variant
 */
#define COM_GET_SIGTYPEUINT8_BUFF(MF_ID,SIGNAL_INDEX)            (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SigType_pu8[SIGNAL_INDEX])
#ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint8_dyn buffer in PC variant
 */
#define COM_GET_SIGTYPEUINT8DYN_BUFF(MF_ID,SIGNAL_INDEX)         (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SigType_dyn_pu8[SIGNAL_INDEX])
#endif/* #ifdef COM_RX_DYNAMIC_SIGNAL_SUPPORT */
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint16 buffer in PC variant
 */
#define COM_GET_SIGTYPEUINT16_BUFF(MF_ID,SIGNAL_INDEX)           (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SigType_pu16[SIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint32 buffer in PC variant
 */
#define COM_GET_SIGTYPEUINT32_BUFF(MF_ID,SIGNAL_INDEX)           (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SigType_pu32[SIGNAL_INDEX])

#ifdef COM_RXSIG_INT64
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type uint64 buffer in PC variant
 */
#define COM_GET_SIGTYPEUINT64_BUFF(MF_ID,SIGNAL_INDEX)           (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SigType_pu64[SIGNAL_INDEX])
#endif /* #ifdef COM_RXSIG_INT64 */

#ifdef COM_RXSIG_FLOAT64SUPP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Signal type float64 buffer in PC variant
 */
#define COM_GET_SIGTYPEFLOAT64_BUFF(MF_ID,SIGNAL_INDEX)          (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SigType_pf64[SIGNAL_INDEX])
#endif /* #ifdef COM_RXSIG_FLOAT64SUPP */

#ifdef COM_RX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type uint8 buffer in PC variant
 */
#define COM_GET_RXGRPSIGTYPEUINT8_BUFF(MF_ID,GRPSIGNAL_INDEX)    (Com_Prv_xRxRamBuf_acst[MF_ID].Com_RxGrpSigType_pu8[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type uint16 buffer in PC variant
 */
#define COM_GET_RXGRPSIGTYPEUINT16_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_xRxRamBuf_acst[MF_ID].Com_RxGrpSigType_pu16[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type uint32 buffer in PC variant
 */
#define COM_GET_RXGRPSIGTYPEUINT32_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_xRxRamBuf_acst[MF_ID].Com_RxGrpSigType_pu32[GRPSIGNAL_INDEX])

#ifdef COM_RXGRPSIG_INT64
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type uint64 buffer in PC variant
 */
#define COM_GET_RXGRPSIGTYPEUINT64_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_xRxRamBuf_acst[MF_ID].Com_RxGrpSigType_pu64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_RXGRPSIG_INT64 */

#ifdef COM_RXGRPSIG_FLOAT64SUPP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx GroupSignal type float64 buffer in PC variant
 */
#define COM_GET_RXGRPSIGTYPEFLOAT64_BUFF(MF_ID,GRPSIGNAL_INDEX)  (Com_Prv_xRxRamBuf_acst[MF_ID].Com_RxGrpSigType_pf64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_RXGRPSIG_FLOAT64SUPP */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type uint8 buffer in PC variant
 */
#define COM_GET_SECRXGRPSIGTYPEU8_BUFF(MF_ID,GRPSIGNAL_INDEX)    (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SecondRxGrpSigType_pu8[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type uint16 buffer in PC variant
 */
#define COM_GET_SECRXGRPSIGTYPEU16_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SecondRxGrpSigType_pu16[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type uint32 buffer in PC variant
 */
#define COM_GET_SECRXGRPSIGTYPEU32_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SecondRxGrpSigType_pu32[GRPSIGNAL_INDEX])

#ifdef COM_RXGRPSIG_INT64
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type uint64 buffer in PC variant
 */
#define COM_GET_SECRXGRPSIGTYPEU64_BUFF(MF_ID,GRPSIGNAL_INDEX)   (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SecondRxGrpSigType_pu64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_RXGRPSIG_INT64 */

#ifdef COM_RXGRPSIG_FLOAT64SUPP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Second Rx GroupSignal type float64 buffer in PC variant
 */
#define COM_GET_SECRXGRPSIGTYPEFLOAT64_BUFF(MF_ID,GRPSIGNAL_INDEX) (Com_Prv_xRxRamBuf_acst[MF_ID].Com_SecondRxGrpSigType_pf64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_RXGRPSIG_FLOAT64SUPP */

#ifdef COM_RX_SIGNALGROUP_ARRAY
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Rx SignalGroup of Array type buffer in PC variant
 */
# define COM_GET_RXSIGGRP_ARRAY_BUFF(MF_ID,SIGGRP_INDEX)           (Com_Prv_xRxRamBuf_acst[MF_ID].Com_RxSigGrpArrayBuf_pu8[SIGGRP_INDEX])
#endif

#endif/* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_TX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type uint8 buffer in PC variant
 */
#define COM_GET_TXGRPSIGTYPEU8_BUFF(MF_ID,GRPSIGNAL_INDEX)              \
        (Com_Prv_xTxSigGrpRamBuf_acst[MF_ID].Com_TxGrpSigType_pu8[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type uint16 buffer in PC variant
 */
#define COM_GET_TXGRPSIGTYPEU16_BUFF(MF_ID,GRPSIGNAL_INDEX)             \
        (Com_Prv_xTxSigGrpRamBuf_acst[MF_ID].Com_TxGrpSigType_pu16[GRPSIGNAL_INDEX])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type uint32 buffer in PC variant
 */
#define COM_GET_TXGRPSIGTYPEU32_BUFF(MF_ID,GRPSIGNAL_INDEX)             \
        (Com_Prv_xTxSigGrpRamBuf_acst[MF_ID].Com_TxGrpSigType_pu32[GRPSIGNAL_INDEX])

#ifdef COM_TXGRPSIG_INT64
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type uint64 buffer in PC variant
 */
#define COM_GET_TXGRPSIGTYPEU64_BUFF(MF_ID,GRPSIGNAL_INDEX)             \
        (Com_Prv_xTxSigGrpRamBuf_acst[MF_ID].Com_TxGrpSigType_pu64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_TXGRPSIG_INT64 */

#ifdef COM_TXGRPSIG_FLOAT64SUPP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds Tx GroupSignal type float64 buffer in PC variant
 */
#define COM_GET_TXGRPSIGTYPEFLOAT64_BUFF(MF_ID,GRPSIGNAL_INDEX)         \
        (Com_Prv_xTxSigGrpRamBuf_acst[MF_ID].Com_TxGrpSigType_pf64[GRPSIGNAL_INDEX])
#endif /* #ifdef COM_TXGRPSIG_FLOAT64SUPP */

#endif/* #ifdef COM_TX_SIGNALGROUP */

#ifdef COM_TP_IPDUTYPE
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds PduLength for tx-ipdu in PC variant
 */
#define COM_GET_TPTXIPDULENGTH_AUO(COMTXIPDUID)            (Com_TpTxIpduLength_auo[COMTXIPDUID])
#endif /* #ifdef COM_TP_IPDUTYPE */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Tx ipdu RAM information in PC variant
 */
#define COM_GET_TXPDURAM_S(COMTXIPDUID)                    (Com_TxIpduRam_s[COMTXIPDUID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Rx ipdu RAM information in PC variant
 */
#define COM_GET_RXPDURAM_S(COMRXIPDUID)                    (Com_RxIpduRam_s[COMRXIPDUID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Tx Signal flag information in PC variant
 */
#define COM_GET_TXSIGNALFLAG(COMTXSIGID)                    (Com_TxSignalFlag[COMTXSIGID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Rx Signal flag information in PC variant
 */
#define COM_GET_RXSIGNALFLAG(COMRXSIGID)                    (Com_RxSignalFlag[COMRXSIGID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Ipdu counter information in PC variant
 */
#define COM_GET_IPDUCOUNTER_S(COMPDUID)                    (Com_IpduCounter_s[COMPDUID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Ipdu counter deadline monitoring information in PC variant
 */
#define COM_GET_IPDUCOUNTER_DM(COMPDUID)                    (Com_IpduCounter_DM[COMPDUID])

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds array for Rx Gateway queue in PC variant
 */
#define COM_GET_RXGWQUEUE_AUX(COMPDUID)                    (Com_RxGwQueue_aux[COMPDUID])

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for Rx Gateway queue in PC variant
 */
#define COM_GET_RXGWQUEUE_S                                &(Com_RxGwQueue_S)
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

#ifdef COM_RX_SIGNALGROUP
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for Rx signal group flag in PC variant
 */
#define COM_GET_RXSIGGRPFLAG(COMSIGGRPID)                  (Com_RxSignalGrpFlag[COMSIGGRPID])
#endif /* #ifdef COM_RX_SIGNALGROUP */

#ifdef COM_TX_SIGNALGROUP
#if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for Tx signal group flag in PC variant
 */
#define COM_GET_TXSIGGRPFLAG(COMSIGGRPID)                  (Com_TxSignalGrpFlag[COMSIGGRPID])
#endif /* #if defined(COM_EffectiveSigGrpTOC) || defined(COM_SIGNALGROUPGATEWAY) */

/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for Tx group signal flag in PC variant
 */
#define COM_GET_TXGRPSIGFLAG(COMGRPSIGID)                  (Com_TxGrpSignalFlag[COMGRPSIGID])
#endif /* #ifdef COM_TX_SIGNALGROUP */

#ifdef COM_RxIPduDeferredProcessing
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds structure for deferred buffer in PC variant
 */
#define COM_GET_DEFERREDBUFFER(MF_ID)                   (Com_Prv_xRxRamBuf_acst[MF_ID].Com_DeferredBuffer_pu8)
#endif /* #ifdef COM_RxIPduDeferredProcessing */

#if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY)
/**
 * @ingroup COM_PRV_H
 *
 * Access macro to get RAM array which holds if Gw Signals are updated in RxIndication, in PC variant
 */
#define COM_GET_IS_GWSIG_UPDATED(RXIPDU_ID)                (Com_IsGwSignalsUpdated_ab[(RXIPDU_ID)])
#endif /* #if defined (COM_SIGNALGATEWAY) || defined (COM_SIGNALGROUPGATEWAY) */

#define COM_GET_NUM_RX_SIGNALS                             (COM_NUM_RX_SIGNALS)

#define COM_GET_NUM_TX_SIGNALS                             (COM_NUM_TX_SIGNALS)

#define COM_GET_NUM_TX_SIGNALGRP                           (COM_NUM_TX_SIGNALGRP)

#define COM_GET_NUM_RX_SIGNALGRP                           (COM_NUM_RX_SIGNALGRP)

#define COM_GET_NUM_TX_GRPSIGNALS                          (COM_NUM_TX_GRPSIGNALS)

#define COM_GET_NUM_RX_GRPSIGNALS                          (COM_NUM_RX_GRPSIGNALS)

#define COM_GET_NUM_TX_IPDU                                (COM_NUM_TX_IPDU)

#define COM_GET_NUM_RX_IPDU                                (COM_NUM_RX_IPDU)

#define COM_GET_NUM_GWSRC_SIGNALS                          (COM_NUM_GWSRC_SIGNALS)

#define COM_GET_NUM_GWSRC_GRPSIGNALS                       (COM_NUM_GWSRC_GRPSIGNALS)

#define COM_GET_NUM_GWSRC_IPDUS                            (COM_NUM_GWSRC_IPDUS)

#define COM_GET_NUM_GRPSIG_NOGW                            (COM_NUM_GRPSIG_NOGW)

#ifdef COM_F_ONEEVERYN
#define COM_GET_NUM_SIG_GRPSIG_WITH_ONE_EVERY_N_FILTER     (COM_NUM_OF_SIG_GRPSIG_WITH_ONE_EVERY_N)
#endif /* #ifdef COM_F_ONEEVERYN */

#endif /* #if (COM_CONFIGURATION_VARIANT != COM_VARIANT_PRE_COMPILE) */

/* Access macros for calibration */

#ifdef COM_RxIPduTimeout
/* FC_VariationPoint_START */
#ifdef COM_RXPDU_TIMEOUT_VIA_CALIBRATION
/* Call function to get the calibration value RxIPduTimeout */
#define COM_GET_RXIPDU_TIMEOUT(ComRxPduId, RxIPduConstPtr)             Com_GetRxIPduTimeoutCalibration((PduIdType)ComRxPduId)
#else
/* FC_VariationPoint_END */
/* Get RxIPduTimeout */
#define COM_GET_RXIPDU_TIMEOUT(ComRxPduId, RxIPduConstPtr)             (RxIPduConstPtr->Timeout_Fact)
/* FC_VariationPoint_START */
#endif /* #ifdef COM_RXPDU_TIMEOUT_VIA_CALIBRATION */
/* FC_VariationPoint_END */
#endif /* #ifdef COM_RxIPduTimeout */

/* FC_VariationPoint_START */
#ifdef COM_TXPDU_TIMEOFFSET_VIA_CALIBRATION
/* Call function to get the calibration value TxIPduTimeOffset */
#define COM_GET_TXIPDU_TIMEOFFSET(ComTxPduId, CurrentTxModePtr)        Com_GetTxIPduTimeOffsetCalibration((PduIdType)ComTxPduId)
#else
/* FC_VariationPoint_END */
/* Get TxIPduTimeOffset */
#define COM_GET_TXIPDU_TIMEOFFSET(ComTxPduId, CurrentTxModePtr)        (CurrentTxModePtr->TimeOffsetFact)
/* FC_VariationPoint_START */
#endif /* COM_TXPDU_TIMEOFFSET_VIA_CALIBRATION */
/* FC_VariationPoint_END */

/* FC_VariationPoint_START */
#ifdef COM_TXPDU_TIMEPERIOD_VIA_CALIBRATION
/* Call function to get the calibration value TxIPduTimePeriod */
#define COM_GET_TXIPDU_TIMEPERIOD(ComTxPduId, CurrentTxModePtr)        COM_getTxIPduCyclePeriod((PduIdType)ComTxPduId)
#elif (COM_COMMON_TX_PERIOD == STD_ON)
/* Get TxIPduTimePeriod */
#define COM_GET_TXIPDU_TIMEPERIOD(ComTxPduId, CurrentTxModePtr)        ((Com_ReducedPeriod_u16 == 0)? \
                                                                       ((CurrentTxModePtr)->TimePeriodFact) : Com_ReducedPeriod_u16)
#else
/* FC_VariationPoint_END */
/* Get TxIPduTimePeriod */
#define COM_GET_TXIPDU_TIMEPERIOD(ComTxPduId, CurrentTxModePtr)        ((CurrentTxModePtr)->TimePeriodFact)
/* FC_VariationPoint_START */
#endif
/* FC_VariationPoint_END */

/* FC_VariationPoint_START */
#ifdef COM_TXPDU_NUMOFREPETITION_VIA_CALIBRATION
/* Call function to get the calibration value TxIPduNumOfRepetition */
#define COM_GET_TXIPDU_NUMOFREPETITION(ComTxPduId, CurrentTxModePtr)   Com_GetTxIPduNumberOfRepetitionCalibration((PduIdType)ComTxPduId)
#else
/* FC_VariationPoint_END */
/* Get TxIPduNumOfRepetition */
#define COM_GET_TXIPDU_NUMOFREPETITION(ComTxPduId, CurrentTxModePtr)   (CurrentTxModePtr->NumOfRepetitions)
/* FC_VariationPoint_START */
#endif /* #define COM_TXPDU_NUMOFREPETITION_VIA_CALIBRATION */
/* FC_VariationPoint_END */

/* END: Access MACROS */

#endif /* COM_PRV_H */
