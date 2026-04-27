#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

    unsigned int VBOs[2],VAOs[2],shaders[2];
    setupBasicShape(VBOs,VAOs);
    setupShaders(shaders);

    //Render Loop
    while(!glfwWindowShouldClose(window))
    {
        //Rendering commands 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Rendering
        glUseProgram(shaders[1]); 
        glBindVertexArray(VAOs[1]);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glUseProgram(shaders[0]);
        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        //
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    ///
    return 0;
}
