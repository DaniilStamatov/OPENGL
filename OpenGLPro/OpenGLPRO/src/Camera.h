#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glfw3.h"
enum CameraMovement
{
	FORWARD,
	BACK,
	RIGHT,
	LEFT
};
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SENCITIVITY = 0.01f;
const float SPEED = 2.5f;
const float FOV = 45.0f;
class Camera
{
private:
	

	glm::vec3 cameraUp;
	glm::vec3 cameraRight;
	glm::vec3 worldUp;
	float cameraYaw;
	float cameraPitch;
	float speed;
	float mouseSensitivity;
public:
	float fov;
	glm::vec3 Position;
	glm::vec3 Front;

	Camera(glm::vec3 position= glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	void ProcessMouseScroll(float yoffset);
	void ProcessMouseInput(float xoffset, float yoffset, GLboolean constrainPitch);
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	glm::mat4 GetViewMatrix();
private:
	void CalculateCameraVectors();

	glm::mat4 CalculateLookAtMatrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
	{
		glm::vec3 zAxis = glm::normalize(position - target);
		glm::vec3 xAxis = glm::cross(glm::normalize(worldUp), zAxis);
		glm::vec3 yAxis = glm::cross(zAxis, xAxis);

		glm::mat4 translation = glm::mat4(1.0f);
		translation[3][0] = -position.x;
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;

		glm::mat4 rotation = glm::mat4{
			glm::vec4(xAxis.x, yAxis.x, zAxis.x, 0.0f),
			glm::vec4(xAxis.y, yAxis.y, zAxis.y, 0.0f),
			glm::vec4(xAxis.z, yAxis.z, zAxis.z, 0.0f),
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
		};

		return rotation * translation;

	}
};