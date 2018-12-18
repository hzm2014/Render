//
//  ArcGLFrameBufferManager.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLFrameBufferManager_hpp
#define ArcGLFrameBufferManager_hpp

#include <stdio.h>
#include "ArcGLFrameBuffer.hpp"
#include <map>

class ArcGLFrameBufferManager {

public:
    
    ArcGLFrameBufferManager();
    virtual ~ArcGLFrameBufferManager();
    
    virtual FrameBufferPtr frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, bool onlyTexture = false) = 0;
    virtual FrameBufferPtr frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, GLuint texture) = 0;
    
protected:
    
    using FrameBufferMap = map<string, FrameBufferPtr>;
    
    ArcGLFrameBuffer* m_frameBuffer;
    FrameBufferMap m_frameBufferMap;
    string hashForFrameBuffer(const ArcGLSize& size, const ArcGLTextureOption& option) const;
    
};

#endif /* ArcGLFrameBufferManager_hpp */
