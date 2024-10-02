#include <iostream>
#include <string>
#include "Shader.h"
#include "vendor/stb_image/stb_image.h"
#include "Camera.h"
#include "Model.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastY = 960 / 2;
float lastX = 640 / 2;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));


void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseInput(xOffset, yOffset, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow* window)
{
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACK, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
}

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(960, 640, "OPEN_GL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 960, 640);

	glfwSwapInterval(1);
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	
	GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	
	glEnable(GL_DEPTH_TEST);

	//unsigned int yellowShader;
	//yellowShader = CreateShader(vertexShaderSource, fragmentYellowShaderSource);
	Shader shader("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
	Model myModel("res/models/Cryzis/Crysis2_scar.obj");
	
	shader.Bind();

	float radius = 10.0f;
	glm::mat4 projection = glm::mat4(1.0f);
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	GlCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		glClearColor(1.0f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), 960.0f / 640.0f, 0.1f, 100.0f);

		shader.Bind();
		shader.SetVector3f("viewPos", camera.Position);

		shader.SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
		shader.SetVector3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		shader.SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		shader.SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f);

		for (int i = 0; i < 4; i++)
		{
			shader.SetVector3f("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
			shader.SetVector3f("pointLights[" + std::to_string(i) + "].ambient", 0.05f, 0.05f, 0.05f);
			shader.SetVector3f("pointLights[" + std::to_string(i) + "].diffuse", 0.8f, 0.8f, 0.8f);
			shader.SetVector3f("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
			shader.SetUniform1f("pointLights[" + std::to_string(i) + "].constant", 1.0f);
			shader.SetUniform1f("pointLights[" + std::to_string(i) + "].linear", 0.09f);
			shader.SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
		}

		shader.SetVector3f("spotLight.position", camera.Position);
		shader.SetVector3f("spotLight.direction", camera.Front);
		shader.SetVector3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		shader.SetVector3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		shader.SetVector3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		shader.SetUniform1f("spotLight.constant", 1.0f);
		shader.SetUniform1f("spotLight.linear", 0.09f);
		shader.SetUniform1f("spotLight.quadratic", 0.032f);
		shader.SetUniform1f("spotLight.innerAngle", glm::cos(glm::radians(12.5f)));
		shader.SetUniform1f("spotLight.outerAngle", glm::cos(glm::radians(15.0f)));
		shader.SetUniformMat4f("projection", projection);
		shader.SetUniformMat4f("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		shader.SetUniformMat4f("model", model);
		myModel.Draw(shader);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	shader.Unbind();
	glfwTerminate();
	return 0;
}
