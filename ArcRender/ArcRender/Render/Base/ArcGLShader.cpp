//
//  ArcGLShader.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLShader.hpp"

ArcGLShader::ArcGLShader(const string& shaderStr): m_shaderSrc(shaderStr) {
    m_shader = 0;
}

ArcGLShader::~ArcGLShader() {
    
    if(m_shader) {
        glDeleteShader(m_shader);
    }
}
