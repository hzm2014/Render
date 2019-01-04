//
//  ArcRGBFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2019/1/4.
//  Copyright © 2019 zhiming he. All rights reserved.
//

#ifndef ArcRGBFilter_hpp
#define ArcRGBFilter_hpp

#include <stdio.h>
#include "ArcGLFilter.hpp"

class ArcRGBFilter: public ArcGLFilter {
    
public:
    
    ArcRGBFilter();
    
    void setRed(float value);
    void setGreen(float value);
    void setBlue(float value);
    
protected:
    
    float m_red = 1.0;
    float m_green = 1.0;
    float m_blue = 1.0;
    
};

#endif /* ArcRGBFilter_hpp */
