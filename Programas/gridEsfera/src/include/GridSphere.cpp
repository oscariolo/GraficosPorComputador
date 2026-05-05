#include "GridSphere.h"
#include <glad/glad.h>
#include <math.h>
#include <vector>
#include <iostream>

float x_coordinate(float center_x, float radius, float theta, float alpha){
    return center_x + radius*sin(alpha)*cos(theta);
};

float y_coordinate(float center_y, float radius, float theta, float alpha){
    return center_y - radius*cos(alpha);
};

float z_coordinate(float center_z, float radius, float theta, float alpha){
    return center_z + radius*sin(alpha)*sin(theta);
};

void GridSphere::setShape(){

    vertices.clear();
    indices.clear();
    parallelIndices.clear();
    meridianIndices.clear();
    polarIndices.clear();

    const int N = NUM_LINES; // same count for meridians and parallels

    const float thetaStep = 2.0f * PI / N;      // around the sphere
    const float alphaStep = PI / (N);      // pole to pole

    unsigned int indexCount = 0;

    for (int i = 0; i <= N; ++i) {     // move from pole to pole      
        
        float alpha = i * alphaStep;

        for (int j = 0; j < N; ++j) {   // move around the sphere

            float theta = j * thetaStep;
            vertices.push_back(x_coordinate(center[0], radius, theta, alpha));
            vertices.push_back(y_coordinate(center[1], radius, theta, alpha));
            vertices.push_back(z_coordinate(center[2], radius, theta, alpha));

            if((i==0 && polarIndices.empty())|| i == N-1){ //primer punto de la esfera en el polo y ultimo punto deben escapar del bucle
                polarIndices.push_back(indexCount);
                indexCount++;
                break;//no necesita seguir aumentando vertices en polos
            }

            if(i==1 || i==N-2){//outer north and south ring
                polarIndices.push_back(indexCount);
            }

            parallelIndices.push_back(indexCount); 
            meridianIndices.push_back(j* N + i);
            indexCount++;

        }
        
    }
    
    //std::cout << polarIndices.size();
    std::cout << meridianIndices.size();
    std::cout << meridianIndices[0];

    setUpIndices();


};

void GridSphere::setUpIndices(){

    indices.insert(indices.end(),meridianIndices.begin(),meridianIndices.end());
    indices.insert(indices.end(),parallelIndices.begin(),parallelIndices.end());
    //indices.insert(indices.end(),polarIndices.begin(),polarIndices.end());


}


void GridSphere::setUpBuffers(){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),indices.data(),GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
};


