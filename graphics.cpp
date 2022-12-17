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
	m_shader = new Shader();
	//m_shader = new Shader2("base.vs", "base.fs");

	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		//printf("HERE-------\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	//Skybox Stuff


	skyboxShader = new Shader2("skybox.vs", "skybox.fs");

	float skyboxVertices[] = {
		// positions          
		-50.0f,  50.0f, -50.0f,
		-50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,

		-50.0f, -50.0f,  50.0f,
		-50.0f, -50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f, -50.0f,
		-50.0f,  50.0f,  50.0f,
		-50.0f, -50.0f,  50.0f,

		 50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,

		-50.0f, -50.0f,  50.0f,
		-50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f, -50.0f,  50.0f,
		-50.0f, -50.0f,  50.0f,

		-50.0f,  50.0f, -50.0f,
		 50.0f,  50.0f, -50.0f,
		 50.0f,  50.0f,  50.0f,
		 50.0f,  50.0f,  50.0f,
		-50.0f,  50.0f,  50.0f,
		-50.0f,  50.0f, -50.0f,

		-50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		 50.0f, -50.0f, -50.0f,
		 50.0f, -50.0f, -50.0f,
		-50.0f, -50.0f,  50.0f,
		 50.0f, -50.0f,  50.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces=
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


	// Starship
	m_mesh = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\Objects\\SpaceShip-1.obj", "assets\\Objects\\SpaceShip-1.png");

	// The Sun
	m_sun = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 200, 1.5,64, "assets\\PlanetaryTextures\\2k_sun.jpg");

	//Mercury
	m_mercury = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Mercury.jpg");
	m_mercury_orbit = new Mesh(glm::vec3(0.0f, -3.0f, 0.0f), "assets\\Objects\\Ring.obj", "assets\\Objects\\Ring2.png");

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

	//Jupiter
	m_jupiter = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Jupiter.jpg");

	// Saturn
	m_saturn = new Sphere(glm::vec3(-3.f, -2.f, 100.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Saturn.jpg");

	m_saturn_moon1 = new Sphere(glm::vec3(-3.f, -2.f, 100.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\2k_moon.jpg");
	m_saturn_moon2 = new Sphere(glm::vec3(-3.f, -2.f, 100.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Jupiter.jpg");
	//Saturn Ring


	//Uranus
	m_uranus = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Uranus.jpg");

	//Neptune
	m_neptune = new Sphere(glm::vec3(-3.f, -2.f, 2.f), 25, 1.5, 48, "assets\\PlanetaryTextures\\Neptune.jpg");



	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) 
{

	

	getCamera()->Update();
	//Initial jump start for planets
	if (firstFrame = true)
	{
		dt = dt +8500;
	}

	std::vector<float> speed, dist, rotSpeed, scale;
	glm::vec3 rotVector;
	glm::mat4 localTransform;
	// position of the sun	
	modelStack.push(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)));  // sun's coordinate
	localTransform = modelStack.top();		// The sun origin
	localTransform *= glm::rotate(glm::mat4(1.0f), (float)dt/7, glm::vec3(0.f, 1.f, 0.f));
	localTransform *= glm::scale(glm::vec3(3.0, 3.0, 3.0));
	if (m_sun != NULL)
		m_sun->Update(localTransform);

	//The Starship
	speed = { 1., 1., -1. };
	dist = { 5.5, 5.5, 5.5 };
	rotVector = { 0.,1.,1. };
	rotSpeed = { 1., 1., 1. };
	scale = { 0.02,0.02,0.02 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mesh != NULL)
		m_mesh->Update(localTransform);

	// position of the mercury
	speed = { .08, .08, .08 };
	dist = { 6., 0, 6. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .3,.3,.3 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mercury != NULL)
		m_mercury->Update(localTransform);

	// position of the mercury orbit

	if (firstFrame = true)
	{
		dt = dt + 85000000;
	}
	speed = { 0., 0.00000001, 0. };
	dist = { 0., -0.5, 0. };
	rotVector = { 1.,0.,0. };
	rotSpeed = { 0., 0., 0. };
	scale = { .11,.001, .11 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mercury_orbit != NULL)
		m_mercury_orbit->Update(localTransform);
	if (firstFrame = true)
	{
		dt = dt - 85000000;
	}


	// position of the venus
	speed = { 0.04, 0.04, 0.04 };
	dist = { 9., 0, 9. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .4,.4,.4 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_venus != NULL)
		m_venus->Update(localTransform);


	// position of the earth
	speed = { 0.02, 0.02, 0.02 };
	dist = { 12., 0, 12. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .5,.5,.5 };
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
	dist = { 1.25, 1.25, 0. };
	rotVector = { 0.,0.,1. };
	rotSpeed = { .25, .25, .25 };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store moon-planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_earth_moon != NULL)
		m_earth_moon->Update(localTransform);




	modelStack.pop(); 	// back to the planet coordinate

	modelStack.pop(); 	// back to the sun coordinate

	// position of the mars
	speed = { 0.01, 0.01, 0.01 };
	dist = { 15., 0, 15. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .3,.3,.3 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mars != NULL)
		m_mars->Update(localTransform);

	// position of the mars moon1
	speed = { 0.01, 0.01, 0.01 };
	dist = { .5, 0, .5 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .1,.1,.1 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mars_moon1 != NULL)
		m_mars_moon1->Update(localTransform);

	// position of the mars moon2
	speed = { 0.07, 0.07, 0.07 };
	dist = { 1., 0, 1. };
	rotVector = { 1.,0.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .08,.08,.08 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mars_moon2 != NULL)
		m_mars_moon2->Update(localTransform);

	modelStack.pop();

	//position of astroid belt
	//Ceres in astroid belt

	// position of the jupiter
	speed = { 0.005, 0.008, 0.005 };
	dist = { 18., 0, 18. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .9,.9,.9 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_jupiter != NULL)
		m_jupiter->Update(localTransform);

	// position of the saturn
	speed = { 0.0025, 0.0025, 0.0025 };
	dist = { 21., 0, 21. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .8,.8,.8 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_saturn != NULL)
		m_saturn->Update(localTransform);

	// position of the saturn moon1
	speed = { 0.025, 0.025, 0.025 };
	dist = { 2., 0, 2. };
	rotVector = { 0.,0.,1. };
	rotSpeed = { 1., 1., 1. };
	scale = { .3,.3,.3 };
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
	dist = { 1., 0, 1. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .1,.1,.1 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_saturn_moon2 != NULL)
		m_saturn_moon2->Update(localTransform);

	//Saturns Ring


	modelStack.pop();

	// position of the uranus
	speed = { 0.00125, 0.00125, 0.00125 };
	dist = { 24., 0, 24. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .7,.7,.7 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_uranus != NULL)
		m_uranus->Update(localTransform);



	//Uranus rings

	// position of the neptune
	speed = { 0.000625, 0.000625, 0.000625 };
	dist = { 27., 0, 27. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .6,.6,.6 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	//modelStack.push(localTransform);			// store planet-sun coordinate
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_neptune != NULL)
		m_neptune->Update(localTransform);

	//Eris=Dwarf planet far away
	//Haumea = Kuiper belt object, past neptune

	//modelStack.pop();	// empy stack
	if (firstFrame = true)
	{
		firstFrame = false;
	}


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
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	// Start the correct program

	m_shader->Enable();

	//m_shader->use();
	//model = glm::mat4(1.0f);
	//view = m_camera->GetView();
	//projection = m_camera->GetProjection();
	//m_shader->setMat4("model", model);
	//m_shader->setMat4("view", view);
	//m_shader->setMat4("projection", projection);

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
	

	if (m_mesh != NULL) {
		glUniform1i(m_hasTexture, false);
		
		//m_shader->setMat4("model", m_mesh->GetModel());

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mesh->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mesh->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	error = glGetError();
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
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sun->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Mercury-----------------------------------------------------------------------------------
	if (m_mercury != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury->GetModel()));
		if (m_mercury->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mercury->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	//Mercury Orbit
	if (m_mercury_orbit != NULL) {
		glUniform1i(m_hasTexture, false);

		//m_shader->setMat4("model", m_mesh->GetModel());

		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury_orbit->GetModel()));
		if (m_mesh->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mercury_orbit->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mercury_orbit->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Venus
	if (m_venus != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_venus->GetModel()));
		if (m_venus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_venus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_venus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Earth
	if (m_earth != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_earth->GetModel()));
		if (m_earth->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_earth->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	//Earth Moon
	if (m_earth_moon != NULL) {
		//m_shader->setMat4("model", m_earth_moon->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_earth_moon->GetModel()));
		if (m_earth_moon->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth_moon->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_earth_moon->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Mars
	if (m_mars != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars->GetModel()));
		if (m_mars->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mars->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Mars moon1
	if (m_mars_moon1 != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars_moon1->GetModel()));
		if (m_mars_moon1->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars_moon1->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mars_moon1->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Mars moon2
	if (m_mars_moon2 != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars_moon2->GetModel()));
		if (m_mars_moon2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars_moon2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mars_moon2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Astroid Belt

	//Jupiter
	if (m_jupiter != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jupiter->GetModel()));
		if (m_jupiter->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jupiter->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jupiter->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	//Saturn
	if (m_saturn != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn->GetModel()));
		if (m_saturn->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Saturn moon1
	if (m_saturn_moon1 != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn_moon1->GetModel()));
		if (m_saturn_moon1->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn_moon1->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn_moon1->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//Saturn moon2
	if (m_saturn_moon2 != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn_moon2->GetModel()));
		if (m_saturn_moon2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn_moon2->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn_moon2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	//Saturn Ring

	
	//Uranus
	if (m_uranus != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_uranus->GetModel()));
		if (m_uranus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uranus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_uranus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	//Uranus Ring

	//Neptune
	if (m_neptune != NULL) {
		//m_shader->setMat4("model", m_earth->GetModel());
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_neptune->GetModel()));
		if (m_neptune->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_neptune->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_neptune->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	// Get any errors from OpenGL



}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_color");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
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

