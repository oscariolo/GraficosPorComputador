#include "Lighting.h"
#include <glad/glad.h>
#include <iostream>

void Lighting::setPosition(glm::vec3 position){
    this->position = position;
}

void Lighting::setSourceColor(glm::vec3 color, unsigned int shaderID){
    this->sourceColor = color;
    glUseProgram(shaderID);
    unsigned int lightColorLoc = glGetUniformLocation(shaderID, "lightColor");
    glUniform3f(lightColorLoc, color[0], color[1], color[2]);
    glUseProgram(0);
}


void Lighting::applyPhong(float ambientStrenght, glm::vec3 cameraPos, unsigned int shaderID, bool lightingByFace){
    
    glUseProgram(shaderID);
    unsigned int lightingByFaceLoc = glGetUniformLocation(shaderID, "lightingByFace");
    glUniform1i(lightingByFaceLoc, lightingByFace);

    this->applyAmbient(ambientStrenght,shaderID);
    this->applyDiffuse(shaderID);
    this->applySpecular(cameraPos,shaderID);
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


