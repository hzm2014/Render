//
//  ArcGLBrightnessFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/20.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLBrightnessFilter.hpp"
#include <iostream>
#include "ArcGLContext.hpp"

static const char* k_Brightness_FragmentShaderString = SHADER300_STRING (
    in vec2 textureCoordinate;

    uniform sampler2D inputImageTexture[1];
    uniform float brightness;
                                                                       
    out vec4 out_color;

    void main() {
     vec4 textureColor = texture(inputImageTexture[0], textureCoordinate);

     out_color = vec4((textureColor.rgb + vec3(brightness)), textureColor.w);
    }
);

static string vertexShader = ArcGLFilter::g_vertext300_shader;
static string fragmentShader = k_Brightness_FragmentShaderString;

ArcGLBrightnessFilter::ArcGLBrightnessFilter(float value): ArcGLFilter(vertexShader, fragmentShader) {
    m_name = "ArcGLBrightnessFilter";
    m_uniformBrightness = m_context -> uniform("brightness");
    setInputs(1);
    setBrightness(value);
}

ArcGLBrightnessFilter::~ArcGLBrightnessFilter() {
    
}

void ArcGLBrightnessFilter::setBrightness(float value) {
    if(value > 1.0 || value < -1.0) {
        std::cout<<"Error setBrightness value exceed the range!"<<std::endl;
        return;
    }
    m_brightness = value;
    glUniform1f(m_uniformBrightness, m_brightness);
}

