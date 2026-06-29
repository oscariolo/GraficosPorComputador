#include "Lighting.h"
#include <glad/glad.h>

void Lighting::setPosition(glm::vec3 position){
    this->position = position;
}

void Lighting::applyAmbient(float strenght, unsigned int shaderID){

    glUseProgram(shaderID);
    unsigned int transformLoc = glGetUniformLocation(shaderID, "ambientStrenght");
    glUniform1f(transformLoc,strenght);
    glUseProgram(0);

};

void Lighting::applyDiffuse(unsigned int shaderID){
    glUseProgram(shaderID);
    unsigned int lightSource = glGetUniformLocation(shaderID, "lightSource");
    glUniform3f(lightSource,position[0],position[1],position[2]);
    glUseProgram(0);
};

void Lighting::applySpecular(glm::vec3 cameraPos, unsigned int shaderID){
    
    glUseProgram(shaderID);
    unsigned int cameraPosId = glGetUniformLocation(shaderID, "cameraPos");
    glUniform3f(cameraPosId,cameraPos[0],cameraPos[1],cameraPos[2]);
    glUseProgram(0);

}