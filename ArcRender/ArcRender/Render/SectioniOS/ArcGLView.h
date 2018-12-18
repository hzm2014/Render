//
//  ArcGLView.h
//  ArcRender
//
//  Created by Zhiming He on 2018/11/27.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ArcGLHeader.h"
#import "ArcGLFrameBuffer.hpp"

NS_ASSUME_NONNULL_BEGIN

@interface ArcGLView : UIView

@property(assign, nonatomic) ArcGLFillMode fillMode;
@property(nonatomic, assign)ArcGLRotation rotation;
@property(assign, nonatomic) CGSize sizeInPixels;
@property(readonly, nonatomic)CGRect viewFrame;
@property(assign, nonatomic)UIColor* previewColor;

@property(nonatomic) BOOL enabled;

-(void)setInputs:(unsigned)inputsNum;
- (void)newFrame;
-(void)setInputFrameBuffer:(FrameBufferPtr)frameBuffer;

@end

NS_ASSUME_NONNULL_END
