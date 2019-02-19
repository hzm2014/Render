//
//  ArcGLImage.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/12/1.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLImage.hpp"
#include "ArcGLContext.hpp"
#include "ArcGLInput.hpp"

ArcGLImage::ArcGLImage(void* cgImageRef) {
    m_cgImagePtr = cgImageRef;
}

ArcGLImage::~ArcGLImage() {
    
}

void ArcGLImage::createOutputFrameBuffer(ArcGLSize &size, ArcGLTextureOption &option) {
    
}

void ArcGLImage::setPixelToOutBuffer(uint8_t* data) {
    if(data == NULL) {
        return;
    }
    m_outFrameBuffer -> setPixels(data);
}

void ArcGLImage::informTargets() {
    for (int i = 0; i < targetsCount(); i++) {
        ArcGLInput* input = m_targets[i];
        input -> setInputFrameBuffer(m_outFrameBuffer, 1);
        input -> newFrame();
    }
}
