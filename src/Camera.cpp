#include "Camera.h"

#include <glad/glad.h>

#include "FileReader.h"

Camera::Camera(Logger& logger)
	: pos(glm::vec3(0.0f, 0.0f, 3.0f)),
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	up(glm::vec3(0.0f, 1.0f, 0.0f)), 
	logger_(logger) {
}

Camera::~Camera() {
}

int Camera::MoveForward() {
	assert(speed != 0);
	pos += front * speed;
	return 0;
}

int Camera::MoveBackward() {
	assert(speed != 0);
	pos -= front * speed;
	return 0;
}

int Camera::MoveLeft() {
	assert(speed != 0);
	pos -= glm::normalize(glm::cross(front, up)) * speed;
	return 0;
}

int Camera::MoveRight() {
	assert(speed != 0);
	pos += glm::normalize(glm::cross(front, up)) * speed;
	return 0;
}

int Camera::CalculateFront(const float pitch, const float yaw) {
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front);
	return 0;
}
