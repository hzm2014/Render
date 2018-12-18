//
//  ArcGLVertexBuffer.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/26.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLVertexBuffer_hpp
#define ArcGLVertexBuffer_hpp

#include <stdio.h>
#include "ArcGLHeader.h"

class ArcGLVertexBuffer {
    
public:
    ArcGLVertexBuffer(unsigned vertexCount = 4, unsigned dimension = 2, bool cache = true);
    ~ArcGLVertexBuffer();
    
    inline GLuint vertexBuffer() {
        return m_vertexbuffer;
    }
    inline unsigned dimension() {
        return m_dimension;
    }
    inline unsigned count() {
        return m_count;
    }
    inline uint32_t dataSize() {
        return m_dataSize;
    }
    inline bool cache() {
        return m_cache;
    }
    
    void activeBuffer(GLuint attribute, const GLvoid *data, int offset = 0);
    void activeBuffer(GLuint attribute);
    void disableBuffer(GLuint attribute);
    void setBuffer(const GLvoid *data, int offset=0);
protected:
    uint32_t        m_count;
    uint32_t        m_dimension;
    uint32_t        m_dataSize;
    GLuint          m_vertexbuffer;
    
    bool            m_cache;    // 是否使用顶点缓存
};


#endif /* ArcGLVertexBuffer_hpp */
