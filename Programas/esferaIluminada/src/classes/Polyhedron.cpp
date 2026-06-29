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

void Polyhedron::calculateNormals() {
    //el calculo asume que las caras son triangulos, toma informacion de los indices asignados
    
    for (unsigned int i=0; i<indexData.size(); i += 3){
        
        glm::vec3 p1 = this->vertices[indexData[i]].position;
        glm::vec3 p2 = this->vertices[indexData[i+1]].position;
        glm::vec3 p3 = this->vertices[indexData[i+2]].position;

        glm::vec3 U = p2 - p1;
        glm::vec3 V = p3 - p1;

        glm::vec3 normal = glm::normalize(glm::cross(U,V));

        this->vertices[indexData[i]].normal += normal;
        this->vertices[indexData[i+1]].normal += normal;
        this->vertices[indexData[i+2]].normal += normal;

    };

    
    

}

void Polyhedron::setShaders(unsigned int shaderId){
    this->shaderID = shaderId;
}

void Polyhedron::setUniformColor(glm::vec3 color){
    for(auto& vertex : vertices){
        vertex.color = color;
    }
}