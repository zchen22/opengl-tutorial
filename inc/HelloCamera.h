#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Logger.h"

class HelloCamera {
public:
	HelloCamera();
	HelloCamera(const int width, const int height);
	~HelloCamera();
	int Main();
private:
	// Internal functions
	int Init_();
	int CreateVertexShader_();
	int CreateFragmentShader_();
	int LinkShaders_();
	int SetUpVertexData_();
	int CreateTexture1_();
	int CreateTexture2_();
	int BindSamplerToTexUnit_();
	int ProcessInput_(GLFWwindow *window);
	int CreateModelMatrix_();
	int CreateViewMatrix_();
	int CreateProjectionMatrix_();
	int Render_();
	int CleanUp_();
	static void FramebufferSizeCallback_(GLFWwindow* window, int width, int height);  // This function must be static in order to be used as a function pointer in a GLFW API
	// Data
	Logger logger_;
	const int windowWidth_;
	const int windowHeight_;
	GLFWwindow* window_;
	char* vertexShaderSource_;
	char* fragmentShaderSource_;
	int vertexShader_;
	int fragmentShader_;
	int shaderProgram_;
	unsigned int vao_;
	unsigned int vbo_;
	unsigned int tex1_;
	unsigned int tex2_;
};
