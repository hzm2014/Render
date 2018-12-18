//
//  ArcBlendImageFilter.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/12/1.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcBlendImageFilter_hpp
#define ArcBlendImageFilter_hpp

#include <stdio.h>
#include "ArcGLFilter.hpp"

class ArcBlendImageFilter: public ArcGLFilter {
    
public:
    
    ArcBlendImageFilter(ArcGLRect rect, ArcGLSize viewSize);
    virtual ~ArcBlendImageFilter();
    
    void updateImageRectViewSize(ArcGLRect rect, ArcGLSize viewSize);
    
    virtual void setInputFrameBuffer(FrameBufferPtr frameBuffer, unsigned location = 0);
    
    virtual void activeOutFrameBuffer();
        
    virtual void newFrame();
    
    virtual void render();
    
protected:
    
    ArcGLRect m_imageRect;
    ArcGLSize m_viewSize;
    ArcGLSize m_adjustedViewSize; //按照输出比例调整过的viewSize
    
    virtual void calculateAspectRatio();
    void calculateAdjustViewSize();
    virtual void calculateImageRect(ArcGLRect rect);
    void updateImageVertex();
};

#endif /* ArcBlendImageFilter_hpp */
