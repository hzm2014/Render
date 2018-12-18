//
//  ArcGLFrameBuffer.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLFrameBuffer_hpp
#define ArcGLFrameBuffer_hpp

#include <stdio.h>
#include "ArcGLTexture.hpp"
#include <memory>

using namespace std;
class ArcGLFrameBuffer;

using FrameBufferPtr = shared_ptr<ArcGLFrameBuffer>;

class ArcGLFrameBuffer {
    
public:
    
    ArcGLFrameBuffer(ArcGLSize size, ArcGLTextureOption option, bool onlyTexture = false);
    ArcGLFrameBuffer(ArcGLSize size, ArcGLTextureOption option, GLuint texture);
    virtual ~ArcGLFrameBuffer();
    
    void active(int layer = 0);
    void active(GLuint textureUnit);
    void activeTexture();
    void deactive();
    
    virtual void updateTexture(GLuint texture);
        
    void setPixels(void* data);
    virtual uint8_t* readPixels(uint8_t* data) = 0;
    
#ifdef __ANDROID__
    
#else
    virtual void* pixelBuffer() = 0;
#endif
    
    inline ArcGLSize size() { return m_size; };
    inline ArcGLTextureOption option() { return m_option; };
    inline ArcGLTexture* texture() { return m_texture; }
    
    void retainForUse();
    void releaseForUnUse();
    
    inline bool idle() { return m_useCount == 0; } //是否处于空闲模式
    
protected:

    ArcGLSize m_size;
    ArcGLTextureOption m_option;
    
    GLuint m_frameBuffer;
    ArcGLTexture* m_texture;
    int m_layer = 0; //3D纹理当前的层
    uint8_t* m_pixelData;
    
    unsigned m_useCount = 0;
    
    virtual void createFrameBuffer() = 0;
    void createTexture(GLuint texture);
    void createTexture();
    
};

#endif /* ArcGLFrameBuffer_hpp */
