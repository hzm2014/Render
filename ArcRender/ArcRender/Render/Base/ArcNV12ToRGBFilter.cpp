//
//  ArcNV12ToRGBFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/11/27.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcNV12ToRGBFilter.hpp"

static const char* g_nv12torgb_fragment_shader = SHADER300_STRING(
    in vec2 textureCoordinate;
    uniform sampler2D inputImageTexture[2];
    out vec4 out_color;
    mediump mat3 colorConversionMatrix = mat3(1.0, 1.0, 1.0, 0.0, -0.343, 1.765, 1.4, -0.711, 0.0);

    void main()
    {
       mediump vec3 yuv;
       mediump vec3 rgb;
       
       yuv.x = texture(inputImageTexture[0], textureCoordinate).r;
       yuv.yz = texture(inputImageTexture[1], textureCoordinate).ra - vec2(0.5, 0.5);
       rgb = colorConversionMatrix * yuv;
       
       out_color = vec4(rgb, 1.0).bgra;
    }
);

static string vertexShader = ArcGLFilter::g_vertext300_shader;
static string fragmentShader = g_nv12torgb_fragment_shader;

ArcNV12ToRGBFilter::ArcNV12ToRGBFilter(): ArcGLFilter(vertexShader, fragmentShader) {
    m_name = "ArcNV12ToRGBFilter";
    setInputs(2);
}

ArcNV12ToRGBFilter::~ArcNV12ToRGBFilter() {
    
}
