//
//  ArcSoftBeautyFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2019/1/3.
//  Copyright © 2019 zhiming he. All rights reserved.
//

#ifndef ArcSoftBeautyFilter_hpp
#define ArcSoftBeautyFilter_hpp

#include <stdio.h>
#include "ArcGLHeader.h"
#include "ArcFaceDeformationFilter.hpp"
#include "arcsoft_spotlight_beauty.h"
#include "arcsoft_spotlight_2dsticker.h"
#include "arcsoft_spotlight_face_alignment.h"
#include <string>

using namespace std;

class ArcSoftBeautyFilter: public ArcFaceDeformationFilter {

public:
    
    ArcSoftBeautyFilter(const string& trackDataPath, const ArcGLSize& size, MBool mirror, MUInt32 orientation);
    MRESULT setStickerTemplate(const string& path);
    void setSkinSoft(MLong value);
    void setSkinBright(MLong value);
    void updateFaceDeformation(const PMPOINT points);
    void processNV12Data(const ArcGLSize& size, uint8_t* baseAddress0, uint8_t* baseAddress1);
    void processRGBASLST2D(GLuint inTexture, GLuint* outTexture);
        
protected:
    
    MRESULT createASLFAEngine();
    MRESULT createASLST2DEngine();
    MRESULT createASLFBEngine();
    
    void* m_spotlightOutline = nullptr;
    void* m_spotlightBeauty = nullptr;
    void* m_spotlight2DSticker = nullptr;
    ASLFA_FACE_INFORMATION m_faceInfomation = {0};
    ASLFA_FACE_STATUS m_faceStatus = {0};
    float m_eyeScale = 0.0;
    float m_faceScale = 0.0;
    ASVLOFFSCREEN m_offScreenOut = {0};
    
    string m_outlineTrackDataFilePath;
    string m_stickerTemplatePath;
    ArcGLSize m_outputSize;
    MBool m_mirror = false;
    MUInt32 m_orientation = 0;
    
};

#endif /* ArcSoftBeautyFilter_hpp */
