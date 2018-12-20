//
//  ArcGLBrightnessFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/20.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLBrightnessFilter_hpp
#define ArcGLBrightnessFilter_hpp

#include <stdio.h>
#include "ArcGLFilter.hpp"

class ArcGLBrightnessFilter: public ArcGLFilter {

public:
    
    ArcGLBrightnessFilter(float brightness = 0.0);
    ~ArcGLBrightnessFilter();
    
    void setBrightness(float value = 0.0);
    inline float brightness() { return m_brightness; }
        
protected:
    
    // Brightness ranges from -1.0 to 1.0, with 0.0 as the normal level
    float m_brightness = 0.0;
    GLuint m_uniformBrightness = 0;
};

#endif /* ArcGLBrightnessFilter_hpp */
