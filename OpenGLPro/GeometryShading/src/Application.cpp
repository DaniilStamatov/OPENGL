#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include <iostream>
#include <map>
#include "Model.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int load_cubemap(std::vector<std::string> faces);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

glm::vec3 calculatePositionOnCircle(float radius, float angleInRadians) {
    float x = radius * cos(angleInRadians);
    float y = 0; // keep height of field smaller compared to width of x and z
    float z = radius * sin(angleInRadians);
    return glm::vec3(x, y, z);
}
void decompose(const glm::mat4& mat, glm::vec3& scale, glm::quat& rotation, glm::vec3& translation) {
    // Extract translation
    translation = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);

    // Create a copy of the top-left 3x3 submatrix
    glm::mat3 submat(
        mat[0][0], mat[0][1], mat[0][2],
        mat[1][0], mat[1][1], mat[1][2],
        mat[2][0], mat[2][1], mat[2][2]
    );

    // Extract scale
    scale = glm::vec3(glm::length(submat[0]), glm::length(submat[1]), glm::length(submat[2]));

    // Extract rotation
    rotation = glm::quat_cast(mat);
}

glm::mat4 quatToMat4(const glm::quat& q) {
    glm::mat4 mat(1.0f);
    float qx = q.x, qy = q.y, qz = q.z, qw = q.w;
    mat[0][0] = 1.0f - 2.0f * (qy * qy + qz * qz);
    mat[0][1] = 2.0f * (qx * qy - qz * qw);
    mat[0][2] = 2.0f * (qx * qz + qy * qw);
    mat[1][0] = 2.0f * (qx * qy + qz * qw);
    mat[1][1] = 1.0f - 2.0f * (qx * qx + qz * qz);
    mat[1][2] = 2.0f * (qy * qz - qx * qw);
    mat[2][0] = 2.0f * (qx * qz - qy * qw);
    mat[2][1] = 2.0f * (qy * qz + qx * qw);
    mat[2][2] = 1.0f - 2.0f * (qx * qx + qy * qy);
    return mat;
}

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
     // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    stbi_set_flip_vertically_on_load(true);
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    Shader shader("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
    Shader instanceShader("res/shaders/Instance.shader", "res/shaders/Fragment.shader");

    Shader skyboxShader("res/shaders/SkyboxVertex.shader", "res/shaders/SkyboxFragment.shader");



    //Shader shader2("res/shaders/VertexGeom2.shader", "res/shaders/Fragment.shader", "res/shaders/GeomShader2.shader");


    // build and compile shaders
    // -------------------------
    
    float quadVertices[] = {
        // positions     // colors
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f
    };

    //glm::vec2 translations[100];
    //int index = 0;
    //float offset = 0.1f;
    //for (int y = -10; y < 10; y += 2)
    //{
    //    for (int x = -10; x < 10; x += 2)
    //    {
    //        glm::vec2 translation;
    //        translation.x = (float)x / 10.0f + offset;
    //        translation.y = (float)y / 10.0f + offset;
    //        translations[index++] = translation;
    //    }
    //}

    //unsigned int instanceVBO;
    //glGenBuffers(1, &instanceVBO);
    //glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //// set up vertex data (and buffer(s)) and configure vertex attributes
    //// ------------------------------------------------------------------
    //unsigned int VAO, VBO;
    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);
    //glBindVertexArray(VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2*sizeof(float)));
    //glEnableVertexAttribArray(2);
    //glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glVertexAttribDivisor(2, 1);

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


    std::vector<std::string> textures_faces{
        "res/textures/Skybox2/skybox_left.png",
        "res/textures/Skybox2/skybox_right.png",
        "res/textures/Skybox2/skybox_down.png",
        "res/textures/Skybox2/skybox_up.png",
        "res/textures/Skybox2/skybox_front.png",
        "res/textures/Skybox2/skybox_back.png"
    };

    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    Model rock("res/models/Rock/rock.obj");
    Model planet("res/models/Planet/planet.obj");
    Texture text("res/models/Rock/rock.png");
    unsigned int cubemapTexture = load_cubemap(textures_faces);
    unsigned int amount = 1000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed	
    float radius = 20.0;
    float offset = 2.5f;
    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float radiuses[1000];
    glm::quat rotations[1000];
    glm::vec3 scales[1000];
    glm::vec3 translations[1000];
    float rotAngles[1000];

    for (unsigned int i = 0; i < amount; i++)
    {
        
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacementX = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float displacementY = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float displacementZ = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;

        glm::mat4 model = glm::mat4(1.0f);
        float x = sin(angle) * radius + displacementX;
        float y = displacementY * 0.4f; // keep height of field smaller compared to width of x and z
        float z = 4.0f + cos(angle) * radius + displacementZ;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        scales[i] = glm::vec3(scale);

        //// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        rotAngles[i] = rotAngle;
        // 4. now add to list of matrices
        modelMatrices[i] = model;
        glm::vec3 direction = glm::vec3(modelMatrices[i][3]) - glm::vec3(0.0f, -3.0f, 0.0f);

        // Calculate radius and angle for this object
        float rad = glm::length(direction);
        radiuses[i] = rad;
        glm::vec3 scal;
        glm::quat rotation;
        glm::vec3 translation;
        decompose(model, scal, rotation, translation);
        translations[i] = translation;
    }


    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    std::size_t vec4Size = sizeof(glm::vec4);
    for (unsigned int i = 0; i < rock.meshes.size(); i++)
    {
        unsigned int VAO = rock.meshes[i].vao;
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1*vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2*vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3*vec4Size));
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        shader.Bind();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();


        shader.SetVector3f("viewPos", camera.Position);

        shader.SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
        shader.SetVector3f("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        shader.SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        shader.SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f);

        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::rotate(model, (float)glfwGetTime() * -0.005f, glm::vec3(0.1f, 1.0f, 0.1f));

        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        shader.SetUniformMat4f("model", model);
        shader.SetUniformMat4f("projection", projection);
        shader.SetUniformMat4f("view", view);
        planet.Draw(shader);

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.Bind();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.SetUniformMat4f("view", view);
        skyboxShader.SetUniformMat4f("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        view = camera.GetViewMatrix();
        // draw meteorites
        instanceShader.Bind();
        instanceShader.SetUniform1f("centerX", 0.0f);
        instanceShader.SetUniform1f("centerY", 0.0f);
        instanceShader.SetVector3f("viewPos", camera.Position);
        instanceShader.SetVector3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
        instanceShader.SetVector3f("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        instanceShader.SetVector3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        instanceShader.SetVector3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
        instanceShader.SetUniformMat4f("projection", projection);
        instanceShader.SetUniformMat4f("view", view);
        instanceShader.SetUniform1i("texture_diffuse1", 0);
        instanceShader.SetUniform1f("time", glfwGetTime());
        text.Bind(0);

        for (unsigned int i = 0; i < amount; i++)
        {
            
            glm::mat4 modelmt = modelMatrices[i];

            glm::vec3 direction = glm::vec3(modelmt[3]) - glm::vec3(0.0f, 0.0f, 0.0f);

            // Calculate radius and angle for this object
            float angle = atan2(direction.z, direction.x);

            // Calculate final position of object based on radius and angle
            float x = cos(angle + glfwGetTime() * 0.000001f) * radiuses[i];
            float z = sin(angle + glfwGetTime() * 0.000001f) * radiuses[i];
            glm::vec3 finalPosition(x, 0, z);

            // Extract scale, rotation, and translation from the original model matrix
           
            glm::vec3 translation = glm::vec3(finalPosition.x, translations[i].y, finalPosition.z);
            modelmt = glm::translate(glm::mat4(1.0f), translation);
            modelmt = glm::rotate(modelmt, rotAngles[i], glm::vec3(0.4f, 0.6f, 0.8f));
            modelmt = glm::rotate(modelmt, (float)glfwGetTime() * 0.05f, glm::vec3(0.4f, 0.6f, 0.8f));
            modelmt = glm::scale(modelmt, scales[i]);
            modelMatrices[i] = modelmt;
            // Update model matrix with final position
            instanceShader.SetUniformMat4f("model", modelMatrices[i]);
            
            rock.Draw(instanceShader);
           /* glBindVertexArray(rock.meshes[i].vao);
            glDrawElementsInstanced(
                GL_TRIANGLES, rock.meshes[i].indecies.size(), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);*/
        }

       
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
   
   
    glfwTerminate();
    return 0;
}


unsigned int load_cubemap(std::vector<std::string> faces)
{
    unsigned int cubemapID;
    glGenTextures(1, &cubemapID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return cubemapID;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACK, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseInput(xoffset, yoffset, true);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
