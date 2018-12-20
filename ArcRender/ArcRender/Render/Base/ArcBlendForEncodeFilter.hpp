//
//  ArcBlendForEncodeFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/5.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcBlendForEncodeFilter_hpp
#define ArcBlendForEncodeFilter_hpp

#include <stdio.h>
#include "ArcBlendImageFilter.hpp"

class ArcBlendForEncodeFilter: public ArcBlendImageFilter {
    
public:
    
    ArcBlendForEncodeFilter(ArcGLRect rect, ArcGLSize viewSize);
    ~ArcBlendForEncodeFilter();
    
    virtual void newFrame();
    
protected:
    
    virtual void calculateImageRect(ArcGLRect rect);
    
};



#endif /* ArcBlendForEncodeFilter_hpp */
