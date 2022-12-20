#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "object.h"
#include "sphere.h"
#include "shader2.h"

#include "stb_image.h"

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

    //Function to load up the skybox
    unsigned int loadCubemap(vector<std::string> faces);

    //This is where we would have our instancing, however unfortunately it is just a bunch of renders
    void updateAsteroidBelt(double dt);
    void renderAsteroidBelt();
    void updateKuiperBelt(double dt);
    void renderKuiperBelt();

    //Functions to make code easier to manage
    void initSkybox();
    void initLighting();
    void initObjects();
    void initShaders();

    //Special transform for spaceship, similar to local transform
    glm::mat4 spaceshipTransform;

    std::vector<float> speed, dist, rotSpeed, scale;
    glm::vec3 rotVector;
    glm::mat4 localTransform;
    float multiplier = 4;
    float multiplierD = 12;
    float distM;
    float scaler = .005;

    bool gameMode = false;

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;

    Camera *m_camera;
    Shader2* m_shader2;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_normalAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;


    //All of our objects
    Sphere* m_sun;
    
    Sphere* m_mercury;
    void renderMercury();
    void updateMercury(double dt);


    
    Sphere* m_venus;
    void updateVenus(double dt);
    void renderVenus();


    Sphere* m_earth;
    Sphere* m_earth_moon;
    void renderEarth();
    void updateEarth(double dt);


    Sphere* m_mars;
    Sphere* m_mars_moon1;
    Sphere* m_mars_moon2;
    void renderMars();
    void updateMars(double dt);


    //Astroid Belt, unfortunately no time for instancing. I am sorry
    Sphere* m_ceres1;
    Sphere* m_ceres2;
    Sphere* m_ceres3;
    Sphere* m_ceres4;
    Sphere* m_ceres5;
    Sphere* m_ceres6;
    Sphere* m_ceres7;
    Sphere* m_ceres8;
    Sphere* m_ceres9;
    Sphere* m_ceres10;
    Sphere* m_ceres11;
    Sphere* m_ceres12;
    Sphere* m_ceres13;
    Sphere* m_ceres14;
    Sphere* m_ceres15;
    Sphere* m_ceres16;
    Sphere* m_ceres17;
    Sphere* m_ceres18;
    Sphere* m_ceres19;
    Sphere* m_ceres20;

    Sphere* m_jupiter;
    Sphere* m_jupiter_moon1;
    Sphere* m_jupiter_moon2;
    Sphere* m_jupiter_moon3;
    void renderJupiter();
    void updateJupiter(double dt);


    Sphere* m_saturn;
    Mesh* m_saturn_ring;
    Sphere* m_saturn_moon1;
    Sphere* m_saturn_moon2;
    void renderSaturn();
    void updateSaturn(double dt);


    Sphere* m_uranus;
    Mesh* m_uranus_ring;
    void renderUranus();
    void updateUranus(double dt);


    Sphere* m_neptune;
    void renderNeptune();
    void updateNeptune(double dt);


    Sphere* m_haley_comet;
    void renderComet();
    void updateComet(double dt);


    //Kuiper belt, again sorry no instancing
    Sphere* m_kuiper1;
    Sphere* m_kuiper2;
    Sphere* m_kuiper3;
    Sphere* m_kuiper4;
    Sphere* m_kuiper5;
    Sphere* m_kuiper6;
    Sphere* m_kuiper7;
    Sphere* m_kuiper8;
    Sphere* m_kuiper9;
    Sphere* m_kuiper10;
    Sphere* m_kuiper11;
    Sphere* m_kuiper12;
    Sphere* m_kuiper13;
    Sphere* m_kuiper14;
    Sphere* m_kuiper15;
    Sphere* m_kuiper16;
    Sphere* m_kuiper17;
    Sphere* m_kuiper18;
    Sphere* m_kuiper19;
    Sphere* m_kuiper20;


    //Starship
    Mesh* m_mesh;

    //Skybox
    Shader2* skyboxShader;
    unsigned int skyboxVAO;
    unsigned int skyboxVBO;
    unsigned int cubemapTexture;

    //Lighting
    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int lightVBO;
    Shader2* lightingShader;
    Shader2* lightCubeShader;
    glm::vec3 lightPos;
    glm::vec3 lightColor;

    unsigned int diffuseMapTestCube;
    unsigned int specularMapTestCube;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    bool firstFrame;

    unsigned int loadTexture(char const* path);

};

#endif /* GRAPHICS_H */
