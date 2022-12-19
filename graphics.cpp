#include "graphics.h"



Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif

	firstFrame = true;

	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Set up the shaders
	initShaders();

	//Skybox Stuff
	initSkybox();

	//Lighting
	initLighting();

	//Objects
	initObjects();

	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}

	spaceshipTransform = glm::mat4(1.0f);

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::initShaders()
{
	m_shader2 = new Shader2("assets\\Shaders\\base.vs", "assets\\Shaders\\base.fs");
	m_shader2->use();
	m_shader2->setInt("shader", 0);

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}
}

void Graphics::initObjects()
{
	// Starship
	m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\Objects\\SpaceShip-1.obj", "assets\\Objects\\SpaceShip-1.png");

	// The Sun
	m_sun = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 200, 1.5, 64, "assets\\PlanetaryTextures\\2k_sun.jpg");

	//Mercury
	m_mercury = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Mercury.jpg");

	//Venus
	m_venus = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Venus.jpg");

	// The Earth
	m_earth = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\2k_earth_daymap.jpg");

	// The earth moon
	m_earth_moon = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\2k_moon.jpg");

	//mars
	m_mars = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Mars.jpg");

	m_mars_moon1 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\2k_moon.jpg");
	m_mars_moon2 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Mars.jpg");

	//Astroid Belt
	m_ceres1 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres2 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres3 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres4 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres5 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres6 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres7 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres8 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres9 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres10 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres11 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres12 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres13 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres14 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres15 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres16 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres17 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres18 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres19 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");
	m_ceres20 = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Ceres.jpg");

	//Jupiter
	m_jupiter = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Jupiter.jpg");

	// Saturn
	m_saturn = new Sphere(glm::vec3(-3.f, -2.f, 100.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Saturn.jpg");
	m_saturn_ring = new Mesh(glm::vec3(0.0f, -3.0f, 0.0f), "assets\\Objects\\Ring.obj", "assets\\Objects\\Ring.png");

	m_saturn_moon1 = new Sphere(glm::vec3(-3.f, -2.f, 100.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\2k_moon.jpg");
	m_saturn_moon2 = new Sphere(glm::vec3(-3.f, -2.f, 100.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Jupiter.jpg");
	//Saturn Ring


	//Uranus
	m_uranus = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Uranus.jpg");
	m_uranus_ring = new Mesh(glm::vec3(0.0f, -3.0f, 0.0f), "assets\\Objects\\Ring.obj", "assets\\Objects\\Ring.png");

	//Neptune
	m_neptune = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Neptune.jpg");
}

void Graphics::initSkybox()
{
	skyboxShader = new Shader2("assets\\Shaders\\skybox.vs", "assets\\Shaders\\skybox.fs");

	float skyboxVertices[] = {
		// positions          
		-1024.0f,  1024.0f, -1024.0f,
		-1024.0f, -1024.0f, -1024.0f,
		 1024.0f, -1024.0f, -1024.0f,
		 1024.0f, -1024.0f, -1024.0f,
		 1024.0f,  1024.0f, -1024.0f,
		-1024.0f,  1024.0f, -1024.0f,

		-1024.0f, -1024.0f,  1024.0f,
		-1024.0f, -1024.0f, -1024.0f,
		-1024.0f,  1024.0f, -1024.0f,
		-1024.0f,  1024.0f, -1024.0f,
		-1024.0f,  1024.0f,  1024.0f,
		-1024.0f, -1024.0f,  1024.0f,

		 1024.0f, -1024.0f, -1024.0f,
		 1024.0f, -1024.0f,  1024.0f,
		 1024.0f,  1024.0f,  1024.0f,
		 1024.0f,  1024.0f,  1024.0f,
		 1024.0f,  1024.0f, -1024.0f,
		 1024.0f, -1024.0f, -1024.0f,

		-1024.0f, -1024.0f,  1024.0f,
		-1024.0f,  1024.0f,  1024.0f,
		 1024.0f,  1024.0f,  1024.0f,
		 1024.0f,  1024.0f,  1024.0f,
		 1024.0f, -1024.0f,  1024.0f,
		-1024.0f, -1024.0f,  1024.0f,

		-1024.0f,  1024.0f, -1024.0f,
		 1024.0f,  1024.0f, -1024.0f,
		 1024.0f,  1024.0f,  1024.0f,
		 1024.0f,  1024.0f,  1024.0f,
		-1024.0f,  1024.0f,  1024.0f,
		-1024.0f,  1024.0f, -1024.0f,

		-1024.0f, -1024.0f, -1024.0f,
		-1024.0f, -1024.0f,  1024.0f,
		 1024.0f, -1024.0f, -1024.0f,
		 1024.0f, -1024.0f, -1024.0f,
		-1024.0f, -1024.0f,  1024.0f,
		 1024.0f, -1024.0f,  1024.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces =
	{
		"assets\\Cubemaps\\right.png",
		"assets\\Cubemaps\\left.png",
		"assets\\Cubemaps\\top.png",
		"assets\\Cubemaps\\bottom.png",
		"assets\\Cubemaps\\front.png",
		"assets\\Cubemaps\\back.png"
	};


	cubemapTexture = loadCubemap(faces);

	skyboxShader->use();
	skyboxShader->setInt("skybox", 0);
}

void Graphics::initLighting()
{
	float lightVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &lightVBO);

	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	lightPos = { 1.2f, 1.0f, 2.0f };

	lightingShader = new Shader2("assets\\Shaders\\lightMap1.vs", "assets\\Shaders\\lightMap1.fs");
	lightCubeShader = new Shader2("assets\\Shaders\\lightCube.vs", "assets\\Shaders\\lightCube.fs");

	diffuseMapTestCube = loadTexture("assets\\Objects\\container2.png");
	specularMapTestCube = loadTexture("assets\\Objects\\container2_specular.png");

	lightingShader->use();
	lightingShader->setInt("material.diffuse", 0);
	lightingShader->setInt("material.specular", 1);
}

void Graphics::HierarchicalUpdate2(double dt) 
{

	

	getCamera()->Update();
	//Initial jump start for planets
	if (firstFrame = true)
	{
		dt = dt +7000;
	}



	//The Starship
	speed = { 0., 0., 0. };
	dist = { .0, .0, .1 };
	rotVector = { 0.,0.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .005;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	spaceshipTransform = glm::mat4(1.0f);
	spaceshipTransform = glm::translate(spaceshipTransform, m_camera->cameraPos + m_camera->cameraFront);		// start with camera matrix coordinate
	spaceshipTransform = glm::translate(spaceshipTransform, glm::vec3(0., -.4f, -1.));
	spaceshipTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mesh != NULL)
		m_mesh->Update(spaceshipTransform);
	getCamera()->Update();


	// position of the sun	
	modelStack.push(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)));  // sun's coordinate
	localTransform = modelStack.top();		// The sun origin
	localTransform *= glm::rotate(glm::mat4(1.0f), (float)dt/7, glm::vec3(0.f, 1.f, 0.f));
	localTransform *= glm::scale(glm::vec3(20., 20., 20.));
	if (m_sun != NULL)
		m_sun->Update(localTransform);

	// position of the mercury
	updateMercury(dt);

	// position of the venus
	updateVenus(dt);

	// position of the earth
	updateEarth(dt);

	// position of the mars
	updateMars(dt);

	//updateAsteroidBelt(dt);

	// position of the jupiter
	updateJupiter(dt);

	// position of the saturn
	updateSaturn(dt);

	// position of the uranus
	updateUranus(dt);

	// position of the neptune
	updateNeptune(dt);

	//Eris=Dwarf planet far away
	//Haumea = Kuiper belt object, past neptune

	//modelStack.pop();	// empy stack
	if (firstFrame = true)
	{
		firstFrame = false;
	}


}

void Graphics::updateMercury(double dt)
{
	speed = { .08, .08, .08 };
	distM = 6;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .6;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mercury != NULL)
		m_mercury->Update(localTransform);
}

void Graphics::updateVenus(double dt)
{
	speed = { 0.04, 0.04, 0.04 };
	distM = 9;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .8;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_venus != NULL)
		m_venus->Update(localTransform);
}

void Graphics::updateEarth(double dt)
{
	speed = { 0.02, 0.02, 0.02 };
	distM = 12;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { .2, .2, .2 };
	scaler = 1.;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_earth != NULL)
		m_earth->Update(localTransform);



	// position of the earth moon
	speed = { .8, .8, .8 };
	distM = 1.25;
	distM = distM * multiplier;
	dist = { distM, 0, distM };
	rotVector = { 0.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scaler = .2;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_earth_moon != NULL)
		m_earth_moon->Update(localTransform);

	modelStack.pop(); 	// back to the sun coordinate
}

void Graphics::updateMars(double dt)
{
	speed = { 0.01, 0.01, 0.01 };
	distM = 15;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { .2, .2, .2 };
	scaler = .6;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store mars coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mars != NULL)
		m_mars->Update(localTransform);

	// position of the mars moon1
	speed = { 0.01, 0.01, 0.01 };
	distM = 1.5;
	distM = distM * multiplier;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .2;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// use mars coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mars_moon1 != NULL)
		m_mars_moon1->Update(localTransform);

	// position of the mars moon2
	speed = { 0.07, 0.07, 0.07 };
	distM = 1;
	distM = distM * multiplier;
	dist = { distM, 0, distM };
	rotVector = { 1.,0.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .16;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with mars coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mars_moon2 != NULL)
		m_mars_moon2->Update(localTransform);

	modelStack.pop();
}

void Graphics::updateJupiter(double dt)
{
	speed = { 0.005, 0.008, 0.005 };
	distM = 18;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { .2, .2, .2 };
	scaler = 2.;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_jupiter != NULL)
		m_jupiter->Update(localTransform);
}

void Graphics::updateSaturn(double dt)
{
	speed = { 0.0025, 0.0025, 0.0025 };
	distM = 21;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = 1.6;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_saturn != NULL)
		m_saturn->Update(localTransform);

	//Saturn Ring
	if (firstFrame = true)
	{
		dt = dt + 85000000;
	}
	speed = { 0., 0.00000001, 0. };
	dist = { 0., 0, 0. };
	rotVector = { 1.,0.,0. };
	rotSpeed = { 0., 0., 0. };
	scale = { .2,.001, .2 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_saturn_ring != NULL)
		m_saturn_ring->Update(localTransform);
	if (firstFrame = true)
	{
		dt = dt - 85000000;
	}

	// position of the saturn moon1
	speed = { 0.025, 0.025, 0.025 };
	distM = 2;
	distM = distM * multiplier;
	dist = { distM, 0, distM };
	rotVector = { 0.,0.,1. };
	rotSpeed = { 1., 1., 1. };
	scaler = .1;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_saturn_moon1 != NULL)
		m_saturn_moon1->Update(localTransform);

	// position of the saturn moon2
	speed = { 0.0025, 0.0025, 0.0025 };
	distM = 1.5;
	distM = distM * multiplier;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .05;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_saturn_moon2 != NULL)
		m_saturn_moon2->Update(localTransform);


	modelStack.pop();
}

void Graphics::updateUranus(double dt)
{
	speed = { 0.00125, 0.00125, 0.00125 };
	distM = 24;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = 1.2;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_uranus != NULL)
		m_uranus->Update(localTransform);



	//Uranus ring
	speed = { 0., 0.00000001, 0. };
	dist = { 0., 0, 0. };
	rotVector = { 1.,0.,0. };
	rotSpeed = { 0., 0., 0. };
	scale = { .2,.001, .2 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_uranus_ring != NULL)
		m_uranus_ring->Update(localTransform);

	modelStack.pop();
}

void Graphics::updateNeptune(double dt)
{
	speed = { 0.000625, 0.000625, 0.000625 };
	distM = 27;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = 1.2;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_neptune != NULL)
		m_neptune->Update(localTransform);
}

void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist, 
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) {
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//SKybox Shading---------------
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader->use();

	model = glm::mat4(1.0f);
	view = m_camera->GetViewSkybox();
	projection = m_camera->GetProjection();
	skyboxShader->setMat4("model", model);
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", projection);

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	GLuint globalAmbLoc = m_shader2->GetUniformLocation("normalSP");
	if (globalAmbLoc == -1)
	{
		printf("globalAmbient attribute not found\n");
	}

	// Locate the light ambient attribute
	GLuint lightALoc = m_shader2->GetUniformLocation("PositionalLight.ambient");
	if (lightALoc == -1)
	{
		printf("lightAmbient attribute not found\n");
	}

	// Locate the light diffuse attribute
	GLuint lightDLoc = m_shader2->GetUniformLocation("PositionalLight.diffuse");
	if (lightDLoc == -1)
	{
		printf("lightDiffuse attribute not found\n");

	}
	//Locate light specular attribute
	GLuint lightSLoc = m_shader2->GetUniformLocation("PositionalLight.spec");
	if (lightSLoc == INVALID_UNIFORM_LOCATION) {
		printf("lightSpec uniform not found\n");
	}

	// Locate the light position attribute
	GLuint lightPosLoc = m_shader2->GetUniformLocation("PositionalLight.position");
	if (lightPosLoc == -1)
	{
		printf("lightPosition attribute not found\n");
	}

	float matAmbient[4] = { .2,.2,.2,1.0 };
	float matDiff[4] = { 1., 1., 1., 1. };
	float matSpec[4] = { 1., 1., 1., 1. };
	float matShininess = 20.0;

	GLuint mAmbLoc = glGetUniformLocation(m_shader2->GetUniformLocation("light.ambient"), "materal.ambient");
	glProgramUniform4fv(m_shader2->GetUniformLocation("light.ambient"), mAmbLoc, 1, matAmbient);
	GLuint mDiffLoc = glGetUniformLocation(m_shader2->GetUniformLocation("light.diffuse"), "material.diffuse");
	glProgramUniform4fv(m_shader2->GetUniformLocation("light.diffuse"), mDiffLoc, 1, matDiff);
	GLuint mSpecLoc = glGetUniformLocation(m_shader2->GetUniformLocation("light.spec"), "materal.spec");
	glProgramUniform4fv(m_shader2->GetUniformLocation("light.spec"), mSpecLoc, 1, matSpec);
	GLuint mShineLoc = glGetUniformLocation(m_shader2->GetUniformLocation("light.spec"), "material.shininess");
	glProgramUniform1f(m_shader2->GetUniformLocation("light.spec"), mShineLoc, matShininess);
	// Start the correct program

	m_shader2->use();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView() * m_mercury->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sun->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView() * m_mercury->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView() * m_venus->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_venus->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView() * m_earth->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_earth->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView() * m_mars->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView() * m_ceres1->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres1->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView() * m_ceres2->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres2->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView() * m_ceres3->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres3->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView() * m_ceres4->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres4->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres5->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres5->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres6->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres6->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres7->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres7->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres8->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres8->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres9->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres9->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres10->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres10->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres11->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres11->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres12->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres12->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres13->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres13->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres14->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres14->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres15->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres15->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres16->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres16->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres17->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres17->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres18->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres18->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres19->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres19->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_ceres20->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres20->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_jupiter->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jupiter->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_saturn->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_neptune->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_neptune->GetModel()));
	glUniformMatrix3fv(m_normalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::inverse(glm::mat3(m_camera->GetView()* m_comet->GetModel()))))));
	glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_comet->GetModel()));





	
	
	if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		
		//m_shader->setMat4("model", m_mesh->GetModel());

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mesh->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mesh->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}

	if (m_sun != NULL) {
		//m_shader->setMat4("model", m_sun->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sun->GetModel()));
		if (m_sun->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sun->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Mercury-----------------------------------------------------------------------------------
	renderMercury();

	//Venus
	renderVenus();

	//Earth
	renderEarth();

	//Mars
	renderMars();

	//Astroid Belt
	//renderAsteroidBelt();

	//Jupiter
	renderJupiter();

	//Saturn
	renderSaturn();

	//Uranus
	renderUranus();

	//Neptune
	renderNeptune();

}

void Graphics::renderMercury()
{
	if (m_mercury != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury->GetModel()));
		if (m_mercury->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mercury->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
}

void Graphics::renderVenus()
{
	if (m_venus != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_venus->GetModel()));
		if (m_venus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_venus->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_venus->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
}

void Graphics::renderEarth()
{
	if (m_earth != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_earth->GetModel()));
		if (m_earth->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_earth->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	//Earth Moon
	if (m_earth_moon != NULL) {
		//m_shader->setMat4("model", m_earth_moon->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_earth_moon->GetModel()));
		if (m_earth_moon->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth_moon->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_earth_moon->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
}

void Graphics::renderMars()
{
	if (m_mars != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars->GetModel()));
		if (m_mars->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mars->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Mars moon1
	if (m_mars_moon1 != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars_moon1->GetModel()));
		if (m_mars_moon1->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars_moon1->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mars_moon1->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Mars moon2
	if (m_mars_moon2 != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars_moon2->GetModel()));
		if (m_mars_moon2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars_moon2->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mars_moon2->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
}

void Graphics::renderJupiter()
{
	if (m_jupiter != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jupiter->GetModel()));
		if (m_jupiter->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jupiter->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jupiter->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
}

void Graphics::renderSaturn()
{
	if (m_saturn != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn->GetModel()));
		if (m_saturn->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Saturn moon1
	if (m_saturn_moon1 != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn_moon1->GetModel()));
		if (m_saturn_moon1->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn_moon1->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn_moon1->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Saturn moon2
	if (m_saturn_moon2 != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn_moon2->GetModel()));
		if (m_saturn_moon2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn_moon2->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn_moon2->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	//Saturn Ring
	if (m_saturn_ring != NULL) {
		glUniform1i(m_hasTexture, false);

		//m_shader->setMat4("model", m_mesh->GetModel());

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn_ring->GetModel()));
		if (m_saturn_ring->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn_ring->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn_ring->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
}

void Graphics::renderUranus()
{
	if (m_uranus != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_uranus->GetModel()));
		if (m_uranus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uranus->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_uranus->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	//Uranus Ring
	if (m_uranus_ring != NULL) {
		glUniform1i(m_hasTexture, false);

		//m_shader->setMat4("model", m_mesh->GetModel());

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_uranus_ring->GetModel()));
		if (m_uranus_ring->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uranus_ring->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_uranus_ring->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
}

void Graphics::renderNeptune()
{
	if (m_neptune != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_neptune->GetModel()));
		if (m_neptune->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_neptune->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_neptune->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader2->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader2->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader2->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader2->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the normal vertex attribute
	m_normalAttrib = m_shader2->GetAttribLocation("v_color");
	if (m_normalAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader2->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader2->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

unsigned int Graphics::loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


unsigned int Graphics::loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void Graphics::updateAsteroidBelt(double dt)
{
	//Ceres1 in astroid belt
	speed = { 0.005, 0.008, 0.005 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres1 != NULL)
		m_ceres1->Update(localTransform);

	//Ceres2 in astroid belt
	speed = { -0.01, 0.001, -0.01 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres2 != NULL)
		m_ceres2->Update(localTransform);
	
	//Ceres3 in astroid belt
	speed = { 0.003, 0.005, 0.003 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres3 != NULL)
		m_ceres3->Update(localTransform);
	
	//Ceres4 in astroid belt
	speed = { 0.001, 0.0013, 0.01 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres4 != NULL)
		m_ceres4->Update(localTransform);

	//Ceres5 in astroid belt
	speed = { 0.009, 0.012, 0.009 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres5 != NULL)
		m_ceres5->Update(localTransform);

	//Ceres6 in astroid belt
	speed = { 0.008, 0.011, 0.08 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres6 != NULL)
		m_ceres6->Update(localTransform);

	//Ceres7 in astroid belt
	speed = { 0.007, 0.01, 0.007 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres7 != NULL)
		m_ceres7->Update(localTransform);

	//Ceres8 in astroid belt
	speed = { 0.006, 0.009, 0.006 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres8 != NULL)
		m_ceres8->Update(localTransform);

	//Ceres9 in astroid belt
	speed = { -0.005, -0.008, -0.005 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres9 != NULL)
		m_ceres9->Update(localTransform);

	//Ceres10 in astroid belt
	speed = { 0.001, 0.001, 0.004 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres10 != NULL)
		m_ceres10->Update(localTransform);

	//Ceres11 in astroid belt
	speed = { -0.008, 0.002, 0.005 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres11 != NULL)
		m_ceres11->Update(localTransform);

	//Ceres12 in astroid belt
	speed = { 0.005, 0.007, 0.001 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres12 != NULL)
		m_ceres12->Update(localTransform);

	//Ceres13 in astroid belt
	speed = { -0.004, -0.001, -0.004 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres13 != NULL)
		m_ceres13->Update(localTransform);

	//Ceres14 in astroid belt
	speed = { -0.003, 0.001, -0.003 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres14 != NULL)
		m_ceres14->Update(localTransform);

	//Ceres15 in astroid belt
	speed = {-0.002, 0.002, -0.002 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres15 != NULL)
		m_ceres15->Update(localTransform);

	//Ceres16 in astroid belt
	speed = { -0.001, 0.003, -0.001 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres16 != NULL)
		m_ceres16->Update(localTransform);

	//Ceres17 in astroid belt
	speed = { 0.001, 0.004, 0.001 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres17 != NULL)
		m_ceres17->Update(localTransform);

	//Ceres18 in astroid belt
	speed = { 0.002, 0.005, 0.002 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres18 != NULL)
		m_ceres18->Update(localTransform);

	//Ceres19 in astroid belt
	speed = { 0.003, 0.006, 0.003 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres19 != NULL)
		m_ceres19->Update(localTransform);

	//Ceres20 in astroid belt
	speed = { 0.004, 0.007, 0.004 };
	distM = 16.5;
	distM = distM * multiplierD;
	dist = { distM, 0, distM };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scaler = .4;
	scaler = scaler * multiplier;
	scale = { scaler, scaler, scaler };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_ceres20 != NULL)
		m_ceres20->Update(localTransform);
}

void Graphics::renderAsteroidBelt()
{

	//Ceres1
	if (m_ceres1 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres1->GetModel()));
		if (m_ceres1->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres1->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres1->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres2
	if (m_ceres2 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres2->GetModel()));
		if (m_ceres2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres2->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres2->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres3
	if (m_ceres3 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres3->GetModel()));
		if (m_ceres3->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres3->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres3->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres4
	if (m_ceres4 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres4->GetModel()));
		if (m_ceres4->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres4->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres4->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres5
	if (m_ceres5 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres5->GetModel()));
		if (m_ceres5->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres5->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres5->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres6
	if (m_ceres6 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres6->GetModel()));
		if (m_ceres6->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres6->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres6->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres7
	if (m_ceres7 != NULL) {
		glUniform1i(m_hasTexture, false);

		//m_shader->setMat4("model", m_mesh->GetModel());

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres7->GetModel()));
		if (m_ceres7->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres7->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres7->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres8
	if (m_ceres8 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres8->GetModel()));
		if (m_ceres8->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres8->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres8->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres9
	if (m_ceres9 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres9->GetModel()));
		if (m_ceres9->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres9->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres9->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Ceres10
	if (m_ceres10 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres10->GetModel()));
		if (m_ceres10->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres10->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres10->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Ceres11
	if (m_ceres11 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres11->GetModel()));
		if (m_ceres11->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres11->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres11->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres12
	if (m_ceres12 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres12->GetModel()));
		if (m_ceres12->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres12->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres12->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres13
	if (m_ceres13 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres13->GetModel()));
		if (m_ceres13->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres13->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres13->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres14
	if (m_ceres14 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres14->GetModel()));
		if (m_ceres14->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres14->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres14->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres15
	if (m_ceres15 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres15->GetModel()));
		if (m_ceres15->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres15->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres15->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Ceres16
	if (m_ceres16 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres16->GetModel()));
		if (m_ceres16->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres16->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres16->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres17
	if (m_ceres17 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres17->GetModel()));
		if (m_ceres17->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres17->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres17->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres18
	if (m_ceres18 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres18->GetModel()));
		if (m_ceres18->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres18->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres18->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres19
	if (m_ceres19 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres19->GetModel()));
		if (m_ceres19->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres19->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres19->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Ceres20
	if (m_ceres20 != NULL) {
		glUniform1i(m_hasTexture, false);

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ceres20->GetModel()));
		if (m_ceres20->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_ceres20->getTextureID());
			GLuint sampler = m_shader2->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_ceres20->Render(m_positionAttrib, m_normalAttrib, m_tcAttrib, m_hasTexture);
		}
	}

}