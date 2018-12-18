//
//  ArcVideoProvider.m
//  ArcCamera
//
//  Created by Zhiming He on 2018/8/6.
//  Copyright © 2018年 Zhiming He. All rights reserved.
//

#import "ArcVideoProvider.h"
#import "ArcRunProcess.h"
#import "AVCaptureDevice+Custom.h"

@interface ArcVideoProvider () <AVCaptureVideoDataOutputSampleBufferDelegate>
{
    AVCaptureSession * mCaptureSession;
    AVCaptureDevice  * mInputDevice;
    AVCaptureDeviceInput * mDeviceInput;
    NSString * mSessionPreset;
    AVCaptureDevicePosition mCameraPosition;
}
@end

@implementation ArcVideoProvider

- (instancetype)initWithSessionPreset:(NSString *)sessionPreset position:(AVCaptureDevicePosition)position {
    if(self = [super init]) {
        mSessionPreset = sessionPreset;
        mCameraPosition = position;
        [self setupCaptureSession];
    }
    return self;
}

- (void)setupInputDevice {
    
    mInputDevice = [AVCaptureDevice deviceWithMediaType:AVMediaTypeVideo position:mCameraPosition];
    
    mDeviceInput = [[AVCaptureDeviceInput alloc] initWithDevice:mInputDevice error:nil];
    if ([mCaptureSession canAddInput:mDeviceInput]) {
        [mCaptureSession addInput:mDeviceInput];
    }
}

- (void)setupOutputDevice {
    AVCaptureVideoDataOutput * outputDevice = [[AVCaptureVideoDataOutput alloc] init];
    outputDevice.alwaysDiscardsLateVideoFrames = NO;

    [outputDevice setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange] forKey:(id)kCVPixelBufferPixelFormatTypeKey]];
    [outputDevice setSampleBufferDelegate:self queue:dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0)];
    if ([mCaptureSession canAddOutput:outputDevice]) {
        [mCaptureSession addOutput:outputDevice];
    }
}

- (void)processMirror {
    AVCaptureConnection *connection = [[mCaptureSession.outputs firstObject] connectionWithMediaType:AVMediaTypeVideo];
    if (mInputDevice.position == AVCaptureDevicePositionBack) {
        connection.videoMirrored = NO;
    }else {
        connection.videoMirrored = YES;
    }
}

- (void)setupCaptureSession {
    mCaptureSession = [[AVCaptureSession alloc] init];
    
    [mCaptureSession beginConfiguration];
    mCaptureSession.sessionPreset = mSessionPreset;
    
    [self setupInputDevice];
    [self setupOutputDevice];
    
    [mCaptureSession commitConfiguration];
}

- (void)startRunning {
//    需要注意的是startRunning函数会花费一定的时间，所以不能在主线程（UI线程）调用，防止卡顿。
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        [self->mCaptureSession startRunning];
    });
}

- (void)setMFrameRate:(int32_t)mFrameRate {
    if(mFrameRate <= 0) {
        NSLog(@"setMFrameRate Error:(%d)", mFrameRate);
    }
    _mFrameRate = mFrameRate;
    [mCaptureSession beginConfiguration];
    [mInputDevice setFrameRate:mFrameRate];
    [mCaptureSession commitConfiguration];
}

- (void)switchCamera {
    
    if (!mCaptureSession.isRunning) {
        return;
    }
    
    
    AVCaptureDevicePosition position = mInputDevice.position == AVCaptureDevicePositionFront?AVCaptureDevicePositionBack:AVCaptureDevicePositionFront;
    
    AVCaptureDevice *newCamera = [AVCaptureDevice deviceWithMediaType:AVMediaTypeVideo position:position];
    AVCaptureDeviceInput *newInput = [AVCaptureDeviceInput deviceInputWithDevice:newCamera error:nil];
    
    [mCaptureSession beginConfiguration];
    
    [mCaptureSession removeInput:mDeviceInput];
    if([mCaptureSession canAddInput:newInput]) {
        [mCaptureSession addInput:newInput];
        mInputDevice = newCamera;
        mDeviceInput = newInput;
    } else {
        [mCaptureSession addInput:mDeviceInput];
    }
    
    [mCaptureSession commitConfiguration];
}

#pragma mark - AVCaptureVideoDataOutputSampleBufferDelegate
- (void)captureOutput:(AVCaptureOutput *)output didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    if (!mCaptureSession.isRunning) {
        return;
    }
    
    if([self.delegate respondsToSelector:@selector(willOutputSampleBuffer:)]) {
        [self.delegate willOutputSampleBuffer:sampleBuffer];
    }
    
}

- (void)dealloc {
   
}

@end
