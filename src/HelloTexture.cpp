#include "HelloTexture.h"

#include <iostream>

#include "FileReader.h"
#include "stb_image.h"

HelloTexture::HelloTexture()
	: vertexShaderSource_(nullptr),
	fragmentShaderSource_(nullptr),
	vertexShader_(0),
	fragmentShader_(0),
	shaderProgram_(0),
	vao_(0),
	vbo_(0),
	ebo_(0) {
}

HelloTexture::~HelloTexture() {
}

int HelloTexture::Main() {
	Init_();
	CreateVertexShader_();
	CreateFragmentShader_();
	LinkShaders_();
	SetUpVertexData_();
	CreateTexture1_();
	CreateTexture2_();
	BindSamplerToTexUnit_();
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
int HelloTexture::Init_() {
	// Initialize and configure GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Create a window
	window_ = glfwCreateWindow(800, 600, "HelloTexture", NULL, NULL);
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
	return 0;
}

// Create a vertex shader
int HelloTexture::CreateVertexShader_() {
	FileReader f("src/HelloTextureVertex.glsl");
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
int HelloTexture::CreateFragmentShader_() {
	FileReader f("src/HelloTextureFragment.glsl");
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
int HelloTexture::LinkShaders_() {
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
int HelloTexture::SetUpVertexData_() {
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
int HelloTexture::CreateTexture1_() {
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
int HelloTexture::CreateTexture2_() {
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
int HelloTexture::BindSamplerToTexUnit_() {
	glUseProgram(shaderProgram_);
	glUniform1i(glGetUniformLocation(shaderProgram_, "sampler1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram_, "sampler2"), 1);
	return 0;
}

// Process all input
int HelloTexture::ProcessInput_(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	return 0;
}

// Render
int HelloTexture::Render_() {
	// Clear
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// Bind textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1_);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2_);
	// Draw
	glUseProgram(shaderProgram_);
	glBindVertexArray(vao_);  // No need to bind it every time 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);  // No need to unbind it every time 
	return 0;
}

// Clean up resources
int HelloTexture::CleanUp_() {
	glDeleteVertexArrays(1, &vao_);
	glDeleteBuffers(1, &vbo_);
	glDeleteBuffers(1, &ebo_);
	glfwTerminate();
	return 0;
}

// Whenever the window size changed, this callback function executes
void HelloTexture::FramebufferSizeCallback_(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}
