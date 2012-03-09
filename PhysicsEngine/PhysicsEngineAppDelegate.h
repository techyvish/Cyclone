//
//  PhysicsEngineAppDelegate.h
//  PhysicsEngine
//
//  Created by Vishal Patel on 9/06/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class  GLView;
@interface PhysicsEngineAppDelegate : NSObject <UIApplicationDelegate> {

    GLView  *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end
