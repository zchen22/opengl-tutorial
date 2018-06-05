#include "Cube.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

Cube::Cube(Logger& logger)
	: shader(std::make_unique<Shader>(logger_)),
	vao(0),
	tex1(0),
	tex2(0),
	vbo_(0),
	logger_(logger) {
}

Cube::~Cube() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo_);
}

int Cube::SetUpShader(const char* vertexSourcePath, const char* fragmentSourcePath) {
	shader->AddVertex(vertexSourcePath);
	shader->AddFragment(fragmentSourcePath);
	shader->Link();
	return 0;
}

int Cube::SetUpVertices() {
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};
	numVertices = 36;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return 0;
}

// Initialize resources
int Cube::SetUpTextures() {
	// Set up the first texture
	glGenTextures(1, &tex1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width = 0, height = 0, numChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("res/container.jpg", &width, &height, &numChannels, 0);
	if (!data) {
		logger_.Error("Failed to load texture 1\n");
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	// Set up the second texture
	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("res/awesomeface.png", &width, &height, &numChannels, 0);
	if (!data) {
		logger_.Error("Failed to load texture 2\n");
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	// Bind samplers to texture units
	glUseProgram(shader->program);
	glUniform1i(glGetUniformLocation(shader->program, "sampler1"), 0);
	glUniform1i(glGetUniformLocation(shader->program, "sampler2"), 1);
	return 0;
}

int Cube::SetUpLight(const LightSource& lightSource) {
	glUseProgram(shader->program);
	glUniform3fv(glGetUniformLocation(shader->program, "lightPos"), 1, glm::value_ptr(lightSource.position));
	glUniform3fv(glGetUniformLocation(shader->program, "lightColor"), 1, glm::value_ptr(lightSource.color));
	return 0;
}

int Cube::SetUpCamera(const Camera& camera) {
	glUseProgram(shader->program);
	glUniform3fv(glGetUniformLocation(shader->program, "viewPos"), 1, glm::value_ptr(camera.pos));
	return 0;
}

int Cube::SetUpColor(const float r, const float g, const float b) {
	const GLfloat color[] = { r, g, b };
	glUseProgram(shader->program);
	glUniform3fv(glGetUniformLocation(shader->program, "objectColor"), 1, color);
	return 0;
}

int Cube::SetUpModelMatrix(const glm::mat4& model, const glm::mat4& lightModel) {
	int loc = glGetUniformLocation(shader->program, "model");
	glUseProgram(shader->program);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
	loc = glGetUniformLocation(shader->program, "modelInv");
	glUseProgram(shader->program);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(glm::inverse(model)));
	loc = glGetUniformLocation(shader->program, "lightModel");
	glUseProgram(shader->program);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(lightModel));
	return 0;
}

int Cube::SetUpViewMatrix(const glm::mat4& view) {
	const int loc = glGetUniformLocation(shader->program, "view");
	glUseProgram(shader->program);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
	return 0;
}

int Cube::SetUpProjectionMatrix(const glm::mat4& projection) {
	const int loc = glGetUniformLocation(shader->program, "projection");
	glUseProgram(shader->program);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
	return 0;
}