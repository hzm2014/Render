//
//  AVCaptureDevice+Custom.m
//  ArcRender
//
//  Created by Zhiming He on 2018/11/30.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "AVCaptureDevice+Custom.h"

@implementation AVCaptureDevice (Custom)

+ (AVCaptureDevice *)deviceWithMediaType:(AVMediaType)mediaType position:(AVCaptureDevicePosition)position {
    AVCaptureDevice* device = nil;
    if (@available(iOS 10.0, *)) {
        device = [self defaultDeviceWithDeviceType:AVCaptureDeviceTypeBuiltInWideAngleCamera mediaType:mediaType position:position];
    } else {
        NSArray *devices = [AVCaptureDevice devicesWithMediaType:mediaType];
        for (AVCaptureDevice *d in devices)
        {
            if ([d position] == position)
            {
                return d;
            }
        }
    }
    return device;
}

- (BOOL)supportsHighFrameRateCapture {
    if (![self hasMediaType:AVMediaTypeVideo]) {    // 是否支持video
        return NO;
    }
    
    AVFrameRateRange *maxFrameRateRange = nil;
    
    for (AVCaptureDeviceFormat *format in self.formats) {
        // 遍历所有设备支持的formats.获取相应的codecType . 筛选出视频格式.
        FourCharCode codecType = CMVideoFormatDescriptionGetCodecType(format.formatDescription);
        if (codecType == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange) {
            
            NSArray *frameRateRanges = format.videoSupportedFrameRateRanges;
            // 遍历视频格式找到这个摄像头所提供的最高format和帧率.
            for (AVFrameRateRange *range in frameRateRanges) {
                if (range.maxFrameRate > maxFrameRateRange.maxFrameRate) {
                    maxFrameRateRange = range;
                }
            }
        }
    }
    return maxFrameRateRange.maxFrameRate > 30.0f;
}


- (BOOL)setFrameRate:(float)frameRatio {
    
    if(frameRatio <= 30) {
        [self lockForConfiguration:NULL];
        self.activeVideoMinFrameDuration = CMTimeMake(1, frameRatio);
        self.activeVideoMaxFrameDuration = CMTimeMake(1, frameRatio);
        [self unlockForConfiguration];
        return YES;
    }
    
    
    AVCaptureDeviceFormat *bestFormat = nil;
    AVFrameRateRange *bestFrameRateRange = nil;
    for ( AVCaptureDeviceFormat *format in self.formats) {
        for ( AVFrameRateRange *range in format.videoSupportedFrameRateRanges ) {
            if (range.maxFrameRate > bestFrameRateRange.maxFrameRate ) {
                bestFormat = format;
                bestFrameRateRange = range;
                NSLog(@"format:%@ FrameRateRange:%@",format , bestFormat);
            }
        }
    }
    
    if ( bestFormat ) {
        if ([self lockForConfiguration:NULL]) {
            self.activeFormat = bestFormat;
            self.activeVideoMinFrameDuration = bestFrameRateRange.minFrameDuration;
            self.activeVideoMaxFrameDuration = bestFrameRateRange.minFrameDuration;
            [self unlockForConfiguration];
        }
    }
    
    return YES;
}

- (void)configureCameraForHighestFrameRate:(AVCaptureDevice *)device
{
    AVCaptureDeviceFormat *bestFormat = nil;
    AVFrameRateRange *bestFrameRateRange = nil;
    for ( AVCaptureDeviceFormat *format in [device formats] ) {
        for ( AVFrameRateRange *range in format.videoSupportedFrameRateRanges ) {
            if ( range.maxFrameRate > bestFrameRateRange.maxFrameRate ) {
                bestFormat = format;
                bestFrameRateRange = range;
                NSLog(@"format:%@ FrameRateRange:%@",format , bestFormat);
            }
        }
    }
    if ( bestFormat ) {
        if ( [device lockForConfiguration:NULL] == YES ) {
            device.activeFormat = bestFormat;
            device.activeVideoMinFrameDuration = bestFrameRateRange.minFrameDuration;
            device.activeVideoMaxFrameDuration = bestFrameRateRange.minFrameDuration;
            [device unlockForConfiguration];
        }
    }
}

@end
