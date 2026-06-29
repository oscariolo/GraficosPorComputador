#ifndef LIGHTING_H
#define LIGHTING_H
#include <glm/glm.hpp>

class Lighting{

    public:
        
        glm::vec3 sourceColor = glm::vec3(1.0f,1.0f,1.0f);
        glm::vec3 position = glm::vec3(1.0f,0.0f,0.0f);

        void setPosition(glm::vec3 position);

        void applyAmbient(float strenght,unsigned int shaderID);
        void applyDiffuse(unsigned int shaderID);
        void applySpecular(glm::vec3 cameraPos, unsigned int shaderID);

        void applyPhong();


        


};


#endif