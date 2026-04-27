#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::string loadShaderFromSource(const char* type, const char* name);
void checkShaderCompilation(unsigned int shader);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


void askRectangleAttributes(float* startPosition, float* size){

    std::cout << "X position ";
    std::cin >> startPosition[0];
    std::cout << "Y position ";
    std::cin >> startPosition[1];
    std::cout << "length ";
    std::cin >> size[0];
    std::cout << "height ";
    std::cin >> size[1];
}

void setupBasicShape(unsigned int &VBOs,unsigned int &VAOs)
{
    
    float startPosition[2];
    float size[2];
    askRectangleAttributes(startPosition,size);

    float top_right[2] = {startPosition[0]+size[0],startPosition[1]+size[1]};
    float bottom_right[2] = {startPosition[0]+size[0],startPosition[1]};
    float bottom_left[2] = {startPosition[0],startPosition[1]};
    float top_left[2] = {startPosition[0],startPosition[1]+size[1]};

    //Border colors red green blue yellow and red

    float red[3] = {1.0f,0.0f,0.0f};
    float green[3] = {0.0f,1.0f,0.0f};
    float blue[3] = {0.0f,0.0f,1.0f};
    float yellow[3] = {1.0f,1.0f,0.0f};

    float rectangle[] = {
        top_right[0],  top_right[1], 0.0f, red[0], red[1], red[2],  // top right
        bottom_right[0],bottom_right[1], 0.0f, green[0], green[1], green[2],  // bottom right
        bottom_left[0], bottom_left[1], 0.0f, blue[0], blue[1], blue[2],  // bottom left
        top_left[0],  top_left[1], 0.0f, yellow[0], yellow[1], yellow[2]   // top left 
    };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    unsigned int EBO;

    glGenBuffers(1, &VBOs);
    glGenBuffers(1,&EBO);
    
    //VAO debe ser creado antes para recordar estado de los VBOs
    glGenVertexArrays(1, &VAOs);


    glBindVertexArray(VAOs);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // attribute index (matches your vertex shader)
        3,                  // number of components (x, y, z)
        GL_FLOAT,           // data type
        GL_FALSE,           // should OpenGL normalize values?
        6 * sizeof(float),  // stride: total size of one vertex
        (void*)0            // offset: where this attribute starts
    );
    glEnableVertexAttribArray(0);

        glVertexAttribPointer(
        1,                  // attribute index (matches your vertex shader)
        3,                  // number of components (x, y, z)
        GL_FLOAT,           // data type
        GL_FALSE,           // should OpenGL normalize values?
        6 * sizeof(float),  // stride: total size of one vertex
        (void*)(3 * sizeof(float))  // offset: where this attribute starts
    );
    glEnableVertexAttribArray(1);


}


void setupShaders(unsigned int &shaders){
    //Cargamos un shader de vertices
    std::string vertexCode = loadShaderFromSource("vertex","forward.shader");
    const char* vertexShaderSource = vertexCode.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    checkShaderCompilation(vertexShader);

    //Cargamos el shader de fragmentos
    std::string fragmentCode1 = loadShaderFromSource("fragment","orange.shader");
    const char* fragmentShaderSource = fragmentCode1.c_str();

    unsigned int fragmentShader1;
    fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader1);

    checkShaderCompilation(fragmentShader1);

    //Generamos el programa shader para ligar los dos tipos de shader
    shaders = glCreateProgram();
    glAttachShader(shaders,vertexShader);
    glAttachShader(shaders,fragmentShader1);
    glLinkProgram(shaders);

    //Ya se genero el programa por lo que ya no necesitamos los shaders previos generados
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1); 
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

std::string loadShaderFromSource(const char* type, const char* name)
{

    fs::path shaderRoot = fs::path(__FILE__).parent_path() / "shaders";
    fs::path shaderPath = shaderRoot / type / name;

    std::ifstream file(shaderPath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open shader file: " << shaderPath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void checkShaderCompilation(unsigned int shader){
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);    
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

    unsigned int VBO,VAO,shaders;
    setupBasicShape(VBO,VAO);
    setupShaders(shaders);

    //Render Loop
    while(!glfwWindowShouldClose(window))
    {
        //Rendering commands 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaders);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        //
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    ///
    return 0;
}
