//
//  ArcSoftBeautyFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2019/1/3.
//  Copyright © 2019 zhiming he. All rights reserved.
//

#include "ArcSoftBeautyFilter.hpp"
#include <iostream>

ArcSoftBeautyFilter::ArcSoftBeautyFilter(const string& trackDataPath, const ArcGLSize& size, MBool mirror, MUInt32 orientation): ArcFaceDeformationFilter(size) {
    m_name = "ArcSoftBeautyFilter";
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

MRESULT ArcSoftBeautyFilter::setStickerTemplate(const string &path) {
    m_stickerTemplatePath = path;
    
    MRESULT ret = ASLST2D_SetStickerTemplate(m_spotlight2DSticker, m_stickerTemplatePath.c_str());
    return ret;
}

MRESULT ArcSoftBeautyFilter::createASLFBEngine() {
    m_spotlightBeauty = ASLFB_CreateEngine();
    MRESULT ret = ASLFB_Initialize(m_spotlightBeauty, nullptr, nullptr);
    ASLFB_SetFeatureLevel(m_spotlightBeauty, ASLFB_Feature_SkinSoften, 65);
    ASLFB_SetFeatureLevel(m_spotlightBeauty, ASLFB_Feature_SkinBright, 25);
    return ret;
}

void ArcSoftBeautyFilter::setSkinSoft(MLong value) {
    ASLFB_SetFeatureLevel(m_spotlightBeauty, ASLFB_Feature_SkinSoften, value);
}

void ArcSoftBeautyFilter::setSkinBright(MLong value) {
    ASLFB_SetFeatureLevel(m_spotlightBeauty, ASLFB_Feature_SkinBright, value);
}

float getY(int y, int h) {
    return y*1.0f/h;
}

float getX(int x, int w) {
    return x*1.0f/w;
}

void ArcSoftBeautyFilter::updateFaceDeformation(const PMPOINT points) {
    if(points == nullptr) {
        return;
    }
    
    float left[26] = {0};
    float right[26] = {0};
    float face[22] = {0};
    int facelength = 22;
    
    left[0] = getX(points[96].x, m_outputSize.width);
    left[1] = getY(points[96].y, m_outputSize.height);
    right[0] = getX(points[95].x, m_outputSize.width);
    right[1] = getY(points[95].y, m_outputSize.height);
    
    for(int i=1; i<=12; i++) {
        left[i*2] = getX(points[50+i].x, m_outputSize.width);
        left[i*2+1] = getY(points[50+i].y, m_outputSize.height);
        right[i*2] = getX(points[38+i].x, m_outputSize.width);
        right[i*2+1] = getY(points[38+i].y, m_outputSize.height);
    }
    
    for(int i=0; i<(facelength/2); i++) {
        face[i*2] = getX(points[4+i].x, m_outputSize.width);
        face[i*2+1] = getY(points[4+i].y, m_outputSize.height);
    }
    
    setFaceContour(left, right, face, facelength);
}

void ArcSoftBeautyFilter::processNV12Data(const ArcGLSize &size, uint8_t *baseAddress0, uint8_t *baseAddress1) {
    ASVLOFFSCREEN data = {0};
    data.u32PixelArrayFormat = ASVL_PAF_NV12;
    data.i32Height = size.height;
    data.i32Width = size.width;
    data.pi32Pitch[0] = size.width;
    data.pi32Pitch[1] = size.width;
    data.ppu8Plane[0] = baseAddress0;
    data.ppu8Plane[1] = baseAddress1;
    
    MRESULT ret = ASLFA_Process(m_spotlightOutline, &data, ASLFA_FOP_0_HIGHER_EXT, &m_faceInfomation);
    if(ret != MOK) {
        std::cout<<"ASLFA_Process failed"<<std::endl;
        return;
    }
    
    ASLFA_GetFaceStatus(m_spotlightOutline, &m_faceStatus);
    ASLST2D_Process(m_spotlight2DSticker, &m_faceInfomation, &m_faceStatus);
    ASLFB_Process(m_spotlightBeauty, &data, nullptr);
    
    if(m_faceInfomation.nFaceCount == 0) {
        setEyeScale(0);
        setFaceScale(0);
        return;
    }
    
    int focusIndex = 0;
    float distance = 0.0;
    for (int index = 0; index < m_faceInfomation.nFaceCount; index++)
    {
        float tmpdistance = m_faceInfomation.rcFaceRect[index].right - m_faceInfomation.rcFaceRect[index].left;
        
        if (distance < tmpdistance) {
            distance = tmpdistance;
            focusIndex = index;
        }
    }
    
    updateFaceDeformation(m_faceInfomation.ptFaceAlignmentPoint + focusIndex*101);
}

void ArcSoftBeautyFilter::processRGBASLST2D(GLuint inTexture, GLuint* outTexture) {
    m_offScreenOut.i32Width = m_outputSize.width;
    m_offScreenOut.i32Height = m_outputSize.height;
    m_offScreenOut.u32PixelArrayFormat = ASVL_PAF_RGB32_B8G8R8A8;
    m_offScreenOut.pi32Pitch[0] = m_outputSize.width * 4;
    
    ASLST2D_RenderWithTexture(m_spotlight2DSticker, inTexture, m_outputSize.width, m_outputSize.height, m_mirror, m_orientation, outTexture, nullptr);
}
