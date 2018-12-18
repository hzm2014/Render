//
//  ArcGLProgram.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLProgram_hpp
#define ArcGLProgram_hpp

#include <stdio.h>
#include "ArcGLHeader.h"
#include <string>
#include "ArcGLVertexShader.hpp"
#include "ArcGLFragmentShader.hpp"

using namespace std;

class ArcGLProgram {
    
    
public:
    
    ArcGLProgram(const string& vertexShaderStr, const string& fragmentShaderStr);
    
    ArcGLProgram() = delete;
    
    ~ArcGLProgram();
    
    void attach() const;
    
    bool link();
    
    void use() const;
    
    const GLuint uniform(const string& name) const;
    const GLuint attibute(const string& name) const;
    
    bool initialized() const;
    
protected:
    
    GLuint m_program;
    ArcGLVertexShader* m_vertexShader;
    ArcGLFragmentShader* m_fragmentShader;
    bool m_initialized;
    
    void createProgram();
    bool createShaders(const string& vertexShaderStr, const string& fragmentShaderStr);
};

#endif /* ArcGLProgram_hpp */
