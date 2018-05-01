#include "HelloTriangle.h"

#include <cassert>

#include "FileReader.h"

HelloTriangle::HelloTriangle()
	: shader_(nullptr),
	vao_(0),
	vbo_(0),
	ebo_(0) {
}

HelloTriangle::~HelloTriangle() {
}

int HelloTriangle::Main() {
	Init_();
	SetUpShader_();
	SetUpVertexData_();
	// Render loop
	while (!glfwWindowShouldClose(window_)) {
		ProcessInput_(window_);
		Render_();
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
	CleanUp_();
	return 0;
}

// Initialize resources
int HelloTriangle::Init_() {
	// Initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a window
	window_ = glfwCreateWindow(800, 600, "HelloTriangle", NULL, NULL);
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
	// Create an empty shader program
	shader_ = std::make_unique<Shader>(logger_);
	return 0;
}

// Set up the shader program
int HelloTriangle::SetUpShader_() {
	assert(shader_ != nullptr);
	shader_->AddVertex("src/HelloTriangleVertex.glsl");
	shader_->AddFragment("src/HelloTriangleFragment.glsl");
	shader_->Link();
	return 0;
}

// Set up vertex data
int HelloTriangle::SetUpVertexData_() {
	float vertices[] = {
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f  // top left
	};
	unsigned int indices[] = {
		0, 1, 2,  // first triangle
		0, 2, 3  // second triangle
	};
	glGenVertexArrays(1, &vao_);
	glGenBuffers(1, &vbo_);
	glGenBuffers(1, &ebo_);
	// Bind VAO
	glBindVertexArray(vao_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Unbind VBO since glVertexAttribPointer registered it, but no unbinding EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO
	glBindVertexArray(0);
	return 0;
}

// Process all input
int HelloTriangle::ProcessInput_(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	return 0;
}

// Render
int HelloTriangle::Render_() {
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shader_->program);
	glBindVertexArray(vao_);  // No need to bind it every time 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);  // No need to unbind it every time 
	return 0;
}

// Clean up resources
int HelloTriangle::CleanUp_() {
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
	glfwTerminate();
	return 0;
}

// Whenever the window size changed, this callback function executes
void HelloTriangle::FramebufferSizeCallback_(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}
