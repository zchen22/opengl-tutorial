#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

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
	int GetFrameTime_();
	int ProcessInput_(GLFWwindow *window);
	int CreateModelMatrix_();
	int CreateViewMatrix_();
	int CreateProjectionMatrix_();
	int Render_();
	int CleanUp_();
	// The following functions must be static to be used as a function pointer in GLFW APIs
	static void FramebufferSizeCallback_(GLFWwindow* window, int width, int height);
	static void MouseCallback_(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback_(GLFWwindow* window, double xoffset, double yoffset);
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
	glm::vec3 camPos_;
	glm::vec3 camFront_;
	glm::vec3 camUp_;
	float deltaFrameTime_;
	float lastFrameTime_;
	static bool firstFrame_;
	static float lastCursorX_;
	static float lastCursorY_;
	static float yaw_;
	static float pitch_;
	static float fov_;
};
