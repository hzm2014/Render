//
//  ArcGLVertexShader.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLVertexShader_hpp
#define ArcGLVertexShader_hpp

#include <stdio.h>
#include "ArcGLShader.hpp"

class ArcGLVertexShader: public ArcGLShader {
    
public:
    
    ArcGLVertexShader(const string& vertexShaderStr);
    
    ArcGLVertexShader() = delete;
    
    virtual bool compile();
    
protected:
    
    virtual void createShader();
    
};

#endif /* ArcGLVertexShader_hpp */
