//
//  ArcRGBFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2019/1/4.
//  Copyright © 2019 zhiming he. All rights reserved.
//

#include "ArcRGBFilter.hpp"

static const char* k_GBFragmentShaderString = SHADER300_STRING (
    in highp vec2 textureCoordinate;
    out vec4 out_color;
                                                                
    uniform sampler2D inputImageTexture[1];
                                                                
    uniform highp float redAdjustment;
    uniform highp float greenAdjustment;
    uniform highp float blueAdjustment;

    void main() {
     highp vec4 textureColor = texture(inputImageTexture[0], textureCoordinate);
     
     out_color = vec4(textureColor.r * redAdjustment, textureColor.g * greenAdjustment, textureColor.b * blueAdjustment, textureColor.a);
    }
);

static string vertexShader = ArcGLFilter::g_vertext300_shader;
static string fragmentShader = k_GBFragmentShaderString;

ArcRGBFilter::ArcRGBFilter(): ArcGLFilter(vertexShader, fragmentShader) {
    setInputs(1);
    
    setFloat("redAdjustment", m_red);
    setFloat("greenAdjustment", m_green);
    setFloat("blueAdjustment", m_blue);
}

void ArcRGBFilter::setRed(float value) {
    m_red = value;
    
    setFloat("redAdjustment", m_red);
}

void ArcRGBFilter::setGreen(float value) {
    m_green = value;
    
    setFloat("greenAdjustment", m_green);
}

void ArcRGBFilter::setBlue(float value) {
    m_blue = value;
    
    setFloat("blueAdjustment", m_blue);
}




