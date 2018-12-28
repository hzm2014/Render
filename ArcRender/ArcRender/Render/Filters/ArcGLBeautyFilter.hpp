//
//  ArcGLBeautyFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/25.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLBeautyFilter_hpp
#define ArcGLBeautyFilter_hpp

#include <stdio.h>
#include "ArcGLFilter.hpp"

class ArcGLBeautyFilter: public ArcGLFilter {
    
public:
    
    ArcGLBeautyFilter();
    ~ArcGLBeautyFilter();
    
    virtual void setFrameSize(ArcGLSize size);
    
protected:
    
    float m_beautyLevel;
    float m_brightLevel;
    float m_toneLevel;
    
    void setBrightLevel(float level);
    void setBeautyLevel(float level);
    void setParams(float beautyLevel, float toneLevel);
    
};

#endif /* ArcGLBeautyFilter_hpp */
