


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#ifndef COM_PBCFG_H
#define COM_PBCFG_H

/*  START: Local #defines  */

#define COM_NUM_RX_SIGNALS        2u
#define COM_NUM_TX_SIGNALS        8u
#define COM_NUM_TX_IPDU         1u
#define COM_NUM_RX_IPDU         1u
#define COM_NUM_GWSRC_SIGNALS  0u
#define COM_NUM_GWSRC_GRPSIGNALS  0u
#define COM_NUM_GWSRC_IPDUS     0u
#define COM_NUM_GRPSIG_NOGW     0u

#ifdef COM_F_ONEEVERYN
#define COM_NUM_OF_SIG_GRPSIG_WITH_ONE_EVERY_N 0u
#endif /* #ifdef COM_F_ONEEVERYN */


#define COM_NUM_IPDUS_IN_LAST_IPDUGRP 1u




#define COM_UPDATE_MAX      0xFF    /* max ipdu size */


#ifdef COM_RxIPduDeferredProcessing

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"

/* Buffer to hold data for deferred signal processing */
extern VAR(uint8,COM_VAR) Com_DeferredBuffer[];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"


#endif /* #ifdef COM_RxIPduDeferredProcessing */

/* START: I-PDU Buffers */

    /* Tx-Ipdu Local Buffers */


/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"

extern VAR(uint8,COM_VAR) Com_IpduBuf_BMS_14_Can_Network_CANNODE_0[];


/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"



    /* Rx-Ipdu Local Buffers */

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint8 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"

extern VAR(uint8,COM_VAR) Com_LocalBuf_TMR_3_Can_Network_CANNODE_0[];


/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint8 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_8
#include "Com_MemMap.h"


/* END: I-PDU Buffers */

#ifdef COM_TX_SIGNALGROUP


/* Tx Group Signal Buffers - Com_MainFunctionTx */



#endif /* #ifdef COM_TX_SIGNALGROUP */



/*Start: Signal Buffer -----> uint16/sint16*/

/* ------------------------------------------------------------------------ */
/* Begin section for RAM variables of uint16 type */
#define COM_START_SEC_VAR_CLEARED_POSTBUILD_16
#include "Com_MemMap.h"
extern VAR(uint16,COM_VAR) Com_SigType_u16[];

/* ------------------------------------------------------------------------ */
/* End section for RAM variables of uint16 type */
#define COM_STOP_SEC_VAR_CLEARED_POSTBUILD_16
#include "Com_MemMap.h"
/*End: Signal Buffer -----> uint16/sint16*/



#ifdef COM_RX_SIGNALGROUP


#endif /* #ifdef COM_RX_SIGNALGROUP*/



#ifdef COM_RxSigUpdateTimeout
#endif /* #ifdef COM_RxSigUpdateTimeout */

#ifdef COM_RxSigGrpUpdateTimeout
#endif /* #ifdef COM_RxSigGrpUpdateTimeout */

#endif   /* COM_PBCFG_H */
