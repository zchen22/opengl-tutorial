#include "HelloCoordSystems.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileReader.h"
#include "stb_image.h"

HelloCoordSystems::HelloCoordSystems()
	: windowWidth_(0),
	windowHeight_(0),
	shader_(nullptr),
	vao_(0),
	vbo_(0) {
}

HelloCoordSystems::HelloCoordSystems(const int width, const int height)
	: windowWidth_(width),
	windowHeight_(height),
	shader_(nullptr),
	vao_(0),
	vbo_(0) {
}

HelloCoordSystems::~HelloCoordSystems() {
}

int HelloCoordSystems::Main() {
	Init_();
	SetUpShader_();
	SetUpVertexData_();
	CreateTexture1_();
	CreateTexture2_();
	BindSamplerToTexUnit_();
	CreateModelMatrix_();
	// Render loop
	while (!glfwWindowShouldClose(window_)) {
		ProcessInput_(window_);
		CreateModelMatrix_();
		CreateViewMatrix_();
		CreateProjectionMatrix_();
		Render_();
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
	CleanUp_();
	return 0;
}

// Initialize resources
int HelloCoordSystems::Init_() {
	// Initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a window
	window_ = glfwCreateWindow(windowWidth_, windowHeight_, "HelloCoordSystems", NULL, NULL);
	if (window_ == NULL) {
		logger_.Error("Failed to create a GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window_);
	glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback_);
	// Load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logger_.Error("Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	// Create an empty shader program
	shader_ = std::make_unique<Shader>(Shader(logger_));
	return 0;
}

// Set up the shader program
int HelloCoordSystems::SetUpShader_() {
	assert(shader_ != nullptr);
	shader_->AddVertex("src/HelloCoordSystemsVertex.glsl");
	shader_->AddFragment("src/HelloCoordSystemsFragment.glsl");
	shader_->Link();
	return 0;
}

// Set up vertex data
int HelloCoordSystems::SetUpVertexData_() {
	// Cube
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	// Bind VAO
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO
	glBindVertexArray(0);
	return 0;
}

// Create a texture
int HelloCoordSystems::CreateTexture1_() {
	// Create and configure the texture
	glGenTextures(1, &tex1_);
	glBindTexture(GL_TEXTURE_2D, tex1_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	int width = 0, height = 0, numChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("res/container.jpg", &width, &height, &numChannels, 0);
	if (!data) {
		logger_.Error("Failed to load texture1\n");
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return 0;
}

// Create a texture
int HelloCoordSystems::CreateTexture2_() {
	// Create and configure the texture
	glGenTextures(1, &tex2_);
	glBindTexture(GL_TEXTURE_2D, tex2_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	int width = 0, height = 0, numChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("res/awesomeface.png", &width, &height, &numChannels, 0);
	if (!data) {
		logger_.Error("Failed to load texture2\n");
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return 0;
}

// Bind samplers to texture units
int HelloCoordSystems::BindSamplerToTexUnit_() {
	glUseProgram(shader_->program);
	glUniform1i(glGetUniformLocation(shader_->program, "sampler1"), 0);
	glUniform1i(glGetUniformLocation(shader_->program, "sampler2"), 1);
	return 0;
}

// Process all input
int HelloCoordSystems::ProcessInput_(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	return 0;
}

// Create the model matrix
int HelloCoordSystems::CreateModelMatrix_() {
	glm::mat4 model;
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	const int modelLoc = glGetUniformLocation(shader_->program, "model");
	glUseProgram(shader_->program);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	return 0;
}

// Create the view matrix
int HelloCoordSystems::CreateViewMatrix_() {
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	const int viewLoc = glGetUniformLocation(shader_->program, "view");
	glUseProgram(shader_->program);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	return 0;
}

// Create the projection matrix
int HelloCoordSystems::CreateProjectionMatrix_() {
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1.0f * windowWidth_ / windowHeight_, 0.1f, 100.0f);
	const int projectionLoc = glGetUniformLocation(shader_->program, "projection");
	glUseProgram(shader_->program);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	return 0;
}

// Render
int HelloCoordSystems::Render_() {
	// Clear
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2_);
	// Draw
	glUseProgram(shader_->program);
	glBindVertexArray(vao_);  // No need to bind it every time 
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);  // No need to unbind it every time 
	return 0;
}

// Clean up resources
int HelloCoordSystems::CleanUp_() {
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	glfwTerminate();
	return 0;
}

// Whenever the window size changed, this callback function executes
void HelloCoordSystems::FramebufferSizeCallback_(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}
