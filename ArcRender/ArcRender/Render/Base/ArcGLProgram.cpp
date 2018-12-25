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
            }
            free(buf);
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

void ArcGLProgram::setInteger(const string& name, int val){
    
    GLint index = uniform(name);
    if(index >= 0)
        glUniform1i(index, val);
}

void ArcGLProgram::setIntegerv(const string& name, int* val, int num){
    
    GLint index = uniform(name);
    glUniform1iv(index, num, val);
}

void ArcGLProgram::setUIntegerv(const string& name, uint32_t* val, int num){
   
    GLint index = uniform(name);
    glUniform1uiv(index, num, val);
}

void ArcGLProgram::setFloat(const string& name, GLfloat val){
    setFloat(name, &val, 1);
}

void ArcGLProgram::setFloat(const string& name, GLfloat* val, int num){

    GLint index = uniform(name);
    if(index >= 0) {
        switch(num){
            case 1:    glUniform1f(index, *val);
                break;
            case 2:    glUniform2fv(index, 1, val);
                break;
            case 3:    glUniform3fv(index, 1, val);
                break;
            case 4:    glUniform4fv(index, 1, val);
                break;
        }
    }
}

void ArcGLProgram::setFloatv(const string& name, GLfloat* val, int num){
    
    GLint index = uniform(name);
    if(index >= 0)
        setFloat(name, val, num);
}

void ArcGLProgram::setMatrix(const string& name, GLfloat* val, int num){
    
    GLint index = uniform(name);
    if(index >= 0) {
        switch(num){
            case 2:    glUniformMatrix2fv(index, 1, GL_FALSE, val);
                break;
            case 3:    glUniformMatrix3fv(index, 1, GL_FALSE, val);
                break;
            case 4:    glUniformMatrix4fv(index, 1, GL_FALSE, val);
                break;
        }
    }
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
