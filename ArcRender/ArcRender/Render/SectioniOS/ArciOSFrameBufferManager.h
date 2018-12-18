//
//  ArciOSFrameBufferManager.h
//  ArcRender
//
//  Created by Zhiming He on 2018/11/21.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ArcGLFrameBufferManager.hpp"

class ArciOSFrameBufferManager: public ArcGLFrameBufferManager {
    
public:
    
    ArciOSFrameBufferManager();
    ~ArciOSFrameBufferManager();
    
    virtual FrameBufferPtr frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, bool onlyTexture = false);
    virtual FrameBufferPtr frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, GLuint texture);
    
};
