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


void Lighting::applyPhong(float ambientStrenght, glm::vec3 cameraPos, unsigned int shaderID){
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

void Lighting::applyPhongByFace(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indexData, float ambientStrenght, glm::vec3 cameraPos, unsigned int shaderID){
    
    glUseProgram(shaderID);
    unsigned int lightingByFaceLoc = glGetUniformLocation(shaderID, "lightingByFace");
    glUniform1i(lightingByFaceLoc, true);

    for (unsigned int i = 0; i < indexData.size(); i += 3){
        unsigned int i0 = indexData[i];
        unsigned int i1 = indexData[i+1];
        unsigned int i2 = indexData[i+2];

        glm::vec3 v0 = vertices[i0].position;
        glm::vec3 v1 = vertices[i1].position;
        glm::vec3 v2 = vertices[i2].position;

        // Calculamos U y V
        glm::vec3 U = v1 - v0;
        glm::vec3 V = v2 - v0;

        // Producto cruz: U x V
        glm::vec3 faceNormal = glm::normalize(glm::cross(U, V));

        // VERIFICACIÓN DE ORIENTACIÓN:
        // Si la esfera está centrada en 0,0,0, el vector normal debe apuntar 
        // en la misma dirección que el centroide del triángulo.
        glm::vec3 centroid = (v0 + v1 + v2) / 3.0f;
        if (glm::dot(faceNormal, centroid) < 0.0f) {
            // Si el producto punto es negativo, la normal apunta hacia adentro.
            // Invertimos el orden (o la normal) para corregir el winding.
            faceNormal = -faceNormal;
        }

        // Aplicamos la normal calculada
        vertices[i0].normal = faceNormal;
        vertices[i1].normal = faceNormal;
        vertices[i2].normal = faceNormal;
    }
    
    this->applyPhong(ambientStrenght, cameraPos, shaderID);

}

