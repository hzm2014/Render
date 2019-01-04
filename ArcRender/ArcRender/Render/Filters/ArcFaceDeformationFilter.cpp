//
//  ArcFaceDeformationFilter.cpp
//  ArcRender
//
//  Created by Zhiming He on 2019/1/3.
//  Copyright © 2019 zhiming he. All rights reserved.
//

#include "ArcFaceDeformationFilter.hpp"
#include <cmath>

static const char* k_FaceDeformationFragment = SHADER300_STRING (
                                                                 
    in vec2 textureCoordinate;
    uniform sampler2D inputImageTexture[1];
    out vec4 out_color;

    uniform mediump float eyeScaleRatio;
    uniform mediump float radius;
    uniform mediump vec2 leftEyePos;
    uniform mediump vec2 rightEyePos;
    uniform mediump float aspectRatio;
    uniform mediump float radius2;
    uniform mediump float leftContour[16];
    uniform mediump float rightContour[16];
    uniform mediump float deltaArray[8];
    uniform int arraySize;
    uniform int needFaceDeformation;
    uniform int needEyeDeformation;

    vec2 warpPositionToUse(vec2 centerPostion, vec2 currentPosition, float radius, float eyeScaleRatio, float aspectRatio) {
     vec2 positionToUse = currentPosition;
     vec2 currentPositionToUse = vec2(currentPosition.x, currentPosition.y * aspectRatio + 0.5 - 0.5 * aspectRatio);
     vec2 centerPostionToUse = vec2(centerPostion.x, centerPostion.y * aspectRatio + 0.5 - 0.5 * aspectRatio);
     float r = distance(currentPositionToUse, centerPostionToUse);
     if(r < radius) {
         float alpha = 1.0 - eyeScaleRatio * (r/radius-1.0)*(r/radius-1.0);
         positionToUse = centerPostion + alpha * (currentPosition - centerPostion);
     }
     return positionToUse;
    }

    vec2 warpPositionToUse2(vec2 currentPoint, vec2 contourPointA,  vec2 contourPointB, float radius, float delta, float aspectRatio) {
     vec2 positionToUse = currentPoint;
     vec2 currentPointToUse = vec2(currentPoint.x, currentPoint.y * aspectRatio + 0.5 - 0.5 * aspectRatio);
     vec2 contourPointAToUse = vec2(contourPointA.x, contourPointA.y * aspectRatio + 0.5 - 0.5 * aspectRatio);
     float r = distance(currentPointToUse, contourPointAToUse);
     
     if(r < radius) {
         vec2 dir = normalize(contourPointB - contourPointA);
         float dist = radius * radius - r * r;
         float alpha = dist / (dist + (r-delta) * (r-delta));
         alpha = alpha * alpha;
         positionToUse = positionToUse - alpha * delta * dir;
     }
     return positionToUse;
    }
    void main() {
     
     vec2 positionToUse = textureCoordinate;
     if(needFaceDeformation==1){
         for(int i=0;i<arraySize;i++){
             positionToUse = warpPositionToUse2(positionToUse, vec2(leftContour[i * 2], leftContour[i * 2 + 1]), vec2(rightContour[i * 2], rightContour[i * 2 + 1]), radius2, deltaArray[i], aspectRatio);
             positionToUse = warpPositionToUse2(positionToUse, vec2(rightContour[i * 2], rightContour[i * 2 + 1]), vec2(leftContour[i * 2], leftContour[i * 2 + 1]), radius2, deltaArray[i], aspectRatio);
         }
     }
     
     if(needEyeDeformation==1){
         positionToUse = warpPositionToUse(leftEyePos, positionToUse, radius, eyeScaleRatio, aspectRatio);
         positionToUse = warpPositionToUse(rightEyePos, positionToUse, radius, eyeScaleRatio, aspectRatio);
     }
     
     out_color = texture(inputImageTexture[0], positionToUse);
    }
);

static string vertexShader = ArcGLFilter::g_vertext300_shader;
static string fragmentShader = k_FaceDeformationFragment;

ArcFaceDeformationFilter::ArcFaceDeformationFilter(const ArcGLSize& size): ArcGLFilter(vertexShader, fragmentShader) {
    m_name = "ArcFaceDeformationFilter";
    setInputs(1);
    
    m_eyeScale = 0.0f;
    m_faceScale = 0.0f;
    m_eyeRadius = 0.08f; //眼睛瞳孔中心到眼角半径
    m_faceRadius = 0.01f; //下巴到脸颊最边缘的距离
    m_aspectRatio = float(size.height*1.0/size.width); //宽高比
    m_needFaceDeformation = 0; //是否需要启用瘦脸操作
    m_needEyeDeformation = 0;  //是否需要启用大眼操作
    
    setFloat("aspectratio", m_aspectRatio);
    setFloat("eyescale", 0.0);
    setFloat("facescale", 0.0);
    setInteger("needEyeDeformation", m_needEyeDeformation);
}

void ArcFaceDeformationFilter::setOutputSize(ArcGLSize size) {
    ArcGLFilter::setOutputSize(size);
    
    m_aspectRatio = float(size.height*1.0/size.width);
}

void ArcFaceDeformationFilter::setEyeScale(float value) {
    m_eyeScale = value;
    
    if(value == 0.0) {
        m_needEyeDeformation = 0;
    } else {
        m_needEyeDeformation = 1;
    }
    setFloat("eyescale", m_eyeScale);
    setInteger("needEyeDeformation", m_needEyeDeformation);
}

void ArcFaceDeformationFilter::setFaceScale(float value) {
    m_faceScale = value;
    
    if(value == 0.0) {
        m_needFaceDeformation = 0;
    } else {
        m_needFaceDeformation = 1;
    }
    setFloat("facescale", m_faceScale);
    setInteger("needEyeDeformation", m_needFaceDeformation);
}

float distance(float x1, float y1, float x2, float y2) {
    return sqrtf((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

void ArcFaceDeformationFilter::setFaceContour(float *leftEye, float *rightEye, float *face, int length) {
    if (m_needEyeDeformation == 1) {
        m_leftEyeCenterPos[0] = leftEye[0];
        m_leftEyeCenterPos[1] = leftEye[1];
        m_rightEyeCenterPos[0] = rightEye[0];
        m_rightEyeCenterPos[1] = rightEye[1];
        
        //现在是取中心-外眼角的距离
        m_eyeRadius = fmax(distance(leftEye[0], leftEye[1], leftEye[14], leftEye[15]),
                         distance(rightEye[0], rightEye[1], rightEye[14], rightEye[15]));
        
        setFloat("radius", m_eyeRadius);
        setFloatv("leftEyePos", m_leftEyeCenterPos, 2);
        setFloatv("rightEyePos", m_rightEyeCenterPos, 2);
    }
    
    if (m_needFaceDeformation == 1) {
        int fpc = length/2;
        //不确定含义，先随便算一个
        m_faceRadius = distance(face[(fpc-1)*2], face[(fpc-1)*2+1], face[0], face[1])/2;
        
        for (int i = 0; i < 3; i++) {
            m_leftFaceContour[i*2] = face[(fpc-1-i)*2];
            m_leftFaceContour[i*2+1] = face[(fpc-1-i)*2+1];
            m_rightFaceContour[i*2] = face[i*2];
            m_rightFaceContour[i*2+1] = face[i*2+1];
            //不确定含义，随便算一个
            m_deltaArray[i] = 0.1f*m_faceScale*distance(face[(fpc-1-i)*2],
                                                    face[(fpc-1-i)*2+1], face[i*2], face[i*2+1])/2;
        }
        
        setFloat("radius2", m_faceRadius);
        setFloatv("leftContour", m_leftFaceContour, 6);
        setFloatv("rightContour", m_rightFaceContour, 6);
        setFloatv("deltaArray", m_deltaArray, 3);
        setInteger("arraySize", 3);
    }
}


