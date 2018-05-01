#include "HelloTransformations.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "FileReader.h"

HelloTransformations::HelloTransformations()
	: shader_(nullptr),
	vao_(0),
	vbo_(0),
	ebo_(0) {
}

HelloTransformations::~HelloTransformations() {
}

int HelloTransformations::Main() {
	Init_();
	SetUpShader_();
	SetUpVertexData_();
	CreateTexture1_();
	CreateTexture2_();
	BindSamplerToTexUnit_();
	// Render loop
	while (!glfwWindowShouldClose(window_)) {
		ProcessInput_(window_);
		SetUpTransform_();
		Render_();
		glfwSwapBuffers(window_);
		glfwPollEvents();
	}
	CleanUp_();
	return 0;
}

// Initialize resources
int HelloTransformations::Init_() {
	// Initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a window
	window_ = glfwCreateWindow(800, 600, "HelloTransformations", NULL, NULL);
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
int HelloTransformations::SetUpShader_() {
	assert(shader_ != nullptr);
	shader_->AddVertex("src/HelloTransformationsVertex.glsl");
	shader_->AddFragment("src/HelloTransformationsFragment.glsl");
	shader_->Link();
	return 0;
}

// Set up vertex data
int HelloTransformations::SetUpVertexData_() {
	float vertices[] = {
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // top left
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// Unbind VBO since glVertexAttribPointer registered it, but no unbinding EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO
	glBindVertexArray(0);
	return 0;
}

// Create a texture
int HelloTransformations::CreateTexture1_() {
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
int HelloTransformations::CreateTexture2_() {
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
int HelloTransformations::BindSamplerToTexUnit_() {
	glUseProgram(shader_->program);
	glUniform1i(glGetUniformLocation(shader_->program, "sampler1"), 0);
	glUniform1i(glGetUniformLocation(shader_->program, "sampler2"), 1);
	return 0;
}

// Process all input
int HelloTransformations::ProcessInput_(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	return 0;
}

// Set up the transformation matrix
int HelloTransformations::SetUpTransform_() {
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.8, 0.8, 0.8));
	const unsigned int transformLoc = glGetUniformLocation(shader_->program, "transform");
	glUseProgram(shader_->program);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	return 0;
}

// Render
int HelloTransformations::Render_() {
	// Clear
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2_);
	// Draw
	glUseProgram(shader_->program);
	glBindVertexArray(vao_);  // No need to bind it every time 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);  // No need to unbind it every time 
	return 0;
}

// Clean up resources
int HelloTransformations::CleanUp_() {
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
	glfwTerminate();
	return 0;
}

// Whenever the window size changed, this callback function executes
void HelloTransformations::FramebufferSizeCallback_(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}
