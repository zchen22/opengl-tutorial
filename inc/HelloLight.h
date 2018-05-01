#pragma once

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Cube.h"
#include "LightSource.h"
#include "Logger.h"
#include "Shader.h"

class HelloLight {
public:
	HelloLight(const int width, const int height);
	~HelloLight();
	int Main();
private:
	// Methods
	int Init_();
	int SetUpScene_();
	int SetUpCamera_();
	int ProcessInput_(GLFWwindow *window);
	int UpdateMVPMatrices_();
	int Render_();
	// The following functions must be static to be used as a function pointer in GLFW C APIs
	static void FramebufferSizeCallback_(GLFWwindow* window, int width, int height);
	static void MouseCallback_(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback_(GLFWwindow* window, double xoffset, double yoffset);
	// Data
	const int windowWidth_;
	const int windowHeight_;
	GLFWwindow* window_;
	std::unique_ptr<LightSource> lightSource_;
	std::unique_ptr<Cube> cube_;
	std::unique_ptr<Camera> camera_;
	float deltaFrameTime_;
	float lastFrameTime_;
	static bool firstFrame_;
	static float lastCursorX_;
	static float lastCursorY_;
	static float yaw_;
	static float pitch_;
	static float fov_;
	Logger logger_;
};
