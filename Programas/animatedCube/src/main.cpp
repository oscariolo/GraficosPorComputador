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
#include "classes/Cube.h"
#include <chrono>


enum class ToolMode{
    Scaling,
    Translating,
    Rotating,
    None,
};


const int VIEWPORT_SIZE[] = {1080,1080};

bool wireframeMode = false;

bool renderWithGPU = true; // true for CPU, false for GPU

ToolMode currentMode = ToolMode::None;

glm::vec3 focusAxis = {1,0,0};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

void manageAnimation(){

    

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);    

    if (key == GLFW_KEY_S && action == GLFW_PRESS){ //modo escalamiento
        currentMode = ToolMode::Scaling;
        std::cout << "Modo de animación actual: Escalamiento \n";
    }

    if (key == GLFW_KEY_T && action == GLFW_PRESS){ //modo translacion
        currentMode = ToolMode::Translating;
        std::cout << "Modo de animación actual: Translación \n";
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS){// modo rotacion
        currentMode = ToolMode::Rotating;
        std::cout << "Modo de animación actual: Rotación \n";
    }

    if (key == GLFW_KEY_X && action == GLFW_PRESS){ //eje x
        focusAxis = {1,0,0};
        std::cout << "Eje de enfoque: X \n";
    }

    if (key == GLFW_KEY_Y && action == GLFW_PRESS){ //eje y
        focusAxis = {0,1,0};
        std::cout << "Eje de enfoque: Y \n";
    }

    if (key == GLFW_KEY_Z && action == GLFW_PRESS){ //eje z
        focusAxis = {0,0,1};
        std::cout << "Eje de enfoque: Z \n";
    }


    if (key == GLFW_KEY_M && action == GLFW_PRESS){ //cambiar entre CPU Y GPU

        renderWithGPU = !renderWithGPU;
        std::cout << "Modo de renderizado: " << (renderWithGPU ? "GPU" : "CPU") << "\n";
    }

    if(key == GLFW_KEY_P && action == GLFW_PRESS){
        wireframeMode = !wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
    }

  

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

void pollAnimationEvent(GLFWwindow* window, Cube& instance){
    
    const float delta = 0.01;


    if(currentMode == ToolMode::Translating){
        glm::vec3 translation(0.0f);
        
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            translation.y += delta;
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            translation.y -= delta;
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            translation.x -= delta;
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            translation.x += delta;
        
        if(translation != glm::vec3(0.0f)) {
            instance.transform(translation);
        }
    }

    const float rotationSpeed = 50;

    if(currentMode == ToolMode::Rotating){
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            instance.transform(focusAxis, delta * rotationSpeed);
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            instance.transform(-focusAxis, delta * rotationSpeed);
    }

    if(currentMode == ToolMode::Scaling){

        //escala solo en el eje de enfoque

        glm::vec3 scale(1.0f);
        if(focusAxis.x == 1){
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                scale.x += delta;
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                scale.x -= delta;
        }
        if(focusAxis.y == 1){
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                scale.y += delta;
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                scale.y -= delta;
        }
        if(focusAxis.z == 1){
            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                scale.z += delta;
            if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                scale.z -= delta;
        }

        instance.transform(scale.x, scale.y, scale.z);
      
    }
    
    instance.applyTransform(renderWithGPU);

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
    


    unsigned int shaders;
    setUpShaders(shaders);

    Cube mainCube(0.5);

    mainCube.instantiate();

    glEnable(GL_DEPTH_TEST); //Para considerar profundidad en z en el renderizado

    //Render Loop
    while(!glfwWindowShouldClose(window))
    {
        auto currentTime = std::chrono::steady_clock::now();
        double deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        accumulator += deltaTime;

        // Only update and render if enough time has passed
        while(accumulator >= timePerFrame) {
            glfwPollEvents();
            pollAnimationEvent(window,mainCube);
            //Rendering
            
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(mainCube.VAO);
            glUseProgram(shaders);
            
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

            glfwSwapBuffers(window);

            accumulator -= timePerFrame;

        }


    }

    glfwTerminate();
    ///
    return 0;
}

