//
//  ArcGLTwoPassFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/25.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLTwoPassFilter_hpp
#define ArcGLTwoPassFilter_hpp

#include <stdio.h>
#include "ArcGLFilter.hpp"

class ArcGLTwoPassFilter: public ArcGLFilter {
    
public:
    
    ArcGLTwoPassFilter(const string& vertexShader, const string& fragmentShader, const string& secVertexShader, const string& secFragmentShader);
    
    virtual ~ArcGLTwoPassFilter();
    
    virtual void newFrame();
    
    void activeSecOutFrameBuffer();
        
protected:
    
    const string m_secVertexShader;
    const string m_secFragmentShader;
    FrameBufferPtr m_secOutFrameBuffer;
    
    
};

#endif /* ArcGLTwoPassFilter_hpp */
