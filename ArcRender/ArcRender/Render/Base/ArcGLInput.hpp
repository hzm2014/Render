//
//  ArcGLInput.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLInput_hpp
#define ArcGLInput_hpp

#include <stdio.h>
#include <vector>
#include "ArcGLFrameBuffer.hpp"

class ArcGLOutput;

class ArcGLInput {
    
public:
    
    ArcGLInput(unsigned inputsNum = 1);
    virtual ~ArcGLInput();
    
    virtual void setInputs(unsigned inputsNum);
    virtual void setInputFrameBuffer(FrameBufferPtr frameBuffer, unsigned location = 0);
    virtual void newFrame();
    
    inline virtual void setOutputRotation(ArcGLRotation rotation) { m_rotation = rotation; }
    inline virtual ArcGLRotation getOutputRotation() { return m_rotation; }
    virtual void setFrameSize(ArcGLSize size);
    inline virtual ArcGLSize frameSize() { return m_frameSize; }
    
    inline virtual void disable() { m_enable = true; }
    inline virtual void enable() { m_enable = false; }
    
    void unlockInputFrameBuffersForUse();
    bool ready();
    
protected:
    
    unsigned m_inputsNum;
    std::vector<FrameBufferPtr> m_inputBuffers;
    FrameBufferPtr m_firstFrameBuffer;
    bool m_enable = true; //是否执行newFrame
    bool m_ready = false; //Input Buffer是否都是非空
    
    ArcGLSize m_frameSize;
    ArcGLRotation m_rotation;
};

#endif /* ArcGLInput_hpp */
