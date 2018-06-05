#pragma once

#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Logger.h"
#include "Shader.h"

class LightSource {
public:
	LightSource(Logger& logger);
	~LightSource();
	// Methods
	int SetUpShader(const char* vertexSourcePath, const char* fragmentSourcePath);
	int SetUpVertices();
	int SetUpColor(const float r, const float g, const float b);
	int SetUpPosition(const float x, const float y, const float z);
	int SetUpModelMatrix(const glm::mat4& model);
	int SetUpViewMatrix(const glm::mat4& view);
	int SetUpProjectionMatrix(const glm::mat4& projection);
	// Data
	glm::vec3 position;
	glm::vec3 color;
	std::unique_ptr<Shader> shader;
	unsigned int vao;
	unsigned int numVertices;
private:
	unsigned int vbo_;
	Logger logger_;
};
