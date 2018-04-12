#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class HelloWindow {
public:
	HelloWindow();
	~HelloWindow();
	int Main();
private:
	void ProcessInput(GLFWwindow *window);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);  // This function must be static in order to be used as a function pointer in a GLFW API
};
