#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "utils/shader_utils.h"
#include <iostream>
#include <filesystem>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);    
}

void baseTriangle(unsigned int &VBO, unsigned int &VAO)
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  // left  
         0.5f, -0.5f, 0.0f,  // right 
         0.0f,  0.5f, 0.0f   // top   
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void generatePoint(unsigned int &VBO, unsigned int &VAO)
{
    float vertices[] = {
        0.0f, 0.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}   




void setUpShaders(unsigned int &shaders)
{
    unsigned int vertexShader;
    shader_utils::sourceShader("vertex.shader",vertexShader,GL_VERTEX_SHADER);

    unsigned int fragmentShader;
    shader_utils::sourceShader("fragment.shader",fragmentShader,GL_FRAGMENT_SHADER);

    shaders = glCreateProgram();
    glAttachShader(shaders,vertexShader);
    glAttachShader(shaders,fragmentShader);
    glLinkProgram(shaders);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    glfwSetKeyCallback(window,key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    glViewport(0, 0, 800, 600);    

    unsigned int triangleVBO,triangleVAO,shaders;
    unsigned int pointVBO,pointVAO;
    baseTriangle(triangleVBO,triangleVAO);
    generatePoint(pointVBO,pointVAO);
    setUpShaders(shaders); 

    //Render Loop
    while(!glfwWindowShouldClose(window))
    {
        //Rendering commands 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaders);
        glBindVertexArray(triangleVAO); 
        glDrawArrays(GL_LINE_LOOP,0,3);

        glBindVertexArray(pointVAO);
        glDrawArrays(GL_POINTS,0,1);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    ///
    return 0;
}

