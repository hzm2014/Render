//
//  ArcAFrameBufferManager.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/21.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcAFrameBufferManager_hpp
#define ArcAFrameBufferManager_hpp

#include <stdio.h>
#include "ArcGLFrameBufferManager.hpp"

class ArcAFrameBufferManager: public ArcGLFrameBufferManager {
    
public:
    
    ArcAFrameBufferManager();
    ~ArcAFrameBufferManager();
    
    virtual FrameBufferPtr frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, bool onlyTexture = false);
    virtual FrameBufferPtr frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, GLuint texture);
    
};


#endif /* ArcAFrameBufferManager_hpp */
