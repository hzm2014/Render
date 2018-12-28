//
//  ArciOSFrameBuffer.h
//  ArcRender
//
//  Created by Zhiming He on 2018/11/16.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ArcGLFrameBuffer.hpp"
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

class ArciOSFrameBuffer: public ArcGLFrameBuffer {
    
public:
    
    ArciOSFrameBuffer(ArcGLSize& size, ArcGLTextureOption& option, bool onlyTexture = false);
    ArciOSFrameBuffer(ArcGLSize& size, ArcGLTextureOption& option, GLuint texture);
    ~ArciOSFrameBuffer();
    
    virtual uint8_t* readPixels(uint8_t* data);
    virtual void* pixelBuffer();
    
protected:
    
    CVPixelBufferRef m_renderTarget = nullptr;
    CVOpenGLESTextureRef m_renderTexture = nullptr;
    
    virtual void createFrameBuffer();
    
};

NS_ASSUME_NONNULL_END
