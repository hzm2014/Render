//
//  ArcGLFragmentShader.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLFragmentShader.hpp"
#include <iostream>

ArcGLFragmentShader::ArcGLFragmentShader(const string& vertexShaderStr): ArcGLShader(vertexShaderStr) {
    createShader();
}

void ArcGLFragmentShader::createShader() {
    const GLchar *source = m_shaderSrc.c_str();
    m_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_shader, 1, &source, NULL);
}

bool ArcGLFragmentShader::compile() {
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
            std::cout<<"ArcGLFragmentShader Compile shader Error: %s"<<log<<std::endl;
            free(log);
        }
    }
    
    return status == GL_TRUE;
}
