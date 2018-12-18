//
//  ArcGLTexture.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLTexture.hpp"
#include <iostream>

ArcGLTexture::ArcGLTexture(ArcGLTextureOption option, ArcGLSize size, int depth): m_textureOption(option), m_size(size), m_depth(depth) {
    createTexture();
}

ArcGLTexture::ArcGLTexture(ArcGLTextureOption option, ArcGLSize size, GLuint texture): m_textureOption(option), m_size(size), m_texture(texture) {
    
}

void ArcGLTexture::createTexture() {
    
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glActiveTexture(GL_TEXTURE2);
    
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    if (m_depth <= 1) {
        
        glBindTexture(GL_TEXTURE_2D, textureId);
        
        glTexImage2D(GL_TEXTURE_2D, 0, m_textureOption.color_format, m_size.width, m_size.height, 0, m_textureOption.format, m_textureOption.type, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_textureOption.min_filter);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_textureOption.mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_textureOption.wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_textureOption.wrap_t);
        // 默认过滤方式是GL_LINEAR_MIPMAP_LINEAR，需要生成贴图所有必要的mips
        if (m_textureOption.min_filter>=GL_NEAREST_MIPMAP_NEAREST || m_textureOption.mag_filter>=GL_NEAREST_MIPMAP_NEAREST) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else{
        if (m_textureOption.format == GL_INVALID_ENUM) {
            std::cout<<"Depth texture can't be 3D_texture!"<<std::endl;
            return;
        }
        glTexImage3D(GL_TEXTURE_3D, 0, m_textureOption.color_format, m_size.width, m_size.height, m_depth, 0, m_textureOption.format, m_textureOption.type, NULL);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, m_textureOption.wrap_s);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, m_textureOption.wrap_t);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, m_textureOption.wrap_t);
        // 默认过滤方式是GL_LINEAR_MIPMAP_LINEAR，需要生成贴图所有必要的mips
        if (m_textureOption.min_filter>=GL_NEAREST_MIPMAP_NEAREST || m_textureOption.mag_filter>=GL_NEAREST_MIPMAP_NEAREST) {
            glGenerateMipmap(GL_TEXTURE_3D);
        }
        glBindTexture(GL_TEXTURE_3D, 0);
    }
    
    m_texture = textureId;
}

void ArcGLTexture::active(GLuint textureUnit) {
    
    glActiveTexture(textureUnit);
    
    if (m_textureOption.texture_dim == GL_TEXTURE_2D) {
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }
    else if(m_textureOption.texture_dim==GL_TEXTURE_3D){
        glBindTexture(GL_TEXTURE_3D, m_texture);
    }
}

void ArcGLTexture::deactive() {
    
    if (m_textureOption.texture_dim == GL_TEXTURE_2D) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else if(m_textureOption.texture_dim==GL_TEXTURE_3D){
        glBindTexture(GL_TEXTURE_3D, 0);
    }
}

void ArcGLTexture::updateTexture(GLuint texture) {
    m_texture = texture;
}

void ArcGLTexture::setPixels(void *data) {
    
    active(GL_TEXTURE0);
    
    if(m_textureOption.texture_dim == GL_TEXTURE_2D) {
        glTexImage2D(GL_TEXTURE_2D, 0, m_textureOption.color_format, m_size.width, m_size.height, 0, m_textureOption.format, m_textureOption.type, data);//装载贴图
    } else if(m_textureOption.texture_dim == GL_TEXTURE_3D) {
        glTexImage3D(GL_TEXTURE_3D, 0, m_textureOption.color_format, m_size.width, m_size.height, m_depth, 0, m_textureOption.format, m_textureOption.type, data);//装载贴图
    }
    
    // 默认过滤方式是GL_LINEAR_MIPMAP_LINEAR
    if ( m_textureOption.min_filter >= GL_NEAREST_MIPMAP_NEAREST ||  m_textureOption.mag_filter >= GL_NEAREST_MIPMAP_NEAREST) {
        glGenerateMipmap( GL_TEXTURE_2D);
    }
}

ArcGLTextureOption ArcGLTexture::defaultOption() {
    static ArcGLTextureOption option = {
        ArcGLTextureTypeRGBA,   // 纹理类别
        GL_TEXTURE_2D,      // 纹理维度
        GL_LINEAR,          // min_filter
        GL_LINEAR,          // max_filter, Mipmap只作用于min_filter, 设置mag_filter的Mipmap选项会导致无效操作，错误码为GL_INVALID_ENUM
        GL_CLAMP_TO_EDGE,   // wrap_s
        GL_CLAMP_TO_EDGE,   // wrap_t
        GL_CLAMP_TO_EDGE,   // wrap_r
        GL_RGBA,            // color_internal_format
        GL_RGBA,            // format
        GL_UNSIGNED_BYTE,   // type
        GL_DEPTH_COMPONENT16,// depth_internal_format
        GL_COLOR_ATTACHMENT0// attachment type
    };
    return option;
}

ArcGLTexture::~ArcGLTexture() {
    glDeleteTextures(1, &m_texture);
}
