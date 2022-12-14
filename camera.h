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
    void movUp(double camSpeed);
    void movDown(double camSpeed);

    void movForBack(double camVel);
    void movLeftRight(double camVel);
    void movUpDown(double camVel);

    void ProcessMouseMovement(double xPosIn, double yPosIn, float fov, double yZoomOffset);

    void moveTest();

    float forBackVel =0;
    float leftRightVel = 0;
    float upDownVel = 0;

    glm::vec3 worldUp;

    glm::vec3 front;

    glm::mat4 GetViewSkybox();

    glm::vec3 cameraPos;
    glm::vec3 cameraLookTarget;
    glm::vec3 cameraDirection;
    glm::vec3 Up;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::vec3 cameraFront;

    //void mouseUpdate(const glm::vec2& newMousePos);

private:
    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 direction;



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
