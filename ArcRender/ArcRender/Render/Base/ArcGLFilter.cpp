//
//  ArcGLFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/26.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLFilter.hpp"
#include "ArcGLContext.hpp"
#include "ArcGLVBOCache.hpp"

#ifdef __ANDROID__
#include "ArcAndroidContext.hpp"
#else
#include "ArciOSContext.h"
#endif


ArcGLFilter::ArcGLFilter(const string& vertexShader, const string& fragmentShader) {
    m_vertexShader = vertexShader;
    m_fragmentShader = fragmentShader;
    m_name = "ArcGLFilter";
    configContext(m_vertexShader, m_fragmentShader);
    initParams();
    createVertexArray();
    createVertexBuffers();
}

ArcGLFilter::~ArcGLFilter() {
    
    m_outFrameBuffer = nullptr;
    delete m_vertexBuffer;
    m_vertexBuffer = nullptr;
    delete m_texCoorBuffer;
    m_texCoorBuffer = nullptr;
    
    delete m_vertexArray;
    m_vertexArray = nullptr;
    
    ArcGLVBOCache::destroyInstance();
}

void ArcGLFilter::configContext(string &vertexShader, string &fragmentShader) {
    m_context = ArcGLContext::getInstance();
    m_context -> makeAsCurrent();
    m_context -> useProgram(vertexShader, fragmentShader);
}

void ArcGLFilter::initParams() {
    
    m_texCoorBuffer = nullptr;
    m_vertexBuffer = nullptr;
    m_clearColor[0] = 1.0f;
    m_clearColor[1] = 1.0f;
    m_clearColor[2] = 1.0f;
    m_clearColor[3] = 1.0f;
    m_rotation = ArcGLNoRotation;
    m_texCoor.resize(8);
    m_texCoor[0] = -1;
    // 初始化顶点数据
    m_vertices.resize(8);
    memcpy(&m_vertices[0], g_vertices, sizeof(GLfloat) * 8);
    
    m_fillMode = ArcGLFillModePreserveAspectRatioAndFill;
    
    m_position = m_context -> attribute("position");
    m_inputTexCoor = m_context -> attribute("inputTextureCoordinate");
}

/**
 @param inputs 纹理单元数量
 */
void ArcGLFilter::setInputs(int inputs){
    ArcGLInput::setInputs(inputs);
    m_uniforms.resize(inputs);
    
    for (int i = 0; i < inputs; ++i) {
        m_uniforms[i] = i;
    }
    
    m_context -> setUniformIndex("inputImageTexture", m_inputsNum, &m_uniforms[0]);
    for (int i = 0; i < inputs; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glUniform1i(m_uniforms[i], i);
    }
}

void ArcGLFilter::createVertexArray() {
    m_vertexArray = new ArcGLVertexArray();
}

void ArcGLFilter::setFillMode(ArcGLFillMode fillMode) {
    m_fillMode = fillMode;
    calculateAspectRatio();
}

void ArcGLFilter::calculateAspectRatio() {
    
    if (m_outputSize.width == 0 || m_outputSize.height == 0 || m_frameSize.width == 0 || m_frameSize.height == 0) {
        return;
    }
    
    switch(m_fillMode){
            // 用黑框填充
        case ArcGLFillModePreserveAspectRatio:
        {
            float frameRatio = m_frameSize.width * 1.0 / m_frameSize.height;
            float outRatio = m_outputSize.width * 1.0 / m_outputSize.height;
            if (frameRatio > outRatio) {
                // 按照宽度压缩，上下填黑
                float w_ratio = m_outputSize.width * 1.0 / m_frameSize.width;
                float normal_h = w_ratio * m_frameSize.height;
                m_vertices[0] = -1.0;
                m_vertices[1] = (-1.0) * normal_h / m_outputSize.height;
                m_vertices[2] = 1.0;
                m_vertices[3] = m_vertices[1];
                m_vertices[4] = -1.0;
                m_vertices[5] = normal_h / m_outputSize.height;
                m_vertices[6] = 1.0;
                m_vertices[7] = m_vertices[5];
                
            } else {
                // 按照高度压缩，左右填黑
                float h_ratio = m_outputSize.height * 1.0 / m_frameSize.height;
                float normal_w = h_ratio * m_frameSize.width;
                m_vertices[0] = (-1.0) * normal_w / m_outputSize.width;
                m_vertices[1] = -1.0;
                m_vertices[2] = normal_w / m_outputSize.width;
                m_vertices[3] = -1.0;
                m_vertices[4] = m_vertices[0];
                m_vertices[5] = 1.0;
                m_vertices[6] = m_vertices[2];
                m_vertices[7] = 1.0;
            }
        }
            break;
            // 按照比例裁剪
        case ArcGLFillModePreserveAspectRatioAndFill:
        {
            float wh_ratio = m_outputSize.width * 1.0 / m_outputSize.height;
            float hw_ratio = m_outputSize.height * 1.0 / m_outputSize.width;
            if (wh_ratio > m_frameSize.width * 1.0 / m_frameSize.height) {
                float ratioy = m_frameSize.width * hw_ratio;
                m_vertices[0] = -1.0;
                m_vertices[1] = (-1.0) * m_frameSize.height / ratioy;
                m_vertices[2] = 1.0;
                m_vertices[3] = m_vertices[1];
                m_vertices[4] = -1.0;
                m_vertices[5] = m_frameSize.height / ratioy;
                m_vertices[6] = 1.0;
                m_vertices[7] = m_vertices[5];
            }
            else{
                float ratiox = m_frameSize.height * wh_ratio;
                m_vertices[0] = (-1.0)*m_frameSize.width / ratiox;
                m_vertices[1] = -1.0;
                m_vertices[2] = m_frameSize.width / ratiox;
                m_vertices[3] = -1.0;
                m_vertices[4] = m_vertices[0];
                m_vertices[5] = 1.0;
                m_vertices[6] = m_vertices[2];
                m_vertices[7] = 1.0;
            }
        }
            break;
            // 直接拉伸
        case ArcGLFillModeStretch:
        default:
            memcpy(&m_vertices[0], g_vertices, sizeof(GLfloat) * 8);
            break;
    }
    
    if(m_mirror) {
        m_vertices[0] = -m_vertices[0];
        m_vertices[2] = -m_vertices[2];
        m_vertices[4] = -m_vertices[4];
        m_vertices[6] = -m_vertices[6];
    }
    
    memcpy(&m_texCoor[0], coordinatesRotation(m_rotation), sizeof(GLfloat)*8);
    updateVertexArray();
    
    if(m_name == "ArcBlendImageFilter") {
        
    }
}

void ArcGLFilter::setOutputSize(ArcGLSize size) {
    ArcGLOutput::setOutputSize(size);
    calculateAspectRatio();
}

void ArcGLFilter::setFrameSize(ArcGLSize size) {
    ArcGLInput::setFrameSize(size);
    calculateAspectRatio();
}

ArcGLSize ArcGLFilter::sizeOfFBO() {
    if(m_outputSize.width != 0 && m_outputSize.height != 0) {
        return m_outputSize;
    }
    return m_frameSize;
}

void ArcGLFilter::setVertices(float *v) {
    memcpy(&m_vertices[0], v, sizeof(float) * 8);
}

void ArcGLFilter::setMirror(bool mirror) {
    m_mirror = mirror;
    
//    calculateAspectRatio();
}

void ArcGLFilter::createVertexBuffers() {
    m_context -> makeAsCurrent();
    
    m_vertexArray -> bind();
    
    ArcGLVBOCache* cache = ArcGLVBOCache::getInstance();
    
    m_vertexBuffer = cache -> newVertexBuffer();
    m_vertexBuffer -> activeBuffer(m_position);
    m_vertexBuffer -> setBuffer(&m_vertices[0]);
    
    m_texCoorBuffer = cache -> newVertexBuffer();
    m_texCoorBuffer -> activeBuffer(m_inputTexCoor);
    m_texCoorBuffer -> setBuffer(ArcGLFilter::coordinatesRotation(m_rotation));

    m_vertexArray -> unbind();
}

void ArcGLFilter::updateVertexArray() {
    
    m_vertexArray -> bind();
    m_vertexBuffer -> setBuffer(&m_vertices[0]);
    m_texCoorBuffer -> setBuffer(&m_texCoor[0]);
    m_vertexArray -> unbind();
}

void ArcGLFilter::newFrame() {
    
    if(!ready()) {
        return;
    }
    
    m_context -> makeAsCurrent();
    
    if (!m_enable) {
        unlockInputFrameBuffersForUse();
        return;
    }
    
    render();
    // render后的回调
    if (m_complete != nullptr) {
        m_complete(this, m_para);
    }
    unlockInputFrameBuffersForUse();
    
    //Debug
    if(m_name == "ArcSampleBufferFilter") {
        
    }
    
    if(m_name == "ArcGLBrightnessFilter") {
        
    }
    
    if(m_name == "ArcBlendImageFilter") {
        
    }
    
    if(m_name == "ArcBlendForEncodeFilter") {
        
    }
    
    if(m_name == "ArcGLWhiteningFilter") {
        
    }
    
    if(m_name == "ArcGLSmoothFilter") {
        
    }
    
    if(m_name == "ArcGLBeautyFilter") {
        
    }
    
    //Debug end
    
    informTargets();
    
}

void ArcGLFilter::activeOutFrameBuffer() {
    
    m_outFrameBuffer = m_context -> frameBuffer(m_outputSize, m_option);
    
    m_outFrameBuffer -> active();

    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ArcGLFilter::render() {
    
    m_context -> useProgram(m_vertexShader, m_fragmentShader);
    
    activeOutFrameBuffer();
    
    for (int i = 0; i < m_inputsNum; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        m_inputBuffers[i] -> activeTexture();
    }
    m_vertexArray -> bind();
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    for (int i = 0; i < m_inputsNum; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    m_vertexArray -> unbind();
    m_outFrameBuffer -> deactive();
}

void ArcGLFilter::setClearColor(float r, float g, float b){
    m_clearColor[0] = r;
    m_clearColor[1] = g;
    m_clearColor[2] = b;
}

void ArcGLFilter::setFloat(const string& name, GLfloat val){
    m_context->setFloat(name, val);
}
void ArcGLFilter::setFloat(const string& name, GLfloat *val, int num){
    m_context->setFloat(name, val, num);
}
void ArcGLFilter::setFloatv(const string& name, GLfloat* val, int num){
    m_context->setFloatv(name, val, num);
}

void ArcGLFilter::setMatrix(const string& name, GLfloat *val, int num){
    m_context->setMatrix(name, val, num);
}

void ArcGLFilter::setInteger(const string& name, int i){
    m_context->setInteger(name, i);
}

#pragma --mark "全局变量的定义"
const char* ArcGLFilter::g_vertext300_shader = SHADER300_STRING (
    in vec2 position;
    in vec2 inputTextureCoordinate;
    out vec2 textureCoordinate;
    void main()
    {
        gl_Position = vec4(position, 0, 1);
        textureCoordinate = inputTextureCoordinate;
    }
);

const char* ArcGLFilter::g_fragment300_shader_RGBA = SHADER300_STRING (
     in vec2 textureCoordinate;
     uniform sampler2D inputImageTexture[1];
     out vec4 out_color;
     void main()
     {
         out_color = texture(inputImageTexture[0], textureCoordinate);
     }
 );

const char* ArcGLFilter::g_fragment300_shader_BGRA = SHADER300_STRING (
      in vec2 textureCoordinate;
      uniform sampler2D inputImageTexture[1];
      out vec4 out_color;
      void main()
      {
          out_color = texture(inputImageTexture[0], textureCoordinate).bgra;
      }
);

const GLfloat ArcGLFilter::g_vertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f,  1.0f,
    1.0f,  1.0f,
};

const GLfloat* ArcGLFilter::coordinatesRotation(ArcGLRotation mode) {
    static const GLfloat noRotationTextureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    
    static const GLfloat rotateLeftTextureCoordinates[] = {
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
    };
    
    static const GLfloat rotateRightTextureCoordinates[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };
    
    static const GLfloat verticalFlipTextureCoordinates[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
    };
    
    static const GLfloat horizontalFlipTextureCoordinates[] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f,  1.0f,
        0.0f,  1.0f,
    };
    
    static const GLfloat rotateRightVerticalFlipTextureCoordinates[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
    };
    
    static const GLfloat rotateRightHorizontalFlipTextureCoordinates[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };
    
    /*static const GLfloat rotateLeftHorizontalFlipTextureCoordinates[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
    };*/
    
    static const GLfloat rotate180TextureCoordinates[] = {
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
    };
    
    switch(mode)
    {
        case ArcGLNoRotation: return noRotationTextureCoordinates;
        case ArcGLRotateLeft: return rotateLeftTextureCoordinates;
        case ArcGLRotateRight: return rotateRightTextureCoordinates;
        case ArcGLFlipVertical: return verticalFlipTextureCoordinates;
        case ArcGLFlipHorizonal: return horizontalFlipTextureCoordinates;
        case ArcGLRotateRightFlipVertical: return rotateRightVerticalFlipTextureCoordinates;
        case ArcGLRotateRightFlipHorizontal: return rotateRightHorizontalFlipTextureCoordinates;
        case ArcGLRotate180: return rotate180TextureCoordinates;
        default: return noRotationTextureCoordinates;
    }
}
