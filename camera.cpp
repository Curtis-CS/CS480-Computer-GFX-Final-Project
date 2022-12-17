#include "camera.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{

    width = w;
    height = h;

    cameraPos = glm::vec3(0.f, 3.f, -20.f);
    cameraFront = glm::vec3(0.f, 0.f, 1.f);
    Up = glm::vec3(0.f, 1.f, 0.f);

    //direction.x = cos(glm::radians(yaw));

    cameraLookTarget = glm::vec3(0.f, 0.f, 0.f);
    cameraDirection = glm::normalize(cameraPos - cameraLookTarget);

    cameraRight = glm::normalize(glm::cross(Up, cameraDirection));
    //cameraUp = glm::cross(cameraDirection, cameraRight);

    yaw = YAW;
    pitch = PITCH;
    mouseSen = SENSITIVITY;

    //--Init the view and projection matrices
    //  if you will be having a moving camera the view matrix will need to more dynamic
    //  ...Like you should update it before you render more dynamic 
    //  for this project having them static will be fine
    view = glm::lookAt(cameraPos, //Eye Position
        cameraPos + cameraFront, //Focus point
        Up); //Positive Y is up

    FOV = 40.f;

    projection = glm::perspective(glm::radians(FOV), //the FoV typically 90 degrees is good which is what this is set to
        float(w) / float(h), //Aspect Ratio, so Circles stay Circular
        0.01f, //Distance to the near plane, normally a small value like this
        512.0f); //Distance to the far plane, 
    return true;
}

glm::mat4 Camera::GetProjection()
{
    return projection;
}

glm::mat4 Camera::GetView()
{
    return view;
}

void Camera::Update()
{
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;


    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    // also re-calculate the Right and Up vector
    cameraRight = glm::normalize(glm::cross(cameraFront, Up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(cameraRight, cameraFront));

    view = glm::lookAt(cameraPos, //Eye Position
        cameraPos + cameraFront, //Focus point
        Up); //Positive Y is up
    projection = glm::perspective(glm::radians(FOV), float(width) / float(height), .01f, 100.0f);
}


void Camera::movForward(double camSpeed)
{
    cameraPos += (float)camSpeed * cameraFront;
}
void Camera::movBack(double camSpeed)
{
    cameraPos -= (float)camSpeed * cameraFront;
}
void Camera::movLeft(double camSpeed)
{
    cameraPos -= glm::normalize(glm::cross(cameraFront, Up)) * (float)camSpeed;
}
void Camera::movRight(double camSpeed)
{
    cameraPos += glm::normalize(glm::cross(cameraFront, Up)) * (float)camSpeed;
}

void Camera::moveTest()
{
    const float radius = 10.0f;
    float camX = sin(glfwGetTime()) * radius;
    float camZ = cos(glfwGetTime()) * radius;
    view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
}

void Camera::ProcessMouseMovement(double xPosIn, double yPosIn, float fov, double yZoomOffset)
{

    oldMXPos = xPosIn;
    oldMYPos = yPosIn;

    if (firstFrame)
    {
        lastX = oldMXPos;
        lastY = oldMYPos;
        firstFrame = false;
    }
    Xoffset = xPosIn - lastX;
    Yoffset = lastY - yPosIn;
    lastX = xPosIn;
    lastY = yPosIn;
    Xoffset *= mouseSen;
    Yoffset *= mouseSen;

    yaw += Xoffset;
    pitch += Yoffset;

    FOV = fov;
    //std::cout << FOV << std::endl;
    if (FOV == 40 && zoomMovement==false)
    {
        
    }
    else
    {
        zoomMovement = true;
        FOV -= (float)yZoomOffset;
        if (FOV < 1.0f)
        {
            FOV = 1.0f;
        }
        if (FOV > 45.0f)
        {
            FOV = 45.0f;
        }
    }
    //std::cout << FOV << std::endl;
}

glm::mat4 Camera::GetViewSkybox()
{
    glm::mat4 skyboxView = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, Up)));
    return skyboxView;
}




