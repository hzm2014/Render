//
//  ArcGLWhiteningFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/21.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLWhiteningFilter_hpp
#define ArcGLWhiteningFilter_hpp

#include <stdio.h>
#include "ArcGLFilter.hpp"

class ArcGLWhiteningFilter: public ArcGLFilter {
    
public:
    
    ArcGLWhiteningFilter(float s = 0);
    
    void setIdeal(float ideal[4]);
    void setThreshold(float thresh[4]);
    
    /**
     设置美白等级

     @param s 范围为[0, 1];
     */
    void setStrength(float s);
    
protected:
    
    GLfloat m_idealpnts[8];
    GLfloat m_threshold[4];
    float     m_strength;
    
    GLfloat m_rgb2yuv[9];
    GLfloat m_off2yuv[3];
    
    virtual void calculateAspectRatio();
};

#endif /* ArcGLWhiteningFilter_hpp */
