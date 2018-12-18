//
//  ArcGLHeader.h
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#ifndef ArcGLHeader_h
#define ArcGLHeader_h

#include <stdio.h>

#ifdef __ANDROID__
#include <jni.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <EGL/egl.h>
#include <android/log.h>

typedef struct GPU_Context {
    EGLDisplay            eglDisplay;
    EGLSurface            eglSurface;
    EGLContext            eglContext;
} GPU_Context;

#else

#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#endif

// 裁剪方式
typedef enum{
    ArcGLFillModeStretch,                       // 完全匹配，直接拉伸
    ArcGLFillModePreserveAspectRatio,           // 适配输出尺寸，可能有边框
    ArcGLFillModePreserveAspectRatioAndFill     // 按照输出比例裁剪，不保留边框
}ArcGLFillMode;

// 视频帧格式
typedef enum{
    ArcGLPixelFormatUNkNOW = 0,
    ArcGLPixelFormatRGBA = 1,
    ArcGLPixelFormatNV21 = 2,
    ArcGLPixelFormatI420 = 3,
    ArcGLPixelFormatYV12 = 4,
    ArcGLPixelFormatYUV444 = 5,
    ArcGLPixelFormatNV12 = 6,
    ArcGLPixelFormatBGRA = 7,
    ArcGLPixelFormatGRAY = 8,
}ArcGLPixelFormat;

// texture类别，GPU_TEXTURE_OES只能用于安卓
typedef enum{
    ArcGLTextureTypeRGBA = 0,   // 默认纹理，属于非规范化纹理
    ArcGLTextureTypeOES = 1,    // 用于安卓oes类型texutre，实际是nv21格式
    ArcGLTextureTypeUNNORMAL,   // 非规范化纹理
    /* 以下是gles3.0中才有 */
    ArcGLTextureTypeNORMAL,     // 规范化纹理
    ArcGLTextureTypeFLOAT,      // 浮点纹理
    ArcGLTextureTypeINTEGER,    // 整数纹理
    ArcGLTextureTypeSHARED,     // 共享指数纹理
    ArcGLTextureTypeDEPTH,      // 深度纹理
    ArcGLTextureTypeSRGB        // sRGB纹理
}ArcGLTextureType;

// 旋转方向，一般用于指定人脸方向，均为顺时针旋转
typedef enum{
    ArcGLOrientationClockwise_0,
    ArcGLOrientationClockwise_90,
    ArcGLOrientationClockwise_180,
    ArcGLOrientationClockwise_270,
}ArcGLOrientation;

typedef enum{
    ArcGLCameraPositionFront,
    ArcGLCameraPositionBack
}ArcGLCameraPosition;

// 指定输出texture的旋转方向
typedef enum{
    ArcGLNoRotation = 0,
    ArcGLRotateLeft = 1,
    ArcGLRotateRight = 2,
    ArcGLFlipVertical = 3,
    ArcGLFlipHorizonal = 4,
    ArcGLRotateRightFlipVertical = 5,
    ArcGLRotateRightFlipHorizontal = 6,
    ArcGLRotate180 = 7,
}ArcGLRotation;

typedef struct ArcGLSize{
    unsigned    width;
    unsigned    height;
}ArcGLSize;

typedef struct ArcGLBorder {
    int left;    ///< 矩形最左边的坐标
    int top;    ///< 矩形最上边的坐标
    int right;    ///< 矩形最右边的坐标
    int bottom;    ///< 矩形最下边的坐标
} ArcGLBorder;

typedef struct ArcGLPoint{
    float x;
    float y;
}ArcGLPoint;

typedef struct ArcGLColorI{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
}ArcGLColorI;

typedef struct ArcGLColorF{
    float r;
    float g;
    float b;
    float a;
}ArcGLColorF;

typedef struct ArcGLRect {
    ArcGLPoint point;
    ArcGLSize size;
}ArcGLRect;

#define    STRINGIZE(x)    #x
#define SHADER300_STRING(text)   "#version 300 es\nprecision highp float;precision highp int;" STRINGIZE(text)

#define ArcRotationSwapsWidthAndHeight(rotation) ((rotation) == ArcGLRotateLeft || (rotation) == ArcGLRotateRight || (rotation) == ArcGLRotateRightFlipVertical || (rotation) == ArcGLRotateRightFlipHorizontal)

#endif /* ArcGLHeader_h */
