//
//  ArcGLProgram.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLProgram.hpp"
#include <iostream>

ArcGLProgram::ArcGLProgram(const string& vertexShaderStr, const string& fragmentShaderStr) {
    m_program = 0;
    m_initialized = false;
    m_vertexShader = nullptr;
    m_fragmentShader = nullptr;
    
    createProgram();
    createShaders(vertexShaderStr, fragmentShaderStr);
}

void ArcGLProgram::createProgram() {
    m_program = glCreateProgram();
}

bool ArcGLProgram::createShaders(const string &vertexShaderStr, const string &fragmentShaderStr) {
    
    m_vertexShader = new ArcGLVertexShader(vertexShaderStr);
    bool ret1 = m_vertexShader -> compile();
    if(ret1 == false) {
        return false;
    }
    m_fragmentShader = new ArcGLFragmentShader(fragmentShaderStr);
    bool ret2 = m_fragmentShader -> compile();
    if(ret2 == false) {
        return false;
    }
    
    return true;
}

void ArcGLProgram::attach() const {
    glAttachShader(m_program, m_vertexShader -> shader());
    glAttachShader(m_program, m_fragmentShader -> shader());
}

bool ArcGLProgram::link() {
    
    GLint status = GL_FALSE;
    
    glLinkProgram(m_program);
    
    glGetProgramiv(m_program, GL_LINK_STATUS, &status);
    
    if(status != GL_TRUE) {
        
        GLint bufferLen = 0;
        glGetProgramiv(m_program, GL_LINK_STATUS, &bufferLen);
        if (bufferLen) {
            char* buf = (char*) malloc(bufferLen);
            if (buf) {
                glGetProgramInfoLog(m_program, bufferLen, NULL, buf);
                std::cout<<"Visionin Error: Could not link program:%s"<<buf<<std::endl;
                free(buf);
            }
        }
        
        return false;
    }
    
    if (m_vertexShader != nullptr) {
        delete m_vertexShader;
        m_vertexShader = nullptr;
    }
    
    if (m_fragmentShader != nullptr) {
        delete m_fragmentShader;
        m_fragmentShader = nullptr;
    }
    
    m_initialized = true;
    
    return true;
}

void ArcGLProgram::use() const {
    glUseProgram(m_program);
}

const GLuint ArcGLProgram::uniform(const string &name) const {
    return glGetUniformLocation(m_program, name.c_str());
}

const GLuint ArcGLProgram::attibute(const string &name) const {
    return  glGetAttribLocation(m_program, name.c_str());
}

ArcGLProgram::~ArcGLProgram() {
    
    if(m_program) {
        glDeleteProgram(m_program);
    }
    
    if(m_vertexShader != nullptr) {
        delete m_vertexShader;
        m_vertexShader = nullptr;
    }
    
    if(m_fragmentShader != nullptr) {
        delete m_fragmentShader;
        m_fragmentShader = nullptr;
    }
}
