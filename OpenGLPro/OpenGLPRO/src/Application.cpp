#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include <string>
#include "Shader.h"
#include "vendor/stb_image/stb_image.h"
#include "Texture.h"
#include "Renderer.h"
#include "Camera.h"


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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 960, 640);
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	float textures[]{
		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,
		 1.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 0.0f,

		 0.0f, 0.0f,
		 1.0f, 0.0f,
		 1.0f, 1.0f,
		 1.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 0.0f,

		 1.0f, 0.0f,
		 1.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,

		 1.0f, 0.0f,
		 1.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 1.0f,
		 0.0f, 0.0f,
		 1.0f, 0.0f,

		 0.0f, 1.0f,
		 1.0f, 1.0f,
		 1.0f, 0.0f,
		 1.0f, 0.0f,
		 0.0f, 0.0f,
		 0.0f, 1.0f,

		 0.0f, 1.0f,
		 1.0f, 1.0f,
		 1.0f, 0.0f,
		 1.0f, 0.0f,
		 0.0f, 0.0f,
		 0.0f, 1.0f

	};

	unsigned int indecies[] = {
		0,1,2,
		2,3,0
	};

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glfwSwapInterval(1);
	//unsigned int vaos[2];
	//unsigned int vbos[2];
	//GlCall(glGenVertexArrays(2, vaos));
	//GlCall(glGenBuffers(2, vbos));

	////GL_STATIC_DRAW - set once used many 
	////GL_STREAM_DRAW - set once use few times
	////GL_DYNAMIC_DRAW - set many use many
	//GlCall(glBindVertexArray(vaos[0]));

	//GlCall(glBindBuffer(GL_ARRAY_BUFFER,vbos[0]));

	//GlCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW));//function copies previously defined vertex data(verticies) to the buffers memory currently bounded
	//GlCall(glEnableVertexAttribArray(0));
	//GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	//GlCall(glEnableVertexAttribArray(1));
	//GlCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));// 2nd-size of attrib, 5 - size of all vertex, 6- offset from first index

	//GlCall(glBindVertexArray(vaos[1]));
	//GlCall(glBindBuffer(GL_ARRAY_BUFFER, vbos[1]));
	//GlCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW));//function copies previously defined vertex data(verticies) to the buffers memory currently bounded
	//GlCall(glEnableVertexAttribArray(0));
	//GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	//GlCall(glEnableVertexAttribArray(1));
	//GlCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));

	
	
	
	GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;
	
	glGenVertexArrays(1, &vao);
	GlCall(glGenBuffers(1, &vbo));
	GlCall(glGenBuffers(1, &ibo));

	GlCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));//now using GL_ARRAY_BUFFER will be used to  configure currently bound buffer(vbo)
	GlCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));//function copies previously defined vertex data(verticies) to the buffers memory currently bounded
	GlCall(glBindVertexArray(vao));
	
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW));
	GlCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0));// binds to the active GL_ARRAY_BUFFER
	GlCall(glEnableVertexAttribArray(0));

	GlCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3* sizeof(float))));
	glEnableVertexAttribArray(1);
	GlCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	glEnableVertexAttribArray(2);

	/*GlCall(glEnableVertexAttribArray(1));
	GlCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2*sizeof(float))));*/

	/*GlCall(glEnableVertexAttribArray(1));
	GlCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float))));*/

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// set the vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GlCall(glBindVertexArray(0));

	glEnable(GL_DEPTH_TEST);

	//unsigned int yellowShader;
	//yellowShader = CreateShader(vertexShaderSource, fragmentYellowShaderSource);
	Shader shader("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
	Shader lightCubeShader("res/shaders/LightCubeVertex.shader", "res/shaders/LightCube.shader");

	Texture texture1("res/textures/container2.png");
	
	Texture texture2("res/textures/container2_specular.png");
	Texture texture3("res/textures/matrix.jpg");
	shader.Bind();
	shader.SetUniform1i("material.diffuse", 0);
	shader.SetUniform1i("material.specular", 1);
	shader.SetUniform1i("material.emission", 2);
	
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	

	float radius = 10.0f;
	glm::mat4 projection = glm::mat4(1.0f);
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	GlCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		processInput(window);
		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.fov), 960.0f / 640.0f, 0.1f, 100.0f);

		view = camera.GetViewMatrix();
		lightCubeShader.Bind();

		lightCubeShader.SetUniformMat4f("view", view);
		lightCubeShader.SetUniformMat4f("projection", projection);
		float time = glfwGetTime();
		
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		lightCubeShader.SetUniformMat4f("model", model);
		lightCubeShader.SetVector3f("color", glm::vec3(1.0f,1.0f,1.0f));
		glBindVertexArray(lightVAO);


		glDrawArrays(GL_TRIANGLES, 0, 36);

		


		shader.Bind();
		shader.SetVector3f("viewPos", camera.Position);
		shader.SetUniform1f("material.brightnes", 64.0f);
		
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


		//shader.SetUniform1f("light.outerAngle", glm::cos(glm::radians(17.5f)));
		//shader.SetUniform1f("light.innerAngle", glm::cos(glm::radians(12.5f)));
		shader.SetUniform1f("time", glfwGetTime());
		
		
		
		//shader.SetUniform1f("u_mixValue", mixValue);
		model = glm::mat4(1.0f);

		shader.SetUniformMat4f("view", view);
		shader.SetUniformMat4f("projection", projection);
		//shader.SetUniformMat4f("model", model);
		texture1.Bind(0);
		texture2.Bind(1);
		texture3.Bind(2);
		glBindVertexArray(vao);
		

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i % 2 == 0)
			{
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(5.0f), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else
			{
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-5.0f), glm::vec3(0.3f, 1.0f, 0.5f));
			}

			shader.SetUniformMat4f("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &lightVAO);
	shader.Unbind();
	glfwTerminate();
	return 0;
}
