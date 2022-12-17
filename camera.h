#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void Update();

    void movForward(double camSpeed);
    void movBack(double camSpeed);
    void movLeft(double camSpeed);
    void movRight(double camSpeed);

    void ProcessMouseMovement(double xPosIn, double yPosIn, float fov, double yZoomOffset);

    void moveTest();

    glm::mat4 GetViewSkybox();

    //void mouseUpdate(const glm::vec2& newMousePos);

private:
    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 direction;

    glm::vec3 cameraPos;
    glm::vec3 cameraLookTarget;
    glm::vec3 cameraDirection;
    glm::vec3 Up;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;

    double oldMXPos;
    double oldMYPos;
    float Xoffset;
    float lastX;
    float Yoffset;
    float lastY;
    const float snesitivity = .0000000000001f;

    bool firstFrame = true;

    float yaw;
    float pitch;
    float mouseSen;

    float FOV = 40.f;

    float width;
    float height;

    bool zoomMovement = false;


};

#endif /* CAMERA_H */
