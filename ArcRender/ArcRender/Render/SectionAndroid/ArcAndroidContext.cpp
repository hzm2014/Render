//
//  ArcAndroidContext.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/18.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcAndroidContext.hpp"
#include "ArcAFrameBufferManager.hpp"

ArcAndroidContext::ArcAndroidContext() {
    createContext();
    createFrameBufferManager();
}

ArcAndroidContext::~ArcAndroidContext() {
    delete m_frameBufferManager;
}

void ArcAndroidContext::createContext() {
    
}

void ArcAndroidContext::makeAsCurrent() const {
    
}

void ArcAndroidContext::swapBuffer(void* surface) {
    
}

void ArcAndroidContext::createFrameBufferManager() {
    m_frameBufferManager = new ArcAFrameBufferManager();
}
