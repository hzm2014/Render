//
//  ArciOSContext.h
//  ArcRender
//
//  Created by zhiming he on 2018/11/18.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArcGLContext.hpp"

struct GL_Context;

class ArciOSContext: public ArcGLContext {
    
public:
    
    ArciOSContext();
    ~ArciOSContext();
    
    void* context() const;
    virtual void makeAsCurrent() const;
    virtual void swapBuffer(void* surface = NULL);
    
    void* coreVideoTextureCache() const;
    
    void* runProcess() const;
    
protected:
    
    GL_Context* m_glContext;
    
    void* m_runProcess;
    
    void createRunProcess();
    
    virtual void createContext();
    void createCoreVideoTextureCache();
    
    virtual void createFrameBufferManager();
};
