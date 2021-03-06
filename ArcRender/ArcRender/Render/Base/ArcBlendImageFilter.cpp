//
//  ArcBlendImageFilter.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/12/1.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcBlendImageFilter.hpp"
#include "ArcGLVBOCache.hpp"
#include "ArcGLContext.hpp"
#include "ArciOSGLImage.h"
#include "iostream"

static string vertexShader = ArcGLFilter::g_vertext300_shader;
static string fragmentShader = ArcGLFilter::g_fragment300_shader_RGBA;

ArcBlendImageFilter::ArcBlendImageFilter(ArcGLRect rect, ArcGLSize viewSize): ArcGLFilter(vertexShader, fragmentShader) {
    m_name = "ArcBlendImageFilter";
    setInputs(1);
    m_viewSize = viewSize;
    m_imageRect = rect;
}

void ArcBlendImageFilter::setOutputSize(ArcGLSize size) {
    ArcGLFilter::setOutputSize(size);
    
    updateImageRectViewSize(m_imageRect, m_viewSize);
}

void ArcBlendImageFilter::updateImageRectViewSize(ArcGLRect rect, ArcGLSize viewSize) {
    m_viewSize = viewSize;
    calculateAdjustViewSize();
    calculateImageRect(rect);
    updateImageVertex();
    updateVertexArray();
}

void ArcBlendImageFilter::calculateImageRect(ArcGLRect rect) {
    m_imageRect = rect;
}

void ArcBlendImageFilter::calculateAdjustViewSize() {
    if(m_outputSize.width == 0 || m_outputSize.height == 0) {
        return;
    }
    ArcGLRect frameRect = {0,0,0,0};
    float imageWidth = m_outputSize.width;
    float imageHeight = m_outputSize.height;
    float previewWidth = m_viewSize.width;
    float previewHeight = m_viewSize.height;
    
    float previewScale = previewHeight / previewWidth;
    float imageScale = imageHeight / imageWidth;
    bool state = previewScale > imageScale? true:false;
    
    // state：true表示H不变，调整W靠近，false表示W不变，调整H靠近
    if (state) {
        float tmpWidth = previewHeight * imageWidth / imageHeight;
        frameRect.size.width = tmpWidth;
        frameRect.size.height = previewHeight;
    }else {
        float tmpHeight = previewWidth * imageHeight / imageWidth;
        frameRect.size.width = previewWidth;
        frameRect.size.height = tmpHeight;
    }
    
    m_adjustedViewSize = frameRect.size;
}

void ArcBlendImageFilter::updateImageVertex() {
    
    float wScale = 0.0;
    float hScale = 0.0;
    wScale = (m_adjustedViewSize.width - m_viewSize.width)*1.00/m_adjustedViewSize.width * 0.5;
    hScale = (m_adjustedViewSize.height - m_viewSize.height)*1.00/m_adjustedViewSize.height * 0.5;
    
    float x1 = 0.0;
    float y1 = 0.0;
    float x2 = 0.0;
    float y2 = 0.0;
    float x3 = 0.0;
    float y3 = 0.0;
    float x4 = 0.0;
    float y4 = 0.0;
    
    x1 = m_imageRect.point.x*1.00/m_adjustedViewSize.width + wScale - 0.5;
    
    if (m_imageRect.point.y > m_adjustedViewSize.width/2) {
        y1 = 0.5 - (m_imageRect.point.y*1.00/m_adjustedViewSize.height + hScale);
        y3 = 0.5 - (m_imageRect.point.y*1.00/m_adjustedViewSize.height + hScale);
    } else {
        y1 = - ((m_imageRect.point.y*1.00/m_adjustedViewSize.height + hScale) - 0.5);
        y3 = - ((m_imageRect.point.y*1.00/m_adjustedViewSize.height + hScale) - 0.5);
    }
    
    x3 = (m_imageRect.point.x + m_imageRect.size.width)*1.00/m_adjustedViewSize.width + wScale - 0.5;
    
    x2 = m_imageRect.point.x*1.00/m_adjustedViewSize.width + wScale - 0.5;
    
    if ((m_imageRect.point.y + m_imageRect.size.height) > m_adjustedViewSize.width/2) {
        y2 = 0.5 - ((m_imageRect.point.y + m_imageRect.size.height)*1.00/m_adjustedViewSize.height + hScale);
        y4 = 0.5 - ((m_imageRect.point.y + m_imageRect.size.height)*1.00/m_adjustedViewSize.height + hScale);
    } else {
        y2 = - (((m_imageRect.point.y + m_imageRect.size.height)*1.00/m_adjustedViewSize.height + hScale) - 0.5);
        y4 = - (((m_imageRect.point.y + m_imageRect.size.height)*1.00/m_adjustedViewSize.height + hScale) - 0.5);
    }
    
    x4 = (m_imageRect.point.x + m_imageRect.size.width)*1.00/m_adjustedViewSize.width + wScale - 0.5;
    
    // 由于渲染是从左下角为基点，而默认的view的区域是从左上角为基点，映射后是围绕x轴的对称点
    
    int factor = 1;
    if(m_mirror) {
        factor = -1;
    }
    
    float v3 = x1*2*factor;
    float v4 = -y1*2;
    float v7 = x2*2*factor;
    float v8 = -y2*2;
    float v1 = x3*2*factor;
    float v2 = -y3*2;
    float v5 = x4*2*factor;
    float v6 = -y4*2;
    
    m_vertices[0] = v1;
    m_vertices[1] = v2;
    m_vertices[2] = v3;
    m_vertices[3] = v4;
    m_vertices[4] = v5;
    m_vertices[5] = v6;
    m_vertices[6] = v7;
    m_vertices[7] = v8;
}

void ArcBlendImageFilter::setInputFrameBuffer(FrameBufferPtr frameBuffer, unsigned location) {
    
    if (frameBuffer == nullptr) {
        std::cout<<"setInputFrameBuffer NULL!"<<std::endl;
        return;
    }
    
    if(location == 0) {
        m_outFrameBuffer = frameBuffer;
        return;
    }
    
    
    frameBuffer -> retainForUse();
    m_inputBuffers[0] = frameBuffer;
}

void ArcBlendImageFilter::activeOutFrameBuffer() {
    if(m_inputBuffers[0] == NULL) {
        return;
    }
    glActiveTexture(GL_TEXTURE1);
    m_outFrameBuffer -> active();
}

void ArcBlendImageFilter::newFrame() {
    if(m_inputBuffers[0] == NULL || m_outFrameBuffer == nullptr) {
        return;
    }
    ArcGLFilter::newFrame();
}

void ArcBlendImageFilter::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    ArcGLFilter::render();
    glDisable(GL_BLEND);
}

ArcBlendImageFilter::~ArcBlendImageFilter() {
    
}
