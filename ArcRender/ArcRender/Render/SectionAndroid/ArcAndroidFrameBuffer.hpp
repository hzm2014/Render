//
//  ArcAndroidFrameBuffer.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/16.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcAndroidFrameBuffer_hpp
#define ArcAndroidFrameBuffer_hpp

#include <stdio.h>
#include "ArcGLFrameBuffer.hpp"
#include "ArcGLTextureManager.hpp"

class ArcAndroidFrameBuffer: public ArcGLFrameBuffer {
    
public:
    
    ArcAndroidFrameBuffer(ArcGLSize& size, ArcGLTextureOption& option);
    
    virtual uint8_t* readPixels(uint8_t* data);
    
protected:
        
    virtual void createFrameBuffer();
    void createTexture();
};

#endif /* ArcAndroidFrameBuffer_hpp */
