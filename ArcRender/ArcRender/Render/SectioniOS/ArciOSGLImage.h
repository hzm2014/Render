//
//  ArciOSGLImage.h
//  ArcRender
//
//  Created by zhiming he on 2018/12/2.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArcGLImage.hpp"

class ArciOSGLImage: public ArcGLImage {
    
public:
    
    ArciOSGLImage(void* cgImageRef);
    ~ArciOSGLImage();
    
protected:
    
    virtual void createOutputFrameBuffer(ArcGLSize &size, ArcGLTextureOption &option);
    void process(void* cgImageRef);
};
