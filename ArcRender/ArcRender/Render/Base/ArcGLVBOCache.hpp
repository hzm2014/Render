//
//  ArcGLVBOCache.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/26.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLVBOCache_hpp
#define ArcGLVBOCache_hpp

#include <stdio.h>
#include <vector>

class ArcGLVertexBuffer;

class ArcGLVBOCache{
public:
    static ArcGLVBOCache* getInstance();
    static void destroyInstance();
    
    ArcGLVBOCache();
    ~ArcGLVBOCache();
    
    ArcGLVertexBuffer* vertexBuffer(unsigned vertexCount = 4 , unsigned dimension = 2, bool cache = true);
    virtual ArcGLVertexBuffer* newVertexBuffer(unsigned vertexCount = 4 , unsigned dimension = 2, bool cache = true);
    void clear();
    /**
     * 设置是否使用顶点缓存。
     */
    static void setCache(bool cache){
        m_cache = cache;
    }
    
public:
    static ArcGLVBOCache*           m_instance;
    static bool                     m_cache;
    
    std::vector<ArcGLVertexBuffer*> m_bufferlist;
};

#endif /* ArcGLVBOCache_hpp */
