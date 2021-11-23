


 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: Com / AR42.6.0.0                Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 
 </VersionHead>*/

#ifndef COM_CFG_SYMBOLICNAMES_H
#define COM_CFG_SYMBOLICNAMES_H

/* Note: Module variant generation is done here, specifically to make below macros available on the inclusion of 
 * Com_MemMap.h by other modules without Com.h inclusion */

/* definition of supported module variant  */

#define COM_VARIANT_PRE_COMPILE                   0

#define COM_VARIANT_LINK_TIME                     1

#define COM_VARIANT_POSTBUILD_LOADABLE            2

#define COM_VARIANT_POSTBUILD_SELECTABLE          3

/* Macro to hold configured module variant */
#define COM_CONFIGURATION_VARIANT                 COM_VARIANT_PRE_COMPILE


/* if COM_DontUseExternalSymbolicNames is defined while including this file, then the below symbolic names will not
   be visible in the including file */
#ifndef     COM_DontUseExternalSymbolicNames

/* ------------------------------------------------------------------------ */
/* Begin section for IPdu symbolic names */

/* Tx IPdus */
        #define ComConf_ComIPdu_BMS_20_Can_Network_CANNODE_0 0
        #define ComConf_ComIPdu_BMS_Vol_8_Can_Network_CANNODE_0 1
    /* Rx IPdus */
        #define ComConf_ComIPdu_OBC_1_Can_Network_CANNODE_0 0
        #define ComConf_ComIPdu_TMR_3_Can_Network_CANNODE_0 1

/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Signal symbolic names */



/* Signal IDs*/
/* Tx Signal ID*/
    #define ComConf_ComSignal_BMSH_19_Reserved1 0
    #define ComConf_ComSignal_BMSH_CellSensor_TotalNum 1
    #define ComConf_ComSignal_BMSH_SingleFeedbackEnergy 2
    #define ComConf_ComSignal_BMSH_SingleDisChargeEnergy 3
    #define ComConf_ComSignal_BMSH_SingleChargeEnergy 4
    #define ComConf_ComSignal_BMS_BattCellOrder32Vol 5
    #define ComConf_ComSignal_BMS_BattCellOrder31Vol 6
    #define ComConf_ComSignal_BMS_BattCellOrder30Vol 7
    #define ComConf_ComSignal_BMS_BattCellOrder29Vol 8

/* Rx Signal ID*/
    #define ComConf_ComSignal_OBC_FaultLevelSts 0
    #define ComConf_ComSignal_OBC_ChgSts 1
    #define ComConf_ComSignal_OBC_OutputPowerLimt 2
    #define ComConf_ComSignal_OBC_ChgVolt 3
    #define ComConf_ComSignal_OBC_ChgCurr 4
    #define ComConf_ComSignal_TMR_VoltageHV 5
    #define ComConf_ComSignal_TMR_CurrentHV 6





/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Signal group symbolic names */



/* Tx SignalGroup ID*/

/* Rx SignalGroup ID*/







/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Group signal symbolic names */



/* Tx GroupSignal ID*/
#ifdef COM_TX_SIGNALGROUP

#endif
#ifdef COM_RX_SIGNALGROUP
/* Rx GroupSignal ID*/

#endif



/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for IPdu group symbolic names */


/* IPduGroup ID*/
    #define ComConf_ComIPduGroup_ComIPduGroup_Rx 0
    #define ComConf_ComIPduGroup_ComIPduGroup_Tx 1
/* ------------------------------------------------------------------------ */
/* End section */

#endif /* #ifndef     COM_DontUseExternalSymbolicNames */


#endif /* COM_CFG_SYMBOLICNAMES_H */
