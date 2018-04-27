#pragma once

#include <string>

#include "Logger.h"

class Shader {
public:
	Shader(Logger& logger);
	~Shader();
	int AddVertex(const char* vertexSourcePath);
	int AddFragment(const char* fragmentSourcePath);
	int Link();
	int program;
private:
	std::string vertexSource_;
	int vertex_;
	std::string fragmentSource_;
	int fragment_;
	Logger logger_;
};
