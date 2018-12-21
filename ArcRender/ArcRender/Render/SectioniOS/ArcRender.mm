//
//  ArcRender.m
//  ArcRender
//
//  Created by Zhiming He on 2018/11/22.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArcRender.h"
#import "ArciOSContext.h"
#import "ArcRunProcess.h"
#import "ArcSampleBufferFilter.h"
#import "ArcRenderView.h"
#include "ArciOSGLImage.h"
#import "ArcBlendImageFilter.hpp"
#import "ArcBlendForEncodeFilter.hpp"
#import "ArcGLBrightnessFilter.hpp"
#import <list>


@interface ArcRender ()
{
    __unsafe_unretained ArcRunProcess* mRunProcess;
    dispatch_semaphore_t mRenderSemaphore;
    ArcSampleBufferFilter* mSampleBufferFilter;
    ArcRenderView* mRenderView;
    CMTime mCurrentFrameTimeStamp; //当前帧的时间戳
    CGSize mOriginalSize;
    int    mRotateAngle;
    CGRect mBlendImageRect;
    ArcGLImage* mBlendImage;
    ArcBlendImageFilter* mBlendImageFilter;
    ArcBlendForEncodeFilter* mBlendForEncodeFilter;
    ArcGLBrightnessFilter* mBrightnessFilter;
    
    list<ArcGLFilter*> mFilters; //不包含SampleBufferFilter和renderView
    BOOL mReady;
}

@property (nonatomic, copy) PixelBufferForEncode mPixelBufferBlock;

@end

@implementation ArcRender

- (instancetype)initWithViewFrame:(CGRect)frame cameraPosition:(AVCaptureDevicePosition)position {
    if(self = [super init]) {
        
        _mirrorFrontCamera = NO;
        _mirrorBackCamera = NO;
        _mirrorFrontPreview = YES;
        _mirrorBackPreview = NO;
        mRotateAngle = 0;
        _viewFrame = frame;
        mBlendImage = nullptr;
        mBlendImageFilter = nullptr;
        mBlendForEncodeFilter = nullptr;
        mBrightnessFilter = nullptr;
        mReady = NO;
        
        _outputOrientation = UIInterfaceOrientationPortrait;
        _frontVideoOrientation = AVCaptureVideoOrientationLandscapeLeft;
        _backVideoOrientation = AVCaptureVideoOrientationLandscapeRight;
        _previewFillMode = ArcGLFillModePreserveAspectRatioAndFill;
        
        [self setRunProcess];
        [self setSemaphore];
        [self setSampleBufferFilter];
        [self setRenderView];
        
        self.cameraPosition = position;
    }
    return self;
}

- (void)setRunProcess {
    ArciOSContext* context = static_cast<ArciOSContext*>(ArcGLContext::getInstance());
    void* process = context -> runProcess();
    mRunProcess = (__bridge ArcRunProcess*)process;
}

- (void)setSemaphore {
    mRenderSemaphore = dispatch_semaphore_create(1);
}

- (void)setSampleBufferFilter {
    mSampleBufferFilter = new ArcSampleBufferFilter();
    mSampleBufferFilter -> setFillMode(ArcGLFillModePreserveAspectRatioAndFill);
    mSampleBufferFilter -> setCompleteCallback(renderCompleteForEncode, (__bridge void*)self);
}

- (void)setRenderView {
    mRenderView = new ArcRenderView(_viewFrame);
    mRenderView -> setFillMode(_previewFillMode);
}

- (UIView *)renderView {
    return mRenderView -> view();
}

- (void)setViewFrame:(CGRect)viewFrame {
    _viewFrame = viewFrame;
    if(NSThread.isMainThread) {
        UIView* view = mRenderView -> view();
        view.frame = viewFrame;
        return;
    }
    dispatch_async(dispatch_get_main_queue(), ^{
        UIView* view = self->mRenderView -> view();
        view.frame = viewFrame;
    });
}

- (void)setOutPutSize:(CGSize)outPutSize {
    _outPutSize = outPutSize;
}

- (void)setVideoSize:(CGSize)videoSize {
    _videoSize = videoSize;
}

- (void)receiveSampleBuffer:(CMSampleBufferRef)sampleBuffer {
    
    if (dispatch_semaphore_wait(mRenderSemaphore, DISPATCH_TIME_NOW) != 0)
    {
        return;
    }

    CFRetain(sampleBuffer);
    
    __weak __typeof(self) weakSelf = self;
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        [weakSelf processVideoSampleBuffer:sampleBuffer];
        CFRelease(sampleBuffer);
        dispatch_semaphore_signal(self -> mRenderSemaphore);
    });
}

- (void)processVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer {
    ArciOSContext::getInstance() -> makeAsCurrent();
    mCurrentFrameTimeStamp = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);
    CVPixelBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    _sampleBuffer = sampleBuffer;
    [self processPixelBuffer:imageBuffer];
}

- (void)processPixelBuffer:(CVPixelBufferRef)pixelBuffer {
    
    float width = CVPixelBufferGetWidth(pixelBuffer);
    float height = CVPixelBufferGetHeight(pixelBuffer);
    
    if((width != mOriginalSize.width || height != mOriginalSize.height)){
        mOriginalSize = CGSizeMake(width, height);
        [self calculateVideoSize];
    }
    
    if(mReady == NO) {
        [self linkFilters];
        mReady = YES;
    }
    
    mSampleBufferFilter->processPixelBuffer(pixelBuffer);
}

- (void)setOutputOrientation:(UIInterfaceOrientation)outputOrientation {
    _outputOrientation = outputOrientation;
    [self setCameraPosition:_cameraPosition];
}

-(void)setCameraPosition:(AVCaptureDevicePosition)cameraPosition{
    _cameraPosition = cameraPosition;
    // 根据设备方向和视频帧方向计算旋转角度
    mRotateAngle = 0;
    switch(_outputOrientation)
    {
        case UIInterfaceOrientationPortraitUpsideDown:
            mRotateAngle = 180;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            mRotateAngle = 90;
            break;
        case UIInterfaceOrientationLandscapeRight:
            mRotateAngle = -90;
            break;
        case UIInterfaceOrientationPortrait:
        default:
            mRotateAngle = 0;
    }

    AVCaptureVideoOrientation orientation = AVCaptureVideoOrientationPortrait;
    if (_cameraPosition == AVCaptureDevicePositionFront) {
        orientation = _frontVideoOrientation;
    }
    else if (_cameraPosition == AVCaptureDevicePositionBack) {
        orientation = _backVideoOrientation;
    }

    switch(orientation)
    {
        case AVCaptureVideoOrientationPortraitUpsideDown:
            mRotateAngle += 180;
            break;
        case AVCaptureVideoOrientationLandscapeLeft:
            mRotateAngle -= 90;
            break;
        case AVCaptureVideoOrientationLandscapeRight:
            mRotateAngle += 90;
            break;
        case AVCaptureVideoOrientationPortrait:
        default:
            mRotateAngle += 0;
            break;
    }
    
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        [self calRotation:self->mSampleBufferFilter];
        if (self.cameraPosition == AVCaptureDevicePositionFront) {
            [self setMirrorFrontPreview:self.mirrorFrontPreview];
            [self setMirrorFrontCamera:self.mirrorFrontCamera];
        }
        else if (self.cameraPosition == AVCaptureDevicePositionBack) {
            [self setMirrorBackPreview:self.mirrorBackPreview];
            [self setMirrorBackCamera:self.mirrorBackCamera];
        }
        [self calculateVideoSize];
    });
}

- (void)setMirrorFrontPreview:(BOOL)mirrorFrontPreview {
    _mirrorFrontPreview = mirrorFrontPreview;
    if (_cameraPosition != AVCaptureDevicePositionFront) {
        return;
    }
    
    ArcGLRotation rotation = _mirrorFrontPreview ? ArcGLFlipHorizonal : ArcGLNoRotation;
    mRenderView -> setOutputRotation(rotation);
}

- (void)setMirrorFrontCamera:(BOOL)mirrorFrontCamera {
    _mirrorFrontCamera = mirrorFrontCamera;
    if (_cameraPosition != AVCaptureDevicePositionFront) {
        return;
    }
    
    if(mBlendImageFilter) {
        mBlendImageFilter -> setMirror(true);
    }
    
    if(mBlendForEncodeFilter) {
        mBlendForEncodeFilter -> setMirror(true);
    }
}

- (void)setMirrorBackPreview:(BOOL)mirrorBackPreview {
    _mirrorBackPreview = mirrorBackPreview;
    if (_cameraPosition != AVCaptureDevicePositionBack) {
        return;
    }
    
    ArcGLRotation rotation = _mirrorBackPreview ? ArcGLFlipHorizonal : ArcGLNoRotation;
    mRenderView -> setOutputRotation(rotation);
}

- (void)setMirrorBackCamera:(BOOL)mirrorBackCamera {
    _mirrorBackCamera = mirrorBackCamera;
    if (_cameraPosition != AVCaptureDevicePositionBack) {
        return;
    }
    
    if(mBlendImageFilter) {
        mBlendImageFilter -> setMirror(false);
    }
    
    if(mBlendForEncodeFilter) {
        mBlendForEncodeFilter -> setMirror(false);
    }
}

- (void)setFrontVideoOrientation:(AVCaptureVideoOrientation)frontVideoOrientation {
    _frontVideoOrientation = frontVideoOrientation;
    if (_cameraPosition != AVCaptureDevicePositionFront) {
        return;
    }
    [self setCameraPosition:_cameraPosition];
}

-(void)setBackVideoOrientation:(AVCaptureVideoOrientation)backVideoOrientation{
    _backVideoOrientation = backVideoOrientation;
    if (_cameraPosition!=AVCaptureDevicePositionBack) {
        return;
    }
    [self setCameraPosition:_cameraPosition];
}

-(void)calRotation:(ArcGLInput*)input{
    if (_cameraPosition==AVCaptureDevicePositionBack) {
        switch(mRotateAngle)
        {
            case 0:
            case 360:
                _outputRotation = ArcGLNoRotation; break;
            case 90:
                _outputRotation = ArcGLRotateRight; break;
            case -90:
                _outputRotation = ArcGLRotateLeft; break;
            case 180:
            case -180:
                _outputRotation = ArcGLRotate180; break;
            default:_outputRotation = ArcGLNoRotation;
        }
    } else {
        switch(mRotateAngle)
        {
            case 0:
            case 360:
                _outputRotation = ArcGLNoRotation; break;
            case 90:
                _outputRotation = ArcGLRotateLeft; break;
            case -90:
                _outputRotation = ArcGLRotateRight; break;
            case 180:
            case -180:
                _outputRotation = ArcGLRotate180; break;
            default:_outputRotation = ArcGLNoRotation;
        }
    }
    input -> setOutputRotation(_outputRotation);
}

-(void)calculateVideoSize{
    switch(mRotateAngle)
    {
        case 90:
        case -90:
            _videoSize = CGSizeMake(mOriginalSize.height, mOriginalSize.width);
            break;
        case 0:
        case 360:
        case 180:
        case -180:
        default:
            _videoSize = CGSizeMake(mOriginalSize.width, mOriginalSize.height);
    }
    
    CGSize s = _outPutSize;
    if(_viewFrame.size.width > _viewFrame.size.height) {
        s = CGSizeMake(max(_outPutSize.width, _outPutSize.height), min(_outPutSize.width, _outPutSize.height));
    } else {
        s = CGSizeMake(min(_outPutSize.width, _outPutSize.height), max(_outPutSize.width, _outPutSize.height));
    }
    
    ArcGLSize size = [self getGLSize:s];
    ArcGLSize frameSize = [self getGLSize:_videoSize];
    mSampleBufferFilter -> setFrameSize(frameSize);
    mSampleBufferFilter -> setOutputSize(size);
    if(mBlendImageFilter) {
        mBlendImageFilter -> setFrameSize(frameSize);
        mBlendImageFilter -> setOutputSize(size);
        ArcGLSize viewSize = [self getGLSize:self.viewFrame.size];
        ArcGLRect imagaRect = [self getGLRect:mBlendImageRect];
        mBlendImageFilter -> updateImageRectViewSize(imagaRect, viewSize);
    }
    if(mBlendForEncodeFilter) {
        mBlendForEncodeFilter -> setFrameSize(frameSize);
        mBlendForEncodeFilter -> setOutputSize(size);
        ArcGLSize viewSize = [self getGLSize:self.viewFrame.size];
        ArcGLRect imagaRect = [self getGLRect:mBlendImageRect];
        mBlendForEncodeFilter -> updateImageRectViewSize(imagaRect, viewSize);
    }
    
    if(mBrightnessFilter) {
        mBrightnessFilter -> setFrameSize(frameSize);
        mBrightnessFilter -> setOutputSize(size);
    }
}

- (void)setPreviewRotation:(ArcGLRotation)previewRotation {
    _previewRotation = previewRotation;
    mRenderView -> setOutputRotation(previewRotation);
}

- (void)setOutputRotation:(ArcGLRotation)outputRotation {
    _outputRotation = outputRotation;
    mSampleBufferFilter -> setOutputRotation(outputRotation);
}

- (void)setOutputFillMode:(ArcGLFillMode)outputFillMode {
    _outputFillMode = outputFillMode;
    mSampleBufferFilter -> setFillMode(outputFillMode);
}

- (void)setPreviewFillMode:(ArcGLFillMode)previewFillMode {
    _previewFillMode = previewFillMode;
    mRenderView -> setFillMode(previewFillMode);
}

- (void)setPreviewColor:(UIColor *)color {
    mRenderView -> setPreviewColor(color);
}

- (ArcGLSize)getGLSize:(CGSize)size {
    ArcGLSize s = {static_cast<unsigned>(size.width), static_cast<unsigned>(size.height)};
    return s;
}

- (ArcGLRect)getGLRect:(CGRect)rect {
    ArcGLRect glRect = {static_cast<float>(rect.origin.x), static_cast<float>(rect.origin.y), static_cast<unsigned>(rect.size.width), static_cast<unsigned>(rect.size.height)};
    return glRect;
}

- (void)setHasEncodeVideoFrame:(BOOL)hasEncodeVideoFrame {
    _hasEncodeVideoFrame = hasEncodeVideoFrame;
    if(mBlendForEncodeFilter) {
        if(_hasEncodeVideoFrame) {
            mBlendForEncodeFilter -> enable();
        } else {
            mBlendForEncodeFilter -> disable();
        }
    }
}

- (void)createBlendFilter {
    
    if(mBlendImageFilter == nullptr) {
        ArcGLSize viewSize = [self getGLSize:_viewFrame.size];
        ArcGLRect imageRect = {static_cast<float>(mBlendImageRect.origin.x), static_cast<float>(mBlendImageRect.origin.y), static_cast<unsigned>(mBlendImageRect.size.width), static_cast<unsigned>(mBlendImageRect.size.height)};
        mBlendImageFilter = new ArcBlendImageFilter(imageRect, viewSize);
        mFilters.push_back(mBlendImageFilter);
        if(_cameraPosition == AVCaptureDevicePositionFront) {
            mBlendImageFilter -> setMirror(true);
        } else {
            mBlendImageFilter -> setMirror(false);
        }
        ArcGLSize size = [self getGLSize:_outPutSize];
        mBlendImageFilter -> setOutputSize(size);
        mBlendImageFilter -> setOutputRotation(_outputRotation);
        mReady = NO;
    }
    
}

- (void)removeBlendImageFilter {
    if(mBlendImageFilter != nullptr) {
        mBlendImageFilter -> removeAllTargets();
        mFilters.remove(mBlendImageFilter);
        delete mBlendImageFilter;
        mBlendImageFilter = nullptr;
    }
}

- (void)createBlendForEncodeFilter {
    
    if(mBlendForEncodeFilter == nullptr) {
        ArcGLSize viewSize = [self getGLSize:_viewFrame.size];
        ArcGLRect imageRect = {static_cast<float>(mBlendImageRect.origin.x), static_cast<float>(mBlendImageRect.origin.y), static_cast<unsigned>(mBlendImageRect.size.width), static_cast<unsigned>(mBlendImageRect.size.height)};
        mBlendForEncodeFilter = new ArcBlendForEncodeFilter(imageRect, viewSize);
        mFilters.push_back(mBlendForEncodeFilter);
        if(_cameraPosition == AVCaptureDevicePositionFront) {
            mBlendForEncodeFilter -> setMirror(true);
        } else {
            mBlendForEncodeFilter -> setMirror(false);
        }
        ArcGLSize size = [self getGLSize:_outPutSize];
        mBlendForEncodeFilter -> setOutputSize(size);
        mBlendForEncodeFilter -> setOutputRotation(_outputRotation);
    
        mBlendForEncodeFilter -> setCompleteCallback(blendRenderCompleteForEncode, (__bridge void*)self);
    }
}

- (void)removeBlendImageForEncodeFilter {
    if(mBlendForEncodeFilter != nullptr) {
        mFilters.remove(mBlendForEncodeFilter);
        delete mBlendForEncodeFilter;
        mBlendForEncodeFilter = nullptr;
    }
}

- (void)createBlendImageWithImage:(UIImage*)image {
    [self deleteBlendImage];
    mBlendImage = new ArciOSGLImage(image.CGImage);
    
    mBlendImage -> addTarget(mBlendImageFilter);
    mBlendImage -> addTarget(mBlendForEncodeFilter);
    
    mBlendImage -> informTargets();
}

- (void)setBlendImage:(UIImage *)image rect:(CGRect)rect {
    mBlendImageRect = rect;
    
    if(image) {
        [self processBlendImage:image];
    } else {
        [self removeBlendImage];
    }
}

- (void)processBlendImage:(UIImage*)image {
    __weak __typeof(self) weakSelf = self;
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        [weakSelf createBlendFilter];
        [weakSelf createBlendForEncodeFilter];
        [weakSelf createBlendImageWithImage:image];
    });
}

- (void)removeBlendImage {
    __weak __typeof(self) weakSelf = self;
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        [weakSelf removeBlendImageFilter];
        [weakSelf removeBlendImageForEncodeFilter];
        [weakSelf deleteBlendImage];
        self->mReady = NO;
    }); 
}

- (void)setEnableBlackFrame:(BOOL)enableBlackFrame {
    _enableBlackFrame = enableBlackFrame;
    __weak __typeof(self) weakSelf = self;
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        __strong __typeof(weakSelf) strongSelf = weakSelf;
        strongSelf -> mSampleBufferFilter -> enableBlackFrame(strongSelf.enableBlackFrame);
    });
}

- (void)deleteBlendImage {
    if(mBlendImage != nullptr) {
        delete mBlendImage;
        mBlendImage = nullptr;
    }
}

- (void)linkFilters {
    ArcGLFilter* f = mSampleBufferFilter;
    for(list<ArcGLFilter*>::iterator iter = mFilters.begin(); iter != mFilters.end(); ++iter) {
        f -> removeAllTargets();
        if(*iter == mBlendImageFilter) {
            f -> addTarget(*iter);
            list<ArcGLFilter*>::iterator iter2 = iter;
            f -> addTarget(*(++iter));
            f = *iter2;
            continue;
        }
        
        f -> addTarget(*iter);
        f = *iter;
    }
    f -> addTarget(mRenderView);
}

#pragma mark - Brightness
- (void)setBrightness:(float)brightness {
    
    if(brightness < -1.0 || brightness > 1.0) {
        return;
    }
    
    runSynchronouslyOnProcessQueue(mRunProcess, ^{
        self->_brightness = brightness;
        if(self->mBrightnessFilter == nullptr) {
            [self createBrightnessFilterWithValue:brightness];
        } else {
            self->mBrightnessFilter -> setBrightness(brightness);
        }
    });
}

- (void)createBrightnessFilterWithValue:(float)value {
    mBrightnessFilter = new ArcGLBrightnessFilter(value);
    mFilters.push_front(mBrightnessFilter);
    ArcGLSize size = [self getGLSize:_outPutSize];
    mBrightnessFilter -> setOutputSize(size);
    mBrightnessFilter -> setOutputRotation(_outputRotation);
    mReady = NO;
}

#pragma mark - Callback
- (void)setPixelBufferForEncodeCallback:(PixelBufferForEncode)pixelBufferCb {
    _mPixelBufferBlock = pixelBufferCb;
}

void renderCompleteForEncode(ArcGLOutput* output, void* para) {
    FrameBufferPtr frameBuffer = output -> m_outFrameBuffer;
    CVPixelBufferRef pixelBuffer = static_cast<CVPixelBufferRef>(frameBuffer -> pixelBuffer());
    ArcRender* render = (__bridge ArcRender*)para;
    
    if(render.mPixelBufferBlock && render.hasEncodeVideoFrame && render -> mBlendForEncodeFilter == nil) {
        render.mPixelBufferBlock(pixelBuffer);
    }
}

void blendRenderCompleteForEncode(ArcGLOutput* output, void* para) {
    FrameBufferPtr frameBuffer = output -> m_outFrameBuffer;
    CVPixelBufferRef pixelBuffer = static_cast<CVPixelBufferRef>(frameBuffer -> pixelBuffer());
    ArcRender* render = (__bridge ArcRender*)para;
    
    if(render.mPixelBufferBlock && render.hasEncodeVideoFrame && render -> mBlendForEncodeFilter) {
        render.mPixelBufferBlock(pixelBuffer);
    }
}

#pragma mark - dealloc
- (void)dealloc {
    mRunProcess = nil;
    delete mSampleBufferFilter;
    ArciOSContext::destroyInstance();
    [self deleteBlendImage];
}

@end
