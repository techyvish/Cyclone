// Physical orientation of a handheld device, equivalent to UIDeviceOrientation
#include "Vectors.cpp"

enum DeviceOrientation {
    DeviceOrientationUnknown,
    DeviceOrientationPortrait,
    DeviceOrientationPortraitUpsideDown,
    DeviceOrientationLandscapeLeft,
    DeviceOrientationLandscapeRight,
    DeviceOrientationFaceUp,
    DeviceOrientationFaceDown,
};

// Create an instance of the renderer and set up various OpenGL state.
struct IRenderingEngine* CreateRenderer1();
//struct IRenderingEngine* CreateRenderer2();

// Interface to the OpenGL ES renderer; consumed by GLView.
struct IRenderingEngine {
    virtual void Initialize(int width, int height) = 0;    
    virtual void Render()   = 0;
    virtual void UpdateAnimation(float timeStep) = 0;
    virtual void OnRotate(DeviceOrientation newOrientation) = 0;
    virtual void InputShaders(const char* vShader, const char* fshader) {}
    virtual ~IRenderingEngine() {}
    virtual void OnFingerUp(ivec2 location) {}
    virtual void OnFingerDown(ivec2 location) {}
    virtual void OnFingerMove(ivec2 oldLocatioin, ivec2 newLocation) {}
};
