//
//  ArcAndroidFrameBuffer.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/16.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcAndroidFrameBuffer.hpp"
#include <stdlib.h>
#include <iostream>

ArcAndroidFrameBuffer::ArcAndroidFrameBuffer(ArcGLSize& size, ArcGLTextureOption& option): ArcGLFrameBuffer(size, option) {
    createTexture();
    createFrameBuffer();
}

void ArcAndroidFrameBuffer::createTexture() {
    m_texture = new ArcGLTexture(m_option, m_size);
}

void ArcAndroidFrameBuffer::createFrameBuffer() {
    GLint defaultFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    
    glGenFramebuffers(1, &m_frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    
//    glGenRenderbuffers(1, &m_renderBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, m_width, m_height);
    glRenderbufferStorage(GL_RENDERBUFFER, m_option.depth_format, m_size.width, m_size.height);
    
    // 设置附着点类型，一般颜色渲染使用GL_COLOR_ATTACHMENT0
    if (m_option.texture_dim==GL_TEXTURE_2D) {
        glBindTexture(GL_TEXTURE_2D, m_texture -> texture());
        glFramebufferTexture2D(GL_FRAMEBUFFER, m_option.attachment, GL_TEXTURE_2D, m_texture -> texture(), 0);
    }
    else if(m_option.texture_dim==GL_TEXTURE_3D){
        glBindTexture(GL_TEXTURE_3D, m_texture -> texture());
        glFramebufferTextureLayer(GL_FRAMEBUFFER, m_option.attachment, m_texture -> texture(), 0, 0);
    }
    
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE:
            m_layer = 0;
            break;
            
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout<<"fbo unsupported"<<std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cout<<"fbo GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"<<std::endl;
            break;
        default:
            std::cout<<"Framebuffer Error"<<std::endl;
            break;
    }
}

uint8_t* ArcAndroidFrameBuffer::readPixels(uint8_t* data) {
    // glReadPixels从显存中读取，必须先绑定到fbo
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
//    glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
    glActiveTexture(GL_TEXTURE0);
    
    if(m_option.texture_dim == GL_TEXTURE_3D) {
        // 3D纹理时候
        //TODO
    }
    
    glBindTexture(GL_TEXTURE_2D, m_texture -> texture());
    if (data == NULL) {
        if (m_pixelData == NULL) {
            if (m_option.type == GL_FLOAT || m_option.type==GL_UNSIGNED_INT || m_option.type == GL_INT){
                m_pixelData = (uint8_t*)malloc(m_size.width * m_size.height * 4 * 4);
            }
            else{
                m_pixelData = (uint8_t*)malloc(m_size.width * m_size.height * 4);
            }
        }
        data = m_pixelData;
    }
    glReadPixels(0, 0, m_size.width, m_size.height, m_option.format, m_option.type, data);
    
    glBindTexture(m_option.texture_dim, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return data;
}
