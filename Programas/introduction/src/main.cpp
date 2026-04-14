#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void setupBasicShape();
std::string loadShaderFromSource(const char* type, const char* name);
void checkShaderCompilation(unsigned int shader);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);



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

    setupBasicShape();


    //Render Loop
    while(!glfwWindowShouldClose(window))
    {
        //Rendering commands 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Rendering 
        
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);


        //
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    ///
    return 0;
}

void setupBasicShape()
{

    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    float position[3] = {
        0.5f, 0.5f, 0.0f
    };

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    //Ligamos VBO 
    unsigned int VBO,VAO,EBO; //vertex buffer object

    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);
    
    //VAO debe ser creado antes para recordar estado de los VBOs
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);


    //ESTE VBO ES EL FOCO DE DONDE SE TOMARAN LOS DATOS DE ATRIBUTOS DE LOS VERTICES
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //EBO es creado y copiamos los indices al buffer 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


    //Toma en cuenta el ultimo VBO asignado al GL_ARRAY_BUFFER
    //LINKING, COMO OPENGL INTERPRETA INPUT DATA PARA LOS SHADERS DESDE LA DATA DE VERTICES
    // position attribute
    glVertexAttribPointer(
        0,                  // attribute index (matches your vertex shader)
        3,                  // number of components (x, y, z)
        GL_FLOAT,           // data type
        GL_FALSE,           // should OpenGL normalize values?
        3 * sizeof(float),  // stride: total size of one vertex
        (void*)0            // offset: where this attribute starts
    );
    glEnableVertexAttribArray(0);
    

    //Cargamos un shader de vertices
    std::string vertexCode = loadShaderFromSource("vertex","forward.shader");
    const char* vertexShaderSource = vertexCode.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
    glCompileShader(vertexShader);

    checkShaderCompilation(vertexShader);

    //Cargamos el shader de fragmentos
    std::string fragmentCode = loadShaderFromSource("fragment","orange.shader");
    const char* fragmentShaderSource = fragmentCode.c_str();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
    glCompileShader(fragmentShader);

    checkShaderCompilation(fragmentShader);

    //Generamos el programa shader para ligar los dos tipos de shader
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);

    //Usamos el shader generado
    glUseProgram(shaderProgram);

    //Ya se genero el programa por lo que ya no necesitamos los shaders previos generados
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 

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

    if (key == GLFW_KEY_I && action == GLFW_PRESS)
    std::cout << "Input Mode\n";
}
