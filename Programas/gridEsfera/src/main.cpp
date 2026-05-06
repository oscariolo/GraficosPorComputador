#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "include/utils/shader_utils.h"
#include "include/GridSphere.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);    
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

    unsigned int shaders;
    setUpShaders(shaders);
    
    GridSphere sphere;
    sphere.setShape();
    sphere.setUpBuffers();
    glPointSize(4.0f);
    //Render Loop
    while(!glfwWindowShouldClose(window))
    {
        //Rendering commands 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 view = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        GLint viewLoc = glGetUniformLocation(shaders, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(sphere.VAO);
        glUseProgram(shaders);
        
        // glDrawElements(GL_TRIANGLE_FAN,sphere.NUM_LINES+1,GL_UNSIGNED_INT, (void*)(sphere.indexPolarStartOffset));
        // glDrawElements(GL_TRIANGLE_FAN,sphere.NUM_LINES+1,GL_UNSIGNED_INT, (void*)(sphere.indexPolarStartOffset + (sphere.NUM_LINES+1)*sizeof(GLuint)));
        for(int i=0;i<sphere.NUM_LINES;i++){ 
            //glDrawElements(GL_LINE_LOOP, sphere.NUM_LINES, GL_UNSIGNED_INT, (void*)(i*sphere.NUM_LINES*sizeof(GLuint)));//paralel lines
            glDrawElements(GL_LINE_STRIP, sphere.NUM_LINES , GL_UNSIGNED_INT, (void*)(i*(sphere.NUM_LINES)*sizeof(GLuint)));//meridian lines
            
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    
    }

    glfwTerminate();
    ///
    return 0;
}

