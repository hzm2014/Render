//
//  ArcGLTextureManager.hpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/16.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLTextureManager_hpp
#define ArcGLTextureManager_hpp

#include <stdio.h>
#include <vector>
#include "ArcGLTexture.hpp"


class ArcGLTextureManager {
    
    
public:
    
    ArcGLTextureManager();
    ~ArcGLTextureManager();
    
    ArcGLTexture* createTexture(const ArcGLTextureOption& option, ArcGLSize& size, int depth);
    GLint maxTextureUnitsForThisDevice();
    
protected:
    std::vector<ArcGLTexture *> m_textures;
    void removeAllTextures();
    
};

#endif /* ArcTextureManager_hpp */
