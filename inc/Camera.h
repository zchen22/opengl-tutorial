#pragma once

#include <glm/glm.hpp>

#include "Logger.h"

class Camera {
public:
	Camera(Logger& logger);
	~Camera();
	int MoveForward();
	int MoveBackward();
	int MoveLeft();
	int MoveRight();
	int CalculateFront(const float pitch, const float yaw);
	float speed;
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
private:
	Logger logger_;
};
