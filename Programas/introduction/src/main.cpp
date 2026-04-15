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

void setupBasicShape(unsigned int* VBOs,unsigned int* VAOs)
{
    
    float startPosition[2];
    float size[2];
    askRectangleAttributes(startPosition,size);

    float top_right[2] = {startPosition[0]+size[0],startPosition[1]+size[1]};
    float bottom_right[2] = {startPosition[0]+size[0],startPosition[1]};
    float bottom_left[2] = {startPosition[0],startPosition[1]};
    float top_left[2] = {startPosition[0],startPosition[1]+size[1]};


    float rectangle[] = {
        top_right[0],  top_right[1], 0.0f,  // top right
        bottom_right[0],bottom_right[1], 0.0f,  // bottom right
        bottom_left[0], bottom_left[1], 0.0f,  // bottom left
        top_left[0],  top_right[1], 0.0f   // top left 
    };

    const float thickness = 0.2f;

    float border[12];
    const float signX[4] = { 1.0f,  1.0f, -1.0f, -1.0f}; // TR, BR, BL, TL
    const float signY[4] = { 1.0f, -1.0f, -1.0f,  1.0f}; // TR, BR, BL, TL

    for (int i = 0; i < 12; ++i)
    {
        int vertex = i / 3;   // 0..3
        int coord  = i % 3;   // 0=x, 1=y, 2=z

        if (coord == 0) {
            border[i] = rectangle[i] + signX[vertex] * thickness;
        } else if (coord == 1) {
            border[i] = rectangle[i] + signY[vertex] * thickness;
        } else {
            border[i] = rectangle[i]; // keep z unchanged
        }
    }

    unsigned int indices[] = {
        0,1,3,
        1,2,3
    };

    unsigned int EBO;

    glGenBuffers(2, VBOs);
    glGenBuffers(1,&EBO);
    
    //VAO debe ser creado antes para recordar estado de los VBOs
    glGenVertexArrays(2,VAOs);


    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // attribute index (matches your vertex shader)
        3,                  // number of components (x, y, z)
        GL_FLOAT,           // data type
        GL_FALSE,           // should OpenGL normalize values?
        3 * sizeof(float),  // stride: total size of one vertex
        (void*)0            // offset: where this attribute starts
    );
    glEnableVertexAttribArray(0);

    //VBO y VAO para el borde
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(border), border, GL_STATIC_DRAW);

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

}


void setupShaders(unsigned int* shaders){
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

    //Shader del borde
    std::string fragmentCode2 = loadShaderFromSource("fragment","yellow.shader");
    const char* fragmentShaderSource2 = fragmentCode2.c_str();

    unsigned int fragmentShader2;
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2,1,&fragmentShaderSource2,NULL);
    glCompileShader(fragmentShader2);

    checkShaderCompilation(fragmentShader2);

    //Generamos el programa shader para ligar los dos tipos de shader
    shaders[0] = glCreateProgram();
    glAttachShader(shaders[0],vertexShader);
    glAttachShader(shaders[0],fragmentShader1);
    glLinkProgram(shaders[0]);

    shaders[1] = glCreateProgram();
    glAttachShader(shaders[1],vertexShader);
    glAttachShader(shaders[1],fragmentShader2);
    glLinkProgram(shaders[1]);

    //Ya se genero el programa por lo que ya no necesitamos los shaders previos generados
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1); 
    glDeleteShader(fragmentShader2);
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
