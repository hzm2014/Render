//
//  ViewController.m
//  ArcRender
//
//  Created by zhiming he on 2018/11/15.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ViewController.h"
#import "ArcRender.h"
#import "ArcVideoProvider.h"

@interface ViewController ()<ArcVideoProviderDelegate>
{
    ArcRender* mRender;
    ArcVideoProvider* mVideoProvider;
    AVCaptureDevicePosition mPosition;
    UIImage* image;
    UIImageView* videoView;
    UIButton* mSwitchCameraBtn;
    UIButton* mSwitchBlackFrameBtn;
    UIButton* mShowOrHideLogoBtn;
}
@end

@implementation ViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged:) name:UIApplicationDidChangeStatusBarOrientationNotification object:nil];
    [self setRender];
    [self setVideoProvider];
    [self setupSwitchCameraBtn];
    [self setupSwitchBlackFrameBtn];
    [self setupShowOrHideLogoBtn];
}

- (void)setRender {
    mPosition = AVCaptureDevicePositionFront;
    mRender = [[ArcRender alloc] initWithViewFrame:self.view.frame];
    mRender.outPutSize = CGSizeMake(720, 1280);
    mRender.outputRotation = ArcGLRotateRight;
    mRender.mirrorForOutput = YES;
    mRender.hasEncodeVideoFrame = YES;
    image = [UIImage imageNamed:@"for_test.png"];
    [mRender setBlendImage:image rect:CGRectMake(20, 160, 120, 120)];
    mRender.enableBeauty = YES;
    
    [self.view addSubview:[mRender renderView]];
    
//    videoView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 150, 135, 240)];
//    [videoView setBackgroundColor:[UIColor whiteColor]];
//    [self.view addSubview:videoView];

//    __weak __typeof(self) weakSelf = self;
//    __weak __typeof(videoView) weakVideoView = videoView;
    [mRender setPixelBufferForEncodeCallback:^(CVPixelBufferRef pixelBuffer) {
//        UIImage* image = [weakSelf pixelBuffer2Image:pixelBuffer];
//        dispatch_async(dispatch_get_main_queue(), ^(){
//            [weakVideoView setImage:image];
//        });
    }];
}


- (void)setVideoProvider {
    mVideoProvider = [[ArcVideoProvider alloc] initWithSessionPreset:AVCaptureSessionPreset1280x720 position:mPosition];
    mVideoProvider.delegate = self;
    mVideoProvider.mFrameRate = 30;
    [mVideoProvider startRunning];
}

- (void)willOutputSampleBuffer:(CMSampleBufferRef)sampleBufferRef {
    [mRender receiveSampleBuffer:sampleBufferRef];
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator {
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    
    
}

- (void)orientationChanged:(NSNotification*)nofication{
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    [mRender setViewFrame:self.view.bounds];
    ArcGLRotation rotation = [self calRotation:orientation];
    
    mRender.outputRotation = rotation;
    CGSize size = CGSizeMake(720, 1280);
    if(UIInterfaceOrientationIsPortrait(orientation) == NO) {
        size = CGSizeMake(1280, 720);
    }
    mRender.outPutSize = size;
}

// 根据设备方向和视频帧方向计算旋转角度
- (ArcGLRotation)calRotation:(UIInterfaceOrientation)interfaceOrientation{
    
    int rotateAngle = 0;
    switch(interfaceOrientation)
    {
        case UIInterfaceOrientationPortraitUpsideDown:
            rotateAngle = 180;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            rotateAngle = 90;
            break;
        case UIInterfaceOrientationLandscapeRight:
            rotateAngle = -90;
            break;
        case UIInterfaceOrientationPortrait:
        default:
            rotateAngle = 0;
    }
    
    AVCaptureVideoOrientation orientation = AVCaptureVideoOrientationPortrait;
    if (mPosition == AVCaptureDevicePositionFront) {
        orientation = AVCaptureVideoOrientationLandscapeLeft;
    } else {
        orientation = AVCaptureVideoOrientationLandscapeRight;
    }
    
    switch(orientation)
    {
        case AVCaptureVideoOrientationPortraitUpsideDown:
            rotateAngle += 180;
            break;
        case AVCaptureVideoOrientationLandscapeLeft:
            rotateAngle -= 90;
            break;
        case AVCaptureVideoOrientationLandscapeRight:
            rotateAngle += 90;
            break;
        case AVCaptureVideoOrientationPortrait:
        default:
            rotateAngle += 0;
            break;
    }
    
    ArcGLRotation rotation = ArcGLNoRotation;
    if (mPosition==AVCaptureDevicePositionBack) {
        switch(rotateAngle)
        {
            case 0:
            case 360:
                rotation = ArcGLNoRotation; break;
            case 90:
                rotation = ArcGLRotateRight; break;
            case -90:
                rotation = ArcGLRotateLeft; break;
            case 180:
            case -180:
                rotation = ArcGLRotate180; break;
            default:rotation = ArcGLNoRotation;
        }
    } else {
        switch(rotateAngle)
        {
            case 0:
            case 360:
                rotation = ArcGLNoRotation; break;
            case 90:
                rotation = ArcGLRotateLeft; break;
            case -90:
                rotation = ArcGLRotateRight; break;
            case 180:
            case -180:
                rotation = ArcGLRotate180; break;
            default:rotation = ArcGLNoRotation;
        }
    }
    return rotation;
}

- (void)setupSwitchCameraBtn {
    mSwitchCameraBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    mSwitchCameraBtn.frame = CGRectMake(self.view.frame.size.width - 80, 90, 60, 30);
    [self.view addSubview:mSwitchCameraBtn];
    mSwitchCameraBtn.backgroundColor = [UIColor orangeColor];
    [mSwitchCameraBtn setTitle:@"Switch1" forState:UIControlStateNormal];
    mSwitchCameraBtn.titleLabel.font = [UIFont systemFontOfSize:12.0];
    [mSwitchCameraBtn addTarget:self action:@selector(switchCamera) forControlEvents:UIControlEventTouchUpInside];
}

- (void)switchCamera {
    [mVideoProvider switchCamera];
    mPosition = mPosition == AVCaptureDevicePositionFront?AVCaptureDevicePositionBack:AVCaptureDevicePositionFront;
    
    if(mPosition == AVCaptureDevicePositionFront) {
        mRender.mirrorForOutput = YES;
    } else {
        mRender.mirrorForOutput = NO;
    }
}

- (void)setupSwitchBlackFrameBtn {
    mSwitchBlackFrameBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    mSwitchBlackFrameBtn.frame = CGRectMake(self.view.frame.size.width - 80, CGRectGetMaxY(mSwitchCameraBtn.frame) + 10, 60, 30);
    [self.view addSubview:mSwitchBlackFrameBtn];
    mSwitchBlackFrameBtn.backgroundColor = [UIColor orangeColor];
    [mSwitchBlackFrameBtn setTitle:@"Switch2" forState:UIControlStateNormal];
    mSwitchBlackFrameBtn.titleLabel.font = [UIFont systemFontOfSize:12.0];
    [mSwitchBlackFrameBtn addTarget:self action:@selector(switchblackFrame) forControlEvents:UIControlEventTouchUpInside];
}

- (void)switchblackFrame {
    static BOOL enableBlackFrame = NO;
    enableBlackFrame = !enableBlackFrame;
    mRender.enableBlackFrame = enableBlackFrame;
}

- (void)setupShowOrHideLogoBtn {
    mShowOrHideLogoBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    mShowOrHideLogoBtn.frame = CGRectMake(self.view.frame.size.width - 80, CGRectGetMaxY(mSwitchBlackFrameBtn.frame) + 10, 60, 30);
    [self.view addSubview:mShowOrHideLogoBtn];
    mShowOrHideLogoBtn.backgroundColor = [UIColor orangeColor];
    [mShowOrHideLogoBtn setTitle:@"Switch3" forState:UIControlStateNormal];
    mShowOrHideLogoBtn.titleLabel.font = [UIFont systemFontOfSize:12.0];
    [mShowOrHideLogoBtn addTarget:self action:@selector(showOrHideLogo) forControlEvents:UIControlEventTouchUpInside];
}

- (void)showOrHideLogo {
    static BOOL flag = YES;
    flag = !flag;
    if(flag) {
        [mRender setBlendImage:image rect:CGRectMake(20, 160, 120, 120)];
    }else {
        [mRender setBlendImage:nil rect:CGRectZero];
    }
}

-(UIImage*) pixelBuffer2Image:(CVPixelBufferRef) pixelBuffer;{
    CIImage *coreImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
    
    CIContext *context = [CIContext contextWithOptions:[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] forKey:kCIContextUseSoftwareRenderer]];//CPU渲染
    CGImageRef cgimg = [context createCGImage:coreImage fromRect:[coreImage extent]];
    UIImage* image = [UIImage imageWithCGImage:cgimg];
    CFRelease(cgimg);
    return image;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end
