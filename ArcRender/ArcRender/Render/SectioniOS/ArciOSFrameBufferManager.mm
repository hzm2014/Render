//
//  ArciOSFrameBufferManager.m
//  ArcRender
//
//  Created by Zhiming He on 2018/11/21.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArciOSFrameBufferManager.h"
#include <iostream>
#import "ArciOSFrameBuffer.h"

ArciOSFrameBufferManager::ArciOSFrameBufferManager() {
    
}

ArciOSFrameBufferManager::~ArciOSFrameBufferManager() {
    
}

FrameBufferPtr ArciOSFrameBufferManager::frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, bool onlyTexture) {
    
    string hashValue = hashForFrameBuffer(size, option);
    FrameBufferMap::iterator iter = m_frameBufferMap.find(hashValue);
    if(iter != m_frameBufferMap.end() && iter -> second -> idle()) {
        
        return iter -> second;
    }
    
    //not find, then create
    FrameBufferPtr f(new ArciOSFrameBuffer(size, option, onlyTexture));
    m_frameBufferMap.insert(FrameBufferMap::value_type (hashValue, f));
    std::cout<<"new framebuffer size:"<<size.width<<","<<size.height<<std::endl;
    std::cout<<"framebuffer count:"<<m_frameBufferMap.size()<<std::endl;
    return f;
}

FrameBufferPtr ArciOSFrameBufferManager::frameBuffer(ArcGLSize &size, ArcGLTextureOption &option, GLuint texture) {
    string hashValue = hashForFrameBuffer(size, option);
    FrameBufferMap::iterator iter = m_frameBufferMap.find(hashValue);
    FrameBufferPtr f = nullptr;
    if(iter != m_frameBufferMap.end() && iter -> second -> idle()) {
        f = iter -> second;
    } else {
        //not find, then create
        FrameBufferPtr f2(new ArciOSFrameBuffer(size, option, texture));
        f = f2;
        m_frameBufferMap.insert(FrameBufferMap::value_type (hashValue, f));
    }
    
    f -> updateTexture(texture);
    return f;
}

