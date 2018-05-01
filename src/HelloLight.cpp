#include "HelloLight.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "FileReader.h"

bool HelloLight::firstFrame_ = true;
float HelloLight::lastCursorX_ = 0;
float HelloLight::lastCursorY_ = 0;
float HelloLight::yaw_ = -90;
float HelloLight::pitch_ = 0;
float HelloLight::fov_ = 45;

HelloLight::HelloLight(const int width = 800, const int height = 600)
	: windowWidth_(width),
	windowHeight_(height),
	window_(nullptr),
	lightSource_(nullptr),
	cube_(nullptr),
	camera_(nullptr),
	deltaFrameTime_(0.0f),
	lastFrameTime_(0.0f) {
	lastCursorX_ = width / 2;
	lastCursorY_ = height / 2;
}

HelloLight::~HelloLight() {
	glfwTerminate();
}

int HelloLight::Main() {
	Init_();
	SetUpScene_();
	SetUpCamera_();
	// Render loop
	while (!glfwWindowShouldClose(window_)) {
		ProcessInput_(window_);
		UpdateMVPMatrices_();
		Render_();
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
	return 0;
}

// Initialize resources
int HelloLight::Init_() {
	// Initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a window
	window_ = glfwCreateWindow(windowWidth_, windowHeight_, "HelloLight", NULL, NULL);
	if (window_ == NULL) {
		logger_.Error("Failed to create a GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window_);
	glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback_);
	// Register mouse callback functions
	glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window_, MouseCallback_);
	glfwSetScrollCallback(window_, ScrollCallback_);
	// Load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		logger_.Error("Failed to initialize GLAD\n");
		exit(EXIT_FAILURE);
	}
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	logger_.Debug("GL initialized.\n");
	return 0;
}

// Set up the scene
int HelloLight::SetUpScene_() {
	// Create a light source
	lightSource_ = std::make_unique<LightSource>(logger_);
	lightSource_->SetUpShader("src/LightSourceVertex.glsl", "src/LightSourceFragment.glsl");
	lightSource_->SetUpVertices();
	const GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f };
	lightSource_->SetUpColor(lightColor);
	// Create a cube
	cube_ = std::make_unique<Cube>(logger_);
	cube_->SetUpShader("src/CubeVertex.glsl", "src/CubeFragment.glsl");
	cube_->SetUpVertices();
	const GLfloat cubeColor[] = { 1.0f, 0.5f, 0.31f };
	cube_->SetUpColor(cubeColor, lightColor);
	cube_->SetUpTextures();
	logger_.Debug("Scene set up.\n");
	return 0;
}

// Set up the camera
int HelloLight::SetUpCamera_() {
	camera_ = std::make_unique<Camera>(logger_);
	logger_.Debug("Camera set up.\n");
	return 0;
}

// Process inputs
int HelloLight::ProcessInput_(GLFWwindow *window) {
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

// Update the model, view, and projection matrices
int HelloLight::UpdateMVPMatrices_() {
	// Get delta frame time for camera
	const float currentFrameTime = glfwGetTime();
	deltaFrameTime_ = currentFrameTime - lastFrameTime_;
	lastFrameTime_ = currentFrameTime;
	// Update the model matrix
	cube_->SetUpModelMatrix();
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
	model = glm::scale(model, glm::vec3(0.2f));
	lightSource_->SetUpModelMatrix(model);
	// Update the view matrix
	camera_->CalculateFront(pitch_, yaw_);
	const glm::mat4 view = glm::lookAt(camera_->pos, camera_->pos + camera_->front, camera_->up);
	lightSource_->SetUpViewMatrix(view);
	cube_->SetUpViewMatrix(view);
	// Update the projection matrix
	const glm::mat4 projection = glm::perspective(glm::radians(fov_), 1.0f * windowWidth_ / windowHeight_, 0.1f, 100.0f);
	lightSource_->SetUpProjectionMatrix(projection);
	cube_->SetUpProjectionMatrix(projection);
	return 0;
}

// Render
int HelloLight::Render_() {
	// Clear
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube_->tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, cube_->tex2);
	// Draw
	glUseProgram(lightSource_->shader->program);
	glBindVertexArray(lightSource_->vao);  // No need to bind it every time
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);  // No need to unbind it every time
	glUseProgram(cube_->shader->program);
	glBindVertexArray(cube_->vao);  // No need to bind it every time
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);  // No need to unbind it every time
	return 0;
}

// Whenever the window size changed, this callback function executes
void HelloLight::FramebufferSizeCallback_(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}

// Whenever the mouse moves, this callback function executes
void HelloLight::MouseCallback_(GLFWwindow* window, double xPos, double yPos) {
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
void HelloLight::ScrollCallback_(GLFWwindow* window, double xOffset, double yOffset) {
	if (fov_ < 1.0f) {
		fov_ = 1.0f;
	} else if (fov_ > 45.0f) {
		fov_ = 45.0f;
	} else {
		fov_ -= yOffset;
	}
}
