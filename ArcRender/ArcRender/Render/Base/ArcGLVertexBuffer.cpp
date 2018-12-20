//
//  ArcGLVertexBuffer.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/26.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLVertexBuffer.hpp"

ArcGLVertexBuffer::ArcGLVertexBuffer(unsigned vertexCount, unsigned dimension, bool cache) {
    
    m_count = vertexCount;
    m_dimension = dimension;
    m_cache = cache;
    m_dataSize = m_count * sizeof(GLfloat) * m_dimension;
    m_vertexbuffer = -1;
    
    if (m_cache) {
        glGenBuffers(1, &m_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, m_dataSize, NULL, GL_STREAM_DRAW);
    }
}

void ArcGLVertexBuffer::activeBuffer(GLuint attribute, const GLvoid *data, int offset) {
    if (m_cache) {
        glEnableVertexAttribArray(attribute);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
        glBufferSubData(GL_ARRAY_BUFFER, offset, m_dataSize, data);
        glVertexAttribPointer(attribute, m_dimension, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*m_dimension, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    else{
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, m_dimension , GL_FLOAT, GL_FALSE, 0, data);
    }
}

void ArcGLVertexBuffer::activeBuffer(GLuint attribute) {
    if (m_cache) {
        glEnableVertexAttribArray(attribute);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
        glVertexAttribPointer(attribute, m_dimension, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*m_dimension, 0);
    }
}

void ArcGLVertexBuffer::setBuffer(const GLvoid *data, int offset) {
    if (m_cache) {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
        glBufferSubData(GL_ARRAY_BUFFER, offset, m_dataSize, data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void ArcGLVertexBuffer::disableBuffer(GLuint attribute){
    if (m_cache){
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableVertexAttribArray(attribute);
    }
    else{
        glDisableVertexAttribArray(attribute);
    }
}

ArcGLVertexBuffer::~ArcGLVertexBuffer(){
    if (m_cache){
        glDeleteBuffers(1, &m_vertexbuffer);
    }
}
