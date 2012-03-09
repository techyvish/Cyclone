//
//  RenderingEngine1.cpp
//  HelloArrpwTest
//
//  Created by Vishal Patel on 1/06/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
// test 


#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "IRenderingEngine.h"
#include <vector>
#include "Quaternion.cpp"
#include "Geometry.hpp"


#define USE_DEPTH_BUFFER 1
#define DEGREES_TO_RADIANS(__ANGLE) ((__ANGLE) / 180.0 * M_PI)


static const float RevolutionsPerSecond = 1;
static const float AnimationDuration = 0.25f;

using namespace std;

struct Vertex {
  
    vec3 Position; 
    vec4 Color;
    
};



struct Animation {
    
    Quaternion start;
    Quaternion end;
    Quaternion current;
    float Elapsed;
    float Duration;
    
};


class RenderingEngine1 : public IRenderingEngine {
public:
    
    //static IRenderingEngine *CreateRenderer1() { return  new RenderingEngine1(); }
    RenderingEngine1();
    void Initialize(int width, int height);
    void Render()  ;
    void UpdateAnimation(float timeStep);
    void OnRotate(DeviceOrientation newOrientation);
    vector<Vertex> m_cone;
    vector<Vertex> m_disk;
    void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
                                     GLfloat centerx, GLfloat centery, GLfloat centerz,
                   GLfloat upx, GLfloat upy, GLfloat upz);
    void gluPerspective(double fovy, double aspect, double zNear , double zFar);
    void setUpFrustum(int width, int height);
    void OnFingereUp(ivec2 location);
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocatioin, ivec2 newLocation);
    vec3 MapToSphere(ivec2 touchpoint) const;
    void fire();
        

    
private:
    float RotationDirection() const;
    float m_desiredAngle;
    float m_currentAngle;
    GLuint m_framebuffer;
    GLuint m_ColorRenderBuffer;
    GLuint m_depthBuffer;
    
    GLfloat m_rotationAngle;
    GLfloat m_scale;
    ivec2 m_pivotePoint;
    
    ivec2 m_fingerStart;
    bool m_spinning;
    Quaternion m_orientation;
    Quaternion m_previousOrientation;
    float m_trackballRadius;
    
    mat4 m_translation;
    
    
    //float theta;
    float x;
    float z;
    
    float xd;
    float zd;
    
};

IRenderingEngine* CreateRenderer1()
{
    return new RenderingEngine1();
}

RenderingEngine1::RenderingEngine1(): m_rotationAngle(0), m_scale(1)
{
    // Create & bind the color buffer so that the caller can allocate its space.
    glGenRenderbuffersOES(1, &m_ColorRenderBuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_ColorRenderBuffer);
}

//GLfloat LightPosition[]= { -2.0f, 2.0f, 0.0f, 1.0f };				 // Light Position ( NEW )

static GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0};
GLfloat mShininess[] = {128}; //set the shininess of the 

GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0}; //set the  light specular to white
GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0}; //set the  light ambient to black
GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0}; //set the  diffuse light to white
GLfloat blankMaterial[] = {0.0, 0.0, 0.0}; //set the diffuse light to white

void RenderingEngine1::Initialize(int width, int height)
{
    
    
    m_pivotePoint = ivec2 ( width/2 ,height/2 );
    m_orientation = Quaternion();
    
    m_trackballRadius = width / 3 ;
    
    // Create and bind Depth buffer.
    glGenRenderbuffersOES( 1 , & m_depthBuffer );
    glBindRenderbufferOES( GL_RENDERBUFFER_OES ,  m_depthBuffer);
    glRenderbufferStorageOES( GL_RENDERBUFFER_OES , GL_DEPTH_COMPONENT16_OES , width , height );
    
    // Create the framebuffer object and attach the color buffer.
    glGenFramebuffersOES(1, &m_framebuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffer);
    glFramebufferRenderbufferOES( GL_FRAMEBUFFER_OES,
                                  GL_COLOR_ATTACHMENT0_OES,
                                  GL_RENDERBUFFER_OES,
                                  m_ColorRenderBuffer);
    
    glFramebufferRenderbufferOES( GL_FRAMEBUFFER_OES , 
                                  GL_DEPTH_ATTACHMENT_OES,  
                                  GL_RENDERBUFFER_OES ,  
                                  m_depthBuffer );
    
    glBindRenderbufferOES( GL_RENDERBUFFER_OES , m_ColorRenderBuffer );
    
    
    { // corret way to show
        // Set the projection mode, set the Frustum a view Cone.
        glViewport(0, 0, width, height);
        glEnable(GL_DEPTH_TEST );
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        setUpFrustum(width, height);
        
        
        // Set the Model View
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(-6.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f , 1.0f, 0.0f);
        
    }
    
    
//    {
//        glViewport(0, 0,width,height);
//        glEnable(GL_DEPTH_TEST );
//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
//        gluPerspective(60, (GLfloat)width/height, 0.5f, 1500.0f);
//        
//        //setUpFrustum(height, width);
//        
//        glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
//        glTranslatef(0, 0, -7.0);
//        
////        gluLookAt(-6.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f , 1.0f, 0.0f);
////        gluLookAt( width/2, height/2, height / 1.1566f,
////                   width/2, height/2, 0,
////                   0.0f, 1.0f, 0.0f);			
//
//        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
//      //  glTranslatef(0.0f, -320.0f, 0.0f);
//
//    }
    
     m_translation = mat4::Translate(0, 0, -7);
}
struct _Point {
    double x;
    double y;
};
typedef struct _Point Point1;

/* Sizes. */


struct _Size {
    double width;
    double height;
};
typedef struct _Size Size1 ;

/* Rectangles. */

struct _Rect {
    Point1 origin;
    Size1  size;
};
typedef struct _Rect Rect1;

Rect1 RectMake ( double x, double y, double width ,double height ) {
    
    Rect1 r;
    
    r.origin.x = x;
    r.origin.y = y;
    r.size.width = width;
    r.size.height = height;
 
    return  r;
}

void RenderingEngine1::setUpFrustum(int width, int height) {
    const GLfloat zNear = 0.1, zFar = 1000.0, fieldOfView = 60.0;
    GLfloat size;
	
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    size = zNear * tanf(DEGREES_TO_RADIANS(fieldOfView) / 2.0);
	
	// This give us the size of the iPhone display
    Rect1 rect = RectMake( 0 , 0 , width, height );
    glFrustumf(-size , size, -size / (rect.size.width / rect.size.height), size / (rect.size.width / rect.size.height), zNear, zFar);
    
    glViewport(0, 0, rect.size.width, rect.size.height);

}


struct VertexPoint {
    
    float Position[3];
  

};

static float rota = 0.0f;
// Define the positions and colors of two triangles.


static const GLubyte triVertexColors[] = { 
    255,   0,   0, 255,  
    0,   255,   0, 255,  
    0,     0, 255, 255,  
    0,   255,   0, 255,  
    255,   0,   0, 255,
    0,   255,   0, 255,
    0,     0, 255, 255,
    255,   0,   0, 255 
};



void RenderingEngine1::Render()  {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor( 0.7, 0.7, 0.7, 1.0 );
    
    glPopMatrix();
    {
        
        glEnableClientState(GL_COLOR_ARRAY); 
        glColorPointer( 4 , GL_UNSIGNED_BYTE , 0, triVertexColors );
        
        /**
         * Conversion of matrix3 to matrix4 using homogenious coordinates.
         *
         */
        mat4 rotation = m_orientation.ToMatrix();
        mat4 modelview = rotation * m_translation ;
            //glMatrixMode(GL_MODELVIEW); // Discard changes by glLookAt(..);
        glLoadMatrixf(modelview.Pointer());
            //glMultMatrixf(modelview.Pointer());
       
        
        glutSolidBox(3.0, 2.0, 2.0);
        glDisableClientState(GL_COLOR_ARRAY);
    } 
    
   
    
     
//    glPopMatrix();
//    {
//        
//        glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
//        glTranslatef(1.6, 1.6, -7.0);
//        glColor4f(1.0, 0.0, 0.0, 1.0);
//        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, whiteSpecularMaterial);
//        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mShininess);
//        glutSolidSphere(2.0, 40 , 16);
//        //glDisableClientState(GL_COLOR_ARRAY);
//    }
//    glPushMatrix();
    
        
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //vec4 rotation = m_orientation.ToVector();
    //gluLookAt(rotation.x, rotation.y, rotation.z, 0.0f, 0.0f, 0.0f, 0.0f , 1.0f, 0.0f);
    
}

vec3 RenderingEngine1::MapToSphere(ivec2 touchpoint) const
{
    vec2 p = touchpoint - m_pivotePoint;
    
    // Flip the Y axis because pixel coords increase towards the bottom.
    p.y = -p.y;
    
    const float radius = m_trackballRadius;
    const float safeRadius = radius - 1;
    
    if (p.Length() > safeRadius) {
        float theta = atan2(p.y, p.x);
        p.x = safeRadius * cos(theta);
        p.y = safeRadius * sin(theta);
    }
    
    float z = sqrt(radius * radius - p.LengthSqured());
    vec3 mapped = vec3(p.x, p.y, z);
    return mapped / radius;
}


float RenderingEngine1::RotationDirection() const
{
    float delta = m_desiredAngle - m_currentAngle;
    if (delta == 0)
        return 0;
    
    bool counterclockwise = ((delta > 0 && delta <= 180) || (delta < -180));
    return counterclockwise ? +1 : -1;
}

void RenderingEngine1::UpdateAnimation(float timeStep)
{
    
    /*
    float direction = RotationDirection();
    if (direction == 0)
        return;
    
    float degrees = timeStep * 360 * RevolutionsPerSecond;
    m_currentAngle += degrees * direction;
    
    // Normalize the angle to [0, 360)
    if (m_currentAngle >= 360)
        m_currentAngle -= 360;
    else if (m_currentAngle < 0)
        m_currentAngle += 360;
    
    // If the rotation direction changed, then we overshot the desired angle.
    if (RotationDirection() != direction)
        m_currentAngle = m_desiredAngle;
     */
}

void RenderingEngine1::OnRotate(DeviceOrientation orientation)
{
    float angle = 0;
    
    switch (orientation) {
        case DeviceOrientationLandscapeLeft:
        {
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glTranslatef(0.0f, -320.0f, 0.0f);
        

        }
            angle = 270;
            break;
            
        case DeviceOrientationPortraitUpsideDown:
            angle = 180;
            break;
            
        case DeviceOrientationLandscapeRight:
            angle = 90;
            break;
    }
    
    m_desiredAngle = angle;
}


void RenderingEngine1::gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
                                 GLfloat centerx, GLfloat centery, GLfloat centerz,
                                 GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    
    /* Make rotation matrix */
    
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {          /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
    
    /* X vector = Y cross Z */
    x[0] =  y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] =  y[0] * z[1] - y[1] * z[0];
    
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    
    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    
    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
    
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    glMultMatrixf(m);
    
    /* Translate Eye to Origin */
    glTranslatef(-eyex, -eyey, -eyez);
    
    //GLint locationUniform = glGetUniformLocation(m_simpleProgram, "Position");
    //glUniformMatrix4fv(locationUniform, 1, 0, &m[0]);
    
}

void RenderingEngine1::gluPerspective(double fovy, double aspect, double zNear , double zFar)
{
    // Start in projection mode.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double xmin, xmax, ymin, ymax;
    ymax = zNear * tan(fovy * M_PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;
    glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}



void RenderingEngine1::OnFingereUp(ivec2 location)  {
 
    m_scale = 1.0f ;
}


void RenderingEngine1::OnFingerDown(ivec2 location) {

    m_fingerStart = location;
    m_previousOrientation = m_orientation;

}


void RenderingEngine1::OnFingerMove(ivec2 previous , ivec2 location){
    
    
    vec3 start = MapToSphere(m_fingerStart); // hummm.... whats MapToSphere ?? need some testing....
    vec3 end = MapToSphere(location);
    Quaternion delta = Quaternion::CreateFromVectors(start, end);
    m_orientation = delta.Rotated(m_previousOrientation);
    
}

