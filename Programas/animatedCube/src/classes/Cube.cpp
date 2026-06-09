#include "Cube.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>





Cube::Cube(float baseSize){

    //generamos el cubo partiendo desde las caras, de ahi por cada cara sus aristas con sus vertices
    
    //el tamaño practicamente dictamina la posicion (ej. 0.5) de ahi solo alterna el signo y cual de los ejes se toma como referencia para la cara

    //cara frontal
    Vertex v1 = Vertex(-baseSize/2,baseSize/2,baseSize/2,1,1,1); 
    Vertex v2 = Vertex(baseSize/2,baseSize/2,baseSize/2,1,1,0);
    Vertex v3 = Vertex(-baseSize/2,-baseSize/2,baseSize/2,1,0,1);
    Vertex v4 = Vertex(baseSize/2,-baseSize/2,baseSize/2,0,0,1);

    //cara trasera
    
    Vertex v5 = Vertex(-baseSize/2,baseSize/2,-baseSize/2,1,1,1);
    Vertex v6 = Vertex(baseSize/2,baseSize/2,-baseSize/2,1,1,0);
    Vertex v7 = Vertex(-baseSize/2,-baseSize/2,-baseSize/2,1,0,1);
    Vertex v8 = Vertex(baseSize/2,-baseSize/2,-baseSize/2,0,0,1);

    //cara izquiera
    //v1 v5 v3 v7

    //cara derecha
    //v2 v6 v4 v8

    //cara superior
    //v1 v2 v5 v6

    //cara inferior
    //v3 v4 v7 v8

    this->vertexData = {v1,v2,v3,v4,v5,v6,v7,v8};
    this->originalVertexData = vertexData;
    
    std::vector<std::pair<int, int>> edgeConnections = {
    {0,1}, {2,3}, {0,2}, {1,3},  // front
    {4,5}, {6,7}, {4,6}, {5,7},  // back
    {0,4}, {1,5}, {2,6}, {3,7},  //left
    {1,5}, {3,7}, {1,3}, {5,7}, // right
    {0,1}, {4,5}, {0,4}, {1,5}, //top
    {2,3}, {6,7}, {2,6}, {3,7}, //down

    };

    int edgeCount = 0;
    int faceCount = 0;

    Face f = Face();
    for (auto& pair : edgeConnections) { //cada 4 pares es una cara
        
        Edge e = Edge(vertexData[pair.first], vertexData[pair.second]);

        if(edgeCount == 4){

            this->faces[faceCount] = f;
            f = Face(); //new Face
            edgeCount = 0;
            faceCount += 1;

        }else{
            f.edges[edgeCount] = e;
            edgeCount += 1;
        }

    }

}

void Cube::instantiate(){
    
    auto bufferData = getVertexBufferData();

    int indices[] = {
        //frente
        2,3,1,
        2,0,1,
        //atras
        5,4,6,
        5,7,6,
        //derecha
        3,1,5   ,
        3,7,5,
        //izquierda
        2,0,4,
        2,6,4,
        //superior
        0,4,5,
        0,1,5,
        //inferior
        2,6,7,
        2,3,7
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*bufferData.size(), bufferData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


}



void Cube::transformCPU(glm::vec3 translation){
    this->model = this->model * glm::translate(glm::mat4(1.0f),translation);

}

void Cube::transformCPU(float scaleX, float scaleY, float scaleZ){
    this->model = this->model * glm::scale(glm::mat4(1.0f),glm::vec3(scaleX,scaleY,scaleZ));
}

void Cube::transformCPU(glm::vec3 axis, float angle){
    this->model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis) * this->model;
}


void Cube::applyTransform(){

    for(size_t i = 0; i < originalVertexData.size(); i++){
        // Read from originalVertexData
        glm::vec4 v(originalVertexData[i].position[0], originalVertexData[i].position[1], originalVertexData[i].position[2], 1.0f);
        glm::vec4 transform = this->model * v;

        // Write to vertexData (not originalVertexData!)
        vertexData[i].position[0] = transform.x;
        vertexData[i].position[1] = transform.y;
        vertexData[i].position[2] = transform.z;
    }

    update();
}



void Cube::update(){

    auto bufferData = getVertexBufferData(); 

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Add this line
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bufferData.size(), bufferData.data(), GL_DYNAMIC_DRAW);

}