//
//  ArcGLFragmentShader.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLFragmentShader_hpp
#define ArcGLFragmentShader_hpp

#include <stdio.h>
#include "ArcGLShader.hpp"

class ArcGLFragmentShader: public ArcGLShader {
    
public:
    
    ArcGLFragmentShader(const string& vertexShaderStr);
    
    ArcGLFragmentShader() = delete;
    
    virtual bool compile();
    
protected:
    
    virtual void createShader();
    
};

#endif /* ArcGLFragmentShader_hpp */
