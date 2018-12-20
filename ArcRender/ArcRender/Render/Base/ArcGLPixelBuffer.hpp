//
//  ArcGLPixelBuffer.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/18.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLPixelBuffer_hpp
#define ArcGLPixelBuffer_hpp

#include <stdio.h>
#include "ArcGLFrameBuffer.hpp"
#include <mutex>

class ArcGLPixelBuffer {
   
public:
    
    /**
     * @size:   pbo大小，width*height, size*4为实际rgba容量
     * @sync:   为同步或者异步模式
     */
    ArcGLPixelBuffer(long size, bool sync = true, int elementSize = 1);
    ~ArcGLPixelBuffer();
    
    void* read(ArcGLFrameBuffer* frameBuffer);
    void unActive();
    
    inline void lock(){
        pthread_mutex_lock(&m_lock);
    }
    inline void unlock(){
        pthread_mutex_unlock(&m_lock);
    }
    
protected:
    
    ArcGLFrameBuffer* m_frameBuffer;
    GLuint            m_buffer[2];
    GLuint            m_mapIndex;
    GLubyte*          m_ptr;
    
    pthread_mutex_t m_lock;
    GLuint          m_pack_mode;
    uint64_t        m_size;
    bool            m_sync;     // 是否同步
    bool            m_first;    // 是否第一帧
    
};

#endif /* ArcGLPixel_hpp */
