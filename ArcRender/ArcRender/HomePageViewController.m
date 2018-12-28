//
//  HomePageViewController.m
//  ArcRender
//
//  Created by Zhiming He on 2018/12/28.
//  Copyright © 2018 zhiming he. All rights reserved.
//

#import "HomePageViewController.h"
#import "ViewController.h"

@interface HomePageViewController ()
{
    UIButton* videoBtn;
}
@end

@implementation HomePageViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.title = @"主页";
    self.navigationController.navigationBar.barTintColor = [UIColor orangeColor];
    self.view.backgroundColor = [UIColor whiteColor];
    
    [self setupVideoBtn];
}

- (void)setupVideoBtn {
    videoBtn = [UIButton buttonWithType:UIButtonTypeCustom];
    videoBtn.frame = CGRectMake(self.view.frame.size.width - 80, 90, 60, 30);
    [self.view addSubview:videoBtn];
    videoBtn.backgroundColor = [UIColor orangeColor];
    [videoBtn setTitle:@"Video" forState:UIControlStateNormal];
    videoBtn.titleLabel.font = [UIFont systemFontOfSize:14.0];
    [videoBtn addTarget:self action:@selector(startVideo) forControlEvents:UIControlEventTouchUpInside];
}

- (void)startVideo {
    ViewController* vc = [[ViewController alloc] init];
    [self.navigationController pushViewController:vc animated:YES];
}

@end
