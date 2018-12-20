//
//  ArcGLPixelBuffer.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/18.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLPixelBuffer.hpp"

ArcGLPixelBuffer::ArcGLPixelBuffer(long size, bool sync, int elementSize) {
    m_sync = sync;
    m_size = size;
    m_first = true;
    pthread_mutex_init(&m_lock, NULL);
    
    m_pack_mode = GL_PIXEL_PACK_BUFFER;
    glGenBuffers(2, m_buffer);
    m_mapIndex= 1;
    /*
     Stream-缓冲区的对象需要时常更新,但使用次数很少.
     Static-只需要一次指定缓冲区对象中的数据,但使用次数很多.
     Dynamic-数据不仅需要时常更新,使用次数也很多.
     */
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_buffer[0]);
    glBufferData(GL_PIXEL_PACK_BUFFER, size * 4 * elementSize, 0, GL_STATIC_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_buffer[1]);
    glBufferData(GL_PIXEL_PACK_BUFFER, size * 4 * elementSize, 0, GL_STATIC_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

ArcGLPixelBuffer::~ArcGLPixelBuffer(){
    glUnmapBuffer(m_pack_mode);
    glDeleteBuffers(2, m_buffer);
    pthread_mutex_destroy(&m_lock);
}

void* ArcGLPixelBuffer::read(ArcGLFrameBuffer* framebuffer){
    m_frameBuffer = framebuffer;
    
    m_frameBuffer -> active();
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_buffer[1-m_mapIndex]);
    glReadPixels(0, 0, m_frameBuffer -> size().width, m_frameBuffer -> size().height, m_frameBuffer -> option().format, m_frameBuffer -> option().type, 0);
    
    if (m_sync) {
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_buffer[1 - m_mapIndex]);
    }
    else{
        // 异步状态下的第一帧直接返回，在下一帧再映射当前帧的内存，这样交替进行提高效率
        if (m_first) {
            m_frameBuffer -> deactive();
            m_mapIndex = 1 - m_mapIndex;
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
            m_first = false;
            return NULL;
        }
        
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_buffer[m_mapIndex]);
    }
    
    int elem_size = 1;
    if (m_frameBuffer -> option().type == GL_UNSIGNED_INT || m_frameBuffer -> option().type == GL_INT || m_frameBuffer -> option().type == GL_FLOAT) {
        elem_size = 4;
    }
    else if(m_frameBuffer -> option().type==GL_SHORT || m_frameBuffer -> option().type==GL_UNSIGNED_SHORT){
        elem_size = 2;
    }
    long totalSize = m_frameBuffer -> size().width * m_frameBuffer -> size().width * 4 * elem_size;
    void* p = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, totalSize, GL_MAP_READ_BIT);
    m_frameBuffer -> deactive();
    m_mapIndex = 1 - m_mapIndex;
    unActive();
    
    return p;
}

void ArcGLPixelBuffer::unActive(){
    if (m_frameBuffer != NULL) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }
}
/*
 void ArcGLPixelBuffer::upload(GLubyte* bytes, uint32_t format){
 unpack();
 glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, format, GL_UNSIGNED_BYTE, 0);
 switch(format){
 case GL_LUMINANCE:
 glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width*m_height, bytes, GL_STREAM_DRAW);
 break;
 case GL_LUMINANCE_ALPHA:
 glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width*m_height*2, bytes, GL_STREAM_DRAW);
 break;
 default:
 glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width*m_height*4, bytes, GL_STREAM_DRAW);
 }
 }
 
 void ArcGLPixelBuffer::unpack(){
 GPUContext::shareInstance()->makeCurrent();
 glBindTexture(GL_TEXTURE_2D, m_texture);
 glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_buffer);
 }
 
 GLubyte* ArcGLPixelBuffer::download(){
 return m_ptr;
 }
 
 GLubyte* ArcGLPixelBuffer::download(GLuint texture, uint32_t width, uint32_t height){
 m_texture = texture;
 m_width = width;
 m_height = height;
 m_pack_mode = GPU_PACK;
 pack();
 return m_ptr;
 }
 
 void ArcGLPixelBuffer::pack(){
 GPUContext::shareInstance()->makeCurrent();
 glReadBuffer(GL_FRONT);
 glBindBuffer(GL_PIXEL_PACK_BUFFER, m_buffer);
 glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
 // m_ptr = (GLubyte*)glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, m_width*m_height*4, GL_READ_WRITE);
 }
 */
