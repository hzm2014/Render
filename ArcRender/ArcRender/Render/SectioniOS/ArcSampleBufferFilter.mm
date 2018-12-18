//
//  ArcSampleBufferFilter.m
//  ArcRender
//
//  Created by Zhiming He on 2018/11/27.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArcSampleBufferFilter.h"
#import "ArciOSContext.h"

ArcSampleBufferFilter::ArcSampleBufferFilter() {
    m_name = "ArcSampleBufferFilter";
    m_format = 0;
    m_rawBuffer = nullptr;
}

void ArcSampleBufferFilter::processSampleBuffer(CMSampleBufferRef sampleBuffer){
    CVImageBufferRef cameraFrame = CMSampleBufferGetImageBuffer(sampleBuffer);
    processPixelBuffer(cameraFrame);
}

void ArcSampleBufferFilter::processPixelBuffer(CVImageBufferRef cameraFrame){
    m_frameSize.width = (unsigned)CVPixelBufferGetWidth(cameraFrame);
    m_frameSize.height  = (unsigned)CVPixelBufferGetHeight(cameraFrame);
    
    m_format = CVPixelBufferGetPixelFormatType(cameraFrame);
    
    m_context -> makeAsCurrent();
    ArciOSContext* context = static_cast<ArciOSContext*>(m_context);
    
    if(m_format == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange || m_format==kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange) {
        CVOpenGLESTextureRef luminanceTextureRef = NULL;
        CVOpenGLESTextureRef chrominanceTextureRef = NULL;
        CVPixelBufferLockBaseAddress(cameraFrame, 0);
        
        CVReturn err;
        // Y-plane
        glActiveTexture(GL_TEXTURE0);
        err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, (CVOpenGLESTextureCacheRef)context -> coreVideoTextureCache(), cameraFrame, NULL, GL_TEXTURE_2D, GL_LUMINANCE, m_frameSize.width, m_frameSize.height, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0, &luminanceTextureRef);
        
        if (err)
        {
            NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
        }
        
        GLuint luminanceTexture = CVOpenGLESTextureGetName(luminanceTextureRef);
        glBindTexture(GL_TEXTURE_2D, luminanceTexture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        // UV-plane
        glActiveTexture(GL_TEXTURE1);
        err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, (CVOpenGLESTextureCacheRef)context -> coreVideoTextureCache(), cameraFrame, NULL, GL_TEXTURE_2D, GL_LUMINANCE_ALPHA, m_frameSize.width/2, m_frameSize.height/2, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 1, &chrominanceTextureRef);
        
        if (err)
        {
            NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
        }
        
        GLuint chrominanceTexture = CVOpenGLESTextureGetName(chrominanceTextureRef);
        glBindTexture(GL_TEXTURE_2D, chrominanceTexture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        ArcGLTextureOption option = ArcGLTexture::defaultOption();
        option.format = GL_LUMINANCE;
        
        FrameBufferPtr lumaBuffer = m_context -> frameBuffer(m_frameSize, option, luminanceTexture);
        
        option.format = GL_LUMINANCE_ALPHA;
        FrameBufferPtr chromBuffer = m_context -> frameBuffer(m_frameSize, option, chrominanceTexture);
        
        CVPixelBufferUnlockBaseAddress(cameraFrame, 0);
        
        setInputFrameBuffer(lumaBuffer, 0);
        setInputFrameBuffer(chromBuffer, 1);
        
        newFrame();
        
        CFRelease(luminanceTextureRef);
        CFRelease(chrominanceTextureRef);
    }
    else{
        //TODO
    }
    
    // render后的回调
    if (m_complete != nullptr) {
        m_complete(this, m_para);
    }
}

void ArcSampleBufferFilter::enableBlackFrame(bool enable) {
    m_enableBlackFrame = enable;
}

void ArcSampleBufferFilter::informTargets() {
    if(m_enableBlackFrame) {
        m_outFrameBuffer -> active();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        m_outFrameBuffer -> deactive();
    }
    ArcGLOutput::informTargets();
}

ArcSampleBufferFilter::~ArcSampleBufferFilter(){
    
    if(m_rawBuffer != nullptr){
        delete m_rawBuffer;
    }
}
