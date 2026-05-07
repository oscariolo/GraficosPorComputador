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
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);    
}

double prevMouseX = 0.0;
double prevMouseY = 0.0;
bool leftMouseButtonPressed = false;
glm::mat4 accumulatedRotation = glm::mat4(1.0f);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        leftMouseButtonPressed = true;
        glfwGetCursorPos(window, &prevMouseX, &prevMouseY);
    }
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        leftMouseButtonPressed = false;
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
    glfwSetMouseButtonCallback(window,mouse_button_callback);
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    glViewport(0, 0, 800, 600);    

    unsigned int shaders;
    setUpShaders(shaders);

    unsigned int transformLoc = glGetUniformLocation(shaders, "transform");
    
    GridSphere sphere;
    sphere.setShape();
    sphere.setUpBuffers();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); //para que el triangle fan se muestre como lineas 
    //Render Loop
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if(leftMouseButtonPressed) //control de movimiento con mouse para rotar la esfera
        
        {
            double currX, currY;
            glfwGetCursorPos(window, &currX, &currY);

            float dx = static_cast<float>(currX - prevMouseX);
            float dy = static_cast<float>(currY - prevMouseY);

            prevMouseX = currX;
            prevMouseY = currY;

            float drag = sqrtf(dx * dx + dy * dy);
            if (drag > 0.0001f)
            {//transformacion es mas directo que estar moneando vertices
                glm::vec3 axis = glm::normalize(glm::vec3(dy, dx, 0.0f));
                const float sensitivity = 0.005f;
                float angle = sensitivity * drag;

                glm::mat4 incremental = glm::rotate(glm::mat4(1.0f), angle, axis);
                accumulatedRotation = incremental * accumulatedRotation;
            }

        }

        //Rendering commands 
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(sphere.VAO);
        glUseProgram(shaders);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(accumulatedRotation));
        
        const unsigned int fanCount = sphere.NUM_LINES + 2;
        glDrawElements(GL_TRIANGLE_FAN, fanCount, GL_UNSIGNED_INT, (void*)(sphere.indexPolarStartOffset*sizeof(unsigned int)));//polar fans
        glDrawElements(GL_TRIANGLE_FAN, fanCount, GL_UNSIGNED_INT, (void*)(sphere.indexPolarStartOffset + fanCount * sizeof(unsigned int)));

        for(int i=0;i<sphere.NUM_LINES;i++){ 
            glDrawElements(GL_LINE_STRIP, sphere.NUM_LINES , GL_UNSIGNED_INT, (void*)(sphere.indexMeridianStartOffset + i*(sphere.NUM_LINES)*sizeof(unsigned int)));//meridian lines
            glDrawElements(GL_LINE_LOOP, sphere.NUM_LINES, GL_UNSIGNED_INT, (void*)(sphere.indexParalelStartOffset + i*(sphere.NUM_LINES)*sizeof(unsigned int)));//paralel lines

            
        }

        glfwSwapBuffers(window);
    
    }

    glfwTerminate();
    ///
    return 0;
}

