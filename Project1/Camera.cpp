#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup) {
	Position = position;
	Worldup = worldup;
	Forward = glm::normalize(glm::normalize(target - position)); 
	Right = glm::normalize(glm::cross(Forward, Worldup)); 
	Up = glm::normalize(glm::cross(Right, Forward));
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup) {
	Position = position;
	Worldup = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, Worldup));
	Up = glm::normalize(glm::cross(Right, Forward));
}

Camera::~Camera() {

}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(Position, Position + Forward, Worldup);
}

void Camera::UpdateCameraVectors() {
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, Worldup));
	Up = glm::normalize(glm::cross(Right, Forward));
}

void Camera::ProcessMouseMovement(float deltaX, float deltaY) {
	Pitch -= deltaY * senseX;
	Yaw -= deltaX * senseY;
	UpdateCameraVectors();
}

void Camera::UpdateCameraPos() {
	Position += Forward*speedZ*senseZ;
}