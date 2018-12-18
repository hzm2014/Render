//
//  ArcGLInput.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLInput.hpp"
#include "ArcGLFrameBuffer.hpp"
#include <iostream>
#include "ArcGLOutput.hpp"

ArcGLInput::ArcGLInput(unsigned inputsNum) {
    m_frameSize = {0,0};
    m_rotation = ArcGLNoRotation;
    m_firstFrameBuffer = nullptr;
    setInputs(inputsNum);
}

ArcGLInput::~ArcGLInput() {
    removeAllSources();
    m_inputBuffers.clear();
}

void ArcGLInput::setInputs(unsigned inputsNum) {
    
    m_inputsNum = inputsNum;
    m_inputBuffers.resize(inputsNum);
    for (int i = 0; i < m_inputsNum; i++) {
        m_inputBuffers[i] = nullptr;
    }
}

void ArcGLInput::setInputFrameBuffer(FrameBufferPtr frameBuffer, unsigned location) {
    
    if (frameBuffer == nullptr) {
        std::cout<<"setInputFrameBuffer NULL!"<<std::endl;
        return;
    }
    frameBuffer -> retainForUse();
    m_inputBuffers[location] = frameBuffer;
    
    if(location == 0){
        m_firstFrameBuffer = frameBuffer;
        // 存储帧大小
        if(m_frameSize.width != frameBuffer -> size().width || m_frameSize.width != frameBuffer -> size().width) {
            setFrameSize(frameBuffer -> size());
        }
    }
}

void ArcGLInput::newFrame() {
    
}

void ArcGLInput::setFrameSize(ArcGLSize size) {
    m_frameSize = size;
}

unsigned ArcGLInput::addSource(ArcGLOutput *source) {
    if(source == nullptr) {
        return static_cast<unsigned>(m_sources.size());
    }
    m_sources.push_back(source);
    return static_cast<unsigned>(m_sources.size());
}

unsigned ArcGLInput::removeSource(ArcGLOutput *source) {
    vector<ArcGLOutput*>::iterator iter = std::find(m_sources.begin(), m_sources.end(), source);
    if (iter != m_sources.end()) {
        m_sources.erase(iter);
    }
    
    return (int)m_sources.size();
}

bool ArcGLInput::removeAllSources() {
    
    for (int i=0; i < sourcesCount(); i++) {
        ArcGLOutput* source = m_sources[i];
        if (source != nullptr) {
            source -> removeTarget(this);
        }
    }
    
    m_sources.clear();
    return true;
    
}

void ArcGLInput::unlockInputFrameBuffersForUse() {
    for (int i = 0; i < m_inputsNum; i++) {
        if (m_inputBuffers[i] == nullptr) {
            std::cout<<"Visionin: input framebuffer[%d] is NULL!"<<i<<std::endl;
            continue;
        }
        m_inputBuffers[i] -> releaseForUnUse();
    }
    m_ready = false;
}

bool ArcGLInput::ready() {
    for (int i = 0; i < m_inputsNum; i++) {
        if (m_inputBuffers[i] == nullptr) {
            m_ready = false;
            return false;
        }
    }
    m_ready = true;
    return true;
}

