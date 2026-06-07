#pragma once

#include <glad/glad.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace shader_utils {

inline std::string loadShaderFromSource(const std::filesystem::path& sourceFileDir, const char* type, const char* name)
{
    std::filesystem::path shaderPath = sourceFileDir / "shaders" / type / name;

    std::ifstream file(shaderPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << shaderPath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

inline void checkShaderCompilation(unsigned int shader)
{
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

} // namespace shader_utils
