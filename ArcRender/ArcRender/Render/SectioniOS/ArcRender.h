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

@property(nonatomic, assign) BOOL mirrorForPreview;       // 预览是否镜像
@property(nonatomic, assign) BOOL mirrorForOutput;        // 输出是否镜像

@property(nonatomic, assign) ArcGLFillMode previewFillMode;   // 预览的视频显示填充模式，默认ArcGLFillModePreserveAspectRatioAndFill
@property(nonatomic, assign) ArcGLFillMode outputFillMode;    // 输出视频流裁剪模式

// 输出预览和视频流旋转方向，如果设置会使视频流镜像函数失效，注意不能和镜像设置的4个参数同时使用
@property(nonatomic, assign) ArcGLRotation previewRotation;
@property(nonatomic, assign) ArcGLRotation outputRotation;

@property(nonatomic, assign) CGRect viewFrame;  // 预览窗口尺寸
@property(nonatomic, assign) CGSize outPutSize; //裁剪后输出视频流尺寸

@property(nonatomic, assign) BOOL hasEncodeVideoFrame;
@property(nonatomic, assign) BOOL enableBlackFrame; //是否黑帧显示
@property(nonatomic) CMSampleBufferRef sampleBuffer;
@property(nonatomic, assign) float brightness; //设置亮度，范围[-1, 1],默认0
@property(nonatomic, assign) float whitening; //设置美白，范围[0, 1],默认0
@property(nonatomic, assign) float smooth; //设置磨皮，范围[0, 1],默认0
@property(nonatomic, assign) BOOL enableBeauty; //开启一键美颜


- (instancetype)initWithViewFrame:(CGRect)frame;

// 预览背景颜色
- (void)setPreviewColor:(UIColor*)color;

- (UIView*)renderView;

- (void)receiveSampleBuffer:(CMSampleBufferRef)sampleBuffer;

- (void)setBlendImage:(UIImage*)image rect:(CGRect)rect;

- (void)setPixelBufferForEncodeCallback:(PixelBufferForEncode)pixelBufferCb;

@end



