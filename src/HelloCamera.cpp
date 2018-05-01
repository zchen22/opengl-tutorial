#include "HelloCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "FileReader.h"

bool HelloCamera::firstFrame_ = true;
float HelloCamera::lastCursorX_ = 0;
float HelloCamera::lastCursorY_ = 0;
float HelloCamera::yaw_ = -90;
float HelloCamera::pitch_ = 0;
float HelloCamera::fov_ = 45;

HelloCamera::HelloCamera(const int width = 800, const int height = 600)
	: windowWidth_(width),
	windowHeight_(height),
	shader_(nullptr),
	vao_(0),
	vbo_(0),
	tex1_(0),
	tex2_(0),
	deltaFrameTime_(0.0f),
	lastFrameTime_(0.0f) {
	lastCursorX_ = width / 2;
	lastCursorY_ = height / 2;
}

HelloCamera::~HelloCamera() {
}

int HelloCamera::Main() {
	Init_();
	SetUpShader_();
	SetUpVertexData_();
	CreateTexture1_();
	CreateTexture2_();
	BindSamplerToTexUnit_();
	// Render loop
	while (!glfwWindowShouldClose(window_)) {
		GetFrameTime_();
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
	// Register mouse callback functions
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window_, MouseCallback_);
	glfwSetScrollCallback(window_, ScrollCallback_);
	// Create an empty shader program
	shader_ = std::make_unique<Shader>(logger_);
	// Create a camera
	camera_ = std::make_unique<Camera>(logger_);
	return 0;
}

// Set up the shader program
int HelloCamera::SetUpShader_() {
	assert(shader_ != nullptr);
	shader_->AddVertex("src/HelloCameraVertex.glsl");
	shader_->AddFragment("src/HelloCameraFragment.glsl");
	shader_->Link();
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
	glUseProgram(shader_->program);
	glUniform1i(glGetUniformLocation(shader_->program, "sampler1"), 0);
	glUniform1i(glGetUniformLocation(shader_->program, "sampler2"), 1);
	return 0;
}

// Get time
int HelloCamera::GetFrameTime_() {
	float currentFrameTime = glfwGetTime();
	deltaFrameTime_ = currentFrameTime - lastFrameTime_;
	lastFrameTime_ = currentFrameTime;
	return 0;
}

// Process all input
int HelloCamera::ProcessInput_(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	camera_->speed = 1.0f * deltaFrameTime_;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera_->MoveForward();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera_->MoveBackward();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera_->MoveLeft();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera_->MoveRight();
	}
	
	return 0;
}

// Create the model matrix
int HelloCamera::CreateModelMatrix_() {
	glm::mat4 model(1.0f);
	const int modelLoc = glGetUniformLocation(shader_->program, "model");
	glUseProgram(shader_->program);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	return 0;
}

// Create the view matrix
int HelloCamera::CreateViewMatrix_() {
	camera_->CalculateFront(pitch_, yaw_);
	glm::mat4 view = glm::lookAt(camera_->pos, camera_->pos + camera_->front, camera_->up);
	const int viewLoc = glGetUniformLocation(shader_->program, "view");
	glUseProgram(shader_->program);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	return 0;
}

// Create the projection matrix
int HelloCamera::CreateProjectionMatrix_() {
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov_), 1.0f * windowWidth_ / windowHeight_, 0.1f, 100.0f);
	const int projectionLoc = glGetUniformLocation(shader_->program, "projection");
	glUseProgram(shader_->program);
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
	glUseProgram(shader_->program);
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

// Whenever the mouse moves, this callback function executes
void HelloCamera::MouseCallback_(GLFWwindow* window, double xPos, double yPos) {
	if (firstFrame_) {
		lastCursorX_ = xPos;
		lastCursorY_ = yPos;
		firstFrame_ = false;
	}
	const float sensitivity = 0.05f;
	float xOffset = (xPos - lastCursorX_) * sensitivity;
	float yOffset = (lastCursorY_ - yPos) * sensitivity;
	lastCursorX_ = xPos;
	lastCursorY_ = yPos;
	yaw_ += xOffset;
	pitch_ += yOffset;
	if (pitch_ > 89.0f) {
		pitch_ = 89.0f;
	}
	if (pitch_ < -89.0f) {
		pitch_ = -89.0f;
	}
}

// Whenever the mouse scrolls, this callback function executes
void HelloCamera::ScrollCallback_(GLFWwindow* window, double xOffset, double yOffset) {
	if (fov_ < 1.0f) {
		fov_ = 1.0f;
	} else if (fov_ > 45.0f) {
		fov_ = 45.0f;
	} else {
		fov_ -= yOffset;
	}
}
