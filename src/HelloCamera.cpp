#include "HelloCamera.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileReader.h"
#include "stb_image.h"

HelloCamera::HelloCamera()
	: windowWidth_(0),
	windowHeight_(0),
	vertexShaderSource_(nullptr),
	fragmentShaderSource_(nullptr),
	vertexShader_(0),
	fragmentShader_(0),
	shaderProgram_(0),
	vao_(0),
	vbo_(0) {
}

HelloCamera::HelloCamera(const int width, const int height)
	: windowWidth_(width),
	windowHeight_(height),
	vertexShaderSource_(nullptr),
	fragmentShaderSource_(nullptr),
	vertexShader_(0),
	fragmentShader_(0),
	shaderProgram_(0),
	vao_(0),
	vbo_(0) {
}

HelloCamera::~HelloCamera() {
}

int HelloCamera::Main() {
	Init_();
	CreateVertexShader_();
	CreateFragmentShader_();
	LinkShaders_();
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
int HelloCamera::Init_() {
	// Initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a window
	window_ = glfwCreateWindow(windowWidth_, windowHeight_, "HelloCamera", NULL, NULL);
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
	return 0;
}

// Create a vertex shader
int HelloCamera::CreateVertexShader_() {
	FileReader f("src/HelloCameraVertex.glsl");
	vertexShaderSource_ = new char[f.GetSize() + 1]();
	f.ReadAsString(vertexShaderSource_);
	vertexShader_ = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader_, 1, &vertexShaderSource_, NULL);
	glCompileShader(vertexShader_);
	int success = 0;
	char infoLog[512] = "";
	glGetShaderiv(vertexShader_, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader_, 512, NULL, infoLog);
		logger_.Error("Failed to build the vertex shader\n%s\n", infoLog);
	}
	return 0;
}

// Create a fragment shader
int HelloCamera::CreateFragmentShader_() {
	FileReader f("src/HelloCameraFragment.glsl");
	fragmentShaderSource_ = new char[f.GetSize() + 1]();
	f.ReadAsString(fragmentShaderSource_);
	fragmentShader_ = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader_, 1, &fragmentShaderSource_, NULL);
	glCompileShader(fragmentShader_);
	int success = 0;
	char infoLog[512] = "";
	glGetShaderiv(fragmentShader_, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader_, 512, NULL, infoLog);
		logger_.Error("Failed to build the fragment shader\n%s\n", infoLog);
	}
	return 0;
}

// Link shaders
int HelloCamera::LinkShaders_() {
	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShader_);
	glAttachShader(shaderProgram_, fragmentShader_);
	glLinkProgram(shaderProgram_);
	int success = 0;
	char infoLog[512] = "";
	glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
		logger_.Error("Failed to link shaders\n");
	}
	delete[] vertexShaderSource_;
	delete[] fragmentShaderSource_;
	glDeleteShader(vertexShader_);
	glDeleteShader(fragmentShader_);
	return 0;
}

// Set up vertex data
int HelloCamera::SetUpVertexData_() {
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
int HelloCamera::CreateTexture1_() {
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
int HelloCamera::CreateTexture2_() {
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
int HelloCamera::BindSamplerToTexUnit_() {
	glUseProgram(shaderProgram_);
	glUniform1i(glGetUniformLocation(shaderProgram_, "sampler1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram_, "sampler2"), 1);
	return 0;
}

// Process all input
int HelloCamera::ProcessInput_(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	return 0;
}

// Create the model matrix
int HelloCamera::CreateModelMatrix_() {
	glm::mat4 model;
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	const int modelLoc = glGetUniformLocation(shaderProgram_, "model");
	glUseProgram(shaderProgram_);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	return 0;
}

// Create the view matrix
int HelloCamera::CreateViewMatrix_() {
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	const int viewLoc = glGetUniformLocation(shaderProgram_, "view");
	glUseProgram(shaderProgram_);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	return 0;
}

// Create the projection matrix
int HelloCamera::CreateProjectionMatrix_() {
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1.0f * windowWidth_ / windowHeight_, 0.1f, 100.0f);
	const int projectionLoc = glGetUniformLocation(shaderProgram_, "projection");
	glUseProgram(shaderProgram_);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	return 0;
}

// Render
int HelloCamera::Render_() {
	// Clear
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2_);
	// Draw
	glUseProgram(shaderProgram_);
	glBindVertexArray(vao_);  // No need to bind it every time 
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);  // No need to unbind it every time 
	return 0;
}

// Clean up resources
int HelloCamera::CleanUp_() {
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	glfwTerminate();
	return 0;
}

// Whenever the window size changed, this callback function executes
void HelloCamera::FramebufferSizeCallback_(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}
