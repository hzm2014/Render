//
//  ArciOSGLImage.m
//  ArcRender
//
//  Created by zhiming he on 2018/12/2.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "ArciOSGLImage.h"
#import <CoreGraphics/CoreGraphics.h>
#import "ArcGLContext.hpp"

ArciOSGLImage::ArciOSGLImage(void* cgImageRef) : ArcGLImage(cgImageRef) {
    process(cgImageRef);
}

ArciOSGLImage::~ArciOSGLImage() {
    
}

void ArciOSGLImage::createOutputFrameBuffer(ArcGLSize &size, ArcGLTextureOption &option) {
    if(m_outFrameBuffer == nullptr) {
        m_outFrameBuffer = ArcGLContext::getInstance() -> frameBuffer(size, option, true);
    }
}

void ArciOSGLImage::process(void *cgImageRef) {
    CGImageRef newImageSource = (CGImageRef)cgImageRef;
    
    // TODO: Dispatch this whole thing asynchronously to move image loading off main thread
    unsigned widthOfImage = (unsigned)CGImageGetWidth(newImageSource);
    unsigned heightOfImage = (unsigned)CGImageGetHeight(newImageSource);
        
    ArcGLSize imageSize = {widthOfImage, heightOfImage};
    bool shouldRedrawUsingCoreGraphics = false;
    
    // For now, deal with images larger than the maximum texture size by resizing to be within that limit
    ArcGLSize scaledImageSizeToFitOnGPU = ArcGLContext::getInstance() -> sizeFitsTextureMaxSize(imageSize);
    
    if (!(scaledImageSizeToFitOnGPU.width == imageSize.width && scaledImageSizeToFitOnGPU.height == imageSize.height))
    {
        imageSize = scaledImageSizeToFitOnGPU;
        shouldRedrawUsingCoreGraphics = true;
    }
    
    setOutputSize(imageSize);
    
    GLubyte *imageData = NULL;
    CFDataRef dataFromImageDataProvider = NULL;
    GLenum format = GL_BGRA;
    
    if (!shouldRedrawUsingCoreGraphics) {
        /* Check that the memory layout is compatible with GL, as we cannot use glPixelStore to
         * tell GL about the memory layout with GLES.
         */
        if (CGImageGetBytesPerRow(newImageSource) != CGImageGetWidth(newImageSource) * 4 || CGImageGetBitsPerPixel(newImageSource) != 32 ||CGImageGetBitsPerComponent(newImageSource) != 8) {
            shouldRedrawUsingCoreGraphics = true;
        } else {
            /* Check that the bitmap pixel format is compatible with GL */
            CGBitmapInfo bitmapInfo = CGImageGetBitmapInfo(newImageSource);
            if ((bitmapInfo & kCGBitmapFloatComponents) != 0) {
                /* We don't support float components for use directly in GL */
                shouldRedrawUsingCoreGraphics = true;
            } else {
                CGBitmapInfo byteOrderInfo = bitmapInfo & kCGBitmapByteOrderMask;
                if (byteOrderInfo == kCGBitmapByteOrder32Little) {
                    /* Little endian, for alpha-first we can use this bitmap directly in GL */
                    CGImageAlphaInfo alphaInfo = (CGImageAlphaInfo)((uint32_t)bitmapInfo & (uint32_t)kCGBitmapAlphaInfoMask);
                    if (alphaInfo != kCGImageAlphaPremultipliedFirst && alphaInfo != kCGImageAlphaFirst &&
                        alphaInfo != kCGImageAlphaNoneSkipFirst) {
                        shouldRedrawUsingCoreGraphics = true;
                    }
                } else if (byteOrderInfo == kCGBitmapByteOrderDefault || byteOrderInfo == kCGBitmapByteOrder32Big) {
                    
                    /* Big endian, for alpha-last we can use this bitmap directly in GL */
                    CGImageAlphaInfo alphaInfo = (CGImageAlphaInfo)((uint32_t)bitmapInfo & (uint32_t)kCGBitmapAlphaInfoMask);
                    if (alphaInfo != kCGImageAlphaPremultipliedLast && alphaInfo != kCGImageAlphaLast &&
                        alphaInfo != kCGImageAlphaNoneSkipLast) {
                        shouldRedrawUsingCoreGraphics = true;
                    } else {
                        /* Can access directly using GL_RGBA pixel format */
                        format = GL_RGBA;
                    }
                }
            }
        }
    }
    
    //    CFAbsoluteTime elapsedTime, startTime = CFAbsoluteTimeGetCurrent();
    
    if (shouldRedrawUsingCoreGraphics) {
        // For resized or incompatible image: redraw
        imageData = (GLubyte *) calloc(1, imageSize.width * imageSize.height * 4);
        
        CGColorSpaceRef genericRGBColorspace = CGColorSpaceCreateDeviceRGB();
        
        CGContextRef imageContext = CGBitmapContextCreate(imageData, (size_t)imageSize.width, (size_t)imageSize.height, 8, (size_t)imageSize.width * 4, genericRGBColorspace,  kCGBitmapByteOrder32Little | kCGImageAlphaPremultipliedFirst);
        //        CGContextSetBlendMode(imageContext, kCGBlendModeCopy); // From Technical Q&A QA1708: http://developer.apple.com/library/ios/#qa/qa1708/_index.html
        CGContextDrawImage(imageContext, CGRectMake(0.0, 0.0, imageSize.width, imageSize.height), newImageSource);
        CGContextRelease(imageContext);
        CGColorSpaceRelease(genericRGBColorspace);
    } else {
        // Access the raw image bytes directly
        dataFromImageDataProvider = CGDataProviderCopyData(CGImageGetDataProvider(newImageSource));
        imageData = (GLubyte *)CFDataGetBytePtr(dataFromImageDataProvider);
    }
    
    ArcGLTextureOption option = ArcGLTexture::defaultOption();
    option.format = format;
    createOutputFrameBuffer(imageSize, option);
    m_outFrameBuffer -> setPixels(imageData);
    if (shouldRedrawUsingCoreGraphics)
    {
        free(imageData);
    }
    else
    {
        if (dataFromImageDataProvider)
        {
            CFRelease(dataFromImageDataProvider);
        }
    }
}
