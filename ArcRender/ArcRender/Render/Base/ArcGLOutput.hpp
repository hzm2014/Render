//
//  ArcGLOutput.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLOutput_hpp
#define ArcGLOutput_hpp

#include <stdio.h>
#include "ArcGLFrameBuffer.hpp"
#include <vector>

class ArcGLInput;
class ArcGLOutput;

typedef void (*ArcRenderCompleteCallback)(ArcGLOutput *output, void *para);

class ArcGLOutput {
    
public:
    
    ArcGLOutput();
    virtual ~ArcGLOutput();
    
    virtual int addTarget(ArcGLInput* target, int location = 0);
    virtual bool removeTarget(ArcGLInput *target, int* location = nullptr);
    virtual bool removeAllTargets();
    virtual void informTargets();
    
    inline unsigned targetsCount() { return static_cast<unsigned>(m_targets.size()); }
    ArcGLInput* target(unsigned loc);
    
    /**
     设置Render完成后的回调函数

     @param complete 回调函数
     @param para 参数
     */
    inline void setCompleteCallback(ArcRenderCompleteCallback complete, void* para) {
        m_complete = complete;
        m_para = para;
    }
    
    virtual void setOutputSize(ArcGLSize size);
    inline ArcGLSize outputSize() { return m_outputSize; }

    FrameBufferPtr           m_outFrameBuffer;
protected:
    std::vector<ArcGLInput*> m_targets;
    std::vector<int>         m_location;

    ArcRenderCompleteCallback m_complete;
    void*                     m_para;

    // 输出尺寸
    ArcGLSize m_outputSize;
    
};

#endif /* ArcGLOutput_hpp */
