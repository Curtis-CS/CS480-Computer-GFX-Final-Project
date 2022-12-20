
#include "engine.h"

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float fov = 40.f;
double yZoomOffset;

Engine::Engine(const char* name, int width, int height)
{
    m_WINDOW_NAME = name;
    m_WINDOW_WIDTH = width;
    m_WINDOW_HEIGHT = height;

}


Engine::~Engine()
{
    delete m_window;
    delete m_graphics;
    m_window = NULL;
    m_graphics = NULL;
}

bool Engine::Initialize()
{
    // Start a window
    m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
    if (!m_window->Initialize())
    {
        printf("The window failed to initialize.\n");
        return false;
    }

    // Start the graphics
    m_graphics = new Graphics();
    if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
    {
        printf("The graphics failed to initialize.\n");
        return false;
    }


    glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetScrollCallback(m_window->getWindow(), scroll_callback);



    // No errors
    return true;
}

void Engine::Run()
{
    m_running = true;

    while (!glfwWindowShouldClose(m_window->getWindow()))
    {
        ProcessInput();
        Display(m_window->getWindow(), glfwGetTime());
        glfwPollEvents();
    }
    m_running = false;

}

void Engine::ProcessInput()
{

    timer = timer + 1;

    float dt = getDT();
    float camSpeed = .02f * dt;

    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);



    //Initial movement of camera to position the user at a nice distance
    if (counter < 10)
    {
        m_graphics->getCamera()->movBack(58);
        m_graphics->getCamera()->movUp(3);
    }
    if (m_graphics->gameMode == false)
    {
        //Movement of camera/spaceship with keys, R and F to go down
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        {
            m_graphics->getCamera()->forBackVel = m_graphics->getCamera()->forBackVel + cameraSpeed;
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        {
            m_graphics->getCamera()->forBackVel = m_graphics->getCamera()->forBackVel - cameraSpeed;
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        {
            m_graphics->getCamera()->leftRightVel = m_graphics->getCamera()->leftRightVel - cameraSpeed;
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        {
            m_graphics->getCamera()->leftRightVel = m_graphics->getCamera()->leftRightVel + cameraSpeed;
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_R) == GLFW_PRESS)
        {
            m_graphics->getCamera()->upDownVel = m_graphics->getCamera()->upDownVel + cameraSpeed;
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_F) == GLFW_PRESS)
        {
            m_graphics->getCamera()->upDownVel = m_graphics->getCamera()->upDownVel - cameraSpeed;
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            m_graphics->getCamera()->upDownVel = 0;
            m_graphics->getCamera()->leftRightVel = 0;
            m_graphics->getCamera()->forBackVel = 0;
        }
    }
    else
    {
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        {
            m_graphics->getCamera()->movForward(camSpeed);
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        {
            m_graphics->getCamera()->movBack(camSpeed);
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        {
            m_graphics->getCamera()->movLeft(camSpeed);
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        {
            m_graphics->getCamera()->movRight(camSpeed);
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_R) == GLFW_PRESS)
        {
            m_graphics->getCamera()->movUp(camSpeed);
        }

        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_F) == GLFW_PRESS)
        {
            m_graphics->getCamera()->movDown(camSpeed);
        }
        if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            m_graphics->getCamera()->upDownVel = 0;
            m_graphics->getCamera()->leftRightVel = 0;
            m_graphics->getCamera()->forBackVel = 0;
        }
    }

    //Our 'gamemode' key, with the timer to make sure us holding the button longer doesn't swap it constatnly
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_E) == GLFW_PRESS)
    {
        if (timer > 30)
        {
            m_graphics->getCamera()->upDownVel = 0;
            m_graphics->getCamera()->leftRightVel = 0;
            m_graphics->getCamera()->forBackVel = 0;
            //Swap gamemode
            m_graphics->gameMode = !m_graphics->gameMode;
            timer = 0;
        }
    }

    //Our method of getting and tracking mouse movement
    glfwGetCursorPos(m_window->getWindow(), &mXpos, &mYpos);
    //Our method of detecting mouse scroll whell activity to zoom in and out
    //If the user hasn't zoomed at all yet do this
    if (yZoomOffset == nullptr)
    {
        m_graphics->getCamera()->ProcessMouseMovement(mXpos, mYpos, fov, 0);
    }
    else
    {
        m_graphics->getCamera()->ProcessMouseMovement(mXpos, mYpos, fov, *yZoomOffset);
    }

    counter = counter + 1;

}

//Get DT
unsigned int Engine::getDT()
{
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
    return (long long)glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();

    // update graphics -> updates model
    //m_graphics->Update(time);
    m_graphics->HierarchicalUpdate2(time);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    yZoomOffset = yoffset;
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}