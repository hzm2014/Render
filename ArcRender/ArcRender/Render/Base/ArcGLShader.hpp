//
//  ArcGLShader.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLShader_hpp
#define ArcGLShader_hpp

#include <stdio.h>
#include "ArcGLHeader.h"
#include <string>

using namespace std;

class ArcGLShader {
    
public:
    
    ArcGLShader(const string& shaderStr);
    ArcGLShader() = delete;
    
    ~ArcGLShader();
    
    virtual bool compile() = 0;
    inline GLuint shader() { return m_shader; };
    
protected:
    
    virtual void createShader() = 0;
    
    GLuint m_shader;
    string m_shaderSrc;
};

#endif /* ArcGLShader_hpp */
