//
//  ArcRenderView.h
//  ArcRender
//
//  Created by Zhiming He on 2018/11/27.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import "ArcGLInput.hpp"
#import "ArcGLView.h"

class ArcRenderView: public ArcGLInput {
    
public:
    
    ArcRenderView(CGRect rect);
    ~ArcRenderView();
    
    void setPreviewColor(UIColor* color);
    virtual void setInputs(unsigned inputsNum);
    void setFillMode(ArcGLFillMode mode);
    virtual void setInputFrameBuffer(FrameBufferPtr frameBuffer, unsigned location = 0);
    virtual void setOutputRotation(ArcGLRotation rotation);
    virtual void newFrame();
    
    inline ArcGLView* view() { return m_view; }
    
protected:
    ArcGLView*  m_view;
};
