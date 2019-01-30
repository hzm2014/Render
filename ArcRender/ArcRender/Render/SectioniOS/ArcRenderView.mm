//
//  ArcRenderView.m
//  ArcRender
//
//  Created by Zhiming He on 2018/11/27.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArcRenderView.h"

ArcRenderView::ArcRenderView(CGRect rect) {
    m_view = [[ArcGLView alloc] initWithFrame:rect];
    setInputs(1);
}

ArcRenderView::~ArcRenderView() {
    m_view = nil;
}

void ArcRenderView::setInputs(unsigned inputsNum) {
    ArcGLInput::setInputs(inputsNum);
    [m_view setInputs:inputsNum];
}

void ArcRenderView::setFillMode(ArcGLFillMode mode) {
    [m_view setFillMode:mode];
}

void ArcRenderView::setPreviewColor(UIColor *color) {
    m_view.previewColor = color;
}

void ArcRenderView::setInputFrameBuffer(FrameBufferPtr frameBuffer, unsigned location) {
    ArcGLInput::setInputFrameBuffer(frameBuffer, location);
    [m_view setInputFrameBuffer:frameBuffer];
}

void ArcRenderView::setOutputRotation(ArcGLRotation rotation) {
    m_rotation = rotation;
    m_view.rotation = rotation;
}

void ArcRenderView::newFrame() {
    [m_view newFrame];
}



