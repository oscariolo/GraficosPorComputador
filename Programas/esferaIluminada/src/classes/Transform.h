#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>

class Transform{
    public:

        static void rotate(glm::mat4& model ,glm::vec3 axis, float degrees){
            model = glm::rotate(model,glm::radians(degrees),axis);
        }

        static void isometricTransform(glm::mat4& model){
            model = glm::rotate(model, glm::radians(45.0f),glm::vec3(0,1,0));
            model = glm::rotate(model, glm::radians(35.2564f),glm::vec3(1,0,0));
        }

};


#endif