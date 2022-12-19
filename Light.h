#pragma once
#include "graphics_headers.h"
#include "object.h"
#include "Texture.h"
#include <vector>

class Light
{
public:
    Light(glm::mat4 viewMatrix);
    ~Light();
    void Render();

private:

    glm::vec3 pivotLocation;
    glm::mat4 model;
    std::vector<Vertex> lightVertices;
    std::vector<unsigned int> lightIndices;
    GLuint VB;
    GLuint IB;

    GLuint vao;

    float angle;

    float size;

    GLuint m_globalAmbient;
    GLuint m_lightAmbient;
    GLuint m_lightDiffuse;
    glm::vec3 m_lightPosition;
    glm::vec4 m_lightPositionViewSpace;
    GLuint m_lightSpecular;

 };

