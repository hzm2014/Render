//
//  ArcGLFrameBufferManager.cpp
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLFrameBufferManager.hpp"
#include <iostream>

ArcGLFrameBufferManager::ArcGLFrameBufferManager() {
    
}

ArcGLFrameBufferManager::~ArcGLFrameBufferManager() {
    
}

string ArcGLFrameBufferManager::hashForFrameBuffer(const ArcGLSize &size, const ArcGLTextureOption &option) const {
    
    string hashStr = to_string(size.width) + to_string(size.height) + to_string(option.min_filter) + to_string(option.mag_filter) + to_string(option.wrap_s) + to_string(option.wrap_t) + to_string(option.color_format) + to_string(option.format) + to_string(option.type);
    
    return hashStr;
}
    

