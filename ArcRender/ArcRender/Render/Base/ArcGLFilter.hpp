//
//  ArcGLFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/26.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLFilter_hpp
#define ArcGLFilter_hpp

#include <stdio.h>
#include "ArcGLInput.hpp"
#include "ArcGLOutput.hpp"
#include <string>
#include "ArcGLVertexArray.hpp"

class ArcGLVertexBuffer;
class ArcGLContext;

class ArcGLFilter: public ArcGLInput, public ArcGLOutput {
    
public:
    
    ArcGLFilter(const string& vertexShader, const string& fragmentShader);
    virtual ~ArcGLFilter();
    
    //定义全局变量
    static const char* g_vertext300_shader;
    static const char* g_fragment300_shader_RGBA;
    static const char* g_fragment300_shader_BGRA;
    static const GLfloat g_vertices[];
    
    inline string name() const { return m_name; }
    
    //设置填充并计算裁剪
    virtual void setFillMode(ArcGLFillMode fillMode);
    inline ArcGLFillMode fillMode(){ return m_fillMode; }
    //设置输出重新计算裁剪
    virtual void setOutputSize(ArcGLSize size);
    //更新帧尺寸并重新计算裁剪
    virtual void setFrameSize(ArcGLSize size);
            
    // 设置输出framebuffer，如果设置了，则不再从cache中获取buffer
    virtual void setOutputFrameBuffer(FrameBufferPtr buffer){ m_outFrameBuffer = buffer; }
    
    void setTextureOption(ArcGLTextureOption& option);
    inline ArcGLTextureOption textureOption() { return m_option; }
    
    virtual ArcGLSize sizeOfFBO();
    
    void setVertices(float* v);
    
    inline float* vertices(){
        return &m_vertices[0];
    }
    
    inline float* getCoordinates(){
        return &m_texCoor[0];
    }
    
    virtual void setMirror(bool mirror = false);
    
    virtual void setInputs(int inputs);
    virtual void newFrame();
    virtual void render();
    
    virtual void activeOutFrameBuffer();
    
    void setClearColor(float r, float g, float b);
    
    void setFloat(const string& name, GLfloat val);
    void setFloat(const string& name, GLfloat *val, int num);
    void setFloatv(const string& name, GLfloat* val, int num);
    void setMatrix(const string& name, GLfloat *val, int num);
    void setInteger(const string& name, int i);
    
protected:
    
    string m_vertexShader;
    string m_fragmentShader;
    
    ArcGLContext* m_context;
    
    GLuint      m_position = 0;
    GLuint      m_inputTexCoor = 0;
    
    string m_name;
    
    // 顶点，涉及裁剪
    std::vector<GLfloat> m_vertices;      //顶点坐标
    std::vector<GLfloat> m_texCoor;   //纹理贴图坐标
    
    bool                 m_mirror = false; //是否镜像
    
    ArcGLVertexBuffer*    m_texCoorBuffer;    // 纹理坐标缓存
    ArcGLVertexBuffer*    m_vertexBuffer;        // 顶点缓存
    
    ArcGLFillMode m_fillMode;
    
    std::vector<GLint> m_uniforms;
    
    ArcGLTextureOption m_option = ArcGLTexture::defaultOption();
    float               m_clearColor[4];
    
    //VAO
    ArcGLVertexArray* m_vertexArray;
    
    void configContext(string& vertexShader, string& fragmentShader);
    
    void initParams();
    
    void createVertexArray();
        
    // 计算裁剪比例，更新顶点
    virtual void calculateAspectRatio();
    
    static const GLfloat* coordinatesRotation(ArcGLRotation mode);
    
    virtual void createVertexBuffers();
    
    virtual void updateVertexArray();
};

#endif /* ArcGLFilter_hpp */
