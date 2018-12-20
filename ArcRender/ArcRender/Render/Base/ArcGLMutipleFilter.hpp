//
//  ArcGLMutipleFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/27.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLMutipleFilter_hpp
#define ArcGLMutipleFilter_hpp

#include <stdio.h>
#include "ArcGLFilter.hpp"

class ArcGLMutipleFilter: public ArcGLFilter {
    
public:
    
    ArcGLMutipleFilter(string& vertexShader, string& fragmentShader);
    virtual ~ArcGLMutipleFilter();
    virtual void newFrame();
    virtual void setInputFrameBuffer(FrameBufferPtr frameBuffer, unsigned location = 0);
    virtual void setOutputRotation(ArcGLRotation rotation);
    
    inline void setFirstFilter(ArcGLFilter* filter) {
        m_input = filter;
    }

    inline void setLastFilter(ArcGLFilter* filter) {
        m_output = filter;
    }

    inline ArcGLFilter* firstFilter() { return m_input; }
    inline ArcGLFilter* lastFilter() { return m_output; }
    
protected:

    ArcGLFilter*  m_input;    // Group内第一个filter
    ArcGLFilter*  m_output;   // Group内最后一个filter
};

#endif /* ArcGLMutipleFilter_hpp */
