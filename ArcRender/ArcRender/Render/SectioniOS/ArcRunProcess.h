//
//  ArcRunProcess.h
//  ArcRender
//
//  Created by Zhiming He on 2018/9/5.
//  Copyright © 2018年 Zhiming He. All rights reserved.
//


#import <Foundation/Foundation.h>

@interface ArcRunProcess : NSObject

- (void)createProcessQueueWithLabel:(NSString *)LabelStr;

- (dispatch_queue_t)processQueue;

@end

#ifdef __cplusplus
extern "C" {
#endif
extern void runSynchronouslyOnMainQueue(void (^block)(void));
extern void runAsynchronouslyOnMainQueue(void (^block)(void));
extern void runSynchronouslyOnProcessQueue(ArcRunProcess *process, void (^block)(void));
extern void runAsynchronouslyOnProcessQueue(ArcRunProcess *process, void (^block)(void));
#ifdef __cplusplus
}
#endif
