//
//  ArciOSContext.m
//  ArcRender
//
//  Created by zhiming he on 2018/11/18.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArciOSContext.h"
#import <iostream>
#import "ArciOSFrameBufferManager.h"
#import <OpenGLES/EAGL.h>
#import <AVFoundation/AVFoundation.h>
#import "ArcRunProcess.h"

typedef struct GL_Context{
    EAGLContext* m_context;
    EAGLSharegroup* m_sharegroup;
    CVOpenGLESTextureCacheRef m_textureCache;
}GL_Context;

ArciOSContext::ArciOSContext() {
    createRunProcess();
    createContext();
    createCoreVideoTextureCache();
    createFrameBufferManager();
}

ArciOSContext::~ArciOSContext() {
    CVOpenGLESTextureCacheFlush(m_glContext -> m_textureCache, 0);
    delete m_glContext;
    [EAGLContext setCurrentContext:nil];
    delete m_frameBufferManager;
}

void ArciOSContext::createRunProcess() {
    static ArcRunProcess* runProcess = [[ArcRunProcess alloc] init];
    [runProcess createProcessQueueWithLabel:@"com.arcrender.context"];
    m_runProcess = (__bridge void*)runProcess;
}

void* ArciOSContext::runProcess() const {
    return m_runProcess;
}

void ArciOSContext::createContext() {
    m_glContext = new GL_Context;
    m_glContext -> m_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3 sharegroup:m_glContext -> m_sharegroup];
    
    assert(m_glContext -> m_context);
    
    [EAGLContext setCurrentContext:m_glContext -> m_context];
    glDisable(GL_DEPTH_TEST);
}

void* ArciOSContext::context() const {
    return m_glContext;
}

void* ArciOSContext::coreVideoTextureCache() const {
    return m_glContext -> m_textureCache;
}

void ArciOSContext::makeAsCurrent() const {
    
    if([EAGLContext currentContext] != m_glContext -> m_context) {
        [EAGLContext setCurrentContext: m_glContext -> m_context];
    }
}

void ArciOSContext::createCoreVideoTextureCache() {
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, m_glContext -> m_context, NULL, &m_glContext -> m_textureCache);
    if(err) {
        assert(false);
    }
}

void ArciOSContext::swapBuffer(void*) {
    [m_glContext -> m_context presentRenderbuffer:GL_RENDERBUFFER];
}

void ArciOSContext::createFrameBufferManager() {
    m_frameBufferManager = new ArciOSFrameBufferManager();
}


