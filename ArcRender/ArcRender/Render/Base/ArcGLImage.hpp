//
//  ArcGLImage.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/12/1.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLImage_hpp
#define ArcGLImage_hpp

#include <stdio.h>
#include "ArcGLOutput.hpp"

class ArcGLImage: public ArcGLOutput {
public:

    ArcGLImage(void* cgImageRef);

    virtual ~ArcGLImage();
    
    virtual void informTargets();
    
protected:
    
    void* m_cgImagePtr;
    
    void setPixelToOutBuffer(uint8_t* data);
    
    virtual void createOutputFrameBuffer(ArcGLSize &size, ArcGLTextureOption &option);
};

#endif /* ArcGLImage_hpp */
