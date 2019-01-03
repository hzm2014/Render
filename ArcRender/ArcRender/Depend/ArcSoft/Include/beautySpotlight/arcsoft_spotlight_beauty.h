/*******************************************************************************
 Copyright(c) ArcSoft, All right reserved.
 
 This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary
 and confidential information.
 
 The information and code contained in this file is only for authorized ArcSoft
 employees to design, create, modify, or review.
 
 DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER
 AUTHORIZATION.
 
 If you are not an intended recipient of this file, you must not copy,
 distribute, modify, or take any action in reliance on it.
 
 If you have received this file in error, please immediately notify ArcSoft and
 permanently delete the original and any copy of any file and any printout
 thereof.
 *******************************************************************************/

#ifndef     _ARCSOFT_SPOTLIGHT_BEAUTY_H_
#define     _ARCSOFT_SPOTLIGHT_BEAUTY_H_

#include "arcsoft_spotlight_common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    MHandle         ASLFB_CreateEngine();
    MVoid           ASLFB_DestroyEngine(MHandle hHandle);
    
    MRESULT         ASLFB_Initialize(MHandle hHandle,MVoid* JNIEnv, MVoid** jcontext);
    MRESULT         ASLFB_Uninitialize(MHandle hHandle);
    
    MVoid           ASLFB_SetFeatureLevel(MHandle hHandle, ASLFB_Feature eFeature, MLong lLevel); // 0-100
    
    MRESULT         ASLFB_Process(MHandle hHandle,
                                  LPASVLOFFSCREEN pOffScreenIn,
                                  LPASVLOFFSCREEN pOffScreenOut
                                  );
    
    const ASVL_VERSION* ASLFB_GetVersion();
    
#ifdef __cplusplus
}
#endif

#endif //_ARCSOFT_SPOTLIGHT_BEAUTY_H_

