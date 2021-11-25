

/*
 ***************************************************************************************************
 * Includes
 ***************************************************************************************************
 */

#include "ComM_Priv.h"

#if ((COMM_FULLCOMREQ_NOTIF != STD_OFF)&&(COMM_RTE_SUPPORT != STD_OFF))
/*
 ***************************************************************************************************
 * Defines
 ***************************************************************************************************
 */

/*
 ***************************************************************************************************
 * Variables
 ***************************************************************************************************
 */

VAR(ComM_UserHandleArrayType,COMM_VAR)  ComM_FullComRequestors[COMM_NO_OF_CHANNELS]; /*Global ram structure for FullComRequestors*/

/*
 ***************************************************************************************************
 * Functions
 ***************************************************************************************************
 */

LOCAL_INLINE FUNC(void,COMM_CODE) ComM_LUpdateUser
                        (
                         VAR(ComM_UserHandleType, COMM_VAR) User
                         );

LOCAL_INLINE FUNC(void,COMM_CODE) ComM_LDeleteUser
                        (
                         VAR(ComM_UserHandleType, COMM_VAR) User
                         );



#define COMM_START_SEC_CODE
#include "ComM_Cfg_MemMap.h"

/***************************************************************************************************
 * Function name    : ComM_LUpdateFullComRequestors
 * Syntax           : void ComM_LUpdateFullComRequestors(ComM_UserHandleType User, ComM_ModeType ComMode)
 * Description      : Update FullComRequestors for the channels corresponding to the user requesting COMM_FULL_COMMUNICATION mode.
 * Parameter        : User -> Handle of the user who requests a mode
                      ComMode -> COMM_FULL_COMMUNICATION / COMM_NO_COMMUNICATION
 * Return value     : void
 ***************************************************************************************************/
/* User Requesting Communication mode*/
FUNC(void,COMM_CODE) ComM_LUpdateFullComRequestors
                                                (
                                                VAR(ComM_UserHandleType, COMM_VAR) User,
                                                VAR(ComM_ModeType, COMM_VAR) ComMode
                                                )
{
    /* Local Variables Declaration */
    P2VAR(ComM_UserVarType, AUTOMATIC, COMM_APPL_DATA) userRamPtr_ps;
    P2CONST(ComM_UsersType,AUTOMATIC,COMM_APPL_CONST) userConfPtr_pcs;

    /* Local variable initialization */
    userRamPtr_ps = &ComM_UserStruct[User];
    userConfPtr_pcs = &(COMM_GET_USER_LIST[User]);

    if (ComMode == COMM_FULL_COMMUNICATION)
    {
        #if (COMM_INHIBITION_ENABLED)
            if((userRamPtr_ps->LimitToNoComCtr_u16 != C_ZERO) ||
                ((userRamPtr_ps->WakeUpInhibitionCtr_u16 != C_ZERO) && (userRamPtr_ps->numChannelsInFullCom_u8 != userConfPtr_pcs->NumAllChannels_u8)))
            {
                /* If inhibition is active, then FULL_COM request for the user may not be processed for channels of the user
                     * which have inhibition enabled and FullComRequestors is not updated  */
            }
            else
            {
                /*update user in FullComRequestors*/
                ComM_LUpdateUser(User);
            }
        #else
            {
                /*update user in FullComRequestors*/
                ComM_LUpdateUser(User);
            }
        #endif

    }
    else if(ComMode == COMM_NO_COMMUNICATION)
    {
        /*Delete user from FullComRequestors*/
        ComM_LDeleteUser(User);

    }
    else
    {
        /*do nothing*/
    }

}


#define COMM_STOP_SEC_CODE
#include "ComM_Cfg_MemMap.h"

/***************************************************************************************************
 * Function name    : ComM_LUpdateUser
 * Syntax           : void ComM_LUpdateUser(ComM_UserHandleType User,ComM_ModeType ComMode)
 * Description      : Updating the user handle for coresponding channels in FullComRequestors.
 * Parameter        : User -> Handle of the user who requests a mode
 * Return value     : void
 ***************************************************************************************************/

LOCAL_INLINE FUNC(void,COMM_CODE) ComM_LUpdateUser
                        (
                         VAR(ComM_UserHandleType, COMM_VAR) User
                        )
{

    /* Local Variables Declaration */
    P2VAR(ComM_UserHandleArrayType, AUTOMATIC, COMM_APPL_DATA) fullComRequestorRamPtr_ps;
    P2VAR(ComM_ChannelVarType, AUTOMATIC, COMM_APPL_DATA) channelRamPtr_ps;
    P2CONST(ComM_UsersType,AUTOMATIC,COMM_APPL_CONST) userConfigPtr_pcs;
    VAR(ComM_UserHandleArrayType, AUTOMATIC) fullComRequestor_t;
    VAR(uint8, AUTOMATIC) loopCounter_u8;
    VAR(uint8, AUTOMATIC) channelId_u8;
    VAR(uint8, AUTOMATIC) numChannels_u8;
    VAR(uint8, AUTOMATIC) Index_u8;

    /* Local variable initialization */
    userConfigPtr_pcs = &(COMM_GET_USER_LIST[User]);
    numChannels_u8 = userConfigPtr_pcs->NumDirectChannels_u8;

    /*loop through channels coresponding to user*/
    for(loopCounter_u8 = C_ZERO;loopCounter_u8 < numChannels_u8;loopCounter_u8++)
    {
       /*Get the channel id*/
       channelId_u8 = (userConfigPtr_pcs->DirectChannels_p[loopCounter_u8]);

       channelRamPtr_ps = &ComM_ChannelStruct[channelId_u8];

       fullComRequestorRamPtr_ps = &ComM_FullComRequestors[channelId_u8];

       /*Critical section start*/
       SchM_Enter_ComM_FullComRequestors();

       /*Get the local copy of ComM_FullComRequestors for the channel id*/
       fullComRequestor_t.numberOfRequesters = fullComRequestorRamPtr_ps->numberOfRequesters;

       for(Index_u8 = 0;Index_u8 < COMM_MAX_CHANNEL_REQUESTERS;Index_u8++)
       {
           fullComRequestor_t.handleArray[Index_u8] = fullComRequestorRamPtr_ps->handleArray[Index_u8];

       }

       /*add userid into handlearray*/
       fullComRequestor_t.handleArray[fullComRequestor_t.numberOfRequesters] =User;

       /*increment numberOfRequesters*/
       fullComRequestor_t.numberOfRequesters++;

       /*update local copy to global ComM_FullComRequestors*/
       fullComRequestorRamPtr_ps->numberOfRequesters = fullComRequestor_t.numberOfRequesters;

       for(Index_u8 = 0;Index_u8 < COMM_MAX_CHANNEL_REQUESTERS;Index_u8++)
       {
           fullComRequestorRamPtr_ps->handleArray[Index_u8]=fullComRequestor_t.handleArray[Index_u8];

       }

       /*Critical section end*/
       SchM_Exit_ComM_FullComRequestors();

       /*Set a Flag for the call rte_Write to be called from ComM_MainFunction for corresponding to the channel*/

       channelRamPtr_ps->ComMFullComReqNotify_b = TRUE;
   }

}

/***************************************************************************************************
 * Function name    : ComM_LDeleteUser
 * Syntax           : void ComM_LDeleteUser(ComM_UserHandleType User)
 * Description      : delting the user handle for coresponding channels in FullComRequestors.
 * Parameter        : User -> Handle of the user who requests a mode
 * Return value     : void
 ***************************************************************************************************/

LOCAL_INLINE FUNC(void,COMM_CODE) ComM_LDeleteUser
                        (
                         VAR(ComM_UserHandleType, COMM_VAR) User
                         )
{
    /*get the channel id;
        get local copy of the fullcomrequestors using channelid as index
        delete user from the handleArray by searching user in the array.
        rearrange the array values such that invalid handle(0xFF) are after valid handle values.
        Decrement numberOfRequesters
        copy back to fullcomrequestors*/

        /* Local Variables Declaration */
        P2VAR(ComM_UserHandleArrayType, AUTOMATIC, COMM_APPL_DATA) fullComReqRamPtr_ps;
        P2VAR(ComM_ChannelVarType, AUTOMATIC, COMM_APPL_DATA) channelRamPtr_ps;
        P2CONST(ComM_UsersType,AUTOMATIC,COMM_APPL_CONST) userCfgPtr_pcs;
        VAR(ComM_UserHandleArrayType, AUTOMATIC) fullComRequestors_t;
        VAR(uint8, AUTOMATIC) loopCount_u8;
        VAR(uint8, AUTOMATIC) channelId_u8;
        VAR(uint8, AUTOMATIC) numofChannels_u8;
        VAR(uint8, AUTOMATIC) arrayIndex_u8;
        VAR(uint8, AUTOMATIC) searchIndex_u8;
        VAR(uint8, AUTOMATIC) nextElementIndex_u8;

        /* Local variable initialization */
        userCfgPtr_pcs = &(COMM_GET_USER_LIST[User]);
        numofChannels_u8 = userCfgPtr_pcs->NumDirectChannels_u8;

        /*loop through channels coresponding to user*/
        for(loopCount_u8 = C_ZERO;loopCount_u8 < numofChannels_u8;loopCount_u8++)
        {
           /*Get the channel id*/
           channelId_u8 = (userCfgPtr_pcs->DirectChannels_p[loopCount_u8]);

           fullComReqRamPtr_ps = &ComM_FullComRequestors[channelId_u8];

           channelRamPtr_ps = &ComM_ChannelStruct[channelId_u8];

           /*Critical section start*/
           SchM_Enter_ComM_FullComRequestors();

           /*Get the local copy of ComM_FullComRequestors for the channel id*/
           fullComRequestors_t.numberOfRequesters = fullComReqRamPtr_ps->numberOfRequesters;

           for(arrayIndex_u8 = 0;arrayIndex_u8 < COMM_MAX_CHANNEL_REQUESTERS;arrayIndex_u8++)
           {
               fullComRequestors_t.handleArray[arrayIndex_u8] = fullComReqRamPtr_ps->handleArray[arrayIndex_u8];

           }

           /*Search user in the handlearray and invalidate the user*/
           searchIndex_u8 =0;

           while ((searchIndex_u8 < COMM_MAX_CHANNEL_REQUESTERS) && (User != fullComRequestors_t.handleArray[searchIndex_u8]))
           {

               searchIndex_u8++;
           }
           if(searchIndex_u8 < COMM_MAX_CHANNEL_REQUESTERS)
           {
               /* MR12 RULE 14.2 VIOLATION: The variable incremented in the third expression of this 'for' statement is actually the variable identified as the 'loop control variable.
                      * Hence it is an incorrect warning reported by Misra */
               /*on sucessfull search, the searched user value is replaced with the next element in the array by shifting the elements of the array*/
               for(nextElementIndex_u8=searchIndex_u8+1;nextElementIndex_u8<COMM_MAX_CHANNEL_REQUESTERS;searchIndex_u8++)
               {
                   fullComRequestors_t.handleArray[searchIndex_u8]=fullComRequestors_t.handleArray[nextElementIndex_u8];
                   nextElementIndex_u8++;

               }
               /*Due to shift of elements of the array last element of the array is filled with invalid userid*/
               fullComRequestors_t.handleArray[nextElementIndex_u8-1]=COMM_NOT_USED_USER_ID;

               /*Decrement numberOfRequesters*/
               fullComRequestors_t.numberOfRequesters--;

               /*update local copy to global ComM_FullComRequestors*/
               fullComReqRamPtr_ps->numberOfRequesters = fullComRequestors_t.numberOfRequesters;

               for(arrayIndex_u8 = 0;arrayIndex_u8 < COMM_MAX_CHANNEL_REQUESTERS;arrayIndex_u8++)
               {
                   fullComReqRamPtr_ps->handleArray[arrayIndex_u8] = fullComRequestors_t.handleArray[arrayIndex_u8];
               }
                 /*Set a Flag for the call rte_Write to be called from ComM_MainFunction for corresponding to the channel*/

                channelRamPtr_ps->ComMFullComReqNotify_b = TRUE;

           }
           else
           {
               /*user not found in the handlearray*/

           }
           /*Critical section end*/
           SchM_Exit_ComM_FullComRequestors();

        }

}
#endif




