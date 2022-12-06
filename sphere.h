#pragma once
#include "object.h"

#include "Texture.h"

class Sphere :
    public Object
{
public:
    Sphere();
    Sphere(glm::vec3 pivot);
    Sphere(glm::vec3 pivot, float angle);
    Sphere(glm::vec3 pivot, float angle, float scale, float sizeNew, const char* fileName);

    void Render(GLint positionAttribLoc, GLint normalAttribLoc, GLint texAttribLoc, GLint hasTexture);
    glm::mat4 GetModel() { return model; }
    void Update(glm::mat4 matModel);

    float toRadians(float degrees);

    Texture* m_texture;

    bool hasTex;

    GLuint getTextureID() { return m_texture->getTextureID(); }

private:
    glm::vec3 pivotLocation;
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    GLuint vao;

    float angle;

    float size;

    void setupVertices();
    void setupBuffers();
    void setupModelMatrix(glm::vec3 pivotLoc, float angle, float scale);

};

