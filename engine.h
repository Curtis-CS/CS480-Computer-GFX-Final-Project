#ifndef ENGINE_H
#define ENGINE_H


#include <assert.h>
#include "window.h"
#include "graphics.h"

class Engine
{
public:
    Engine(const char* name, int width, int height);

    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    void Display(GLFWwindow*, double);


    double mXpos;

    double mYpos;

    bool firstFrame = true;
    int counter = 0;

    float yaw;
    float pitch;


    double* xZoomOffset;
    double* yZoomOffset;

    Window* m_window;
    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;


    Graphics* m_graphics;

    int timer = 0;

private:

    bool m_running;

    const float cameraSpeed = 0.005f;

};

#endif // ENGINE_H
