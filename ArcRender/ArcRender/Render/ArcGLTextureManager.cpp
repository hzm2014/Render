//
//  ArcTextureManager.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/16.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLTextureManager.hpp"
#include <mutex>
#include <iostream>

using namespace std;

ArcGLTexture* ArcGLTextureManager::createTexture(const ArcGLTextureOption& option, ArcGLSize& size, int depth) {
    ArcGLTexture * texture = new ArcGLTexture(option, size);
    m_textures.push_back(texture);
    return texture;
}

GLint ArcGLTextureManager::maxTextureUnitsForThisDevice() {
    static GLint num;
    static once_flag flag;
    call_once(flag,[]() {
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &num);
    });
    return num;
}

void ArcGLTextureManager::removeAllTextures() {
    for(int i = 0; i < m_textures.size(); i++) {
        ArcGLTexture * texture = m_textures[i];
        delete texture;
        texture = nullptr;
    }
    m_textures.clear();
}

ArcGLTextureManager::~ArcGLTextureManager() {
    removeAllTextures();
}








