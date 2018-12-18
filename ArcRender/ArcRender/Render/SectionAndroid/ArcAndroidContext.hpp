//
//  ArcAndroidContext.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/18.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcAndroidContext_hpp
#define ArcAndroidContext_hpp

#include <stdio.h>
#include "ArcGLContext.hpp"

class ArcAndroidContext: public ArcGLContext {
    
public:
    
    ArcAndroidContext();
    ~ArcAndroidContext();
    
    virtual void createContext();
    virtual void makeAsCurrent() const;
    virtual void swapBuffer(void* surface = NULL);
    
    virtual void createFrameBufferManager();
    
protected:
    
    
};

#endif /* ArcAndroidContext_hpp */
