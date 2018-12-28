//
//  ArcGLView.m
//  ArcRender
//
//  Created by Zhiming He on 2018/11/27.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArcGLView.h"
#import "ArciOSContext.h"
#import "ArcGLFilter.hpp"
#import <AVFoundation/AVFoundation.h>
#import "ArcGLVertexArray.hpp"
#import "ArcGLVBOCache.hpp"
#import "ArcRunProcess.h"

typedef struct GL_Context{
    EAGLContext* m_context;
    EAGLSharegroup* m_sharegroup;
    CVOpenGLESTextureCacheRef m_textureCache;
}GL_Context;

@interface ArcGLView ()
{
    __unsafe_unretained ArcRunProcess* mRunProcess;
    FrameBufferPtr inputFramebufferForDisplay;
    GLuint displayRenderbuffer, displayFramebuffer;
    
    GLint displayPositionAttribute, displayTextureCoordinateAttribute;
    GLint displayInputTextureUniform;
    
    CGSize inputImageSize;
    GLfloat imageVertices[8];
    
    NSUInteger aspectRatio;
    
    CGSize boundsSizeAtFrameBufferEpoch;
    ArciOSContext* mContext;
    
    ArcGLVertexBuffer* vertexVBO;
    ArcGLVertexBuffer* texCoorVBO;
    ArcGLVertexArray* VAO;
}
@end

@implementation ArcGLView

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (instancetype)initWithFrame:(CGRect)frame {
    if (!(self = [super initWithFrame:frame]))
    {
        return nil;
    }
    
    [self commonInit];
    return self;
}

- (void)commonInit {
    
    self.contentScaleFactor = [[UIScreen mainScreen] scale];
    _previewColor = [UIColor blackColor];
    _viewFrame = self.bounds;
    _rotation = ArcGLNoRotation;
    _fillMode = ArcGLFillModePreserveAspectRatioAndFill;
    mContext = static_cast<ArciOSContext*>(ArcGLContext::getInstance());
    mContext -> useProgram(ArcGLFilter::g_vertext300_shader, ArcGLFilter::g_fragment300_shader_BGRA);
    displayPositionAttribute = mContext -> attribute("position");
    displayTextureCoordinateAttribute = mContext -> attribute("inputTextureCoordinate");
    
    CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
    eaglLayer.opaque = YES;
    // 设置描绘属性，在这里设置不维持渲染内容以及颜色格式为 RGBA8
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:NO], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
    
    [self setRunProcess];
    
    mContext -> makeAsCurrent();
    [self createVAO];
    [self createVBO];
    [self createDisplayFramebuffer];
    
    _enabled = YES;
}

- (void)setRunProcess {
    ArciOSContext* context = static_cast<ArciOSContext*>(ArcGLContext::getInstance());
    void* process = context -> runProcess();
    mRunProcess = (__bridge ArcRunProcess*)process;
}

- (void)setInputs:(unsigned int)inputsNum {
    int input_textures[1] = {0};
    mContext -> setUniformIndex("inputImageTexture", 1, input_textures);
}

- (void)setRotation:(ArcGLRotation)rotation {
    _rotation = rotation;
    [self recalculateViewGeometry];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    // The frame buffer needs to be trashed and re-created when the view size changes.
    if (!CGSizeEqualToSize(self.bounds.size, boundsSizeAtFrameBufferEpoch) &&
        !CGSizeEqualToSize(self.bounds.size, CGSizeZero)) {
        
        [self destroyDisplayFramebuffer];
        [self createDisplayFramebuffer];
    } else if (!CGSizeEqualToSize(self.bounds.size, CGSizeZero)) {
        [self recalculateViewGeometry];
    }
}

- (void)setFrame:(CGRect)frame {
    [super setFrame:frame];
    
    _viewFrame = frame;
}

- (void)dealloc {
    [self destroyDisplayFramebuffer];
}

#pragma mark -
#pragma mark Managing the display FBOs

- (void)createDisplayFramebuffer {
    
    runSynchronouslyOnProcessQueue(mRunProcess, ^{
        glGenFramebuffers(1, &self->displayFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, self->displayFramebuffer);
        
        glGenRenderbuffers(1, &self->displayRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, self->displayRenderbuffer);
        
        GL_Context* glContext = (GL_Context*)(self->mContext -> context());
        EAGLContext* context = glContext -> m_context;
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
        
        GLint backingWidth, backingHeight;
        
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
        
        if ( (backingWidth == 0) || (backingHeight == 0) )
        {
            [self destroyDisplayFramebuffer];
            return;
        }
        
        self->_sizeInPixels.width = (CGFloat)backingWidth;
        self->_sizeInPixels.height = (CGFloat)backingHeight;
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, self->displayRenderbuffer);
        
        self->boundsSizeAtFrameBufferEpoch = self.bounds.size;
        
        [self recalculateViewGeometry];
    });

}

- (void)destroyDisplayFramebuffer {
    
    runSynchronouslyOnProcessQueue(mRunProcess, ^{
        
        if (self->displayFramebuffer)
        {
            glDeleteFramebuffers(1, &self->displayFramebuffer);
            self->displayFramebuffer = 0;
        }
        
        if (self->displayRenderbuffer)
        {
            glDeleteRenderbuffers(1, &self->displayRenderbuffer);
            self->displayRenderbuffer = 0;
        }
    });
}

- (void)createVBO {
    ArcGLVBOCache* cache = ArcGLVBOCache::getInstance();
    vertexVBO = cache -> newVertexBuffer();
    texCoorVBO = cache -> newVertexBuffer();
}

- (void)createVAO {
    VAO = new ArcGLVertexArray();
}

- (void)updateVAO {
    vertexVBO -> setBuffer(&imageVertices[0]);
    texCoorVBO -> setBuffer([ArcGLView textureCoordinatesForRotation:_rotation]);
    
    VAO -> bind();
    
    vertexVBO -> activeBuffer(displayPositionAttribute);
    texCoorVBO -> activeBuffer(displayTextureCoordinateAttribute);
    
    VAO -> unbind();
}

- (void)activeBuffer {
    if (!displayFramebuffer)
    {
        [self createDisplayFramebuffer];
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, displayFramebuffer);
    glViewport(0, 0, (GLint)_sizeInPixels.width, (GLint)_sizeInPixels.height);
    CGFloat r,g,b,a;
    [_previewColor getRed:&r green:&g blue:&b alpha:&a];
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

- (void)newFrame {
    mContext -> makeAsCurrent();

    [self activeBuffer];

    glUniform1i(0, 0);
    
    VAO -> bind();
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    [self presentFramebuffer];
    
    VAO -> unbind();
    inputFramebufferForDisplay -> releaseForUnUse();
}

- (void)presentFramebuffer {
    glBindRenderbuffer(GL_RENDERBUFFER, displayRenderbuffer);
    mContext -> swapBuffer();
}

#pragma mark -
#pragma mark Accessors

- (void)setFillMode:(ArcGLFillMode)newValue;
{
    _fillMode = newValue;
    [self recalculateViewGeometry];
}

-(void)setInputFrameBuffer:(FrameBufferPtr)frameBuffer{
    inputFramebufferForDisplay = frameBuffer;
    mContext -> useProgram(ArcGLFilter::g_vertext300_shader, ArcGLFilter::g_fragment300_shader_BGRA);
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D, inputFramebufferForDisplay -> texture() -> texture());
    if (inputImageSize.width != frameBuffer -> size().width || inputImageSize.height != frameBuffer -> size().height) {
        inputImageSize = CGSizeMake(frameBuffer -> size().width, frameBuffer -> size().height);
        [self recalculateViewGeometry];
    }
}

#pragma mark -
#pragma mark Handling fill mode

- (void)recalculateViewGeometry {
    
    runAsynchronouslyOnProcessQueue(mRunProcess, ^{
        CGFloat heightScaling, widthScaling;
        CGRect insetRect = AVMakeRectWithAspectRatioInsideRect(self->inputImageSize, self.viewFrame);
        switch(self.fillMode)
        {
            case ArcGLFillModeStretch:
            {
                widthScaling = 1.0;
                heightScaling = 1.0;
            }; break;
            case ArcGLFillModePreserveAspectRatio:
            {
                widthScaling = insetRect.size.width / self.viewFrame.size.width;
                heightScaling = insetRect.size.height / self.viewFrame.size.height;
            }; break;
            case ArcGLFillModePreserveAspectRatioAndFill:
            {
                widthScaling = self.viewFrame.size.height / insetRect.size.height;
                heightScaling = self.viewFrame.size.width / insetRect.size.width;
            }; break;
        }
        self->imageVertices[0] = -widthScaling;
        self->imageVertices[1] = -heightScaling;
        self->imageVertices[2] = widthScaling;
        self->imageVertices[3] = -heightScaling;
        self->imageVertices[4] = -widthScaling;
        self->imageVertices[5] = heightScaling;
        self->imageVertices[6] = widthScaling;
        self->imageVertices[7] = heightScaling;
        
        
        [self updateVAO];
    });
}

+ (const GLfloat *)textureCoordinatesForRotation:(ArcGLRotation)rotationMode {
    
    static const GLfloat noRotationTextureCoordinates[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
    };
    
    static const GLfloat rotateRightTextureCoordinates[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
    };
    
    static const GLfloat rotateLeftTextureCoordinates[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };
    
    static const GLfloat verticalFlipTextureCoordinates[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
    };
    
    static const GLfloat horizontalFlipTextureCoordinates[] = {
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
    };
    
    static const GLfloat rotateRightVerticalFlipTextureCoordinates[] = {
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
    };
    
    static const GLfloat rotateRightHorizontalFlipTextureCoordinates[] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
    };
    
    static const GLfloat rotate180TextureCoordinates[] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };
    
    switch(rotationMode)
    {
        case ArcGLNoRotation: return noRotationTextureCoordinates;
        case ArcGLRotateLeft: return rotateLeftTextureCoordinates;
        case ArcGLRotateRight: return rotateRightTextureCoordinates;
        case ArcGLFlipVertical: return verticalFlipTextureCoordinates;
        case ArcGLFlipHorizonal: return horizontalFlipTextureCoordinates;
        case ArcGLRotateRightFlipVertical: return rotateRightVerticalFlipTextureCoordinates;
        case ArcGLRotateRightFlipHorizontal: return rotateRightHorizontalFlipTextureCoordinates;
        case ArcGLRotate180: return rotate180TextureCoordinates;
    }
}

@end
