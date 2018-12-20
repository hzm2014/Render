//
//  ArcGLVertexShader.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLVertexShader.hpp"
#include <iostream>

ArcGLVertexShader::ArcGLVertexShader(const string& vertexShaderStr): ArcGLShader(vertexShaderStr) {
    createShader();
};

void ArcGLVertexShader::createShader() {
    const GLchar *source = m_shaderSrc.c_str();
    m_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_shader, 1, &source, NULL);
}

bool ArcGLVertexShader::compile() {
    GLint status;
    glCompileShader(m_shader);
    
    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &status);
    
    if (status != GL_TRUE)
    {
        GLint logLength;
        glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(m_shader, logLength, &logLength, log);
            std::cout<<"ArcVertexShader Compile shader Error: %s"<<log<<std::endl;
            free(log);
        }
    }
    
    return status == GL_TRUE;
}
