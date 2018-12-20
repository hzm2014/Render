//
//  ArcGLTexture.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLTexture_hpp
#define ArcGLTexture_hpp

#include <stdio.h>
#include "ArcGLHeader.h"

typedef struct ArcGLTextureOption{
    ArcGLTextureType texture_type;    // 纹理类别
    GLenum texture_dim;     // 纹理维度，GL_TEXTURE_2D或者GL_TEXTURE_3D
    GLenum min_filter;      // 缩放差值方法
    GLenum mag_filter;      // 缩放差值方法
    GLenum wrap_s;          // s方向边缘填充方式
    GLenum wrap_t;          // t方向边缘填充方式
    GLenum wrap_r;          // r方向边缘填充方式
    GLenum color_format;    // 纹理内存存储格式，未确定大小纹理格式GL_RGBA GL_ALPHA，确定大小GL_R8 RGBA8UI
    GLenum format;          // 纹理格式，如GL_RED GL_RGBA GL_ALPHA
    GLenum type;            // 输入像素数据类型，如GL_UNSIGNED_BYTE
    GLenum depth_format;    // 深度纹理位数
    GLenum attachment;      // 缓冲区附着类型
}ArcGLTextureOption;


class ArcGLTexture {
    
public:
    
    ArcGLTexture(ArcGLTextureOption option, ArcGLSize size, int depth = 0);
    ArcGLTexture(ArcGLTextureOption option, ArcGLSize size, GLuint texture);
    ArcGLTexture() = delete;
    
    inline GLuint texture() const {
        return m_texture;
    }
    
    static ArcGLTextureOption defaultOption();
    
    inline const ArcGLTextureOption& textureOption() {
        return m_textureOption;
    }
    
    inline const ArcGLSize& size() { return m_size; };
    
    inline const int depth() { return m_depth; };
    
    void active(GLuint textureUnit);
    void deactive();
    
    void updateTexture(GLuint texture);
    
    void setPixels(void* data);
        
    ~ArcGLTexture();
    
protected:
    
    ArcGLTextureOption m_textureOption;
    GLuint m_texture;
    ArcGLSize m_size; //纹理宽高
    int m_depth = 0;
    
    void createTexture();
};

#endif /* ArcGLTexture_hpp */
