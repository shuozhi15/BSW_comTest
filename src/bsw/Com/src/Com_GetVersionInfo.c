

/**************************************************************************************************/
/* Preprocessor includes                                                                          */
/**************************************************************************************************/
#include "Com_Prv.h"
#include "Com_Prv_Inl.h"
/**************************************************************************************************/
/* Global functions (declared in header files )                                                   */
/**************************************************************************************************/

/***************************************************************************************************
 Function name    : Com_GetVersionInfo
 Syntax           : void Com_GetVersionInfo(Std_VersionInfoType *versioninfo)
 Description      : Service for passing the Information through a pointer "*VersionInfo"
 Parameter        : versioninfo --> pointer to an address where all the version information needs to be copied.
 Return value     : None
***************************************************************************************************/

#if (COM_VERSION_INFO_API == STD_ON)  /* Check whether the version Info is enabled */
#define COM_START_SEC_CODE
#include "Com_MemMap.h"
FUNC(void, COM_CODE) Com_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, COM_APPL_DATA) versioninfo)
{

# if(COM_CONFIGURATION_USE_DET == STD_ON)
    /* Report DET : Development Error for Invalid  Pointer */
    if(Com_Prv_DETCheck_Ptr_Only((NULL_PTR == versioninfo), COMServiceId_GetVersionInfo))
#endif /* (COM_CONFIGURATION_USE_DET == STD_ON) */
    {
        versioninfo->vendorID = COM_VENDOR_ID;
        versioninfo->moduleID = COM_MODULE_ID;
        versioninfo->sw_major_version = COM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = COM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = COM_SW_PATCH_VERSION;
    }
}
#define COM_STOP_SEC_CODE
#include "Com_MemMap.h"
#endif

