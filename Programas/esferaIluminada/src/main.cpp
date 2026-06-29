#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/shader_utils.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <chrono>
#include <classes/Sphere.h>
#include <classes/Transform.h>
#include <classes/Lighting.h>

#include <classes/Cube.h>


const int VIEWPORT_SIZE[] = {720,720};

bool wireframeMode = false;

bool normalByFace = false; 

glm::vec3 focusAxis = {1,0,0};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
    
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        wireframeMode = !wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
    }

    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        normalByFace = !normalByFace;
        std::cout << "Normal by face: " << (normalByFace ? "ON" : "OFF") << std::endl;
    }



}


int main()
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(VIEWPORT_SIZE[0], VIEWPORT_SIZE[1], "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    glfwSetKeyCallback(window,key_callback);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    glViewport(0, 0, VIEWPORT_SIZE[0], VIEWPORT_SIZE[1]);   
    
    //Manejo de loop para limitar a FPS
    const int FPS = 60;
    const double timePerFrame = 1.0 / FPS;
    double accumulator = 0.0;
    auto lastTime = std::chrono::steady_clock::now();
     
    unsigned int lightShader;
    shader_utils::setUpShaders(lightShader,"lighting.shader","fragment.shader");

    Sphere sphere;
    sphere.generateSphere(4);
    sphere.setShaders(lightShader);
    sphere.setUniformColor(glm::vec3(0.7,0.45,0.1));

    Lighting light;
    glm::vec3 lightColor = glm::vec3(1.0f,1.0f,1.0f);
    glm::vec3 lightPos = glm::vec3(0,0,-3);
    glm::vec3 cameraPos = glm::vec3(0,0,-3);

    light.setSourceColor(lightColor,lightShader);
    light.setPosition(lightPos);

    sphere.instantiate();

    glEnable(GL_DEPTH_TEST); //Para considerar profundidad en z en el renderizado
    //Render Loop
    while(!glfwWindowShouldClose(window))
    {
        auto currentTime = std::chrono::steady_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        accumulator += deltaTime;

        light.applyPhong(0.4f,cameraPos,lightShader,normalByFace);

        // Only update and render if enough time has passed
        while(accumulator >= timePerFrame) {
            glfwPollEvents();
            //Rendering
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Transform::rotate(sphere.model,glm::vec3(0,1,0),0.5f);
            sphere.draw();

            glfwSwapBuffers(window);

            accumulator -= timePerFrame;

        }
    }

    glfwTerminate();
    ///
    return 0;
}

