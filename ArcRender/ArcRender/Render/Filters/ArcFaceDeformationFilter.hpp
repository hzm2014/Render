//
//  ArcFaceDeformationFilter.hpp
//  ArcRender
//
//  Created by Zhiming He on 2019/1/3.
//  Copyright © 2019 zhiming he. All rights reserved.
//

#ifndef ArcFaceDeformationFilter_hpp
#define ArcFaceDeformationFilter_hpp

#include <stdio.h>
#include "ArcGLFilter.hpp"

class ArcFaceDeformationFilter: public ArcGLFilter {
    
public:
    
    ArcFaceDeformationFilter(const ArcGLSize& size);
    virtual void setOutputSize(ArcGLSize size);
    void setEyeScale(float value);
    void setFaceScale(float value);
    void setFaceContour(float* leftEye, float* rightEye, float* face, int length);
    
protected:
    
    float       m_eyeScale;
    float       m_faceScale;
    float       m_eyeRadius;              //眼睛瞳孔中心到眼角半径
    float       m_faceRadius;             //下巴到脸颊最边缘的距离
    float       m_aspectRatio;                //宽高比
    float       m_leftEyeCenterPos[2] = {0};      //左眼的中心坐标
    float       m_rightEyeCenterPos[2] = {0};     //右眼的中心坐标
    float       m_leftFaceContour[6] = {0};       //左边脸部轮廓点
    float       m_rightFaceContour[6] = {0};      //右边脸部轮廓点
    float       m_deltaArray[3] = {0};
    int         m_needFaceDeformation;    //是否需要启用瘦脸操作
    int         m_needEyeDeformation;     //是否需要启用大眼操作    
};

#endif /* ArcFaceDeformationFilter_hpp */
