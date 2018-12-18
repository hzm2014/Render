//
//  ArcAFrameBufferManager.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/21.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcAFrameBufferManager.hpp"
#include <iostream>
#include "ArcAndroidFrameBuffer.hpp"

ArcAFrameBufferManager::ArcAFrameBufferManager() {
    
}

ArcAFrameBufferManager::~ArcAFrameBufferManager() {
    
}

FrameBufferPtr ArcAFrameBufferManager::frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, bool onlyTexture) {
    string hashValue = hashForFrameBuffer(size, option);
    FrameBufferMap::iterator iter = m_frameBufferMap.find(hashValue);
    if(iter == m_frameBufferMap.end()) {
        //no find, then create
        FrameBufferPtr f(new ArcAndroidFrameBuffer(size, option));
        m_frameBufferMap.insert(FrameBufferMap::value_type (hashValue, f));
        return f;
    }
    
    return iter -> second;
}

FrameBufferPtr ArcAFrameBufferManager::frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, GLuint texture) {
    return nullptr;
}
