

/*<VersionHead>
 * This Configuration File is generated using versions (automatically filled in) as listed below.
 *
 * Generator__: ComM / AR42.5.4.0                Module Package Version
 * Editor_____: ISOLAR-A/B 7.0.1_7.0.1                Tool Version
 * Model______: 2.3.0.4             ECU Parameter Definition Version
 *
 </VersionHead>*/



/* ---------------------------------------------------------------------*/
/* Include protection                                                   */
/* ---------------------------------------------------------------------*/
#ifndef  COMM_MAIN_H
#define  COMM_MAIN_H

#include "ComM_Cfg.h"

/* ---------------------------------------------------------------------*/
/* External declarations                                                */
/* ---------------------------------------------------------------------*/
#define COMM_START_SEC_CODE
#include "ComM_Cfg_MemMap.h"

/* ---------------------------------------------------------------------*/
/*  Name : ComM_MainFunction_ComMChannel_Can_Cluster_Channel                                          */
/*  Description : Main function for Bus Type COMM_BUS_TYPE_CAN  channel ComMChannel_Can_Cluster_Channel           */
/* ---------------------------------------------------------------------*/

#if ( COMM_ECUC_RB_RTE_IN_USE != STD_ON )
extern FUNC(void,COMM_CODE) ComM_MainFunction_ComMChannel_Can_Cluster_Channel(void);
#endif

#define COMM_STOP_SEC_CODE
#include "ComM_Cfg_MemMap.h"




#endif
