//
//  ArcSampleBufferFilter.h
//  ArcRender
//
//  Created by Zhiming He on 2018/11/27.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ArcNV12ToRGBFilter.hpp"
#include <AVFoundation/AVFoundation.h>
#import "ArciOSFrameBuffer.h"

class ArcSampleBufferFilter: public ArcNV12ToRGBFilter {
    
public:
    
    ArcSampleBufferFilter();
    ~ArcSampleBufferFilter();
    // 视频流处理
    void processSampleBuffer(CMSampleBufferRef sampleBuffer);
    void processPixelBuffer(CVImageBufferRef pixelBuffer);
    
    void enableBlackFrame(bool enable);
    
    
protected:
    
    OSType              m_format;        // 是否YUV数据
    ArciOSFrameBuffer*  m_rawBuffer;
    bool                m_enableBlackFrame = false; //是否启用黑帧
    
    virtual void informTargets();
};
