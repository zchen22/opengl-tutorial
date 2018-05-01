#pragma once

#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Logger.h"
#include "Shader.h"

class Cube {
public:
	Cube(Logger& logger);
	~Cube();
	// Methods
	int SetUpShader(const char* vertexSourcePath, const char* fragmentSourcePath);
	int SetUpVertices();
	int SetUpTextures();
	int SetUpColor(const GLfloat* color, const GLfloat* lightColor);
	int SetUpModelMatrix(const glm::mat4& model = glm::mat4(1.0f));
	int SetUpViewMatrix(const glm::mat4& view);
	int SetUpProjectionMatrix(const glm::mat4& projection);
	// Data
	std::unique_ptr<Shader> shader;
	unsigned int vao;
	unsigned int tex1;
	unsigned int tex2;
private:
	unsigned int vbo_;
	Logger logger_;
};
