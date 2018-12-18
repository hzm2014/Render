//
//  ArcGLVertexArray.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/26.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLVertexArray_hpp
#define ArcGLVertexArray_hpp

#include <stdio.h>
#include "ArcGLHeader.h"
#include "ArcGLVertexBuffer.hpp"

class ArcGLVertexArray {
    
public:
    
    ArcGLVertexArray();
    ~ArcGLVertexArray();
    
    void bind();
    void unbind();
    
protected:
    
    void createVAO();
    
    GLuint m_vertexArray;
};

#endif /* ArcGLVertexArray_hpp */
