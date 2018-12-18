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
    UIInterfaceOrientation old_orientation;
    UIImageView* videoView;
    UIButton* mSwitchCameraBtn;
}
@end

@implementation ViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    old_orientation = UIInterfaceOrientationUnknown;
    // 屏幕旋转
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged:) name:UIDeviceOrientationDidChangeNotification object:nil];
    [self setRender];
    [self setVideoProvider];
    [self setupSwitchCameraBtn];
}

- (void)setRender {
    mPosition = AVCaptureDevicePositionFront;
    mRender = [[ArcRender alloc] initWithViewFrame:self.view.frame cameraPosition:mPosition];
    mRender.outputOrientation = UIInterfaceOrientationPortrait;
    mRender.outPutSize = CGSizeMake(480, 640);
    mRender.mirrorFrontPreview = YES;
    image = [UIImage imageNamed:@"for_test.png"];
    [mRender setBlendImage:image rect:CGRectMake(20, 160, 120, 120)];
    [self.view addSubview:[mRender renderView]];
    
//    videoView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 150, 135, 240)];
//    [videoView setBackgroundColor:[UIColor whiteColor]];
//    [self.view addSubview:videoView];
    
//    __weak __typeof(self) weakSelf = self;
    [mRender setPixelBufferForEncodeCallback:^(CVPixelBufferRef pixelBuffer) {
//        UIImage* image = [weakSelf pixelBuffer2Image:pixelBuffer];
//        dispatch_async(dispatch_get_main_queue(), ^(){
//            [self->videoView setImage:image];
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
    if (old_orientation == orientation) {
        return;
    }
    
    [mRender setViewFrame:self.view.bounds];
    NSLog(@"setViewFrame %@", NSStringFromCGRect(self.view.bounds));
    old_orientation = orientation;
    
    switch (orientation) {
        case UIInterfaceOrientationPortraitUpsideDown:
            [mRender setOutputOrientation:UIInterfaceOrientationPortraitUpsideDown];
            break;
        case UIInterfaceOrientationLandscapeRight:
            [mRender setOutputOrientation:UIInterfaceOrientationLandscapeRight];
            break;
        case UIInterfaceOrientationLandscapeLeft:
            [mRender setOutputOrientation:UIInterfaceOrientationLandscapeLeft];
            break;
        case UIInterfaceOrientationPortrait:
            [mRender setOutputOrientation:UIInterfaceOrientationPortrait];
            break;
        default:
            break;
    }
}

- (void)setupSwitchCameraBtn {
    mSwitchCameraBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    mSwitchCameraBtn.frame = CGRectMake(self.view.frame.size.width - 80, 30, 60, 30);
    [self.view addSubview:mSwitchCameraBtn];
    mSwitchCameraBtn.backgroundColor = [UIColor orangeColor];
    [mSwitchCameraBtn setTitle:@"Switch" forState:UIControlStateNormal];
    [mSwitchCameraBtn addTarget:self action:@selector(switchCamera) forControlEvents:UIControlEventTouchUpInside];
}

- (void)switchCamera {
    [mVideoProvider switchCamera];
    mPosition = mPosition == AVCaptureDevicePositionFront?AVCaptureDevicePositionBack:AVCaptureDevicePositionFront;
    mRender.cameraPosition = mPosition;
}

-(UIImage*) pixelBuffer2Image:(CVPixelBufferRef) pixelBuffer;{
    CIImage *coreImage = [CIImage imageWithCVPixelBuffer:pixelBuffer];
    
    CIContext *context = [CIContext contextWithOptions:[NSDictionary dictionaryWithObject:[NSNumber numberWithBool:YES] forKey:kCIContextUseSoftwareRenderer]];//CPU渲染
    CGImageRef cgimg = [context createCGImage:coreImage fromRect:[coreImage extent]];
    UIImage* image = [UIImage imageWithCGImage:cgimg];
    CFRelease(cgimg);
    return image;
}


@end
