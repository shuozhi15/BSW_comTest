


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/
#ifndef COM_PBCFG_INTERNALID_H
#define COM_PBCFG_INTERNALID_H

/*
 * VERY IMP : This file contains internal symbolic names , which are defined as Internal IDs
 *            This file should be generated from Com_Generate_PB action.
 *            This file is generated for both pre-compile & post-build variant.
 *            For pre-compile , Internal ID and External ID would be the same.
 *            No other code should be generated in this file
 *            This file should be only included in COM_PBcfg.c file.
 * COM internal structures and tables (defined in COM_PBcfg.c file) must use internal symolic names, defined in this file
 *
 */

/* ------------------------------------------------------------------------ */
/* Begin section for Tx IPdu internal symbolic names */

#define     ComConf_ComIPdu_Internal_BMS_Vol_5_CANNODE_0               0


/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Rx IPdu internal symbolic names */

#define     ComConf_ComIPdu_Internal_OBC_4_CANNODE_0               0
#define     ComConf_ComIPdu_Internal_TMR_3_CANNODE_0               1


/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Tx signal internal symbolic names */

#define     ComSignal_Internal_BMS_BattCellOrder20Vol               0
#define     ComSignal_Internal_BMS_BattCellOrder19Vol               1
#define     ComSignal_Internal_BMS_BattCellOrder18Vol               2
#define     ComSignal_Internal_BMS_BattCellOrder17Vol               3


/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Rx signal internal symbolic names */

#define     ComSignal_Internal_OBC_ChargeSourceMode               0
#define     ComSignal_Internal_OBC_FaultCode               1
#define     ComSignal_Internal_OBC_MaxOutputVoltage               2
#define     ComSignal_Internal_OBC_MaxOutputCurrent               3
#define     ComSignal_Internal_TMR_VoltageHV               4
#define     ComSignal_Internal_TMR_CurrentHV               5


/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Tx signal group internal symbolic names */



/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Rx signal group internal symbolic names */



/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Tx group signal internal symbolic names */



/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Rx group signal internal symbolic names */



/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for mapping IPdu group symbolic names to internal IDs */

/* Currently IPdu group symbolic names are not used in any COM tables.
  Hence it is currently not required to generate Ipdu group internal symbolic names */

/* ------------------------------------------------------------------------ */
/* End section */


#endif /* #ifndef COM_PBCFG_INTERNALID_H */
