//
//  ArciOSFrameBuffer.m
//  ArcRender
//
//  Created by Zhiming He on 2018/11/16.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArciOSFrameBuffer.h"
#import "ArciOSContext.h"

ArciOSFrameBuffer::ArciOSFrameBuffer(ArcGLSize& size, ArcGLTextureOption& option, bool onlyTexture): ArcGLFrameBuffer(size, option, onlyTexture) {
    if(!onlyTexture) {
        createFrameBuffer();
    }
}

ArciOSFrameBuffer::ArciOSFrameBuffer(ArcGLSize& size, ArcGLTextureOption& option, GLuint texture): ArcGLFrameBuffer(size, option, texture){
    
}

void ArciOSFrameBuffer::createFrameBuffer() {
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
   
    ArciOSContext* context = static_cast<ArciOSContext*>(ArcGLContext::getInstance());
    CVOpenGLESTextureCacheRef coreVideoTextureCache =  (CVOpenGLESTextureCacheRef)context -> coreVideoTextureCache();
    
    CFDictionaryRef empty; // empty value for attr value.
    CFMutableDictionaryRef attrs;
    empty = CFDictionaryCreate(kCFAllocatorDefault, NULL, NULL, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks); 
    attrs = CFDictionaryCreateMutable(kCFAllocatorDefault, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    CFDictionarySetValue(attrs, kCVPixelBufferIOSurfacePropertiesKey, empty);
    
    CVReturn err = CVPixelBufferCreate(kCFAllocatorDefault, m_size.width, m_size.height, kCVPixelFormatType_32BGRA, attrs, &m_renderTarget);
    if (err) {
        
    }
    
    err = CVOpenGLESTextureCacheCreateTextureFromImage (kCFAllocatorDefault, coreVideoTextureCache, m_renderTarget,
                                                        NULL, // texture attributes
                                                        GL_TEXTURE_2D,
                                                        m_option.color_format, // opengl format
                                                        m_size.width,
                                                        m_size.height,
                                                        m_option.format, // native iOS format
                                                        m_option.type,
                                                        0,
                                                        &m_renderTexture);
    if (err) {
        
    }
    
    CFRelease(attrs);
    CFRelease(empty);
    
    GLuint renderTexture = CVOpenGLESTextureGetName(m_renderTexture);
    glBindTexture(CVOpenGLESTextureGetTarget(m_renderTexture), renderTexture);
    m_texture = new ArcGLTexture(m_option, m_size, renderTexture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_option.wrap_s);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_option.wrap_t);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture -> texture(), 0);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE:
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            
            break;

        default:
            
            break;
    }
    
    deactive();
}

uint8_t* ArciOSFrameBuffer::readPixels(uint8_t* data) {
    
    CVPixelBufferLockBaseAddress(m_renderTarget, 0);
    uint8_t * bufferBytes = (uint8_t*)CVPixelBufferGetBaseAddress(m_renderTarget);
    CVPixelBufferUnlockBaseAddress(m_renderTarget, 0);
    return bufferBytes;
}

void* ArciOSFrameBuffer::pixelBuffer() {
    return m_renderTarget;
}

ArciOSFrameBuffer::~ArciOSFrameBuffer() {
        
    if(m_renderTarget) {
        CFRelease(m_renderTarget);
    }
    if(m_renderTexture) {
        CFRelease(m_renderTexture);
    }
    
    glDeleteFramebuffers(1, &m_frameBuffer);
    m_frameBuffer = 0;
    
    delete m_texture;
    m_texture = nullptr;
}
