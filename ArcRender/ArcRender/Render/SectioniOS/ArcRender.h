//
//  ArcRender.h
//  ArcRender
//
//  Created by Zhiming He on 2018/11/22.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <UIKit/UIKit.h>
#import "ArcGLHeader.h"

typedef void(^PixelBufferForEncode)(CVPixelBufferRef pixelBuffer);

@interface ArcRender : NSObject

@property(nonatomic, assign)AVCaptureDevicePosition cameraPosition;

@property(nonatomic, assign) BOOL mirrorFrontCamera;  // 前置摄像头视频流是否镜像显示
@property(nonatomic, assign) BOOL mirrorBackCamera;   // 后置摄像头视频流是否镜像显示
@property(nonatomic, assign) BOOL mirrorFrontPreview;       // 前置摄像头下的preview镜像显示
@property(nonatomic, assign) BOOL mirrorBackPreview;        // 后置摄像头下的preview镜像显示
@property(nonatomic, assign) UIInterfaceOrientation outputOrientation; // 输出视频方向，默认UIInterfaceOrientationPortrait
@property(nonatomic, assign) ArcGLFillMode previewFillMode;   // 预览的视频显示填充模式，默认GPUFillModePreserveAspectRatioAndFill
@property(nonatomic, assign) ArcGLFillMode outputFillMode;    // 输出视频流裁剪模式

// 输出预览和视频流旋转方向，如果设置会使视频流镜像函数失效，注意不能和镜像设置的4个参数同时使用
@property(nonatomic, assign) ArcGLRotation previewRotation;
@property(nonatomic, assign) ArcGLRotation outputRotation;

// 以下两个方法如果没有设置AVCaptureConnection则不需要调用
@property(nonatomic, assign)AVCaptureVideoOrientation   frontVideoOrientation;  // 前置摄像头输出视频流方向, 默认为AVCaptureVideoOrientationLandscapeLeft
@property(nonatomic, assign)AVCaptureVideoOrientation   backVideoOrientation;   // 后置摄像头输出视频流方向, 默认为AVCaptureVideoOrientationLandscapeRight

@property(nonatomic, assign) CGRect viewFrame;  // 预览窗口尺寸
@property(nonatomic, assign) CGSize videoSize;  // 未裁剪前视频流尺寸
@property(nonatomic, assign) CGSize outPutSize; //裁剪后输出视频流尺寸

@property(nonatomic, assign) BOOL hasEncodeVideoFrame;
@property(nonatomic, assign) BOOL enableBlackFrame; //是否黑帧显示
@property(nonatomic) CMSampleBufferRef sampleBuffer;
@property(nonatomic, assign) float brightness; //设置亮度，范围[-1, 1],默认0
@property(nonatomic, assign) float whitening; //设置美白，范围[0, 1],默认0
@property(nonatomic, assign) float smooth; //设置磨皮，范围[0, 1],默认0


- (instancetype)initWithViewFrame:(CGRect)frame cameraPosition:(AVCaptureDevicePosition)position;

// 预览背景颜色
- (void)setPreviewColor:(UIColor*)color;

- (UIView*)renderView;

- (void)receiveSampleBuffer:(CMSampleBufferRef)sampleBuffer;

- (void)setBlendImage:(UIImage*)image rect:(CGRect)rect;

- (void)setPixelBufferForEncodeCallback:(PixelBufferForEncode)pixelBufferCb;

@end



