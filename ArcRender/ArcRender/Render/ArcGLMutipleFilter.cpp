//
//  ArcGLMutipleFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/27.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLMutipleFilter.hpp"

ArcGLMutipleFilter::ArcGLMutipleFilter(string& vertexShader, string& fragmentShader): ArcGLFilter(vertexShader, fragmentShader) {
    m_name = "ArcGLMutipleFilter";
    m_input = nullptr;
    m_output = nullptr;
}

ArcGLMutipleFilter::~ArcGLMutipleFilter() {
    
}

void ArcGLMutipleFilter::setInputFrameBuffer(FrameBufferPtr frameBuffer, unsigned location) {
    // ArcGLMutipleFilter的inputs为1，用m_input_buffers[0]存储第一个framebuffer，用于bypass时的传递
    if (location == 0 && frameBuffer!= nullptr) {
        m_inputBuffers[location] = frameBuffer;
        if (m_frameSize.width != frameBuffer -> size().width || m_frameSize.height != frameBuffer -> size().height) {
            setFrameSize(frameBuffer -> size());
        }
    }
    
    if (m_enable && m_input != nullptr) {
        m_input->setInputFrameBuffer(frameBuffer, location);
    }
}

void ArcGLMutipleFilter::setOutputRotation(ArcGLRotation rotation) {
    ArcGLInput::setOutputRotation(rotation);
    if (m_input != nullptr) {
        m_input->setOutputRotation(rotation);
    }
}

void ArcGLMutipleFilter::newFrame() {
    
    if (m_enable && m_input != nullptr) {
        
        // 在设置时候有可能input还没有生成
        m_input->newFrame();
        
        if (m_output == nullptr || m_output -> m_outFrameBuffer == nullptr) {
            assert(false);
            return;
        }
        
        m_outFrameBuffer = m_output -> m_outFrameBuffer;
        
    } else {
        m_outFrameBuffer = m_inputBuffers[0];
    }

    informTargets();
}




