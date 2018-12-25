//
//  ArcGLFrameBuffer.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLFrameBuffer.hpp"

ArcGLFrameBuffer::ArcGLFrameBuffer(ArcGLSize size, ArcGLTextureOption option, bool onlyTexture): m_size(size), m_option(option) {
    if(onlyTexture) {
        createTexture();
    }
}

ArcGLFrameBuffer::ArcGLFrameBuffer(ArcGLSize size, ArcGLTextureOption option, GLuint texture): m_size(size), m_option(option) {
    createTexture(texture);
}

void ArcGLFrameBuffer::createTexture() {
    m_texture = new ArcGLTexture(m_option, m_size);
}

void ArcGLFrameBuffer::createTexture(GLuint texture) {
    m_texture = new ArcGLTexture(m_option, m_size, texture);
}

void ArcGLFrameBuffer::active() {

    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glViewport(0, 0, m_size.width, m_size.height);
}

void ArcGLFrameBuffer::activeTexture() {
    glBindTexture(GL_TEXTURE_2D, m_texture -> texture());
}

void ArcGLFrameBuffer::active(GLuint textureUnit) {
    glActiveTexture(textureUnit);
    activeTexture();
}

void ArcGLFrameBuffer::deactive() {
    
    glBindTexture(m_option.texture_dim, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ArcGLFrameBuffer::updateTexture(GLuint texture) {
    m_texture -> updateTexture(texture);
}

void ArcGLFrameBuffer::setPixels(void *data) {
    
    glActiveTexture(GL_TEXTURE0);
    activeTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, m_option.color_format, m_size.width, m_size.height, 0, m_option.format, m_option.type, data);//装载贴图
    /** 3.0中才支持
     else if(m_option.texture_type == GL_TEXTURE_3D){
     glTexImage3D(GL_TEXTURE_3D, 0, m_option.color_format, m_width, m_height, m_depth, 0, m_option.format, m_option.type, data);//装载贴图
     }
     */
    // 默认过滤方式是GL_LINEAR_MIPMAP_LINEAR，需要生成贴图所有必要的mips
    if ( m_option.min_filter>=GL_NEAREST_MIPMAP_NEAREST ||  m_option.mag_filter>=GL_NEAREST_MIPMAP_NEAREST) {
        glGenerateMipmap( GL_TEXTURE_2D);
    }
}

void ArcGLFrameBuffer::retainForUse() {
    m_useCount++;
}

void ArcGLFrameBuffer::releaseForUnUse() {
    if(m_useCount <= 0) {
        return;
    }
    
    m_useCount--;
}

ArcGLFrameBuffer::~ArcGLFrameBuffer() {
    glDeleteFramebuffers(1, &m_frameBuffer);
    delete m_texture;
}
