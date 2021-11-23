

 
/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * $Generator__: CanNm / AR42.6.0.0             Module Package Version
 * $Editor_____: ISOLAR-A/B 7.0.1_7.0.1               Tool Version
 * $Model______: 2.3.0.4                ECU Parameter Definition Version
 *
 </VersionHead>*/




#ifndef CANNM_PBCFG_H
#define CANNM_PBCFG_H


 
/*
 **********************************************************************************************************************
 * Defines/Macros
 **********************************************************************************************************************
*/

#if (CANNM_CONFIGURATION_VARIANT != CANNM_VARIANT_PRE_COMPILE)


/* Number of PN's relevent: Useful for EIRA and ERA calculation */
#define CANNM_NUMBER_OF_PNS    0U



#else
/* Number of PN's relevent: Useful for EIRA and ERA calculation */
#define CANNM_NUMBER_OF_PNS                                                0U
#endif

/*
 *******************************************************************************
 * Extern declarations
 *******************************************************************************
 */  
 #if (CANNM_CONFIGURATION_VARIANT == CANNM_VARIANT_POSTBUILD_SELECTABLE)

extern CONST(CanNm_GlobalConfigType, CANNM_CONST) CanNm_GlobalConfigData_cs;

extern CONST(CanNm_ChannelConfigType, CANNM_CONST) CanNm_ChannelConfigData_cs[];

extern CONST(CanNm_ConfigType, CANNM_CONST) CanNmGlobalConfig;


#endif


#endif /* CANNM_PBCFG_H */

