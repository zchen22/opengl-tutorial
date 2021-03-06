#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Logger.h"
#include "Shader.h"

class HelloTexture {
public:
	HelloTexture();
	~HelloTexture();
	int Main();
private:
	// Internal functions
	int Init_();
	int SetUpShader_();
	int SetUpVertexData_();
	int CreateTexture1_();
	int CreateTexture2_();
	int BindSamplerToTexUnit_();
	int ProcessInput_(GLFWwindow *window);
	int Render_();
	int CleanUp_();
	static void FramebufferSizeCallback_(GLFWwindow* window, int width, int height);  // This function must be static in order to be used as a function pointer in a GLFW API
	// Data
	Logger logger_;
	GLFWwindow* window_;
	std::unique_ptr<Shader> shader_;
	unsigned int vao_;
	unsigned int vbo_;
	unsigned int ebo_;
	unsigned int tex1_;
	unsigned int tex2_;
};
