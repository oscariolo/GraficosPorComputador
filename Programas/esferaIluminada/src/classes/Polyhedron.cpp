#include "Polyhedron.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//Encola al pipeline para graficar el poliedro
void Polyhedron::instantiate() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexData.size(), indexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);


}

void Polyhedron::draw() {

    glUseProgram(this->shaderID);

    unsigned int transformLoc = glGetUniformLocation(this->shaderID, "model");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(this->model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, 0);

}

void Polyhedron::setShaders(unsigned int shaderId){
    this->shaderID = shaderId;
}

void Polyhedron::calculateNormals(){
    
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
    
}

void Polyhedron::setUniformColor(glm::vec3 color){
    for(auto& vertex : vertices){
        vertex.color = color;
    }
}