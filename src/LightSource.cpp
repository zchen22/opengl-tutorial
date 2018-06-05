#include "LightSource.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

LightSource::LightSource(Logger& logger)
	: shader(std::make_unique<Shader>(logger_)),
	vao(0),
	vbo_(0),
	logger_(logger) {
}

LightSource::~LightSource() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo_);
}

int LightSource::SetUpShader(const char* vertexSourcePath, const char* fragmentSourcePath) {
	shader->AddVertex(vertexSourcePath);
	shader->AddFragment(fragmentSourcePath);
	shader->Link();
	return 0;
}

int LightSource::SetUpVertices() {
	// Cube
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	numVertices = 36;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo_);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return 0;
}

int LightSource::SetUpColor(const float r, const float g, const float b) {
	color = glm::vec3(r, g, b);
	glUseProgram(shader->program);
	glUniform3fv(glGetUniformLocation(shader->program, "color"), 1, glm::value_ptr(color));
	return 0;
}

int LightSource::SetUpPosition(const float x, const float y, const float z) {
	position = glm::vec3(x, y, z);
	return 0;
}

int LightSource::SetUpModelMatrix(const glm::mat4& model) {
	const int loc = glGetUniformLocation(shader->program, "model");
	glUseProgram(shader->program);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model));
	return 0;
}

int LightSource::SetUpViewMatrix(const glm::mat4& view) {
	const int loc = glGetUniformLocation(shader->program, "view");
	glUseProgram(shader->program);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));
	return 0;
}

int LightSource::SetUpProjectionMatrix(const glm::mat4& projection) {
	const int loc = glGetUniformLocation(shader->program, "projection");
	glUseProgram(shader->program);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
	return 0;
}
