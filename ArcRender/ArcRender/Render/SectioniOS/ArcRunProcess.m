//
//  ArcRunProcess.m
//  ArcRender
//
//  Created by Zhiming He on 2018/9/5.
//  Copyright © 2018年 Zhiming He. All rights reserved.
//


#import "ArcRunProcess.h"

@interface ArcRunProcess()
{
    void *mProcessQueueKey;
    NSString * mLabelStr;
}

@end


@implementation ArcRunProcess

- (instancetype)init {
    if(self = [super init]) {
        mLabelStr = @"RunProcess";
        mProcessQueueKey = &mProcessQueueKey;
    }
    return self;
}

- (void)createProcessQueueWithLabel:(NSString *)LabelStr {
    
    if(_mProcessQueue) {
        NSLog(@"%@ contextQueue has being created!", self.class);
        return;
    }
    
    if(LabelStr || LabelStr.length > 0) {
        mLabelStr = LabelStr;
    }
    _mProcessQueue = dispatch_queue_create(mLabelStr.UTF8String, NULL);
    dispatch_queue_set_specific(self.mProcessQueue,mProcessQueueKey, (__bridge void *)self, NULL);
}

- (void *)processQueueKey {
    return mProcessQueueKey;
}

- (void)dealloc {
    _mProcessQueue = nil;
}

@end

void runSynchronouslyOnMainQueue(void (^block)(void))
{
    if ([NSThread isMainThread])
    {
        block();
    }
    else
    {
        dispatch_sync(dispatch_get_main_queue(), block);
    }
}

void runAsynchronouslyOnMainQueue(void (^block)(void))
{
    if ([NSThread isMainThread])
    {
        block();
    }
    else
    {
        dispatch_async(dispatch_get_main_queue(), block);
    }
}

void runSynchronouslyOnProcessQueue(ArcRunProcess *process, void (^block)(void))
{
    if (process == nil) {
        return;
    }
    
    dispatch_queue_t shareProcessQueue = process.mProcessQueue;
    if (dispatch_get_specific([process processQueueKey]))
    {
        block();
    }
    else
    {
        dispatch_sync(shareProcessQueue, block);
    }
}

void runAsynchronouslyOnProcessQueue(ArcRunProcess *process, void (^block)(void))
{
    if (process == nil) {
        return;
    }
    
    dispatch_queue_t shareProcessQueue = process.mProcessQueue;
    if (dispatch_get_specific([process processQueueKey]))
    {
        block();
    }
    else
    {
        dispatch_async(shareProcessQueue, block);
    }
}
