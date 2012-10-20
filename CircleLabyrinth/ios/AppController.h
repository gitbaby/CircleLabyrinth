//
//  CircleLabyrinthAppController.h
//  CircleLabyrinth
//
//  Created by Oleksii Kozlov on 09.12.11.
//  Copyright 2012 Oleksii Kozlov. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController	*viewController;
}

@end

