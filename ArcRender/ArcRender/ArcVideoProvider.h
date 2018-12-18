//
//  ArcVideoProvider.h
//  ArcCamera
//
//  Created by Zhiming He on 2018/8/6.
//  Copyright © 2018年 Zhiming He. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ArcVideoProviderDelegate <NSObject>

- (void)willOutputSampleBuffer:(CMSampleBufferRef)sampleBufferRef;

@end

@interface ArcVideoProvider : NSObject

@property (nonatomic, weak) id <ArcVideoProviderDelegate> delegate;
@property (nonatomic, readwrite) int32_t mFrameRate; //设置帧率

- (instancetype)initWithSessionPreset:(NSString *)sessionPreset position:(AVCaptureDevicePosition)position;
- (void)startRunning;
- (void)switchCamera;

@end

NS_ASSUME_NONNULL_END
