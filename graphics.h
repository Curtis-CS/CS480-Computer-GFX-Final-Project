#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "shader2.h"

#include "stb_image.h"

//#include "sphere2.h"
//#include "cube.h"

#include "mesh.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void HierarchicalUpdate2(double dt);
    void Render();

    Camera* getCamera() { return m_camera; }

    unsigned int loadCubemap(vector<std::string> faces);

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;

    //Cube* m_cube;

    Sphere* m_sphere_sun;
    Sphere* m_sphere_planet;
    Sphere* m_sphere_moon;

    Mesh* m_mesh;

    Shader2* skyboxShader;
    unsigned int skyboxVAO;
    unsigned int skyboxVBO;
    unsigned int cubemapTexture;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;


};

#endif /* GRAPHICS_H */
