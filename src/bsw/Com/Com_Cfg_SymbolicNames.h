


 
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
        #define ComConf_ComIPdu_BMS_Tem_4_CANNODE_0 0
    /* Rx IPdus */
        #define ComConf_ComIPdu_TMR_3_CANNODE_0 0

/* ------------------------------------------------------------------------ */
/* End section */

/* ------------------------------------------------------------------------ */
/* Begin section for Signal symbolic names */



/* Signal IDs*/
/* Tx Signal ID*/
    #define ComConf_ComSignal_BMS_BattTempSensorOrder32 0
    #define ComConf_ComSignal_BMS_BattTempSensorOrder31 1
    #define ComConf_ComSignal_BMS_BattTempSensorOrder30 2
    #define ComConf_ComSignal_BMS_BattTempSensorOrder29 3
    #define ComConf_ComSignal_BMS_BattTempSensorOrder28 4
    #define ComConf_ComSignal_BMS_BattTempSensorOrder27 5
    #define ComConf_ComSignal_BMS_BattTempSensorOrder26 6
    #define ComConf_ComSignal_BMS_BattTempSensorOrder25 7

/* Rx Signal ID*/
    #define ComConf_ComSignal_TMR_VoltageHV 0
    #define ComConf_ComSignal_TMR_CurrentHV 1





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
