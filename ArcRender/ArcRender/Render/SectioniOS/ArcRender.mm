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
#import "ArciOSGLImage.h"
#import "ArcBlendImageFilter.hpp"
#import "ArcBlendForEncodeFilter.hpp"
#import "ArcGLBrightnessFilter.hpp"
#import "ArcGLWhiteningFilter.hpp"
#import "ArcGLSmoothFilter.hpp"
#import "ArcGLBeautyFilter.hpp"
#import "ArcSoftBeautyFilter.hpp"
#import "ArcRGBFilter.hpp"
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
    ArcGLWhiteningFilter* mWhiteningFilter;
    ArcGLSmoothFilter* mSmoothFilter;
    ArcGLBeautyFilter* mBeautyFilter;
    ArcSoftBeautyFilter* mArcSoftBeautyFilter;
    ArcRGBFilter* mRGBFilter;
    
    list<ArcGLFilter*> mFilters; //不包含SampleBufferFilter和renderView
    BOOL mReady;
}

@property (nonatomic, copy) PixelBufferForEncode mPixelBufferBlock;

@end

@implementation ArcRender

static ArcRender* render = nil;

+ (instancetype)sharedInstance {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        render = [[self alloc] init];
    });
    return render;
}

+ (void)releaseInstance {
    render = nil;
}

- (instancetype)init {
    if(!(self = [super init])) {
        return nil;
    }
    [self commonInit];
    return self;
}

- (instancetype)initWithViewFrame:(CGRect)frame {
    if(self = [super init]) {
        
        [self commonInit];
        self.viewFrame = frame;
    }
    return self;
}

- (void)commonInit {
    _mirrorForPreview = NO;
    _mirrorForOutput = NO;
    mRotateAngle = 0;
    
    _previewFillMode = ArcGLFillModePreserveAspectRatioAndFill;
    _outputFillMode = ArcGLFillModePreserveAspectRatioAndFill;
    _outputRotation = ArcGLNoRotation;
    _previewRotation = ArcGLNoRotation;
    
    _outPutSize = CGSizeMake(720, 1280);
    
    mBlendImage = nullptr;
    mBlendImageFilter = nullptr;
    mBlendForEncodeFilter = nullptr;
    mBrightnessFilter = nullptr;
    mWhiteningFilter = nullptr;
    mSmoothFilter = nullptr;
    mBeautyFilter = nullptr;
    mReady = NO;
    
    [self setRunProcess];
    [self setSemaphore];
    [self setSampleBufferFilter];
    [self setRenderView];
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
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        [self calculateVideoSize];
    });
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

- (void)setMirrorForPreview:(BOOL)mirrorForPreview {
    _mirrorForPreview = mirrorForPreview;
    //TODO
}

- (void)setMirrorForOutput:(BOOL)mirrorForOutput {
    _mirrorForOutput = mirrorForOutput;
    
    __weak __typeof(self) weakSelf = self;
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        __strong __typeof(weakSelf) strongSelf = weakSelf;
        strongSelf->mSampleBufferFilter -> setMirror(mirrorForOutput);
        [weakSelf calculateVideoSize];
    });
}

-(void)calculateVideoSize{

    if(_outPutSize.width == 0 || _outPutSize.height == 0 || mOriginalSize.width == 0 || mOriginalSize.height == 0) {
        return;
    }
    
    CGSize frameSize = mOriginalSize;
    if(_outPutSize.width > _outPutSize.height) {
        frameSize.width = MAX(mOriginalSize.width, mOriginalSize.height);
        frameSize.height = MIN(mOriginalSize.width, mOriginalSize.height);
    } else {
        frameSize.width = MIN(mOriginalSize.width, mOriginalSize.height);
        frameSize.height = MAX(mOriginalSize.width, mOriginalSize.height);
    }
    
    ArcGLSize outputSize = [self getGLSize:_outPutSize];
    ArcGLSize originalSize = [self getGLSize:frameSize];
    mSampleBufferFilter -> setFrameSize(originalSize);
    mSampleBufferFilter -> setOutputSize(outputSize);
    if(mBlendImageFilter) {
        mBlendImageFilter -> setFrameSize(outputSize);
        mBlendImageFilter -> setOutputSize(outputSize);
        ArcGLSize viewSize = [self getGLSize:_viewFrame.size];
        ArcGLRect imagaRect = [self getGLRect:mBlendImageRect];
        mBlendImageFilter -> updateImageRectViewSize(imagaRect, viewSize);
    }
    if(mBlendForEncodeFilter) {
        mBlendForEncodeFilter -> setFrameSize(outputSize);
        mBlendForEncodeFilter -> setOutputSize(outputSize);
        ArcGLSize viewSize = [self getGLSize:self.viewFrame.size];
        ArcGLRect imagaRect = [self getGLRect:mBlendImageRect];
        mBlendForEncodeFilter -> updateImageRectViewSize(imagaRect, viewSize);
    }
    
    if(mBrightnessFilter) {
        mBrightnessFilter -> setFrameSize(outputSize);
        mBrightnessFilter -> setOutputSize(outputSize);
    }
    
    if(mWhiteningFilter) {
        mWhiteningFilter -> setFrameSize(outputSize);
        mWhiteningFilter -> setOutputSize(outputSize);
    }
    
    if(mSmoothFilter) {
        mSmoothFilter -> setFrameSize(outputSize);
        mSmoothFilter -> setOutputSize(outputSize);
    }
    
    if(mBeautyFilter) {
        mBeautyFilter -> setFrameSize(outputSize);
        mBeautyFilter -> setOutputSize(outputSize);
    }
    
}

- (void)setPreviewRotation:(ArcGLRotation)previewRotation {
    _previewRotation = previewRotation;
    mRenderView -> setOutputRotation(previewRotation);
}

- (void)setOutputRotation:(ArcGLRotation)outputRotation {
    _outputRotation = outputRotation;
    __weak __typeof(self) weakSelf = self;
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        __strong __typeof(weakSelf) strongSelf = weakSelf;
        strongSelf->mSampleBufferFilter -> setOutputRotation(outputRotation);
    });
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
        ArcGLSize size = [self getGLSize:_outPutSize];
        mBlendImageFilter -> setFrameSize(size);
        mBlendImageFilter -> setOutputSize(size);
        mReady = NO;
    }
    
}

- (void)removeBlendImageFilter {
    if(mBlendImageFilter != nullptr) {
        mBlendImageFilter -> removeAllTargets();
        mBlendImageFilter -> removeAllSources();
        mFilters.remove(mBlendImageFilter);
        delete mBlendImageFilter;
        mBlendImageFilter = nullptr;
    }
}

- (void)createBlendForEncodeFilter {
    if(_hasEncodeVideoFrame == NO) {
        return;
    }
    if(mBlendForEncodeFilter == nullptr) {
        ArcGLSize viewSize = [self getGLSize:_viewFrame.size];
        ArcGLRect imageRect = {static_cast<float>(mBlendImageRect.origin.x), static_cast<float>(mBlendImageRect.origin.y), static_cast<unsigned>(mBlendImageRect.size.width), static_cast<unsigned>(mBlendImageRect.size.height)};
        mBlendForEncodeFilter = new ArcBlendForEncodeFilter(imageRect, viewSize);
        mFilters.push_back(mBlendForEncodeFilter);
        ArcGLSize size = [self getGLSize:_outPutSize];
        mBlendForEncodeFilter -> setFrameSize(size);
        mBlendForEncodeFilter -> setOutputSize(size);
    }
}

- (void)removeBlendImageForEncodeFilter {
    if(mBlendForEncodeFilter != nullptr) {
        mBlendForEncodeFilter -> removeAllTargets();
        mBlendForEncodeFilter -> removeAllSources();
        mFilters.remove(mBlendForEncodeFilter);
        delete mBlendForEncodeFilter;
        mBlendForEncodeFilter = nullptr;
    }
}

- (void)createBlendImageWithImage:(UIImage*)image {
    [self deleteBlendImage];
    mBlendImage = new ArciOSGLImage(image.CGImage);
    
    mBlendImage -> addTarget(mBlendImageFilter);
    if(mBlendForEncodeFilter) {
        mBlendImage -> addTarget(mBlendForEncodeFilter);
    }
    
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
    f -> setCompleteCallback(nullptr, nullptr);
    for(list<ArcGLFilter*>::iterator iter = mFilters.begin(); iter != mFilters.end(); ) {
        f -> removeAllTargets();
        f -> setCompleteCallback(nullptr, nullptr);
        if(*iter == mBlendImageFilter) {
            
            list<ArcGLFilter*>::iterator iter2 = iter;
            if(*(++iter2) == mBlendForEncodeFilter) {
                f -> addTarget(mBlendImageFilter);
                f -> addTarget(mBlendForEncodeFilter);
                f = mBlendImageFilter;
                ++iter;
                ++iter;
                continue;
            }
        }
        
        f -> addTarget(*iter);
        f = *iter;
        ++iter;
    }
    
    if(_hasEncodeVideoFrame) {
        
        if(mBlendForEncodeFilter) {
            mBlendForEncodeFilter -> setCompleteCallback(renderCompleteForEncode, (__bridge void*)self);
        } else {
            f -> setCompleteCallback(renderCompleteForEncode, (__bridge void*)self);
        }
    }
    f -> removeAllTargets();
    f -> addTarget(mRenderView);
}

- (void)removeAllFilters {
    
    if(mSampleBufferFilter) {
        delete mSampleBufferFilter;
        mSampleBufferFilter = nullptr;
    }
    
    if(mRenderView) {
        delete mRenderView;
        mRenderView = nullptr;
    }
    
    for(list<ArcGLFilter*>::iterator iter = mFilters.begin(); iter != mFilters.end(); ++iter) {
        ArcGLFilter* filter = *iter;
        filter -> removeAllTargets();
        delete filter;
    }
    mFilters.clear();
}

#pragma mark - Brightness
- (void)setBrightness:(float)brightness {
    
    if(brightness < -1.0 || brightness > 1.0) {
        return;
    }
    
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
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
    mReady = NO;
}

- (void)setWhitening:(float)whitening {
    
    if(whitening < 0 || whitening > 1.0) {
        return;
    }
    
    _whitening = whitening;
    
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        if(self->mWhiteningFilter == nullptr) {
            [self createWhiteningFilterWithValue:self.whitening];
        } else {
            self->mWhiteningFilter -> setStrength(self.whitening);
        }
    });
}

- (void)createWhiteningFilterWithValue:(float)val {
    mWhiteningFilter = new ArcGLWhiteningFilter(val);
    mFilters.push_front(mWhiteningFilter);
    ArcGLSize size = [self getGLSize:_outPutSize];
    mWhiteningFilter -> setOutputSize(size);
    mReady = NO;
}

- (void)setSmooth:(float)smooth {
    
    if(smooth < 0 || smooth > 1.0) {
        return;
    }
    
    _smooth = smooth;
    
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        if(self->mSmoothFilter == nullptr) {
            [self createSmoothFilterWithValue:self.smooth];
        } else {
            self->mSmoothFilter -> setSmooth(self.smooth);
        }
    });
}

- (void)createSmoothFilterWithValue:(float)val {
    mSmoothFilter = new ArcGLSmoothFilter(val);
    mFilters.push_front(mSmoothFilter);
    ArcGLSize size = [self getGLSize:_outPutSize];
    mSmoothFilter -> setOutputSize(size);
    mReady = NO;
}

- (void)setEnableBeauty:(BOOL)enableBeauty {
    _enableBeauty = enableBeauty;
    
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        if(enableBeauty == NO) {
            [self removeBeautyFilter];
            return;
        }
        if(self->mBeautyFilter == nullptr) {
            [self createBeautyFilter];
        }
    });
}

- (void)createBeautyFilter {
    mBeautyFilter = new ArcGLBeautyFilter();
    mFilters.push_front(mBeautyFilter);
    ArcGLSize size = [self getGLSize:_outPutSize];
    mBeautyFilter -> setOutputSize(size);
    mReady = NO;
}

- (void)removeBeautyFilter {
    if(mBeautyFilter) {
        mBeautyFilter -> removeAllTargets();
        delete mBeautyFilter;
        mBeautyFilter = nullptr;
        mReady = NO;
    }
}

#pragma mark - RedEffect

- (void)setRedEffectLevel:(int)value {
    __weak __typeof(self) weakSelf = self;
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        __strong __typeof(weakSelf) strongSelf = weakSelf;
        if(strongSelf -> mRGBFilter == nullptr) {
            [weakSelf createRGBFilter];
        }
        
        float tmpValue = (value*1.0/100) * 0.2;
        strongSelf -> mRGBFilter -> setGreen(1.0 - tmpValue);
        strongSelf -> mRGBFilter -> setBlue(1.0 - tmpValue);
    });
}

- (void)createRGBFilter {
    mRGBFilter = new ArcRGBFilter();
    mFilters.push_front(mRGBFilter);
    ArcGLSize size = [self getGLSize:_outPutSize];
    mRGBFilter -> setOutputSize(size);
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
    
    if(render.mPixelBufferBlock && render.hasEncodeVideoFrame) {
        render.mPixelBufferBlock(pixelBuffer);
    }
}

#pragma mark - dealloc
- (void)dealloc {
    mRunProcess = nil;
    ArcGLContext::destroyInstance();
    [self deleteBlendImage];
    [self removeAllFilters];
}

@end
