#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), mouseSensitivity(SENCITIVITY), fov(FOV)
{
	Position = position;
	worldUp = up;
	cameraYaw = yaw;
	cameraPitch = pitch;
	CalculateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void Camera::ProcessMouseInput(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	cameraYaw += xoffset;
	cameraPitch += yoffset;
	if (constrainPitch)
	{
		if (cameraPitch > 89.0f)
			cameraPitch = 89.0f;
		if (cameraPitch < -89.0f)
			cameraPitch = -89.0f;
	}
	
	CalculateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() 
{
	return CalculateLookAtMatrix(Position, Position + Front, cameraUp);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = speed * deltaTime;

	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACK)
		Position -= Front * velocity;
	if (direction == RIGHT)
		Position += cameraRight * velocity;
	if (direction == LEFT)
		Position -= cameraRight * velocity;

	//cameraPosition.y = 0; can be used to fps controller
}



void Camera::CalculateCameraVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	direction.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	direction.y = sin(glm::radians(cameraPitch));
	Front = glm::normalize(direction);
	cameraRight = glm::normalize(glm::cross(Front, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, Front));
}