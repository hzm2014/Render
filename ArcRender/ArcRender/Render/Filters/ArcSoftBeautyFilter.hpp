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
#include "arcsoft_spotlight_beauty.h"
#include "arcsoft_spotlight_2dsticker.h"
#include "arcsoft_spotlight_face_alignment.h"
#include <string>

using namespace std;

class ArcSoftBeautyFilter {

public:
    
    ArcSoftBeautyFilter(const string& trackDataPath, const ArcGLSize& size, MBool mirror, MUInt32 orientation);
    void setOutputSize(const ArcGLSize& size);
    MRESULT setStickerTemplate(const string& path);
        
protected:
    
    MRESULT createASLFAEngine();
    MRESULT createASLST2DEngine();
    
    void* m_spotlightOutline = nullptr;
    void* m_spotlightBeauty = nullptr;
    void* m_spotlight2DSticker = nullptr;
    ASLFA_FACE_INFORMATION m_faceInfomation = {0};
    ASLFA_FACE_STATUS m_faceStatus = {0};
    float m_eyeScale = 0.0;
    float m_faceScale = 0.0;
    ASVLOFFSCREEN m_offScreenOut = {0};
    
    string m_outlineTrackDataFilePath;
    string m_StickerTemplatePath;
    ArcGLSize m_outputSize;
    MBool m_mirror = false;
    MUInt32 m_orientation = 0;
    
};

#endif /* ArcSoftBeautyFilter_hpp */
