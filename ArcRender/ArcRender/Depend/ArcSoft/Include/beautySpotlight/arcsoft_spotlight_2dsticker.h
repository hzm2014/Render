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

#ifndef     _ARCSOFT_SPOTLIGHT_2DSTICKER_H_
#define     _ARCSOFT_SPOTLIGHT_2DSTICKER_H_

#include "arcsoft_spotlight_common.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    MHandle         ASLST2D_CreateEngine();
    MVoid           ASLST2D_DestroyEngine(MHandle hHandle);
    
    MRESULT         ASLST2D_Initialize(MHandle hHandle,
                                       MUInt32 nProcessImageWidth,MUInt32 nProcessImageHeight,MBool bMirror,MUInt32 nOrientation,
                                       MVoid* JNIEnv, MVoid** jcontext);
    MRESULT         ASLST2D_Uninitialize(MHandle hHandle);
    
    MRESULT         ASLST2D_SetStickerTemplate(MHandle hHandle,const MChar* szStickerTemplatePath);
    
    MRESULT         ASLST2D_Process(MHandle hHandle,
                                    LPASLFA_FACE_INFORMATION pFaceInformationIn,
                                    LPASLFA_FACE_STATUS pFaceStatusIn);
    
    // proces in gpu thread
    MRESULT         ASLST2D_RenderWithImageData(MHandle hHandle,
                                                LPASVLOFFSCREEN pOffScreenBackgroundIn,MBool bMirror, MUInt32 nOrientation,
                                                MUInt32* pTextureIDOut,
                                                LPASVLOFFSCREEN pOffScreenOut);
    // proces in gpu thread
    MRESULT         ASLST2D_RenderWithTexture(MHandle hHandle,
                                              MUInt32 nTextureIDBackgroundIn,MUInt32 nTextureWidth,MUInt32 nTextureHeight,MBool bMirror, MUInt32 nOrientation,
                                              MUInt32* pTextureIDOut,
                                              LPASVLOFFSCREEN pOffScreenOut);
    
    const ASVL_VERSION* ASLST2D_GetVersion();
    
#ifdef __cplusplus
}
#endif

#endif

