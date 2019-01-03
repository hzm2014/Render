//
//  ArcSoftBeautyFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2019/1/3.
//  Copyright © 2019 zhiming he. All rights reserved.
//

#include "ArcSoftBeautyFilter.hpp"

ArcSoftBeautyFilter::ArcSoftBeautyFilter(const string& trackDataPath, const ArcGLSize& size, MBool mirror, MUInt32 orientation) {
    m_outlineTrackDataFilePath = trackDataPath;
    m_outputSize = size;
    m_mirror = mirror;
    m_orientation = orientation;
}

MRESULT ArcSoftBeautyFilter::createASLFAEngine() {
    m_spotlightOutline = ASLFA_CreateEngine();
    MRESULT ret = ASLFA_Initialize(m_spotlightOutline, m_outlineTrackDataFilePath.c_str(), ASLFA_MAX_FACE_NUM, nullptr, nullptr);
    return ret;
}

MRESULT ArcSoftBeautyFilter::createASLST2DEngine() {
    m_spotlight2DSticker = ASLST2D_CreateEngine();
    MRESULT ret = ASLST2D_Initialize(m_spotlight2DSticker,m_outputSize.width,m_outputSize.height,m_mirror,m_orientation,NULL,NULL);
    return ret;
}

void ArcSoftBeautyFilter::setOutputSize(const ArcGLSize &size) {
    m_outputSize = size;
}

MRESULT ArcSoftBeautyFilter::setStickerTemplate(const string &path) {
    m_StickerTemplatePath = path;
    
    MRESULT ret = ASLST2D_SetStickerTemplate(m_spotlight2DSticker, m_StickerTemplatePath.c_str());
    return ret;
}
