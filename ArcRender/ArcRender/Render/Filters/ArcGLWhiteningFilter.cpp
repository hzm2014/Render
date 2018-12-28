//
//  ArcGLWhiteningFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2018/12/21.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#include "ArcGLWhiteningFilter.hpp"


const static char* K_Whitening_Fragment = SHADER300_STRING (
    in mediump vec2 textureCoordinate;

    uniform sampler2D inputImageTexture[1];
    uniform mediump vec4 threshold;
    uniform mediump vec4 slope;        

    uniform mediump mat3 rgb2yuv;
    uniform mediump vec3 off2yuv;
    const mediump mat3 YCbCrtoRGB = mat3(1.0, 1.0, 1.0, 0.0, -0.343, 1.765, 1.4, -0.711, 0.0);
                                                            
    out vec4 out_color;

    void main() {
        mediump vec4 color = texture(inputImageTexture[0], textureCoordinate);
        mediump vec3 yuv = rgb2yuv*color.rgb+off2yuv;
        
        mediump float luma = yuv.r;
        mediump float u = yuv.g;
        mediump float v = yuv.b;
        
        mediump float l0 =   min(luma, threshold.x);
        mediump float l1 = clamp(luma, threshold.x, threshold.y)-threshold.x;
        mediump float l2 = clamp(luma, threshold.y, threshold.z)-threshold.y;
        mediump float l3 = clamp(luma, threshold.z, threshold.w)-threshold.z;
        mediump float l4 =   max(luma, threshold.w)             -threshold.w;
        luma = l0+l1*slope.y+l2*slope.z+l3*slope.w+l4;
        
        yuv = vec3(luma, u, v);
        out_color = vec4(YCbCrtoRGB * (yuv-vec3(0.0, 0.5, 0.5)), color.a);
    }
);

static string vertexShader = ArcGLFilter::g_vertext300_shader;
static string fragmentShader = K_Whitening_Fragment;


ArcGLWhiteningFilter::ArcGLWhiteningFilter(float s): ArcGLFilter(vertexShader, fragmentShader) {
    m_name = "ArcGLWhiteningFilter";
    setInputs(1);
    m_strength = s;
    
    float ideal[6] = {0.2, 0.6, 0.7, 0.8, 0.5, 0.6};
    setIdeal(ideal);
   
    GLfloat mtx[] = {0.2984, -0.1690, 0.5012, 0.5875, -0.3328, -0.4196, 0.1142, 0.5019, -0.0815},
    off[] = {0.0, 0.5, 0.5};
    memcpy(&m_rgb2yuv, mtx, sizeof(GLfloat)*9);
    memcpy(&m_off2yuv, off, sizeof(GLfloat)*3);
    
    setMatrix("rgb2yuv", m_rgb2yuv, 3);
    setFloat("off2yuv", m_off2yuv, 3);
}

void ArcGLWhiteningFilter::setIdeal(float ideal[6]) {
    memcpy(m_idealpnts, ideal, 6*sizeof(float));
    m_idealpnts[6] = ideal[4];
    m_idealpnts[7] = ideal[5];
    m_threshold[0] = ideal[0];
    m_threshold[1] = ideal[4];
    m_threshold[2] = ideal[5];
    m_threshold[3] = ideal[3];
    setStrength(m_strength);
}
void ArcGLWhiteningFilter::setThreshold(float thres[4]){
    const float D = .01;
    float s, min, max;
    
    s = m_strength*.2;
    min = m_idealpnts[0]*s+(1-s)*m_idealpnts[4];
    s = m_strength*.3;
    max = m_idealpnts[5]*s+(1-s)*m_idealpnts[4];
    if(thres[1]-D > m_idealpnts[6])
        s = m_threshold[1]+D;
    else if(thres[1]+D < m_idealpnts[6])
        s = m_threshold[1]-D;
    m_threshold[1] = SATURATE(s, min, max);
    
    s = m_strength*.3;
    min = m_idealpnts[4]*s+(1-s)*m_idealpnts[5];
    s = m_strength*.5;
    max = m_idealpnts[3]*s+(1-s)*m_idealpnts[5];
    if(thres[2]-D > m_idealpnts[7])
        s = m_threshold[2]+D;
    else if(thres[2]+D < m_idealpnts[7])
        s = m_threshold[2]-D;
    m_threshold[2] = SATURATE(s, min, max);
    
    setStrength(m_strength);
}

void ArcGLWhiteningFilter::setStrength(float s){
    
    if(s < 0)
        s = m_strength;
    m_strength = s;
    float t = 1-s;
    m_idealpnts[6] = m_idealpnts[1]*s+t*m_idealpnts[4];
    m_idealpnts[7] = m_idealpnts[2]*s+t*m_idealpnts[5];
    
    float s0 = 1;
    float s1 = (m_idealpnts[6]-m_idealpnts[0])/(m_threshold[1]-m_threshold[0]);
    float s2 = (m_idealpnts[7]-m_idealpnts[6])/(m_threshold[2]-m_threshold[1]);
    float s3 = (m_idealpnts[3]-m_idealpnts[7])/(m_threshold[3]-m_threshold[2]);
    
    GLfloat slope[] = {s0,s1,s2,s3};
    setFloat("slope", slope, 4);
    setFloat("threshold", m_threshold, 4);
}

