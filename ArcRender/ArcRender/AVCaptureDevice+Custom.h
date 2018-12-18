//
//  AVCaptureDevice+Custom.h
//  ArcRender
//
//  Created by Zhiming He on 2018/11/30.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface AVCaptureDevice (Custom)

+ (AVCaptureDevice *)deviceWithMediaType:(AVMediaType)mediaType position:(AVCaptureDevicePosition)position;
- (BOOL)setFrameRate:(float)frameRatio;

@end

NS_ASSUME_NONNULL_END
