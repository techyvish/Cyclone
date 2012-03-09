//
//  GLView.h
//  HelloArrpwTest
//
//  Created by Vishal Patel on 31/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "IRenderingEngine.h"


@interface GLView : UIView {
 
    
    EAGLContext *m_context;
    
    IRenderingEngine *m_renderingEngine;
    
    float m_timestamp;
    
}


-(void) drawView : (CADisplayLink*) displayLink; 

-(void) didRotate: (NSNotification*) notification;

@end
