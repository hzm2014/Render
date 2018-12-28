//
//  ArcGLContext.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLContext.hpp"
#ifdef __ANDROID__
#include "ArcAndroidContext.hpp"
#else
#include "ArciOSContext.h"
#endif

#ifdef __ANDROID__
    ArcGLContext* ArcGLContext::m_instance = new ArcAndroidContext;
#else
    ArcGLContext* ArcGLContext::m_instance = new ArciOSContext;
#endif


ArcGLContext* ArcGLContext::getInstance() {
    if(m_instance == nullptr) {
#ifdef __ANDROID__
        m_instance = new ArcAndroidContext;
#else
        m_instance = new ArciOSContext;
#endif
    }
    return m_instance;
}

ArcGLContext::ArcGLContext() {
    m_program = nullptr;
    m_frameBufferManager = nullptr;
}

void ArcGLContext::createProgram(const string &vertexShaderStr, const string &fragmentShaderStr) {
    m_program = new ArcGLProgram(vertexShaderStr, fragmentShaderStr);
    m_program -> attach();
    m_program -> link();
    
    m_programCache.insert(ProgramMap::value_type (vertexShaderStr + fragmentShaderStr, m_program));
}

ArcGLProgram* ArcGLContext::getProgram(const string &vertexShaderStr, const string &fragmentShaderStr) {
    ProgramMap::iterator iter = m_programCache.find(vertexShaderStr + fragmentShaderStr);
    if(iter == m_programCache.end()) {
        //no find
        return nullptr;
    }
    return iter -> second;
}

void ArcGLContext::useProgram(const string &vertexShaderStr, const string &fragmentShaderStr) {
    ArcGLProgram* program = getProgram(vertexShaderStr, fragmentShaderStr);
    if(program == nullptr) {
        createProgram(vertexShaderStr, fragmentShaderStr);
    } else {
        m_program = program;
    }
    
    m_program -> use();
}

void ArcGLContext::activeProgram() {
    if(m_program != nullptr) {
        m_program -> use();
    }
}

GLuint ArcGLContext::attribute(const string name) const {
    return m_program -> attibute(name);
}

GLuint ArcGLContext::uniform(const string name) const {
    return m_program -> uniform(name);
}

void ArcGLContext::setUniformIndex(string name, unsigned size, GLint* index) {
    GLint location = uniform(name);
    for (int i=0; i < size; i++) {
        index[i] = location + i;
    }
    if (location >= 0) {
        glUniform1iv(location, size, index);
    }
}

void ArcGLContext::removeAllPrograms() {
    for(ProgramMap::iterator iter = m_programCache.begin(); iter != m_programCache.end(); iter++) {
        ArcGLProgram* program = iter -> second;
        delete program;
    }
    m_programCache.clear();
    m_program = nullptr;
}

FrameBufferPtr ArcGLContext::frameBuffer(ArcGLSize &size, ArcGLTextureOption &option, bool onlyTexture) {
    return m_frameBufferManager -> frameBuffer(size, option, onlyTexture);
}

FrameBufferPtr ArcGLContext::frameBuffer(ArcGLSize &size, ArcGLTextureOption &option, GLuint texture) {
    return m_frameBufferManager -> frameBuffer(size, option, texture);
}

ArcGLSize ArcGLContext::sizeFitsTextureMaxSize(ArcGLSize inputSize) {
    makeAsCurrent();
    int maxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    
    if ( (inputSize.width < maxTextureSize) && (inputSize.height < maxTextureSize) )
    {
        return inputSize;
    }
    
    ArcGLSize adjustedSize;
    if (inputSize.width > inputSize.height)
    {
        adjustedSize.width = maxTextureSize;
        adjustedSize.height = (maxTextureSize*1.0 / inputSize.width) * inputSize.height;
    }
    else
    {
        adjustedSize.height = maxTextureSize;
        adjustedSize.width = (maxTextureSize*1.0 / inputSize.height) * inputSize.width;
    }
    
    return adjustedSize;
}

void ArcGLContext::setInteger(const string& name, int val) {
    m_program -> setInteger(name, val);
}

void ArcGLContext::setIntegerv(const string& name, int* val, int num) {
    m_program -> setIntegerv(name, val, num);
}

void ArcGLContext::setUIntegerv(const string& name, uint32_t* val, int num){
    m_program -> setUIntegerv(name, val, num);
}

void ArcGLContext::setFloat(const string& name, GLfloat val) {
    m_program -> setFloat(name, val);
}

void ArcGLContext::setFloat(const string& name, GLfloat* val, int num) {
    m_program -> setFloat(name, val, num);
}

void ArcGLContext::setFloatv(const string& name, GLfloat* val, int num) {
    m_program -> setFloatv(name, val, num);
}

void ArcGLContext::setMatrix(const string& name, GLfloat* val, int num) {
    m_program -> setMatrix(name, val, num);
}

ArcGLContext::~ArcGLContext() {
    removeAllPrograms();
}

void ArcGLContext::destroyInstance() {
    if(m_instance != nullptr) {
        delete m_instance;
        m_instance = nullptr;
    }
}
