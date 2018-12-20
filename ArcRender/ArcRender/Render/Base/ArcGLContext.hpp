//
//  ArcGLContext.hpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLContext_hpp
#define ArcGLContext_hpp

#include <stdio.h>
#include "ArcGLHeader.h"
#include "ArcGLProgram.hpp"
#include <map>
#include "ArcGLFrameBufferManager.hpp"

class ArcGLContext {
    
public:
    
    static ArcGLContext* getInstance();
    static void destroyInstance();
    
    ArcGLContext();
    virtual ~ArcGLContext();

    virtual void makeAsCurrent() const = 0;
    virtual void swapBuffer(void* surface = NULL) = 0;
    
    /**
     use program, first it comes from cache, if no, then create

     @param vertexShaderStr vertex shader source string
     @param fragmentShaderStr fragment shader source string
     */
    void useProgram(const string& vertexShaderStr, const string& fragmentShaderStr);
    
    void activeProgram();
    
    FrameBufferPtr frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, bool onlyTexture = false);
    FrameBufferPtr frameBuffer(ArcGLSize& size, ArcGLTextureOption& option, GLuint texture);
    
    GLuint attribute(const string name) const;
    GLuint uniform(const string name) const;
    
    /**
     设置uniform变量位置

     @param name 变量名称
     @param size 变量个数
     @param index 变量位置值指针
     */
    void setUniformIndex(string name, unsigned size, GLint* index);
    
    ArcGLSize sizeFitsTextureMaxSize(ArcGLSize inputSize);
        
protected:
    
    using ProgramMap = map<string, ArcGLProgram*>;
    
    ArcGLProgram * m_program;
    ProgramMap m_programCache;
    
    ArcGLFrameBufferManager* m_frameBufferManager;
    
    virtual void createContext() = 0;
    
    void createProgram(const string& vertexShaderStr, const string& fragmentShaderStr);
    ArcGLProgram* getProgram(const string& vertexShaderStr, const string& fragmentShaderStr);
    virtual void createFrameBufferManager() = 0;
    
    void removeAllPrograms();
    
private:
    
    static ArcGLContext* m_instance;
};


#endif /* ArcGLContext_hpp */
