//
//  GLView.m
//  HelloArrpwTest
//
//  Created by Vishal Patel on 31/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "GLView.h"
#import "RenderingEngine1.h"
#import <OpenGLES/ES2/gl.h> // <-- for GL_RENDERBUFFER only

const bool ForceES1 = true ;

@implementation GLView

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (id) initWithFrame: (CGRect) frame
{
    if ( (self = [super initWithFrame:frame]) ) {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*) super.layer;
        eaglLayer.opaque = YES;
        
        
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        m_context = [[EAGLContext alloc] initWithAPI:api];
        
        if (!m_context || ForceES1) {
            api = kEAGLRenderingAPIOpenGLES1;
            m_context = [[EAGLContext alloc] initWithAPI:api];
        }
        
        if (!m_context || ![EAGLContext setCurrentContext:m_context]) {
            [self release];
            return nil;
        }
        
        if (api == kEAGLRenderingAPIOpenGLES1) {
            NSLog(@"Using OpenGL ES 1.1");
            m_renderingEngine = CreateRenderer1();
        } else {
            NSLog(@"Using OpenGL ES 2.0");
            
            // 1
            NSString* shaderPath = [[NSBundle mainBundle] pathForResource:@"SimpleVert" 
                                                                   ofType:@"glsl"];
            NSError* error;
            NSString* shaderString = [NSString stringWithContentsOfFile:shaderPath 
                                                               encoding:NSUTF8StringEncoding error:&error];
            const char* vShader = [shaderString UTF8String];
            if (!shaderString) {
                NSLog(@"Error loading shader: %@", error.localizedDescription);
                exit(1);
            }
            
            
            shaderPath = [[NSBundle mainBundle] pathForResource:@"Simplefrag" 
                                                         ofType:@"glsl"];
            shaderString = [NSString stringWithContentsOfFile:shaderPath 
                                                        encoding:NSUTF8StringEncoding error:&error];

            const char* fShader = [shaderString UTF8String];
            if (!shaderString) {
                NSLog(@"Error loading shader: %@", error.localizedDescription);
                exit(1);
            }
            
           //m_renderingEngine = CreateRenderer2();
           m_renderingEngine->InputShaders(vShader, fShader);
        
        }
        
        [m_context renderbufferStorage:GL_RENDERBUFFER 
                          fromDrawable: eaglLayer];
        
        m_renderingEngine->Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame));
        
        [self drawView: nil];
        m_timestamp = CACurrentMediaTime();
        
        CADisplayLink* displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self
                                                  selector:@selector(drawView:)];
        
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop]
                          forMode:NSDefaultRunLoopMode];
        
        [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
        
        [[NSNotificationCenter defaultCenter]
         addObserver:self
         selector:@selector(didRotate:)
         name:UIDeviceOrientationDidChangeNotification
         object:nil];
    }
    return self;
}

- (void) didRotate: (NSNotification*) notification
{
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    m_renderingEngine->OnRotate((DeviceOrientation) orientation);
    [self drawView: nil];
}

- (void) drawView: (CADisplayLink*) displayLink
{
    if (displayLink != nil) {
        float elapsedSeconds = displayLink.timestamp - m_timestamp;
        m_timestamp = displayLink.timestamp;
        NSLog(@"%f",elapsedSeconds );
         m_renderingEngine->UpdateAnimation(elapsedSeconds);
    }
    
    m_renderingEngine->Render();
    [m_context presentRenderbuffer:GL_RENDERBUFFER];
}


-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    
    UITouch* touch = [touches anyObject];
    CGPoint location = [touch locationInView:self];
    m_renderingEngine->OnFingerUp(ivec2( location.x, location.y ));
}

-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    
    UITouch* touch = [touches anyObject];
    CGPoint previous = [touch previousLocationInView:self];
    CGPoint current  = [touch locationInView:self];
    m_renderingEngine->OnFingerMove( ivec2( previous.x, previous.y ),
                                      ivec2( current.x , current.y) );
    
}

-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    
    UITouch* touch = [touches anyObject];
    CGPoint location = [touch locationInView:self];
    m_renderingEngine->OnFingerDown(ivec2( location.x, location.y ));
}

@end
