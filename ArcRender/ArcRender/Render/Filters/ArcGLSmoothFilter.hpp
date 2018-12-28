//
//  ArcGLSmoothFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/24.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLSmoothFilter_hpp
#define ArcGLSmoothFilter_hpp

#include <stdio.h>
#include "ArcGLFilter.hpp"

class ArcGLSmoothFilter: public ArcGLFilter {
    
public:
    
    ArcGLSmoothFilter(float s = 0);
    
    ~ArcGLSmoothFilter();
    
    /**
     设置磨皮度

     @param s 磨皮度范围为[0, 1]
     */
    void setSmooth(float s = 0);
    
protected:
    
    float m_smoothValue = 0;
        
};

#endif /* ArcGLSmoothFilter_hpp */
