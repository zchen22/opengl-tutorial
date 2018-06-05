#include "Shader.h"

#include <glad/glad.h>

#include "FileReader.h"

Shader::Shader(Logger& logger)
	: logger_(logger) {
}

Shader::~Shader() {
	glDeleteShader(vertex_);
	glDeleteShader(fragment_);
}

// Create a vertex shader
int Shader::AddVertex(const char* vertexSourcePath) {
	FileReader f(vertexSourcePath);
	f.ReadAsString(vertexSource_);
	vertex_ = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexSourceArray[] = {vertexSource_.c_str()};
	glShaderSource(vertex_, 1, vertexSourceArray, NULL);
	glCompileShader(vertex_);
	int success = 0;
	char infoLog[512] = "";
	glGetShaderiv(vertex_, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_, sizeof infoLog, NULL, infoLog);
		logger_.Error("Failed to build the vertex shader '%s'\n%s\n", vertexSourcePath, infoLog);
	}
	logger_.Debug("Vertex shader '%s' created.\n", vertexSourcePath);
	return 0;
}

// Create a fragment shader
int Shader::AddFragment(const char* fragmentSourcePath) {
	FileReader f(fragmentSourcePath);
	f.ReadAsString(fragmentSource_);
	fragment_ = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentSourceArray[] = {fragmentSource_.c_str()};
	glShaderSource(fragment_, 1, fragmentSourceArray, NULL);
	glCompileShader(fragment_);
	int success = 0;
	char infoLog[512] = "";
	glGetShaderiv(fragment_, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_, sizeof infoLog, NULL, infoLog);
		logger_.Error("Failed to build the fragment shader '%s'\n%s\n", fragmentSourcePath, infoLog);
	}
	logger_.Debug("Fragment shader '%s' created.\n", fragmentSourcePath);
	return 0;
}

// Link shaders
int Shader::Link() {
	program = glCreateProgram();
	glAttachShader(program, vertex_);
	glAttachShader(program, fragment_);
	glLinkProgram(program);
	int success = 0;
	char infoLog[512] = "";
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, sizeof infoLog, NULL, infoLog);
		logger_.Error("Failed to link shaders\n");
	}
	return 0;
}
