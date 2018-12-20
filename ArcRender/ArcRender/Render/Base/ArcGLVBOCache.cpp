//
//  ArcGLVBOCache.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/26.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLVBOCache.hpp"
#include "ArcGLVertexBuffer.hpp"

ArcGLVBOCache* ArcGLVBOCache::m_instance = new ArcGLVBOCache();

ArcGLVBOCache* ArcGLVBOCache::getInstance() {
    return m_instance;
}

void ArcGLVBOCache::destroyInstance(){
    if(m_instance != nullptr) {
        delete m_instance;
        m_instance = nullptr;
    }
}

ArcGLVBOCache::ArcGLVBOCache() {
    
}

ArcGLVertexBuffer* ArcGLVBOCache::vertexBuffer(unsigned vertexCount, unsigned dimension, bool cache){
    for (int i = 0; i < m_bufferlist.size(); i++) {
        ArcGLVertexBuffer* buffer = m_bufferlist[i];
        if (buffer -> count() == vertexCount && buffer -> dimension() == dimension
            && buffer -> cache() == cache) {
            return buffer;
        }
    }
    
    ArcGLVertexBuffer* buffer = newVertexBuffer(vertexCount, dimension, cache);
    m_bufferlist.push_back(buffer);
    return buffer;
}

ArcGLVertexBuffer* ArcGLVBOCache::newVertexBuffer(unsigned vertexCount, unsigned dimension, bool cache){
    ArcGLVertexBuffer* buffer = new ArcGLVertexBuffer(vertexCount, dimension, cache);
    return buffer;
}

void ArcGLVBOCache::clear(){
    for (int i=0; i<m_bufferlist.size(); i++) {
        ArcGLVertexBuffer* buffer = m_bufferlist[i];
        delete buffer;
    }
    m_bufferlist.clear();
}

ArcGLVBOCache::~ArcGLVBOCache(){
    clear();
}
